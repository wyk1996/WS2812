#ifndef __ComBottom_H__
#define __ComBottom_H__


#ifndef COMBOTTOMEXT
    #define COMBOTTOMEXT extern
#else 
    #define COMBOTTOMSELT
#endif

#define COMHEAD             0x68
#define COMTAIL             0x16
#define COMINITLEN          10
#define DLT2007DATALEN      100
/*
typedef struct 
{
    uint8   THValue;
    uint8   Type;              //T1M和SMOD的取值
}GS_BaudRate;
*/
typedef struct 
{
    uint32   THValue;
    uint32   Type;              //T1M和SMOD的取值
}GS_BaudRate;





#ifdef COMBOTTOMSELT

#include "Version.h"

#if ( Fmcu_Val == Fmcu_3M2)
const GS_BaudRate  BaudRateTable[5] =   //3.2M
{
   //{0xe4,   0x82},  //600
   //{0xf2,   0x82},  //1200
   //{0xd5,   0x22},  //2400
   //{0xd5,   0xa2},  //4800
   //{0xeb,   0xa2},  //9600
   {1638400,   600},  //600
   {1638400,   1200},  //1200
   {1638400,   2400},  //2400
   {1638400,   4800},  //4800
   {1638400,   9600},  //9600
 
};
#elif ( Fmcu_Val == Fmcu_6M5)
const GS_BaudRate  BaudRateTable[5] =  //6.5M
{
   //{0xc7,   0x82},  //600
   //{0x55,   0x22},  //1200
   //{0x55,   0xa2},  //2400
   //{0xab,   0xa2},  //4800
   //{0xd5,   0xa2},  //9600

   {3276800,   600},  //600
   {3276800,   1200},  //1200
   {3276800,   2400},  //2400
   {3276800,   4800},  //4800
   {3276800,   9600},  //9600
   
};
#elif ( Fmcu_Val == Fmcu_13M)
const GS_BaudRate  BaudRateTable[5] =  //13M
{
   //{0x8e,   0x82},  //600
   //{0xc7,   0x82},  //1200
   //{0x55,   0x22},  //2400
   //{0x55,   0xa2},  //4800
   //{0xab,   0xa2},  //9600

   {6553600,   600},  //600
   {6553600,   1200},  //1200
   {6553600,   2400},  //2400
   {6553600,   4800},  //4800
   {6553600,   9600},  //9600
};
#else
const GS_BaudRate  BaudRateTable[5] =  //26M
{
   //{0x71,   0x02},  //600
   //{0xC7,   0x02},  //1200
   //{0xC7,   0x82},  //2400
   //{0xE4,   0x82},  //4800
////   {0xED,   0x82},  //7200
   //{0xF2,   0x82},  //9600

    {13107200,   600},  //600
    {13107200,   1200},  //1200
    {13107200,   2400},  //2400
    {13107200,   4800},  //4800
    {13107200,   9600},  //9600
   
};
#endif

#else 
extern const GS_BaudRate BaudRateTable[5];
#endif

//变量
typedef struct s_com
{
    uint8   ucPort;
    uint8   ucStt;          // 通讯状态ComStt_Recv/ComStt_Send/COM_IDLE
    uint8   ucRecvTmr;      // 单位10ms,相当于调用周期为10ms
    uint8   ucOverTmr;      // 单位10ms,相当于调用周期为10ms,保护用
    uint8   ucFrmHeadCnt;   // 帧头处理
    uint8   ucLen;          // 收发的数据总长度
    uint8   ucPos;          // 当前处理数据的位置,相当于指向ucBuf的一个指针
    uint8   ucBuf[360];     // 通讯收发数据缓冲区
}S_COM;

COMBOTTOMEXT S_COM gs_ComGroup[Const_MaxComNum];                    // 通讯用的缓存


typedef struct
{
    volatile uint8 ucStt;
    volatile uint8 ucOverTmr;
    uint8 ucSendLen;
    uint8 ucSendPos;
    uint8 ucRevLen;
    uint8 ucRevPos;
    volatile uint8 ucBuf[128];
}S_7816COM;


//COMBOTTOMEXT  S_7816COM gs_7816Com;                    // 通讯用的缓存
//COMBOTTOMEXT  uint8 guc_IRWakeupBgcnt;



COMBOTTOMEXT uint8 guc_485Bode;         //485波特率
COMBOTTOMEXT uint8 guc_BodeDely;        //波特率切换延时
COMBOTTOMEXT uint8 guc_ComSendDy;       //通讯回复延时，20ms
//函数
void ComBom_Init(uint8 ucNo);
void ComBom_InitAll(void);
void ComBom_StartSend(uint8 ucNo);
void ComBom_10msDy(void);



#endif
