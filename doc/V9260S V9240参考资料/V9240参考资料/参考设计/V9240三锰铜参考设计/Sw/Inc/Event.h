 
#ifndef _AP_EVENT_H_
#define _AP_EVENT_H_

#ifndef AP_EVENT_EXT
#define AP_EVENT_EXT extern
#endif


/*******************************************************************************
**枚举冻结参数的类型
*******************************************************************************/
typedef enum 
{
    E_HourFreeze = 0,                           //小时冻结
    E_DayFreeze,                                //天冻结
    E_MonthFreeze,                              //月冻结
    E_DSFreeze,                                 //定时冻结
    E_SQFreeze,                                 //时区表切换冻结
    E_SDFreeze,                                 //日时段表切换冻结
}e_TypeOfFrez;

AP_EVENT_EXT e_TypeOfFrez ge_TypeOfFrezP;       //需要刷新冻结参数的类型
AP_EVENT_EXT e_TypeOfFrez ge_TypeOfFrez;        //冻结的类型

/*******************************************************************************
**冻结时间结构体
*******************************************************************************/
typedef struct s_FrzTime
{
  uint8         HourFrzTime;                    //小时冻结时间
  uint8         DayFrzTime[2];                  //日冻结时间
  uint8         MonthFrzTime[3];                //月冻结时间
  S_TIMEDATA    DSFrzTime;                      //定时冻结时间
  S_TIMEDATA    SQFrzTime;                      //时区表切换冻结时间
  S_TIMEDATA    SDFrzTime;                      //日时段表切换冻结时间
} S_FRZTIME;
AP_EVENT_EXT S_FRZTIME gs_FrzTime;              //冻结时间


/*******************************************************************************
**清事件的类型
*******************************************************************************/
enum 
{
    ClrType_Power = 1,              //清上下电记录
    ClrType_BC,                     //请编程记录
    ClrType_JS,                     //清校时记录
    ClrType_BG,                     //清表盖记录
    ClrType_POWERR,                 //清电源故障记录
#ifdef SQSDPRLOG
    ClrType_RSDBC,                              //清日时段编程记录
    ClrType_SQBC,                               //清时区编程记录
#endif

#ifdef ZXRPRLOG
    ClrType_ZXRBC,                              //清周休日编程记录
#endif

#ifdef YGZHZPRLOG
    ClrType_YGZHBC,                             //清有功组合方式编程记录
#endif

#ifdef JSRPRLOG
    ClrType_JSRBC,                              //清结算日编程记录
#endif
    ClrType_All,                                //清所有记录
};

/*******************************************************************************
**清事件结构体
*******************************************************************************/
typedef struct s_ClearEvent
{
  uint32    ID;                                 //数据标识
  uint8     type;                               //清某个记录
} S_CLEAREVENT;

                                                //清事件变量
extern const   S_CLEAREVENT code gs_ClearEvent[];
extern const   uint8 code CntgClrE;           //项数
AP_EVENT_EXT uint8 guc_ClearType;               //清事件的类型

AP_EVENT_EXT uint32 gui_LHZCoder[2];

/*******************************************************************************
**函数申明
*******************************************************************************/
uint8 SaveProgramLog(S_FRAMEINFO *s_FrmInfo);
void PowerUpProc(void);
void PowerDnProc(void);
void PowerJL(uint8 type);
void Com_JSJL(uint32 code, S_TIMEDATA* time);
void Com_ClearMeterJL(void);
void Com_ClearEventJL(uint8 type);
void Data_ClearEvent(void);

void CoverOpenLog(void);
void CoverCloseLog(void);

void HurryFrez(void);
void SQFrez(void);
void RSDFrez(void);
void DSFrez(void);
void ZDFrez(void);
void DayFrez(void);
uint8 ZDFreStartTm(void);


#endif

