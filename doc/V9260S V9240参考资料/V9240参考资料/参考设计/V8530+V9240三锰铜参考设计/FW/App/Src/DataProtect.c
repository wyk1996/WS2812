#define DATAPROTECTEXT
#include "Include.h"

//uint8 guc_RTCSave;//zzp1203
uint8_t test_dim;

const S_E2PARACHK  gCs_E2ParaChkTab[] =
{//注意:建表需要参考E2参数排布对应的文档,下面两个秒对应量仅用于参考

    {E2_PROTECT_LEN,    0*E2_PROTECT_LEN},  //0x00
    {E2_PROTECT_LEN,    1*E2_PROTECT_LEN},  //0x01
    {E2_PROTECT_LEN,    2*E2_PROTECT_LEN},  //0x02
    {E2_PROTECT_LEN,    3*E2_PROTECT_LEN},  //0x03
//    {RTC_LEN,           4*E2_PROTECT_LEN},  //0x04
    {JBLEN,             5*E2_PROTECT_LEN},  //0x05 校表参数
//    {TIME_LEN,          6*E2_PROTECT_LEN},  //0x06
//    {E2_PROTECT_LEN,    7*E2_PROTECT_LEN},  //0x07 参数  
//    {LOAD_LEN,          8*E2_PROTECT_LEN},  //0x08 新增负荷记录参数 
// 
};
 
