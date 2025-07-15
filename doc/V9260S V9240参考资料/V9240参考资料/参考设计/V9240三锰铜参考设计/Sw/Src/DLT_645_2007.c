
#define DLT_2007_645_EXT
#include "Include.h"

/*=========================================================================================\n
* @function_name: ReadMeterAddr
* @function_file: DLT_645_2007.c
* @描述: 读通讯地址
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-02-28)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadMeterAddr(S_FRAMEINFO *s_FrmInfo)
{
    if(s_FrmInfo->ucAtb == Const_Addr_Any)
    {
        SysE2ReadData(EEP_COMADD, s_FrmInfo->ucbuf, 6);     //读取表地址
        s_FrmInfo->ucLen = 6;                               //回复的数据域长度
    }
    else
    {
        s_FrmInfo->ucReply = 0;                             //除了AA...AA之外其他不能读取表地址,不需要回复
    }
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: WriteMeterAddr
* @function_file: DLT_645_2007.c
* @描述: 写表地址
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-02-28)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 WriteMeterAddr(S_FRAMEINFO *s_FrmInfo)
{
    if(s_FrmInfo->ucAtb != Const_Addr_Any)              //通讯地址为AA....AA
    {

        s_FrmInfo->ucReply = 0;                         //除了AA...AA之外其他不能读取表地址,不需要回复
        return Ret_OK;
    }

    if((gui_SystemState & flgStSys_Program)
       || (guc_FactoryType == FactorMd))                      // 不在厂内需要编程键配合
    {
        if(JudgeIsNum(s_FrmInfo->ucbuf,6)==Const_Data_OK)
        {
            SysE2ParaSetManage(EEP_COMADD, s_FrmInfo->ucbuf,6); //写通讯地址
            memcpy(guc_MeteAddr,s_FrmInfo->ucbuf,6);            //赋值地址
            s_FrmInfo->ucLen = 0;                               //回复的数据域长度
        }
        else
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Other;            //其他错误
            s_FrmInfo->ucLen = 1;
        }
    }
    else
    {
        s_FrmInfo->ucErr.word |= ComErrBit_ErrPw;            //权限不足
        s_FrmInfo->ucLen = 1;
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RWData
* @function_file: DLT_645_2007.c
* @描述: 读写数据
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RWData(S_FRAMEINFO *s_FrmInfo)
{
//    uint8 uclen;
    uint8 Index,i;
//    if(s_FrmInfo->ucLevel==0x99)
//    {
//        if( s_FrmInfo->uiID.lword==USERIDNUM        //客户编号
//           ||s_FrmInfo->uiID.lword==METERIDNUM)     //表号            
//        {
//            ApiReverseData(s_FrmInfo->ucbuf,6);     //反转数据内容
//            ApiReverseData(s_FrmInfo->ucbuf+6,4);   //反转MAC
//
//            if(s_FrmInfo->uiID.lword==METERIDNUM)   //表号跟新必须都在公钥下
//            {
//                s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,MeterIdFm,6);
//                if(s_FrmInfo->ucErr.word!=0)
//                {    
//                    s_FrmInfo->ucLen = 1;
//                    return Ret_OK;
//                }
//
//                RemoteControl(s_FrmInfo->ucbuf,12,0);      
//            }else 
//            {
//                RemoteControl(s_FrmInfo->ucbuf,7,0);      
//            }
//            if(gun_EsamError.word.word==0)
//            {
//                ApiReverseData(s_FrmInfo->ucbuf,6);         //反转回来
//                if(s_FrmInfo->uiID.lword==METERIDNUM)       //表号
//                {
//                    SysE2ParaSetManage(EEP_METERNO,s_FrmInfo->ucbuf,6);
//                }else                                       //客户编号
//                {
//                    SysE2ParaSetManage(EEP_KHBH,s_FrmInfo->ucbuf,6);
//                }
//                s_FrmInfo->ucErr.word=0;
//                s_FrmInfo->ucLen=0;
//            }
//            else 
//            {  
//                s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//            }
//        }else
//        {
//            s_FrmInfo->ucErr.word=ComErrBit_ErrPw;                  //密级错误
//        }
//        
//        if(s_FrmInfo->ucErr.word!=0)
//        {    
//            s_FrmInfo->ucLen = 1;
//            return Ret_OK;
//        }
//    }
//    else   
//    {
//        if(s_FrmInfo->ucLevel==0x98)
//        {
//            uclen=s_FrmInfo->ucLen-12;;
//            ApiReverseData(s_FrmInfo->ucbuf,(uclen-4));
//            ApiReverseData(s_FrmInfo->ucbuf-4+uclen,4);
//            if(  guc_EsamAuthSt!=0x55
//               ||guc_EsamSuspend==true)         //身份认证不成功或者挂起，返回密码错
//            {
//                s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//                gun_EsamError.ESAMERRORWord.EsamAuthError=true;
//            }else
//            {
//                uclen=StrDep(s_FrmInfo->uiID.byte[2]%5,s_FrmInfo->ucbuf,uclen);
//                ApiReverseData(s_FrmInfo->ucbuf,4); //数据标识
//                if(s_FrmInfo->uiID.lword!=Uint8_To_Uint32(s_FrmInfo->ucbuf))
//                {
//                    s_FrmInfo->ucErr.word = ComErrBit_Nodata;            //没有找到数据标识
//                    s_FrmInfo->ucLen = 1;
//                    return Ret_OK;
//                }
//                
//                s_FrmInfo->ucbuf+=4;            //去掉数据标识的偏移量
//                s_FrmInfo->ucLen=uclen-4;       //减去数据标识的长度
//
//                if((s_FrmInfo->uiID.lword&0xffffff00)==0x04010000
//                   ||(s_FrmInfo->uiID.lword&0xffffff00)==0x04020000)
//                {                                                           //时区、时段反转
//                    for(i=0;i<s_FrmInfo->ucLen/3;i++)
//                    {
//                        ApiReverseData(s_FrmInfo->ucbuf+i*3,3);             //取反操作
//                    }
//                }else
//                {
//                    ApiReverseData(s_FrmInfo->ucbuf,s_FrmInfo->ucLen);      //取反操作
//                }
//            }
//           
//            if(gun_EsamError.word.word!=0)
//            {
//                if(   gun_EsamError.ESAMERRORWord.EsamError
//                   || gun_EsamError.ESAMERRORWord.EsamAuthError)
//                {    
//                    s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//                }
//                else
//                {    
//                    s_FrmInfo->ucErr.word=ComErrBit_Other;
//                }
//                s_FrmInfo->ucLen = 1;
//                return Ret_OK;
//            }
//        } 

        //02 或者04级
        for(i = 0; i < NumofCoreID; i++)                    //首先分类查找
        {
            if(s_FrmInfo->uiID.byte[3] == gs_CoreID[i].uiID3)
            {
                break;
            }
        }
    
        if(i >= NumofCoreID)                                    //看看有没有找到ID
        {
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;            //没有找到数据标识
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }

    
        Index = MidFindID(s_FrmInfo->uiID,gs_CoreID[i].pTab, gs_CoreID[i].ucNum);
    
        if(Index == 0xFF)                                       //如果没有找到
        {
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;           //没有找到数据标识
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    
        if(s_FrmInfo->uiID.byte[1] == 0xFF)                     //看看是否为集抄
        {
            s_FrmInfo->ucAtb |= Const_Atb_DataJC;
        }

        if(s_FrmInfo->ucCmd == 0x14)
        {
            if(((s_FrmInfo->uiID.lword&0xFFFFFF00)!=0x04010000)
               &&((s_FrmInfo->uiID.lword&0xFFFFFF00)!=0x04020000)
               )
            {
                if(s_FrmInfo->ucLen>(12+(gs_CoreID[i].pTab + Index)->ucLen))             
                {
                    s_FrmInfo->ucErr.word = ComErrBit_Other;           //没有找到数据标识
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }
        }
        
        if(s_FrmInfo->ucCmd == 0x12)    //如果主站不能分辨是否后续读
        {          
#ifdef SQSDPRLOG          
            if(((s_FrmInfo->uiID.lword&0xffffff00) == 0x03300500 && s_FrmInfo->uiID.byte[0] != 0)       //时段表编程
               || ((s_FrmInfo->uiID.lword&0xffffff00) == 0x03300600 && s_FrmInfo->uiID.byte[0] != 0)    //时区表编程
               || ((s_FrmInfo->uiID.lword&0xff00ff00) == 0x06000000))   //负荷记录相关
            {
                if(Ret_Err == FunList[(gs_CoreID[i].pTab + Index)->ucFunNo](s_FrmInfo, gs_CoreID[i].pTab + Index))
                {
                    return Ret_Err;
                }
            }
            else
#endif               
            if(Ret_Err == Report(s_FrmInfo, (S_CORETAB code *)gs_CoreTab4 + 0x32))
            {
                return Ret_Err;
            }
        }else
        //调用处理函数
        if(Ret_Err == FunList[(gs_CoreID[i].pTab + Index)->ucFunNo](s_FrmInfo, gs_CoreID[i].pTab + Index))
        {
            return Ret_Err;
        }
//    }


    if(s_FrmInfo->ucCmd == 0x14&&
       s_FrmInfo->ucErr.word==0)                        //如果是写数据
    {
        //SaveProgramLog(s_FrmInfo);                      //编程记录
    }
    return Ret_OK;
}

const Gs_E2RWCtr code gs_E2RwCtrTalbe[]=
{
    {0x20000002,    EEP_JBTOTAL,        sizeof(S_JBPM)  },      //0 校表参数
    {0x20000026,    EEP_RTCTEMPOFFSET,  1               },      //1 温度偏移量
    {0x20000027,    EEP_RTCBVAL,        20              },      //2 RTC B值
    {0x20000028,    EEP_RTCDDTEMP,      1               },      //3 RTC顶点温度
    {0x20000029,    EEP_RTCFRQOFFSET,   4               },      //4 RTC常温偏移量
    {0x4F520000,    EEP_AUTOCAL,        27              },      //5 设置标准表数据

//  {0x20000050,    EEP_PROTIME,        2               },      //6  编程有效时间(自扩)
//  {0x20000051,    EEP_BSTIME,         2               },      //7  闭锁时间(自扩)
//  {0x20000052,    EEP_BSCOUNT,        1               },      //8  闭锁次数(自扩)
    {0x20000053,    EEP_TZDLVAL,        4               },      //9  跳闸电流阀值（自扩）
    {0x20000054,    EEP_TZDLDELAY,      2               },      //10 跳闸电流延时（自扩）
    {0x20000055,    EEP_POWUPJXTM,      1               },      //11 有电下键显显示时间(自扩)
    {0x20000056,    EEP_POWDWJXTM,      1               },      //12 停电下键显显示时间(自扩)
    {0x20000057,    EEP_POWOFFXSTM,     2               },      //13 停电显示时间(自扩)
    {0x20000058,    EEP_FULLSHOWTM,     1               },      //14 全屏显示时间(自扩)
    {0x20000059,    EEP_POWUPBGTM,      1               },      //15 上电背光亮时间(自扩)
    {0x2000005A,    EEP_IRBGTM,         1               },      //16 红外触发背光亮时间(自扩)
    {0x2000005B,    EEP_KEYBGTM,        1               },      //17 按键触发背光亮时间(自扩)
    {0x2000005C,    EEP_ESAMERRBGTM,    1               },      //18 ESAM错误背光亮时间(自扩)
    {0x2000005D,    EEP_LEDSFCKTM,      1               },      //19 LED自检时间(自扩)
    {0x2000005E,    EEP_PRODUCE,        1               },      //20 生产模式状态(自扩)
    {0x2000005F,    EEP_PRODUCETM,      2               },      //21 生产模式退出时间(自扩)

    {0x20000060,    EEP_CFSAVECFG,      1               },      //22  CF保存配置项（自扩）
//  {0x20000061,    EEP_CBRCFG,         1               },      //23  抄表日补冻配置项（自扩）
//  {0x20000062,    EEP_DJCFG,          1               },      //24  冻结补冻配置项（自扩）
//  {0x20000063,    EEP_PWLCFG,         2               },      //25  密码等级有效配置项（自扩）
    {0x20000064,    EEP_SQSDCFG,        1               },      //26 时区和时段切换时间设置判别配置项（自扩）
    {0x20000065,    EEP_ENSHOWCFG,      1               },      //27 电量显示格式配置项（自扩）
    {0x20000066,    EEP_TMSHOWCFG,      1               },      //28 时间显示格式配置项（自扩）
    {0x20000067,    EEP_PWOFFSHOWMDCFG, 1               },      //29 停电显示模式配置项（自扩）
//  {0x20000068,    EEP_JDQCKCFG,       1               },      //30 继电器检测方式配置项（自扩）
    {0x20000069,    EEP_METERCLRCNT,    1               },      //31 电表清零次数（自扩）
    {0x2000006A,    EEP_METERCLFENGVAL, 4               },      //32 电表清零电量阀值（自扩）
    {0x2000006B,    EEP_SHOWERRPOS,     1,              },      //33 ERR显示位置
    {0x20000080,    EEP_PDIRTIME,       1,              },      //34 功率反向事件判定延时时间
    {0x20000081,    EEP_PDIRGATE,       3,              },      //35 功率反向事件有功功率触发下限
    
};
/*=========================================================================================\n
* @function_name: FactoryRD
* @function_file: DLT_645_2007.c
* @描述: 厂内读函数
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 FactoryRD(S_FRAMEINFO *s_FrmInfo)
{
    uint8 i;
    int16 temp;
//    uint32 ulValue;
    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);    //数据标识

    if(s_FrmInfo->uiID.lword==0x20000011)
    {
        //temp=gi_Temprature/10;
        temp = 0;
        s_FrmInfo->ucbuf[4]=(uint8)(temp>>8);
        s_FrmInfo->ucbuf[5]=(uint8)(temp);
        s_FrmInfo->ucLen = 6;
        return Ret_OK;
    }else if(s_FrmInfo->uiID.lword==0x20000070)
    {
//      ulValue=EnyB_ReadMeterParaACK(CRPST); 
//      s_FrmInfo->ucbuf[4]=(uint8)ulValue;
        s_FrmInfo->ucLen = 5;
        return Ret_OK;
    }
    else if(s_FrmInfo->uiID.lword==0x25000000)
    {
        uint8 len=s_FrmInfo->ucbuf[6];
        uint8 phy;
        //选择读取那一相
        switch(s_FrmInfo->ucbuf[7])
        {
            case 0x01:
                phy=APhy;
                break;
            case 0x02:
                phy=BPhy;
                break;
            case 0x03:
                phy=CPhy;
                break;
        }
        if(ReadRaccoon(Uint8_To_Uint16(s_FrmInfo->ucbuf + 4),len,phy))
        {
            memcpy(s_FrmInfo->ucbuf + 4,gs_RacCtrl.ucBuf+3,len*4);
            s_FrmInfo->ucLen = len*4+4;
        }else
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Other;            //其他错误
        }
        return Ret_OK;
    }
    else
    {
        for(i=0;i<sizeof(gs_E2RwCtrTalbe)/sizeof(Gs_E2RWCtr);i++)
        {
            if(s_FrmInfo->uiID.lword==gs_E2RwCtrTalbe[i].ulId)
            {
                BE_ReadP(gs_E2RwCtrTalbe[i].E2Addr,s_FrmInfo->ucbuf + 4,gs_E2RwCtrTalbe[i].ucLen);
                s_FrmInfo->ucLen = 4+gs_E2RwCtrTalbe[i].ucLen;

                return Ret_OK;
            }
        }
    }

    s_FrmInfo->ucReply = 0;                             //不需要回复*/
    return Ret_OK;
}

