#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#ifndef VAREXT
    #define VAREXT extern
#endif


#include "LeoPartReg.h"
#include "string.h"
#include "intrinsics.h"
#include "math.h"
#include "TypeDef.h"
#include "BEBottom.h"
#include "DataProtect.h"
#include "Api.h"
#include "System.h"
#include "McuDrive.h"
#include "Key.h"
#include "Powmanger.h"
#include "ComConfig.h"
#include "ComBottom.h"
#include "Uart2.h"
//#include "Uart1.h"
#include "Uart4.h"
#include "ComApplication.h"
#include "Prcfrm.h"
//#include "LoadCurve.h"
#include "ComCoretab.h"
//#include "PrologFee.h"
#include "E2AddTab.h"
#include "DLT_645_2007.h"
#include "EnergyBottom.h"
#include "Energy.h"
//#include "Multariff.h"
#include "Timeproc.h"
#include "LcdBottom.h"
#include "Lcdmiddle.h"
#include "Lcdshow.h"
#include "Dataproc.h"
#include "FunCfg.h"
//#include "Event.h"
#include "InitPara.h"
//#include "RtcCpt.h"
//#include "CmpCode.h"
#include "Version.h"
#include "Raccoon.h"




VAREXT uint16 gui_ReadABCPhy;  //秒读功率，电压
#define flgEtPhy_A 0x01
#define flgEtPhy_B 0x02
#define flgEtPhy_C 0x04
#define flgEtread    0x01
#define flgEtreadint 0x02
#define flgEtwrite   0x04
#define flgEtBwrite   0x08


VAREXT uint16 gui_SystemEvent;
#define flgEtGlb_PowDown        0x01
#define flgEtGlb_EnergyCalcN    0x02
#define flgEtGlb_EnergyCalc     0x04
#define flgEtGlb_RefreshLcd     0x08
#define flgEtGlb_NeedPrtclChk   0x10
#define flgEtGlb_ClearEv        0x20
#define flgEtGlb_ClearMeter     0x40
#define flgEtGlb_ReadPower      0x80  //读电量时间
#define flgEtGlb_ABCPhy         0x0700
#define flgEtGlb_ABC_Phase      0x1000
//#define flgEtGlb_RacTimeOut     0x2000



VAREXT uint8 guc_TimeEvent;
#define flgEtTim_Second         0x01
#define flgEtTim_Minute         0x02
#define flgEtTim_Hour           0x04
#define flgEtTim_SecSoft        0x08
#define flgEtTim_Day            0x10


VAREXT uint8 guc_KeyEvent;
#define flgEtKey_Up             0x01      //已用
#define flgEtKey_Pr             0x02


VAREXT uint16 gui_RefreshEvent;
#define flgEtPara_Fee           0x0001
#define flgEtPara_EnyBottom     0x0002    //已用
#define flgEtPara_Config        0x0004
#define flgEtPara_RTC           0x0008
#define flgEtPara_Bode          0x0010    //已用
#define flgEtPara_Show          0x0020    //已用
#define flgEtPara_Init          0x0040    //已用
#define flgEtPara_RtcParaFsToE2 0x0080
#define flgEtPara_LoadCurve     0x0100
#define flgEtPara_PowDir        0x0200
#define flgEtPara_FrsJsr        0x0400
#define flgEtPara_InitA         0x1000    //已用
#define flgEtPara_InitB         0x2000    //已用
#define flgEtPara_InitC         0x4000    //已用


VAREXT uint8 guc_DjCheckFlg;    
#define flgDsDjCheck        0x01        
#define flgZdDjCheck        0x02
#define flgDayDjCheck       0x04
#define flgInsDjEvent       0x08
 
VAREXT uint8  guc_SleepFlg;
VAREXT uint8  guc_PowOffRuning;
VAREXT uint16 gui_WakeUpCnt;
VAREXT uint8  guc_SecCnt;
VAREXT uint8  guc_ABCreadbegian;
VAREXT uint8 guc_1280msCnt;
VAREXT uint8 guc_ShowErr;
VAREXT uint8 guc_80msCntCF1;
VAREXT uint8 guc_80msCntFLG1;
VAREXT uint8 guc_80msCntCF2;
VAREXT uint8 guc_80msCntFLG2;

typedef struct
{
    uint32 ZXDatReg;
    uint32 ZXDat;
    uint32 PHDat;
    uint32 NoDef;
    uint32 T8Baud;
}S_PHASEDAT;      //9240 相位相关寄存器数据

typedef struct
{
    S_PHASEDAT gs_APhaseDat;
    S_PHASEDAT gs_BPhaseDat;
    S_PHASEDAT gs_CPhaseDat;
    uint32 APhytim;
    uint32 BPhytim;
    uint32 CPhytim;
    uint8  RevPhaseFlg;
}S_PHYTIM;
VAREXT S_PHYTIM gs_phytim;  //电压相角计数值

//typedef struct
//{
//    uint8 Aint;
//    uint8 Bint;
//    uint8 Cint;
//}S_PHYINT;
//VAREXT S_PHYINT gs_phyint; //中断时间


typedef struct
{
    uint32  ul_power[4];         
    uint32  ul_power_all[4];          
    uint16  ui_power_rem[4];
    uint32  ul_Npower[4];         
    uint32  ul_Npower_all[4];          
    uint16  ui_Npower_rem[4];
    uint32  ul_underpow;
    uint32  ul_Nunderpow;
}S_POWER;

VAREXT S_POWER gs_power; 

typedef struct
{
    uint32  ul_U[3];         
    uint32  ul_I[3];    
    uint32  ul_P[4];
    uint32  ul_Hz[3];
}S_UI;

VAREXT S_UI gs_UI; 

typedef struct
{
    uint32  ul_power[3];         
    uint32  ul_Npower[3];         
}S_POWER_ENERYCAL;

VAREXT S_POWER_ENERYCAL gs_power_enerycal;

VAREXT uint16 gui_SystemState;
#define flgStSys_Program  0x01
#define flgStSys_IsTodayJS 0x02
#define flgStSys_PowOffing 0x04
#define flgStSys_PowOff 0x08
#define flgStSys_PowErr 0x10

VAREXT uint16 gui_SystemSleepState;

VAREXT uint8  guc_CheckABCPhase_Cnt;
VAREXT uint8  guc_CheckABCPhase_Dly;

#define  _IsSecuProduct  (gui_SystemState&flgStSys_Program)
#define _SYS_IS_PWR_OFF  (gui_SystemState&flgStSys_PowOff)


VAREXT uint16 gui_DispErrState;

VAREXT uint8 guc_RevCntA;
VAREXT uint8 guc_RevCntB;
VAREXT uint8 guc_RevCntC;

#endif