/*=========================================================================================\n
* @function_name: GetE2Type
* @function_file: DataProtect.c
* @描述: 根据地址获取页地址
* 
* 
* @参数: 
* @param:uiAddr  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 GetE2Type(uint16 uiAddr)
{//返回0xFF就表示一定读写大E2
    if(uiAddr >= dim(gCs_E2ParaChkTab)*E2_PROTECT_LEN)
    {
        return 0xFF;
    }
    return  (uint8)(uiAddr>>6);
}
/*=========================================================================================\n
* @function_name: DataSecurityInit
* @function_file: DataProtect.c
* @描述: 部分数据安全性相关的变量初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void DataSecurityInit(void)
{//部分数据安全性相关的变量初始化
    gul_E2ParaStt = 0x0FFFFFFF;         //复位后默认参数区大E2有效
    gui_RefreshEvent|=(flgEtPara_PowDir+flgEtPara_LoadCurve);

//    if(gs_EsamKeyUpDateCtrl.ucUpDateOk==0x55)           //密钥跟新没有完成，上电密钥补跟新
//    {
//        //设置密钥跟新
//        gui_RefreshEvent|=flgEtpara_EsamStatus;
//    }
}
/*=========================================================================================\n
* @function_name: E2ParaCheck
* @function_file: DataProtect.c
* @描述: 每秒调用一次,实际E2需要校验的周期是分级,而不是每秒校验,这里这样做只是为了将各个区的校验在不同时间打散
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void E2ParaCheck(void)
{//对指定no号的E2参数区间进行crc校验判断
    uint8   i = 0;
    uint8   ucLen;
    uint8   ucNo;
    uint16  uiAddr;

    if(gs_DateTime.ucMinute>=0x03)      //从第3分钟开始检测E2
    {
       ucNo=APIByteBCD2HEX(gs_DateTime.ucMinute)-0x03;
    }else
    {
        return;//前几分钟系统负荷太大不处理
    }

    if(ucNo >= dim(gCs_E2ParaChkTab))                                   //正常检测时间未到
    {
        return;
    }

     

    ucLen = gCs_E2ParaChkTab[ucNo].ucLen;
    uiAddr = gCs_E2ParaChkTab[ucNo].uiAddr;

    BE_ReadP(uiAddr, XDATA_RAMZONE, ucLen);                      //读取大E2参数
    
#if (_SW_SE_Type > 0)
    SE_ReadP(uiAddr, XDATA_RAMZONE1, ucLen);                    //读取小E2参数
#else
    BE_ReadP(uiAddr+EEP_BAK_LEN, XDATA_RAMZONE1, ucLen);
#endif  

    if(do_CRC(XDATA_RAMZONE, ucLen - 2) != Uint8_To_Uint16(XDATA_RAMZONE + ucLen - 2))                  //判断大E2是否正确
    {   
        i = 1;                                                                                          //大E2不正确
    }

    if(do_CRC(XDATA_RAMZONE1, ucLen - 2) != Uint8_To_Uint16(XDATA_RAMZONE1 + ucLen - 2))                //判断小E2是否正确
    {
        i += 2;                                                                                         //小E2不正确
    }
    
    if(i == 0)                                                                                          //判断大小E2是否都正确
    {                                                                                                   //大小E2CRC都正确的话
        if(Uint8_To_Uint16(XDATA_RAMZONE + ucLen - 2) != Uint8_To_Uint16(XDATA_RAMZONE1 + ucLen - 2))   //判断大小E2CRC是否相等
        {
            i = 2;                                                                                      //不等，认为小E2数据不正确
        }
    }

    gul_E2ParaStt |= BIT32MAP[ucNo];

    if(i == 1)
    {                                                                               //大E2数据不正确，则用小E2数据覆盖
        gul_E2ParaStt &= ~BIT32MAP[ucNo];            
        BE_WriteP(uiAddr, XDATA_RAMZONE1, ucLen);
    }
    else if(i == 2)
    {                                                                               //小E2数据不正确，则用大E2数据覆盖
    #if (_SW_SE_Type > 0)
        SE_WriteP(uiAddr, XDATA_RAMZONE, ucLen);
    #else
        BE_WriteP(uiAddr+EEP_BAK_LEN, XDATA_RAMZONE, ucLen);
    #endif
    }
}
 
/*=========================================================================================\n
* @function_name: SysE2ParaSetManage
* @function_file: DataProtect.c
* @描述: E2参数备份处理,在参数设置的时候先写了大E2,
*        再对对应的页进行crc校验,同时将这页的数据备份入对应的小E2
* 
* @参数: 
* @param:ucLen  
* @param:uiAddr  
* @param:pucBuf  
* @返回: 
* @作者:   lwb (2012-05-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysE2ParaSetManage(uint16 uiAddr, uint8* pucBuf,uint8 ucLen)
{
    uint8   ucNo; 
    uint8   ucBlkLen;
    uint16  uiE2Addr;
    uint8   ucszChk1[0x80];
    uint8   ucszChk2[0x80];
    uint8*  p = ucszChk1;

    ucNo = GetE2Type(uiAddr);

    if(ucLen == 0)
    {    
        return;
    }
    
    if(ucNo < dim(gCs_E2ParaChkTab))
    {
        //这里假设在设置参数的时候,
        //所设置的参数不会存在跨页的情况,
        //这个时候会导致查gCs_E2ParaChkTab有两个项存在,
        //但实际会只用第一个.
        //这个需要在配置E2参数的时候注意,
        //另外一个是实际情况,没有一个参数能超过页的情况存在,目前至少如此
        ucBlkLen = gCs_E2ParaChkTab[ucNo].ucLen;
        uiE2Addr = gCs_E2ParaChkTab[ucNo].uiAddr;
        BE_ReadP(uiE2Addr, ucszChk1, ucBlkLen);

        if(do_CRC(ucszChk1, ucBlkLen - 2) != Uint8_To_Uint16(ucszChk1 + ucBlkLen - 2))
        {
        #if (_SW_SE_Type > 0)
            SE_ReadP(uiE2Addr, ucszChk2, ucBlkLen);
        #else
            BE_ReadP(uiE2Addr+EEP_BAK_LEN,ucszChk2, ucBlkLen);
        #endif             
            if(do_CRC(ucszChk2, ucBlkLen - 2) == Uint8_To_Uint16(ucszChk2 + ucBlkLen - 2))
            {
                p = ucszChk2;
            }
        }
        

        MemCpy((p+(uiAddr-uiE2Addr)), pucBuf, ucLen);

        Uint16_To_Uint8(do_CRC(p, ucBlkLen - 2), p + ucBlkLen - 2);
        BE_WriteP(uiE2Addr, p, ucBlkLen);
    #if (_SW_SE_Type > 0)
        SE_WriteP(uiE2Addr, p, ucBlkLen);
    #else 
        BE_WriteP(uiE2Addr+EEP_BAK_LEN, p, ucBlkLen);
    #endif
    }
    else
    {
        BE_WriteP(uiAddr, pucBuf, ucLen);
    }
}
/*=========================================================================================\n
* @function_name: SysE2ReadData
* @function_file: DataProtect.c
* @描述: 读取E2数据
* 
* 
* @参数: 
* @param:ucLen  
* @param:uiAddr  
* @param:pucBuf  
* @返回: 
* @作者:   lwb (2012-05-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysE2ReadData(uint16 uiAddr, uint8* pucBuf,uint8 ucLen)
{
    uint8 ucNo = GetE2Type(uiAddr);

    if(ucNo < dim(gCs_E2ParaChkTab))
    {
        if(!(gul_E2ParaStt & BIT32MAP[ucNo]))
        {
        #if (_SW_SE_Type > 0)
            SE_ReadP(uiAddr, pucBuf, ucLen);                //当前小E2数据有效
        #else
            //BE_ReadP(uiAddr+EEP_BAK_LEN,pucBuf, ucLen);  //zzp1128
            BE_ReadP(uiAddr,pucBuf, ucLen);
        #endif  
        }
        else
        {
            BE_ReadP(uiAddr, pucBuf, ucLen);                //当前大E2有效
        }
    }
    else
    {
        BE_ReadP(uiAddr, pucBuf, ucLen);
    }
}
/*=========================================================================================\n
* @function_name: DataCheck
* @function_file: DataProtect.c
* @描述: 检查数据是否合法，不合法使用默认数据
* 
* 
* @参数: 
* @param:ucNum  
* @param:p  
* @param:psDC  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-05-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 DataCheck(uint8 ucNum, uint8* p, S_DATACHK* psDC)
{//用以判断数据的合法性,即判断数据是否在表格设定的上下限范围之内,如果还需要用默认参数进行替代的话,则用默认参数更新
    uint8 i;

    for(i=0; i<ucNum; i++,p++,psDC++)
    {//注意ucMin可能是0,而这里*p又是uint8型的
        if((*p < psDC->ucMin) || (*p > psDC->ucMax))
        {
            //不使用默认值的话就认为这个函数当前功能只是用于判断数据合法性的
            return Const_Data_Err;

        }
    }

    return Const_Data_OK;
}
/*=========================================================================================\n
* @function_name: RefreshPara
* @function_file: DataProtect.c
* @描述: 参数刷新
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RefreshPara(void)
{
    if(gui_RefreshEvent & flgEtPara_EnyBottom)
    {
        Eny_RefVar();         //zzp1122
        gui_RefreshEvent &= ~flgEtPara_EnyBottom;
    }

    else if(gui_RefreshEvent&flgEtPara_Bode)           //更改波特率
    {
        ChangeBodeProc();
        gui_RefreshEvent&=~flgEtPara_Bode;
    }

    else if(gui_RefreshEvent&flgEtPara_Show)
    {
        ShowInitial();
        //显示初始化 从E2读出gs_DispCont结构体用于显示 循显屏数 时间等
        gui_RefreshEvent&=~flgEtPara_Show;
    }
    else if(gui_RefreshEvent&flgEtPara_Init)
    {
        if(CheckCRCWord(guc_InitWd))
        {
            InitE2Data(guc_RTCSave);
            
        }
        //InitE2Data(0);//1127zzp添加测试用
        gui_RefreshEvent&=~flgEtPara_Init;
    }
    else if(gui_RefreshEvent&flgEtPara_InitA)
    {
        UpdateRaccoonParABC(APhy);
        gui_RefreshEvent&=~flgEtPara_InitA;
    }
    else if(gui_RefreshEvent&flgEtPara_InitB)
    {
        UpdateRaccoonParABC(BPhy);
        gui_RefreshEvent&=~flgEtPara_InitB;
    }
    else if(gui_RefreshEvent&flgEtPara_InitC)
    {
        UpdateRaccoonParABC(CPhy);
        gui_RefreshEvent&=~flgEtPara_InitC;
    }
    else
    {
        gui_RefreshEvent =0;
    } 
}

const uint8   SQDateTable[]=
{
    0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,0x30,0x31,0x30,0x31
};
/*=========================================================================================\n
* @function_name: CheckSqFormat
* @function_file: DataProtect.c
* @描述: 时区格式判断
* 
* 
* @参数: 
* @param:str  
* @param:len  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 CheckSqFormat(uint8 * str,uint8 len)
{
    uint8 i,j,flag;
    uint8  ucMonth;
    
    flag=0;   
    if(JudgeIsNum(str,len)==Const_Data_OK)              //BCD码判断
    {
        for(i=0;i<len;i++)
        {
            if((*(str+3*i+2)<0x13)&&(*(str+3*i+2)>0))       //判断月份是否正确
            {
                ucMonth=APIByteBCD2HEX(*(str+3*i+2));     //月份具体判断
                if((*(str+3*i+1)<=SQDateTable[ucMonth-1])&&(*(str+3*i+1)>0))    //天数判断
                {
                    if((*(str+3*i)<9)&&(*(str+3*i)>0))
                    {
                    }
                    else
                    {
                        return ComErrBit_TimeD;        //时段表数超
                    }
                }
                else
                {
                    return ComErrBit_Other;
                }

            }else
            {
                return ComErrBit_Other;
            }

        }
        for(i=0;i<len;i++)
        {
            for(j=i+1;j<len;j++)
            {
                //if(*(str+3*i+2) >= *(str+3*j+2))       //判断月顺序大于等于
                //{
                    if(*(str+3*i+2) > *(str+3*j+2))
                    {
                        return ComErrBit_Other;
                    }       
                    else if(*(str+3*i+2) == *(str+3*j+2))   //月等于时，判断日大于等于                     
                    {
                        if( *(str+3*i+1) > *(str+3*j+1))    //日大于
                        {
                            return ComErrBit_Other;
                        }
                        else if(*(str+3*i+1) == *(str+3*j+1))  //时段不同
                        {
                            if(*(str+3*i) != *(str+3*j))
                            {
                                return ComErrBit_Other;
                            }
                            else
                            {
                                flag=1;
                            }
                        }                      
                    }
                    if(flag)
                    {
                        if(CMP_EQU != ApiBufCmp((str+3*i), (str+3*j), 3))
                        //if(*(str+3*i+1) != *(str+3*j+1) || *(str+3*i+1) != *(str+3*j+1) || *(str+3*i) != *(str+3*j))
                        {
                           return ComErrBit_Other; 
                        }
                    }
                    
                //}
            }
        }

    }else
    {
        return ComErrBit_Other;
    }
    
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: CheckSdFormat
* @function_file: DataProtect.c
* @描述: 时段判断
* 
* 
* @参数: 
* @param:str  
* @param:len  
* @返回: 
* @作者:   lwb (2012-06-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 CheckSdFormat(uint8 * str,uint8 len)
{
    uint8 i;

    if(JudgeIsNum(str,len)==Const_Data_OK)              //BCD码判断
    {
        for(i=0;i<len/3;i++)
        {
            if((*(str+3*i+2)<0x24))                     //时
            {
                if((*(str+3*i+1)<0x60))                 //分
                {
                    if((*(str+3*i)<5)&&(*(str+3*i)>0))
                    {

                    }else
                    {
                        return ComErrBit_Fee;
                    }

                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }
            
        }

    }else
    {
        return ComErrBit_Other;
    }

    return Ret_OK;
}

 
/*=========================================================================================\n
* @function_name: ComCheckFormat
* @function_file: DataProtect.c
* @描述: 检测数据跟随是否正确
* 
* 
* @参数: 
* @param:Databuf  
* @param:type  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-13)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ComCheckFormat(uint8 *Databuf,uint8 type,uint8 len)
{
    uint8 result;
    uint8 flag;
    uint8 i;
    uint16 time1;
    uint16 time2;
    uint16 time3;
    uint8  uc_Tm[6];
    if(type==0)
    {
        return Ret_OK;
    }
    
    switch(type)
    {
        case QhTmFm:                    //2两套时区表切换时间
                                        //分时日月年
            MemCpy(uc_Tm+1,Databuf,5);  //分时日月年
            uc_Tm[0]=0x00;              //秒
            if(Ret_OK == ValidTime(uc_Tm,2))      //判断时间的合法性
            {
                //判断日期是否正确
                return Ret_OK;
            }else
            {
                for(i=0;i<5;i++)
                {
                    if(Databuf[i]!=0x00)
                    {
                        break;
                    }
                }

                if(i>=5)
                {
                    //判断是否全是0
                    return Ret_OK;
                }

                for(i=0;i<5;i++)
                {
                    if(Databuf[i]!=0x99)
                    {
                        break;
                    }
                }

                if(i>=5)
                {
                    //判断是否全是99
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }
            /*
            //判断设置时间是否小于当前时间，小于不然设置
            GetSysClock(uc_NowTm,  Const_YYMMDDhhmm);           //获取系统当前时间
            result=ApiRevBufCmp(uc_NowTm,Databuf,5);            //比较两个时间
            if(result==CMP_LG)
            {
                for(i=0;i<5;i++)
                {
                    if(Databuf[i]!=0x00)
                    {
                        break;
                    }
                }

                if(i>=5)
                {
                    //判断是否全是0
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                //判断年月日是否正确
                return Ret_OK;
            }*/
        case QhCnFm:            //4年时区数p≤14
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                if(*Databuf>=0x01&&*Databuf<=0x14)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_YearQ;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case SqCntFm:
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                if(*Databuf>=0x01&&*Databuf<=0x14)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_TimeD;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case SdBCnFm:           //5日时段表数q≤8
            if(*Databuf>=0x01&&*Databuf<=0x08)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case FeeCnFm:           //7费率数k≤63
            if(*Databuf>=0x01&&*Databuf<=0x04)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Fee;
            }
        case HldCnFm://8公共假日数n≤60
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                uint16 uiGgjrCnt;
                uiGgjrCnt=Uint8_To_Uint16(Databuf);
                uiGgjrCnt=IntBCD2Hex(uiGgjrCnt);//BCD转HEX
                if(uiGgjrCnt<=60)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case AtSwCnFm://9自动循环显示屏数
        case KeySwCnFm://13按键循环显示屏数
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                if(Databuf[0]>=0x01
                   &&Databuf[0]<=0x99)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case AtSwTmFm://10每屏显示时间
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                if(Databuf[0]>=0x01
                   &&Databuf[0]<=0x20)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case EngDocFm://11显示电能小数位数
            if(Databuf[0]==0x00 || Databuf[0]==0x01 || Databuf[0]==0x02)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case PowDocFm://12显示功率(最大需量)小数位数
            if((Databuf[0]>=0x01 && Databuf[0]<=0x04) || Databuf[0]==0x00)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
    case MeterIdFm://14表号
        //是否在公钥还是私钥