extern const uint8 code InitPara5[sizeof(S_JBPM)-2];
/*=========================================================================================\n
* @function_name: FactoryWR
* @function_file: DLT_645_2007.c
* @描述: 厂内写函数
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 FactoryWR(S_FRAMEINFO *s_FrmInfo)
{
   uint8 i;
//   uint8 ucbuf[4];
//   uint32 temp;
   uint16 uiCurrent;;
  
    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);   //数据标识

    if(s_FrmInfo->uiID.lword==0xFE005500)
    {
       // if((guc_CoverStatus==1)
        //   ||(guc_FactoryType==FactorMd))              //开表盖状态，可以进入厂内状态  
        {
            guc_FactoryType=FactorMd;
            SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);     //写入厂内状态
            BE_ReadP(EEP_PRODUCETM,(uint8*)&gui_DyProDmOut,0x02);           //厂内时间
            BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);      //厂内剩余时间
            s_FrmInfo->ucLen = 0;
            return Ret_OK;
            
        }
       //else
      //  {    
      //      s_FrmInfo->ucErr.word = ComErrBit_ErrPw;            //没有找到数据标识
      //      s_FrmInfo->ucLen = 1;
      //      return Ret_OK;
      //  }
    }

    if(guc_FactoryType!=FactorMd)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;                //没有找到数据标识
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    if(s_FrmInfo->uiID.lword==0xFE00AA00)
    {
        gui_DyProDmOut=0;
        guc_FactoryType=0;
        BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);      //厂内剩余时间
        SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);     //写入厂内状态
    }else if(s_FrmInfo->uiID.lword==0x4F520000)                         //自动校表 100%Ib
    {   
        BE_WriteP(EEP_AUTOCAL, s_FrmInfo->ucbuf + 4, 27);
        EnyB_AutoCheck();
    }else if(s_FrmInfo->uiID.lword==0x4F520001)                         //自动校表  100%Ib 0.5L 默认PHCCtrl1已写0
    {   
        BE_WriteP(EEP_AUTOCAL, s_FrmInfo->ucbuf + 4, 27);
        EnyB_AutoChkAngle();
    }
  else if(s_FrmInfo->uiID.lword==0x4F520002)     //自动校表  1%Ib 1.0
  {
      BE_WriteP(EEP_AUTOCAL, s_FrmInfo->ucbuf + 4, 27);
      //EnyB_AutoChkPW2();
  }
    else if(s_FrmInfo->uiID.lword==0x4F520003)                          //零线电流校正
    {
        uiCurrent=Uint8_To_Uint16(s_FrmInfo->ucbuf + 4);
        EnyB_AutoChkInrms(uiCurrent);
    }
    else if(s_FrmInfo->uiID.lword==0x2055FF00)                          //初始化校表参数
    {
        SetCRCWord(guc_InitWd);
        guc_RTCSave = 0x55;
        gui_RefreshEvent|=flgEtPara_Init;
    }
    else if(s_FrmInfo->uiID.lword==0x2055FF55)     //初始化校表参数,不带RTC
    {
        SetCRCWord(guc_InitWd);
        guc_RTCSave = 0x00;
        gui_RefreshEvent|=flgEtPara_Init;
    }
    else if(s_FrmInfo->uiID.lword==0x20000001)                          //满度初始化  zyj 1112 需要上位机配合调试
    {
        //b0 04//常数
        //额定电压
        //额定电流
       
        memset((uint8*)&gs_JbPm.ui_MeterC,0,sizeof(S_JBPM));                //先全部清零
        memcpy((uint8*)&gs_JbPm.ui_MeterC,s_FrmInfo->ucbuf+4,6);            //常数,额定电压，额定电流 
        memcpy((uint8*)&gs_JbPm.RacEGYTH,s_FrmInfo->ucbuf+10,8);           //有功门限制、无功门限制 

        gs_JbPm.RacSysCtrl = SysCtrl_ValueDef;    //U: x1  IA: x32  IB: x32  +50 PPM
        gs_JbPm.RacBPFPARA = BPFPARA_ValueDef;
        memcpy((uint8*)&gs_JbPm.ul_PG,s_FrmInfo->ucbuf+42,12);              //功率显示系数、电压显示系数、电流显示系数
        
        gs_JbPm.RacEGYTH /= 2;   //P  9260S:0.6055x10^9  / 9260:1.419x10^9 = 0.4267 = 1/2.34
        gs_JbPm.RacREEGYTH = gs_JbPm.RacEGYTH;
        gs_JbPm.ul_PG /= 2;      //P  9260S:0.6055x10^9  / 9260:1.419x10^9 = 0.4267 = 1/2.34
        gs_JbPm.ul_URmG /= 2;    //U  9260S:0.8719x10^9  / 9260:1.839x10^9 = 0.4741 = 1/2.11
        gs_JbPm.ul_I1RmG /= 2;   //I  9260S:0.7457x10^9  / 9260:1.839x10^9 = 0.4055 = 1/2.47
          
        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
        
//      memset((uint8*)&gs_GiantSCP.ul_Adjust,0,6);
//      SysE2ParaSetManage(EEP_DDLXZ,(uint8*)&gs_GiantSCP.ul_Adjust,6);

        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000003)                                  //功率比差值
    {
        gs_JbPm.gs_JBA.RacPAC=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);             //有功比差值
        gs_JbPm.gs_JBB.RacPAC=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);             //有功比差值
        gs_JbPm.gs_JBC.RacPAC=Uint8_To_Uint32(s_FrmInfo->ucbuf+12);            //有功比差值
        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);             // 校表参数的CRC结果
        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));        //写入E2中

        gui_RefreshEvent |= flgEtPara_EnyBottom;
      
    }
    else if(s_FrmInfo->uiID.lword==0x20000004)                          //电压电流比差值
    {
        gs_JbPm.gs_JBA.RacUC=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);             //有功比差值
        gs_JbPm.gs_JBA.RacIAC=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);            //有功比差值

        gs_JbPm.gs_JBB.RacUC=Uint8_To_Uint32(s_FrmInfo->ucbuf+12);             //有功比差值
        gs_JbPm.gs_JBB.RacIAC=Uint8_To_Uint32(s_FrmInfo->ucbuf+16);            //有功比差值

        gs_JbPm.gs_JBC.RacUC=Uint8_To_Uint32(s_FrmInfo->ucbuf+20);             //有功比差值
        gs_JbPm.gs_JBC.RacIAC=Uint8_To_Uint32(s_FrmInfo->ucbuf+24);            //有功比差值
        
        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中

        gui_RefreshEvent |= flgEtPara_EnyBottom;

    }
    else if(s_FrmInfo->uiID.lword==0x20000005)                          //零线电流比差值
    {
        //gs_JbPm.RacWARTM=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);            //有功比差值
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
                                                                        //
//        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000006)                          //A角差
    {
        uint16 JbJc;
        JbJc=Uint8_To_Uint16(s_FrmInfo->ucbuf+4);                       //角差
        gs_JbPm.gs_JBA.RacPHC = 0;
        if( JbJc & EBIT10)  //符号位
        {
          gs_JbPm.gs_JBA.RacPHC |= EBIT8;
          gs_JbPm.gs_JBA.RacPHC |= ((~(uint8)JbJc)+1);
        }else
        {
          gs_JbPm.gs_JBA.RacPHC |= (uint8)JbJc;
        }
        
        
        JbJc=Uint8_To_Uint16(s_FrmInfo->ucbuf+6);                       //角差
        gs_JbPm.gs_JBB.RacPHC = 0;
        if( JbJc & EBIT10)  //符号位
        {
          gs_JbPm.gs_JBB.RacPHC |= EBIT8;
          gs_JbPm.gs_JBB.RacPHC |= ((~(uint8)JbJc)+1);
        }else
        {
          gs_JbPm.gs_JBB.RacPHC |= (uint8)JbJc;
        }


        JbJc=Uint8_To_Uint16(s_FrmInfo->ucbuf+8);                       //角差
        gs_JbPm.gs_JBC.RacPHC = 0;
        if( JbJc & EBIT10)  //符号位
        {
          gs_JbPm.gs_JBC.RacPHC |= EBIT8;
          gs_JbPm.gs_JBC.RacPHC |= ((~(uint8)JbJc)+1);
        }else
        {
          gs_JbPm.gs_JBC.RacPHC |= (uint8)JbJc;
        }
        
        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));    //写入E2中
        gui_RefreshEvent |= flgEtPara_EnyBottom;

    }
    else if(s_FrmInfo->uiID.lword==0x20000007)                          //二次补偿值
    {
        gs_JbPm.gs_JBA.RacPADCC=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);           //二次补偿值
        gs_JbPm.gs_JBB.RacPADCC=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);           //二次补偿值
        gs_JbPm.gs_JBC.RacPADCC=Uint8_To_Uint32(s_FrmInfo->ucbuf+12); 
        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM)); //写入E2中
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000008)                         //大电流校正补偿
    {
//        gs_GiantSCP.ui_CRC=do_CRC(s_FrmInfo->ucbuf+4,4);                //不带CRC
//        gs_GiantSCP.ul_Adjust = Uint8_To_Uint32(s_FrmInfo->ucbuf+4); 
//        SysE2ParaSetManage(EEP_DDLXZ,(uint8*)&gs_GiantSCP.ul_Adjust,6);
//        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000009)                          //无功二次补偿值
    {
        gs_JbPm.gs_JBA.RacQADCC=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);  //二次补偿值
        gs_JbPm.gs_JBB.RacQADCC=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);  //二次补偿值
        gs_JbPm.gs_JBC.RacQADCC=Uint8_To_Uint32(s_FrmInfo->ucbuf+12); 
        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM)); //写入E2中
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000030)                         //RTC数据从FLASH跟新到E2中
    {
        gui_RefreshEvent |= flgEtPara_RtcParaFsToE2;
    }
    else if(s_FrmInfo->uiID.lword==0x25000000)
    {
        //选择写入那一相
        switch(s_FrmInfo->ucbuf[8])
        {
            case 0x01:
                gs_BPFPARA_Value.BPFPARA_Value[0] = Uint8_To_Uint32(s_FrmInfo->ucbuf+4);
                break;
            case 0x02:
                gs_BPFPARA_Value.BPFPARA_Value[1] = Uint8_To_Uint32(s_FrmInfo->ucbuf+4);
                break;
            case 0x03:
                gs_BPFPARA_Value.BPFPARA_Value[2] = Uint8_To_Uint32(s_FrmInfo->ucbuf+4);
                break;
            default:
                s_FrmInfo->ucErr.word = ComErrBit_Other;                //没有找到数据标识
                s_FrmInfo->ucLen = 1;
                return Ret_OK;
        }
        gs_BPFPARA_Value.ui_CRC=do_CRC((uint8*)&gs_BPFPARA_Value,sizeof(S_BPFPARA_Value)-2);
        BE_WriteP(EEP_BPFPARA_VALUE,(uint8 *)&gs_BPFPARA_Value,sizeof(S_BPFPARA_Value));
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else
    {
        for(i=0;i<sizeof(gs_E2RwCtrTalbe)/sizeof(Gs_E2RWCtr);i++)
        {
            if(s_FrmInfo->uiID.lword==gs_E2RwCtrTalbe[i].ulId)
            {
                SysE2ParaSetManage(gs_E2RwCtrTalbe[i].E2Addr, s_FrmInfo->ucbuf+4,gs_E2RwCtrTalbe[i].ucLen);
                break;
            }
        }

        if(i==0)
        {
            gui_RefreshEvent |= flgEtPara_EnyBottom;
        }
        else if(0<i&&i<=5)
        {
            gui_RefreshEvent |=  flgEtPara_RTC;     //刷新RTC参数
        }
        else if(i>5)
        {

            gui_RefreshEvent |=  (flgEtPara_Config+flgEtPara_Show+flgEtPara_PowDir);  //刷新配置参数
        }


    }
    s_FrmInfo->ucLen = 0;
    return Ret_OK;
}

#define BYTE(num, n)        *((uint8 code*)&(num) + (n))
/*=========================================================================================\n
* @function_name: MidFindID
* @function_file: DLT_645_2007.c
* @描述: 中值法查找ID
*
*
* @参数:
* @param:id
* @param:tab
* @param:num
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-03-01)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 MidFindID(Word32 id, S_CORETAB code * tab, uint8 num)
{
    uint32  CmpID;
    uint8  ucMin = 0;
    uint8  ucMax = num - 1;
    uint8  ucMid;

    if(id.byte[3]==0x00)                                //电量
    {
        if((id.lword&0xFF00ffff)==0x0000ffff)           //没有找到数据标识
        {
            return 0xFF;
        }

        if(id.byte[1]!=0xFF
           &id.byte[1]>=0x05)
        {
            return 0xFF;
        }
        
        if((id.byte[2]!=0x0c)
           &&(id.byte[2]!=0x0b))
        {
            id.byte[1]=0x00;
        }
    }else if(id.byte[1]==0xFF)                          //如果是集抄的话
    {
        id.byte[1]=0x00;
    }

    if(id.lword < tab->uiID)
    {                                                   //当核心数据表中第一个ID不是0时,那么uiID小于这个ID就有可能出错
        return 0xFF;
    }

    while(1)
    {
        ucMid = (ucMin + ucMax) >> 1;                   //相当于除以2取中间值

        CmpID = (tab + ucMid)->uiID;

        if (id.lword == CmpID)                          //找到相等数据
        {
            break;
        }
        else if(id.lword > CmpID)                       //查找数据比找到的数据大
        {
            if (ucMid == ucMax)                         //已经查完
            {
                break;
            }
            ucMin = ucMid + 1;
        }
        else                                            //查找数据比找到的数据小
        {
            if (ucMid == ucMin)                         //已经查完
            {
                if(ucMid > 0)
                {
                    ucMid--;
                }
                break;
            }
            ucMax = ucMid - 1;
        }
    }

    if((id.byte[0] <= (BYTE((tab + ucMid)->uiID,0)+(tab + ucMid)->ucTotal0))
       &&(id.byte[1] <= (BYTE((tab + ucMid)->uiID,1)+(tab + ucMid)->ucTotal1))
       &&(id.byte[2] <= (BYTE((tab + ucMid)->uiID,2))))
    {
        return ucMid;
    }

    return 0xFF;
}
/*=========================================================================================\n
* @function_name: ChangePassWord
* @function_file: DLT_645_2007.c
* @描述: 修改密码
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ChangePassWord(S_FRAMEINFO *s_FrmInfo)
{
    //判读表地址是否正确
    if(s_FrmInfo->ucAtb != Const_Addr_Normal)                //正常地址
    {
        s_FrmInfo->ucReply = 0;                  //不需要回复的
        return Ret_OK;
    }
    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);        //拷贝数据标识
    if(guc_FactoryType!=FactorMd)                           //不在厂内模式
    {
       if((s_FrmInfo->ucbuf[8]!=0x02)
          &&(s_FrmInfo->ucbuf[8]!=0x04)
            )
       {
            s_FrmInfo->ucErr.word = ComErrBit_Other;             //未授权
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
       }else  if((s_FrmInfo->ucbuf[8]==0x02) &&(s_FrmInfo->uiID.lword!=0x04000c03)
           ||(s_FrmInfo->ucbuf[8]==0x04)&&(s_FrmInfo->uiID.lword!=0x04000c05))
        {
            s_FrmInfo->ucErr.word = ComErrBit_Other;             //未授权
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
 
        if((!(gui_SystemState & flgStSys_Program)) 
           ||(s_FrmInfo->ucbuf[4] > s_FrmInfo->ucbuf[8]))   //如果不在编程状态或者新密码比旧密码等级高
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;             //未授权
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }

        s_FrmInfo->ucLevel = JudgePassword(s_FrmInfo->ucbuf + 4);
        if(s_FrmInfo->ucLevel == 0xFF)                           //判断权限
        {
            s_FrmInfo->ucAtb = Const_Atb_Pswd_Err;
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;             //密码错误
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    } 
    SysE2ParaSetManage(EEP_645PASSWD1 + (s_FrmInfo->ucbuf[8]) * 3, s_FrmInfo->ucbuf + 9, 3);//写入新密码
    memcpy(s_FrmInfo->ucbuf,s_FrmInfo->ucbuf+8,4);
    s_FrmInfo->ucLen = 4;
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ChangeBode
* @function_file: DLT_645_2007.c
* @描述: 更改波特率
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ChangeBode(S_FRAMEINFO *s_FrmInfo)
{

    uint8 i;
    uint8 mode;
    if(s_FrmInfo->ucAtb!=Const_Addr_Normal)      //如果设置不是普通地址，则返回错误
    {
        s_FrmInfo->ucReply = 0;                  //不需要回复的
        return Ret_OK;
    }


    mode = s_FrmInfo->ucbuf[0];             //更改的模式,第一个字节是长度

    for(i = 2; i < 6; i++)                  //判断特征字是否正确
    {
        if(mode == BIT8MAP[i])
        {
            break;
        }
    }

    if(i >= 6 || s_FrmInfo->ucPort==Port_Uart2)
    {   
        s_FrmInfo->ucErr.word = ComErrBit_ChBod;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
 
    //判断通道，485还是红外
    if(s_FrmInfo->ucPort==Port_Uart2)               //485通道
    {                                               //设置485波特率 
        SysE2ParaSetManage(EEP_UARTBDM,&mode,1);    //通信口1通信速率特征字

    }
//    else if(s_FrmInfo->ucPort==Port_Uart2)         //红外通道的话，设置红外波特率
//    {
//        SysE2ParaSetManage(EEP_IARBDM,&mode,1);    //调制型红外光口通信速率特征字
//    }
    
    guc_BodeDely=40;
    s_FrmInfo->ucLen = 1;
    return Ret_OK;
} 
/*=========================================================================================\n
* @function_name: ClearMeter
* @function_file: DLT_645_2007.c
* @描述: 清电表
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ClearMeter(S_FRAMEINFO *s_FrmInfo)
{
    if(s_FrmInfo->ucAtb != Const_Addr_Normal)               //正常地址
    {
        s_FrmInfo->ucReply = 0;                             //不需要回复的
        return Ret_OK;
    }
 
    if(guc_FactoryType!=FactorMd)                           //不在厂内模式
    {
        s_FrmInfo->ucLevel = JudgePassword(s_FrmInfo->ucbuf);

        if((!_IsSecuProduct) 
           || (s_FrmInfo->ucLevel != 0x02))                 //没有编程键、密码不对
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;        //权限不足返回
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    }

    gui_SystemEvent|=flgEtGlb_ClearMeter;                   //发送清电表消息
    s_FrmInfo->ucLen = 0;
    memcpy(guw_UseCode.byte,s_FrmInfo->ucbuf+4,4);    //操作者代码
    //复制操作者代码
    SetCRCWord(guc_ClearWd);
    return Ret_OK;
    
}
 
/*=========================================================================================\n
* @function_name: ClearEvent
* @function_file: DLT_645_2007.c
* @描述: 清事件
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ClearEvent(S_FRAMEINFO *s_FrmInfo)
{
  /*  Word32 ID;
    uint8 i;

    if(s_FrmInfo->ucAtb != Const_Addr_Normal)
    {
        s_FrmInfo->ucReply = 0;                  //不需要回复的
        return Ret_OK;
    }

    memcpy(guw_UseCode.byte,s_FrmInfo->ucbuf+4,4);    //操作者代码 
    memcpy(ID.byte,s_FrmInfo->ucbuf+8,4);


    if(guc_FactoryType!=FactorMd)                           //不在厂内模式
    {
        s_FrmInfo->ucLevel = JudgePassword(s_FrmInfo->ucbuf);

        if((!_IsSecuProduct)
           ||(0x02 != s_FrmInfo->ucLevel))                  //没有编程键、密码不对
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;          //权限不足返回
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    }

    if(ID.byte[0] != 0xFF)                              //事件清零数据标识ID0肯定是FF
    {
        s_FrmInfo->ucErr.word |= ComErrBit_ErrPw;            //权限不足
        s_FrmInfo->ucLen = 1;                           //错误信息字一个字节
    }
    else
    {
        for(i = 0; i < CntgClrE; i++)                   //寻找清事件的标志
        {
            if(ID.lword == gs_ClearEvent[i].ID)         //看看是否相等，相等就是找到了
            {
                break;
            }
        }

        if(i >= CntgClrE)                               //看看有没有找到
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Nodata;
            s_FrmInfo->ucLen = 1;
        }
        else
        {
            guc_ClearType = gs_ClearEvent[i].type;          //清事件的类型

            gui_SystemEvent|=flgEtGlb_ClearEv;              //发送电表清事件消息
            s_FrmInfo->ucLen = 0;
        }
    }*/
    return Ret_OK;
}
 
