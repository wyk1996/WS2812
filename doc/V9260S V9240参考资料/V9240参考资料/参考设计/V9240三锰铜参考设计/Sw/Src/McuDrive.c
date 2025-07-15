#define MCUDRIVEEXT
#include "Include.h"
#define V9801A1

/*=========================================================================================\n
* @function_name: Init_Port
* @function_file: main.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-7-26)
* @修改内容:  
===========================================================================================*/
void Init_Port(void)
{
    //JTAG
    P0OE&=(~(BIT0+BIT1+BIT2+BIT3)); //JTAG 输出 0
    P0OD&=(~(BIT0+BIT1+BIT2+BIT3));


    //CF1
//    P9OE&=(~BIT6);       //CF1输出      // CF改用IO翻转 zyj 11.2
//    P9OD&=(~BIT6);       //输出低电平
//    P9FC|=BIT6;          //CF1输出

//    P9OE&=(~BIT5);       //CF1输出
//    P9OD&=(~BIT5);       //输出低电平
//    P9FC|=BIT5;          //CF1输出
    //CT485   配置in uart4.h
//    P9OE&=(~BIT5);       //CF1输出
//    P9IE&=(~BIT5);       //输出低电平
//    P9FC&=~BIT5;         //IO
    //RTC
//    P9OE&=(~BIT3);       //rtc输出  //  P9.3用于输出CF  zyj 11.2
//    P9IE&=(~BIT3);
//    P9OD |=(BIT3);       //输出低电平
//    P9FC&=(~BIT3);          //rtc输出  

    //COM+SEG
    //BG                
    //P8OE&=~BIT2;        //背光输出
   // P8IE&=~BIT2;
    //EEPROM
//    P9FC&=(~(BIT1+BIT2));
//    P9OE&=(~(BIT1+BIT2));    //输出
//    P9OD|=(BIT1+BIT2);       //输出1  
    
   //no foot    
   P1OE&=~(BIT0);       //输出0
   P1OD&=~(BIT0);

   P9OE&=~(BIT3+BIT7);       //P9.3模拟有功CF输出;P9.7模拟无功CF输出
   P9OD&=~(BIT3+BIT7);

   P4OE&=~(BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);       //输出0
   P4OD&=~(BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

   P6OE&=~(BIT0+BIT1);       //输出0
   P6OD&=~(BIT0+BIT1);

   P7OE&=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);       //输出0
   P7OD&=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);

   P2OE&=~(BIT2);       //输出0
   P2OD&=~(BIT2);
   AlarmLedDisable();

   P10OE&=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);       //输出0
   P10OD&=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
//#ifdef E2_WP
//   P9OE&=~(BIT4);
//   P9IE&=~BIT4;
//   P9DO|=((BIT4));          //禁止E2_wp
//#else
//   P9OE|=(BIT4);
//   P9IE|=(~(BIT4));     //高阻
//#endif

   //RS485
   P2OE&=(~BIT5);       //TXD 允许输出
   P2IE&=(~BIT5);       //TXD禁止输出
   P2OE|=(BIT4);        //rxd 禁止输出
   P2IE|=(BIT4);        //RXD 允许输入
   P24FS=2;             //RXD
   P25FS=2;  
 
   //9260_serial
   P2OE|=(BIT0);        //rxd 禁止输出
   P2IE|=(BIT0);        //RXD 允许输入
   P2OE&=(~BIT1);       //TXD 允许输出
   P2IE&=(~BIT1);       //TXD禁止输出
   P20FS=2;             //RXD
   P21FS=2;             //TXD
//   Uart4_RevDisable(); //关闭接收中断

//   P1OE&=~(BIT3+BIT1+BIT2);  //9240 通信控制脚 zyj 11.4
//   P1IE&=~(BIT3+BIT1+BIT2);
//   P1OD|=(BIT3+BIT1+BIT2);
   
   P7OE&=~(BIT6+BIT7);  //9260S 通信控制脚 zyj 11.10 直接输出低，不做控制
   P7IE&=~(BIT6+BIT7);
   P7OD&=~(BIT6+BIT7);
   P8OE&=~BIT0;
   P8IE|=BIT0;
   P8OD&=~BIT0;
   
