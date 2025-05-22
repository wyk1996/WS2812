#include "bsp_meter_v9260.h"
#include "drv_common.h"
#include <stdio.h>
#include "Includes.h"
#include "IndependentDataType.h"


const uint8_t  gCuc_HEX2BCDMap[] =
{// 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, //0-9
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, //10-19
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, //20-29
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, //30-39
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, //40-49
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, //50-59
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, //60-69
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, //70-79
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, //80-89
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, //90-99
};

const uint16_t CrcTab[256] =
{ // X16+X12+X5+1 ?????
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

#define Actual_BAUDRate 4800//4800//9602
#define BAUDRate_1Byte_OverTime  (float)(11*1000/Actual_BAUDRate*1.4)

#define Actual_BAUDRate 4800//4800//9602
#define K_BAUDRate  (float)(Actual_BAUDRate * 0.00390625)

#define   SysCtrl   	0x0180 
#define   BPFPARA 		0x0107  
#define   UC      		0x00FF 
#define   IAC     		0x00FD    
#define   PAC     		0x00F6
#define   PADCC   		0x00F8
#define   QADCC   		0x00FB

#define   QAC     		0x00F9      //A路无功功率比差校正
#define   PHC     		0x00F7      //有功功率角差校正
#define   IADCC   		0x00FE      //A路电流有效值小信号校正
#define   T8BAUD        0x00E0      //0x00DF //T8波特率寄存器
#define   SysSts        0x00CA      //系统状态寄存器

#define     PAAVG       0x00D0      //A路平均有功功率值寄存器
#define     UAVG        0x00D4      //平均电压值寄存器
#define     IAAVG       0x00D3      //A路平均电流值寄存器
#define     DATAFREQ    0x011D      //数据频率寄存器

#define     ULEVEL      200         //电压等级

//是否需要？
#define SysCtrl_ValueDef   0x2C054000   // +40 PPM
#define SysCtrl_ValueA   0x2C054000   // +40 PPM
// #define SysCtrl_ValueB   0x2C054000   // +40 PPM
// #define SysCtrl_ValueC   0x2C054000   // +40 PPM

#define RacWrite 0x02 //写入数据
#define RacRead  0x01 //读取数据
#define flgEtwrite   0x04 //写入标志位
#define flgEtread    0x01 //读取标志位
#define   CKSUM   0x0109  //校验和寄存器
#define flgEtBwrite   0x08 //广播写入标志位
#define RacBroadcast 0 //广播地址
#define flgEtreadint 0x02 //读取中断标志位
#define   AnaCtrl1_Val    0x10000000 //模拟量控制寄存器1
#define flgEtPara_InitA         0x1000 //A路参数初始化标志位
#define BPFPARA_ValueDef   0x806764B6 //带通滤波器参数寄存器值


const uint32_t SysCtrl_Value = SysCtrl_ValueA;//系统控制寄存器值
uint8_t guc_CommDelayTime;  //延时时间
uint8_t  guc_CheckABCPhase_Dly; //延时标志位
uint8_t i,tmp;//临时变量
uint32_t gul_FRQAVG_Value[3]; //频率平均值寄存器
uint16_t gui_RefreshEvent; //刷新事件

typedef union
{
    uint16_t word;
    uint8_t  byte[2];
}Word16;

typedef struct
{
       uint16_t    undefine0:1;       //
       uint16_t    DemandType:1;      //
       uint16_t    RTCCell:1;         //RTC
       uint16_t    MeterCell:1;       //
       uint16_t    PDir:1;            //
       uint16_t    CDir:1;            //
       uint16_t    undefine6:1;       
       uint16_t    undefine7:1;       //
       uint16_t    undefine8:1;       
       uint16_t    undefine10:1;      //
       uint16_t    undefine11:1;      //
       uint16_t    undefine12:1;      //
       uint16_t    undefine13:1;      //
       uint16_t    undefine14:1;      //????
       uint16_t    undefine15:1;      //????
}GS_RUNCODE1;

typedef union 
{
    Word16 Word;
    GS_RUNCODE1 RunCode1Word;
}GT_RUNCODE1;


typedef struct 
{
    GT_RUNCODE1 ui_MeterState1;                  //
    // GT_RUNCODE2 ui_MeterState2;                  //???????????2
    // GT_RUNCODE3 ui_MeterState3;                  //???????????3
    // GT_RUNCODE4 ui_MeterState4;                  //???????????4
    // GT_RUNCODE5 ui_MeterState5;                  //???????????5
    // GT_RUNCODE6 ui_MeterState6;                  //???????????6
    // GT_RUNCODE7 ui_MeterState7;                  //???????????7
} METERSTATE;
METERSTATE gs_MeterState;



uint16_t  RegAddr[] = {
    SysCtrl,                    //系统控制寄存器
    BPFPARA,                    //带通滤波器参数寄存器
    UC,                         //电压有效值比差校正寄存器
    IAC,                        //A路电流有效值比差校正  
    PAC,                        //A通道有功功率比差校正
    PADCC,                      //A通道有功功率小信号校正
    QADCC,                      //A无功功率小信号校正
};


typedef struct {
    uint8_t ucStatus;               
    uint8_t ucSendLen;
    uint8_t ucSendPoint;
    uint8_t ucRevLen;
    uint8_t ucRevPoint;
    uint8_t ucBuf[100];
} S_RacCtrl;
 S_RacCtrl gs_RacCtrl;

typedef struct {
    uint8_t uc_Phystat;
    uint8_t uc_Physendtim;
    uint8_t uc_Sendstat;
    uint8_t uc_Powstat;
    uint8_t uc_flapara;
    uint8_t uc_Uartstat;
} S_RacPhy;
 S_RacPhy gs_RacPhy;

#if 1
enum {
    Rac_Idle=0,
    Rac_Send,
    Rac_Rev,
    Rac_WaitPro,
};
#endif

typedef struct {
    uint32_t  RacPHC;             //
    uint32_t  RacUC;              //
    uint32_t  RacIAC;             //
    uint32_t  RacPAC;             //
    uint32_t  RacPADCC;           //             
    uint32_t  RacQADCC;           //
    uint32_t  RacIADCC;           //
} S_JBRE;

typedef struct {
    uint16_t ui_MeterC;           // 
    uint16_t ui_Un;               // 
    uint16_t ui_Ib;               // 
    uint16_t ui_Resve1;           //
    
    uint32_t  RacEGYTH;           //
    uint32_t  RacREEGYTH;         //
    
    uint32_t  RacSysCtrl;         //
    uint32_t  RacBPFPARA;         //
    
    S_JBRE  gs_JBA;               //
    uint32_t  RacMTPARA0;         //MTPARA0寄存器

    uint32_t ul_PG;               //
    uint32_t ul_URmG;             //
    uint32_t ul_I1RmG;            //
    
    uint16_t ui_Resve2;           //
    uint16_t ui_JbCRC;            // 
} S_JBPM;

 S_JBPM gs_JbPm;                   //

typedef struct {
    uint32_t BPFPARA_Value;
    uint16_t ui_CRC;
} S_BPFPARA_Value;

 S_BPFPARA_Value gs_BPFPARA_Value; 

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

extern S_POWER_ENERYCAL gs_power_enerycal;


//辅助函数:重试写入数据
uint8_t WriteWithRetry(uint32_t data, uint16_t addr) 
{
    uint8_t result = Meter_V9260_write(data, addr, Rac_Send);
    if(result == FALSE) {
        //重试一次
        result = Meter_V9260_write(data, addr, Rac_Send);
    }
    return result;
}

// 辅助函数:计算校验和
uint8_t CalculateChecksum(uint8_t *data, uint8_t length) 
{
    uint8_t sum = 0;
    for(uint8_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return ~sum + 0x33;
}

//辅助函数 - 添加通用的通信包准备函数
void PreparePacket(uint8_t *buffer, uint16_t addr, uint8_t cmd, uint32_t data) 
{
    buffer[0] = 0x7D;
    buffer[1] = (uint8_t)((addr & 0x0f00) >> 4) + cmd;
    buffer[2] = (uint8_t)(addr & 0x00ff);
    
    if(cmd == RacRead) {
        // 读取命令包的特殊处理
        buffer[3] = (uint8_t)data; // num参数
        buffer[4] = 0;
        buffer[5] = 0;
        buffer[6] = 0;
    } else {
        // 写入命令包
        buffer[3] = (uint8_t)(data & 0xFF);
        buffer[4] = (uint8_t)((data >> 8) & 0xFF);
        buffer[5] = (uint8_t)((data >> 16) & 0xFF);
        buffer[6] = (uint8_t)((data >> 24) & 0xFF);
    }
    
    buffer[7] = CalculateChecksum(buffer, 7);
    drv_common_usart0_send(buffer, 8); // 发送数据
}

void Meter_V9260_Port_Init(void)
{
    //meter中断引脚
	gpio_mode_set(RCD_INT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, RCD_INT_PIN);
	gpio_output_options_set(RCD_INT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCD_INT_PIN);
}

void Meter_V9260_Init(void)
{
    
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_9);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_bit_reset(GPIOA, GPIO_PIN_9);   

    delay_1ms(70);
    gpio_bit_set(GPIOA, GPIO_PIN_9);
    delay_1ms(70);
}


/*=========================================================================================\n
* @function_name: V9281_InitDefaultPara
* @function_file: Raccoon.c
* @描述: 初始化默认参数
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void V9260_InitDefaultPara(void)
{
    gs_JbPm.ui_MeterC = 1200;             // 表常数
    gs_JbPm.ui_Un = 22000;                // 标称电压
    gs_JbPm.ui_Ib = 5000;                 // 标称电流    
    gs_JbPm.ui_Resve1=0;
    gs_JbPm.RacEGYTH   = 0x067215D8;    //0x0181 能量累加门限值 6400 5A 0x013A1A40
    gs_JbPm.RacREEGYTH = 0x067215D8;    //0x0181 能量累加门限值 6400 5A 0x013A1A40
    #if 0
    gs_JbPm.RacCTH     = 0x00000000;    //0x0182 起动，潜动门限值
    gs_JbPm.RacRECTH   = 0x00000000;    //0x0182 起动，潜动门限值
    gs_JbPm.RacANCtrl0 = 0xDf0004C0;    //0x185 增益I:32，U:1，M:4 温度系数，全局电流偏置
    gs_JbPm.RacANCtrl1 = 0x30000000;    //0x186 
    gs_JbPm.RacANCtrl2 = 0x0000000F;    //0x187 
    #endif
    gs_JbPm.RacSysCtrl = SysCtrl_Value;  //0x180 不参与校验 时钟3.2MHz
    gs_JbPm.RacBPFPARA = BPFPARA_ValueDef;    //0x0125 带通滤波器系数 0x811D2BA7

    gs_JbPm.RacMTPARA0 = 0xE0000002;    //0x183 CF工作时钟3.2MHz 使能CF输出，能量累加，起动潜动，能量来源：平均全波有功  
    gs_JbPm.gs_JBA.RacPHC = 0x00000000;      //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差   
    gs_JbPm.gs_JBA.RacUC = 0x00000000;        //全波电压有效值比差   
    gs_JbPm.gs_JBA.RacIAC = 0x00000000;        //全波电流有效值比差
    gs_JbPm.gs_JBA.RacPAC = 0x00000000;         //全波有功功率比差
    gs_JbPm.gs_JBA.RacPADCC = 0x00000000;        //全波有功功率二次补偿值
    gs_JbPm.gs_JBA.RacQADCC = 0x00000000;      //全波无功功率二次补偿值
    
    gs_JbPm.ui_Resve2=0;
    gs_JbPm.ul_PG=1;               //功率比例系数
    gs_JbPm.ul_URmG=1;             //电压通道比例系数
    gs_JbPm.ul_I1RmG=1;            //电流通道1比例系数

    gs_JbPm.ui_JbCRC=do_CRC((uint8_t*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果

}


uint16_t do_CRC(uint8_t *str,uint8_t len)
{
    uint8_t ucData;
    uint16_t crc;

    crc = 0x5555;

    for( ;len > 0; len--)
    {
        ucData = (uint8_t) (crc / 0x100);    //取CRC的高8位
        crc <<= 8;                          //CRC左移8位    
        crc ^= CrcTab[ucData ^ (*str)];     //CRC高8位与数据异或
        str++;
    }

    return crc;
}

//初始化函数参数更新
uint8_t UpdateRaccoonPara(void) 
{
    uint8_t success = TRUE;


    //写入系统控制寄存器
    if(!WriteWithRetry(gs_JbPm.RacSysCtrl, RegAddr[0])){
        success = FALSE;
    }

    //写入带通滤波器参数寄存器
    if(!WriteWithRetry(gs_JbPm.RacBPFPARA, RegAddr[1])){
        success = FALSE;
    }

    //写入电压有效值比差校正寄存器
    if(!WriteWithRetry(gs_JbPm.gs_JBA.RacUC, RegAddr[2])){
        success = FALSE;
    }

    // 写入A路电流有效值比差校正寄存器
    if(!WriteWithRetry( gs_JbPm.gs_JBA.RacIAC, RegAddr[3])) {
        success = FALSE;
    }

    // 写入A路有功功率比差校正寄存器
    if(!WriteWithRetry(gs_JbPm.gs_JBA.RacPAC, RegAddr[4])) {
        success = FALSE;
    }

    // 写入A路有功功率小信号校正寄存器
    if(!WriteWithRetry(gs_JbPm.gs_JBA.RacPADCC, RegAddr[5])) {
        success = FALSE;
    }

    // 写入A路无功功率小信号校正寄存器
    if(!WriteWithRetry(gs_JbPm.gs_JBA.RacQADCC, RegAddr[6])) {
        success = FALSE;
    }


    return success;
}


//写函数
uint8_t Meter_V9260_write(uint32_t Data, uint16_t Addr, uint8_t status) 
{
    uint8_t i, ucSum = 0;
    gs_RacPhy.uc_Uartstat = status;
    
    if(gs_RacPhy.uc_Uartstat != 0) {
        return FALSE; // UART 正在使用中，返回错误
    }
    
    // 准备数据包
    gs_RacCtrl.ucBuf[0] = 0x7D;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacWrite;
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr & 0x00ff);
    
    gs_RacCtrl.ucBuf[3] = (uint8_t)(Data & 0xFF);
    gs_RacCtrl.ucBuf[4] = (uint8_t)((Data >> 8) & 0xFF);
    gs_RacCtrl.ucBuf[5] = (uint8_t)((Data >> 16) & 0xFF);
    gs_RacCtrl.ucBuf[6] = (uint8_t)((Data >> 24) & 0xFF);
    
    // 计算校验和
    for(i = 0; i < 7; i++) {
        ucSum += gs_RacCtrl.ucBuf[i];
    }
    gs_RacCtrl.ucBuf[7] = ~ucSum + 0x33;
    
    // 设置传输参数
    #if 0
    gs_RacCtrl.ucStatus = Rac_Send; // 设置状态为发送
    gs_RacCtrl.ucSendLen = 8;// 设置发送长度
    gs_RacCtrl.ucSendPoint = 0;// 设置发送指针
    gs_RacCtrl.ucRevLen = 4;// 设置接收长度
    gs_RacCtrl.ucRevPoint = 0;// 设置接收指针
    gs_RacPhy.uc_Uartstat |= flgEtwrite; // 设置发送标志
    #endif
    drv_common_usart0_send(gs_RacCtrl.ucBuf, 8); // 发送数据
    delay_1ms(10); // 延时10ms，确保数据发送完成


    #if 0
    // 启动传输并等待完成
    guc_CommDelayTime = (uint8_t)((BAUDRate_1Byte_OverTime * 12 + 10) / 10);
    while(gs_RacCtrl.ucStatus != Rac_WaitPro) {
        if(guc_CommDelayTime == 0) {
            gs_RacPhy.uc_Uartstat &= ~flgEtwrite; // 清除发送标志
            return FALSE;
        }
    }
   
    // 验证响应
    gs_RacPhy.uc_Uartstat &= ~flgEtwrite; // 清除发送标志
    #endif

    ucSum = 0;
    for(i = 0; i < 3; i++) {
        ucSum += gs_RacCtrl.ucBuf[i];
    }
    ucSum = ~ucSum + 0x33;
    
    if(gs_RacCtrl.ucBuf[3]==ucSum)
    {
        return TRUE;
    } else{
        return FALSE;
    }
}



//读函数
uint8_t Meter_V9260_read(uint16_t Addr, uint8_t num, uint8_t status)
{
    uint8_t i, ucSum = 0;
    gs_RacPhy.uc_Uartstat = status;

    if(gs_RacPhy.uc_Uartstat != 0) {
        return FALSE;
    }
    
    // 准备数据包
    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacRead;
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr & 0x00ff);
    gs_RacCtrl.ucBuf[3] = num;
    gs_RacCtrl.ucBuf[4] = 0;
    gs_RacCtrl.ucBuf[5] = 0;
    gs_RacCtrl.ucBuf[6] = 0;
    
    // 计算校验和
    for(i = 0; i < 7; i++) {
        ucSum += gs_RacCtrl.ucBuf[i];
    }
    gs_RacCtrl.ucBuf[7] = ~ucSum + 0x33;
    
    #if 0
    // 设置传输参数
    gs_RacCtrl.ucStatus = Rac_Send;
    gs_RacCtrl.ucSendLen = 8;
    gs_RacCtrl.ucSendPoint = 0;
    gs_RacCtrl.ucRevLen = (num * 4) + 4;
    gs_RacCtrl.ucRevPoint = 0;
    gs_RacPhy.uc_Uartstat |= flgEtread;
    #endif
    
    drv_common_usart0_send(gs_RacCtrl.ucBuf, 8); // 发送数据
    delay_1ms(10); // 延时10ms，确保数据发送完成

    #if 0
    // 启动传输并等待完成
    guc_CommDelayTime = (uint8_t)((BAUDRate_1Byte_OverTime * (4 * num + 12) + 10) / 10);
    while(gs_RacCtrl.ucStatus != Rac_WaitPro) {
        if(guc_CommDelayTime == 0) {
            gs_RacPhy.uc_Uartstat &= ~flgEtread;
            return FALSE;
        }
    }
    
    // 验证响应
    gs_RacPhy.uc_Uartstat &= ~flgEtread;
    #endif

    ucSum = 0;
    for(i = 0; i < (num * 4) + 3; i++) {
        ucSum += gs_RacCtrl.ucBuf[i];
    }
    ucSum = ~ucSum + 0x33;
    
    if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
    {
        return TRUE;
    } else{
        return FALSE;
    } 
}




/*=========================================================================================\n
* @function_name: Clashin
* @function_file: EnergyBottom.c
* @描述  系统控制寄存器自检错误中断标志位 检测
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-2-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CHKERR_Bit_scan(void) 
{
    Word32 status;
    
    if(Meter_V9260_read(SysSts, 1, Rac_Rev)) {
        memcpy((uint8_t*)status.byte, gs_RacCtrl.ucBuf + 3, 4);
        
        // 检查CHKERR位（位2）
        if(status.lword & 0x0004) {
            gui_RefreshEvent |= flgEtPara_InitA;
        }
    }
}



/*=========================================================================================\n
* @function_name: CheckRegSum
* @function_file: Raccoon.c
* @描述: 检测校验和
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//合并CheckRegSum和CheckRegSumABC函数
//CheckRegSum函数 - 合并两个校验和函数：
void CheckRegSum(void)
{
    uint32_t ucSum = 0;
    
    // 计算校验和
    ucSum = SysCtrl_Value +
            gs_BPFPARA_Value.BPFPARA_Value +
            gs_JbPm.gs_JBA.RacIAC +
            gs_JbPm.gs_JBA.RacUC +
            gs_JbPm.gs_JBA.RacPAC +
            gs_JbPm.gs_JBA.RacPAC + // 无功比差
            gs_JbPm.gs_JBA.RacPADCC +
            gs_JbPm.gs_JBA.RacQADCC +
            gs_JbPm.gs_JBA.RacPHC +
            gs_JbPm.gs_JBA.RacIADCC +
            AnaCtrl1_Val;
    
    // 写入校验和寄存器
    Meter_V9260_write(0xFFFFFFFF - ucSum, CKSUM, Rac_Send);
}


#if 0
//合并ReadRaccoon和ReadRaccoonInt函数这两个函数有很多相似代码，可以合并为一个带标志的函数：
uint8_t ReadRaccoon(uint16_t Addr, uint8_t num, uint8_t useInterrupt, uint8_t Phyadd) 
{
    uint8_t i, ucSum = 0;
    
    if(gs_RacPhy.uc_Uartstat != 0) {
        return FALSE;
    }
    
    // 准备数据包
    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacRead;
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr & 0x00ff);
    gs_RacCtrl.ucBuf[3] = num;
    gs_RacCtrl.ucBuf[4] = 0;
    gs_RacCtrl.ucBuf[5] = 0;
    gs_RacCtrl.ucBuf[6] = 0;
    
    // 计算校验和
    for(i = 0; i < 7; i++) {
        ucSum += gs_RacCtrl.ucBuf[i];
    }
    gs_RacCtrl.ucBuf[7] = ~ucSum + 0x33;
    
    // 设置传输参数
    gs_RacCtrl.ucStatus = Rac_Send;
    gs_RacCtrl.ucSendLen = 8;
    gs_RacCtrl.ucSendPoint = 0;
    gs_RacCtrl.ucRevLen = (num * 4) + 4;
    gs_RacCtrl.ucRevPoint = 0;
    
    if(useInterrupt) {
        // 使用中断方式
        gs_RacPhy.uc_Physendtim = (uint8_t)((BAUDRate_1Byte_OverTime * (4 * num + 12) + 10) / 10);
        gs_RacPhy.uc_Phystat = Phyadd;
        gs_RacPhy.uc_Sendstat = TRUE;
        gs_RacPhy.uc_Uartstat |= flgEtreadint;
        return TRUE; // 中断模式直接返回，由中断处理后续操作
    } else {
        // 使用轮询方式
        gs_RacPhy.uc_Uartstat |= flgEtread;
        guc_CommDelayTime = (uint8_t)((BAUDRate_1Byte_OverTime * (4 * num + 12) + 10) / 10);
        
        while(gs_RacCtrl.ucStatus != Rac_WaitPro) {
            if(guc_CommDelayTime == 0) {
                gs_RacPhy.uc_Uartstat &= ~flgEtread;
                return FALSE;
            }
        }
        
        // 验证响应
        gs_RacPhy.uc_Uartstat &= ~flgEtread;
        ucSum = 0;
        for(i = 0; i < (num * 4) + 3; i++) {
            ucSum += gs_RacCtrl.ucBuf[i];
        }
        ucSum = ~ucSum + 0x33;
        
        return (gs_RacCtrl.ucBuf[num * 4 + 3] == ucSum);
    }
}
#endif










uint32_t Hex2BCD(uint32_t hex)
{
    uint32_t result = 0;
    uint32_t cmp, bitcap;
    uint8_t uc_i;

    while(hex > 99999999)
    {
        hex-=100000000;
        //return 0xFFFFFFFF;
    }

    for(cmp = 80000000, bitcap = 0x80000000, uc_i = 0; uc_i < 16; uc_i++)
    {
        if(hex >= cmp)
        {
            result |= bitcap;
            hex -= cmp;
        }
        bitcap >>= 1;
        switch(uc_i)
        {
            case 3:
                cmp = 8000000;
                break;
            case 7:
                cmp = 800000;
                break;
            case 11:
                cmp = 80000;
                break;
            default:
                cmp >>= 1;
        }
    }

    result |= IntHex2BCD(hex);
    return result;
}


uint16_t IntHex2BCD(uint16_t hex)
{
    uint16_t result = 0;

    if(hex > 9999)
    {
        return 0xFFFF;
    }

    if(hex >= 8000)
    {
        result |= 0x8000;
        hex -= 8000;
    }

    if(hex >= 4000)
    {
        result |= 0x4000;
        hex -= 4000;
    }

    if(hex >= 2000)
    {
        result |= 0x2000;
        hex -= 2000;
    }

    if(hex >= 1000)
    {
        result |= 0x1000;
        hex -= 1000;
    }

    if(hex >= 800)
    {
        result |= 0x800;
        hex -= 800;
    }

    if(hex >= 400)
    {
        result |= 0x400;
        hex -= 400;
    }

    if(hex >= 200)
    {
        result |= 0x200;
        hex -= 200;
    }

    if(hex >= 100)
    {
        result |= 0x100;
        hex -= 100;
    }

    result |= APIByteHEX2BCD(hex);
    return result;

}

uint8_t APIByteHEX2BCD(uint8_t ucHEX)
{
    if(ucHEX > 99)
    {
        return 0xFF;
    }
    return gCuc_HEX2BCDMap[ucHEX];
}

/*=========================================================================================\n
* @function_name: CalRMS
* @function_file: EnergyBottom.c
* @描述: 计算瞬时量
* 
* 
* @参数: 
* @param:addr  寄存器地址
* 
* @返回: 
* @return: uint32 返回有效值
* @作者:   lwb (2012-05-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  hjj
* @修改内容: 添加phadd, 添加相位选择
===========================================================================================*/
uint32_t Cal_Rms_Value(uint32_t addr, uint8_t value)
{
    #if 1
    Word32 TempValue,TempValue3;
    uint32_t TempPara,TempValue1,TempValue2;

    if(Meter_V9260_read(addr, value, Rac_Rev))
    {
        memcpy((uint8_t*)TempValue.byte,gs_RacCtrl.ucBuf+3,4);
        memcpy((uint8_t*)TempValue3.byte,gs_RacCtrl.ucBuf+7,4);
        
    }else
    {
        TempValue.lword=0;
        TempValue3.lword=0;
    }
    #endif

    switch (addr)
    {
      case PAAVG:
          if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
          {
              TempValue.lword=~TempValue.lword+1;
          }

          TempPara=gs_JbPm.ul_PG;        //读取比例系数

          if(TempPara==0)
          {
              TempValue1=0;
          }
          else
          {
               TempValue.lword=((TempValue.lword)/TempPara);
          }
          if(TempValue.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
          {
               TempValue1=0;
          }
          else
          {
               TempValue1=Hex2BCD(TempValue.lword);

               if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==TRUE)   //反向添加负号
               {
                   TempValue1|=0x800000;
               }
          }
          break;
      case UAVG:
           TempPara=gs_JbPm.ul_URmG;   //ReadRMSPara(1);
           if(TempPara==0)
           {
              return 0;
           }
           else
           {
               TempValue.lword=TempValue.lword/TempPara;
           }

           if(TempValue.lword<ULEVEL)
           {
               return 0;
           }else
           {
               TempValue1=Hex2BCD(TempValue.lword);
           }
          // break;
      case IAAVG:
          //电流的计算
          TempPara=gs_JbPm.ul_I1RmG;    //ReadRMSPara(2);
          if(TempPara==0)
          {
              return 0;
          }
          else
          {
              TempValue3.lword=TempValue3.lword/TempPara;
          }

          if(TempValue3.lword<((gs_JbPm.ui_Ib/1000)*2))
          {
              return 0;
          }else
          {

              TempValue2=Hex2BCD(TempValue.lword);
              //功率的计算
              /*if(ReadRaccoon(DATAP,1,phyadd))
              {
                  memcpy((uint8_t*)TempValue2.byte,gs_RacCtrl.ucBuf+3,4);
              }else
              {
                  TempValue2.lword=0;
              }

              if(TempValue2.byte[3]>0x7f)                         //如果是负数的数，先变成正数
              {
                  TempValue2.lword=~TempValue2.lword+1;
              }

              TempPara=gs_JbPm.ul_PG;        //读取比例系数

              if(TempPara==0)
              {
                  TempValue2.lword=0;
              }
              else
              {
                  TempValue2.lword=((TempValue2.lword)/TempPara);
              }

              if(TempValue2.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
              {
                  //如果功率小于启动功率，这是电流小于0.2A,电流清零
                  if(TempValue1<0x200)
                  {
                      TempValue1=0;
                  }*/


              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==TRUE)   //反向添加负号
              {
                  TempValue2|=0x800000;
              }
            gs_UI.ul_I[value >> 2] =  TempValue2;  
          }
          break;
            #if 0  
                case RMSI2:
                TempPara=gs_JbPm.ul_I2RmG;    //ReadRMSPara(3);
                if(TempPara==0)
                {
                    return 0;
                }
                else
                {
                    TempValue.lword=TempValue.lword/TempPara;
                }

                if(TempValue.lword<IBLEVEL)
                {
                    return 0;
                }else
                {
                    TempValue1=Hex2BCD(TempValue.lword);

                    if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
                    {
                        TempValue1|=0x800000;
                    }
                }
            break;
      
            case DATAFREQ:
                TempValue.lword=(81920000)/TempValue.lword;

                if(TempValue.lword>4950&&TempValue.lword<5050)
                {
                    TempValue.lword=5000;
                }else if(TempValue.lword<4500)
                {
                    TempValue.lword=4500;
                }else if(TempValue.lword>5500)
                {
                    TempValue.lword=5500;
                }
                TempValue1=Hex2BCD(TempValue.lword);
            break;

           
            case RMSIU:
                TempPara=gs_JbPm.ul_URmG/65536;   //ReadRMSPara(1);
                if(TempPara==0)
                {
                return 0;
                }
                else
                {
                    TempValue.lword=TempValue.lword/TempPara;
                }

                if(TempValue.lword<ULEVEL)
                {
                    return 0;
                }else
                {
                    TempValue1=TempValue.lword;
                }
            break;
            #endif
            default:
                TempValue1=0;
            break;
    }
    return TempValue1;

}






