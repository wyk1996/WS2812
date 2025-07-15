#ifndef __RACCOON_H__
#define __RACCOON_H__


#ifndef  RACCOONEXT
    #define RACCOONEXT extern
#else
    #define RACCOONSELF
#endif
#include "TypeDef.h"

//=============================================================
//RACCOON 模拟控制寄存器  ANCtrl1、ANCtrl2必须为0
//=============================================================
//#define   RegANCtrl0   0x0185
//#define   RegANCtrl1   0X0186
//#define   RegANCtrl2   0X0187

//=============================================================
//RACCOON 计量配置寄存器  
//=============================================================
//#define   RegMTPARA0   0x0183
//#define   RegMTPARA1   0x0184

//=============================================================
//RACCOON 系统配置寄存器  
//=============================================================
//#define   RegSysCtrl   0x0180         

//=============================================================
//RACCOON 直流分量寄存器  
//=============================================================
//#define   RegDCU       0x0114
//#define   RegDCI       0x0115
//#define   RegDCM       0x0116
//#define   RegDCIM      0x00F9
//#define   RegDCVTM     0x00FB
//#define   RegDCVDCM1   0x00FD
//#define   RegDCVDCM2   0x00FF
//#define   RegDCTM      0x0101

//=============================================================
//RACCOON 频率寄存器  
//=============================================================
//#define   RegFREQ      0x019A
//#define   RegSAFREQ    0x011D
//#define   RegAFREQ     0x011E

//=============================================================
//RACCOON 电压/电流/M通道有效值寄存器  
//=============================================================
//#define   RegARRTI     0x0105
//#define   RegARRTU     0x0104
//#define   RegARRTM     0x0106
//#define   RegBRRTU     0x0109
//#define   RegBRRTI     0x010A
//#define   RegARTI      0x010E
//#define   RegARTU      0x010D
//#define   RegARTM      0x010F
//#define   RegARTIM     0x00F8
//#define   RegARVTM     0x00FA
//#define   RegARVDCM1   0x00FC
//#define   RegARVDCM2   0x00FE
//#define   RegARTMM     0x0100
//#define   RegBRTU      0x0112
//#define   RegBRTI      0x0113
//#define   RegAARTI     0x011C
//#define   RegAARTU     0x011B
//#define   RegAARTM     0x0117
//#define   RegABRTU     0x0121
//#define   RegABRTI     0x0122

//=============================================================
//RACCOON 有功/无功功率寄存器  
//=============================================================
//#define   RegRAP       0x0102
//#define   RegRAQ       0x0103
//#define   RegRBP       0x0107
//#define   RegRBQ       0x0108
//#define   RegIAP       0x010B
//#define   RegIAQ       0x010C
//#define   RegIBP       0x0110
//#define   RegIBQ       0x0111
//#define   RegAAP       0x0119
//#define   RegAAQ       0x011A
//#define   RegABP       0x011F
//#define   RegABQ       0x0120


//=============================================================
//RACCOON 有功/无功能量累加、CF脉冲阀寄存器  
//=============================================================
//#define   RegPEGY      0x01A1
//#define   RegNEGY      0x01A2
//#define   RegPCFCNT    0x01A3
//#define   RegNCFCNT    0x01A4


//=============================================================
//RACCOON 预设直流偏置值寄存器  
//=============================================================
//#define   RegZZDCI     0x0123
//#define   RegZZDCU     0x0124

//=============================================================
//RACCOON 电压/电流/测量值有效值校正寄存器  
//=============================================================
//#define   RegWARTI     0x012C
//#define   RegWARTU     0x0132
//#define   RegWARTM     0x012D
//#define   RegWBRTI     0x0126
//#define   RegWBRTU     0x012B

//=============================================================
//RACCOON 全波有功/无功功率校正寄存器  
//=============================================================
//#define   RegWAPT      0x012E
//#define   RegWAQT      0x0130
//#define   RegWWAPT     0x012F
//#define   RegWWAQT     0x0131
//#define   RegWBPT      0x0127
//#define   RegWBQT      0x0129
//#define   RegWWBPT     0x0128
//#define   RegWWBQT     0x012A

