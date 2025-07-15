 
#ifndef _AP_DATAPROC_H_
#define _AP_DATAPROC_H_

#ifndef AP_DATAPROC_EXT
    #define AP_DATAPROC_EXT extern
#else 
    #define AP_DATASLEF
#endif

typedef struct 
{
    uint8 PluseType:1;      //0 RTC 1 Rate Switch pluse
    uint8 STAFLAG:1;
    uint8 OpenGapFRFlag:1;
    uint8 PDirFR:1;
    uint8 TimeFR:1;
    uint8 undefine5:1;
    uint8 undefine6:1;
    uint8 undefine7:1;
}GS_SYSRUNST;


AP_DATAPROC_EXT GS_SYSRUNST gs_SysRunSt;



typedef struct  
{
    uint8 OpenGapFRFlag:1;//0 RTC 1 Rate Switch pluse
    uint8 PDirFR:1;
    uint8 TimeFR:1;
    uint8 undefine3:1;
    uint8 undefine4:1;
    uint8 undefine5:1;
    uint8 undefine6:1;
    uint8 undefine7:1;
}GS_FREVENTOUT;

union FREventOutUnion
{
  GS_FREVENTOUT FREventOutStruct;
  uint8 byte;
};

AP_DATAPROC_EXT union FREventOutUnion gs_FREventOut;

typedef struct 
{
    uint16 LoopError:1;      //回路错误 ERR-01
    uint16 EsamError:1;      //ESAM错误，主要指复位不成功 ERR-02
    uint16 BatLow:1;         //电池电压低
    uint16 EEPROMERROR:1;    //EEPROM 错误 指时序错误，校验和不再此列 ERR-06
    uint16 TimeError:1;      //TIMER ERROR
    uint16 MACError:1;       //MAC ERROR ERR-10
    uint16 EsamKeyError:1;   //密匙更新错误
    uint16 EsamJMError:1;    //密文解码错误
    uint16 REVPOWER:1;       //功率反向
}GS_SYSWARTN;
 
typedef union 
{
    GS_SYSWARTN SysWarn;
    uint16 byte;
}GT_SYSWARUN;

AP_DATAPROC_EXT GT_SYSWARUN  gut_SysWarn;
AP_DATAPROC_EXT uint8 XDATA_RAMZONE[256];
AP_DATAPROC_EXT uint8 XDATA_RAMZONE1[256];

AP_DATAPROC_EXT uint8 XDATA_RAMZONE2[256];
AP_DATAPROC_EXT uint16 XDATA_RAMZONE3[256];

struct RunningSysStruct 
{
    uint8 PluseType:1;          //0 RTC 1 Rate Switch pluse
    uint8 Staflag:1;
    uint8 OpenGapFRFlag:1;
    uint8 PDirFR:1;
    uint8 TimeFR:1;
    uint8 undefine5:1;
    uint8 undefine6:1;
    uint8 undefine7:1;
};


/*******************************************************************************
**电表运行状态字1
*******************************************************************************/
//状态运行字1
typedef struct
{
       uint16    undefine0:1;       //保留
       uint16    DemandType:1;      //需量计算方式
       uint16    RTCCell:1;         //RTC电池
       uint16    MeterCell:1;       //抄表电池
       uint16    PDir:1;            //功率方向
       uint16    CDir:1;            //无功方向
       uint16    undefine6:1;       //保留
       uint16    undefine7:1;       //保留
       uint16    undefine8:1;       //保留
       uint16    undefine9:1;       //保留
       uint16    undefine10:1;      //保留
       uint16    undefine11:1;      //保留
       uint16    undefine12:1;      //保留
       uint16    undefine13:1;      //保留
       uint16    undefine14:1;      //保留
       uint16    undefine15:1;      //保留
}GS_RUNCODE1;

typedef union 
{
    Word16 Word;
    GS_RUNCODE1 RunCode1Word;
}GT_RUNCODE1;


