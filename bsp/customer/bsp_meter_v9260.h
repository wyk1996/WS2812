#ifndef _BSP_METER_V9260_H_
#define _BSP_METER_V9260_H_


#include "gd32e23x.h"


#define RCD_DETECT_PORT                                 GPIOB						            //RCD检测
#define RCD_DETECT_PIN                                  GPIO_PIN_3

#define RCD_ZERO_CALI_PORT                              GPIOB                       //RCD校零
#define RCD_ZERO_CALI_PIN                               GPIO_PIN_4                  //校零

#define RCD_CALIBRATION_PORT                            GPIOB                       //RCD有效值校准
#define RCD_CALIBRATION_PIN                             GPIO_PIN_5                  //有效值校准

#define RCD_INT_PORT                           			    GPIOB                    	  //计量中断输出
#define RCD_INT_PIN                            			    GPIO_PIN_8                 	//计量中断输出












// typedef struct {
    // uint32_t  RacPHC;             //�й����ʽǲ� BIT[24:16]ΪIBͨ��;BIT[8:0]ΪIAͨ��
    // uint32_t  RacUC;              //��ѹ��Чֵ�Ȳ�Ĵ���
    // uint32_t  RacIAC;             //A·������Чֵ�Ȳ�Ĵ���
    // uint32_t  RacPAC;             //A·�й����ʱȲ�Ĵ���
    // uint32_t  RacPADCC;           //A·�й����ʶ��β����Ĵ���                
    // uint32_t  RacQADCC;           //A·�޹����ʶ��β����Ĵ���  
    // uint32_t  RacIADCC;           //A·������Чֵ����У���Ĵ���
//} S_JBRE;

// typedef struct {
    // uint16_t ui_MeterC;           // ������
    // uint16_t ui_Un;               // ��Ƶ�ѹ
    // uint16_t ui_Ib;               // ��Ƶ���
    // uint16_t ui_Resve1;           //����
    
    // uint32_t  RacEGYTH;           //E1�й�����ֵ�Ĵ���
    // uint32_t  RacREEGYTH;         //E2�޹�����ֵ�Ĵ���
    
    // uint32_t  RacSysCtrl;         //ϵͳ���üĴ���
    // uint32_t  RacBPFPARA;         //��ͨ�˲���ϵ���Ĵ���
    
    // S_JBRE  gs_JBA;             //A��Ȳ���


    // uint32_t ul_PG;               //���ʱ���ϵ��       zyj 11.12 ��InitPara5����Ӧ
    // uint32_t ul_URmG;             //��ѹͨ������ϵ��
    // uint32_t ul_I1RmG;            //����ͨ��1����ϵ��
    
    // uint16_t ui_Resve2;           //����
    // uint16_t ui_JbCRC;            // У��������CRC���
// } S_JBPM;

// S_JBPM gs_JbPm;            //У������

// typedef struct {
//     uint32_t BPFPARA_Value[3];
//     uint16_t ui_CRC;
// } S_BPFPARA_Value;

// S_BPFPARA_Value gs_BPFPARA_Value; 


#if 0
#define APhy  0x00
#define BPhy  0x04
#define CPhy  0x08
#define ABCPhy 0x03
#define RacRead  0x01
#define RacWrite 0x02 
#define RacBroadcast 0

#define DATAFREQ    0x011D
#define   SysCtrl   0x0180 


#define   PAC     0x00F6       
#define   PHC     0x00F7      
#define   PADCC   0x00F8       
#define   QAC     0x00F9     
#define   QBC     0x00FA      
#define   QADCC   0x00FB       
#define   QBDCC   0x00FC     
#define   IAC     0x00FD      
#define   IADCC   0x00FE      
#define   UC      0x00FF       
#define   PBC     0x0100      
#define   PBDCC   0x0101       
#define   IBC     0x0102      
#define   IBDCC   0x0103      
#define   IAADCC  0x0104     
#define   IBADCC  0x0105      
#define   UADCC   0x0106      
#define   BPFPARA 0x0107      


#define   CKSUM   0x0109       

#define   SFTRST  0x01BF   

#define   SysSts  0x00CA   