//=============================================================
//RACCOON 门限值寄存器  
//=============================================================
//#define   RegEGYTH     0x0181
//#define   RegCTH       0x0182
//#define   RegIDETTH    0x0134

//=============================================================
//RACCOON 校验和寄存器  
//=============================================================
//#define   RegCKSUM     0x0133


//=============================================================
//RACCOON 带通滤波器系数寄存器  
//=============================================================
//#define   RegBPFPARA   0x0125

//=============================================================
//RACCOON 功率值寄存器  
//=============================================================
//#define   RegDATACP    0x0189



//=============================================================
//函数功能:RACCOON的读写控制
//
//=============================================================
#define APhy  0x00
#define BPhy  0x04
#define CPhy  0x08
#define ABCPhy 0x03
#define RacRead  0x01
#define RacWrite 0x02 
#define RacBroadcast 0

//=============================================================
//函数功能:RACCOON的从机地址
//
//=============================================================


//下面是为了编译通过

////功率/电流数据寄存器
//#define DATAIP      0x10d1
//#define DATAIQ      0X10D2
//#define RMSIU       0x10d3
//#define RMSII1      0x10D4
//#define RMSII2      0x10d5
//#define DATAP       0x0119
//#define DATAQ       0x10d7
//#define RMSU        0x011B
//#define RMSI1       0x011C
//#define RMSI2       0x0117

//#define RAP         0x0102  //全波有功功率原始值
//#define RAQ         0x0103  //全波无功功率原始值
//#define ARRTU       0x0104  //全波电流有效值的原始值
//#define ARRTI       0x0105  //全波电压有效值的原始值
 
//
#define DATAFREQ    0x011D
//
//上面是为了编译通过


//****************************************************************
//V9240 寄存器定义 
//****************************************************************

//=============================================================
//V9240 系统控制寄存器  
//=============================================================
#define   SysCtrl   0x0180 

//=============================================================
//V9240 校表参数寄存器  
//=============================================================
#define   PAC     0x00F6       //A通道有功功率比差校正
#define   PHC     0x00F7       //有功功率角差校正
#define   PADCC   0x00F8       //A通道有功功率小信号校正
#define   QAC     0x00F9       //A路无功功率比差校正
#define   QBC     0x00FA       //B路无功功率比差校正
#define   QADCC   0x00FB       //A路无功功率小信号校正
#define   QBDCC   0x00FC       //B路无功功率小信号校正
#define   IAC     0x00FD       //A路电流有效值比差校正
#define   IADCC   0x00FE       //A路电流有效值小信号校正
#define   UC      0x00FF       //电压有效值比差校正
#define   PBC     0x0100       //B路有功功率比差校正
#define   PBDCC   0x0101       //B路有功功率小信号校正
#define   IBC     0x0102       //B路电流有效值比差校正
#define   IBDCC   0x0103       //B路电流有效值小信号校正
#define   IAADCC  0x0104       //A路电流ADC直流偏置校正
#define   IBADCC  0x0105       //B路电流ADC直流偏置校正
#define   UADCC   0x0106       //电压ADC直流偏置校正
#define   BPFPARA 0x0107       //带通滤波器系数

//=============================================================
//V9240 校验和寄存器  
//=============================================================
#define   CKSUM   0x0109       //校验和寄存器

//=============================================================
//V9240 软件复位寄存器  
//=============================================================
#define   SFTRST  0x01BF       //软件复位寄存器    写入0x4572BEAF，复位所有电路

//=============================================================
//V9240 系统状态寄存器  
//=============================================================
#define   SysSts  0x00CA       //系统状态寄存器