//   P1OE|=(BIT3+BIT1+BIT2);  //9260S三个中断 高阻
//   P1IE&=~(BIT3+BIT1+BIT2);
//   P11FS = 0;
//   P12FS = 0;
//   P13FS = 0;
   PHACtrlDisable();
   PHBCtrlDisable();
   PHCCtrlDisable();
//    P1OE|=(BIT3+BIT1+BIT2);  //9260S三个中断输入
//    P1IE|=(BIT3+BIT1+BIT2);
//    P11FS = 4;    //端口2中断 A
//    P12FS = 4;   //端口3中断 B
//    P13FS = 3;  //端口0中断 C
//  //中断使能
//    IT0 = 1;        //端口0中断配置下降沿
//    EX0= 1;     // 端口0中断使能
//    ExInt4IE|=(BIT2+BIT3);// 端口2、3中断使能
//    EIE|=BIT2;// 端口2、3中断使能
  
  }
    
/*=========================================================================================\n
* @function_name: Init_Timer0
* @function_file: main.c
* @描述: 初始化定时器0
* 
* @参数: 
* @返回: 
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-8-8)
* @修改内容:  
===========================================================================================*/
void Init_Timer0(void)
{
    CKCON &= ~BIT3;         //1/12clk
    TMOD &=0xf0;
    TMOD |= BIT0;           // 定时器为方式1 16位计数器
    TMOD &=~ BIT1;          // 定时器为方式1 16位计数器
#if ( Fmcu_Val == Fmcu_3M2)
  #if ( CF_TimeBase == CF_TimeBase_1MS)
    TH0 = 0xFE;             //定时1ms   3.2M
    TL0 = 0xEF; 
  #elif ( CF_TimeBase == CF_TimeBase_20MS)
    TH0 = 0xEA;             //定时20ms   3.2M
    TL0 = 0xAC;
  #else
    #error "CF_TimeBase is error"
  #endif
    
#elif ( Fmcu_Val == Fmcu_6M5)
  #if ( CF_TimeBase == CF_TimeBase_1MS)
    TH0 = 0xFD;             //定时1ms   6.5M
    TL0 = 0xDE;
  #elif ( CF_TimeBase == CF_TimeBase_20MS)  
    TH0 = 0xD5;             //定时20ms   6.5M
    TL0 = 0x56;
  #else
    #error "CF_TimeBase is error"
  #endif
    
#elif ( Fmcu_Val == Fmcu_13M)
  #if ( CF_TimeBase == CF_TimeBase_1MS)    
    TH0 = 0xFB;             //定时1ms   13M
    TL0 = 0xBC;
  #elif ( CF_TimeBase == CF_TimeBase_20MS)
    TH0 = 0xAA;             //定时20ms   13M
    TL0 = 0xAC;
  #else
    #error "CF_TimeBase is error"
  #endif
    
#else
  #if ( CF_TimeBase == CF_TimeBase_1MS)
    TH0 = 0xF7;             //定时1ms   26M
    TL0 = 0x77;
  #elif ( CF_TimeBase == CF_TimeBase_20MS)  
    TH0 = 0x55;             //定时20ms   26M
    TL0 = 0x56;
  #else
    #error "CF_TimeBase is error"
  #endif  
#endif    
    TR0 = 0;                // 暂时不开定时器0
    IE |= BIT1;
}
/*=========================================================================================\n
* @function_name: Init_Timer1
* @function_file: main.c
* @描述: 定时器1定时为10ms
* 
* @参数: 
* @返回: 
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-8-9)
* @修改内容:  
===========================================================================================*/
void Init_Timer1(void)
{
    CKCON &= ~BIT4;         // 1/12clk
    TMOD &=0x0f;
    TMOD |= BIT4;           // 定时器为方式1 16位计数器
    TMOD &=~ BIT5;          // 定时器为方式1 16位计数器
#if ( Fmcu_Val == Fmcu_3M2)
  #if ( CF_TimeBase == CF_TimeBase_1MS)
    TH1 = 0xFE;             //定时1ms   3.2M
    TL1 = 0xEF; 
  #elif ( CF_TimeBase == CF_TimeBase_20MS)
    TH1 = 0xEA;             //定时20ms   3.2M
    TL1 = 0xAC;
  #else
    #error "CF_TimeBase is error"
  #endif
    
#elif ( Fmcu_Val == Fmcu_6M5)
  #if ( CF_TimeBase == CF_TimeBase_1MS)
    TH1 = 0xFD;             //定时1ms   6.5M
    TL1 = 0xDE;
  #elif ( CF_TimeBase == CF_TimeBase_20MS)  
    TH1 = 0xD5;             //定时20ms   6.5M
    TL1 = 0x56;
  #else
    #error "CF_TimeBase is error"
  #endif
    
#elif ( Fmcu_Val == Fmcu_13M)
  #if ( CF_TimeBase == CF_TimeBase_1MS)    
    TH1 = 0xFB;             //定时1ms   13M
    TL1 = 0xBC;
  #elif ( CF_TimeBase == CF_TimeBase_20MS)
    TH1 = 0xAA;             //定时20ms   13M
    TL1 = 0xAC;
  #else
    #error "CF_TimeBase is error"
  #endif
    
#else
  #if ( CF_TimeBase == CF_TimeBase_1MS)
    TH1 = 0xF7;             //定时1ms   26M
    TL1 = 0x77;
  #elif ( CF_TimeBase == CF_TimeBase_20MS)  
    TH1 = 0x55;             //定时20ms   26M
    TL1 = 0x56;
  #else
    #error "CF_TimeBase is error"
  #endif  
#endif    
    TR1 = 0;                // 暂时不开定时器1 
    IE |= BIT3;             // TIME1 interrupt 
}
/*=========================================================================================\n
* @function_name: Init_Timer2
* @function_file: main.c
* @描述: 定时器2定时为10ms
* 
* @参数: 
* @返回: 
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  xuqf (2013-1-15)
* @修改内容:  
===========================================================================================*/
void Init_Timer2(void)
{
    CKCON &= ~BIT5;         // 1/12clk
    T2CON &= ~BIT1;         // 做定时器用
#if ( Fmcu_Val == Fmcu_3M2)  
    TH2 = 0xF5;             //定时10ms   3.2M
    TL2 = 0x56; 
#elif ( Fmcu_Val == Fmcu_6M5)    
    TH2 = 0xEA;             //定时10ms   6.5M
    TL2 = 0xAB;
#elif ( Fmcu_Val == Fmcu_13M)    
    TH2 = 0xD5;             //定时10ms   13M
    TL2 = 0x56;
#else    
    TH2 = 0xAA;             //定时10ms   26M
    TL2 = 0xAB;
#endif    
    TR2 = 1;                // 开启定时器2
    IE |= BIT5;             // TIME2 interrupt
}
/*=========================================================================================\n
* @function_name: RAMInit
* @function_file: McuDriver.c
* @描述: 关闭加密功能
* 
* @参数: 
* @返回: 
* @作者:   lim (2011-12-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RAMInit(void)
{
    XRAMPWD=0x33;   //解除保护
}
 
/*=========================================================================================\n
* @function_name: Delay1OSC
* @function_file: McuDrive.c
* @描述: 延时1OSC 在13Mpll 下
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-13)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Delay1OSC(void)
{
    for(uint8 i=0;i<20;i++)
    {
    
    }
} 
/*=========================================================================================\n
* @function_name: DelayOSC
* @function_file: McuDrive.c
* @描述: 在13M主频下延时num个OSC
* 
* 
* @参数: 
* @param:num  个数
* @返回: 
* @作者:   lwb (2012-03-13)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void DelayOSC(uint8 num)
{
    for(uint8 i=0;i<num;i++)
    {
        Delay1OSC();
    }
}
/*=========================================================================================\n
* @function_name: RTCWakeUpTm
* @function_file: McuDrive.c
* @描述: RTC唤醒时间
* 
* 
* @参数: 
* @param:Tm  1s-00，1min-01   1hour-02
* @返回: 
* @作者:   lwb (2012-10-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RTCWakeUpTm(uint8 Tm)
{
    if(Tm>0x02)
    {
        Tm=0x02;
    }
    RTCPEN=0x96;            
    RTCPWD=0x57;
    DelayOSC(5);
    INTRTC=Tm;             
    DelayOSC(5);
    RTCPEN=0x96;
    RTCPWD=0x56;
    DelayOSC(5);
}
/*=========================================================================================\n
* @function_name: EnableRTCInt
* @function_file: McuDrive.c
* @描述: 开启RTC中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-28)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnableRTCInt(void)
{
    EIE|=BIT1;              //秒中断
    ExInt3IE|=BIT6;
}
/*=========================================================================================\n
* @function_name: DisableRTCInt
* @function_file: McuDrive.c
* @描述: 关闭RTC中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-28)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void DisableRTCInt(void)
{
    ExInt3IE&=~BIT6;    //秒中断
}
/*=========================================================================================\n
* @function_name: RTCInitForSleep
* @function_file: Timeproc.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*void RTCInitForSleep(void)
{
    RTCPEN=0x96;        //1S唤醒
    RTCPWD=0x57;
    DelayOSC(5);
    INTRTC=0x00;
    DelayOSC(5);
    RTCPEN=0x96;
    RTCPWD=0x56;
    DelayOSC(5);
}*/
/*=========================================================================================\n
* @function_name: CPUInit
* @function_file: Init.c
* @描述: 初始化MCU的LCD ，TIMER0和 
* 
* @参数: 
* @返回: 
* @作者:   lim (2011-12-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CPUInit(void)
{
    //CLRWDT();               //喂狗
    SLPWDT();
    RAMInit();              //解除RAM保护
    Init_Port();            //初始化IO
    InitLCD();              //LCD初始化
    gs_RacPhy.uc_Uartstat = 0;
    ChangeBodeProc();       //485初始化
    ComBom_InitAll();       //初始化串口
    Init_Timer0();
    Init_Timer2();          //初始化定时器1 
    Init_Timer1();
    Init_Uart4(3); 
    KeyInit();              //按键初始化
    RTCWakeUpTm(RTC_HOUR);  //RTC初始化 1HOUR
//  EnableRTCInt();         //开启RTC秒中断
    
    EIP = 0x03;  //CF1、CF2中断优先级变高,防止其他中断执行时间过长，程序出脉冲时会出现一个大误差
    IP = 0x0A;   //timer1、timer0
}
/*=========================================================================================\n
* @function_name: SetPLL
* @function_file: McuDrive.c
* @描述: Fmcu切换
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-12)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 SetPLL(uint8 fmcu)
{
    uint8 i=0;
    if( fmcu == Fmcu_3M2)
    {
      CtrlCLK=0xEA;               //ADC 800K  dsp 3.2M MCU 3.2M;
      CtrlPLL=0x0;
    }else if( fmcu == Fmcu_6M5)
    {
      CtrlCLK=0xEb;               //ADC 800K  dsp 3.2M MCU 6.5M;
      CtrlPLL=0x0;
    }else if( fmcu == Fmcu_13M)
    {
      CtrlCLK=0xEb;               //ADC 800K  dsp 3.2M MCU 6.5M;
      CtrlPLL=BIT6;               //13M
    }else
    {
      CtrlCLK=0xEb;               //ADC 800K  dsp 3.2M MCU 6.5M;
      CtrlPLL=0xc0;               //26M
    }

    while(!(PLLLCK&BIT0))
    {
        i++;
        if(i>50)
        {
            return false;       //在一定时间内没有锁定
        }
    }
    MCUFRQ=1;
    i=0;

    while(!(MCUFRQ))
    {
        i++;
        if(i>20)
        {
            return false;       //在一定时间内没有锁定
        }
    }

    MEAFRQ=1;
    i=0;
    while(!(MEAFRQ))
    {
        i++;
        if(i>20)
        {
            return false;       //在一定时间内没有锁定
        }
    }

    CtrlCry0=0;                 //调整启振波形
    CtrlCry1=3;
    CtrlCry2|=BIT5;

//  CtrlBGP&=(~(BIT2+BIT1+BIT3));
//  CtrlBGP|=(BIT2);
//  CtrlBGP=InitPara38[0];

    CtrlLDO&=(~BIT5);
    XBYTE[0x2865]=BIT0;

    return true;
} 
/*=========================================================================================\n
* @function_name: SetPLL3DOT2M
* @function_file: McuDrive.c
* @描述: PLL切换到3.2M
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 SetPLL3DOT2M(void)
{
    FSC=0;
    FWC=1;
    MCUFRQ=1;
    MEAFRQ=1;
    CtrlPLL=0;

    CtrlCry0=0;                 //调整启振波形
    CtrlCry1=3;
    CtrlCry2|=BIT5;
    
//  CtrlBGP&=(~(BIT2+BIT1+BIT3));
//  CtrlBGP|=(BIT2);
//  CtrlBGP=InitPara38[0];
    XBYTE[0x2865]=BIT0;
    return true;
}
/*=========================================================================================\n
* @function_name: SetPLL800K
* @function_file: McuDrive.c
* @描述: 800k
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-09-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 SetPLL800K(void)
{
    uint8 i;
    i=0;
    FWC=0;
    FSC=0;
    

    //PMG=1;                      //关闭计量时钟
    CtrlCLK=0xC0;               //MCU 800K
    CtrlPLL=0;                  //
    while(!(PLLLCK&BIT0))
    {
        i++;
        if(i>50)
        {
            return false;       //在一定时间内没有锁定
        }
    }

    MCUFRQ=1;
    i=0;
    while(!(MCUFRQ))
    {
        i++;
        if(i>20)
        {
            return false;       //在一定时间内没有锁定
        }
    }

    MEAFRQ=1;
    i=0;
    while(!(MEAFRQ))
    {
        i++;
        if(i>20)
        {
            return false;       //在一定时间内没有锁定
        }
    }

    CtrlCry0=0;                 //调整启振波形
    CtrlCry1=3;
    CtrlCry2|=BIT5;
//  CtrlBGP&=(~(BIT2+BIT1+BIT3));
//  CtrlBGP|=(BIT2);
//  CtrlBGP=InitPara38[0];
    CtrlLDO|=(BIT5);
    XBYTE[0x2865]=BIT0;
 

    return true;
}
/*=========================================================================================\n
* @function_name: MChannelCal
* @function_file: McuDrive.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void MChannelCal(void)
{
    if((gs_DateTime.ucSecond==0x05))
    {
        GetBat();                                                   //获取电池电压
    }else  if((gs_DateTime.ucSecond==0x07)||(gs_DateTime.ucSecond==0x08))
    {
        if(CtrlADC5!=0x81)
        {
            CtrlADC5=0x81;
        }
    }
//    else if(gs_DateTime.ucSecond==0x10)                            //每分钟第10s
//    {
//        CalRTC();
//    }
}

 
/*=========================================================================================\n
* @function_name: GetBat
* @function_file: McuDrive.c
* @描述: 计算电池电压
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void GetBat(void)
{
    Word32 tempvalue;
    {
        CtrlADC5=0x92;                              //开启电池测量功能

        DelayXms(15);                               //延时30ms
        tempvalue.lword=EnyB_ReadMeterParaACK(DATAOM);

        if(tempvalue.byte[3]>0x80)                  //电池悬空的时候读取可能是负值
        {
            tempvalue.lword=(~tempvalue.lword)+1;
        }

        tempvalue.lword=(tempvalue.lword+BATOFFSET)/BATDIV;     //计算电池电压

        //gul_CellVoltage=tempvalue.lword;

        if(tempvalue.lword<=BATLEVEL)                       //电池电压低于门限制,电池告警
        {
            guc_batstate = BATON;

           // gul_ShowState|=flgShowSt_Bat;
          /*  gut_SysWarn.SysWarn.BatLow=true;
            gs_MeterState.ui_MeterState1.RunCode1Word.MeterCell=true;
            gs_MeterState.ui_MeterState1.RunCode1Word.RTCCell=true;
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt == false      //时钟电池低 状态标志未置
               && gs_ReportWord.ui_ReportWord1.ReportWord1.RtcBatRpt == 1)           //时钟电池低 需要主动上报
            {
                gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = true;         //时钟电池低 状态置位
            }
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt == false      //时钟电池低 状态标志未置
               && gs_ReportWord.ui_ReportWord1.ReportWord1.CutBatRpt == 1)           //时钟电池低 需要主动上报
            {
                gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = true;         //时钟电池低 状态置位
            }*/
        }
        else
        {
            //gul_ShowState&=~flgShowSt_Bat;
            guc_batstate = BATDOWN;
           /* gut_SysWarn.SysWarn.BatLow=false;
            gs_MeterState.ui_MeterState1.RunCode1Word.MeterCell=false;
            gs_MeterState.ui_MeterState1.RunCode1Word.RTCCell=false;
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt == true)     //时钟电池低 判断状态标志
            {
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = false;
            }
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt == true)     //时钟电池低 判断状态标志
            {
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = false;
            }*/
        }

        //guc_CheckBatFlg=false;
        CtrlADC5=0x81;      //切换到温度测量
    }
}
/*=========================================================================================\n
* @function_name: ChangeBodeProc
* @function_file: McuDrive.c
* @描述: 修改波特率处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ChangeBodeProc(void)
{
    uint8 ucMode;

    SysE2ReadData(EEP_UARTBDM,&ucMode,1);

    if(ucMode>=0x40)
    {
        ucMode=0x08;
    }

    if(ucMode==0x02)
    {
        guc_485Bode=0;
    }else if(ucMode==0x04)
    {
        guc_485Bode=1;
    }else if(ucMode==0x08)
    {
        guc_485Bode=2;
    }else if(ucMode==0x10)
    {
        guc_485Bode=3;
    }else if(ucMode==0x20)
    {
        guc_485Bode=4;
    }else
    {
        guc_485Bode=2;
    }

    Init_Uart2(guc_485Bode);
    ComBom_Init(ComIndex_Uart2);    //将uart4 端口对应的 COM初始化
}
/*=========================================================================================\n
* @function_name: IOOFF
* @function_file: McuDrive.c
* @描述: 掉电处理IO口
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void IOOFF(void)
{
    CloseBeiGuang();
 
    //IR TXD
 
    P20FS=0;
    P21FS=0;
    P24FS=0;
    P25FS=0;
    P2OE|=(BIT0+BIT1+BIT4+BIT5);   //禁止输出
    P2IE&=~(BIT0+BIT1+BIT4+BIT5);
    //SP CT485
    P9OE|=(BIT3+BIT5);
    P9IE&=~(BIT3+BIT5);
    P9FC&=~BIT3;    //rtc sp out

    //FRst 端口处理
   // FRRSTNO();
    
    P11FS=0;
    P12FS=0;
    P13FS=0;
//    P1OE&=(~(BIT1+BIT2));
//    P1OD&=~(BIT1+BIT2);
    P1OE|=(BIT1+BIT2+BIT3);
    P1IE&=~(BIT1+BIT2+BIT3);
    
//    EVENTOUTLOW();
#ifdef FRCom
    EVENTOUTHIGH();        //载波
#endif
   
    KeyUpEnable();
   // KeyPrDisable();
    //KeyBgEnable();
}
/*=========================================================================================\n
* @function_name: PowUpLedCtl
* @function_file: McuDrive.c
* @描述: 上电背光，LED控制
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-19)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void PowUpLedCtl(void)
{
    if(gs_DispCont.ucPowUpBgTm!=0)
    {
        //OpenBeiGuang();         //上电开背光，背光时间由显示初始化设置
        //CloseBeiGuang() 
    }
}
/*=========================================================================================\n
* @function_name: IntOFF
* @function_file: McuDrive.c
* @描述: 关闭中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void IntOFF(void)
{
    IE=0;
    EIE=0;
    EXIF=0;
}
/*=========================================================================================\n
* @function_name: UARTOFF
* @function_file: McuDrive.c
* @描述: 关闭UART
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void UARTOFF(void)
{
    TCON2=0;
    TCON3=0;
    TCON4=0;
    CFGA=0;
}
/*=========================================================================================\n
* @function_name: MCUForPowerOff
* @function_file: McuDrive.c
* @描述: 掉电处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void MCUForPowerOff(void)
{
    uint8 i;
    CtrlADC6=0;                         //关AD通道
    CtrlLDO|=BIT5;                      //关LDO
    EnyB_SetMeterCfgACK(0x00,PMCtrl4);   //关数字的AD通道
    EnyB_SetMeterCfgACK(0x00,PMCtrl1);   //关CF脉冲

    for(i=0;i<20;i++);

    PMG=1;                              //关闭计量时钟
}

/*=========================================================================================\n
* @function_name: SwichPluseOutType
* @function_file: McuDrive.c
* @描述: 切换端子功能
* 
* 
* @参数: 
* @param:type  0-RTC  2-时段投切
* @返回: 
* @作者:   lwb (2012-06-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SwichPluseOutType(uint8 type)
{
//    if(type==0)
//    {
//        P9FC|=BIT3;                                                     //输出RTC P9.3改为脉冲输出 zyj 11.2
//        P9OE&=(~BIT3);
//    }
//    if(type==2)
//    {
//        P9FC&=(~BIT3);
//        P9OE&=(~BIT3);
//    }
}


/*=========================================================================================\n
* @function_name: SleepRTC
* @function_file: McuDriver.c
* @描述: 关LDO33;
*        关AD;
*        禁止计量电路访问双口ram;
*        关闭所有计量通道;
*        关PM时钟;
*        将系统切到低频工作;
*        关PLL;
*        让系统慢速进入浅睡眠。 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lim (2011-12-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 SleepRTC(void)
{
    uint8 i;

    gs_KeyCtr[UpKey].Status=KeyRls;             //按键设置为释放
    BE_I2C_SDA_1();
    BE_I2C_CLK_1();                     //I2C输出高
    FWC=0;
    FSC=0;
    if((Systate&BIT0)==0x01)
    {   
        return false;
    }
    CtrlADC6=0;                 //关闭所有的ADC
    CtrlLDO|=BIT5;

    for(i=0;i<20;i++);          //等待

    PMG=1; //关闭计量时钟
    if((Systate&BIT0)==0x01)
    {   
        return false;
    }

    MCUFRQ=0;
    while(MCUFRQ);
    
    MEAFRQ=0; 
    while(MEAFRQ);

    if((Systate&BIT0)==BIT0)
    {   
        return false; 
    }

    for(i=0;i<3;i++);           //等待

    CtrlCLK=0x00;
    while(CtrlCLK); 
     
    SLEEP0=1;

    return false;               //睡眠不成功返回错误
}

/*=========================================================================================\n
* @function_name: Sleep
* @function_file: McuDrive.c
* @描述: 关LDO33;
*        关AD;
*       禁止计量电路访问双口ram;
*       关闭所有计量通道;
*       关PM时钟;
*       将系统切到低频工作;
*       关PLL;
*       让系统慢速进入浅睡眠;* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 Sleep(void)
{
    uint8 i;
    FWC=0;
    FSC=0;

    // RTCWakeUpTm(RTC_SEC);
    gui_SystemState|=flgStSys_PowOff;    //置掉电状态

    if((Systate&BIT0)==0x01)
    {   
        return false;
    }

    CtrlADC6=0;             //关闭所有的ADC
    //LCDCtrl=0;
    //LCDG=1;
    CtrlLDO|=BIT5;
    CtrlLCDV=0x00;

    for(i=0;i<20;i++);      //等待

    PMG=1;                  //关闭计量时钟

    if((Systate&BIT0)==0x01)
    {   
        return false;
    }

    MCUFRQ=0;
    while(MCUFRQ);


    MEAFRQ=0;
    while(MEAFRQ);

    if((Systate&BIT0)==0x01)
    {   
        return false; 
    }

    for(i=0;i<3;i++);       //等待

    CtrlCLK=0x00;
    while(CtrlCLK);

    SLEEP0=1;
    DelayOSC(5);
    return false;           //睡眠不成功，返回FLASE
}
/*=========================================================================================\n
* @function_name: BgCtrFun
* @function_file: McuDrive.c
* @描述: 背光控制
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void BgCtrFun(void)
{

//    if(gut_SysWarn.byte>0)              //有错误字，开启背光和告警等
//    {
//        //OpenBeiGuang();
//    }
//
//    if(guc_DyBgLed>0)
//    {
//        guc_DyBgLed--;
//        if(guc_DyBgLed==0)
//        {
//            if(gut_SysWarn.byte==0)                     //没有告警字
//            {
//                CloseBeiGuang();//关闭背光
//            }
//        }else
//        {
//            OpenBeiGuang();
//        }
//    }else
//    {
//        if(gut_SysWarn.byte==0 )                     //没有告警字
//        {
//            CloseBeiGuang();//关闭背光
//        }
//        else
//        {
//            OpenBeiGuang();
//        }
//    }
}
#ifdef _SW_EXT_RTC
/*=========================================================================================\n
* @function_name: GetExtRTC
* @function_file: McuDrive.c
* @描述: 读取硬件时钟
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-02-29)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void GetExtRTC(void)
{
    volatile uint8 RTCFLAG ;
    RTCFLAG=RTCLATCH;
    DelayOSC(5);
    gs_ClkTmp.ucSecond  = RTCSC;
    gs_ClkTmp.ucMinute  = RTCMiC;
    gs_ClkTmp.ucHour    = RTCHC;
    gs_ClkTmp.ucWeek    = RTCWC;
    gs_ClkTmp.ucDay     = RTCDC;
    gs_ClkTmp.ucMonth   = RTCMoC;
    gs_ClkTmp.ucYear    = RTCYC;
}
/*=========================================================================================\n
* @function_name: SetExtRTC
* @function_file: McuDrive.c
* @描述: 设置RTC时钟
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-02-29)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SetExtRTC(void)
{
    //RTC允许写 
    RTCPEN = 0x96;      //password 1
    RTCPWD = 0x57;      //password 2
    DelayOSC(5);
    //写RTC时间
    RTCYC   = gs_DateTime.ucYear;
    RTCMoC  = gs_DateTime.ucMonth;
    RTCDC   = gs_DateTime.ucDay;
    RTCHC   = gs_DateTime.ucHour;
    RTCMiC  = gs_DateTime.ucMinute;
    RTCSC   = gs_DateTime.ucSecond;
    RTCWC   = gs_DateTime.ucWeek;
    DelayOSC(5);
    //RTC禁止写  
    RTCPEN = 0x96;
    RTCPWD = 0x56;
    DelayOSC(5);
}
#endif

/*=========================================================================================\n
* @function_name: Check_Sfr
* @function_file: McuDrive.c
* @描述: 保护特殊功能寄存器
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-07-19)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Check_Sfr(void)
{
    if(EIE!=0xE3)
    {
        EIE=0xE3;
    }
    if(IE!=0xF2)
    {
        IE=0xF2;
    }
#ifdef FRCom
    if((TCON&0x50)!=0x50)
    {
        TCON|=0x50;
    }
#else
   if((TCON&0x10)!=0x10)
    {
        TCON|=0x10;
    }
#endif

    if(guc_CfOpenFlg==false)
    {
       if((ExInt2IE&0x8E)!=0x8E)
       {
           ExInt2IE|=0x8E;
       }
    }else
    {
        if((ExInt2IE&0x0E)!=0x0E)
        {
           ExInt2IE|=0x0E;
        }
    }

//  if(ExInt3IE!=0x40)
//  {
//      ExInt3IE=0x40;
//  }
#ifdef FRCom
    if(  P11FS!=0x02
       ||P12FS!=0x02
       ||P20FS!=0x02
       ||P21FS!=0x02
       ||P24FS!=0x02
       ||P25FS!=0x02)
    {
        P11FS=0x02;
        P12FS=0x02;
        P20FS=0x02;
        P21FS=0x02;
        P24FS=0x02;
        P25FS=0x02;
    }
#else
    if( P20FS!=0x02
        ||P21FS!=0x02
        ||P24FS!=0x02
        ||P25FS!=0x02)
    {
        P20FS=0x02;
        P21FS=0x02;
        P24FS=0x02;
        P25FS=0x02;
    }
#endif
    
}

#ifdef FRCom
/*=========================================================================================\n
* @function_name: EventOutProc
* @function_file: McuDrive.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-03-19)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EventOutProc(void)
{
    if(((gs_ReportState.ui_ReportState1.Word.word&0x0528) != 0) 
       || (gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt != 0)
       || (gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt != 0))  //静止式多费率判断
    {        
        EVENTOUTHIGH();
    }
    else
    {
        EVENTOUTLOW();
    }
}
#endif