#define   FREQINST  0x00CB     //˲ʱƵ��ֵ
#define   PAINST    0x00CC     //A·˲ʱ�й�����ֵ
#define   QINST     0x00CD     //˲ʱ�޹�����ֵ
#define   IAINST    0x00CE     //A·˲ʱ������Чֵ
#define   UINST     0x00CF     //˲ʱ��ѹ��Чֵ
#define   PAAVG     0x00D0     //A·ƽ���й�����ֵ
#define   QAVG      0x00D1     //ƽ���޹�����ֵ 
#define   FRQAVG    0x00D2     //ƽ��Ƶ��ֵ 
#define   IAAVG     0x00D3     //A·ƽ��������Чֵ
#define   UAVG      0x00D4     //ƽ����ѹ��Чֵ
#define   PBINST    0x00D5     //B·˲ʱ�й�����ֵ
#define   IBINST    0x00D6     //B·˲ʱ������Чֵ
#define   PBAVG     0x00D7     //B·ƽ���й�����ֵ
#define   IBAVG     0x00D8     //B·ƽ��������Чֵ
#define   UDCINST   0x00D9     //��ѹ˲ʱֱ������ֵ
#define   IADCINST  0x00DA     //A·˲ʱֱ������ֵ
#define   IBDCINST  0x00DB     //B·˲ʱֱ������ֵ
#define   ZXDATREG  0x00DC     //��ѹ�����ǰһ�β���ֵ
#define   ZXDAT     0x00DD     //��ѹ����㵱ǰ�����ֵ
#define   PHDAT     0x00DE     //��ѹ��λ����
#define   T8BAUD    0x00E0//0x00DF     //��ǰ������ͨ��ʱ����8bit�½���ʱ������


#define   AnaCtrl1_Val    0x10000000

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

#define BPFPARA_ValueDef   0x806764B6
#define Actual_BAUDRate 4800//4800//9602
#define K_BAUDRate  (float)(Actual_BAUDRate * 0.00390625)

#define T8_BAUDRate (8 * 3276800 / Actual_BAUDRate)

#define BAUDRate_1Byte_OverTime  (float)(11*1000/Actual_BAUDRate*1.4)

#define Slave0  0
#define Slave1  1
#define Slave2  2
#define Slave3  3

#define SlaveNum    Slave0

#define HZCONST   409600

#define VARHZ   0x4c000

uint16_t gui_ReadABCPhy;
#define flgEtPhy_A 0x01
#define flgEtPhy_B 0x02
#define flgEtPhy_C 0x04
#define flgEtread    0x01
#define flgEtreadint 0x02
#define flgEtwrite   0x04
#define flgEtBwrite   0x08


uint8_t  guc_ABCreadbegian;



enum {
  Rac_Idle=0,
  Rac_Send,
  Rac_Rev,
  Rac_WaitPro,
};


typedef struct {
    uint32_t  RacPHC;             //�й����ʽǲ� BIT[24:16]ΪIBͨ��;BIT[8:0]ΪIAͨ��
    uint32_t  RacUC;              //��ѹ��Чֵ�Ȳ�Ĵ���
    uint32_t  RacIAC;             //A·������Чֵ�Ȳ�Ĵ���
    uint32_t  RacPAC;             //A·�й����ʱȲ�Ĵ���
    uint32_t  RacPADCC;           //A·�й����ʶ��β����Ĵ���                
    uint32_t  RacQADCC;           //A·�޹����ʶ��β����Ĵ���  
    uint32_t  RacIADCC;           //A·������Чֵ����У���Ĵ���
} S_JBRE;




typedef struct {
    uint32_t BPFPARA_Value[3];
    uint16_t ui_CRC;
} S_BPFPARA_Value;

S_BPFPARA_Value gs_BPFPARA_Value; 

uint32_t gul_FRQAVG_Value[3];

typedef struct {
  uint8_t ucStatus;               
  uint8_t ucSendLen;
  uint8_t ucSendPoint;
  uint8_t ucRevLen;
  uint8_t ucRevPoint;
  uint8_t ucBuf[100];
} S_RacCtrl;

S_RacCtrl gs_RacCtrl;

