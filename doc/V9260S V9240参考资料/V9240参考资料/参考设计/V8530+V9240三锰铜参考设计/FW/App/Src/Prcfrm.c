#define CM_PRCFRM_EXT
#include "Include.h"

uint8 DoNothing(S_FRAMEINFO *s_FrmInfo)
{
    return s_FrmInfo->ucAtb;
}
/*==============================================================================
命令与对应的函数处理
命令字， 属性， 处理函数(在另外一个文件中定义的)
==============================================================================*/
const S_CMDPROC gs_CmdProc[] =
{
//    {0x03,ComCtertEsam2007},            //安全认证
    {0x11,RWData},                      //读数据
    {0x12,RWData},                      //读后续数据
    {0x14,RWData},                      //写数据
    {0x13,ReadMeterAddr},               //读通讯地址
    {0x15,WriteMeterAddr},              //写通讯地址
    {0x08,BroadcastTime},               //广播校时
    {0x16,Freeze},                      //冻结命令
    {0x17,ChangeBode},                  //更改通讯速率
    {0x18,ChangePassWord},              //更改密码
    {0x1A,ClearMeter},                  //电表清零
    {0x1B,ClearEvent},                  //事件清零
//    {0x1A,SwitchControl},               //电表清零
//    {0x1B,SwitchControl},               //事件清零
//    {0x1C,SwitchControl},               //继电器控制  
    {0x1D,SecPulsCtrl},                 //多功能端子输出信号控制
    {0x01,FactoryRD},                   //厂内读
    {0x02,FactoryWR},                   //厂内写
};

