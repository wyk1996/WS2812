#define TIMEPROC_EXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: TimerProc
* @function_file: Timeproc.c
* @描述: 实时钟（每秒调用1次）
*
* @参数:
* @返回:
* @作者:   lwb (2012-02-29)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void TimerProc(void)
{
    uint8 uc_maxday;
    uint8 uc_SoftTimer;                     // 是否采用软时钟数据，1-是，0-否
	uint8 ucDay;
    uc_SoftTimer = CheckTime();             // 检查软硬时钟

    if (uc_SoftTimer != 0)                  // 采用软时钟数据
    {
        uint8 ucSec = APIByteBCD2HEX(gs_DateTime.ucSecond);
        ucSec++;                            //增加1秒
        if(ucSec >= 60)
        {
            ucSec = 0;
        }
        gs_DateTime.ucSecond = APIByteHEX2BCD(ucSec);
    }

    // 达到一分钟
    // 当前秒比原有秒小，且相差超过30秒。
    // 正常情况下 59->0，也可能出现类似50->3的情况，这些情况均能判为过分
    if (((int16)gs_LastDateTime.ucSecond - gs_DateTime.ucSecond) > 30)
    {
        guc_TimeEvent|=flgEtTim_Minute;
        //MessageSend(msg_Min);                                                     // 发送分消息
        if (uc_SoftTimer != 0)                                                  // 采用软时钟数据
        {
            uint8 ucMin = APIByteBCD2HEX(gs_DateTime.ucMinute);
            ucMin++;                                                            //增加1分
            if(ucMin >= 60)
            {
                ucMin = 0;
            }
            gs_DateTime.ucMinute = APIByteHEX2BCD(ucMin);
        }

                                                                                // 达到一小时
        if (gs_DateTime.ucMinute == 0 && gs_LastDateTime.ucMinute > 0)
        {

            guc_TimeEvent|=flgEtTim_Hour;
            //MessageSend(msg_Hour);                                                // 发送小时消息
            if (uc_SoftTimer != 0)                                              // 采用软时钟数据
            {
                uint8 ucHour = APIByteBCD2HEX(gs_DateTime.ucHour);
                ucHour++;                                                       //增加1小时
                if(ucHour >= 24)
                {
                    ucHour = 0;
                }
                gs_DateTime.ucHour = APIByteHEX2BCD(ucHour);
            }

                                                                                // 达到一天
            if (gs_DateTime.ucHour == 0 && gs_LastDateTime.ucHour > 0)
            {

                guc_TimeEvent|=flgEtTim_Day;
                //MessageSend(msg_Day);                                             // 发送过天消息
                if (uc_SoftTimer != 0)                                          // 采用软时钟数据
                {
                    gs_DateTime.ucWeek ++;                                      // 周 +1
                    if (gs_DateTime.ucWeek >= 0x07)
                    {
                        gs_DateTime.ucWeek = 0;
                    }

                    uc_maxday = MonthDays[gs_DateTime.ucMonth];                 //用这种方法的话这个表就要改为HEX表了
                    if(gs_DateTime.ucMonth == 0x02)
                    {
                        if((APIByteBCD2HEX(gs_DateTime.ucYear) & 0x03) == 0)      // 闰年
                        {
                            uc_maxday++;                                        //闰年二月最大天数增加1
                        }
                    }

                     ucDay = APIByteBCD2HEX(gs_DateTime.ucDay);
                    ucDay++;                                                    //增加1天
                    if(ucDay > uc_maxday)
                    {
                        ucDay = 1;
                    }
                    gs_DateTime.ucDay = APIByteHEX2BCD(ucDay);
                }

                //gui_DateModifyCount++;  //added by zlq 06.12.28
                //EnergySaveEvSet(E_SAVE_ZEROEP);
                // 跨月
                if (gs_DateTime.ucDay == 0x01 && gs_LastDateTime.ucDay > 0x01)
                {
                    if (uc_SoftTimer != 0)                                      // 采用软时钟数据
                    {
                        uint8 ucMonth = APIByteBCD2HEX(gs_DateTime.ucMonth);
                        ucMonth++;                                              //增加1月
                        if(ucMonth > 12)
                        {
                            ucMonth = 1;
                        }
                        gs_DateTime.ucMonth = APIByteHEX2BCD(ucMonth);
                    }

                    // 跨年
                    if (gs_DateTime.ucMonth == 0x01 && gs_LastDateTime.ucMonth > 0x01)
                    {
                        uint8 ucYear = APIByteBCD2HEX(gs_DateTime.ucYear);
                        ucYear++;
                        if(ucYear > 99)
                        {
                            ucYear = 0;
                        }
                        gs_DateTime.ucYear = APIByteHEX2BCD(ucYear);
                    }
                }
            }
        }
    }

    ClockBAK_RAM();                                                             //备份时钟
}
/**=============================================================================================\n
 * @function_name: CalcAbsDay
 * @function_file: AP_timeproc.c
 * @描述:计算现在年月日距离2000.1.1日的天数
 *
 *
 * @param:psTm
 *
 * @返回: uint16
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
uint16 CalcAbsDay(S_TIMEDATA* psTm)
{//以2000年1月1日为基准到现在的所有天数,因此时间不能在2000年之前(其实2000年用00表示,百年以上都略去)
    uint8 ucYear = APIByteBCD2HEX(psTm->ucYear);        //年转换成Hex
    uint8 ucMonth = APIByteBCD2HEX(psTm->ucMonth)-1;      //月转换成Hex
    uint8 ucDay = APIByteBCD2HEX(psTm->ucDay) - 1;      //日转换成Hex

    if((ucYear & 0x03) == 0)                            //判断是否过了闰年的2月
    {
        if(ucMonth >= 2)
            ucDay++;                                    //大于等于3月份，闰年应该加上2。29的一天
    }
    else
    {
        ucDay++;                                        //非闰年加上00年闰月多出来的一天
    }
    //year >> 2,是加上过去的整4年的闰年的一天
    return (ucYear*365 + gui_MonthBeginDay[ucMonth] + (ucDay + (ucYear>>2)));
}
/**=============================================================================================\n
 * @function_name: CalcAbsSecond
 * @function_file: AP_timeproc.c
 * @描述:
 *
 *
 * @param:psTm
 *
 * @返回: uint32
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
uint32 CalcAbsSecond(S_TIMEDATA* psTm)
{
    //以当日00时00分00秒为基准秒
    uint8 ucSec = APIByteBCD2HEX(psTm->ucSecond);       //秒转换成Hex
    uint8 ucMin = APIByteBCD2HEX(psTm->ucMinute);       //分转换成Hex
    uint8 ucHour = APIByteBCD2HEX(psTm->ucHour);        //时转换成Hex

    return (ucSec + ucMin*60 + (uint32)ucHour*HourSecCount);
}
/**=============================================================================================\n
 * @function_name: CalcAbsTime
 * @function_file: AP_timeproc.c
 * @描述:获取绝对时间（相对于2000年1月1日 0:00:00 开始的秒）
 *
 *
 * @param:psTm
 *
 * @返回: uint32 秒值
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
uint32 CalcAbsTime(S_TIMEDATA* psTm)
{
    return (CalcAbsDay(psTm)*DaySecCount + CalcAbsSecond(psTm));
}
/*=========================================================================================\n
* @function_name: CalcTimeGap
* @function_file: Timeproc.c
* @描述: 计算软硬时钟相差的秒数
*
*
* @参数:
* @param:psTm1
* @param:psTm2
*
* @返回:
* @return: uint32
* @作者:   lwb (2012-02-29)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint32 CalcTimeGap(S_TIMEDATA* psTm1, S_TIMEDATA* psTm2)
{
    int32 lGap;
    lGap = (int32)CalcAbsTime(psTm1) - (int32)CalcAbsTime(psTm2);
    if(lGap < 0)
    {
        lGap = -lGap;
    }
    return ((uint32)lGap);
}

/*=========================================================================================\n
* @function_name: ClockCheck
* @function_file: Timeproc.c
* @描述: 检测时钟CRC和数据格式是否正确
*
*
* @参数:
* @param:psTm
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-29)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 ClockCheck(S_TIMEDATA* psTm)
{
    if(do_CRC((uint8*)psTm, sizeof(S_TIMEDATA)-2) == psTm->uiCRC)
    {
        return (RTCCheck((uint8*)psTm));
    }
    return Const_Data_Err;
}
/*=========================================================================================\n
* @function_name: SoftClockCheck
* @function_file: Timeproc.c
* @描述: 软时钟数据和E2时钟数据正确性和有效性检验
*
* @参数:
*
* @返回:
* @return: uint8    SOFT_RTC_MAIN, 软时钟ram部分至少有一个正确
*                   SOFT_RTC_E2,   软时钟靠E2部分的来更新了
*                   SOFT_RTC_ERR,  软时钟部分连E2备份都无效了
* @作者:   lwb (2012-02-29)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 SoftClockCheck(void)
{
    uint8   ucSoftRtc = SOFT_RTC_MAIN;                                                //当前主时钟有效

    if(ClockCheck(&gs_DateTime) != Const_Data_OK)
    {                                                                               //当前软时钟CRC校验错误
        if(ClockCheck(&gs_LastDateTime) != Const_Data_OK)
        {                                                                           //当前软时钟备份CRC校验错误
            BE_ReadP(EEP_DATETIME,(uint8*)&gs_DateTime, sizeof(S_TIMEDATA));        //从E2中获取时间数据

            if(ClockCheck(&gs_DateTime) == Const_Data_OK)                           //检测E2备份时钟的有效性
            {
                ucSoftRtc = SOFT_RTC_E2;
            }
            else                                                                    //主E2时钟数据不正确
            {
            #if (_SW_SE_Type > 0)
                SE_ReadP(EEP_DATETIME,(uint8*)&gs_DateTime,sizeof(S_TIMEDATA));     //读取备份时钟数据
            #else
                BE_ReadP(EEP_DATETIME+EEP_BAK_LEN, (uint8*)&gs_DateTime, sizeof(S_TIMEDATA));
            #endif
                if(ClockCheck(&gs_DateTime) == Const_Data_OK)
                {
                    ucSoftRtc = SOFT_RTC_E2;
                }
                else
                {
                    ucSoftRtc = SOFT_RTC_ERR;
                }
            }
        }
        else
        {
            MemCpy((uint8*)(&gs_DateTime), (uint8*)(&gs_LastDateTime), sizeof(S_TIMEDATA)); //从RAM备份恢复
//          memcpy((uint8*)(&gs_DateTime), (uint8*)(&gs_LastDateTime), sizeof(S_TIMEDATA)); //从RAM备份恢复
        }
    }
    return ucSoftRtc;
}
/*=========================================================================================\n
* @function_name: CheckTime
* @function_file: Timeproc.c
* @描述: 获取实时钟
*
* @参数:
*
* @返回:
* @return: uint8 0-采用硬时钟，1-采用软时钟
* @作者:   lwb (2012-02-29)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 CheckTime(void)
{
#ifndef _SW_EXT_RTC
    SoftClockCheck();
#else
    uint8   ucSoftRtc = SoftClockCheck();
    uint8   ucExtRtc = EXT_RTC_NO_WORK;
    if(gs_DateTime.ucSecond == 0x33)                                        //有电时每分钟33s时用外部rtc更新,停电时每小时27min的33s更新
    {
        if(_SYS_IS_PWR_OFF)
        {
            if(gs_DateTime.ucMinute == 0x27)                                //停电时每小时27min的33s更新
            {
                ucExtRtc = EXT_RTC_WORK;
            }
        }
        else
        {
            ucExtRtc = EXT_RTC_WORK;
        }
    }

    if((ucExtRtc == EXT_RTC_NO_WORK) && (ucSoftRtc == SOFT_RTC_MAIN))       //软时钟正确并没有到跟硬时钟比对是直接返回
    {
        gut_SysWarn.SysWarn.TimeError=false;
        return 1;                                                           //采用软时钟
    }

    GetExtRTC();                                                            //读外部时钟存放到RAM时钟的临时变量gs_ClkTmp之中
    if(RTCCheck((uint8*)&gs_ClkTmp) == Const_Data_OK)                       //外部时钟数据,BCD码判断,时间合法性判断是否有效
    {
        if(ucSoftRtc != SOFT_RTC_ERR)                                       //判断当前软时钟是否有效
        {
            if(CalcTimeGap(&gs_ClkTmp, &gs_DateTime) > MAX_CLOCK_BAK_GAP)   //当软硬时钟相差5S以上时，相信软时钟
            {
                ucExtRtc = EXT_RTC_INVALID;                                 //这种情况下信任软时钟
            }
            else
            {
                ucExtRtc = EXT_RTC_VALID;                                   //信任外部时钟
            }
        }
        else
        {
            ucExtRtc = EXT_RTC_VALID;                                         //软时钟数据无效，信任硬时钟
        }
        gs_SysRunSt.TimeFR=false;
    }
    else
    {                                                                       //外部时钟有误
        if(ucSoftRtc != SOFT_RTC_ERR)                                       //若软时钟数据合法
        {
            ucExtRtc = EXT_RTC_INVALID;                                     //则用软时钟覆盖硬时钟
            gs_SysRunSt.TimeFR=false;
        }
        else 
        {
            if(gs_SysRunSt.TimeFR==false);
            {
//      #ifdef FRCom
//            EVENTOUTHIGH();
//      #endif
              gs_SysRunSt.TimeFR=true;
//            gs_FREventOut.FREventOutStruct.TimeFR=true;
            }
            gut_SysWarn.SysWarn.TimeError=true;
        }
        //这里有一个else没有判断是下面恰好能使ucSoftRtc置为TIMERPROC_HARD
    }

    if(ucExtRtc == EXT_RTC_INVALID)                                         //硬时钟数据无效时
    {                                                                       //用软时钟覆盖硬时钟
        SetExtRTC();
        guc_ExtRtcErrCnt++;
    }
    else
    {                                                                       //硬时钟覆盖内部软时钟
        MemCpy((void*)&gs_DateTime, (void*)&gs_ClkTmp, 7);
        gs_DateTime.uiCRC = do_CRC((uint8*)(&gs_DateTime), sizeof(S_TIMEDATA) - 2);     //先对主时钟做crc校验
        if(gs_DateTime.ucMinute==0x00)                                              //每个小时写一次
        {
            ClockBAK_E2();                                                      
        }
        return 0;
    }
#endif
    return 1;
}
/*==============================================================================
Function: CalcWeek
;功能: 星期算法:2000为基准，+年+日+闰年数+（非闰年，1，2月+1）-1 ％7
;入口：year/month/day 当前的年月日
;返回：对应的周（0-6分别对应周日-周六）
;算法原理说明:一年为365日,365%7=1,恰好每一年多一天,因此从00年开始的年每年多一天,
;00年作为特例,需要查表计算,同时作为开头的年计算,其他的应该简单了吧,呵呵
==============================================================================*/
/**=============================================================================================\n
 * @function_name: CalcWeek
 * @function_file: AP_timeproc.c
 * @描述:计算星期,设置时钟的时候调用
 *
 *
 * @param:ucYear
 * @param:ucMonth
 * @param:ucDay
 *
 * @返回: uint8
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
uint8 CalcWeek(uint8 ucYear, uint8 ucMonth, uint8 ucDay)
{
    uint8 ucWeek;
    ucMonth = APIByteBCD2HEX(ucMonth);
    ucWeek = Tab_Week_2K[ucMonth];

    ucYear = APIByteBCD2HEX(ucYear);
    if((ucYear & 0x03) != 0 && ucMonth <= 0x02) // 非闰年前2月
    {
        ucWeek++;
    }

    ucWeek += ucYear;
    ucWeek += (ucYear >> 2); //闰年

    ucWeek += APIByteBCD2HEX(ucDay);

    ucWeek --;
    ucWeek %= 7;

    return ucWeek;
}

/*=========================================================================================\n
* @function_name: GetSysClock
* @function_file: Timeproc.c
* @描述: 1.读取系统时钟,这里仅设置主ram中的值,如果要读取外部时钟,则可以在另外的接口里面设置
*
*
* @参数:
* @param:pucBuf  用以读取的数据指针
* @param:ucType  指明读取哪几个数据,数据排列方式参考ucType的定义
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void GetSysClock(uint8* pucBuf, uint8 ucType)
{
    switch(ucType)
    {
    case Const_YYMMDDWW:
        *(pucBuf++) = gs_DateTime.ucWeek;
        //注意,这里不需要break
    case Const_YYMMDD:
        *(pucBuf++) = gs_DateTime.ucDay;
        *(pucBuf++) = gs_DateTime.ucMonth;
        *(pucBuf++) = gs_DateTime.ucYear;
        break;
    case Const_hhmmss:
        *(pucBuf++) = gs_DateTime.ucSecond;
        *(pucBuf++) = gs_DateTime.ucMinute;
        *(pucBuf++) = gs_DateTime.ucHour;
        break;
    case Const_YYMMDDhhmmss:
        *(pucBuf++) = gs_DateTime.ucSecond;
    case Const_YYMMDDhhmm:
        *(pucBuf++) = gs_DateTime.ucMinute;
        *(pucBuf++) = gs_DateTime.ucHour;
        *(pucBuf++) = gs_DateTime.ucDay;
        *(pucBuf++) = gs_DateTime.ucMonth;
        *(pucBuf++) = gs_DateTime.ucYear;
        break;
    case Const_hhmmssYYMMDDWW:
        *(pucBuf++) = gs_DateTime.ucSecond;
        *(pucBuf++) = gs_DateTime.ucMinute;
        *(pucBuf++) = gs_DateTime.ucHour;
        *(pucBuf++) = gs_DateTime.ucWeek;
        *(pucBuf++) = gs_DateTime.ucDay;
        *(pucBuf++) = gs_DateTime.ucMonth;
        *(pucBuf++) = gs_DateTime.ucYear;
        break;
    default:
        break;
    }
}
/**=============================================================================================\n
 * @function_name: SetSysClock
 * @function_file: AP_timeproc.c
 * @描述:设置系统时钟,这里仅设置主ram中的值,如果要设置外部时钟,则可以在另外的接口里面设置
 *
 *
 * @param:pucBuf  用以设置的数据指针
 * @param:ucType  指明设置哪几个数据,数据排列方式参考ucType的定义
 * @作者: lwb  (2010-2-5)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-5)\n
 ================================================================================================*/
