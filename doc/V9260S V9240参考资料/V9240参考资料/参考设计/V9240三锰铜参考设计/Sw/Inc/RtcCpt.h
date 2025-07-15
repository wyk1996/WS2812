#ifndef __RTCCPT_H__
    #define __RTCCPT_H__

#ifndef RTCCPTEXT
    #define RTCCPTEXT extern
#endif


typedef struct
{
    uint32 ul_TempParaA;      //系数A
    uint32 ul_TempParaB;      //系数B
    uint32 ul_TempParaC;      //系数C
    uint32 ul_TempParaD;      //系数D
    uint32 ul_TempParaE;      //系数E
    uint8  uc_Add33;          //和校验
}S_TEMPPARA;


typedef struct
{
    uint8   ucVersion;      //版本
    uint8   ucCtrlBGP;      //CtrlBGP
    uint8   uc_Add33;        //和校验
}S_CTRLBGP;

RTCCPTEXT uint32 gul_CellVoltage;
RTCCPTEXT uint8  guc_CheckBatFlg;
RTCCPTEXT int16  gi_Temprature;
RTCCPTEXT uint8  guc_FrsCalRTCDelay;


//void SetRTCNormal(int16 offset);
//void CalRTC(void);
//void RecoverRtcPara(void);
//void RecoverRtcParaFrmFalsh(void);


#endif


