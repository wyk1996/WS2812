#ifndef __TIMEPROC_H__
#define __TIMEPROC_H__

#ifndef TIMEPROC_EXT
    #define TIMEPROC_EXT extern
#else
    #define TIMEPROC_SELF 
#endif

#define _SW_EXT_RTC
#define CLOCKERR  0x55

#define Const_ClkBAK_RAM BIT0                               //ram时钟备份,一般每秒备份一次
#define Const_ClkBAK_E2  BIT1                               //E2中时钟备份,一般半小时备份一次
#define Const_ClkBAK (Const_ClkBAK_RAM + Const_ClkBAK_E2)   //所有时钟都备份一次
#define CLOCK_LEN   (sizeof(S_TIMEDATA))

#define DayMinCount     (1440)                              //每天的分钟数
#define HourSecCount    (3600)                              //每小时的秒数
#define QuartYearDay    (1461)                              //4年的天的数量
#define DaySecCount     (86400)                             //1天中秒的数量

#define MAX_CLOCK_BAK_GAP (300)                               //定义软硬时钟能覆盖最大相差的秒数 300s

enum
{
    SOFT_RTC_MAIN,                                          //软时钟ram部分至少有一个正确
    SOFT_RTC_E2,                                            //软时钟靠E2部分的来更新了
    SOFT_RTC_ERR,                                           //软时钟部分连E2备份都无效了
};

enum EXT_RTC_WORK_TYPE
{                                                           //在实际处理时,外部时钟的工作状态
    EXT_RTC_NO_WORK,
    EXT_RTC_WORK,
    EXT_RTC_VALID,
    EXT_RTC_INVALID,
};

enum CLOCKTYPE
{//下面的标记YY为年,MM为月,DD为日,WW为星期,hh为小时,mm为分,ss为秒,数据从右到左对应数据字节从低到高
    Const_YYMMDDWW,         //数据从低字节开始到高排列次序为星期,日,月,年
    Const_YYMMDD,           //日,月,年
    Const_hhmmss,           //秒,分,时
    Const_YYMMDDhhmmss,     //日,月,年,秒,分,时,
    Const_YYMMDDhhmm,       //分,时,日,月,年
    Const_hhmmssYYMMDDWW,   //星期,日,月,年,秒,分,时,
};

typedef struct strTimeData
{

    uint8   ucSecond;       //秒
    uint8   ucMinute;       //分
    uint8   ucHour;         //时
    uint8   ucDay;          //日
    uint8   ucMonth;        //月
    uint8   ucYear;         //年
    uint8   ucWeek;         //周

    uint16  uiCRC;          // CRC校验
} S_TIMEDATA;
TIMEPROC_EXT S_TIMEDATA gs_DateTime;        //当前时钟数据
TIMEPROC_EXT S_TIMEDATA gs_LastDateTime;    //上一秒时钟数据
TIMEPROC_EXT S_TIMEDATA gs_ClkTmp;          //时钟数据临时变量
TIMEPROC_EXT  uint8 guc_ExtRtcErrCnt;       //硬时钟错误次数
TIMEPROC_EXT  uint8 guc_ClockErr;

typedef struct 
{
    uint8   ucMinute;       //分
    uint8   ucHour;         //时
    uint8   ucDay;          //日
    uint8   ucMonth;        //月
    uint8   ucYear;         //年
} S_CURTIME;
TIMEPROC_EXT S_CURTIME gs_CurDateTime;        //当前时钟数据
TIMEPROC_EXT S_CURTIME gs_StartLoadCurve;     //负荷记录起始时间
TIMEPROC_EXT uint32 gul_LoadCurveMin;         //当前分钟数
TIMEPROC_EXT uint8 guc_LCTimeFlag;                  //起始时间设置标志

#ifdef TIMEPROC_SELF
const S_DATACHK  gCs_ClkChkTab[] =
{

    {0, 0x59,},  //0x00, ucSecond
    {0, 0x59,},  //0x01, ucMinute
    {0, 0x23,},  //0x02, ucHour
    {1, 0x31,},  //0x03, ucDay
    {1, 0x12,},  //0x04, ucMonth
    {0, 0x99,},  //0x05, ucYear
    {0, 0x06,},  //0x06, week,这里比较特殊
};

const uint8 Tab_Week_2K[] =
//0,    1,  2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12     月份,为了查表方便,第一个为无效值
{0x00,0x06,0x02,0x03,0x06,0x01,0x04,0x06,0x02,0x05,0x00,0x03,0x05};  //2000年每个月的第一天是星期几的排布表

const uint8 MonthDays[] =
{
    0,                                          // 0x00 (无效)
    31, 28, 31, 30, 31, 30, 31, 31, 30,         // 0x01 - 0x09
    0, 0, 0, 0, 0, 0,                           // 0x0A - 0x0F (无效)
    31, 30, 31,                                 // 0x10 - 0x12
};

// 每月其始的相对天数
const uint16 gui_MonthBeginDay[12] = 
{0, 31, 59, 90,120, 151, 181, 212, 243, 273, 304, 334};
#else
extern const uint8  MonthDays[];
extern const S_DATACHK  gCs_ClkChkTab[];
#endif


void TimerProc(void);                           //软时钟走时函数，每秒调用
void GetSysClock(uint8* pucBuf, uint8 ucType);  //获取系统时钟
void SetSysClock(uint8* pucBuf, uint8 ucType);  //设置系统时钟
void ClockBAK_RAM(void);                        //备份RAM时钟
void ClockBAK_E2(void);                         //备份E2时钟
void ClockBAK_All(void);                        //备份RAM和E2时钟
void GetYYMMDDhhmm(uint8* pucBuf);              //获取YMDHM数据

uint8 CalcWeek(uint8 year, uint8 month, uint8 day);               //设置时间时，计算星期时调用
uint8 YearMonthDayCheck(uint8 ucYear, uint8 ucMonth, uint8 ucDay);//设置时钟时，检查设置的时钟是否符合格式

uint8 RTCCheck(uint8* pucClk);
uint8 CheckTime(void);
uint32 CalcAbsSecond(S_TIMEDATA* psTm);
uint32 CalcAbsTime(S_TIMEDATA* psTm);
uint16 CalcAbsDay(S_TIMEDATA* psTm);
uint32 CalcTimeGap(S_TIMEDATA* psTm1, S_TIMEDATA* psTm2);
uint8 ValidTime(uint8* time, uint8 type);
void SleepTimeProc(void);

#endif  