void SetSysClock(uint8* pucBuf, uint8 ucType)
{
    switch(ucType)
    {
    case Const_YYMMDDWW:
        gs_DateTime.ucWeek = *(pucBuf++);
        //注意,这里不需要break
    case Const_YYMMDD:
        gs_DateTime.ucDay = *(pucBuf++);
        gs_DateTime.ucMonth = *(pucBuf++);
        gs_DateTime.ucYear = *(pucBuf++);
        gs_DateTime.ucWeek = CalcWeek(gs_DateTime.ucYear,gs_DateTime.ucMonth,gs_DateTime.ucDay);
        break;
    case Const_hhmmss:
        gs_DateTime.ucSecond = *(pucBuf++);
        gs_DateTime.ucMinute = *(pucBuf++);
        gs_DateTime.ucHour = *(pucBuf++);
        break;
    case Const_YYMMDDhhmmss:
        gs_DateTime.ucSecond = *(pucBuf++);
    case Const_YYMMDDhhmm:
        gs_DateTime.ucMinute = *(pucBuf++);
        gs_DateTime.ucHour = *(pucBuf++);
        gs_DateTime.ucDay = *(pucBuf++);
        gs_DateTime.ucMonth = *(pucBuf++);
        gs_DateTime.ucYear = *(pucBuf++);
        gs_DateTime.ucWeek = CalcWeek(gs_DateTime.ucYear,gs_DateTime.ucMonth,gs_DateTime.ucDay);
        break;
    case Const_hhmmssYYMMDDWW:
        gs_DateTime.ucSecond = *(pucBuf++);
        gs_DateTime.ucMinute = *(pucBuf++);
        gs_DateTime.ucHour = *(pucBuf++);
        gs_DateTime.ucWeek = *(pucBuf++);
        gs_DateTime.ucDay = *(pucBuf++);
        gs_DateTime.ucMonth = *(pucBuf++);
        gs_DateTime.ucYear = *(pucBuf++);
        gs_DateTime.ucWeek = CalcWeek(gs_DateTime.ucYear,gs_DateTime.ucMonth,gs_DateTime.ucDay);
        break;
    default:
        break;
    }
    ClockBAK_All();                             //设置ram、硬时钟和E2时钟
}
/**=============================================================================================\n
 * @function_name: GetYYMMDDhhmm
 * @function_file: AP_timeproc.c
 * @描述:获取年月日时分
 *
 *
 * @param:pucBuf
 * @作者: lwb  (2010-2-5)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-5)\n
 ================================================================================================*/
