#ifndef __MCUDRIVE_H__
    #define __MCUDRIVE_H__

#ifndef MCUDRIVEEXT
    #define MCUDRIVEEXT extern
#endif

#define SETPLL_CLOSE 0x00
#define SETPLL_800K 0x60
#define SETPLL_1_6M 0x65
#define SETPLL_3_2M 0x6a
#define SETPLL_6_4M 0x6f



#define SETLDO_25V 0
#define SETLDO_28V 1
#define SETLDO_22V 2
#define SETLDO_24V 3



#define SETLCD_30V 0x80 
#define SETLCD_33V 0
#define SETLCD_35V 0x10

#define RTCWAKEUP()     ((Systate&BIT2))    //RTC RST
#define IOWAKEUP()      ((Systate&BIT3))    //IO RST
#define POWERSTATE()    ((ANA->COMPOUT)&BIT8)//(!(Systate&BIT0))   //  0 有电   1 没电

//#define VERSION         XBYTE[0x286b]
#define OSCSTATE()      !((PMU->STS)&BIT2)//(VERSION&BIT7)        //晶振 1起振/0未起振


//#define CLRWDT()    {EA=0;wdt0=0xA5;wdt1=0x5a;EA=1;}
#define CLRWDT()    {PMU->WDTCLR = 0X55AAAA55;}//{wdt0=0xA5;wdt1=0x5a;wdt0=0xA5;wdt1=0x5a;}  //连续喂2次狗  zyj 11.2
#define SLPWDT()    {PMU->WDTCLR = 0X55AAAA55;}//{wdt0=0xA5;wdt1=0x5A;}


#define Init_RTC_SEC   0x00
#define Init_RTC_MIN   0x01
#define Init_RTC_HOUR  0x02


#define LCDSETTING4COM  0x82    //265hz  600K 4COM
#define LCDSETTING6COM  0x92    //265hz  600K 6com  


#define BATDIV      403948
#define BATLEVEL    2600
#define LCDREGLEN   17
#define BATOFFSET   77428752//23836396//5353488

#define BATON 1
#define BATDOWN 0

//载波 接口
//#define EVENTOUTHIGH() {P8OE&=(~BIT1);P8OD|=BIT1;}
//#define EVENTOUTLOW()  {P8OE&=(~BIT1);P8OD&=(~BIT1);}
//#define STAOUTHIGH()    {}
//#define STAOUTLOW()     {}
//#define FRRST()         {P8OE&=(~BIT0);P8OD&=(~BIT0);}  //输出低
//#define FRRSTNO()       {P8OE|=BIT0;P8IE&=(~BIT0);}     //禁止输入输出

//LED
//打开背光
#define OpenBeiGuang()      {}
//关闭背光
#define CloseBeiGuang()     {}


//按键
#define KeyPortIn()     {PMU->IOAOEN |= BIT4;PMU->IOAIE |= BIT4;}//{P1IE|=BIT4;P1OE|=BIT4;P14FS=0;}   //                                               

#define KeyUpEnable()   {PMU->IOAOEN |= BIT4;PMU->IOAIE |= BIT4;}//{P1IE|=BIT4;P1OE|=BIT4;P14FS=0;}
#define KeyUpDisable()  {PMU->IOAOEN |= BIT4;PMU->IOAIE &=~ BIT4;}//{P1IE&=(~BIT4);P1OE|=BIT4;}
#define KeyStUp()       (!(PMU->IOASTS & BIT4))                // 上翻按键

//#define KeyPrEnable()    {P9IE|=BIT0;P9OE|=BIT0;P9FC&=~BIT0;}
//#define KeyPrDisable()    {P9IE&=(~BIT0);P9OE|=BIT0;}
//#define KeyStPr()       (!(P9ID & BIT0))               // 编程按键
//硬件接口预留
//#define KeyBgEnable()    {P1IE|=BIT3;P1OE|=BIT3;P13FS=0;}
//#define KeyBgDisable()    {P1IE&=(~BIT3);P1OE|=BIT3;}
//#define KeyStBg()       (!(P1ID&BIT3))                           //表盖检测按键

//秒脉冲，多功能口
#define RATESWITCHIOHIGH() { P9OD|=(BIT3);}
#define RATESWITCHIOLOW() { P9OD&=(~BIT3);}


//#define AlarmLedEnable()     {P2IE&=(~BIT3);P2OE&=(~BIT3);P2OD&=(~BIT3);}  //增加报警灯P2.3 zyj 11.2
//#define AlarmLedDisable()    {P2IE&=(~BIT3);P2OE|=BIT3;P2OD|=BIT3;}