//=============================================================
//V9240 计量数据寄存器  
//=============================================================
#define   FREQINST  0x00CB     //瞬时频率值
#define   PAINST    0x00CC     //A路瞬时有功功率值
#define   QINST     0x00CD     //瞬时无功功率值
#define   IAINST    0x00CE     //A路瞬时电流有效值
#define   UINST     0x00CF     //瞬时电压有效值
#define   PAAVG     0x00D0     //A路平均有功功率值
#define   QAVG      0x00D1     //平均无功功率值 
#define   FRQAVG    0x00D2     //平均频率值 
#define   IAAVG     0x00D3     //A路平均电流有效值
#define   UAVG      0x00D4     //平均电压有效值
#define   PBINST    0x00D5     //B路瞬时有功功率值
#define   IBINST    0x00D6     //B路瞬时电流有效值
#define   PBAVG     0x00D7     //B路平均有功功率值
#define   IBAVG     0x00D8     //B路平均电流有效值
#define   UDCINST   0x00D9     //电压瞬时直流分量值
#define   IADCINST  0x00DA     //A路瞬时直流分量值
#define   IBDCINST  0x00DB     //B路瞬时直流分量值
#define   ZXDATREG  0x00DC     //电压过零点前一次采样值
#define   ZXDAT     0x00DD     //电压过零点当前点采样值
#define   PHDAT     0x00DE     //电压相位数据
#define   T8BAUD    0x00E0//0x00DF     //当前波特率通信时连续8bit下降沿时间数据
//#define   T1BAUD    0x00E0     //当前波特率通信时连续1bit下降沿时间数据

#define   AnaCtrl1_Val    0x10000000
//======================================================================
#define SysCtrl_20PPM    0x02
#define SysCtrl_30PPM    0x03
#define SysCtrl_40PPM    0x04
#define SysCtrl_50PPM    0x05
#define SysCtrl_60PPM    0x06
#define SysCtrl_70PPM    0x07
#define SysCtrl_80PPM    0x08

#define PPM_VALUE  SysCtrl_40PPM

#define FRC_50HZ    0x01
#define FRC_60HZ    0x02

#define HZ_CONFIG  FRC_50HZ
//--------------------------------------------------------------
//#define SysCtrl_Value   0x2C064000   //SysCtrl寄存器值  U: x1  IA: x32  IB: x32  +50 PPM

//--------------------------------------------------------------
#if ( PPM_VALUE == SysCtrl_80PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C014000   // +80 PPM
    #define SysCtrl_ValueA   0x2C014000   // +80 PPM
    #define SysCtrl_ValueB   0x2C014000   // +80 PPM
    #define SysCtrl_ValueC   0x2C014000   // +80 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C014040   // +80 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C014040   // +80 PPM
    #define SysCtrl_ValueB   0x2C014040   // +80 PPM
    #define SysCtrl_ValueC   0x2C014040   // +80 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_70PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C004000   // +70 PPM
    #define SysCtrl_ValueA   0x2C004000   // +70 PPM
    #define SysCtrl_ValueB   0x2C004000   // +70 PPM
    #define SysCtrl_ValueC   0x2C004000   // +70 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C004040   // +70 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C004040   // +70 PPM
    #define SysCtrl_ValueB   0x2C004040   // +70 PPM
    #define SysCtrl_ValueC   0x2C004040   // +70 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_60PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C074000   // +60 PPM
    #define SysCtrl_ValueA   0x2C074000   // +60 PPM
    #define SysCtrl_ValueB   0x2C074000   // +60 PPM
    #define SysCtrl_ValueC   0x2C074000   // +60 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C074040   // +60 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C074040   // +60 PPM
    #define SysCtrl_ValueB   0x2C074040   // +60 PPM
    #define SysCtrl_ValueC   0x2C074040   // +60 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_50PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C064000   // +50 PPM
    #define SysCtrl_ValueA   0x2C064000   // +50 PPM
    #define SysCtrl_ValueB   0x2C064000   // +50 PPM
    #define SysCtrl_ValueC   0x2C064000   // +50 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C064040   // +50 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C064040   // +50 PPM
    #define SysCtrl_ValueB   0x2C064040   // +50 PPM
    #define SysCtrl_ValueC   0x2C064040   // +50 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_40PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C054000   // +40 PPM
    #define SysCtrl_ValueA   0x2C054000   // +40 PPM
    #define SysCtrl_ValueB   0x2C054000   // +40 PPM
    #define SysCtrl_ValueC   0x2C054000   // +40 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C054040   // +40 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C054040   // +40 PPM
    #define SysCtrl_ValueB   0x2C054040   // +40 PPM
    #define SysCtrl_ValueC   0x2C054040   // +40 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_30PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C044000   // +30 PPM
    #define SysCtrl_ValueA   0x2C044000   // +30 PPM
    #define SysCtrl_ValueB   0x2C044000   // +30 PPM
    #define SysCtrl_ValueC   0x2C044000   // +30 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C044040   // +30 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C044040   // +30 PPM
    #define SysCtrl_ValueB   0x2C044040   // +30 PPM
    #define SysCtrl_ValueC   0x2C044040   // +30 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_20PPM)

  #if ( HZ_CONFIG == FRC_50HZ)
    #define SysCtrl_ValueDef   0x2C020000   // +20 PPM
    #define SysCtrl_ValueA   0x2C020000   // +20 PPM
    #define SysCtrl_ValueB   0x2C020000   // +20 PPM
    #define SysCtrl_ValueC   0x2C020000   // +20 PPM
  #elif ( HZ_CONFIG == FRC_60HZ)
    #define SysCtrl_ValueDef   0x2C020040   // +20 PPM  RC x 1.2 60Hz
    #define SysCtrl_ValueA   0x2C020040   // +20 PPM
    #define SysCtrl_ValueB   0x2C020040   // +20 PPM
    #define SysCtrl_ValueC   0x2C020040   // +20 PPM
  #else
    #error "HZ_CONFIG is error"
  #endif