void GetYYMMDDhhmm(uint8* pucBuf)
{
    GetSysClock(pucBuf, Const_YYMMDDhhmm);
}
/*=========================================================================================\n
* @function_name: RTCCheck
* @function_file: Timeproc.c
* @描述: 检查日期时间数据是否符合格式
*
*
* @参数:
* @param:pucClk
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-02-29)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 RTCCheck(uint8* pucClk)
{
    if(DataCheck(7, pucClk, (S_DATACHK*)&gCs_ClkChkTab) == Const_Data_OK)   //时钟数据极限判断
    {
        return JudgeIsNum(pucClk, 7);                                       //时钟数据BCD码判断
    }
    return Const_Data_Err;
}
/**=============================================================================================\n
 * @function_name: ClockBAK_RAM
 * @function_file: AP_timeproc.c
 * @描述:备份RAM时钟
 *
 * @作者: lwb  (2010-2-5)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-5)\n
 ================================================================================================*/
void ClockBAK_RAM(void)
{
    gs_DateTime.uiCRC = do_CRC((uint8*)(&gs_DateTime), sizeof(S_TIMEDATA) - 2);     //先对主时钟做crc校验
    MemCpy((uint8*)(&gs_LastDateTime), (uint8*)(&gs_DateTime), sizeof(S_TIMEDATA)); //保存为上一秒数据
}
/**=============================================================================================\n
 * @function_name: ClockBAK_E2
 * @function_file: AP_timeproc.c
 * @描述:备份时钟数据
 *
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
void ClockBAK_E2(void)
{
    BE_WriteP(EEP_DATETIME,(uint8*)&gs_DateTime,sizeof(S_TIMEDATA));            //写主E2时钟
#if (_SW_SE_Type > 0)
    SE_WriteP(EEP_DATETIME,(uint8*)&gs_DateTime,sizeof(S_TIMEDATA));            //写备份E2时钟
#else
    BE_WriteP(EEP_DATETIME+EEP_BAK_LEN, (uint8*)&gs_DateTime, sizeof(S_TIMEDATA));
#endif
}
/**=============================================================================================\n
 * @function_name: ClockBAK_All
 * @function_file: AP_timeproc.c
 * @描述:备份时钟 RAM和E2
 *
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
void ClockBAK_All(void)
{//注意下面两个函数调用的先后关系
    ClockBAK_RAM();                     //先备份RAM时钟
    SetExtRTC();                        //设置硬时钟
    ClockBAK_E2();                      //在备份E2时钟
}
/**=============================================================================================\n
 * @function_name: YearMonthDayCheck
 * @function_file: AP_timeproc.c
 * @描述: 检测年月日  传入的参数为BCD格式,返回1正确,0错误
 *        不作参数是否BCD码判断
 *
 * @param:ucYear
 * @param:ucMonth
 * @param:ucDay
 *
 * @返回: uint8
 * @作者: lwb  (2010-2-4)\n
 *------------------------------------------------------------------------------------------------
 * @修改人: lwb (2010-2-4)\n
 ================================================================================================*/