/*******************************************************************************
**电表运行状态字2
*******************************************************************************/
//状态运行字2
typedef struct  
{
       uint16    PADIR:1;           //A相有功功率方向
       uint16    PBDIR:1;           //B相有功功率方向
       uint16    PCDIR:1;           //C相有功功率方向
       uint16    MeterCell:1;       //抄表电池
       uint16    undefine4:1;       //功率方向
       uint16    CADir:1;           //无功方向
       uint16    CBDir:1;           //保留
       uint16    CCDir:1;           //保留
       uint16    undefine8:1;       //保留
       uint16    undefine9:1;       //保留
       uint16    undefine10:1;      //保留
       uint16    undefine11:1;      //保留
       uint16    undefine12:1;      //保留
       uint16    undefine13:1;      //保留
       uint16    undefine14:1;      //保留
       uint16    undefine15:1;      //保留
}GS_RUNCODE2;

typedef union 
{
    Word16 Word;
    GS_RUNCODE2 RunCode2Word;
}GT_RUNCODE2;


/*******************************************************************************
**电表运行状态字3
*******************************************************************************/
//状态运行字3
typedef struct 
{
       uint16    RateTable:1;       //当前运行时段:0第一套/1第二套
       uint16    PowerState0:1;     //供电方式:00主电源/01辅助电源/10电池供电
       uint16    PowerState1:1;     //供电方式
       uint16    Program:1;         //编程允许:0禁止/1许可
       uint16    LoadSwitch:1;      //继电器通断:0通/1断
       uint16    TimeZoneTable:1;   //当前运行时区:0第一套/1第二套
       uint16    LoadSwitchCmd:1;   //0 关闭 1 打开
       uint16    LOWChargeCmd:1;    //0 没有报警 1 报警
       uint16    undefine8:1;       //保留
       uint16    undefine9:1;       //保留
       uint16    undefine10:1;      //保留
       uint16    undefine11:1;      //保留
       uint16    undefine12:1;      //保留
       uint16    undefine13:1;      //保留
       uint16    undefine14:1;      //保留
       uint16    undefine15:1;      //保留
}GS_RUNCODE3;

typedef union 
{
    Word16 Word;
    GS_RUNCODE3 RunCode3Word;
}GT_RUNCODE3;
/*******************************************************************************
**电表运行状态字4(A相)
*******************************************************************************/
//状态运行字4
typedef struct
{
       uint16    LossOfvoltage:1;       //失压:0正常/1失压
       uint16    UnderVoltage:1;        //欠压:0正常//1欠压
       uint16    OverVoltage:1;         //过压:0正常//1过压
       uint16    LossOfCurrent:1;       //失流:0正向/1失流
       uint16    OverCurrent:1;         //过流:0正向/1过流
       uint16    OverPower:1;           //过载:0正向/1过载
       uint16    PowerDIR:1;            //潮流反向:0正向/1反向
       uint16    undefine7:1;           //断相:0正向/1断相
       uint16    undefine8:1;           //断流:0正向/1断相
       uint16    undefine9:1;           //保留
       uint16    undefine10:1;          //保留
       uint16    undefine11:1;          //保留
       uint16    undefine12:1;          //保留
       uint16    undefine13:1;          //保留
       uint16    undefine14:1;          //保留
       uint16    undefine15:1;          //保留
}GS_RUNCODE4;

typedef union
{
    Word16 Word;
    GS_RUNCODE4 RunCode4Word;
}GT_RUNCODE4;

/*******************************************************************************
**电表运行状态字5(B相)
*******************************************************************************/
//状态运行字5
typedef struct  
{
       uint16    LossOfvoltage:1;       //失压:0正常/1失压
       uint16    UnderVoltage:1;        //欠压:0正常/1欠压
       uint16    OverVoltage:1;         //过压:0正常/1过压
       uint16    LossOfCurrent:1;       //失流:0正常/1失流
       uint16    OverCurrent:1;         //过流:0正常/1过流
       uint16    OverPower:1;           //过载:0正常/1过载
       uint16    PowerDIR:1;            //潮流反向:0正向/1反向
       uint16    undefine7:1;           //断相:0正常/1断相
       uint16    undefine8:1;           //断流:0正常/1断相
       uint16    undefine9:1;           //保留
       uint16    undefine10:1;          //保留
       uint16    undefine11:1;          //保留
       uint16    undefine12:1;          //保留
       uint16    undefine13:1;          //保留
       uint16    undefine14:1;          //保留
       uint16    undefine15:1;          //保留
}GS_RUNCODE5;