/*=========================================================================================\n
* @function_name: BD_ProcFrame
* @function_file: Prcfrm.c
* @描述:
*
*
* @参数:
* @param:ComProcCtr
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 BD_ProcFrame(S_COM *ComProcCtr)
{
    uint32 ulDey;
    uint8 i;
    S_FRAMEINFO s_FrmInfo;                              //开辟临时缓存来处理信息
    s_FrmInfo.ucPort=ComProcCtr->ucPort;
    ulDey=0xffff;
    if(Judge645Frm(ComProcCtr, &s_FrmInfo) == Ret_Err)  //判断645帧是否是合法帧
    {
        return Ret_Err;                                 //非法帧
    }

    if(Decode645Frame(&s_FrmInfo) == Ret_Err)           //解析645帧
    {
        return Ret_Err;
    }

    gul_ShowState |= flgShowSt_Com;                     //通讯显示电话机符号
    guc_ShowStCom = 3;                                  //显示3s

    if(s_FrmInfo.ucErr.word == 0)
    {
        for(i = 0; i < sizeof(gs_CmdProc) / sizeof(S_CMDPROC); i++)
        {
            if(gs_CmdProc[i].ucCmd == s_FrmInfo.ucCmd)                  //查找规约命令字
            {                                                           //执行相应函数
                if(Ret_Err == gs_CmdProc[i].ProcFun(&s_FrmInfo))
                {
                    return Ret_Err;
                }
                break;
            }
        }
        if(i >= sizeof(gs_CmdProc) / sizeof(S_CMDPROC))
        {
            return Ret_Err;                                     //找不到命令字就直接返回错误
        }
    }

    if(s_FrmInfo.ucReply != 0)                                  //如果需要回复
    {
        if(Encode645Frame(ComProcCtr, &s_FrmInfo) == Ret_OK)    //帧的打包
        {
            while(guc_ComSendDy)
            {
                if(ulDey!=0)
                {
                    ulDey--;
                }else
                {
                    break;
                }
            }

            ComBom_StartSend(ComProcCtr->ucPort);
            return Ret_OK;
         }
        return Ret_Err;
    }else
    {
        return Ret_Err;
    }
    //return Ret_OK;
}
/*=========================================================================================\n
* @function_name: Judge645Frm
* @function_file: Prcfrm.c
* @描述: 判断645帧是否是合法的帧
*
*
* @参数:
* @param:psCom  通讯串口
* @param:sFrm
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 Judge645Frm(S_COM* psCom, S_FRAMEINFO* sFrm)
{
	uint8 i;
    for(i = 0; i < LeadCharNum; i++)
    {
        if(psCom->ucLen < (MinFrmLen + i))          //收到的数据至少需要满足最小帧长度
        {
            return Ret_Err;
        }
                                                    //判断是否是有效帧
        else if(Ret_OK == IsValidFrame(psCom->ucBuf+i,psCom->ucLen - i))
        {                                           //拷贝数据到临时缓存

            //判断是否数据长度正确
            //memcpy(sFrm->ucbuf, psCom->ucBuf + i, psCom->ucLen - i);
            sFrm->ucbuf=(psCom->ucBuf + i);
            return Ret_OK;                          //返回有效帧
        }
    }
    return Ret_Err;                                     //非有效帧
}
/*=========================================================================================\n
* @function_name: IsValidFrame
* @function_file: Prcfrm.c
* @描述: 合法帧判断
*
*
* @参数:
* @param:pucBuf
* @param:ucFrmLen
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 IsValidFrame(uint8* pucBuf, uint8 ucFrmLen)
{
    uint8 ucSum;
	uint8 ucLen;

    if((pucBuf[0]!= 0x68) || (pucBuf[7] != 0x68))   //帧头0x68判断
    {
        return Ret_Err;
    }

     ucLen = pucBuf[PosDataLen];
    if(ucFrmLen < (MinFrmLen + ucLen))              //收到的完整帧与实际长度比较
    {
        return Ret_Err;
    }

    ucSum = DoSum(pucBuf, ucLen + MinFrmLen - 2);   //计算校验和
    if(pucBuf[ucLen + MinFrmLen - 2] != ucSum)
    {                                               //校验和不对
        return Ret_Err;
    }

    if(pucBuf[ucLen + MinFrmLen - 1] != 0x16)       //判断帧尾是否为0x16
    {
        return Ret_Err;
    }

    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: DoSum
* @function_file: Prcfrm.c
* @描述: 计算校验和
*
*
* @参数:
* @param:pucBuf
* @param:ucLen
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 DoSum(uint8* pucBuf, uint8 ucLen)
{
    uint8 ucSum = 0;
	uint8 i;
    for(i= 0; i < ucLen; i ++)
    {
        ucSum += pucBuf[i];
    }
    return ucSum;
}
/*=========================================================================================\n
* @function_name: Decode645Frame
* @function_file: Prcfrm.c
* @描述: 解析645帧
*
*
* @参数:
* @param:sFrm
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 Decode645Frame(S_FRAMEINFO* sFrm)
{
    sFrm->ucReply = 1;                              //默认是需要回复的
    sFrm->ucErr.word = 0;                                //默认没有错误
    sFrm->uiID.lword = 0xFFFFFFFF;                  //假设默认的ID为全F

    if(sFrm->ucbuf[9] > Max645FrmLen)               //防止下面减0x33过程中将非缓存中数据改写
    {
        sFrm->ucReply = 0;                          //不需要回复
        return Ret_Err;
    }

    if(Judge645Addr(sFrm) == Ret_Err)
    {
        sFrm->ucAtb = Const_Addr_Err;               //表地址错误
        sFrm->ucReply = 0;                          //不需要回复
        return Ret_Err;
    }

    UnDo33(sFrm->ucbuf + 10, sFrm->ucbuf[9]);       //数据区减0x33

    sFrm->ucCmd = sFrm->ucbuf[8];                   //控制字
    sFrm->ucLen = sFrm->ucbuf[9];                   //数据区长度长度

    if((sFrm->ucCmd == 0x11)||(sFrm->ucCmd == 0x14)
#ifdef SQSDPRLOG
       ||(sFrm->ucCmd == 0x12)
#endif
      )
    {                                               //如果为读写的话
        memcpy(sFrm->uiID.byte,sFrm->ucbuf+10,4);   //数据标识

        if(sFrm->ucCmd == 0x14)                     //如果是写数据的话
        {
            if(sFrm->ucAtb!=Const_Addr_Normal)      //如果设置不是普通地址，则返回错误
            {
                sFrm->ucReply = 0;                                  //不需要回复的
                return Ret_Err;
            }

//            if((sFrm->ucPort==ComIndex_Uart2)
//               &&(gs_EsamIrCmd.ucCmdStatus!=IRESAMOK))
//            {
//                sFrm->ucErr.word = ComErrBit_ErrPw;
//                sFrm->ucLen = 1;
//                return Ret_OK;
//            }

            if(sFrm->ucLen <= 12)                                    //如果长度没有12的话，就是错误的帧
            {
                return Ret_Err;
            }
            
            memcpy(sFrm->uiUserCode.byte,sFrm->ucbuf+18,4);         //操作者代码
            if(guc_FactoryType != FactorMd)                         //厂内状态没开的话需要判断密码
            {
                sFrm->ucLevel = JudgePassword(sFrm->ucbuf + 14);
                if(sFrm->ucLevel == 0xFF)                           //判断权限
                {
                    sFrm->ucAtb = Const_Atb_Pswd_Err;
                    sFrm->ucErr.word = ComErrBit_ErrPw;             //密码错误
                    sFrm->ucLen = 1;
                    return Ret_OK;
                }
            }
            else
            {
                sFrm->ucLevel=sFrm->ucbuf[14];                      //厂内状态,数据指针指向未处理数据
            }
            sFrm->ucbuf+=22;                                    //数据指针指向未处理数据
            return Ret_OK;
        }
        else                                            //否则为读数据
        {
//            if((sFrm->ucPort==ComIndex_Uart2)                //红外通道判断，没有通过认证
//               &&(gs_EsamIrCmd.ucCmdStatus!=IRESAMOK))
//            {
//
//                if((sFrm->uiID.lword!=0x04000401)             //表号
//                   &&(sFrm->uiID.lword!=0x04000101)           //日期
//                   &&(sFrm->uiID.lword!=0x04000102)           //时间
//                   &&(sFrm->uiID.lword!=0x04000402)           //通讯地址
//                   &&(sFrm->uiID.lword!=0x04800004)           //备案号
//                   &&((sFrm->uiID.lword&0xFF0000FF)!=0x00000000)
//                   )
//                {
//                    sFrm->ucErr.word = ComErrBit_ErrPw;
//                    sFrm->ucLen = 1;
//                    return Ret_OK;
//                }
//            }

            if(sFrm->ucLen < 4)                     //如果长度没有4的话，就是错误的帧
            {
                return Ret_Err;
            }
            sFrm->ucbuf+=14;                        //数据指针指向未处理数据
        }
    }
    else                                            //既不是读也不是写
    {                                               //拷贝数据
      
//        if((sFrm->ucPort==ComIndex_Uart2)                //红外通道判断，没有通过认证
//           &&(gs_EsamIrCmd.ucCmdStatus!=IRESAMOK))
//        {
//            memcpy(sFrm->uiID.byte,sFrm->ucbuf+10,4);   //数据标识
//            if((sFrm->uiID.lword!=0x078003FF)           //红外认证查询命令
//               &&(sFrm->uiID.lword!=0x070003FF)           //红外认证查询命令
//               )
//            {
//                sFrm->ucErr.word = ComErrBit_ErrPw;
//                sFrm->ucLen = 1;
//                return Ret_OK;
//            }
//        }

        sFrm->ucbuf+=10;                            //CMD命令字不是0X11和0X14,后续处理
    }

    return Ret_OK;                                  //返回解析正确
}
/*=========================================================================================\n
* @function_name: PwBs_DyOut
* @function_file: Prcfrm.c
* @描述: 密码闭锁判断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void PwBs_DyOut(void)
{
    //首先判断是否闭锁
    if(gs_PassWordCtr.ucSt02==BSST)
    {
        if(gs_PassWordCtr.uiDyPW2Out>0)
        {
            gs_PassWordCtr.uiDyPW2Out--;
            if(gs_PassWordCtr.uiDyPW2Out==0)
            {
                gs_PassWordCtr.ucSt02=0;
                gs_PassWordCtr.ucPW2ErrCnt=0;
                //闭锁时间
                BE_WriteP(EEP_PW2BSTM,(uint8*)&gs_PassWordCtr.uiDyPW2Out,0x02);    //写入闭锁剩余时间
            }
        }
 
    }

    if(gs_PassWordCtr.ucSt04==BSST)
    {
        if(gs_PassWordCtr.uiDyPW4Out>0)
        {
            gs_PassWordCtr.uiDyPW4Out--;
            if(gs_PassWordCtr.uiDyPW4Out==0)
            {
                gs_PassWordCtr.ucSt04=0;
                gs_PassWordCtr.ucPW4ErrCnt=0;
                //闭锁时间
                BE_WriteP(EEP_PW4BSTM,(uint8*)&gs_PassWordCtr.uiDyPW4Out,0x02);    //写入闭锁剩余时间
            }
        }
 
    }
}
/*=========================================================================================\n
* @function_name: JudgePassword
* @function_file: Prcfrm.c
* @描述: 判断密码是否正确
*
*
* @参数:
* @param:buff
*
* @返回:
* @return: uint8 密码等级，返回0xFF代表密码错误
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 JudgePassword(uint8* buff)
{
//  uint8 PassWord[3];
//  uint8 ucBsCnt;
    if((buff[0] == 0x98) || (buff[0] == 0x99))      //如果是98、99级，那么不需要判断密码直接返回权限
    {
        return buff[0];
    }
    /*
    if((buff[0] != 0x02) && (buff[0] != 0x04))      //如果密码等级不等于02或者04则错误
    {
        return 0xFF;
    }

                                                    //读取相应等级的密码
    BE_ReadP(buff[0] * 3 + EEP_645PASSWD1, PassWord,3);
 
    if(ApiBufCmp(PassWord,buff+1,3)==CMP_EQU)
    {
        //首先判断有没有闭锁,
        if(buff[0]==0x02)               //02级密码闭锁
        {

            if(gs_PassWordCtr.ucSt02==BSST) //02级密码闭锁判断，闭锁直接返回错误
            {
                return 0xFF;
            }else           //不然返回密级,清零密码错误次数
            {
                gs_PassWordCtr.ucPW2ErrCnt=0;
                gs_PassWordCtr.uiDyPW2Out=0;
                return 0x02;
            }

        }else if(buff[0]==0x04)         //04级密码闭锁
        {
            if(gs_PassWordCtr.ucSt04==BSST) //04级密码闭锁判断，闭锁直接返回错误
            {
                return 0xFF;
            }else           //不然返回密级,清零密码错误次数
            {
                gs_PassWordCtr.ucPW4ErrCnt=0;
                gs_PassWordCtr.uiDyPW4Out=0;
                return 0x04;
            }
        }
    }
    else                                            //如果密码错误
    {
        if(!(gui_SystemState & flgStSys_Program))   //编程键没有按就直接返回
        {
            return 0xFF;
        }
        //首先读取闭锁次数

        BE_ReadP(EEP_BSCOUNT,&ucBsCnt,1);               //闭锁次数
        if(buff[0] == 0x02)                         //02级密码错误
        {
            if(gs_PassWordCtr.ucSt02!=BSST)
            {
                gs_PassWordCtr.ucPW2ErrCnt++;                   //错误次数增加
                if(gs_PassWordCtr.ucPW2ErrCnt>=ucBsCnt)         //判断到了闭锁次数
                {
                    gs_PassWordCtr.ucSt02=BSST;                 //闭锁
                    BE_ReadP(EEP_BSTIME,(uint8*)&gs_PassWordCtr.uiDyPW2Out,0x02);   //写入闭锁时间
                    //写入闭锁时间    
                    BE_WriteP(EEP_PW2BSTM,(uint8*)&gs_PassWordCtr.uiDyPW2Out,0x02);
                }
            }
        }
        else if(buff[0] == 0x04)
        {
            if(gs_PassWordCtr.ucSt04!=BSST)
            {
                gs_PassWordCtr.ucPW4ErrCnt++;                   //错误次数增加
                if(gs_PassWordCtr.ucPW4ErrCnt>=ucBsCnt)         //判断到了闭锁次数
                {
                    gs_PassWordCtr.ucSt04=BSST;                 //闭锁
                    BE_ReadP(EEP_BSTIME,(uint8*)&gs_PassWordCtr.uiDyPW4Out,0x02);   //写入闭锁时间
                    //写入闭锁时间    
                    BE_WriteP(EEP_PW4BSTM,(uint8*)&gs_PassWordCtr.uiDyPW4Out,0x02);
                }
            }
        }
        return 0xFF;                                //返回密码不正确
    }*/
    return 0xFF;
}
/*=========================================================================================\n
* @function_name: Judge645Addr
* @function_file: Prcfrm.c
* @描述: 判断645地址是否正确
*
*
* @参数:
* @param:sFrm
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 Judge645Addr(S_FRAMEINFO* sFrm)
{
    uint8 i;
    uint8 flag;
    flag=0;
    BE_ReadP(EEP_COMADD,guc_MeteAddr, 6);           //读取表地址,就算这里不判断,回复的时候也需要
    if(ApiBufCmp(sFrm->ucbuf+1, guc_MeteAddr, 6) != CMP_EQU)
    {
        //不是普通地址，判断是否是全99
        for(i = 1; i <= 6; i++)
        {
            if(sFrm->ucbuf[i] != 0x99)
            {
                break;
            }
        }

        if(i>=6)
        {
            sFrm->ucAtb = Const_Addr_Broadcast;         //广播地址
            sFrm->ucReply = 0;                          //不需要回复
            return Ret_OK;
        }

        //判断是否缩位地址
        for(i = 12; i >0 ; i --)
        {//判断是否为万能表地址或者缩位表号
            if(flag==1)     //已经
            {
                if((i - 1)& 0x01)
                {
                    if((sFrm->ucbuf[(i+1)>>1] & 0xF0) != (guc_MeteAddr[(i - 1)>>1] & 0xF0))
                    {
                        break;
                    }
                }else
                {
                    if((sFrm->ucbuf[(i+1)>>1 ] & 0x0F) != (guc_MeteAddr[(i - 1)>>1] & 0x0F))
                    {
                        break;
                    }
                }
            }else
            {
                if((i - 1)& 0x01)
                {
                    if((sFrm->ucbuf[(i+1)>>1] & 0xF0) != 0xA0)
                    {
                        if((sFrm->ucbuf[(i+1)>>1] & 0xF0) == (guc_MeteAddr[(i - 1)>>1] & 0xF0))
                        {
                            flag=1;
                        }else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if((sFrm->ucbuf[(i+1)>>1 ] & 0x0F) != 0x0A)
                    {
                       if((sFrm->ucbuf[(i+1)>>1 ] & 0x0F) == (guc_MeteAddr[(i - 1)>>1] & 0x0F))
                       {
                           flag=1;
                       }else
                       {
                           break;
                       }
                    }
                }
            }
        }

        if(i > 0)
        {//没有判断完退出则认为是错误的表号
            sFrm->ucAtb = Const_Addr_Err;           //运行到这里，说明表地址错误
            return Ret_Err;
        }else
        {//否则认为是缩位表号或者万能表号
            sFrm->ucAtb = Const_Addr_Any;               //万能地址
        }
    }else
    {
        sFrm->ucAtb = Const_Addr_Normal;                //正常地址
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: Encode645Frame
* @function_file: Prcfrm.c
* @描述: 645帧打包
*
*
* @参数:
* @param:psCom
* @param:sFrm
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 Encode645Frame(S_COM* psCom, S_FRAMEINFO* sFrm)
{
    psCom->ucBuf[0] = 0x68;                                         //填写帧头
    psCom->ucBuf[7] = 0x68;                                         //填写帧头

    memcpy(psCom->ucBuf + 1,guc_MeteAddr,6);                        //填写地址到帧里面去

    if(sFrm->ucErr.word == 0)                                            //正常应答
    {
        psCom->ucBuf[9] = sFrm->ucLen;                              //填写数据域长度
        if(sFrm->ucCmd == 0x11 
#ifdef SQSDPRLOG
           ||sFrm->ucCmd == 0x31
           ||sFrm->ucCmd == 0x32
           ||sFrm->ucCmd == 0x12
#endif
           )                                     //如果是读数据的话
        {
            psCom->ucBuf[9] += 4;                                   //加上数据标识的长度
            memcpy(psCom->ucBuf+10,sFrm->uiID.byte,4);              //数据标识
            memcpy(psCom->ucBuf + 14, (uint8*)sFrm->ucbuf, sFrm->ucLen);    //拷贝数据区
        }
        else
        {                                                           //拷贝数据区
            memcpy(psCom->ucBuf + 10, (uint8*)sFrm->ucbuf, sFrm->ucLen);
        }

        sFrm->ucCmd |= 0x80;                                        //控制字加0x80
        psCom->ucBuf[8] = sFrm->ucCmd;                              //填写控制码

        Do33(psCom->ucBuf + 10, psCom->ucBuf[9]);                   //数据区加0x33操作
        psCom->ucLen = MinFrmLen + psCom->ucBuf[9];                 //数据帧的长度
                                                                    //计算校验和
        psCom->ucBuf[psCom->ucLen - 2] = DoSum(psCom->ucBuf, psCom->ucLen - 2);
        psCom->ucBuf[psCom->ucLen - 1] = 0x16;                      //填写帧尾
    }
    else
    {

        sFrm->ucCmd += 0xC0;                        //控制字加0xC0
        if(sFrm->ucCmd==0xC3)                       //Esam 错误
        {
            psCom->ucBuf[8] = sFrm->ucCmd;              //填写控制码
            psCom->ucBuf[9] = 0x02;                     //填写数据域长度

            psCom->ucBuf[10] = sFrm->ucErr.byte[0] + 0x33;      //填写错误码 + 0x33
            psCom->ucBuf[11] = sFrm->ucErr.byte[1] + 0x33;      //填写错误码 + 0x33

            psCom->ucBuf[12] = DoSum(psCom->ucBuf, 12); //计算校验和
            psCom->ucBuf[13] = 0x16;                    //填写帧尾
            psCom->ucLen = 14;                          //总共要发送的字节数                        
        }else
        {
            psCom->ucBuf[8] = sFrm->ucCmd;              //填写控制码
            psCom->ucBuf[9] = 0x01;                     //填写数据域长度
            psCom->ucBuf[10] = sFrm->ucErr.byte[0] + 0x33;      //填写错误码 + 0x33
            psCom->ucBuf[11] = DoSum(psCom->ucBuf, 11); //计算校验和
            psCom->ucBuf[12] = 0x16;                    //填写帧尾
            psCom->ucLen = 13;                          //总共要发送的字节数
        }
    }
    
//    if((((psCom->ucBuf[8]&0x3f) == 0x11) || ((psCom->ucBuf[8]&0x3f) == 0x12))
//       && (gs_MeterMode.ReportMode == 1)) //读数据才启用主动上报+主动上报模式开
//    {
//        if(((gs_ReportState.ui_ReportState1.Word.word&0x0528) != 0) 
//           || (gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt != 0)
//           || (gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt != 0))  //静止式多费率判断
//        {
//            psCom->ucBuf[8] |= 0x20;
//            psCom->ucBuf[psCom->ucLen - 2] += 0x20;
//        }
//    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: UnDo33
* @function_file: Prcfrm.c
* @描述: 数据区减0x33的操作
*
*
* @参数:
* @param:pucBuf
* @param:ucLen
* @返回:
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void UnDo33(uint8* pucBuf, uint8 ucLen)
{
	uint8 i;
    for(i = 0; i < ucLen; i++)
    {
        pucBuf[i] -= 0x33;
    }
}
/*=========================================================================================\n
* @function_name: Do33
* @function_file: Prcfrm.c
* @描述: 数据区加0x33的操作
*
*
* @参数:
* @param:pucBuf
* @param:ucLen
* @返回:
* @作者:   lwb (2012-02-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Do33(uint8* pucBuf, uint8 ucLen)
{
	uint8 i;
    for(i = 0; i < ucLen; i++)
    {
        pucBuf[i] += 0x33;
    }
}