#else
 #error "PPM_VALUE is error"
#endif


#define BPFPARA_ValueDef   0x806764B6   //BPFPARA寄存器值默认值

//======================================================================
//#define Actual_BAUDRate 4879.9//7195//9600     //实际波特率
#define Actual_BAUDRate 4800//4800//9602     //实际波特率


#define K_BAUDRate  (float)(Actual_BAUDRate * 0.00390625)  //频率计算系数

#define T8_BAUDRate (8 * 3276800 / Actual_BAUDRate) //实际波特率：T8计数值

//======================================================================

#define BAUDRate_1Byte_OverTime  (float)(11*1000/Actual_BAUDRate*1.4)  //通信超时时间率

//======================================================================
#define Slave0  0
#define Slave1  1
#define Slave2  2
#define Slave3  3

#define SlaveNum    Slave0
//#define HZCONST   3200*256*10
#define HZCONST   409600

#define VARHZ   0x4c000


#ifdef RACCOONSELF
//uint16 const  RegAddr[]=
//{
//    RegANCtrl0,     //模拟控制寄存器0
//    RegANCtrl1,     //模拟控制寄存器1
//    RegANCtrl2,     //模拟控制寄存器2
//    RegSysCtrl,     //系统配置寄存器
//    RegBPFPARA,     //带通滤波器系数寄存器
//
//    RegMTPARA0,     //计量控制寄存器0 
//    RegMTPARA1,     //计量控制寄存器1
//
////  RegIDETTH,
//    
//    RegWARTI,        //全波电压有效值比差寄存器
//    RegWARTU,       //全波电流有效值比差寄存器 
////  RegWARTM,
////  RegWBRTI,
////  RegWBRTU,
//    
//    RegWAPT,        //全波有功功率比差寄存器
// // RegWAQT,
//    RegWWAPT,       //全波有功功率二次补偿值寄存器
//    RegWWAQT,
//    
////  RegWBPT,
////  RegWBQT,
////  RegWWBPT,
////  RegWWBQT,
////      
////  RegZZDCI,
////  RegZZDCU
//
//};

uint16 const  RegAddr[]=
{

    SysCtrl,     //系统配置寄存器
    BPFPARA,     //带通滤波器系数寄存器
    
    UC,        //全波电压有效值比差寄存器
    IAC,       //全波电流有效值比差寄存器    
    PAC,        //全波有功功率比差寄存器
    PADCC,      //全波有功功率二次补偿值寄存器
    QADCC,      //全波无功功率二次补偿值寄存器
    
};
#else
extern uint16 const  RegAddr[];
#endif

enum
{
  Rac_Idle=0,
  Rac_Send,
  Rac_Rev,
  Rac_WaitPro,
   
};