typedef union
{
    Word16 Word;
    GS_RUNCODE5 RunCode5Word;
}GT_RUNCODE5;

/*******************************************************************************
**电表运行状态字6（C相）
*******************************************************************************/
//状态运行字6
typedef struct
{
       uint16    LossOfvoltage:1;       //失压:0正常/1失压
       uint16    UnderVoltage:1;        //欠压:0正常/1欠压
       uint16    OverVoltage:1;         //过压:0正常/1过压
       uint16    LossOfCurrent:1;       //失流:0正常/1失流
       uint16    OverCurrent:1;         //过流:0正常/1过流
       uint16    OverPower:1;           //过载:0正常/1过载
       uint16    PowerDIR:1;            //潮流反向:0正向/1反向
       uint16    undefine7:1;           //断相:0正常/1断相
       uint16    undefine8:1;           //断流:0正常/1断相
       uint16    undefine9:1;           //保留
       uint16    undefine10:1;          //保留
       uint16    undefine11:1;          //保留
       uint16    undefine12:1;          //保留
       uint16    undefine13:1;          //保留
       uint16    undefine14:1;          //保留
       uint16    undefine15:1;          //保留
}GS_RUNCODE6;

typedef union
{
    Word16 Word;
    GS_RUNCODE6 RunCode6Word;
}GT_RUNCODE6;
/*******************************************************************************
**电表运行状态字7(合相故障)
*******************************************************************************/

//状态运行字7
typedef struct  
{
       uint16    undefine0:1;       //电压逆相序:0正常/1发生
       uint16    undefine1:1;       //电流逆相序:0正常/1发生
       uint16    undefine2:1;       //电压不平衡:0正常/1发生
       uint16    undefine3:1;       //电流不平衡:0正常/1发生
       uint16    undefine4:1;       //辅助失压:0正常/1发生
       uint16    undefine5:1;       //掉电:0正常/1发生
       uint16    undefine6:1;       //需量超限:0正常/1发生
       uint16    undefine7:1;       //保留
       uint16    undefine8:1;       //保留
       uint16    OpenCover:1;       //保留
       uint16    undefine10:1;      //保留
       uint16    undefine11:1;      //保留
       uint16    undefine12:1;      //保留
       uint16    undefine13:1;      //保留
       uint16    undefine14:1;      //保留
       uint16    undefine15:1;      //保留
}GS_RUNCODE7;

typedef union
{
    Word16 Word;
    GS_RUNCODE7 RunCode7Word;
}GT_RUNCODE7;

/*******************************************************************************
**电表运行状态字定义
*******************************************************************************/
typedef struct 
{
    GT_RUNCODE1 ui_MeterState1;                  //电表运行状态字1
    GT_RUNCODE2 ui_MeterState2;                  //电表运行状态字2
    GT_RUNCODE3 ui_MeterState3;                  //电表运行状态字3
    GT_RUNCODE4 ui_MeterState4;                  //电表运行状态字4
    GT_RUNCODE5 ui_MeterState5;                  //电表运行状态字5
    GT_RUNCODE6 ui_MeterState6;                  //电表运行状态字6
    GT_RUNCODE7 ui_MeterState7;                  //电表运行状态字7
} METERSTATE;
AP_DATAPROC_EXT METERSTATE gs_MeterState;

/*******************************************************************************
**电表运行特征字1
*******************************************************************************/
typedef struct  
{
       uint8    undefine0:1;       //外置开关控制方式:0电平/1脉冲
       uint8    LcdshowMode:1;     //液晶圈1圈2字样意义:0显示1、2套时段/1显示1、2套费率
       uint8    ReportMode:1;      //主动上报模式:0不启用后续标志/1启用后续标志
       uint8    undefine3:1;       //保留
       uint8    undefine4:1;       //保留
       uint8    undefine5:1;       //保留
       uint8    undefine6:1;       //保留
       uint8    undefine7:1;       //保留
}GS_RUNMODE;