/*=========================================================================================\n
* @function_name: BroadcastTime
* @function_file: DLT_645_2007.c
* @描述: 广播校时
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 BroadcastTime(S_FRAMEINFO *s_FrmInfo)
{
    Word16 W16_JSR[3];
    uint8 i;
    if(s_FrmInfo->ucAtb != Const_Addr_Broadcast)        //如果是广播地址的话
    {
        return Ret_Err;
    }
    //首先读取3个结算日

    BE_ReadP(EEP_CBR1,W16_JSR[0].byte, 6);//读取3个结算日


    if(gs_DateTime.ucHour==0x23) //半夜0点前后5MIN不能校准时间
    {
        if(gs_DateTime.ucMinute>=0x55)
        {
            return Ret_Err;
        }
    }

    if(gs_DateTime.ucHour==0)   //半夜0点前后5MIN不能校准时间
    {
        if(gs_DateTime.ucMinute<=0x04)
        {
            return Ret_Err;
        }
    }

    //抄表日前后5min不允许校时
    for(i=0;i<3;i++)                            //三个结算日
    {
        //判断当前
        if(W16_JSR[i].byte[1]==gs_DateTime.ucDay)   //这里要主要一下，小端模式
        {
            if(W16_JSR[i].byte[0]==gs_DateTime.ucHour)
            {
                if(gs_DateTime.ucMinute<=0x04)
                {
                    return Ret_Err;
                }
            }else if(((APIByteBCD2HEX(gs_DateTime.ucHour))+1)==APIByteBCD2HEX(W16_JSR[i].byte[0]))        
            {   //23点上面已经判断
                if(gs_DateTime.ucMinute>=0x55)
                {
                    return Ret_Err;
                }
            }else
            {
                continue;
            }
        }else
        {
            continue;
        }
    }

    S_TIMEDATA time;
 
    memcpy(&time.ucSecond,s_FrmInfo->ucbuf,6);


    if(Ret_OK != ValidTime(s_FrmInfo->ucbuf, 2))    //看看时间是否合法，S mm H D MM YY
    {
        return Ret_Err;
    }
    //这里判断是否在5min内，第一种方法是，首先判断年月日是否相等，相等后，计算时分秒是否相差5min
    //第2中方法是，把年月日时分秒相加是否相差5min，目前这里采用第2中方法
//  if(ApiBufCmp(gs_DateTime.ucYear,s_FrmInfo->ucbuf,3)!=CMP_EQU)
//  {
//
//
//  }

    if(CalcTimeGap(&time, &gs_DateTime) > 300)          //看看是不是在5分钟以内
    {
        return Ret_Err;                                 //不满足条件，返回
    }
    
    if(gui_SystemState &flgStSys_IsTodayJS)             //如果今天已经校正过，就不允许校正
    {
        return Ret_Err;                                 //不满足条件，返回
    }else
    {
        //广播校时不会过日,所以星期不会变,所以这里不需要星期
        //SetSysRTCClock(s_FrmInfo->ucbuf, Const_YYMMDDhhmmss);
        gui_SystemState |= flgStSys_IsTodayJS;          //表示今天已经进行过广播校时了，一天只允许校时一次
        gui_RefreshEvent|= flgEtPara_Fee;               //刷新费率参数
        //guc_Timechange = 0x55;
    }
    s_FrmInfo->ucReply = 0;                             //不需要回复*/
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: Freeze
* @function_file: DLT_645_2007.c
* @描述: 冻结，根据规约来进行相应的冻结
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 Freeze(S_FRAMEINFO *s_FrmInfo)
{

    if((s_FrmInfo->ucAtb != Const_Addr_Broadcast)
       &&(s_FrmInfo->ucAtb != Const_Addr_Normal))           //广播不需要回复
    {
        s_FrmInfo->ucReply = 0;                             //不需要回复的
        return Ret_OK;
    }


 
    if((s_FrmInfo->ucbuf[0] == 0x99) && (s_FrmInfo->ucbuf[1] == 0x99) &&
       (s_FrmInfo->ucbuf[2] == 0x99) && (s_FrmInfo->ucbuf[3] == 0x99))
    {        
        //设置瞬时冻结
        guc_DjCheckFlg|=flgInsDjEvent;
    }
    else
    {

        if(JudgeIsNum(s_FrmInfo->ucbuf,4)==Const_Data_OK)
        {
            if(s_FrmInfo->ucbuf[3]!=0x99)        //月
            {
                if(s_FrmInfo->ucbuf[3]<=0x12     //月       
                   &&s_FrmInfo->ucbuf[3]>=0x01)
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;            //数据帧错误
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }
            
            //2 日   1 时 0 分
            if(s_FrmInfo->ucbuf[2]!=0x99)
            {
                if(s_FrmInfo->ucbuf[2]<=0x28     //日       
                   &&s_FrmInfo->ucbuf[2]>=0x01)
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;            //数据帧错误
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }
    
            if(s_FrmInfo->ucbuf[1]!=0x99)
            {
                if(s_FrmInfo->ucbuf[1]<=0x23                        //时      
                   )
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;       //数据帧错误
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }

            if(s_FrmInfo->ucbuf[0]!=0x99)
            {
                if(s_FrmInfo->ucbuf[0]<=0x59                        //分
                   )
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;       //数据帧错误
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }

        }else
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Other;            //数据帧错误
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
        //判断一下格式是否正确
        SysE2ParaSetManage(EEP_DSFRZ,s_FrmInfo->ucbuf,4);    //写入定时冻结时间
    }

    if(s_FrmInfo->ucAtb == Const_Addr_Broadcast)        //广播不需要回复
    {
        s_FrmInfo->ucReply = 0;                         //不需要回复
    }
    else if((s_FrmInfo->ucAtb == Const_Addr_Normal) ||
        (s_FrmInfo->ucAtb == Const_Addr_Any))
    {
        s_FrmInfo->ucLen = 0;
    }
    else
    {
        s_FrmInfo->ucErr.word|= ComErrBit_Other;            //地址错误
        s_FrmInfo->ucLen = 1;
    }
    return Ret_OK;
}

const uint16 code SqSdTab[2]=
{
    EEP_SQCNT,EEP_FSDCNT,

};