//typedef struct
//{
//    uint32  RacMTPARA1;         //计量控制寄存器1 包含角差
//    uint32  RacWARTU;           //全波电压有效值比差寄存器
//    uint32  RacWARTI;           //全波电流有效值比差寄存器
//    uint32  RacWAPT;            //全波有功功率比差寄存器
//    uint32  RacWWAPT;           //全波有功功率二次补偿寄存器                
//    uint32  RacREWWAPT;         //全波无功功功率二次补偿寄存器                
//}S_JBRE;


//V9240/V9260S校表寄存器
typedef struct
{
    uint32  RacPHC;             //有功功率角差 BIT[24:16]为IB通道;BIT[8:0]为IA通道
    uint32  RacUC;              //电压有效值比差寄存器
    uint32  RacIAC;             //A路电流有效值比差寄存器
    uint32  RacPAC;             //A路有功功率比差寄存器
    uint32  RacPADCC;           //A路有功功率二次补偿寄存器                
    uint32  RacQADCC;           //A路无功功率二次补偿寄存器  
    uint32  RacIADCC;           //A路电流有效值二次校正寄存器
}S_JBRE;


//typedef struct 
//{
//    uint16 ui_MeterC;           // 表常数
//    uint16 ui_Un;               // 标称电压
//    uint16 ui_Ib;               // 标称电流
//    uint16 ui_Resve1;           //保留
//    
//    uint32  RacEGYTH;           //有功门限值寄存器
//    uint32  RacREEGYTH;           //无功门限值寄存器
//    //uint32  RacCTH;             //起动/潜动判断门限值寄存器
//    //uint32  RacRECTH;             //无功起动/潜动判断门限值寄存器
//    
//
//    uint32  RacANCtrl0;         //模拟控制寄存器0
//    uint32  RacANCtrl1;         //模拟控制寄存器1
//    uint32  RacANCtrl2;         //模拟控制寄存器2
////  uint32  RacSysCtrl;         //系统配置寄存器
//    uint32  RacBPFPARA;         //带通滤波器系数寄存器
//    uint32  RacMTPARA0;         //计量控制寄存器0
//    
//    S_JBRE  gs_JBA;             //A相比差组
//    S_JBRE  gs_JBB;             //B相比差组
//    S_JBRE  gs_JBC;             //C相比差组
//
//    uint32 ul_PG;               //功率比例系数
//    uint32 ul_URmG;             //电压通道比例系数
//    uint32 ul_I1RmG;            //电流通道1比例系数
////  uint32 ul_I2RmG;            //电流通道2比例系数
//    
//    uint16 ui_Resve2;           //保留
//    uint16 ui_JbCRC;            // 校表参数的CRC结果
//}S_JBPM;

typedef struct 
{
    uint16 ui_MeterC;           // 表常数
    uint16 ui_Un;               // 标称电压
    uint16 ui_Ib;               // 标称电流
    uint16 ui_Resve1;           //保留
    
    uint32  RacEGYTH;           //E1有功门限值寄存器
    uint32  RacREEGYTH;         //E2无功门限值寄存器
    //uint32  RacCTH;             //起动/潜动判断门限值寄存器
    //uint32  RacRECTH;             //无功起动/潜动判断门限值寄存器
    
    uint32  RacSysCtrl;         //系统配置寄存器
    uint32  RacBPFPARA;         //带通滤波器系数寄存器
    
    S_JBRE  gs_JBA;             //A相比差组
    S_JBRE  gs_JBB;             //B相比差组
    S_JBRE  gs_JBC;             //C相比差组

    uint32 ul_PG;               //功率比例系数       zyj 11.12 与InitPara5不对应
    uint32 ul_URmG;             //电压通道比例系数
    uint32 ul_I1RmG;            //电流通道1比例系数
//  uint32 ul_I2RmG;            //电流通道2比例系数
    
    uint16 ui_Resve2;           //保留
    uint16 ui_JbCRC;            // 校表参数的CRC结果
}S_JBPM;


RACCOONEXT S_JBPM gs_JbPm;            //校表参数