AP_DATAPROC_EXT GS_RUNMODE gs_MeterMode;

/*******************************************************************************
**主动上报模式字
*******************************************************************************/

//上报模式字1
typedef struct  
{
       uint16    LoopErrRpt:1;      //控制回路错误
       uint16    EasmErrRpt:1;      //EASM错误
       uint16    undefine2:1;       //内卡初始化错误
       uint16    RtcBatRpt:1;       //时钟电池电压低
       uint16    FlashErrRpt:1;     //内部程序错误
       uint16    EepErrRpt:1;       //存储器故障或损坏
       uint16    undefine6:1;       //保留
       uint16    RtcErrRpt:1;       //时钟故障
       uint16    CutBatRpt:1;      //停电抄表电池欠压
       uint16    undefine9:1;       //透支状态
       uint16    CoverRpt:1;        //开表盖
       uint16    undefine11:1;       //开端钮盖
       uint16    undefine12:1;       //保留
       uint16    undefine13:1;       //保留
       uint16    undefine14:1;       //保留
       uint16    undefine15:1;       //保留
}GS_REPORTWORD1;

typedef union
{
    Word16 Word;
    GS_REPORTWORD1 ReportWord1;
}GT_REPORTWORD1;

//上报模式字2
typedef struct  
{
       uint16    undefine0:1;       //失压
       uint16    undefine1:1;       //欠压
       uint16    undefine2:1;       //过压
       uint16    undefine3:1;       //失流
       uint16    undefine4:1;       //过流
       uint16    undefine5:1;       //过载
       uint16    PDirRpt:1;         //功率反向
       uint16    undefine7:1;       //断相
       uint16    undefine8:1;       //断流
       uint16    undefine9:1;       //保留
       uint16    undefine10:1;       //保留
       uint16    undefine11:1;       //保留
       uint16    undefine12:1;       //保留
       uint16    undefine13:1;       //保留
       uint16    undefine14:1;       //保留
       uint16    undefine15:1;       //保留
}GS_REPORTWORD2;

typedef union
{
    Word16 Word;
    GS_REPORTWORD2 ReportWord2;
}GT_REPORTWORD2;

//上报模式字3
typedef struct  
{
       uint16    undefine0:1;       //电压逆相序
       uint16    undefine1:1;       //电流逆相序
       uint16    undefine2:1;       //电压不平衡
       uint16    undefine3:1;       //电流不平衡
       uint16    undefine4:1;       //保留
       uint16    undefine5:1;       //掉电
       uint16    undefine6:1;       //需量超限
       uint16    undefine7:1;       //总功率因数超下限
       uint16    undefine8:1;       //电流严重不平衡
       uint16    PDirRpt:1;         //总有功功率反向
       uint16    undefine10:1;       //全失压
       uint16    undefine11:1;       //保留
       uint16    undefine12:1;       //保留
       uint16    undefine13:1;       //保留
       uint16    undefine14:1;       //保留
       uint16    undefine15:1;       //保留
}GS_REPORTWORD3;

typedef union
{
    Word16 Word;
    GS_REPORTWORD3 ReportWord3;
}GT_REPORTWORD3;

/*******************************************************************************
**主动上报模式字
*******************************************************************************/
typedef struct 
{
    GT_REPORTWORD1 ui_ReportWord1;                  //主动上报模式字1
    GT_REPORTWORD2 ui_ReportWord2;                  //主动上报模式字2
    GT_REPORTWORD3 ui_ReportWord3;                  //主动上报模式字3
}REPORTWORD;

AP_DATAPROC_EXT REPORTWORD gs_ReportWord;

