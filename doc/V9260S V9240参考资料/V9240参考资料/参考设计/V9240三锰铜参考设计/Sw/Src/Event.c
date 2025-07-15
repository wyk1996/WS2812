
#define AP_EVENT_EXT
#include "Include.h"


/******************************************************************************************************* 
事件记录
    a） 永久记录电能表清零事件的发生时刻及清零时的电能量数据。
    b） 应记录编程总次数，最近10次编程的时刻、操作者代码、编程项的数据标识。
    c） 应记录校时总次数（不包含广播校时），最近10次校时的时刻、操作者代码。
    d） 应记录掉电的总次数，最近10次掉电发生及结束的时刻。
    e） 应记录最近10次远程控制拉闸和最近10次远程控制合闸事件，记录拉、合闸事件发生时刻和电能量等数据。
    f） 应记录开表盖总次数，最近10次开表盖事件的发生、结束时刻。
 
冻结功能
定时冻结：按照约定的时间及间隔冻结电能量数据；每个冻结量至少应保存12次。
瞬时冻结：在非正常情况下，冻结当前的日历、时间、所有电能量和重要测量量的数据；瞬时冻结量应保存最后3次的数据。
约定冻结：在新老两套费率/时段转换、阶梯电价转换或电力公司认为有特殊需要时，冻结转换时刻的电能量以及其他 
          重要数据，保存最后2次冻结数据。
日冻结：  存储每天零点时刻的电能量，应可存储两个月的数据。
整点冻结：存储整点时刻或半点时刻的有功总电能，应可存储96个数据。
********************************************************************************************************/


/*******************************************************************************
**清事件信息
*******************************************************************************/
const S_CLEAREVENT code gs_ClearEvent[] =
{
    {0x031100FF, ClrType_Power},
    {0x033000FF, ClrType_BC},
    {0x033004FF, ClrType_JS},
    {0x03300DFF, ClrType_BG},
    {0x033700FF, ClrType_POWERR},
#ifdef SQSDPRLOG
    {0x033005FF, ClrType_RSDBC},
    {0x033006FF, ClrType_SQBC},
#endif

#ifdef ZXRPRLOG
    {0x033007FF, ClrType_ZXRBC},
#endif

#ifdef YGZHZPRLOG
    {0x033009FF, ClrType_YGZHBC},
#endif

#ifdef JSRPRLOG
    {0x03300CFF, ClrType_JSRBC},
#endif
    {0xFFFFFFFF, ClrType_All},
      
};

                                                        //项数
const uint8 code CntgClrE = sizeof(gs_ClearEvent) / sizeof(S_CLEAREVENT);