volatile uint8_t guc_CommDelayTime;    
extern const uint8_t flgphy[];
extern const uint8_t phy[];
extern const uint8_t phadd[];

typedef struct {
    uint8_t uc_Phystat;
    uint8_t uc_Physendtim;
    uint8_t uc_Sendstat;
    uint8_t uc_Powstat;
    uint8_t uc_flapara;
    uint8_t uc_Uartstat;
} S_RacPhy;

S_RacPhy gs_RacPhy;


enum {
	power_a = 0x01,
	power_b = 0x02,
	power_c = 0x04,
};

typedef union {
    uint32_t lword;
    uint16_t word[2];
    uint8_t  byte[4]; 
}Word32;

typedef struct
{
    uint32_t  ul_power[4];         
    uint32_t  ul_power_all[4];          
    uint16_t  ui_power_rem[4];
    uint32_t  ul_Npower[4];         
    uint32_t  ul_Npower_all[4];          
    uint16_t  ui_Npower_rem[4];
    uint32_t  ul_underpow;
    uint32_t  ul_Nunderpow;
}S_POWER;

S_POWER gs_power; 

typedef struct
{
    uint32_t  ul_U[3];         
    uint32_t  ul_I[3];    
    uint32_t  ul_P[4];
    uint32_t  ul_Hz[3];
}S_UI;

S_UI gs_UI; 

typedef struct
{
    uint32_t  ul_power[3];         
    uint32_t  ul_Npower[3];         
}S_POWER_ENERYCAL;

S_POWER_ENERYCAL gs_power_enerycal;
uint8_t guc_ReadingABC_Flg; //���ڶ���Чֵ�����ڼ��־

uint8_t  Accumulate_abc_times;

uint32_t  gul_dispbuf_total;
uint32_t  gul_dispbuf_timeoutA;
uint32_t  gul_dispbuf_timeoutB;
uint32_t  gul_dispbuf_timeoutC;
uint32_t  gul_dispbuf_crcerrA;
uint32_t  gul_dispbuf_crcerrC;

uint8_t  guc_CheckABCPhase_Dly;
uint16_t gui_SystemEvent;

uint8_t offtim1;
uint8_t offtim2;
uint8_t offtim3;

uint16_t gui_RefreshEvent;
#define flgEtPara_Fee           0x0001
#define flgEtPara_EnyBottom     0x0002    //����
#define flgEtPara_Config        0x0004
#define flgEtPara_RTC           0x0008
#define flgEtPara_Bode          0x0010    //����
#define flgEtPara_Show          0x0020    //����
#define flgEtPara_Init          0x0040    //����
#define flgEtPara_RtcParaFsToE2 0x0080
#define flgEtPara_LoadCurve     0x0100
#define flgEtPara_PowDir        0x0200
#define flgEtPara_FrsJsr        0x0400
#define flgEtPara_InitA         0x1000    //����
#define flgEtPara_InitB         0x2000    //����
#define flgEtPara_InitC         0x4000    //����


#endif



void Meter_V9260_Init(void);
void PreparePacket(uint8_t *buffer, uint16_t addr, uint8_t cmd, uint32_t data) ;
uint8_t ReadRaccoon(uint16_t Addr, uint8_t num, uint8_t useInterrupt, uint8_t Phyadd) ;
void Meter_V9260_Port_Init(void);
uint8_t WriteWithRetry(uint32_t data, uint16_t addr) ;
uint8_t CalculateChecksum(uint8_t *data, uint8_t length) ;
uint32_t Hex2BCD(uint32_t hex);
uint16_t IntHex2BCD(uint16_t hex);
uint8_t APIByteHEX2BCD(uint8_t ucHEX);
uint16_t do_CRC(uint8_t *str,uint8_t len);
void V9260_InitDefaultPara(void);
uint32_t Cal_Rms_Value(uint32_t addr, uint8_t value);
void CheckRegSum(void);
uint8_t Meter_V9260_write(uint32_t Data, uint16_t Addrm, uint8_t status);
uint8_t Meter_V9260_read(uint16_t Addr, uint8_t num, uint8_t status);
uint8_t UpdateRaccoonPara(void) ;

#endif