#define AlarmLedEnable()     {PMU->IOAIE&=(~BIT7);PMU->IOAOEN&=(~BIT7);PMU->IOADAT&=(~BIT7);}
#define AlarmLedDisable()    {PMU->IOAIE&=(~BIT7);PMU->IOAOEN|=BIT7;PMU->IOADAT|=BIT7;}


#define PHACtrlEnable()     {PMU->IOAIE&=(~BIT8);PMU->IOAOEN&=(~BIT8);PMU->IOADAT&=(~BIT8);}  //A相 P1.1 V9240控制 zyj 11.2
#define PHACtrlDisable()    {PMU->IOAIE&=(~BIT8);PMU->IOAOEN&=(~BIT8);PMU->IOADAT|=(BIT8);}
#define PHBCtrlEnable()     {PMU->IOAIE&=(~BIT9);PMU->IOAOEN&=(~BIT9);PMU->IOADAT&=(~BIT9);}  //B相 P1.2 V9240控制 zyj 11.2
#define PHBCtrlDisable()    {PMU->IOAIE&=(~BIT9);PMU->IOAOEN&=(~BIT9);PMU->IOADAT|=(BIT9);}
#define PHCCtrlEnable()     {PMU->IOAIE&=(~BIT11);PMU->IOAOEN&=(~BIT11);PMU->IOADAT&=(~BIT11);}  //C相 P1.3 V9240控制 zyj 11.2
#define PHCCtrlDisable()    {PMU->IOAIE&=(~BIT11);PMU->IOAOEN&=(~BIT11);PMU->IOADAT|=(BIT11);}




//#define PHACtrlEnable()     {P11FS = 0;P1IE&=(~BIT1);P1OE&=(~BIT1);P1OD&=(~BIT1);}  //A相 P1.1 V9240控制 zyj 11.2
//#define PHACtrlDisable()    {P11FS = 0;P1IE&=(~BIT1);P1OE&=(~BIT1);P1OD|=(BIT1);}
//#define PHBCtrlEnable()     {P12FS = 0;P1IE&=(~BIT2);P1OE&=(~BIT2);P1OD&=(~BIT2);}  //B相 P1.2 V9240控制 zyj 11.2
//#define PHBCtrlDisable()    {P12FS = 0;P1IE&=(~BIT2);P1OE&=(~BIT2);P1OD|=(BIT2);}
//#define PHCCtrlEnable()     {P13FS = 0;P1IE&=(~BIT3);P1OE&=(~BIT3);P1OD&=(~BIT3);}  //C相 P1.3 V9240控制 zyj 11.2
//#define PHCCtrlDisable()    {P13FS = 0;P1IE&=(~BIT3);P1OE&=(~BIT3);P1OD|=(BIT3);}

//#define PHACtrlEnable()     {P7OD&=(~BIT6);}  //A相 P7.6 V9260S控制 zyj 11.10
//#define PHACtrlDisable()    {P7OD|=(BIT6);}
//#define PHBCtrlEnable()     {P7OD&=(~BIT7);}  //B相 P7.7 V9260S控制 zyj 11.10
//#define PHBCtrlDisable()    {P7OD|=(BIT7);}
//#define PHCCtrlEnable()     {P8OD&=(~BIT0);}  //C相 P8.0 V9260S控制 zyj 11.10
//#define PHCCtrlDisable()    {P8OD|=(BIT0);}


MCUDRIVEEXT uint8 guc_PluseCount;
MCUDRIVEEXT uint8 guc_batstate;




void CPUInit(void);
void Init_Timer0(void);
void Init_Timer1(void);
 

void GetExtRTC(void);           //获取硬时钟
void SetExtRTC(void);           //设置硬时钟
uint8 SetPLL(uint8 fmcu);
void GetBat(void);
void DelayOSC(uint8 num);
void MChannelCal(void);
void ChangeBodeProc(void);
void SwichPluseOutType(uint8 type);
void MCUForPowerOff(void);
uint8 Sleep(void);
void UARTOFF(void);
void IntOFF(void);
void IOOFF(void);
uint8 SleepRTC(void);
uint8 SetPLL800K(void);
uint8 SetPLL3DOT2M(void);
void RTCWakeUpTm(uint8 Tm);
void BgCtrFun(void);
void PowUpLedCtl(void);
void EnableRTCInt(void);
void DisableRTCInt(void);
void Check_Sfr(void);
void EventOutProc(void);

#endif
