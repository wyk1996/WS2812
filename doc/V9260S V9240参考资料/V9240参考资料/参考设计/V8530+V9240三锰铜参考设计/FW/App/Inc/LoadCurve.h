#ifndef __LOADCURVE_H__
#define __LOADCURVE_H__

#ifndef LOADCURVEEXT
    #define LOADCURVEEXT extern
#endif

#define LOADCURVE

#define CurveTotalCnt     288
   

#define CurveOneType     0x01
#define CurveTwoType     0x02
#define CurveThreeType   0x04
#define CurveFourType    0x08


typedef struct
{
    uint8 CurCnt;
    uint8 TotalCnt;
    uint8 SeqFrameEnd;
    uint16 LastCurvePt;
    
}S_CURVEFOLLOWFRAME;
LOADCURVEEXT S_CURVEFOLLOWFRAME gs_CurveFollowFrame;

typedef struct
{
    uint32 ulTotalCnt;
    uint32 ulOneCnt;
    uint32 ulTwoCnt;
    uint32 ulThreeCnt;
    uint32 ulFourCnt;
}S_CURVECNT;


typedef struct 
{
    uint8   ucMinute;       //分
    uint8   ucHour;         //时
    uint8   ucDay;          //日
    uint8   ucMonth;        //月
    uint8   ucYear;         //年
}S_CURVETM;

typedef struct
{
    S_CURVETM  STm;
    uint8  ucType;
    uint8  ucOnePt;
    uint8  ucTwoPt;
    uint8  ucThreePt;
    uint8  ucFourPt;
    uint8  ucHighBit;
}S_CURVETOTAL1;

typedef struct
{
    S_CURVETM  STm;
    uint8  ucType;
    uint16 uiOnePt;
    uint16 uiTwoPt;
    uint16 uiThreePt;
    uint16 uiFourPt;
}S_CURVETOTAL;


LOADCURVEEXT uint8 guc_LoadCurveMode;  //负荷记录模式字

typedef struct
{
    uint16 uiFH1;       //第1类负荷记录间隔时间
    uint16 uiFH2;       //第2类负荷记录间隔时间
    uint16 uiFH3;       //第3类负荷记录间隔时间
    uint16 uiFH4;       //第4类负荷记录间隔时间
}S_FHTIME;
 
LOADCURVEEXT S_FHTIME gs_LoadCurveTime; //负荷记录间隔

typedef struct
{
    uint16 uiCntAddr;
    uint16 uiPtAddr;
    uint16 uiBaseAddr;
    uint8  ucLen;
}S_CURVESTORE;

extern const S_CURVESTORE code gs_CurveStoreTab[5];
LOADCURVEEXT uint8 guc_Timechange;  //时间是否改变

LOADCURVEEXT void LoadCurveCheck(void);
void LoadCurveSave(void);
void RecoverLoadCurve(void);

uint8 CmpTm(uint8 const *T1, uint8  const *T2, uint8 n ); 
uint8 BuildOneCurveFrame(uint8 ucType,uint8*DesBuff,uint8*SrcBuff);
uint8 BuildTotalCurveFrame(S_CURVETOTAL *s_TotalCurve,uint8 *buff);
 
uint8 GetOneOldestRecord( uint8 ucType, uint8 ucCnt,uint8 *buff);
uint8 GetTotalOldestRecord(uint8 ucCnt,uint8 *buff);

uint8 GetOneTmRecord(uint8 ucType,uint8 *Tm,uint8 *buff);
uint8 GetOneTmSeqRecord( uint8 ucType, uint8 ucCnt,uint8 *buff);

uint8 GetTotalTmRecord(uint8 *Tm,uint8 *buff);
uint8 GetTotalTmSeqRecord(uint8 ucCnt,uint8 *buff);

uint8 GetOneItemRecord( uint8 ucType, uint8 *buff);
uint8 GetTotalItemRecord(uint8 *buff);






#endif 