//        if((gs_EsamKeyUpDateCtrl.W32Status.lword&EsamKeyRevOkTab[gs_EsamKeyUpDateCtrl.ucTotal-1])
//           !=EsamKeyRevOkTab[gs_EsamKeyUpDateCtrl.ucTotal-1]) //测试密钥
//        {
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
//        }
//        else
//        {
//            return ComErrBit_Other;
//        }
        case MeterAdsFm://通讯地址
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                return Ret_OK;
            }else
            {    
                return ComErrBit_Other;
            }
        case EPCalcFm:
            if(Databuf[0]<=0x0A)
            {
                if(Databuf[0]==0x03
                   ||Databuf[0]==0x07)              //有功组合字除0x00 0x01 0x02 0x04 
                {                                               //0x05 0x06 0x08 0x09 0x0a外都是否非法
                    return ComErrBit_Other;
                }else
                {
                    return Ret_OK;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case IrBdFm://16调制型红外光口通信速率特征字
            if(Databuf[0]==0x04)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_ChBod;
            }
        case RS485Fm:
            for(i = 2; i < 6; i++)                  //判断特征字是否正确
            {
                if(Databuf[0] == BIT8MAP[i])
                {
                    break;
                }
            }

            if(i >= 6)
            {   
                return ComErrBit_ChBod;
            }else
            {
                return Ret_OK;
            }
        case ZxrWdFm://18周休日特征字
            if(Databuf[0]<0x80)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case ZxrSdFm://19周休日采用的日时段表号
            if(Databuf[0]>=0x01
               &&Databuf[0]<=0x08)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case JsrFm: //20每月第1结算日
            if(JudgeIsNum(Databuf,2)==Const_Data_OK)
            {
                if(Databuf[0]<=0x23           //小时判断
                   &&Databuf[1]<=0x28         //日判断
                   &&Databuf[1]>=0x01)
                {
                    return Ret_OK;
                }else
                {
                    if(Databuf[0]==0x99           //小时判断
                       &&Databuf[1]==0x99)        //日判断
                    {
                        return Ret_OK;
                    }else
                    {
                        return ComErrBit_Other;
                    }
                }
            }else
            {
                return ComErrBit_Other;
            }
        case SqDtFm:    //时区格式
            result=CheckSqFormat(Databuf,len/3);
            return result;
        case SdDtFm:
            result=CheckSdFormat(Databuf,len);
            flag=0;
            if(result==0)
            {
                for(i=0;i<len/3-1;i++)
                {
                    time1=HourAndMinToMin(Databuf+i*3+1);
                    time2=HourAndMinToMin(Databuf+(i+1)*3+1);
                    if(flag)                         //已经相等过的话，需要后续都相当
                    {
                        if(time1!=time2||(*(Databuf+i*3)!=*(Databuf+(i+1)*3)))             //如果后续相等的话，允许设置
                        {
                            return ComErrBit_Other;
                        }
                    }else if(time2<time1)
                    {
                        return ComErrBit_Other;
                    }
                    else if(time2-time1<15)
                    {
                        if(time1!=time2||(*(Databuf+i*3)!=*(Databuf+(i+1)*3)))             //如果后续相等的话，允许设置
                        {
                            return ComErrBit_Other;
                        }else
                        {
                            flag=1;
                        }
                    }
                }
                //判断最后一个跟最前面一个是否相差15分钟。
                time1=HourAndMinToMin(Databuf+1);
                time2=HourAndMinToMin(Databuf+len-2);
                time3=1439-time2;
                if((time1+time3)<14)
                {
                    return ComErrBit_Other;
                }else
                {
                    return Ret_OK;
                }
            }else
            {
                return result;
            }
        case ZdDjTmFm:
            if(*Databuf==0x30||*Databuf==0x60)              //除0x60 0x30
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case RDjTmFm:
            //小时日判断
            if(JudgeIsNum(Databuf,2)==Const_Data_OK)
            {
                if(Databuf[0]<=0x59         //分钟判断
                   &&Databuf[1]<=0x23       //小时判断
                   )
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case TzTmFm://跳闸延时时间格式
            if(JudgeIsNum(Databuf,1)==Const_Data_OK)
            {
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case GgJRFm://公共假日格式
          
            for(i=0;i<4;i++)
            {
                if(Databuf[i]!=0x00)
                {
                    break;
                }
            }
            
            if(i>=4)
            {
                return Ret_OK;  
            }
            //时段表号  日  月  年
            if(ValidTime(Databuf+1,0)==Ret_OK)
            {
                if(Databuf[0]<=0x08
                   &&Databuf[0]>=0x01)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }

        case DjMdFm://冻结格式除整点冻结外
            if((Databuf[0]&0x7c)!=0)
            {
                return ComErrBit_Other;
            }else
            {
                return Ret_OK;
            }
        case ZdDjMdFm://整点冻结格式
            if((Databuf[0]&0xFc)!=0)
            {
                return ComErrBit_Other;
            }else
            {
                return Ret_OK;
            }
        case ZdDjStTmFm:
            MemCpy(uc_Tm+1,Databuf,5);  //分时日月年
            uc_Tm[0]=0x00;              //秒
            if(Ret_OK == ValidTime(uc_Tm,2))      //判断时间的合法性
            {
                //判断日期是否正确
                return Ret_OK;
            }else
            {
                return ComErrBit_Other;
            }
        case PowerFullTm:
            if(JudgeIsNum(Databuf,len)==Const_Data_OK)
            {
                if(Databuf[0]>=0x05
                   &&Databuf[0]<=0x30)
                {
                    return Ret_OK;
                }else
                {
                    return ComErrBit_Other;
                }
            }else
            {
                return ComErrBit_Other;
            }
        case LoadCurveFm:
            if(len == 1)  //模式字
            {
                if(Databuf[0]&0xf0)
                {
                    return ComErrBit_Other;    
                }else
                {
                    guc_LCTimeFlag=0xAA;
                    return Ret_OK;
                }
            }
            else if(len == 2)   //时间间隔
            {
                if(JudgeIsNum(Databuf,len)==Const_Data_OK)
                {
                    if(Databuf[1]!=0x00)
                    {
                        return ComErrBit_Other;
                    }else if(Databuf[0]>0x60||Databuf[0]==0x00)                    
                    {
                        return ComErrBit_Other;
                    }
                    else
                    {
                        return Ret_OK;
                    }
                }
            }
            else if(len == 4)   //起始时间
            {
                MemCpy(uc_Tm+1,Databuf,4);  //分时日月
                uc_Tm[0]=0x00;              //秒
                uc_Tm[5]=0x00;              //闰年
                if(Ret_OK == ValidTime(uc_Tm,2))      //判断时间的合法性
                {
                    guc_LCTimeFlag=0x55;
                    return Ret_OK;
                }
                else
                {
                    return ComErrBit_Other;
                }
            }
            return ComErrBit_Other;
//      case PdirFm:
//           if(JudgeIsNum(Databuf,len)==Const_Data_OK)
//           {
//               if(len == 1)  //时间
//               {
//                  if(Databuf[0]<=0x99 && Databuf[0]>=0x10) //时间判断
//                  {
//                      return Ret_OK;
//                  }
//               }
//               else if(len == 3)  //触发下限
//               {
//                  uint32 ulPowdirGate;
//                  uint32 ulPowSet;
//                  ulPowSet = (Uint8_To_Uint32(Databuf)&0x00ffffff);
//                  ulPowSet = BCD2Hex(ulPowSet);
//                  ulPowdirGate = ((gs_JbPm.ui_Un/100)*(gs_JbPm.ui_Ib/1000));
//                  if((ulPowSet >= ulPowdirGate*5/100 && ulPowSet <= ulPowdirGate*50/100) //基本功率0.5%~5%
//                    || ulPowSet == 0) //设置0关闭功率反向
//                  {
//                      return Ret_OK;
//                  }
//               }
//           }
//           return ComErrBit_Other;
        case MeterModeFm:
            if((Databuf[0]&0xFB)!=0)
            {
                return ComErrBit_Other;
            }else
            {
                return Ret_OK;
            }
            
    }
    return 0;
}