/*=========================================================================================\n
* @function_name: RwSqSd
* @function_file: DLT_645_2007.c
* @描述: 读写日时区时段
*
*
* @参数:
* @param:s_FrmInfo
* @param:CTable
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-03-20)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 RwSqSd(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint16 addr;
    uint8 ucNum;
    uint8 ucType;
#ifdef SQSDPRLOG
    uint8 DataID;
#endif

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    //这里是以页为地址，虽然时段表只要42个字节，但是每个时段表占用一页
    addr = CTable->pAddr + (s_FrmInfo->uiID.byte[0]) *64;
    
    BE_ReadP(SqSdTab[CTable->uiOther-1],&ucNum,1);
    if(ucNum>14)
    {
        ucNum=14;
    }
    if(s_FrmInfo->ucCmd == 0x11)                //如果是读数据的话
    {
        //如果是日时段表的话，首先要读取每日切换数

        BE_ReadP(addr,s_FrmInfo->ucbuf, ucNum*3);
        s_FrmInfo->ucLen = ucNum*3;             //数据长度

    }else
    {
        if(CTable->uiOther==0x01)       //时区
        {
            ucType=SqDtFm;
        }else                           //时段
        {
            ucType=SdDtFm;
        }

        if(s_FrmInfo->ucLevel!=0x98)
        {
            s_FrmInfo->ucLen-=12;
        }
        //首先判断数据长度是否是3的整数倍
        if((s_FrmInfo->ucLen)%3==0)
        {
            if((s_FrmInfo->ucLen)<=42)         //需要配合时段表数和费率切换数来判断
            {
                //判断是否是BCD,判断是否费率数超，判断是否是否超过15min
                s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,ucType,s_FrmInfo->ucLen);
                if(s_FrmInfo->ucErr.word==0)
                {  
                #ifdef SQSDPRLOG
                    DataID=SeekDataID(s_FrmInfo->uiID);               //读取ID
                #endif    
                    memcpy(s_FrmInfo->ucbuf+s_FrmInfo->ucLen,s_FrmInfo->ucbuf+s_FrmInfo->ucLen-3,42-s_FrmInfo->ucLen);//补全
                    //BE_WriteP(addr,s_FrmInfo->ucbuf, 42);
                    SysE2ParaSetManage(addr,s_FrmInfo->ucbuf, 42);
                    s_FrmInfo->ucLen =0;                //数据长度
                #ifdef SQSDPRLOG
                    RSDJLlog(s_FrmInfo->uiUserCode.lword,DataID);     //日时段和时区编程记录
                #endif
                    gui_RefreshEvent |= flgEtPara_Fee;  //发送刷新费率参数
                    return Ret_OK;
                }else
                {
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }else
            {
                if(s_FrmInfo->uiID.byte[0]==0x00)
                {
                    s_FrmInfo->ucErr.word = ComErrBit_YearQ;
                }else
                {
                    s_FrmInfo->ucErr.word = ComErrBit_TimeD;
                }

                s_FrmInfo->ucLen = 1;
                return Ret_OK;
            }
        }
        
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ComReadRunCode
* @function_file: DLT_645_2007.c
* @描述: 读取电表运行状态字
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-13)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ComReadRunCode(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint8 ucOffSet;
    uint8 *pData;

    if(s_FrmInfo->ucCmd==0x14)     
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd==0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }


    if(s_FrmInfo->uiID.byte[0]==0xFF)
    {
        memcpy(s_FrmInfo->ucbuf,(uint8*)&gs_MeterState.ui_MeterState1.Word,CTable->ucLen);
        memset(s_FrmInfo->ucbuf+CTable->ucLen,0xFF,4);
        s_FrmInfo->ucLen = CTable->ucLen+4;                           //数据长度
        return Ret_OK;
    }else
    {
        ucOffSet=s_FrmInfo->uiID.lword-CTable->uiID;
        pData= (uint8*)(&gs_MeterState.ui_MeterState1.Word+ucOffSet);
        memcpy(s_FrmInfo->ucbuf,pData,CTable->ucLen);
    }
    s_FrmInfo->ucLen = CTable->ucLen;                           //数据长度
    return Ret_OK;
}

 
/*=========================================================================================\n
* @function_name: SecPulsCtrl
* @function_file: DLT_645_2007.c
* @描述: 多功能端子的控制
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 SecPulsCtrl(S_FRAMEINFO *s_FrmInfo)
{
    if((s_FrmInfo->ucAtb!=Const_Addr_Normal)
       &&(s_FrmInfo->ucAtb!=Const_Addr_Broadcast))      //如果设置不是普通地址，则返回错误
    {
        s_FrmInfo->ucReply = 0;                  //不需要回复的
        return Ret_OK;
    }

    if(s_FrmInfo->ucbuf[0] == 0x00)              //输出信号为秒脉冲
    {
        SwichPluseOutType(0);
        gs_SysRunSt.PluseType=false;
    }
    else if(s_FrmInfo->ucbuf[0] == 0x02)        //输出信号为时段投切
    {
        SwichPluseOutType(2);
        gs_SysRunSt.PluseType=true;
    } 
    else                                        //其他无效
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;     //错误字
    }

    s_FrmInfo->ucLen = 1;

    if(s_FrmInfo->ucAtb == Const_Addr_Broadcast)//广播地址
    {
        s_FrmInfo->ucReply = 0;                 //不需要回复的
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ReadAccuEgy
* @function_file: DLT_645_2007.c
* @描述: 读取月、年累计电量   other  ： 0x00    当前月度
*                                       0x01    上1月度
*                                       0x10    当前年度
*                                       0x11    上1年度
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadAccuEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    Word32 Curdt[3],Lstdt[3],data1;
    int16 iDelta[2];
    uint16 ui_Other=CTable->uiOther;
    if(s_FrmInfo->ucCmd == 0x14)//写
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(ui_Other&0x01)                                           //历史累计电量，上1月度
    {
        DataProcRead2Slice(LSDLID,1,20,4,Lstdt[1].byte);        //读取上2月度正向电量
        DataProcRead2Slice(LSDLID,1,40,4,Lstdt[2].byte);        //读取上2月度反向电量
        Lstdt[0].lword = Lstdt[1].lword + Lstdt[2].lword;       //上2月正+反
        DataProcRead2Slice(LSDLID,0,20,4,Curdt[1].byte);        //读取上1月度正向电量
        DataProcRead2Slice(LSDLID,0,40,4,Curdt[2].byte);        //读取上1月度反向电量
        Curdt[0].lword = Curdt[1].lword + Curdt[2].lword;       //上1月正+反
    }else                       //当前月度，年度电量
    {
        DataProcRead2Slice(LSDLID,0,20,4,Lstdt[1].byte);        //读取上1月度正向电量
        DataProcRead2Slice(LSDLID,0,40,4,Lstdt[2].byte);        //读取上1月度反向电量
        Lstdt[0].lword = Lstdt[1].lword + Lstdt[2].lword;       //上1月正+反
        iDelta[0] = Eny_GetEp1(1, 0,ABCPhy,active);                           //当前正向电量增量
        iDelta[1] = Eny_GetEp1(2, 0,ABCPhy,active);                           //当前反向电量增量
        Curdt[0].lword = gs_EnergyA.ulP[0] + gs_EnergyA.ulN[0] + iDelta[0] + iDelta[1];//正+反
    }
    
    data1.lword=Curdt[0].lword - Lstdt[0].lword;
    if(data1.lword > 0x80000000)                    //如果数据小于零，返回0；
    {
        data1.lword = 0;
    }else
    {
        data1.lword/=10;
        data1.lword = Hex2BCD(data1.lword);         //bcd转换
        //data1.lword >>=4;                           //小数处理
    }

    memcpy(s_FrmInfo->ucbuf,data1.byte,4);
    s_FrmInfo->ucLen=CTable->ucLen;
    return Ret_OK;

}
/*=========================================================================================\n
* @function_name: ReadZHLYEgy
* @function_file: DLT_645_2007.c
* @描述: 
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2013-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadZHLYEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    int16 iDelta[2];
    Word32 Curdt,data1,data2;
    uint8  ucPt;
    if(s_FrmInfo->ucCmd == 0x14)//写
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    BE_ReadB(EEP_SYZHLYDL_PT, (uint8*)&ucPt);                      //指针
    BE_ReadP(EEP_SYZHLYDLADDR+ucPt*4,data1.byte,4);                //读取上一个月的总电量

    if(CTable->uiOther&0x01)                                        //上1月度组合有功总累用电量
    {
        ucPt++;
        ucPt%=2;
        BE_ReadP(EEP_SYZHLYDLADDR+ucPt*4,data2.byte,4);           //读取上2月的总电量
        Curdt.lword=(data1.lword-data2.lword);

    }else                                                           //当前月度总电量
    {
        iDelta[0] = Eny_GetEp1(1, 0,ABCPhy,active);                               //当前正向电量增量
        iDelta[1] = Eny_GetEp1(2, 0,ABCPhy,active);                               //当前反向电量增量
        data2.lword = (gs_EnergyA.ulP[0] + gs_EnergyA.ulN[0] + iDelta[0] + iDelta[1]);//正+反
        Curdt.lword = data2.lword-data1.lword;
    }
    
    if(Curdt.lword > 0x80000000)                                    //如果数据小于零，返回0；
    {
        Curdt.lword = 0;
    }else
    {   
        Curdt.lword/=10;
        Curdt.lword = Hex2BCD(Curdt.lword);                         //bcd转换
//      Curdt.lword >>=4;                                           //小数处理
    }

    memcpy(s_FrmInfo->ucbuf,Curdt.byte,4);
    s_FrmInfo->ucLen=CTable->ucLen;
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ReadCurEgy
* @function_file: DLT_645_2007.c
* @描述: 读取当前电量数据
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadCurEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    int16 Eadd;
    uint32 *pEnyData,*pEnyData1;
    //Word32 data1;
    int32  data1;
    uint8 flag=0;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    pEnyData=(uint32*)gs_EnergyA.lCP;                    //获取电量数据结构首地址
    flag=0;

    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //如果是集抄的话
    {
        for(uint8 i = 0; i <= 4; i++)
        {
//            flag=0;
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+i);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], i,ABCPhy,active);      //取得某个费率的电量增量
            data1 += Eadd;
            if(s_FrmInfo->uiID.byte[2]==0)                      //组合
            {
                if(data1>0x80000000)
                {
                    data1=-data1;
                    flag=1;
                }
            }
            data1/=10;
            data1= Hex2BCD(data1);                              //转换为BCD码
            //data1 >>=4;                                         //小数位
            if(flag)
            {
                data1|=0x80000000;
            }
            else if(s_FrmInfo->uiID.byte[2]==0)                 //组合大于799999.99,从0开始
            {
                data1&=~0x80000000;
            }
            memcpy((uint8*)&s_FrmInfo->ucbuf[i * 4],(uint8*)&data1,4);
        }

        s_FrmInfo->ucLen = 20;
    }
    else                                        //不是集抄
    {                                           //取得某个费率的电量增量
       if(s_FrmInfo->uiID.byte[2] <20)
        {
//            flag=0;
            guc_ShowEnergyFlag=0;  
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+s_FrmInfo->uiID.byte[1]);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], s_FrmInfo->uiID.byte[1],CTable->uiOther,active);
            data1 += Eadd;
        }
       else   //uiID.byte[2] = 0x15、0x29、0x3D
        {
            pEnyData1=(pEnyData+s_FrmInfo->uiID.byte[2]/20 +14 +s_FrmInfo->uiID.byte[1]); //指向 ulAP、ulBP、ulCP
            data1 = *pEnyData1;
            Eadd = Eny_GetEp1(1, 0,CTable->uiOther,active);
            data1 += Eadd;
        }  
        
        if(s_FrmInfo->uiID.byte[2]==0)                      //组合
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=-data1;
                flag=1;
            }
        }else if(s_FrmInfo->uiID.byte[2]==0x02)     //反向电量
        {
            guc_ShowEnergyFlag=0x55;
        }
        data1/=10;
        data1 = Hex2BCD(data1);             //转换为BCD码
        //data1 >>= 4;                        //小数位
        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)                 //组合大于799999.99,从0开始
        {
            data1&=~0x80000000;
        }

        memcpy(s_FrmInfo->ucbuf,(uint8*)&data1,4);
        s_FrmInfo->ucLen = 4;
    }
    return Ret_OK;
}


uint8 ReadCurNEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    int16 Eadd;
    uint32 *pEnyData,*pEnyData1;
    //Word32 data1;
    int32  data1;
    uint8 flag;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    pEnyData=(uint32*)gs_NEnergyA.lCP;                    //获取电量数据结构首地址
    flag=0;

    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //如果是集抄的话
    {
        for(uint8 i = 0; i <= 4; i++)
        {
//            flag=0;
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+i);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], i,ABCPhy,reactive);      //取得某个费率的电量增量
            data1 += Eadd;
            if(s_FrmInfo->uiID.byte[2]==0)                      //组合
            {
                if(data1>0x80000000)
                {
                    data1=-data1;
                    flag=1;
                }
            }
            data1/=10;
            data1= Hex2BCD(data1);                              //转换为BCD码
            //data1 >>=4;                                         //小数位
            if(flag)
            {
                data1|=0x80000000;
            }
            else if(s_FrmInfo->uiID.byte[2]==0)                 //组合大于799999.99,从0开始
            {
                data1&=~0x80000000;
            }
            memcpy((uint8*)&s_FrmInfo->ucbuf[i * 4],(uint8*)&data1,4);
        }

        s_FrmInfo->ucLen = 20;
    }
    else                                        //不是集抄
    {                                           //取得某个费率的电量增量
       if(s_FrmInfo->uiID.byte[2] <20)
        {
//            flag=0;
            guc_ShowEnergyFlag=0;  
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+s_FrmInfo->uiID.byte[1]);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], s_FrmInfo->uiID.byte[1],CTable->uiOther,reactive);
            data1 += Eadd;
        }
       else
        {
            pEnyData1=(pEnyData+s_FrmInfo->uiID.byte[2]/20 +14 +s_FrmInfo->uiID.byte[1]);
            data1 = *pEnyData1;
            Eadd = Eny_GetEp1(1, 0,CTable->uiOther,reactive);
            data1 += Eadd;
        }  
        
        if(s_FrmInfo->uiID.byte[2]==0)                      //组合
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=-data1;
                flag=1;
            }
        }else if(s_FrmInfo->uiID.byte[2]==0x02)     //反向电量
        {
            guc_ShowEnergyFlag=0x55;
        }
        data1/=10;
        data1 = Hex2BCD(data1);             //转换为BCD码
        //data1 >>= 4;                        //小数位
        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)                 //组合大于799999.99,从0开始
        {
            data1&=~0x80000000;
        }

        memcpy(s_FrmInfo->ucbuf,(uint8*)&data1,4);
        s_FrmInfo->ucLen = 4;
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ReadHisEgy
* @function_file: DLT_645_2007.c
* @描述: 读取历史电量
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadHisEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint32 data1;
    uint8 i;
    uint8 ucMonth;
    uint8 ucoffset;
    uint8 flag;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ucMonth=(s_FrmInfo->uiID.byte[0]-1);

    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //如果是集抄的话
    {
        ucoffset=(s_FrmInfo->uiID.byte[2] *20);
        DataProcRead2Slice(LSDLID,ucMonth,ucoffset,20,XDATA_RAMZONE);
        for(i = 0; i <= 4; i++)
        {         
            flag=0;                                      //读取数据
            data1=*((uint32*)(XDATA_RAMZONE+i*4));
            if(s_FrmInfo->uiID.byte[2]==0)                      //组合
            {
                if(data1>0x80000000)
                {
                    data1=~data1+1;
                    flag=1;
                }
            }
            data1/=10;
            data1 = Hex2BCD(data1);             //转换为BCD码
            //data1 >>=4;                         //相当于除以10
            if(flag)
            {
                data1|=0x80000000;
            }
            else if(s_FrmInfo->uiID.byte[2]==0)
            {
                data1&=~0x80000000;
            }

            memcpy(s_FrmInfo->ucbuf + (i << 2), (uint8*)&data1, 4);
        }
        s_FrmInfo->ucLen = 20;
    }
    else                                        //不是集抄
    {
        flag=0;
        guc_ShowEnergyFlag=0;
        ucoffset=(s_FrmInfo->uiID.byte[2] * 20) + (s_FrmInfo->uiID.byte[1] << 2);

        DataProcRead2Slice(LSDLID,ucMonth,ucoffset,4,(uint8*)&data1);
        if(s_FrmInfo->uiID.byte[2]==0)                      //组合
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=~data1+1;
                flag=1;
            }
        }
        else if(s_FrmInfo->uiID.byte[2]==0x02)     //反向电量
        {
            guc_ShowEnergyFlag=0x55;
        }
        
        data1/=10;
        data1 = Hex2BCD(data1);             //转换为BCD码
        //data1>>=4;                          //E2里面的是3为小数，通讯要求2为小数

        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)
        {
            data1&=~0x80000000;
        }

        s_FrmInfo->ucLen = 4;
                                                //拷贝到通讯缓存中去
        memcpy(s_FrmInfo->ucbuf, (uint8*)&data1, 4);
    }

    return Ret_OK;   
}
/*=========================================================================================\n
* @function_name: ReadEnyD0Block
* @function_file: DLT_645_2007.c
* @描述: 读取分费率数据块
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-26)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadEnyD0Block(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    int16 Eadd;
    uint32 *pEnyData;
    int32  data1;
    uint8 flag;
    uint8 i;
    

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    flag=0;
    pEnyData=(uint32*)gs_EnergyA.lCP;                    //获取电量数据结构首地址
    data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+s_FrmInfo->uiID.byte[1]);
    Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], s_FrmInfo->uiID.byte[1],ABCPhy,active);
    data1 += Eadd;
    if(s_FrmInfo->uiID.byte[2]==0)                      //组合
    {
        if(data1>0x80000000)
        {
            data1=-data1;
            flag=1;
        }
    }

    data1/=10;
    data1 = Hex2BCD(data1);             //转换为BCD码
    //data1 >>= 4;                        //小数位

    if(flag)
    {
        data1|=0x80000000;
    }
    else if(s_FrmInfo->uiID.byte[2]==0)
    {
        data1&=~0x80000000;
    }

    memcpy(s_FrmInfo->ucbuf,(uint8*)&data1,4);

    for(i=0;i<12;i++)
    {
        flag=0;
        guc_ShowEnergyFlag=0x00;
        DataProcRead2Slice(LSDLID,i,CTable->pAddr+s_FrmInfo->uiID.byte[1]*4,4,(uint8*)&data1);
        if(s_FrmInfo->uiID.byte[2]==0)                      //组合
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=~data1+1;
                flag=1;
            }
        }
        data1/=10;
        data1 = Hex2BCD(data1);             //转换为BCD码
        //data1>>=4;                          //E2里面的是3为小数，通讯要求2为小数

        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)
        {
            data1&=~0x80000000;
        }

        memcpy(s_FrmInfo->ucbuf+4+i*4,(uint8*)&data1,4);
    }

    s_FrmInfo->ucLen=52;
    return Ret_OK;
}



typedef struct
{
    uint8 ucFormat;
    uint16 ucParaRefFlg; 
}GS_PARAFORMAT;

const GS_PARAFORMAT code gs_ParaFormatTab[]=
{
    {InValid,       0,                  },//0
    {QhTmFm,        flgEtPara_Fee,      },//1两套时区表切换时间//费率刷新
    {QhCnFm,        flgEtPara_Fee,      },//2年时区数p≤14
    {SdBCnFm,       flgEtPara_Fee,      },//3日时段表数q≤8
    {SqCntFm,       flgEtPara_Fee,      },//4日时段数(每日切换数) m≤14
    {FeeCnFm,       flgEtPara_Fee,      },//5费率数k≤63
    {HldCnFm,       flgEtPara_Fee,      },//6公共假日数n≤254
    {AtSwCnFm,      flgEtPara_Show,     },//7自动循环显示屏数
    {AtSwTmFm,      flgEtPara_Show,     },//8每屏显示时间
    {EngDocFm,      flgEtPara_Show,     },//9显示电能小数位数
    {PowDocFm,      flgEtPara_Show,     },//10显示功率(最大需量)小数位数
    {KeySwCnFm,     flgEtPara_Show,     },//11按键循环显示屏数
    {MeterIdFm,     0,                  },//12表号
    {EPCalcFm,      flgEtPara_Config,   },//13有功组合方式字
    {IrBdFm,        0,                  },//14调制型红外光口通信速率特征字
    {RS485Fm,       0,                  },//15通信口1通信速率特征字
    {ZxrWdFm,       flgEtPara_Fee,      },//16周休日特征字
    {ZxrSdFm,       flgEtPara_Fee,      },//17周休日采用的日时段表号
    {JsrFm,         0,                  },//18每月第1结算日
    {ZdDjTmFm,      0,                  },//19整点冻结时间间隔
    {RDjTmFm,       0,                  },//20日冻结时间
    {TzTmFm,        0,                  },//21跳闸延时时间格式
    {GgJRFm,        0,                  },//22公共假日格式
    {DjMdFm,        0,                  },//23冻结格式除整点冻结外
    {ZdDjMdFm,      0,                  },//24整点冻结格式
    {ZdDjStTmFm,    0,                  },//25整点冻结起始时间
    {MeterAdsFm,    0,                  },//26通信地址
    {MeterRptFm,    flgEtPara_Config    },//27电表运行特征字，主动上报模式字
    {PowerFullTm,   0,                  },//28上电全屏时间
    {LoadCurveFm,   flgEtPara_LoadCurve },//29负荷记录模式字
    {PdirFm,        flgEtPara_PowDir    },//30功率反向事件
    {MeterModeFm,   flgEtPara_Config    },//31电表运行状态特征字
//    {EsamStatus,    flgEtpara_EsamStatus},//32密钥总条数
};



/*=========================================================================================\n
* @function_name: ComRWE2Para
* @function_file: DLT_645_2007.c
* @描述: 读写E2参数
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ComRWE2Para(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    
    Word32 temp;

    //Word16 W16_JSR;

    //如果是厂内模式可写的话，设置时不在厂内，则返回错误
    if((CTable->ucAtb&Data_Factor)
       &&(guc_FactoryType != FactorMd)
       &&(s_FrmInfo->ucCmd == 0x14)
       ||((CTable->ucAtb & Data_WO) && (s_FrmInfo->ucCmd == 0x11)))//判断权限
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;  
    }

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }else if(((CTable->ucAtb & Data_RO) && (s_FrmInfo->ucCmd == 0x14)))
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                                                  //返回错误代码
    }

    temp.lword=0;

    if(s_FrmInfo->ucCmd == 0x11)                //如果是读数据的话
    {
        SysE2ReadData(CTable->pAddr,s_FrmInfo->ucbuf,CTable->ucLen);
        if(CTable->ucAtb & Data_BCD)            //如果需要转换为BCD的话
        {
            if(CTable->ucLen==1)
            {
                s_FrmInfo->ucbuf[0] = APIByteHEX2BCD(s_FrmInfo->ucbuf[0]);
            }else if(CTable->ucLen==2)
            {
                temp.word[0]=Uint8_To_Uint16(s_FrmInfo->ucbuf);
                temp.word[0]=IntHex2BCD(temp.word[0]);
                memcpy(s_FrmInfo->ucbuf,temp.byte,2);
            }
            else if(CTable->ucLen==4)
            {
                temp.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
                temp.lword=Hex2BCD(temp.lword);
                memcpy(s_FrmInfo->ucbuf,temp.byte,4);
            }
        }
        s_FrmInfo->ucLen = CTable->ucLen;       //回复的数据长度
    }else
    {
        //检测参数的格式是否正确
        s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,gs_ParaFormatTab[CTable->uiOther].ucFormat,CTable->ucLen);
        if(s_FrmInfo->ucErr.word==0)
        {
            if(CTable->ucAtb & Data_BCD)            //如果需要转换为BCD的话
            {
                if(CTable->ucLen==1)
                {
                    s_FrmInfo->ucbuf[0] = APIByteBCD2HEX(s_FrmInfo->ucbuf[0]);
                }else if(CTable->ucLen==2)
                {
                    temp.word[0]=Uint8_To_Uint16(s_FrmInfo->ucbuf);
                    temp.word[0]=IntBCD2Hex(temp.word[0]);
                    memcpy(s_FrmInfo->ucbuf,temp.byte,2);
                }
                else if(CTable->ucLen==4)
                {
                    temp.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
                    temp.lword=BCD2Hex(temp.lword);
                    memcpy(s_FrmInfo->ucbuf,temp.byte,4);
                }
            }

//          if((s_FrmInfo->uiID.lword&0xffffff00)==0x04000C00)      //修改密码
//          {
//              if(guc_FactoryType!=FactorMd)                           //不在厂内模式
//              {
//                  if(s_FrmInfo->uiID.byte[0]==0x03)                   //02级
//                  {
//                      if(s_FrmInfo->ucLevel!=0x02)                    //02级密码需要02级权限去修改
//                      {
//                          s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
//                          s_FrmInfo->ucLen = 1;
//                          return Ret_OK;
//                      }
//                  }
//              }
//              SysE2ParaSetManage(CTable->pAddr+3*(s_FrmInfo->uiID.byte[0]-1),s_FrmInfo->ucbuf+1, CTable->ucLen);
//          }else
            {
            #ifdef JSRPRLOG
                if((s_FrmInfo->uiID.lword&0xffffff00)==0x04000B00)          //结算日编程记录
                {
                    ProJL(s_FrmInfo->uiUserCode.lword,FLG_JSRJL);           //记录编程前结算日时间
                    if(s_FrmInfo->uiID.byte[0]==0x01)
                    {
                        BE_ReadP(EEP_CBR1,W16_JSR.byte,2); 
                        if(Uint8_To_Uint16(s_FrmInfo->ucbuf)!=W16_JSR.word)
                        {
                            gui_RefreshEvent|=flgEtPara_FrsJsr;                 //第一结算日参数刷新
                        }
                    }
                }
                else 
            #endif

            #ifdef ZXRPRLOG
                if(s_FrmInfo->uiID.lword==0x04000802)                  //周休日编程记录
                {
                    ProJL(s_FrmInfo->uiUserCode.lword,FLG_ZXRJL);           //记录编程前周休日时段表号
                }
                else 
            #endif

            #ifdef YGZHZPRLOG
                if(s_FrmInfo->uiID.lword==0x04000601)                  //有功组合方式编程记录
                {
                    ProJL(s_FrmInfo->uiUserCode.lword,FLG_YGZHJL);          //记录编程前
                }
            #else
                {
                }
            #endif
                //首先要判读该参数在E2的页首地址，然后读取该页数据，计算该页数据CRC，写回E2中
                SysE2ParaSetManage(CTable->pAddr,s_FrmInfo->ucbuf, CTable->ucLen);
                if(s_FrmInfo->uiID.lword==0x04000A01)                  //写负荷记录起始时间
                {
                    temp.lword=0;                                      //判断是否全0
//                    if(ApiBufCmp(s_FrmInfo->ucbuf, (uint8*)&temp.lword, 4) == CMP_EQU)
//                    {
//                        temp.byte[0] = 0xAA;
//                    }
                    temp.byte[0] = gs_DateTime.ucYear;
                    SysE2ParaSetManage(EEP_FHJLFLAG, &temp.byte[0], 1);
                }
            }
            //需要刷新参数的这里刷新
            if(CTable->uiOther==15)           //波特率修改
            {
                guc_BodeDely=40;
            }else if(CTable->ucAtb & Data_RF)           
            {
                gui_RefreshEvent|=gs_ParaFormatTab[CTable->uiOther].ucParaRefFlg;
            } 
            s_FrmInfo->ucLen = 0;                   //写成功
        }else
        {
            s_FrmInfo->ucLen = 1;                   //写成功
        }

    }

    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: WrPassWord
* @function_file: DLT_645_2007.c
* @描述: 普通密码写
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 WrPassWord(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    if(s_FrmInfo->ucCmd == 0x11)                    //判断权限
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                              //返回错误代码
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    //检测参数的格式是否正确
    //s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,gs_ParaFormatTab[CTable->uiOther].ucFormat,CTable->ucLen);
    if(JudgeIsNum(s_FrmInfo->ucbuf+1,3)==Const_Data_OK)
    {
        if(s_FrmInfo->ucLevel == 0x02
           && ((s_FrmInfo->uiID.byte[0] == 0x03 && s_FrmInfo->ucbuf[0]==0x02)
           || (s_FrmInfo->uiID.byte[0] == 0x05 && s_FrmInfo->ucbuf[0]==0x04)))
        {
            SysE2ParaSetManage(CTable->pAddr+(s_FrmInfo->uiID.byte[0]-1)*3,s_FrmInfo->ucbuf+1, 3);
            s_FrmInfo->ucLen = 0;                   //写成功
        }
        else if(s_FrmInfo->ucLevel == 0x04
                && s_FrmInfo->uiID.byte[0] == 0x05
                && s_FrmInfo->ucbuf[0]==0x04)
        {
            SysE2ParaSetManage(CTable->pAddr+(s_FrmInfo->uiID.byte[0]-1)*3,s_FrmInfo->ucbuf+1, 3);
            s_FrmInfo->ucLen = 0;                   //写成功
        }
        else
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
            s_FrmInfo->ucLen = 1;                   //写成功
        }
    }
    else
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;                   //写成功
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ComMeterConst
* @function_file: DLT_645_2007.c
* @描述: 读取电表常数
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ComMeterConst(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{

    Word32 temp;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    temp.lword=0;
    SysE2ReadData(CTable->pAddr,s_FrmInfo->ucbuf,2);
    //2个字节变成3个字节的BCD
    temp.word[0]=Uint8_To_Uint16(s_FrmInfo->ucbuf);
    temp.lword=Hex2BCD(temp.lword);
    //temp.word[0]=IntHex2BCD(temp.word[0]);
    memcpy(s_FrmInfo->ucbuf,temp.byte,3);   //变成3个字节的BCD
    s_FrmInfo->ucLen = CTable->ucLen;       //回复的数据长度
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ComRdEvCnt
* @function_file: DLT_645_2007.c
* @描述: 读取事件记录次数
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ComRdEvCnt(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{

    Word32 W32Count;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }
    
    W32Count.lword=0;
    
    BE_ReadP(CTable->pAddr,(uint8*)W32Count.byte,3);            //读取冻结模式字
   
    W32Count.lword=Hex2BCD(W32Count.lword);                     //HEX转BCD   
                                                                 
    //事件记录都是3个字节

    memcpy(s_FrmInfo->ucbuf,W32Count.byte,3);   //变成3个字节的BCD

    s_FrmInfo->ucLen = CTable->ucLen;           //回复的数据长度
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RwGGJR
* @function_file: DLT_645_2007.c
* @描述: 读写公共假日
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RwGGJR(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    //
    uint32 ulE2Addr;
    ulE2Addr=CTable->pAddr+(s_FrmInfo->uiID.byte[0]-1)*4;

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->ucCmd == 0x11)                            //如果是读数据的话
    {
        BE_ReadP(ulE2Addr,s_FrmInfo->ucbuf,4);            //读取公共假日 
        s_FrmInfo->ucLen=4;
        
    }else
    {
        s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,GgJRFm,4);
        if(s_FrmInfo->ucErr.word==0)
        {
            BE_WriteP(ulE2Addr,s_FrmInfo->ucbuf,4);
            s_FrmInfo->ucLen=0;
            gui_RefreshEvent |= flgEtPara_Fee;  //发送刷新费率参数
        }else
        {
            s_FrmInfo->ucLen=1;
        }
        
    }
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: FunToBeDef
* @function_file: DLT_645_2007.c
* @描述: 空函数
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 FunToBeDef(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: ReadJL
* @function_file: DLT_645_2007.c
* @描述: 读取事件记录
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadJL(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint32 ulEvCnt;
    uint8  ucAddLen;

    if(s_FrmInfo->ucCmd == 0x14)                            //如果是写数据的话
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                                      //不支持写，返回错误代码
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ulEvCnt=0;
    ucAddLen=0;
    //读取者事件次数
    BE_ReadP(CTable->uiOther,(uint8*)&ulEvCnt,3);
    //读取事件数据
    DataProcRead2Slice(CTable->pAddr,s_FrmInfo->uiID.byte[0] - 1,0,CTable->ucLen,XDATA_RAMZONE);
 
    //电表清零记录需要后面添加88个字节
    //事件清零不需要添加
    //校时记录不需要添加
    //编程记录不需要添加
    //掉电记录不需要添加
    //开表盖次数中间需要添加6+4+4+4后16个字节+4+4，后面需要添加16个字节
    if((s_FrmInfo->uiID.lword&0xFFFFFF00)==0x03300100)            //电表清零记录
    {
        memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,CTable->ucLen);   //读取记录
        if(ulEvCnt>=s_FrmInfo->uiID.byte[0])                    //如果事件发生次数大于读取的数据的次数，默认值天0XFF,不然填0x00
        {
            memset(s_FrmInfo->ucbuf+CTable->ucLen,0xff,88);   
        }else
        {
            memset(s_FrmInfo->ucbuf+CTable->ucLen,0x00,88);
        }
        ucAddLen=88;
    }else if((s_FrmInfo->uiID.lword&0xFFFFFF00)==0x03300D00)        //开表盖记录
    {
        memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,20);              //读取记录
        if(ulEvCnt>=s_FrmInfo->uiID.byte[0])                    //如果事件发生次数大于读取的数据的次数，默认值天0XFF,不然填0x00
        {
            memset(s_FrmInfo->ucbuf+20,0xff,16);   
        }else
        {
            memset(s_FrmInfo->ucbuf+20,0x00,16);
        }

        memcpy(s_FrmInfo->ucbuf+36,XDATA_RAMZONE+20,8);     //读取记录

        if(ulEvCnt>=s_FrmInfo->uiID.byte[0])                    //如果事件发生次数大于读取的数据的次数，默认值天0XFF,不然填0x00
        {
            memset(s_FrmInfo->ucbuf+44,0xff,16);   
        }else
        {
            memset(s_FrmInfo->ucbuf+44,0x00,16);
        }

        ucAddLen=32;
 
//  #ifdef FRCom
//      if(s_FrmInfo->ucPort==ComIndex_Uart1)
//      {
//          if(gs_FREventOut.FREventOutStruct.PDirFR==false)        //没有反向
//          {
//              if(gs_FREventOut.FREventOutStruct.TimeFR==false)    //时钟正确
//              {
//                  EVENTOUTLOW();
//              }
//          }
//          gs_FREventOut.FREventOutStruct.OpenGapFRFlag=false;
//      }
//  #endif
    }else
    {

        memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,CTable->ucLen);   //读取记录

        ucAddLen=0;
    }
    s_FrmInfo->ucLen = CTable->ucLen+ucAddLen;                       //回复的数据长度
    return Ret_OK;
}

typedef struct
{
    uint16  uiDjModeAdrr;      //冻结模式字地址
    uint16  uiDjCntAdrr;   //冻结次数地址
}S_RDDjDATA;


const S_RDDjDATA code DjDATATab[]=
{
    {EEP_DSFREEZ,EEP_DSDJHISNO},// 0 定时冻结
    {EEP_YDFREEZ,EEP_SSDJHISNO},// 1 瞬时冻结
    {EEP_SSFREEZ,EEP_SQDJHISNO},// 2 两套时区表切换冻结

    {EEP_SSFREEZ,EEP_RSDJHISNO},// 3 两套日时段表切换冻结
    {EEP_ZDFREEZ,EEP_ZDDJHISNO},// 4 整点冻结
    {EEP_RFREEZ,EEP_RDJHISNO},//  5 日冻结
};
/*=========================================================================================\n
* @function_name: RwDjData
* @function_file: DLT_645_2007.c
* @描述: 冻结数据
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RwDjData(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint8 ucDjMode;
    Word32 data1;
    uint8 ucpos;
    uint32 ulDjCnt;

    if(s_FrmInfo->ucCmd == 0x14)                            //如果是写数据的话
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                                      //不支持写，返回错误代码
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ulDjCnt=0;

    //读取者事件次数
    BE_ReadP(DjDATATab[CTable->uiOther].uiDjCntAdrr,(uint8*)&ulDjCnt,3);

    BE_ReadP(DjDATATab[CTable->uiOther].uiDjModeAdrr,&ucDjMode,1);              //读取冻结模式字
    //读取冻结数据
    DataProcRead2Slice(CTable->pAddr,s_FrmInfo->uiID.byte[0] - 1,0,CTable->ucLen,XDATA_RAMZONE);
 

    if(s_FrmInfo->uiID.byte[2] == 0x04)         //读取整点冻结数据比较特殊
    {
        if(s_FrmInfo->uiID.byte[1] == 0x00)     //如果是读取冻结时间的话
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);
            s_FrmInfo->ucLen = 5;
        }
        else if(s_FrmInfo->uiID.byte[1] == 0x01)        //正向总电量
        {
            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+5,4);
                s_FrmInfo->ucLen = 4;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0x02)        //反向总电量
        {
            if(ucDjMode&BIT1)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+9,4);
                s_FrmInfo->ucLen = 4;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0xFF)//集抄
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);       //时间
            memset(s_FrmInfo->ucbuf+5,0xAA,1);              //AA
            ucpos=6;

            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+5,4);    //正向电量
                memset(s_FrmInfo->ucbuf+ucpos+4,0xAA,1);
                ucpos+=5;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }
  

            if(ucDjMode&BIT1)                                       //反向电量
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+9,20);
                memset(s_FrmInfo->ucbuf+4+ucpos,0xAA,1);
                ucpos+=5;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }
            s_FrmInfo->ucLen = ucpos;
        }
    }
    else                                        //如果不是读取整点冻结数据的话
    {
        if(s_FrmInfo->uiID.byte[1] == 0x00)     //如果是读取冻结时间的话
        {                                       //读取数据
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);
            s_FrmInfo->ucLen = 5;
        }else if(s_FrmInfo->uiID.byte[1] == 0x01)   //正向电量
        {
            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+5,20);
                s_FrmInfo->ucLen = 20;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }

        }
        else if(s_FrmInfo->uiID.byte[1] == 0x02)    //反向电量
        {
            if(ucDjMode&BIT1)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+25,20);
                s_FrmInfo->ucLen = 20;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0x10)                //功率
        {
            if(ucDjMode&BIT7)
            {
                data1.lword = Uint8_To_Uint32(XDATA_RAMZONE+45);
                memcpy(s_FrmInfo->ucbuf,data1.byte,3);          //总功率
                memcpy(s_FrmInfo->ucbuf+3,data1.byte,3);        //A项功率

                if(ulDjCnt>=s_FrmInfo->uiID.byte[0])            //如果发生次数大于读取的数据的次数，默认值填0XFF,不然填0x00
                {
                    //这里根据次数判断是否需要填写0xff，还是0x00
                    memset(s_FrmInfo->ucbuf+6,0xff,18);
                }else
                {
                    memset(s_FrmInfo->ucbuf+6,0x00,18);
                }
                s_FrmInfo->ucLen = 24;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0xFF)
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);       //时间
            memset(s_FrmInfo->ucbuf+5,0xAA,1);              //AA
            ucpos=6;

            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+5,20);    //正向电量
                memset(s_FrmInfo->ucbuf+ucpos+20,0xAA,1);
                ucpos+=21;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }
  

            if(ucDjMode&BIT1)                                       //反向电量
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+25,20);
                memset(s_FrmInfo->ucbuf+20+ucpos,0xAA,9);
                ucpos+=29;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,9);
                ucpos+=9;
            }


            if(ucDjMode&BIT7)
            {
                data1.lword = Uint8_To_Uint32(XDATA_RAMZONE+45);
                memcpy(s_FrmInfo->ucbuf+ucpos,data1.byte,3);          //总功率
                memcpy(s_FrmInfo->ucbuf+ucpos+3,data1.byte,3);        //A项功率

                if(ulDjCnt>=s_FrmInfo->uiID.byte[0])            //如果发生次数大于读取的数据的次数，默认值填0XFF,不然填0x00
                {
                    //这里根据次数判断是否需要填写0xff，还是0x00
                    memset(s_FrmInfo->ucbuf+ucpos+6,0xff,18);
                }else
                {
                    memset(s_FrmInfo->ucbuf+ucpos+6,0x00,18);
                }

                memset(s_FrmInfo->ucbuf+ucpos+24,0xAA,1);
                ucpos+=25;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }

            s_FrmInfo->ucLen = ucpos;
        }
    }
    return Ret_OK;
}

 
/*=========================================================================================\n
* @function_name: RwLcdData
* @function_file: DLT_645_2007.c
* @描述: 设置显示项
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RwLcdData(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint16 addr;
    Word32 ulLcdid;
    uint8 uctemp;

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if((s_FrmInfo->uiID.byte[1]==0x01 && s_FrmInfo->uiID.byte[0]>gs_DispCont.ucLxCnt)    //循显  
      || (s_FrmInfo->uiID.byte[1]==0x02 && s_FrmInfo->uiID.byte[0]>gs_DispCont.ucJxCnt))   //键显
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    addr = CTable->pAddr + (s_FrmInfo->uiID.byte[0] - 1) * 5;   //获取屏号的地址
    if(s_FrmInfo->ucCmd == 0x11)                                //如果是读数据的话
    {
        BE_ReadP(addr, s_FrmInfo->ucbuf, 5);                    //读取数据
        s_FrmInfo->ucLen =5;
    }
    else                                                        //否则就是写数据
    {
        if(s_FrmInfo->ucLevel==0x98)
        {
            uctemp=s_FrmInfo->ucbuf[0];
            memcpy(s_FrmInfo->ucbuf,s_FrmInfo->ucbuf+1,4);
            s_FrmInfo->ucbuf[4]=uctemp;
        }
        
        ulLcdid.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
        if(SearchDispNO(ulLcdid)!=0xff)
        {
            BE_WriteP(addr, s_FrmInfo->ucbuf, 5);                   //写入数据标识
            s_FrmInfo->ucLen = 0;
        }else
        {
            s_FrmInfo->ucErr.word = ComErrBit_Other;
            s_FrmInfo->ucLen = 1;
        }
    }
    return Ret_OK;
}
 
/*=========================================================================================\n
* @function_name: RwSysTime
* @function_file: DLT_645_2007.c
* @描述: 读写系统时间
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RwSysTime(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->ucCmd == 0x11)                        //读数据
    {
        if(s_FrmInfo->uiID.lword == 0x04000101)         //日期和星期
        {
            GetSysClock(s_FrmInfo->ucbuf, Const_YYMMDDWW);
            s_FrmInfo->ucLen = CTable->ucLen;
        }
        else if(s_FrmInfo->uiID.lword == 0x04000102)        //时间
        {
            GetSysClock(s_FrmInfo->ucbuf, Const_hhmmss);
            s_FrmInfo->ucLen = CTable->ucLen;
        }
        else if(s_FrmInfo->uiID.lword == 0x0400010C)         //日期+时间
        {
            GetSysClock(s_FrmInfo->ucbuf, Const_hhmmssYYMMDDWW);
            s_FrmInfo->ucLen = CTable->ucLen;
        }
        else
        {
            s_FrmInfo->ucLen = 1;
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;
        }
//  #ifdef FRCom
//      if(s_FrmInfo->ucPort==ComIndex_Uart1)
//      {
//          if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//          {
//              if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//              {
//                      EVENTOUTLOW();
//              }
//          }
//          gs_FREventOut.FREventOutStruct.TimeFR=0;
//      }
//  #endif
    }
    else                                                        //写数据
    {
        S_TIMEDATA time;
        if(s_FrmInfo->uiID.lword == 0x04000101)                 //日期和星期
        {
            if(Ret_OK == ValidTime(s_FrmInfo->ucbuf+1, 0)       //判断时间的合法性
               && s_FrmInfo->ucbuf[0]<=6)    //增加星期判断  
            {
                memcpy(&time.ucSecond,&gs_DateTime.ucSecond,7);     //拷贝校时前的时钟
                s_FrmInfo->ucLen = 0;
                SetSysClock(s_FrmInfo->ucbuf, Const_YYMMDDWW);  //校正时钟
               // Com_JSJL(s_FrmInfo->uiUserCode.lword, &time);   //传入校时前的时间
                gui_RefreshEvent|= flgEtPara_Fee;               //刷新费率参数
           
//         #ifdef FRCom
//              if(s_FrmInfo->ucPort==ComIndex_Uart1)
//              {
//                  if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//                  {
//                      if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//                      {
//                              EVENTOUTLOW();
//                      }
//                  }
//                  gs_FREventOut.FREventOutStruct.TimeFR=0;
//              }
//          #endif
                     
            
            
            }
            else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
            }
            
        }
        else if(s_FrmInfo->uiID.lword == 0x04000102)            //时间
        {
            if(Ret_OK == ValidTime(s_FrmInfo->ucbuf, 1))        //判断时间的合法性
            {

                memcpy(&time.ucSecond,&gs_DateTime.ucSecond,7);     //拷贝校时前的时钟
                s_FrmInfo->ucLen = 0;
                SetSysClock(s_FrmInfo->ucbuf, Const_hhmmss);
               // Com_JSJL(s_FrmInfo->uiUserCode.lword, &time);
                gui_RefreshEvent|= flgEtPara_Fee;               //刷新费率参数
               
//         #ifdef FRCom
//              if(s_FrmInfo->ucPort==ComIndex_Uart1)
//              {
//                  if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//                  {
//                      if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//                      {
//                              EVENTOUTLOW();
//                      }
//                  }
//                  gs_FREventOut.FREventOutStruct.TimeFR=0;
//              }
//          #endif
                
                
            }
            else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.lword == 0x0400010C)            //日期+时间
        {
//            if(s_FrmInfo->ucLen!=19)
//            {
//                s_FrmInfo->ucErr.word = ComErrBit_Other;
//                s_FrmInfo->ucLen = 1;
//                return Ret_OK;
//            }
            
            if(Ret_OK == ValidTime(s_FrmInfo->ucbuf, 3) 
               && s_FrmInfo->ucbuf[3]<=6)      //判断时间的合法性
            {
                memcpy(&time.ucSecond,&gs_DateTime.ucSecond,7);     //拷贝校时前的时钟
                SetSysClock(s_FrmInfo->ucbuf, Const_hhmmssYYMMDDWW);
               // Com_JSJL(s_FrmInfo->uiUserCode.lword, &time);
                gui_RefreshEvent|= flgEtPara_Fee;               //刷新费率参数
                s_FrmInfo->ucLen = 0;
            
//          #ifdef FRCom
//              if(s_FrmInfo->ucPort==ComIndex_Uart1)
//              {
//                  if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//                  {
//                      if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//                      {
//                              EVENTOUTLOW();
//                      }
//                  }
//                  gs_FREventOut.FREventOutStruct.TimeFR=0;
//              }
//          #endif
            }else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
            }
        }
        else
        {
            s_FrmInfo->ucLen = 1;
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;
        }
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RwRamE2
* @function_file: DLT_645_2007.c
* @描述: 直接读写E2
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RwRamE2(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint8 len;
    Word16 addr;

    if(s_FrmInfo->ucCmd == 0x11)                //读数据
    {
        addr.byte[0] = s_FrmInfo->ucbuf[0];     //读写的地址
        addr.byte[1] = s_FrmInfo->ucbuf[1];
        len = s_FrmInfo->ucbuf[3];              //读写的长度
        if(len > 0x50)                          //长度大于0x40是错误的
        {
            return Ret_Err;
        }
        BE_ReadP(addr.word, s_FrmInfo->ucbuf, len);
        s_FrmInfo->ucLen = len;
    }
    else                                        //否则就是写数据
    {
        addr.byte[0] = s_FrmInfo->ucbuf[0];     //读写的地址
        addr.byte[1] = s_FrmInfo->ucbuf[1];
        len = s_FrmInfo->ucbuf[3];              //读写的长度
        if(len > 0x50)                          //长度大于0x40是错误的
        {
            return Ret_Err;
        }
 
        BE_WriteP(addr.word, s_FrmInfo->ucbuf + 4, len);
        s_FrmInfo->ucLen = 0;
    }
    return Ret_OK;
}

typedef struct
{
    uint32 ulAdrr;
    uint8  phy;
    uint8  Atb;
    uint8  ucLen;
}Gs_VarType;


const Gs_VarType code Gs_RdVarTab[]=
{
    {UAVG,      0x04,   0x01,       0x02,   },      //0 电压数据块
    {UAVG,      0x00,   0x01,       0x02,   },      //1 A电压
    {UAVG,      0x01,   0x01,       0x02,   },      //2 B电压
    {UAVG,      0x02,   0x01,       0x02,   },      //3 C电压
     
    {IAAVG,     0x04,   0x01,       0x03,   },      //4 电流数据块
    {IAAVG,     0x00,   0x01,       0x03,   },      //5 A电流
    {IAAVG,     0x01,   0x01,       0x03,   },      //6 B电流
    {IAAVG,     0x02,   0x01,       0x03,   },      //7 A电流

    {PAAVG,     0x04,   0x01,       0x03,   },      //8  有功功率数据块
    {PAAVG,     0x00,   0x01,       0x03,   },      //9 有功A功率
    {PAAVG,     0x01,   0x01,       0x03,   },      //10 有功B功率
    {PAAVG,     0x02,   0x01,       0x03,   },      //11 有功C功率


    {DATAFREQ,  APhy,   0x02,       0x02,   },      //12 频率
    {0,         0x04,   0x00,       0x02,   },      //13 功率因数总
    {0,         0x00,   0x00,       0x02,   },      //14 功率因数A 
    {0,         0x01,   0x00,       0x02,   },      //15 功率因数B
    {0,         0x02,   0x00,       0x02,   },      //16 功率因数C

    {DATAQ,     0x04,   0x01,       0x03,   },      //17 无功功率数据块
    {DATAQ,     0x00,   0x01,       0x03,   },      //18 无功A功率
    {DATAQ,     0x01,   0x01,       0x03,   },      //19 无功B功率
    {DATAQ,     0x02,   0x01,       0x03,   },      //20 无功C功率

    {DATAFREQ,  BPhy,   0x02,       0x02,   },      //21 B频率
    {DATAFREQ,  CPhy,   0x02,       0x02,   },      //22 C频率
    
    {0,         0x00,   0x00,       0x02,   },      //23 相角A 
    {0,         0x01,   0x00,       0x02,   },      //24 相角B
    {0,         0x02,   0x00,       0x02,   },      //25 相角C
    
    

};
/*=========================================================================================\n
* @function_name: ReadVar
* @function_file: DLT_645_2007.c
* @描述: 读取瞬时变量值
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadVar(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint32 ulVarValue;
    uint16 uiCosVal;
    uint32 bcd;
    uint8  ucpos;

    Gs_VarType code * pVar;
    if(s_FrmInfo->ucCmd == 0x14)                //如果是写的话
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ucpos=0;
    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //如果是集抄的话
    {
        ucpos=0;    
    }

   pVar = (Gs_VarType code*)&Gs_RdVarTab + CTable->uiOther+ucpos;
    if(pVar->Atb==0x01)
    {
        if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //如果是集抄的话
        {
            if(pVar->ulAdrr == UAVG)
            {
                for(uint8 i=0;i<3;i++)
                {
                    ulVarValue = CalRMS2(UAVG,gs_UI.ul_U[i]);
                    memcpy(s_FrmInfo->ucbuf+i*pVar->ucLen,(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*3;
                return Ret_OK;
            }
            else if(pVar->ulAdrr == IAAVG)
            {
                for(uint8 i=0;i<3;i++)
                {
                    ulVarValue = CalRMS2(IAAVG,gs_UI.ul_I[i]);
                    memcpy(s_FrmInfo->ucbuf+i*pVar->ucLen,(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*3;
                return Ret_OK;
            }
            else if (pVar->ulAdrr == PAAVG)
            {
                ulVarValue = CalRMS2(PAAVG,(gs_power.ul_power[0] + gs_power.ul_power[1] + gs_power.ul_power[2]));
                memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,pVar->ucLen);
                for(uint8 i=0;i<3;i++)
                {
                    ulVarValue = CalRMS2(PAAVG,gs_power.ul_power[i]);
                    memcpy(s_FrmInfo->ucbuf+pVar->ucLen*(i+1),(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*4;
                return Ret_OK;
            }
            else if(pVar->ulAdrr == DATAQ)
            {
                ulVarValue = CalRMS2(DATAQ,(gs_power.ul_Npower[0] + gs_power.ul_Npower[1] + gs_power.ul_Npower[2]));
                memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,pVar->ucLen);
                for(uint8 i=0;i<3;i++)
                {
                    ulVarValue = CalRMS2(DATAQ,((gs_power.ul_Npower[i])));
                    memcpy(s_FrmInfo->ucbuf+pVar->ucLen*(i+1),(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*4;
                return Ret_OK;
            }
        }
        else
        {
            if(pVar->ulAdrr == UAVG)
            {
                 ulVarValue = CalRMS2(UAVG,gs_UI.ul_U[pVar->phy]);
            }
            else if(pVar->ulAdrr == IAAVG)
            {
              if(CalRMS2(PAAVG,gs_power.ul_power[pVar->phy]) || CalRMS2(DATAQ,gs_power.ul_power[pVar->phy]))  //功率为零，电流为零
              {
                 ulVarValue = CalRMS2(IAAVG,gs_UI.ul_I[pVar->phy]);
              }else
              {
                ulVarValue = 0;
              }
               
            }
            else if (pVar->ulAdrr == PAAVG)
            {
                switch(pVar->phy)
                {
                    case 0x00:
                    case 0x01:
                    case 0x02:
                        ulVarValue = CalRMS2(PAAVG,gs_power.ul_power[pVar->phy]);
                        break;
                    case 0x04:
                        ulVarValue = CalRMS2(PAAVG,(gs_power.ul_power[0] + gs_power.ul_power[1] + gs_power.ul_power[2]));
                        break;
                    default:
                        break;
                }
            }
            else if(pVar->ulAdrr == DATAQ)
            {
                switch(pVar->phy)
                {
                    case 0x00:
                    case 0x01:
                    case 0x02:
                        ulVarValue = CalRMS2(DATAQ,(gs_power.ul_Npower[pVar->phy]));
                        break;
                    case 0x04:
                        ulVarValue = CalRMS2(DATAQ,(gs_power.ul_Npower[0] + gs_power.ul_Npower[1] + gs_power.ul_Npower[2]));
                        break;
                    default:
                        break;
                }
            }
            memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,pVar->ucLen);
        }
    }else 
    {
        if(CTable->uiOther>=13
           &&CTable->uiOther<=16)                      //功率因数
        {
            if(s_FrmInfo->ucAtb & Const_Atb_DataJC)    //集抄
            {
                for(uint8 i=0;i<4;i++)
                {
                    uiCosVal=CalCos(gs_power.ul_power, gs_UI.ul_U,gs_UI.ul_I,i);   
                    memcpy(s_FrmInfo->ucbuf+i*pVar->ucLen,(uint8*)&uiCosVal,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*4;
                return Ret_OK;
            }else                                       //单相
            {
                uiCosVal=CalCos(gs_power.ul_power, gs_UI.ul_U,gs_UI.ul_I,pVar->phy);           
                memcpy(s_FrmInfo->ucbuf,(uint8*)&uiCosVal,pVar->ucLen);
            }
        }
        else if(CTable->uiOther>=23
           &&CTable->uiOther<=25)
        {
             switch(pVar->phy)
                {
                     case 0x00:
                         bcd = 0;
                        break;
                    case 0x01:
                        bcd =Hex2BCD(gs_phytim.BPhytim);
                        //bcd =Hex2BCD(1200);
                        break;
                    case 0x02:
                        bcd = Hex2BCD(gs_phytim.CPhytim);
                       //bcd =Hex2BCD(2400);
                        break;
                }
            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);      //表内温度
        }
//        else if(CTable->uiOther==11)
//        {
//            bcd=Hex2BCD(gul_CellVoltage);
//            bcd>>=4;
//            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);     //电池电压 
//        }
        else if(CTable->uiOther==12) //频率
        {
            ulVarValue = CalRMS2(DATAFREQ,gs_UI.ul_Hz[0]); //A相
            memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,CTable->ucLen);
        }
        else if(CTable->uiOther==21)
        {
            
            ulVarValue = CalRMS2(DATAFREQ,gs_UI.ul_Hz[1]);
            memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,CTable->ucLen);     //B频率
        }
        else if(CTable->uiOther==22)
        {
            
            ulVarValue = CalRMS2(DATAFREQ,gs_UI.ul_Hz[2]);
            memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,CTable->ucLen);     //C频率
        }
//      else if(CTable->uiOther==13)
//      {
//            //身份认证剩余时间
//            if(gW32_EsamValidTm.lword!=0)
//            {
//                bcd=Hex2BCD(gW32_EsamValidTm.lword/60+1);
//            }else
//            {
//                bcd=0;
//            }
//            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);     //身份认证剩余时间
//        }
//        else if(CTable->uiOther==14)
//        {
//            //红外身份认证剩余时间
//            if(gs_EsamIrCmd.uiStatusDelay!=0)
//            {
//                bcd=Hex2BCD(gs_EsamIrCmd.uiStatusDelay/60+1);
//            }
//            else
//            {
//                bcd=0;
//            }
//            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);     //红外身份认证剩余时间
//        }
    }
    s_FrmInfo->ucLen=CTable->ucLen;
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RTHZjl
* @function_file: DLT_645_2007.c
* @描述: 读取跳合闸记录数据块
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-06)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RTHZjl(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    uint32 ulEvCnt;
    Word32 W32Count;

    if(s_FrmInfo->ucCmd == 0x14)                //如果是写的话
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    ulEvCnt=0;
    //读取者事件次数
    BE_ReadP(CTable->uiOther,(uint8*)&ulEvCnt,3);

    DataProcRead2Slice(CTable->pAddr,s_FrmInfo->uiID.byte[0] - 1,0,18,XDATA_RAMZONE);
    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)                 //如果是集抄的话
    {
        //读取事件数据
        memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,18);          //发生事件，操作者代码，正反总电量

        if(ulEvCnt>=s_FrmInfo->uiID.byte[0])                //发送次数大于读取次数，添加0xff,不然添加0x00
        {
            memset(s_FrmInfo->ucbuf+18,0xff,16);
        }else
        {
            memset(s_FrmInfo->ucbuf+18,0x00,16);
        }
        s_FrmInfo->ucLen = 34;
    }
    else
    {
        if(s_FrmInfo->uiID.byte[1]==0x00)               //读取次数
        {
            W32Count.lword=Hex2BCD(ulEvCnt);            //HEX转BCD   
            //事件记录都是3个字节
            memcpy(s_FrmInfo->ucbuf,W32Count.byte,3);   //变成3个字节的BCD
        }else if(s_FrmInfo->uiID.byte[1]==0x01)         //时间
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,6);   //发生时间

        }else if(s_FrmInfo->uiID.byte[1]==0x02)         //操作者代码
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+6,4); //操作者代码

        }else if(s_FrmInfo->uiID.byte[1]==0x03)         //正向电量
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+10,4);//正向电量
        }else if(s_FrmInfo->uiID.byte[1]==0x04)         //反向电量
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+14,4);//反向电量
        }
        s_FrmInfo->ucLen = CTable->ucLen;           //回复的数据长度
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: IrShowLcd
* @function_file: DLT_645_2007.c
* @描述: 红外指定LCD显示
* 
* 
* @参数: 
* @param:s_FrmInfo  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 IrShowLcd(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    Word32 ulLcdid;
    uint8 ucbuff[5];

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->ucCmd != 0x11)                                //如果是读数据的话
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else                                                        //否则就是写数据
    { 
        memset(ucbuff,0xff,5);
        if(ApiBufCmp(s_FrmInfo->ucbuf,ucbuff,5) == CMP_EQU)
        {
            gs_DispStt.ucMode = Const_DispMode_Full;
            gs_DispStt.ucTmr = 10;
        }//立即刷新
        else
        {
            ulLcdid.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
            if(SearchDispNO(ulLcdid)!=0xff && s_FrmInfo->ucbuf[4] < 2) 
            {
                if(ulLcdid.lword != 0x04000401 && ulLcdid.lword != 0x04000402     //非占2屏且NN不为0的情况
                    && ulLcdid.lword != 0x0400040E && s_FrmInfo->ucbuf[4] != 0)
                {
                    s_FrmInfo->ucErr.word = ComErrBit_Other;
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK; 
                }
                memcpy(guc_IrShowLcdId,s_FrmInfo->ucbuf,5);       //数据内容
                gs_DispStt.ucMode=Const_DispMode_IRDA;
                gs_DispStt.ucTmr = 10;
            }else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
                return Ret_OK; 
            }
        }
    }
                                                    //需要点亮背光,背光时间待定
    //OpenBeiGuang();
    guc_DyBgLed = 10;                               // 设置背光延时时间
    gs_DispStt.ucPwrOffRefresh |= Const_RefreshAll;
    ShowRefresh(); 
    s_FrmInfo->ucLen = 5;
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: ReadSwVersion
* @function_file: DLT_645_2007.c
* @描述: 读取软件版本号
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @返回: 
* @作者:   lwb (2012-06-24)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadSwVersion(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{

    if(s_FrmInfo->ucCmd == 0x14)                //如果是写的话
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->uiID.byte[1]==0x0A)
    {
        FCpyTMem(s_FrmInfo->ucbuf,gCuc_VerSoft,12);                  //软件版本号
        s_FrmInfo->ucErr.word =0;
        s_FrmInfo->ucLen=12;
    }
    else
    {
        s_FrmInfo->ucErr.word=ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
    }

    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ReadHisJSTime
* @function_file: DLT_645_2007.c
* @描述: 读取历史结算时间
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   xuqf (2013-1-5)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadHisJSTime(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{   
    uint8  uiPt;                       //指针
    if(s_FrmInfo->ucCmd == 0x14)        //指令写
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }
    
    BE_ReadP(EEP_JSTIME_PT, &uiPt,1);                                     //读取指针
    
    if(s_FrmInfo->uiID.byte[0] == 0xFF)                                   //集抄
    {
        BE_ReadP(EEP_JSTIME,XDATA_RAMZONE,48);
        
        for(uint8 i = 0;i < 12;i++)
        {
            memcpy(s_FrmInfo->ucbuf+i*4 , XDATA_RAMZONE+uiPt*4, 4);       //重组
            uiPt++;
            uiPt%=12;
        }
        s_FrmInfo->ucLen = 48;
    }else
    {
        DataProcRead2Slice(JSSJJLID,s_FrmInfo->uiID.byte[0]-1,0,4,XDATA_RAMZONE);
        memcpy(s_FrmInfo->ucbuf , XDATA_RAMZONE, 4);
        s_FrmInfo->ucLen = 4;
    }
    
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: CmpCode
* @function_file: DLT_645_2007.c
* @描述: 数据比对
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @返回: 
* @作者:   lm (2012-12-24)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 CmpCode(S_FRAMEINFO *s_FrmInfo)
{
//    uint8 ucChip;
//    uint8 ucKeyindex;
//    Word32 W32Factoraddr,W32Dataaddr;
//
////    if(s_FrmInfo->ucCmd == 0x14)
////    {
////        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
////        s_FrmInfo->ucLen = 1;
////        return Ret_OK;   
////    }
////    else if(s_FrmInfo->ucCmd == 0x12)
////    {
////        s_FrmInfo->ucErr.word = ComErrBit_Other;
////        s_FrmInfo->ucLen = 1;
////        return Ret_OK;   
////    }
//       
////    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);   //数据标识
////    memcpy(s_FrmInfo->uiUserCode.byte,s_FrmInfo->ucbuf+4,4);   //操作者代码
//    
////    if((s_FrmInfo->uiID.lword&0xffffff00)==0x07800200)
////    {
////        if(s_FrmInfo->uiID.byte[0]==0x01 || s_FrmInfo->uiID.byte[0]==0x02)
////        {
////            s_FrmInfo->ucbuf+=8;
////            s_FrmInfo->ucbuf[0] = 0;
////            s_FrmInfo->ucLen = 1;
////        }
////        else if(s_FrmInfo->uiID.byte[0]==0x03 || s_FrmInfo->uiID.byte[0]==0x04)
////        {
////            s_FrmInfo->ucbuf+=8;
////            memset(s_FrmInfo->ucbuf, 0, 4);
////            s_FrmInfo->ucLen = 4;
////        }
////        else if(s_FrmInfo->uiID.byte[0]==0xff)
////        {
////            ucChip = s_FrmInfo->ucbuf[8];
////            ucKeyindex = s_FrmInfo->ucbuf[9];
//            memcpy(W32Factoraddr.byte,s_FrmInfo->ucbuf+10,4);
//            memcpy(W32Dataaddr.byte,s_FrmInfo->ucbuf+14,4);
//            if(W32Factoraddr.lword>0x20000 || W32Dataaddr.lword>0x20000)
//            {
//                s_FrmInfo->ucErr.word=ComErrBit_Address;
//                s_FrmInfo->ucLen = 2; 
//                return Ret_OK;
//            }
////            else if(ucChip==0 && ucKeyindex==0)    //MCU为0，索引为0，长度128内， 数据起始地址20000内
////            {
//                ReadFlash(W32Factoraddr.lword,  XDATA_RAMZONE1, 16);
////                GetFactor(guc_CmpFactor, XDATA_RAMZONE1);
//                //ReadFlash(W32Factoraddr.lword+128,  XDATA_RAMZONE1+128, 128);
//                if(GetCmpData(guc_CmpFactor, XDATA_RAMZONE1,0)==Ret_OK)
//                {
//                    ReadFlash(W32Dataaddr.lword,  XDATA_RAMZONE1, 128);
//                    ReadFlash(W32Dataaddr.lword+128,  XDATA_RAMZONE1+128, 128);
//                    //s_FrmInfo->ucbuf+=8;
//                    GetCmpData(XDATA_RAMZONE, XDATA_RAMZONE1,1);                                        
//                    //s_FrmInfo->ucLen = 64;
//                }
//                else
//                {
//                    s_FrmInfo->ucErr.word=ComErrBit_Address;
//                    s_FrmInfo->ucLen = 2; 
//                }
//                
////            }
////            else
////            {
////                s_FrmInfo->ucErr.word=ComErrBit_Other;
////                s_FrmInfo->ucLen = 2; 
////            }            
////        }
////        else
////        {
////            s_FrmInfo->ucErr.word=ComErrBit_Other;
////            s_FrmInfo->ucLen = 2; 
////        }
////    }
////    else
////    {
////        s_FrmInfo->ucErr.word=ComErrBit_Other;
////        s_FrmInfo->ucLen = 2; 
////    }    
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: Report
* @function_file: DLT_645_2007.c
* @描述: 读取主动上报
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:    (2013-2-2)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 Report(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    
    if(s_FrmInfo->ucCmd == 0x11 || s_FrmInfo->ucCmd == 0x12)        //指令读
    {
        if(gs_MeterMode.ReportMode == 0 && s_FrmInfo->ucCmd == 0x12)
        {
            s_FrmInfo->ucErr.word=ComErrBit_Other;
            s_FrmInfo->ucLen = 1;  
            return Ret_OK;
        }
        else if(s_FrmInfo->ucCmd == 0x12)
        {
            s_FrmInfo->uiID.lword = 0x04001501;
            s_FrmInfo->ucbuf[10]= s_FrmInfo->ucbuf[0];            
            //s_FrmInfo->ucLen = 11;  
        }
        
        memcpy(s_FrmInfo->ucbuf, (uint8*)&gs_ReportState, sizeof(REPORTSTATE));       //
        
        if(gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt)
        {
            gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = false;
            gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = true;
        }
        if(gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt)
        {
            gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = false;
            gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = true;
        }
        if(gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt)
        {
            gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = false;
            gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = true;
        }
        if(gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt)
        {
            gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = false;
            gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = true;            
        }
        if(gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt)
        {
            gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = false;
            gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = true;
        } 
        if(gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt)
        {
            gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = false;
            gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = true;
        }  
        
        s_FrmInfo->ucLen = 10 + (s_FrmInfo->ucCmd - 0x11); 

    }else
    {
        s_FrmInfo->ucErr.word=ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
    }
    
    return Ret_OK;
}
#ifdef LOADCURVE
/*=========================================================================================\n
* @function_name: ReadCurveJl
* @function_file: DLT_645_2007.c
* @描述: 
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2013-02-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadCurveJl(S_FRAMEINFO *s_FrmInfo, S_CORETAB code * CTable)
{
    S_CURVECNT s_CurveCnt; 
    uint32 *ulCntPt;
    uint8 i;
    uint8 ucFrameNum;
    uint8 ucLen;

    if(s_FrmInfo->ucCmd == 0x14)        //指令写
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }


    if(s_FrmInfo->ucCmd == 0x12)        //后续帧
    {
        ucFrameNum=s_FrmInfo->ucbuf[0]; //保存帧序号
    }else
    {
        if(CTable->uiOther>=0x05)       //第5、6类负荷记录，没有返回无数据内容。
        {
            s_FrmInfo->ucLen = 0;
            return Ret_OK;   
        }
    }

    memset((uint8*)&s_CurveCnt,0x00,sizeof(S_CURVECNT));    //清零
    ulCntPt=&s_CurveCnt.ulTotalCnt;

    for(i=0;i<5;i++)
    {
        BE_ReadP(gs_CurveStoreTab[i].uiCntAddr,(uint8*)&ulCntPt[i],3);       //读取总次数
    }

    if(s_FrmInfo->ucCmd==0x11)                                               //首帧
    {
        if(s_FrmInfo->ucLen<0x05
           || JudgeIsNum(s_FrmInfo->ucbuf,1)==Const_Data_Err)
        {
            s_FrmInfo->ucErr.word = ComErrBit_Other;
            s_FrmInfo->ucLen = 1;
            return Ret_OK;        
        }

        gs_CurveFollowFrame.CurCnt=0;
        gs_CurveFollowFrame.SeqFrameEnd=false;
        if(APIByteBCD2HEX(s_FrmInfo->ucbuf[0])>ulCntPt[s_FrmInfo->uiID.byte[2]])
        {
            gs_CurveFollowFrame.TotalCnt=ulCntPt[s_FrmInfo->uiID.byte[2]];
        }else
        {
            gs_CurveFollowFrame.TotalCnt=APIByteBCD2HEX(s_FrmInfo->ucbuf[0]);
        }
        if(gs_CurveFollowFrame.TotalCnt == 0)
        {
            gs_CurveFollowFrame.SeqFrameEnd=true;
            s_FrmInfo->ucLen=0;
            return Ret_OK;
        }
        
    }else if(gs_CurveFollowFrame.SeqFrameEnd==true)
    {
            if(gs_MeterMode.ReportMode == 0)
            {
                s_FrmInfo->ucErr.word=ComErrBit_Other;
                s_FrmInfo->ucLen = 1;  
                return Ret_OK;
            }
            else
            {
                s_FrmInfo->uiID.lword = 0x04001501;
                memcpy(s_FrmInfo->ucbuf, (uint8*)&gs_ReportState, sizeof(REPORTSTATE));       //
                
                if(gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = true;
                }
                if(gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = true;
                }
                if(gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = true;
                }
                if(gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = true;            
                }
                if(gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt)
                {
                    gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = false;
                    gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = true;
                }
                if(gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt)
                {
                    gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = false;
                    gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = true;
                }  
                s_FrmInfo->ucbuf[10]= ucFrameNum;            
                s_FrmInfo->ucLen = 11;           
            }
            return Ret_OK;
    }
    
    if(s_FrmInfo->uiID.byte[2]==0x00)                                         //读取总负荷记录
    {
        if((guc_LoadCurveMode&0x0F)!=0x00)
        {
            if(s_CurveCnt.ulTotalCnt==0)
            {
                s_FrmInfo->ucLen=0;
                return Ret_OK;
            }
        }
        else
        {
            s_FrmInfo->ucLen=0;
            return Ret_OK;
        }
    }
    s_FrmInfo->ucLen=0;
    if(s_FrmInfo->uiID.byte[0]==0x00)           //最早的记录块
    {
        if(s_FrmInfo->uiID.byte[2]==0x00)       //读取总负荷记录
        {
            for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
            {
                ucLen=GetTotalOldestRecord(gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
                if((s_FrmInfo->ucLen+ucLen)>200)
                {
                    gs_CurveFollowFrame.SeqFrameEnd=false;
                    break;
                }
                s_FrmInfo->ucLen+=ucLen;
                gs_CurveFollowFrame.CurCnt++;
            }
        }else                                   //读取分负荷记录
        {
            if(guc_LoadCurveMode&BIT8MAP[(s_FrmInfo->uiID.byte[2]-1)])
            {
                for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
                {
                    ucLen=GetOneOldestRecord(s_FrmInfo->uiID.byte[2],gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
                    if((s_FrmInfo->ucLen+ucLen)>200)
                    {
                        gs_CurveFollowFrame.SeqFrameEnd=false;
                        break;
                    }
                    s_FrmInfo->ucLen+=ucLen;
                    gs_CurveFollowFrame.CurCnt++;
                }
            }else
            {
                s_FrmInfo->ucLen=0;
                return Ret_OK;
            }
        }
    }
    else if(s_FrmInfo->uiID.byte[0]==0x01)     //给定时间的记录块
    {
        if(s_FrmInfo->uiID.byte[2]==0x00)       //读取总负荷记录
        { 
            if(s_FrmInfo->ucCmd==0x11)
            {
                s_FrmInfo->ucLen=GetTotalTmRecord(s_FrmInfo->ucbuf+1,s_FrmInfo->ucbuf);
                if(s_FrmInfo->ucLen==0)
                {
                    s_FrmInfo->ucLen = 0;
                    gs_CurveFollowFrame.SeqFrameEnd=true;
                    return Ret_OK;
                }else if(gs_CurveFollowFrame.SeqFrameEnd==true)
                {
                    return Ret_OK;
                }
                gs_CurveFollowFrame.CurCnt++;
            }

            for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
            {
                ucLen=GetTotalTmSeqRecord(gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
                if(ucLen==0)
                {
                    if(gs_CurveFollowFrame.TotalCnt < s_CurveCnt.ulTotalCnt)
                    {
                        gs_CurveFollowFrame.TotalCnt++;
                    }
                }
                if((s_FrmInfo->ucLen+ucLen)>200)
                {
                    gs_CurveFollowFrame.SeqFrameEnd=false;
                    break;
                }
                s_FrmInfo->ucLen+=ucLen;
                if(gs_CurveFollowFrame.SeqFrameEnd==true)
                {
                    break;
                }
                gs_CurveFollowFrame.CurCnt++;
            }
        }else                                   //读取分负荷记录
        {
            if(guc_LoadCurveMode&BIT8MAP[(s_FrmInfo->uiID.byte[2]-1)])
            {
                if(s_FrmInfo->ucCmd==0x11)
                {    
                    s_FrmInfo->ucLen=GetOneTmRecord(s_FrmInfo->uiID.byte[2],s_FrmInfo->ucbuf+1,s_FrmInfo->ucbuf);
                    if(s_FrmInfo->ucLen==0)
                    {
                        s_FrmInfo->ucLen = 0;
                        gs_CurveFollowFrame.SeqFrameEnd=true;
                        return Ret_OK;
                    }
                    else if(gs_CurveFollowFrame.SeqFrameEnd==true)
                    {
                        return Ret_OK;
                    }
                    gs_CurveFollowFrame.CurCnt++;
                }

                for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
                {
                    ucLen=GetOneTmSeqRecord(s_FrmInfo->uiID.byte[2],gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
                    if((s_FrmInfo->ucLen+ucLen)>200)
                    {
                        gs_CurveFollowFrame.SeqFrameEnd=false;
                        break;
                    }
                    s_FrmInfo->ucLen+=ucLen;
                    if(gs_CurveFollowFrame.SeqFrameEnd==true)
                    {
                        break;
                    }
                    gs_CurveFollowFrame.CurCnt++;
                }

            }else
            {
                s_FrmInfo->ucLen=0;
                return Ret_OK;
            }
        }
    }
    else if(s_FrmInfo->uiID.byte[0]==0x02)     //最近一个记录块
    {
        if(s_FrmInfo->ucbuf[0]!=0x01)
        {
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }else
        {
    
            if(s_FrmInfo->uiID.byte[2]==0x00)       //读取总负荷记录
            {  
                s_FrmInfo->ucLen=GetTotalItemRecord(s_FrmInfo->ucbuf);
            }else                                   //读取分负荷记录
            {   
                if(guc_LoadCurveMode&BIT8MAP[(s_FrmInfo->uiID.byte[2]-1)])
                {
                    s_FrmInfo->ucLen=GetOneItemRecord(s_FrmInfo->uiID.byte[2],s_FrmInfo->ucbuf);
                }else
                {
                    s_FrmInfo->ucLen=0;
                    return Ret_OK;
                }
            }
        }
        gs_CurveFollowFrame.SeqFrameEnd=true;
    }

//  gs_CurveFollowFrame.CurCnt++;
    if(gs_CurveFollowFrame.CurCnt>=gs_CurveFollowFrame.TotalCnt)
    {
        if(s_FrmInfo->ucCmd==0x12)
        {
            s_FrmInfo->ucbuf[s_FrmInfo->ucLen++]=ucFrameNum;            
        }
        gs_CurveFollowFrame.SeqFrameEnd=true;
        return Ret_OK;
    }else
    {
        if(s_FrmInfo->ucCmd==0x12)
        {
            s_FrmInfo->ucbuf[s_FrmInfo->ucLen++]=ucFrameNum;
        }

        if(s_FrmInfo->ucLen!=0
           &&(gs_CurveFollowFrame.SeqFrameEnd==false))
        {
            s_FrmInfo->ucCmd|=0x20;
        }
        else if(gs_MeterMode.ReportMode == 1)
        {
            //最后一帧cmd=0x92;          
            if((gs_ReportState.ui_ReportState1.Word.word&0x0528) != 0 
               || (gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt != 0)  
               || (gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt != 0))//静止式多费率判断
            {
                s_FrmInfo->ucCmd|=0x20;
            }
        }
    }

    return Ret_OK;
}

#endif