/*=========================================================================================\n
* @function_name: Data_ClearEvent
* @function_file: Event.c
* @描述: 清事件
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Data_ClearEvent(void)
{
  /*  uint8 i,j;
    //首先进行判断，是否是误动作

    MemSet(XDATA_RAMZONE,0x00,64);          //先清零外部RAM 
    switch(guc_ClearType)
    {
        case ClrType_All:                                       //清所有记录

            for(i=3;i<dim(gCs_MeterClearAddrTab);i++)
            {
                for(j=0;j<gCs_MeterClearAddrTab[i].ucPageCnt;j++)
                {
                    CLRWDT();               //喂狗
                    BE_WriteP(gCs_MeterClearAddrTab[i].uiE2Addr+64*j,XDATA_RAMZONE,64);    //电表清零记录
                }
            }
            guc_PrKeyNewDown=0x55;
            guc_CoverStatus=0;
            guc_PoweroffCoverJl=0x55;
            MemSet((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //主动上报状态清零
            MemSet((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //主动上报状态标志清零
            break;
        case ClrType_Power:                                     //清上下电记录
            for(i=0;i<2;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_DDETNO+64*i,XDATA_RAMZONE,64);    //掉电记录
            }
            break;

        case ClrType_BC:                                        //清编程记录
            for(i=0;i<8;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_BCETNO+64*i,XDATA_RAMZONE,64);    //编程记录
            }
            guc_PrKeyNewDown=0x55;                              //清完编程记录后，要恢复进去编程
            break;
        case ClrType_JS:                                        //清校时记录
            for(i=0;i<3;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_JSETNO+64*i,XDATA_RAMZONE,64);    //校时记录
            }
            break;

        case ClrType_BG:                                        //清表盖记录
            for(i=0;i<5;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_KBGETNO+64*i,XDATA_RAMZONE,64);    //开表盖记录
            }
            guc_CoverStatus=0;
            guc_PoweroffCoverJl=0x55;
            gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt=0;
            gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt=0;  
            break;
#ifdef SQSDPRLOG
        case ClrType_RSDBC:                                     //清日时段编程记录
            for(i=0;i<9;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_RSDXGNO+64*i,XDATA_RAMZONE,64);    //日时段编程记录
            }
            break;
        case ClrType_SQBC:                                       //清时区编程记录
            for(i=0;i<9;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_SQXGNO+64*i,XDATA_RAMZONE,64);    //时区编程记录
            }
            break;
#endif

#ifdef ZXRPRLOG
        case ClrType_ZXRBC:                                //清周休日编程记录
            for(i=0;i<2;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_ZXRXGNO+64*i,XDATA_RAMZONE,64);    //周休日编程记录
            }
            break;
#endif

#ifdef YGZHZPRLOG
        case ClrType_YGZHBC:                                //清有功组合方式编程记录
            for(i=0;i<2;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_YGZHETNO+64*i,XDATA_RAMZONE,64);    //有功组合方式编程记录
            }
            break;
#endif
#ifdef JSRPRLOG
        case ClrType_JSRBC:                                         //清结算日编程记录
            for(i=0;i<3;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_JSRETNO+64*i,XDATA_RAMZONE,64);    //结算日编程记录
            }
            break;
#endif
        case ClrType_POWERR:             
            for(i=0;i<4;i++)
            {
                CLRWDT();               //喂狗
                BE_WriteP(EEP_DYYCNO+64*i,XDATA_RAMZONE,64);    //电源异常记录
            }
            break;   
    }

    //事件清零记录
    Com_ClearEventJL(guc_ClearType);*/
}
 
