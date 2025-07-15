#define AP_MULTARIFF_EXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: IsNumSD
* @function_file: Multariff.c
* @描述: 判断当前使用第几套时段
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void IsNumSD(void)
{
    uint8 uc_Rst;
    uint8 uc_SdQh[5];
    uint8 uc_NowTm[5];
    uint8 i,j;

    if(gs_FeeData.uc_FeeRSDN>0x02||gs_FeeData.uc_FeeRSDN==0)      //保护作用
    {
        gs_FeeData.uc_FeeRSDN=0x01;
        gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=false;  //修改电表状态字，使用第一套时段
    }

    BE_ReadP(EEP_RSDQHTM,uc_SdQh,5);                    //读E2中的2套时段切换时间
    j=0;
    for(i=0;i<5;i++)                                    //判断是否是全0
    {
        if(uc_SdQh[i]!=0x00)
        {
            if(uc_SdQh[i]!=0x99)
            {
                break;
            }else
            {
                j++;
            }
        }
    }

    if(i>=5&&j<5)                                      //全0,直接返回
    {
        return;
    }
    
    if(j>=5)        //全99
    {
        if(gs_FunCfg.uc_SQSDCfg!=0x55)                  //判断是否全99要强制执行第一套时段
        {
            return;
        }else
        {
            if(gs_FeeData.uc_FeeRSDN!=0x01)             //不是第一套的话
            {
                gs_FeeData.uc_FeeRSDN=0x01;
                gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=false;  //修改电表状态字，使用第一套时段
                RSDFrez();                                      //时段冻结电量
                MemSet((void*)uc_SdQh, 0, 5);                   //时段切换清零
                SysE2ParaSetManage(EEP_RSDQHTM,uc_SdQh,5);      //时段切换时间复位为0写入E2中
            }
            return;
        }
    }


  
    GetSysClock(uc_NowTm,  Const_YYMMDDhhmm);           //获取系统当前时间
    uc_Rst=ApiRevBufCmp(uc_NowTm,uc_SdQh,5);            //比较两个时间

    if(uc_Rst==CMP_EQU || uc_Rst==CMP_LG)
    {
        RSDFrez();                                      //时段冻结电量
        MemSet((void*)uc_SdQh, 0, 5);                   //时段切换清零
        SysE2ParaSetManage(EEP_RSDQHTM,uc_SdQh,5);      //时段切换时间复位为0写入E2中
        if(gs_FeeData.uc_FeeRSDN==0x01)
        {
            gs_FeeData.uc_FeeRSDN=0x02;                                 //当前时间大于或等于切换时间,则选用另一套时段
            gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=true;   //修改电表状态字，使用第二套时段
        }else
        {
            gs_FeeData.uc_FeeRSDN=0x01;
            gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=false;  //修改电表状态字，使用第一套时段
        }
        guc_Pulse |= 0x01;                              //时段表切换
    }
}
#ifdef _SQ_
/*=========================================================================================\n
* @function_name: IsNumSQ
* @function_file: Multariff.c
* @描述: 判断当前使用第几套时区
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void IsNumSQ(void)
{
    uint8 uc_Rst;
    uint8 uc_SqQh[5];
    uint8 uc_NowTm[5];
    uint8 i,j;

    if(gs_FeeData.uc_FeeSQN>0x02||gs_FeeData.uc_FeeSQN==0)      //保护作用
    {
        gs_FeeData.uc_FeeSQN=0x01;
        gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;  //修改电表状态字，使用第一套时段  
    }
    BE_ReadP(EEP_SQQHTM,uc_SqQh,5);                         //读E2中的2套时区切换时间

    j=0;
    for(i=0;i<5;i++)                                    //判断是否是全0
    {
        if(uc_SqQh[i]!=0x00)
        {
            if(uc_SqQh[i]!=0x99)
            {
                break;
            }else
            {
                j++;
            }
        }
    }

    if(i>=5&&j<5)                                      //全0,直接返回
    {
        return;
    }
    
    if(j>=5)        //全99
    {
        if(gs_FunCfg.uc_SQSDCfg!=0x55)                  //判断是否全99要强制执行第一套时段
        {
            return;
        }else
        {
            if(gs_FeeData.uc_FeeSQN!=0x01)             //不是第一套的话
            {
                gs_FeeData.uc_FeeSQN=0x01;                                      //当前时间大于或等于切换时间,则切换另一套时区
                gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;  //修改电表状态字，使用第一套时段  
                SQFrez();                                           //时区冻结
                MemSet((void*)uc_SqQh, 0, 5);                       //时区切换清零
                SysE2ParaSetManage(EEP_SQQHTM,uc_SqQh,5);           //时区切换时间复位为0写入E2中
            }
            return;
        }
    }
 
    GetSysClock(uc_NowTm,  Const_YYMMDDhhmm);               //获取系统当前时间
    uc_Rst=ApiRevBufCmp(uc_NowTm,uc_SqQh,5);                      //比较两个时间
    if(uc_Rst==CMP_EQU || uc_Rst==CMP_LG)
    {
        SQFrez();                                           //时区冻结
        MemSet((void*)uc_SqQh, 0, 5);                       //时区切换清零
        SysE2ParaSetManage(EEP_SQQHTM,uc_SqQh,5);           //时区切换时间复位为0写入E2中
        if(gs_FeeData.uc_FeeSQN==0x01)
        {
            gs_FeeData.uc_FeeSQN=0x02;                                      //当前时间大于或等于切换时间,则切换另一套时区
            gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=true;   //修改电表状态字，使用第二套时区
        }else
        {
            gs_FeeData.uc_FeeSQN=0x01;                                      //当前时间大于或等于切换时间,则切换另一套时区
            gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;  //修改电表状态字，使用第一套时段  
        }
        guc_Pulse |= 0x02;                              //时区表切换
    }
}
#endif
#ifdef _JR_
/*=========================================================================================\n
* @function_name: IsJR
* @function_file: Multariff.c
* @描述: 节假日判断,2007_645标准:YYMMDD+NN,年月日+时段表号,不大于254个,总长度254*4=1016
*           本函数将和其他函数一起使用,不对日时段表号做正确性和合法性判断
* @参数:
*
* @返回:
* @return: uint8 返回:Const_Ret_OK当前日是节假日 / Const_Ret_Err当前日不是节假日
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 IsJR(void)
{
    uint8 uc_Jrcnt;
    uint8 uc_JrData[4];
    uint8 uc_NowTm[3];
    BE_ReadB(EEP_JRCNT,&uc_Jrcnt);                      //读E2中的2套时段切换时间
    if(uc_Jrcnt>254)
    {
        uc_Jrcnt=0x01;
    }
    GetSysClock(uc_NowTm,Const_YYMMDD);                 //获取系统当前时间
    for(uint8 i=0;i<uc_Jrcnt;i++)
    {
        BE_ReadP(EEP_JRS+i*4,uc_JrData,4);
        if(ApiRevBufCmp(uc_NowTm,uc_JrData+1,3)==CMP_EQU)     //比较两个时间
        {                                               //相等,说明今天是节假日
            if(gs_FeeData.uc_FeeRSD != uc_JrData[0])
            {
                guc_Pulse |= 0x20;
            }
            gs_FeeData.uc_FeeRSD = uc_JrData[0];         //返回使用节假日时段
            return Const_Ret_OK;
        }
    }
    return Const_Ret_Err;
}
#endif
#ifdef _ZXR_
/*=========================================================================================\n
* @function_name: IsZXR
* @function_file: Multariff.c
* @描述: 周休日判断
*       本函数将和其他函数一起使用,不对日时段表号做正确性和合法性判断
* @参数:
*
* @返回:
* @return: uint8 返回:Const_Ret_OK当前日是节假日 / Const_Ret_Err当前日不是节假日
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 IsZXR(void)
{
    uint8 uc_Stt;                                       //周休日状态字
    uint8 uc_Sd;                                        //周休日时段号
    BE_ReadB(EEP_SZX,&uc_Stt);                          //读E2中的周休日状态字
    BE_ReadB(EEP_ZXSD,&uc_Sd);                          //读E2中的周休日时段号
    if(uc_Stt&BIT8MAP[gs_DateTime.ucWeek])              //判断是否休息日(0-休息日)
    {                                                   //工作日
        return Const_Ret_Err;                           //返回不使用周休日时段
    }
    if(gs_FeeData.uc_FeeRSD != uc_Sd)
    {       
        guc_Pulse |= 0x20;
    }
    gs_FeeData.uc_FeeRSD =uc_Sd;                        //返回使用周休日时段
    return Const_Ret_OK;
}
#endif
/*=========================================================================================\n
* @function_name: Search_Pos
* @function_file: Multariff.c
* @描述: 查找比Cmp小的最大值，或者数据中最大值
*
*
* @参数:
* @param:buff  查找的数据缓存
* @param:Cmp  比较值
* @param:len  数据缓存的长度,实际的数据缓存长度是3*len
* @param:k
*
* @返回:
* @return: uint8 返回找到的位置
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
/*uint8 Search_Pos(uint8 *buff,uint16 Cmp,uint8 len,uint8 k)
{
    uint16 ui_CmpHigh,ui_CmpLow,ui_CmpTmp;
    uint8 i,uc_pos1=0xff,uc_pos2=0xff;
    for(i=0;i<len;i++)                                  //首先找到一个设置数据正确的设置值。赋值给ui_CmpHigh和ui_CmpLow
    {
        if(DataCheck(2,buff+1+i*3,(S_DATACHK code*)&gCs_ClkChkTab+k) == Const_Data_Err||
        JudgeIsNum(buff+1+i*3,2)==Const_Data_Err)
        {
            continue;
        }
        ui_CmpHigh=ui_CmpLow=Uint8_To_Uint16(buff+1+i*3);
        break;
    }

    for(;i<len;i++)
    {                                                   //继续查找
                                                        //先检查数据的有效值
        if(DataCheck(2,buff+1+i*3,(S_DATACHK code*)&gCs_ClkChkTab+k) == Const_Data_Err||
        JudgeIsNum(buff+1+i*3,2)==Const_Data_Err)
        {
            continue;
        }
        ui_CmpTmp=Uint8_To_Uint16(buff+1+i*3);
        if(ui_CmpTmp<=Cmp)
        {
            if(ui_CmpTmp>=ui_CmpLow)                    //查找比Cmp小的最大值
            {
                ui_CmpLow=ui_CmpTmp;
                uc_pos1=i;
            }
        }else
        {                                               //ui_CmpTemp>Cmp
            if(ui_CmpTmp>=ui_CmpHigh)                   //查找最大值
            {
                ui_CmpHigh=ui_CmpTmp;
                uc_pos2=i;
            }
        }
    }
    if(uc_pos1==0xff &&uc_pos2==0xff)
    {
        return 1;
    }
    else if(uc_pos1!=0xff)                              //找到比Cmp小的值,返回比Cmp小的值中最大值的位置
    {
        return uc_pos1;
    }else
    {
        return uc_pos2;                                 //找不到比Cmp小的值,返回比最大值的位置
    }
}*/