/*******************************************************************************
**主动上报状态字
*******************************************************************************/
typedef struct 
{
    GT_REPORTWORD1 ui_ReportState1;                  //主动上报模式字1
    GT_REPORTWORD2 ui_ReportState2A;                 //主动上报模式字2
    GT_REPORTWORD2 ui_ReportState2B;                 //主动上报模式字2
    GT_REPORTWORD2 ui_ReportState2C;                 //主动上报模式字2
    GT_REPORTWORD3 ui_ReportState3;                  //主动上报模式字3
}REPORTSTATE;

AP_DATAPROC_EXT REPORTSTATE gs_ReportState;
AP_DATAPROC_EXT REPORTSTATE gs_ReportStateFlg;
AP_DATAPROC_EXT uint16 gui_ReportCRC;
AP_DATAPROC_EXT uint32 gul_CoverUpCnt;

enum
{
    LSDLID=0,     //历史电量      0x00
    DSDJID,       //定时冻结      0x01
    SSDJID,       //瞬时冻结      0x02
    SQQHDJID,     //时区切换冻结  0x03
    RSDQHDJID,    //时段切换冻结  0x04
    RDJID,        //日冻结        0x05    
    ZDDJID,       //整点冻结      0x06
    DBQLJLID,     //电表清零记录  0x07
    SJQLJLID,     //事件清零记录  0x08
    JSJLID,       //校时记录      0x09
    BCJLID,       //编程记录      0x0a
    DDJLID,       //掉电记录      0x0b
    KBGJLID,      //开表盖记录    0x0c
    DYGZJLID,     //电源故障记录  0x0d
    JSSJJLID,     //结算时间记录  0x0e

    ZFHJLID,      //总负荷记录次数    0x0f
    FH1JLID,      //第1类负荷记录次数 0x10
    FH2JLID,      //第2类负荷记录次数 0x11
    FH3JLID,      //第3类负荷记录次数 0x12
    FH4JLID,      //第4类负荷记录次数 0x13
    LZJLID,       //拉闸记录      0x14
    HZJLID,       //合闸记录      0x15
    MYGXJLID,     //密钥跟新记录  0x16
    FHKGJLID,     //负荷开关误动作记录0x17
#ifdef ZXRPRLOG
    ZXRJLID,      //周休日记录    0x18
#endif

#ifdef YGZHZPRLOG
    YGZHJLID,     //有功组合记录  0x19
#endif

#ifdef JSRPRLOG
    JSRJLID,      //结算日记录    0x1A
#endif
};

enum
{
    DBQLNUMID=0,    //电表清零次数      0x00
    SJQLNUMID,      //事件清零次数      0x01
    JSNUMID,        //校时次数          0x02
    BCNUMID,        //编程次数          0x03
    DDNUMID,        //掉电次数          0x04
    DSDJNUMID,      //定时冻结次数      0x05
    SSDJNUMID,      //瞬时冻结次数      0x06
    SQQHDJNUMID,    //时区切换冻结次数  0x07
    RSDQHDJNUMID,   //时段切换冻结 次数 0x08
    RDJNUMID,       //日冻结次数        0x09    
    ZDDJNUMID,      //整点冻结次数      0x0a
    KBGNUMID,       //开表盖次数        0x0b
    DYGZNUMID,      //电源故障次数      0x0c
    ZFHNUMID,       //总负荷记录次数    0x0d
    FH1NUMID,       //第1类负荷记录次数 0x0e
    FH2NUMID,       //第2类负荷记录次数 0x0f
    FH3NUMID,       //第3类负荷记录次数 0x10
    FH4NUMID,       //第4类负荷记录次数 0x11

    LZNUMID,        //拉闸次数          0x12
    HZNUMID,        //合闸次数          0x13
    MYGXNUMID,      //密钥跟新次数      0x14
    FUKGNUMID,      //负荷开关误动作次数0x15

#ifdef SQSDPRLOG
    RSDNUMID,       //日时段表编程次数  0x16
    SQNUMID,        //时区表编程次数    0x17
#endif

#ifdef ZXRPRLOG
    ZXRNUMID,       //周休日编程次数    0x18
#endif

#ifdef YGZHZPRLOG
    YGZHNUMID,      //有功组合方式编程次数 0x19
#endif

#ifdef JSRPRLOG
    JSRNUMID,       //结算日编程次数    0x1A
#endif
    PRNUMCNT,
};
enum
{//卷绕表2的指针在写数据时,需要卷绕移动的方式
    R2_NO_MOVE=0,     //不需要移动
    R2_MOVE,        //需要卷绕移动一格
};