typedef struct 
{
    uint32 BPFPARA_Value[3];
    uint16 ui_CRC;
}S_BPFPARA_Value;

RACCOONEXT S_BPFPARA_Value gs_BPFPARA_Value;    //带通滤波器系数寄存器

RACCOONEXT uint32 gul_FRQAVG_Value[3];
RACCOONEXT uint8  guc_CalBPFValue_DelayTm;

typedef struct 
{
  uint8 ucStatus;               
  uint8 ucSendLen;
  uint8 ucSendPoint;
  uint8 ucRevLen;
  uint8 ucRevPoint;
  uint8 ucBuf[100];
}S_RacCtrl;
RACCOONEXT S_RacCtrl gs_RacCtrl;


RACCOONEXT volatile uint8 guc_CommDelayTime;                                 //通讯等待延时
RACCOONEXT uint8 r;
RACCOONEXT uint8 guc_Phy;          //相位
extern const uint8 flgphy[];
extern const uint8 phy[];
extern const uint8 phadd[];

RACCOONEXT uint8 guc_ReadingABC_Flg; //正在读有效值数据期间标志
//RACCOONEXT uint8 guc_StartReadABC_Flg; //读有效值数据标志

RACCOONEXT uint8  guc_DyUart2Over;        //模拟串口超时保护
typedef struct
{
    uint8 uc_Phystat;
    uint8 uc_Physendtim;
    uint8 uc_Sendstat;
    uint8 uc_Powstat;
    uint8 uc_flapara;
    uint8 uc_Uartstat;
}S_RacPhy;
RACCOONEXT S_RacPhy gs_RacPhy;

enum
{
     power_a = 0x01,
     power_b = 0x02,
     power_c = 0x04,
};


RACCOONEXT uint8  Accumulate_abc_times;

RACCOONEXT uint32  gul_dispbuf_total;
RACCOONEXT uint32  gul_dispbuf_timeoutA;
RACCOONEXT uint32  gul_dispbuf_timeoutB;
RACCOONEXT uint32  gul_dispbuf_timeoutC;
RACCOONEXT uint32  gul_dispbuf_crcerrA;
RACCOONEXT uint32  gul_dispbuf_crcerrB;
RACCOONEXT uint32  gul_dispbuf_crcerrC;
//RACCOONEXT uint8 guc_ResetV9821Delay;

#define V9260Start_Send()       { SCON4|=BIT1; ExInt2IFG|=(BIT2); }

#define Inaport() {}  //{P1OE|=BIT1;P1IE|=BIT1; P11FS = 0; }
#define Inbport() {}  //{P1OE|=BIT2;P1IE|=BIT2; P12FS = 0; }
#define Incport() {}  //{P1OE|=BIT3;P1IE|=BIT3; P13FS = 0; }

#define Ina() 0  //(P1ID & BIT1) 
#define Inb() 0  //(P1ID & BIT2) 
#define Inc() 0  //(P1ID & BIT3) 



void Init_RacCtrl(void);
uint8 WriteRaccoon(uint32 Data,uint16 Addr,uint8 Phyadd);
uint8 ReadRaccoon(uint16 Addr,uint8 num,uint8 Phyadd);
void ReadRaccoonInt(uint16 Addr,uint8 num,uint8 Phyadd); 
void ReadbackABCPhy(uint8 Phyadd);  
void RxReset_V9260(void);
void InitRaccoon(void);
void V9260_InitDefaultPara(void);
void CheckRegSum(void);
void CheckSysErr(void);
uint8 UpdateRaccoonPara(void);
uint32 Read_Power(uint16 reg,uint8 phyadd);
void BroadcastWriteRaccoon(uint32 Data,uint16 Addr);
void UpdateRaccoonParABC(uint8 phyadd);
void Accumulate_abc(void);
void Clashin(void);
void Clashout(void);

void RaccoonPara_Check(void);
void CHKERR_Bit_scan(void);
void Recover_from_powerdown_scan(void);
void Check_ABC_Phase(void);
void RecoverBPFPARA_Value(void);
void Cal_BPFPARA_Value(void);





#endif