/*=========================================================================================\n
* @function_name: IsPulse
* @function_file: Multariff.c
* @描述:
*
*
* @参数:
* @param:buff
* @param:time
* @param:len
*
* @返回:
* @return: uint8
* @作者:   liming (2013-01-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 Search_SdNum(uint8 *buff,Word16 time,uint8 len)
{/*
    uint8 i;
    
    for(i=0;i<len;i++)
    {
        if((time.byte[0]==*(buff+i*3+2)) && (time.byte[1]==*(buff+i*3+1)))              //如果是高位时间大于
        {
            return true;
        }
    }
    return false;*/
    uint8 i,count,RetValue;
    count=0;
    for(i=0;i<len;i++)
    {
        if(time.byte[0]>*(buff+i*3+2))              //如果是高位时间大于
        {
            count=1;
        }else if(time.byte[0]==*(buff+i*3+2))
        {
            if(time.byte[1]>=*(buff+i*3+1))
            {
                count=1;
            }
        }

        if(time.byte[0]<*(buff+(i+1)*3+2))
        {
            count++;
        }else if(time.byte[0]==*(buff+(i+1)*3+2))
        {
            if(time.byte[1]<*(buff+(i+1)*3+1))
            {
                count++;
            }
        }

        if(count==2)
        {
            break;
        }
        else
        {
            count=0;
        }
    }
    
    if(count==2)
    {
        RetValue=i;
    }else if(i>=len)                        //查找完的话，然后len的最后一个
    {
        RetValue=len-1;
    }else  if(count==0)
    {
        RetValue=13;                //如果没有查找到数据
    }

    return RetValue;                        
}
/*=========================================================================================\n
* @function_name: Search_Pos
* @function_file: Multariff.c
* @描述:
*
*
* @参数:
* @param:buff
* @param:time
* @param:len
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 Search_Pos(uint8 *buff,Word16 time,uint8 len)
{
    uint8 i,count,RetValue;
    count=0;
    for(i=0;i<len;i++)
    {
        if(time.byte[0]>*(buff+i*3+2))              //如果是高位时间大于
        {
            count=1;
        }else if(time.byte[0]==*(buff+i*3+2))
        {
            if(time.byte[1]>=*(buff+i*3+1))
            {
                count=1;
            }
        }

        if(time.byte[0]<*(buff+(i+1)*3+2))
        {
            count++;
        }else if(time.byte[0]==*(buff+(i+1)*3+2))
        {
            if(time.byte[1]<*(buff+(i+1)*3+1))
            {
                count++;
            }
        }

        if(count==2)
        {
            break;
        }
        else
        {
            count=0;
        }
    }
    
    if(count==2)
    {
        RetValue=*(buff+i*3);
    }else if(i>=len)                        //查找完的话，然后len的最后一个
    {
        RetValue=*(buff+(len-1)*3);
    }else  if(count==0)
    {
        RetValue=*(buff+39);                //如果没有查找到数据
    }

    return RetValue;                        //默认为日时段表号1
}
/*=========================================================================================\n
* @function_name: IsValidSQ
* @function_file: Multariff.c
* @描述: 时区判断
*        本函数将和其他函数一起使用,不对日时段表号做正确性和合法性判断
* @参数:
* @返回:
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void IsValidSQ(void)
{
    uint8  uc_Sqcnt,uc_FeeRSD,uc_SqData[3*14];
    uint32 ul_SqAdrr;
    Word16 ui_NowTm;
    ui_NowTm.byte[0]=gs_DateTime.ucMonth;
    ui_NowTm.byte[1]=gs_DateTime.ucDay;

    if(gs_FeeData.uc_FeeSQN==0x02)                  //判断使用的是那套时区
    {
        gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=true;  //修改电表状态字，使用第一套时段  
        ul_SqAdrr=EEP_2SQ;
    }else
    {
        gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;  //修改电表状态字，使用第一套时段  
        ul_SqAdrr=EEP_1SQ;
    }
    BE_ReadB(EEP_SQCNT,&uc_Sqcnt);                                      //读取年时区数
    BE_ReadP(ul_SqAdrr,uc_SqData,42);                                   //读取时区表

    if(uc_Sqcnt>14)
    {
        uc_Sqcnt=0x01;
    }
    
    uc_FeeRSD=Search_Pos(uc_SqData,ui_NowTm,uc_Sqcnt);       //获取使用的时段表
    if(gs_FeeData.uc_FeeRSD != uc_FeeRSD)
    {
        guc_Pulse |= 0x40;
    }
    gs_FeeData.uc_FeeRSD = uc_FeeRSD;
}

/*=========================================================================================\n
* @function_name: getTimeZone
* @function_file: Multariff.c
* @描述: 通过判断当前时间,决定当前需要使用的时区和时段表
*       判断过程的优先级排列为：节假日,周休日,普通日期
* @参数:
* @返回:
* @作者:   lwb (2012-03-07)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void getTimeZone(void)
{
    uint8 uc_RsdCnt;
    uint8 uc_LastDayType;
    uint8 uc_LastFeeRSD;
    
    guc_Pulse=0;
    uc_LastFeeRSD = gs_FeeData.uc_FeeRSD;
    uc_LastDayType = guc_DayType;
    IsNumSD();                                  //获取当前使用的是第几套时段
#ifdef _SQ_
    IsNumSQ();                                  //获取当前使用的是第几套时区
#else
    gs_FeeData.uc_FeeSQN=0x01;                  //没有年时区，默认使用第一套时区
    gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=false;  //修改电表状态字，使用第一套时段  
#endif

#ifdef _JR_
    if(IsJR() == Const_Ret_OK)
    {                                           //当前日期是公共假日
        guc_DayType = 0x01;                              //
    }else
#endif
#ifdef _ZXR_
    if(IsZXR() == Const_Ret_OK)
    {                                           //当前日期是周休日
        guc_DayType = 0x02;                              //
    }else
#endif
    {
#ifdef _SQ_
      //一般日期,进行时区查询
        guc_DayType = 0x04;                              //
        IsValidSQ();
#else
        gs_FeeData.uc_FeeRSD=1;
#endif
    }

    BE_ReadB(EEP_RSDCNT,&uc_RsdCnt);                    //读时段表数


    if((gs_FeeData.uc_FeeRSD == 0) || (gs_FeeData.uc_FeeRSD > uc_RsdCnt))
    {
        if(uc_LastFeeRSD == 1)
        {
            guc_Pulse&=0x0f;
        }
        gs_FeeData.uc_FeeRSD = 1;               //不符合要求就默认当前有效日时段表值为第一个表
    }
    getFee();
    
    if((guc_Pulse != 0) || (guc_DayType != uc_LastDayType))
    {
        if(gs_SysRunSt.PluseType==true)
        {
            guc_Pulse = 0;
            guc_PluseCount=8;                                   //输出80MS的切换信号
            //RATESWITCHIOLOW();
        }    
    }
}

/*=========================================================================================\n
* @function_name: getFee
* @function_file: Multariff.c
* @描述: 通过判断当前时间,决定当前需要使用的费率和时段,并在费率切换时转存能量
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-21)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void getFee(void)
{
//    uint8 uc_SdData[3*14];
//    uint8 uc_Sdcnt,uc_Fee,uc_FeeCnt,uc_SdNum;
//    Word16 W16Time;                                                 //当前系统时间
//    uint32 ul_SdAdrr;
//    
//    W16Time.byte[0]=gs_DateTime.ucHour;
//    W16Time.byte[1]=gs_DateTime.ucMinute;
//    if(gs_FeeData.uc_FeeRSDN==0x02)                                 //判断使用的是第几套时段,获取现在使用的时段表首地址
//    {
//        gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=true;   //修改电表状态字，使用第一套时段
//        ul_SdAdrr=(gs_FeeData.uc_FeeRSD-1)*64+EEP_2RSDS;            //第2套时段,
//    }else
//    {
//        gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=false;  //修改电表状态字，使用第一套时段
//        ul_SdAdrr=(gs_FeeData.uc_FeeRSD-1)*64+EEP_1RSDS;            //第1套时段,
//    }
//
//    BE_ReadB(EEP_FSDCNT,&uc_Sdcnt);                                 //读取日时段切换数
//    BE_ReadP(ul_SdAdrr,uc_SdData,42);                               //读取时段数据
//    if(uc_Sdcnt>14)
//    {
//        uc_Sdcnt=1;
//    }
//
//    //读取费率数，
//    //uc_Fee=Search_Pos(uc_SdData,W16Time,uc_Sdcnt);                  //获取当前费率
//    uc_SdNum = Search_SdNum(uc_SdData,W16Time,uc_Sdcnt);
//    uc_Fee = *(uc_SdData+uc_SdNum*3);
//    
//    if(guc_SdNum != uc_SdNum)                         //判断是否需要投切,感觉这里错误lwb
//    {
//        guc_SdNum = uc_SdNum;
//        guc_Pulse |= 0x04;                                          //
//    }
//                
//    BE_ReadP(EEP_FEECNT,&uc_FeeCnt,1);                               //读取费率数
//    //uc_FeeCnt = 4;
//    
//    if((uc_Fee == 0) || (uc_Fee > uc_FeeCnt))                        //大于费率数，默认走费率1
//    {
//        uc_Fee = 1;
//    }
//
//    uc_Fee--;
//
//    if (gs_FeeData.uc_FeeNo != uc_Fee)                          //费率切换的时候,是否需要写能量和需量计算
//    {
//        Eny_EnergyActiveSave();
//        gs_FeeData.uc_FeeNo = uc_Fee;
//        guc_Pulse |= 0x04;  
//    }
}