typedef struct s_evcntadd
{
    uint8   ucLen;      //E2中数据保存的字节数,这个值最大不应该超过4,对应一个unsigned long
    uint16  uiAddr;     //E2中存放的地址
}S_EVCNTADD;



typedef struct s_round2
{//卷绕数据内容类型2
    uint8   ucLen;      //数据项长度
    uint16  ucMod;      //数据块总数,用于求模
    uint16  uiPtAddr;   //指针地址,用于指向当前数据块
    uint16  uiDataAddr; //所有数据内容起始地址
}S_ROUND2;

typedef struct 
{
    uint16 uiE2Addr;
    uint8  ucPageCnt;
}S_E2CLRCTR;

enum 
{
    ClrTypeNum = 11,     //基本清零数
#ifdef SQSDPRLOG
    ClrType_RSDBCNum,   //清日时段清零数
    ClrType_SQBCNum,    //清时区清零数
#endif

#ifdef ZXRPRLOG
    ClrType_ZXRBCNum,   //清周休日清零数
#endif

#ifdef YGZHZPRLOG
    ClrType_YGZHBCNum, //清有功组合方式清零数
#endif

#ifdef JSRPRLOG
    ClrType_JSRBCNum,   //清结算日清零数
#endif
    ClrType_AllNum,   //清所有记录
};

enum 
{
  DjClrType_BaseNum=5,
#ifdef LOADCURVE
  CurveClr_Num,
#endif
  DjClrType_AllNum, 
};

#ifdef AP_DATASLEF
const S_ROUND2 gCs_Round2Tab[] =
{//本数据块不关心HEX和BCD之间的转换和小数点位数的转换,请务必保证数据可以直接读取就可以使用

    {60,    12, EEP_EPHIS_PT,   EEP_EPHIS   },  //历史电量      0x00
    {49,    60, EEP_DSDJHIS_PT, EEP_DSDJLHIS},  //定时冻结      0x01
    {49,    3,  EEP_SSDJHIS_PT, EEP_SSDJLHIS},  //瞬时冻结      0x02
    {49,    2,  EEP_SQDJHIS_PT, EEP_SQDJLHIS},  //时区切换冻结  0x03
    {49,    2,  EEP_RSDJHIS_PT, EEP_RSDJLHIS},  //时段切换冻结  0x04
    {49,    62, EEP_RDJHIS_PT,  EEP_RDJLHIS },  //日冻结        0x05    
    {13,    254,EEP_ZDDJHIS_PT, EEP_ZDDJLHIS},  //整点冻结      0x06
    {18,    10, EEP_DBQLET_PT,  EEP_DBQLET  },  //电表清零记录  0x07
    {14,    10, EEP_SJQLET_PT,  EEP_SJQLET  },  //事件清零记录  0x08
    {16,    10, EEP_JSET_PT,    EEP_JSET    },  //校时记录      0x09
    {50,    10, EEP_BCET_PT,    EEP_BCET    },  //编程记录      0x0a
    {12,    10, EEP_DDET_PT,    EEP_DDET    },  //掉电记录      0x0b
    {28,    10, EEP_KBGET_PT,   EEP_KBGET   },  //开表盖记录    0x0c
    {20,    10, EEP_DYYC_PT,    EEP_DYYC    },  //电源故障记录                      0x0d
    {4,     12, EEP_JSTIME_PT,  EEP_JSTIME  },  //结算时间记录                      0x0e
#ifdef LOADCURVE 
    {11,    CurveTotalCnt,  EEP_ZFHET_PT,   EEP_ZFHET   },  //总负荷记录            0x0f
    {12,    CurveTotalCnt,  EEP_FHET1_PT,   EEP_FHET1   },  //第1类负荷记录         0x10
    {8,     CurveTotalCnt,  EEP_FHET2_PT,   EEP_FHET2   },  //第2类负荷记录         0x11
    {7,     CurveTotalCnt,  EEP_FHET3_PT,   EEP_FHET3   },  //第3类负荷记录         0x12
    {13,    CurveTotalCnt,  EEP_FHET4_PT,   EEP_FHET4   },  //第4类负荷记录         0x13
#endif
    {18,    10,             EEP_LZET_PT,    EEP_LZET    },  //拉闸记录              0x14
    {18,    10,             EEP_HZET_PT,    EEP_HZET    },  //合闸记录              0x15
    {15,    2,              EEP_MYGX_PT,    EEP_MYGXET  },  //密钥跟新记录          0x16
    {29,    10,             EEP_FHKG_PT,    EEP_FHKGET  },  //负荷开关误动作记录    0x17
#ifdef ZXRPRLOG
    {11,    10,             EEP_ZXRXGJL_PT, EEP_ZXRXGJL },  //周休日记录            0x18
#endif

#ifdef YGZHZPRLOG
    {11,    10,             EEP_YGZHET_PT,  EEP_YGZHET  },  //有功组合记录  0x19
#endif

#ifdef JSRPRLOG
    {16,    10,             EEP_JSRET_PT,   EEP_JSRET   },  //结算日记录    0x1A
#endif
};