uint8 YearMonthDayCheck(uint8 ucYear, uint8 ucMonth, uint8 ucDay)
{
    uint8 ucMaxDay = MonthDays[ucMonth];

    ucYear = APIByteBCD2HEX(ucYear);
    ucMonth = APIByteBCD2HEX(ucMonth);
    ucDay = APIByteBCD2HEX(ucDay);

    if((ucMonth == 2) && ((ucYear & 0x03) == 0))
    {//闰年同时是2月份
        ucMaxDay++;
    }
    if((ucDay == 0) || (ucDay > ucMaxDay))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*=========================================================================================\n
* @function_name: ValidTime
* @function_file: Timeproc.c
* @描述: 判断是否是合法的时间数据
*
*
* @参数:
* @param:time
* @param:type  0: 检查日期  1：检查时间 2：检查日期和时间 3: 检查日期和时间(数据带星期)
*
* @返回:
* @return: uint8
* @作者:   lwb (2012-03-01)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 ValidTime(uint8* time, uint8 type)
{
    uint8 offset;
    uint8 data1, day;
    uint8 uc_Cklen;
    offset=0;

    if(type==0)
    {
        uc_Cklen=3;
    }else if(type==1)
    {
        uc_Cklen=3;
    }else if(type==2)
    {
        uc_Cklen=6;
    }else
    {
        uc_Cklen=7;
    }

    if(JudgeIsNum(time,uc_Cklen)!=Const_Data_OK)
    {
        return Ret_Err;
    }

    if(type==1|| type == 2|| type==3)
    {
        // 秒 分 时
        //data1 = APIByteBCD2HEX(time[0]);        // 秒
        if(time[0] >= 0x60)
        {                                           //检查时间是否正确
            return Ret_Err;
        }

        //data1 = APIByteBCD2HEX(time[1]);        //分
        if(time[1] >= 0x60)
        {
            return Ret_Err;
        }

        //data1 = APIByteBCD2HEX(time[2]);        //时
        if(time[2] >= 0x24)
        {
            return Ret_Err;
        }
    }


    if(type == 0 || type == 2 || type == 3)
    {
        if(type==3)
        {
            offset=4;
        }
        else if(type == 2)
        {
            offset=3;
        }else
        {
            offset=0;
        }
        //日 月 年

        //data1 = APIByteBCD2HEX(time[1+offset]);        //检查月份是否正确
        if(/*(time[1+offset] == 0x00) ||*/ (time[1+offset] > 0x12))
        {
            return Ret_Err;
        }

        data1 = MonthDays[time[1+offset]];             //首先根据月来检查天是否正确
        day = APIByteBCD2HEX(time[0+offset]);          //日

        if((time[1+offset]== 2) && ((APIByteBCD2HEX(time[2+offset]) & 0x03) == 0))
        {                                           //闰年同时是2月份
            data1++;
        }

        if(/*(day == 0) || */(day > data1))
        {
            return Ret_Err;
        }
    } 
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: SleepTimeProc
* @function_file: Timeproc.c
* @描述: 唤醒走时
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SleepTimeProc(void)
{
    GetExtRTC();
    if(RTCCheck((uint8*)&gs_ClkTmp) != Const_Data_OK)                               //判断RTC时钟是否正确
    {
        //设置时钟错误标志位，提供上电处理流程
        guc_ClockErr=CLOCKERR;    
        if(RTCCheck((uint8*)&gs_DateTime) != Const_Data_OK)                         //不正确的话，判断RAM时钟是否正确
        {
            BE_ReadP(EEP_DATETIME,(uint8*)&gs_DateTime, sizeof(S_TIMEDATA));        //从E2中获取时间数据
        }

        if(RTCCheck((uint8*)&gs_DateTime) == Const_Data_OK)                         //判断新的RAM时钟是否正确,正确则设置外部时钟
        {
            SetExtRTC();
        }
    }else                                                                           //正确，拷贝RTC时钟到RAM时钟
    {
        MemCpy((uint8*)(&gs_DateTime), (uint8*)(&gs_ClkTmp), sizeof(S_TIMEDATA));
    }

    ClockBAK_RAM();                                                             //备份时钟
}