/*=========================================================================================\n
* @function_name: PowerJL
* @function_file: Event.c
* @描述: 停电记录
* 
* 
* @参数: 
* @param:type  1: 停电  2：上电
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void PowerJL(uint8 type)
{
    if(type == 1)                                               //如果是停电
    {
        DataProcEvCountAdd(DDNUMID);                            //增加掉电次数
        MemCpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);
        MemSet(XDATA_RAMZONE+6,0x00,6);                         //上电时间清零
        DataProcWriteRound2(R2_MOVE,DDJLID,XDATA_RAMZONE);      //写入掉电记录
    }
    else if(type == 2)
    {
        DataProcRead2Slice(DDJLID,0,0,6,XDATA_RAMZONE);         //读取最近一次掉电记录
        MemCpy(XDATA_RAMZONE+6,&gs_DateTime.ucSecond,6);         //写入上电时间
        DataProcWriteRound2(R2_NO_MOVE,DDJLID,XDATA_RAMZONE);   //写入掉电记录
    } 
}
 
/*=========================================================================================\n
* @function_name: Com_JSJL
* @function_file: Event.c
* @描述: 校时记录
* 
* 
* @参数: 
* @param:code  
* @param:time  
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Com_JSJL(uint32 usecode, S_TIMEDATA* time)
{
    DataProcEvCountAdd(JSNUMID);                        //增加校时次数
    MemCpy(XDATA_RAMZONE,(uint8*)&usecode,4);           //操作代码
    MemCpy(XDATA_RAMZONE+4,&time->ucSecond,6);          //校正前的时钟
    MemCpy(XDATA_RAMZONE+10,&gs_DateTime.ucSecond,6);   //校正后的时钟
    DataProcWriteRound2(R2_MOVE,JSJLID,XDATA_RAMZONE);  //写入校时记录
    if( gs_DateTime.ucDay != time->ucDay 
       || gs_DateTime.ucMonth != time->ucMonth
       || gs_DateTime.ucYear != time->ucYear )
    {
        gui_SystemState &= ~flgStSys_IsTodayJS;         //去掉已经进行过广播校时标记
    }
    //guc_Timechange = 0x55;
}

 
/*=========================================================================================\n
* @function_name: PowerUpProc
* @function_file: Dataproc.c
* @描述: 上电初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void PowerUpProc(void)
{
    PowerJL(2);                                         //停电记录
    gui_SystemState &= ~flgStSys_PowOff;
}
 
/*=========================================================================================\n
* @function_name: PowerDnProc
* @function_file: Dataproc.c
* @描述: 下电初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void PowerDnProc(void)
{
    PowerJL(1);                                         //停电记录
    gui_SystemState |= flgStSys_PowOff;
} 
/*=========================================================================================\n
* @function_name: Com_ClearMeterJL
* @function_file: Event.c
* @描述: 电表清零记录
* 
* 
* @参数: 
* @param:code  
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Com_ClearMeterJL(void)
{
//    int16 Eadd;
//    Word32 data1;
//
//    //电表清零次数
//    DataProcEvCountAdd(DBQLNUMID);
//
//    MemCpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);          //写入瞬时冻结时间
//    MemCpy(XDATA_RAMZONE+6,(uint8*)&guw_UseCode.byte,4);    //操作者代码
//
//    Eadd = Eny_GetEp1(1, 0);                                //取得某个费率的电量增量
//    data1.lword= gs_EnergyA.ulP[0]+Eadd;
//    data1.lword/=10;
//    data1.lword=Hex2BCD(data1.lword);
//    MemCpy(XDATA_RAMZONE+10,(void*)data1.byte,4);           //正向 总电量
// 
//    Eadd = Eny_GetEp1(2, 0);                                //取得某个费率的电量增量
//    data1.lword= gs_EnergyA.ulN[0]+Eadd;
//    data1.lword/=10;
//    data1.lword=Hex2BCD(data1.lword);
//    MemCpy(XDATA_RAMZONE+14,(void*)data1.byte,4);           //反向 总电量
//
//    DataProcWriteRound2(R2_MOVE,DBQLJLID,XDATA_RAMZONE);    //写入数据  
     
}

/*=========================================================================================\n
* @function_name: Com_ClearEventJL
* @function_file: Event.c
* @描述: 
* 
* 
* @参数: 
* @param:usecode  
* @返回: 
* @作者:   lwb (2012-06-06)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Com_ClearEventJL(uint8 type)
{
    uint8 i;

    for(i = 0; i < CntgClrE; i++)                                       //寻找清事件的标志
    {
        if(type == gs_ClearEvent[i].type)                               //看看是否相等，相等就是找到了
        {
            break;
        }
    }

    if(i >= CntgClrE)                                                   //看看有没有找到
    {
        return;
    }
    //电表清零次数
    DataProcEvCountAdd(SJQLNUMID);
    MemCpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);                      //写入瞬时冻结时间
    MemCpy(XDATA_RAMZONE+6,(uint8*)&guw_UseCode.byte,4);                //操作者代码

    FCpyTMem(XDATA_RAMZONE+10,(uint8 code*)&gs_ClearEvent[i].ID ,4);    //事件标识
    
 
    DataProcWriteRound2(R2_MOVE,SJQLJLID,XDATA_RAMZONE);                //写入数据  

}
/*=========================================================================================\n
* @function_name: SaveProgramLog
* @function_file: Event.c
* @描述: 写编程记录，要写的包括总编程次数、操作者代码、
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
uint8 SaveProgramLog(S_FRAMEINFO *s_FrmInfo)
{
    //编程键按下刚按下标准，1，按下编程键，在身份认证没有通过的情况下，置标志
    //2.事件总清（或者编程事件清零）、电表清零后，编程键按下的状态，
    //3，身份认证完，置刚按下标志
    if((gui_SystemState&flgStSys_Program))                          //首先判断编程键是否按下
    {
        if(guc_PrKeyNewDown==0x55)                                  //根据编程键是否刚按下,没有设置过数据,这样的话，需要增加编程次数，并且编程记录需要在下一设置
        {
            MemCpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);          //获取时间 
            MemCpy(XDATA_RAMZONE+6,s_FrmInfo->uiUserCode.byte,4);   //操作者代码
            guc_PrKeyNewDown=0;                                     //编程键刚按下标志清零
            DataProcEvCountAdd(BCNUMID);                            //读取编程次数，累加1，写入编程次数
            MemCpy(XDATA_RAMZONE+10,s_FrmInfo->uiID.byte,4);        //添加新的数据标识
            MemSet(XDATA_RAMZONE+14,0xff,36);                       //第一个数据标识，其他数据标识填写为0XFF
            DataProcWriteRound2(R2_MOVE,BCJLID,XDATA_RAMZONE);      //写入数据  
        }
        else
        {
            DataProcRead2Slice(BCJLID,0,0,50,XDATA_RAMZONE);        //读取最近编程记录
            MoveBuffer(XDATA_RAMZONE+10,XDATA_RAMZONE+14,36);       //从第10个数据开始外后移动4个数据,移动读取的编程节点的数据
            MemCpy(XDATA_RAMZONE+10,s_FrmInfo->uiID.byte,4);        //添加新的数据标识
            DataProcWriteRound2(R2_NO_MOVE,BCJLID,XDATA_RAMZONE);   //写入数据  
        }
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: StructFreezeDate
* @function_file: Event.c
* @描述: 构建冻结数据
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void StructFreezeDate(void)
{
//    int16 Eadd;
//    Word32 data1;
//    uint8 i;
//
//    MemCpy(XDATA_RAMZONE,&gs_DateTime.ucMinute,5);          //写入瞬时冻结时间
//
//    for(i = 0; i <= 4; i++)
//    {
//        Eadd = Eny_GetEp1(1, i);                            //取得某个费率的电量增量
//        data1.lword= gs_EnergyA.ulP[i]+Eadd;
//        data1.lword/=10;
//        data1.lword=Hex2BCD(data1.lword);
//        MemCpy(XDATA_RAMZONE+5+i*4,(void*)data1.byte,4);    //正向 总尖峰平谷电量
//    }
//
//    for(i = 0; i <= 4; i++)
//    {
//        Eadd = Eny_GetEp1(2, i);                            //取得某个费率的电量增量
//        data1.lword= gs_EnergyA.ulN[i]+Eadd;
//        data1.lword/=10;
//        data1.lword=Hex2BCD(data1.lword);
//        MemCpy(XDATA_RAMZONE+25+i*4,(void*)data1.byte,4);   //反向 总尖峰平谷电量
//    }
//   
//    data1.lword=CalRMS(DATAP);
//    MemCpy(XDATA_RAMZONE+45,data1.byte,4);                  //功率
}
/*=========================================================================================\n
* @function_name: DSFrez
* @function_file: Event.c
* @描述: 定时冻结
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void DSFrez(void)
{
    uint8 time[4];
    uint8 i;
    uint8 * pTm;

    pTm=(uint8*)&gs_DateTime.ucMinute;

    //判断定时冻结是否到了

    BE_ReadP(EEP_DSFRZ,time,4);          //分时天月

    for(i=0;i<4;i++)
    {
        if(time[i]==0x99)
        {
            continue;
        }
  
        if(time[i]!=*(pTm+i))                   //时间不等，直接返回
        {
            return;
        }
    }

    DataProcEvCountAdd(DSDJNUMID);                      //增加日冻结次数
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,DSDJID,XDATA_RAMZONE);  //写入数据  
}
/*=========================================================================================\n
* @function_name: HurryFrez
* @function_file: Event.c
* @描述: 立即冻结
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void HurryFrez(void)
{
    DataProcEvCountAdd(SSDJNUMID);                       //增加瞬时冻结次数
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,SSDJID,XDATA_RAMZONE);   //写入数据  
} 
/*=========================================================================================\n
* @function_name: ZDFrez
* @function_file: Event.c
* @描述: 整点冻结
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ZDFrez(void)
{
//    int16 Eadd;
//    Word32 data1;
//    uint8 time[5];              //年月日时分
//    uint8 Curtime[5];
//    uint8 IntTm;                //间隔时间
//    uint8 temp2,temp3;
//
//    BE_ReadP(EEP_ZDFREEZTM, time,5);  //整点冻结起始时间,分 时 日 月 年
//    ApiReverseData(time,5);            //交换数据 年 月 日 时 分
//    MemCpy(Curtime,(uint8*)&gs_DateTime.ucMinute,5);
//    ApiReverseData(Curtime,5);
//    if(ApiBufCmp(time,Curtime,5)!=CMP_LG)               //如果当前时间比较大一点的话才会去做整点冻结
//    {
//        BE_ReadP(EEP_ZDTMJG, &IntTm, 1);                //冻结时间间隔,已经是HEX码了
//        temp2=APIByteBCD2HEX(gs_DateTime.ucMinute);
//        temp3=temp2%IntTm;
//        if(temp3==0)                                            //整点冻结时间到
//        {
//
//            DataProcEvCountAdd(ZDDJNUMID);                      //增加时区冻结次数
//                                                                //执行整点冻结
//            MemCpy(XDATA_RAMZONE,&gs_DateTime.ucMinute,5);        //写入瞬时冻结时间
//            
//            Eadd = Eny_GetEp1(1, 0);                            //取得某个费率的电量增量
//            data1.lword= gs_EnergyA.ulP[0]+Eadd;
//            data1.lword/=10;
//            data1.lword=Hex2BCD(data1.lword);
//            MemCpy(XDATA_RAMZONE+5,(void*)data1.byte,4);        //正向总电量
//            
//            Eadd = Eny_GetEp1(2, 0);                            //取得某个费率的电量增量
//            data1.lword= gs_EnergyA.ulN[0]+Eadd;
//            data1.lword/=10;
//            data1.lword=Hex2BCD(data1.lword);
//            MemCpy(XDATA_RAMZONE+9,(void*)data1.byte,4);        //正向总电量
//            DataProcWriteRound2(R2_MOVE,ZDDJID,XDATA_RAMZONE);   //写入数据  
//        }
//    }
}
 
/*=========================================================================================\n
* @function_name: DayFrez
* @function_file: Event.c
* @描述: 日冻结
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void DayFrez(void)
{
    Word16 RdjTm;
    //判断日冻结是否到达
    //首先读取日冻结时间
    BE_ReadP(EEP_RFREEZTM,RdjTm.byte,2);                    //读取日冻结时间
    if((gs_DateTime.ucHour==RdjTm.byte[1])
       &&(gs_DateTime.ucMinute==RdjTm.byte[0]))             //日冻结时间到,写入数据
    {

        DataProcEvCountAdd(RDJNUMID);                       //增加日冻结次数
        StructFreezeDate();
        DataProcWriteRound2(R2_MOVE,RDJID,XDATA_RAMZONE);   //写入数据  
    }
}
 
/*=========================================================================================\n
* @function_name: RSDFrez
* @function_file: Event.c
* @描述: 日时段表切换冻结
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RSDFrez(void)
{
    DataProcEvCountAdd(RSDQHDJNUMID);                       //增加时段表次数
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,RSDQHDJID,XDATA_RAMZONE);   //写入数据  
}
/*=========================================================================================\n
* @function_name: SQFrez
* @function_file: Event.c
* @描述: 时区表切换冻结
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SQFrez(void)
{
    DataProcEvCountAdd(SQQHDJNUMID);                        //增加时区冻结次数
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,SQQHDJID,XDATA_RAMZONE);   //写入数据  
} 
/*=========================================================================================\n
* @function_name: CoverOpenLog
* @function_file: Event.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CoverOpenLog(void)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcEvCountAdd(KBGNUMID);                   //增加开表盖次数
//
//    if(_SYS_IS_PWR_OFF)
//    {
//        SleepTimeProc();                                            //读取RTC时钟到RAM中,
//    }
//    MemCpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);    //写入开表盖时间
//    MemSet(XDATA_RAMZONE+6,0x00,6);                 //关表盖时间清零
//    //正向有功
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
//    //反向有功
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//    MemCpy(XDATA_RAMZONE+12,(uint8*)&ul_EnyData,8);     //写入正反向电量
//
//
//    MemSet(XDATA_RAMZONE+20,0x00,8);
//    //无功不需要
//    DataProcWriteRound2(R2_MOVE,KBGJLID,XDATA_RAMZONE); //写入开表盖记录
// 
}
/*=========================================================================================\n
* @function_name: CoverCloseLog
* @function_file: Event.c
* @描述: 关闭表盖的处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CoverCloseLog(void)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcRead2Slice(KBGJLID,0,0,20,XDATA_RAMZONE);               //读取最近一次开表盖记录，读取20个字节，最后8个关表盖电量不需要读取
//
//    if(_SYS_IS_PWR_OFF)
//    {
//        SleepTimeProc();                                            //读取RTC时钟到RAM中,
//    }
//
//    MemCpy(XDATA_RAMZONE+6,&gs_DateTime.ucSecond,6);                //写入关表盖时间
//
//    //正向有功
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
// 
//    //反向有功
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//
//    MemCpy(XDATA_RAMZONE+20,(uint8*)&ul_EnyData,8);         //写入正反向电量
//    DataProcWriteRound2(R2_NO_MOVE,KBGJLID,XDATA_RAMZONE);  //写入开表盖记录
}