const  S_EVCNTADD gCs_EvCntAddTab[] =
{
     {3,    EEP_DBQLETNO    },  //电表清零次数      0x00
     {3,    EEP_SJQLETNO    },  //事件清零次数      0x01
     {3,    EEP_JSETNO      },  //校时记录          0x02
     {3,    EEP_BCETNO      },  //编程记录          0x03
     {3,    EEP_DDETNO      },  //掉电记录          0x04
     {3,    EEP_DSDJHISNO   },  //定时冻结次数      0x05
     {3,    EEP_SSDJHISNO   },  //瞬时冻结次数      0x06
     {3,    EEP_SQDJHISNO   },  //时区切换冻结次数  0x07
     {3,    EEP_RSDJHISNO   },  //时段切换冻结 次数 0x08
     {3,    EEP_RDJHISNO    },  //日冻结次数        0x09    
     {3,    EEP_ZDDJHISNO   },  //整点冻结次数      0x0a
     {3,    EEP_KBGETNO     },  //开表盖记录        0x0b
     {3,    EEP_DYYCNO      },  //电源故障记录      0x0c
#ifdef LOADCURVE    
     {3,    EEP_ZFHETNO     },  //总负荷记录        0x0d
     {3,    EEP_FHET1NO     },  //第1类负荷记录     0x0e
     {3,    EEP_FHET2NO     },  //第2类负荷记录     0x0f
     {3,    EEP_FHET3NO     },  //第3类负荷记录     0x10
     {3,    EEP_FHET4NO     },  //第4类负荷记录     0x11
#endif   
     {3,    EEP_LZETNO      },  //拉闸记录          0x12
     {3,    EEP_HZETNO      },  //合闸记录          0x13
     {3,    EEP_MYGXNO      },  //密钥更新记录      0x14
     {3,    EEP_FHKGNO      },  //负荷开关误动作记录0x14
     
#ifdef SQSDPRLOG
     {3,    EEP_RSDXGNO     },  //日时段表编程次数  0x15
     {3,    EEP_SQXGNO      },  //时区表编程次数    0x16
#endif

#ifdef ZXRPRLOG
     {3,    EEP_ZXRXGNO     },  //周休日编程次数    0x17
#endif

#ifdef YGZHZPRLOG
     {3,    EEP_YGZHETNO    },  //有功组合方式编程次数 0x18
#endif

#ifdef JSRPRLOG
     {3,    EEP_JSRETNO     },  //结算日编程次数    0x19     
#endif
};


const  S_E2CLRCTR gCs_MeterClearAddrTab[ClrType_AllNum]=
{

    {EEP_DBQLETNO,  3,},    //电表清零
    {EEP_SJQLETNO,  3,},    //事件清零
    {EEP_JSTIME_PT, 1,},    //结算时间记录清零

    {EEP_JSETNO,    3,},    //校时清零
    {EEP_BCETNO,    8,},    //编程清零
    {EEP_DDETNO,    2,},    //掉电清零
    {EEP_KBGETNO,   5,},    //开表盖清零
    {EEP_DYYCNO,    4,},    //电源故障清零

    {EEP_LZETNO,    3,},    //拉闸清零
    {EEP_HZETNO,    3,},    //合闸清零
    {EEP_MYGXNO,    3,},    //密钥跟新清零
    {EEP_FHKGNO,    5,},    //负荷开关清零

#ifdef SQSDPRLOG    
    {EEP_RSDXGNO,   9,},    //日时段表清零
    {EEP_SQXGNO,    9,},    //时区表清零
#endif

#ifdef ZXRPRLOG
    {EEP_ZXRXGNO,   2,},    //周休日清零
#endif

#ifdef YGZHZPRLOG
    {EEP_YGZHETNO,  2,},    //有功组合方式清零
#endif

#ifdef JSRPRLOG
    {EEP_JSRETNO,   3,},    //结算日清零
#endif
};

const  S_E2CLRCTR gCs_DJClearAddrTab[DjClrType_AllNum]=
{
    {EEP_SSDJHIS_PT,  3,},    //瞬时冻结清零
    {EEP_SQDJHIS_PT,  2,},    //时区表冻结清零
    {EEP_RSDJHIS_PT,  2,},    //时段表冻结清零
    {EEP_RDJHIS_PT,  48,},    //日冻结清零
    {EEP_ZDDJHIS_PT, 52,},    //整点冻结清零
    {EEP_DSDJHIS_PT, 46,},    //定时冻结清零
#ifdef LOADCURVE     
    {EEP_ZFHETNO,   233,},    //负荷曲线清零
#endif
};

#else
extern const S_ROUND2  gCs_Round2Tab[];
extern const S_EVCNTADD  gCs_EvCntAddTab[];
extern const S_E2CLRCTR  gCs_MeterClearAddrTab[ClrType_AllNum];
extern const  S_E2CLRCTR  gCs_DJClearAddrTab[DjClrType_AllNum];
#endif

#define MAX_DayDJ  7                     //最大上电补日冻结次数

AP_DATAPROC_EXT uint8  guc_InitWd[10];   //初始化状态字
AP_DATAPROC_EXT uint8  guc_ClearWd[10];  //电表清零状态字
AP_DATAPROC_EXT uint32 gul_PowerTm;      //停电时间
 
void Data_ClearMeter(void);
void Data_HourProc(void);
void Data_MinProc(void);
void Data_DayProc(void);
void DataProcWriteRound2(uint8 ucRound, uint8 ucPos, uint8* pucBuf);
void DataProcRead2Slice(uint8 ucR2Pos, uint8 ucCnt, uint8 ucOffset, uint8 ucLen, uint8* pucBuf);
void DataProcEvCountAdd(uint8 ucNo);
uint32 RdEvCount(uint8 ucNo);
void RecoverPara(void);
void DjProc(void);
void SetCRCWord(uint8 *WrodSt);
void ClRCRCWord(uint8 *WrodSt);
uint8 CheckCRCWord(uint8 *WrodSt);
void CalPowDwTm(void);
void RecoverJSRData(void);
void RecoverDayDjData(void);
void RecoverEnegyData(void);
void StructFreezeDate(void);
void RecoverZDFrez(void);
void YearJSProc(void);
void WriLastDJTime(S_CURTIME *LastCbrTm,uint8* time,uint8* pucBuf);
void CalDayBack(S_CURTIME *CurTime,uint8 offset);
uint8 JudgeJSR(uint8* CurTime,uint8* JSRTime,uint8 flg);
uint8 JudgeCurTime(uint8* CurTime,uint8* RdjTime);


#endif
