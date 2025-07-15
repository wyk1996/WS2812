#define MCUDRIVEEXT
#include "Include.h"
#define V9801A1
extern __IO uint8_t adc_done;
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
    //P0OE&=(~(BIT0+BIT1+BIT2+BIT3)); //JTAG 输出 0
    //P0OD&=(~(BIT0+BIT1+BIT2+BIT3));	//zzp 181119
	
                PMU->IOADAT &= (~(BIT0+BIT1)); 	//输出0
                PMU->IOADAT |= BIT7;
                //PMU->IOAIE &=~ (BIT0+BIT1+BIT7);
		PMU->IOAOEN &= (~(BIT0+BIT1+BIT7));  //0 输出 默认1
		
                
                GPIOB->DAT |= ((BIT0+BIT6));    //低电平点亮
                GPIOB->OEN &= (~(BIT0+BIT6));   //有功iob0 无功iob6 输出
		
		//浮空脚禁止输入输出  18个 输出0  zzp181120
		PMU->IOAOEN	&= (~(BIT2+BIT5+BIT6+BIT13));
		PMU->IOADAT &= (~(BIT2+BIT5+BIT6+BIT13)); 	//输出0
		GPIOB->OEN &= (~(BIT3+BIT8+BIT15));
		GPIOB->DAT &= (~(BIT3+BIT8+BIT15));
		GPIOE->OEN	&= (~(BIT0+BIT1+BIT4+BIT8+BIT9+BIT12+BIT13+BIT14+BIT15));
		GPIOE->DAT	&= (~(BIT0+BIT1+BIT4+BIT8+BIT9+BIT12+BIT13+BIT14+BIT15));
		GPIOF->OEN	&= (~(BIT0+BIT1));
		GPIOF->OEN	&= (~(BIT0+BIT1));
				
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
  /*
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
   P10OD&=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);*/ //zzp181120
//#ifdef E2_WP
//   P9OE&=~(BIT4);
//   P9IE&=~BIT4;
//   P9DO|=((BIT4));          //禁止E2_wp
//#else
//   P9OE|=(BIT4);
//   P9IE|=(~(BIT4));     //高阻
//#endif

//485  ZZP181120
	GPIOB->OEN &= (~BIT2); //TXD 允许输出
	GPIOB->IE &= (~BIT2);  //TXD禁止输入
	PMU->IOAOEN |= (BIT12);  //rxd 禁止输出
	PMU->IOAIE |= (BIT12); 	//RXD 允许输入

/*
   //RS485
   P2OE&=(~BIT5);       //TXD 允许输出
   P2IE&=(~BIT5);       //TXD禁止输入
   P2OE|=(BIT4);        //rxd 禁止输出
   P2IE|=(BIT4);        //RXD 允许输入
   P24FS=2;             //RXD
   P25FS=2;  *///ZZP181120
 
		//9240 TXRX zzp181120
	GPIOB->OEN &= (~BIT7);  //tx输出
	GPIOB->IE &= (~BIT7);
	GPIOB->OEN |= (BIT1);		//rx输入
	GPIOB->IE |= (BIT1);
 
 /*
   //9260_serial
   P2OE|=(BIT0);        //rxd 禁止输出
   P2IE|=(BIT0);        //RXD 允许输入
   P2OE&=(~BIT1);       //TXD 允许输出
   P2IE&=(~BIT1);       //TXD禁止输出
   P20FS=2;             //RXD
   P21FS=2;             //TXD*///zzp181120
//   Uart4_RevDisable(); //关闭接收中断

//   P1OE&=~(BIT3+BIT1+BIT2);  //9240 通信控制脚 zyj 11.4
//   P1IE&=~(BIT3+BIT1+BIT2);
//   P1OD|=(BIT3+BIT1+BIT2);
   /*
   P7OE&=~(BIT6+BIT7);  //9260S 通信控制脚 zyj 11.10 直接输出低，不做控制
   P7IE&=~(BIT6+BIT7);
   P7OD&=~(BIT6+BIT7);
   P8OE&=~BIT0;
   P8IE|=BIT0;
   P8OD&=~BIT0;*///zzp181120
   
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
 /*   CKCON &= ~BIT3;         //1/12clk
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
*/
  //**********************************************************************
							//定时器设置       1ms或20ms  由CF_TimeBase决定
	TMR0->CTRL |= 0X08;		//定时器中断使能//13107200  0.0762939453125
        TMR0->VALUE = 655359;
	if(Fmcu_Val == Fmcu_26M)
	{	
                  TMR0->RELOAD |= 655359;//131071;
		//TMR0->RELOAD |= 13107200*CF_TimeBase/1000-1;		
		//RELOAD = Period * APBCLK-1   (13107200Hz)*(0.01s)-1=131071
	}
	else if(Fmcu_Val == Fmcu_13M)
	{	
		TMR0->RELOAD |= 6553600*CF_TimeBase/1000-1;		
	}
	else if(Fmcu_Val == Fmcu_6M5)
	{	
		TMR0->RELOAD |= 3276800*CF_TimeBase/1000-1;		
	}
	else
	{	
		TMR0->RELOAD |= 1638400*CF_TimeBase/1000-1;		
	}
	NVIC_EnableIRQ(TMR0_IRQn);			//使能定时器0中断
	NVIC_ClearPendingIRQ(TMR0_IRQn);	//清除中断标志
	NVIC_SetPriority(TMR0_IRQn,1);		//设置中断优先级
	TMR0->CTRL &=~ 0X01;			//tmr0 bit0 定时器使能位 暂不开启定时器0
	
}
/*=========================================================================================\n
* @function_name: Init_Timer1
* @function_file: main.c
* @描述: 定时器1定时为ms
* 
* @参数: 
* @返回: 
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-8-9)
* @修改内容:  
===========================================================================================*/
void Init_Timer1(void)//电量累加用
{
	//**********************************************************************
	//定时器1设置       1ms或20ms  由CF_TimeBase决定
	TMR1->CTRL |= 0X08;		//定时器中断使能//13107200  0.0762939453125
	TMR1->VALUE = 2620;
        if(Fmcu_Val == Fmcu_26M)
	{	
		//TMR1->RELOAD |= 131072*79/100000-1;//8us	用于三相能量累加，1s5000次
                TMR1->RELOAD |= 2620;//1309  2635;//3931;//1034;//131072*2/100-1;//200us 2620
		//RELOAD = Period * APBCLK-1   (13107200Hz)*(0.01s)-1=131071
	}
	else if(Fmcu_Val == Fmcu_13M)
	{	
		//TMR1->RELOAD |= 65536*79/100000-1;//8us
                TMR1->RELOAD |= 65536*2/100-1;//200us
	}
	else if(Fmcu_Val == Fmcu_6M5)
	{	
//		TMR1->RELOAD |= 32768*79/100000-1;//8us
                TMR1->RELOAD |= 32768*2/100-1;//200us
	}
	else
	{	
//		TMR1->RELOAD |= 16384*79/100000-1;//8us
		TMR1->RELOAD |= 16384*2/100-1;//200us
	}
	
        MISC->IRQLAT |= 0Xf0;
	NVIC_ClearPendingIRQ(TMR1_IRQn);	//清除中断标志
	NVIC_SetPriority(TMR1_IRQn,0);		//设置中断优先级
        NVIC_EnableIRQ(TMR1_IRQn);			//使能定时器1中断
	TMR1->CTRL |= 0X01;			//tmr1 bit0 定时器使能位 开启定时器1
	

/*
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
    IE |= BIT3;             // TIME1 interrupt */
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
	//**********************************************************************
	//定时器设置       10ms
	TMR2->CTRL |= 0X08;		//定时器中断使能//13107200  0.0762939453125
	TMR2->VALUE = 131071;
        if(Fmcu_Val == Fmcu_26M)
	{	
		TMR2->RELOAD |= 131071;//13107200*10/1000-1;		
		//RELOAD = Period * APBCLK-1   (13107200Hz)*(0.01s)-1=131071
	}
	else if(Fmcu_Val == Fmcu_13M)
	{	
		TMR2->RELOAD |= 65535;//6553600*10/1000-1;		
	}
	else if(Fmcu_Val == Fmcu_6M5)
	{	
		TMR2->RELOAD |= 32767;//3276800*10/1000-1;		
	}
	else
	{	
		TMR2->RELOAD |= 166383;//1638400*10/1000-1;		
	}
	NVIC_EnableIRQ(TMR2_IRQn);			//使能定时器2中断
	NVIC_ClearPendingIRQ(TMR2_IRQn);	//清除中断标志
	NVIC_SetPriority(TMR2_IRQn,2);		//设置中断优先级
	TMR2->CTRL |= 0X01;			//tmr2 bit0 定时器使能位 开启定时器2
	

/*
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
    */
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
/*void RAMInit(void)
{
    XRAMPWD=0x33;   //解除保护
}
 *///zzp181120
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
void Delay1OSC(void)		//1/32768hz = 30.5us 
{
	uint8 i;
    for(i=0;i<20;i++)
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
	uint8 i;
    for( i=0;i<num;i++)
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
	RTC->PWD = 0x5AA55AA5;	//解除RTC写保护
    RTC->CE = 0xA55AA55B;  

	switch (Tm)
		{
			case 0:
				RTC->WKUSEC |= 0;		//唤醒时间(WKUSEC+1)*1 S
				RTC->INTSTS |= 0X04;	//清中断标志
				RTC->INTEN = BIT2;		//使能唤醒秒中断
				break;
			case 1:
				RTC->WKUMIN |= 0;		//唤醒时间(WKUSEC+1)*1 min
				RTC->INTSTS |= 0X08;	//清中断标志
				RTC->INTEN = BIT3;		//使能唤醒秒中断
				break;
			case 2:
				RTC->WKUHOUR |= 0;		//唤醒时间(WKUSEC+1)*1 HOUR
				RTC->INTSTS |= 0X10;	//清中断标志
				RTC->INTEN = BIT4;		//使能唤醒秒中断
				break;
			default:
				break;	
		}
	
	RTC->PWD = 0x5AA55AA5;	//使能RTC写保护
    RTC->CE = 0xA55AA55A;  
	while (RTC->CE & RTC_CE_BSY);	//等待rtc相关寄存器更新值 约100us
	/* Clear Pending Interrupt */
  	NVIC_ClearPendingIRQ(RTC_IRQn);
 	 /* Set Interrupt Priority */
  	NVIC_SetPriority(RTC_IRQn, 0);
  	/* Enable Interrupt in NVIC */
  	NVIC_EnableIRQ(RTC_IRQn);

//***********************************************
/*
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
    */
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
/*void EnableRTCInt(void)	//没有调用
{
    EIE|=BIT1;              //秒中断
    ExInt3IE|=BIT6;
}*///zzp181120
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
/*void DisableRTCInt(void)		//没有调用
{
    ExInt3IE&=~BIT6;    //秒中断
}*///zzp181120
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
{/*
    SLPWDT();
    Init_Port();            //初始化IO
    InitLCD();              //LCD初始化
    KeyInit();              //按键初始化*/
    
     SLPWDT();
    InitLCD();              //LCD初始化
    Init_Port();            //初始化IO
    //gs_RacPhy.uc_Uartstat = 0;
    ChangeBodeProc();       //485初始化
    ComBom_InitAll();       //初始化串口
    Init_Timer0();
    Init_Timer2();          //初始化定时器
    //Init_Timer1();
    Init_Uart4(3); //ZZP
    KeyInit();              //按键初始化
    //RTCWakeUpTm(Init_RTC_HOUR);  //RTC初始化 1HOUR  ZZP1203
    
    
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

	//ANA->REG3 |= 0x18;	
        ANA->REG3  &=~ BIT3;    //开启内部rch 开启BGP
        ANA->REG3  &=~ BIT4;
	MISC2->CLKSEL = 0;	//AHB时钟源选为RCH
	ANA->REGA &= ~BIT6;
  	ANA->REG2 &= ~BIT7;

	//PLLL configuration
	
	ANA->REG9 &= 0XFFFFFFF8;//plll频率设置为26.2146M bit0-2			000对应26M   
	PMU->CONTROL = 0;	//plll时钟源选为外部32K bit0 置0 
	ANA->REG3 |= BIT5;	//开启PLLL
        
	while (!(ANA->COMPOUT & 0x02));
	MISC2->CLKSEL |= 0X04; 		//选择AHB时钟为plll 26.2146M
	MISC2->CLKDIVH =0;			//AHB与HCLK时钟分频比为1:1
	MISC2->CLKDIVP =1;			//AHB与APB时钟分频比为2:1
	
	//RTCCLK configuration		RTC时钟上电默认外部32k，1分频
	
	
	/*
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
    XBYTE[0x2865]=BIT0;			//M通道控制寄存器  	  改良测温性能
	*/
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
/*uint8 SetPLL3DOT2M(void)		//没有调用
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
}*///zzp181120
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
	ANA->REG3 |= 0x18;	//开启内部rch 开启BGP
	MISC2->CLKSEL = 0;	//AHB时钟源选为RCH
	ANA->REGA &= ~BIT6;
  	ANA->REG2 &= ~BIT7;

	//PLLL configuration
	
	ANA->REG9 |= 0X05;		//plll频率设置为26.2146M bit0-2			101对应0.8M   
	PMU->CONTROL = 0;	//plll时钟源选为外部32K bit0 置0 
	ANA->REG3 |= BIT5;	//开启PLLL
	while (!(ANA->COMPOUT & 0x02));
	MISC2->CLKSEL |= 0X04; 		//选择AHB时钟为plll 0.8M
	MISC2->CLKDIVH =0;			//AHB与HCLK时钟分频比为1:1
	MISC2->CLKDIVP =0;			//AHB与APB时钟分频比为1:1
	
	/*
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
 */

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
/*void MChannelCal(void)
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
*///zzp181120
 
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
    uint32_t adc_data[5],i;
	float Vbat,Vbatcap;
	NVR_BATMEARES NVR_BATOffsetStruct;

	if (NVR_GetBATOffset(&NVR_BATOffsetStruct))		//从flash的INFO区获得电压补偿值
	{
		NVR_GetBATOffset(&NVR_BATOffsetStruct);		//读取失败重试
		Vbatcap = NVR_BATOffsetStruct.BATCAPResult;
	}
  	else
    	Vbatcap = NVR_BATOffsetStruct.BATCAPResult;//获取电容补偿值

	ANA->ADCCTRL |= 0x300; 			//3左移8位 adc时钟4分频
	//ANA->ADCCTRL |= 0x300;		bit12 时钟源选择 0 位内部RCH 1为plll 默认为0
	//bit21 自动模式选择 0 自动单通道  1 自动12通道  默认为0
	//ANA->ADCCTRL |= 0x300;			//bit20 手动转换选择 0 单通道 1 12通道 默认0
	ANA->ADCCTRL |= 0x02;			//选择手动通道为 BATRTC 

			//使能中断
	adc_done = 0;
	ANA->REG1 |= ANA_REG1_GDE4;		//电容分压1/4
	*ANA_REG1 |= BIT6; 				//选择1M下拉电
  
  	ANA->REG3 |= ANA_REG3_ADCPDN;	//bit0  adc开关 1开启
        ANA->INTEN |= BIT0;		//BIT0 adc手动转换完成中断使能
	CORTEX_SetPriority_ClearPending_EnableIRQ(ANA_IRQn, 0);
	
	for (i=0; i<5; i++)		//手动开启adc转换并获取adc的值
  	{
          ANA->REG7 &= ~BIT3;
          ANA->ADCCTRL |= BIT31;	//BIT31写1手动触发	//ANA_INTSTS的BIT1手动触发完成中断标志
          while (adc_done == 0);
          adc_data[i] = *ANA_ADCDATA2;
          adc_done = 0;
  	}

	for (i=0; i<5; i++)
  	{
    	/* Calculate the voltage of  BAT1 */
    	Vbat = (((int16_t)adc_data[i] - 49.588) / 7088.91575) + Vbatcap; 
  	}

		tempvalue.lword= (uint32) (1000*Vbat);				//计算电池电压 单位mv
	
    /*//
        CtrlADC5=0x92;                              //开启电池测量功能

        DelayXms(15);                               //延时30ms
        tempvalue.lword=EnyB_ReadMeterParaACK(DATAOM);

        if(tempvalue.byte[3]>0x80)                  //电池悬空的时候读取可能是负值
        {
            tempvalue.lword=(~tempvalue.lword)+1;
        }

        tempvalue.lword=(tempvalue.lword+BATOFFSET)/BATDIV;     //计算电池电压

		*/

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
        /*CtrlADC5=0x81;      //切换到温度测量*/
    
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

	PMU->IOAOEN |=BIT12;	//oen bit12置1   禁止ioA12输出功能 rx
	PMU->IOAIE &=~ BIT12;	//ie  bit12置1   禁止ioA12输入功能 rx
	
	GPIOB->OEN |= BIT2; //禁止tx脚输出功能 IOB2
	GPIOB->IE &=~ BIT2; //禁止tx脚输入功能 IOB2

	GPIOB->OEN |=BIT1;	//oen bit1置1   禁止iob1输出功能 rx
	GPIOB->IE &=~ BIT1;	//ie  bit1置1   禁止iob1输入功能 rx
	GPIOB->OEN |=BIT7;  //禁止tx脚输出功能
	GPIOB->IE &=~ BIT7; //禁止tx脚输入功能

	PMU->IOAOEN |=BIT8;		//oen bit12置1   禁止ioA12输出功能 rx
	PMU->IOAIE &=~ BIT8;	//ie  bit12置1   禁止ioA12输入功能 rx
	PMU->IOAOEN |=BIT9;		//oen bit12置1   禁止ioA12输出功能 rx
	PMU->IOAIE &=~ BIT9;	//ie  bit12置1   禁止ioA12输入功能 rx
	PMU->IOAOEN |=BIT11;	//oen bit12置1   禁止ioA12输出功能 rx
	PMU->IOAIE &=~ BIT11;	//ie  bit12置1   禁止ioA12输入功能 rx
	
 
    //IR TXD
 	/*
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
   */
        
    KeyUpEnable();
    PMU->IOAWKUEN = (1<<8);     //下降沿触发
    GPIOA->DAT |= BIT4;
    PMU->IOAINTSTS = BIT4;     //写1清零
    PMU->CONTROL |= 1;          //使能外部中断唤醒
    CORTEX_SetPriority_ClearPending_EnableIRQ(PMU_IRQn, 1);
    
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
void PowUpLedCtl(void)	//没有调用
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
	__disable_irq();		//Disable IRQ Interrupts
    //IE=0;
    //EIE=0;
    //EXIF=0;
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

	UART0->CTRL &=~ 0xfc;		//关闭txrx
	UART5->CTRL &=~ 0xfc;
/*
    TCON2=0;
    TCON3=0;
    TCON4=0;
    CFGA=0;*/
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
	*ANA_REG3 &=~ 0xf6;		//关闭adc和BGP
	

/*
    uint8 i;
    CtrlADC6=0;                         //关AD通道
    CtrlLDO|=BIT5;                      //关LDO
    EnyB_SetMeterCfgACK(0x00,PMCtrl4);   //关数字的AD通道
    EnyB_SetMeterCfgACK(0x00,PMCtrl1);   //关CF脉冲

    for(i=0;i<20;i++);

    PMG=1;                              //关闭计量时钟*/
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
    if(type==0)	//ioa3
    {
			PMU->IOASEL = BIT7;//RTC_PLLDIV
			PMU->IOAOEN &= (~BIT7);
			
//        P9FC|=BIT3;                                                     //输出RTC P9.3改为脉冲输出 zyj 11.2
//        P9OE&=(~BIT3);
    }
    if(type==2)
    {				//IOB0
			GPIOB->OEN &= (~BIT0);
			
//        P9FC&=(~BIT3);
//        P9OE&=(~BIT3);
    }
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
uint8 SleepRTC(void)	//进入浅睡眠之前的设置
{
    uint8 i;

    gs_KeyCtr[UpKey].Status=KeyRls;             //按键设置为释放
    BE_I2C_SDA_1();
    BE_I2C_CLK_1();                     //I2C输出高

	*ANA_REG3 &=~ 0xD6;		//关闭adc,BGP和PLLL时钟
	ANA->REGF &= ~BIT2;		//禁止avccldo输出
	ANA->REGF &= ~ ANA_REGF_PDNADT;			//关闭Tiny adc
	ANA->ADCCTRL &= ~(ANA_ADCCTRL_MCH | ANA_ADCCTRL_ACH);	//关闭adc通道
	ANA->REG1 &= ~(ANA_REG1_RESDIV | ANA_REG1_GDE4);		//关闭电阻电容分压
	ANA->REG3 &= ~ANA_REG3_CMP1PDN;			//关闭比较器1
	ANA->REG3 &= ~ANA_REG3_CMP2PDN;			//关闭比较器2
	//ANA->REGA &= ~ANA_REGA_PD_VDCINDET;		//关闭VDCin电压检测
	//ANA->REG9 &= ~ANA_REG9_PDDET;			//关闭VDD电压检测
	MISC2->HCLKEN &=~ 0X0E0;
	MISC2->PCLKEN &=~ 0XFFF3F210;
	//flash1us 没有动？

	ANA->REG7 &= ~ANA_REG7_SWT2VDD;
  	ANA->REGA |= ANA_REGA_SWT2BAT1 | ANA_REGA_VDD3_OFF;		//控制供电状态

	//进入浅睡眠前还要设置唤醒的中断源 按键 VDCin上电等
	PMU->IOAOEN |= BIT4;
	PMU->IOAIE |= BIT4;		//ioa4设置为输入
	PMU->IOAWKUEN |= 0xc0;					//使能ioa4 外部中断和唤醒
	NVIC_EnableIRQ(PMU_IRQn);

	*ANA_INTEN |= 0X100;				//使能VDCin上电掉电中断
	NVIC_EnableIRQ(ANA_IRQn);

	if(!(POWERSTATE()))
    {   
        return false;
    }
  	
  	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; 		//进入浅睡眠
  	__WFI();
	
	
	/*
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
     
    SLEEP0=1;*/
	
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
uint8 Sleep(void)	//睡眠浅的设置 sleeprtc区别？
{
    uint8 i;
    //FWC=0;	//zzp
    //FSC=0;

    // RTCWakeUpTm(RTC_SEC);
    gui_SystemState|=flgStSys_PowOff;    //置掉电状态


	*ANA_REG3 &=~ 0xD6;		//关闭adc,BGP和PLLL时钟
	ANA->REGF &= ~BIT2;		//禁止avccldo输出
	ANA->REGF &= ~ ANA_REGF_PDNADT;			//关闭Tiny adc
	ANA->ADCCTRL &= ~(ANA_ADCCTRL_MCH | ANA_ADCCTRL_ACH);	//关闭adc通道
	ANA->REG1 &= ~(ANA_REG1_RESDIV | ANA_REG1_GDE4);		//关闭电阻电容分压
	ANA->REG3 &= ~ANA_REG3_CMP1PDN;			//关闭比较器1
	ANA->REG3 &= ~ANA_REG3_CMP2PDN;			//关闭比较器2
	//ANA->REGA &= ~ANA_REGA_PD_VDCINDET;		//关闭VDCin电压检测
	//ANA->REG9 &= ~ANA_REG9_PDDET;			//关闭VDD电压检测
	MISC2->HCLKEN &=~ 0X0EF;
	MISC2->PCLKEN &=~ 0XFFF3F211;
	//flash1us 没有动？

	ANA->REG7 &= ~ANA_REG7_SWT2VDD;
  	ANA->REGA |= ANA_REGA_SWT2BAT1 | ANA_REGA_VDD3_OFF;		//控制供电状态

	//进入浅睡眠前还要设置唤醒的中断源 按键 VDCin上电等
	PMU->IOAOEN |= BIT4;
	PMU->IOAIE |= BIT4;		//ioa4设置为输入
	PMU->IOAWKUEN |= 0xc0;					//使能ioa4 外部中断和唤醒
	NVIC_EnableIRQ(PMU_IRQn);

	*ANA_INTEN |= 0X100;				//使能VDCin上电掉电中断
	NVIC_EnableIRQ(ANA_IRQn);

	if(!(POWERSTATE()))
    {   
        return false;
    }
  	
  	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; 		//进入浅睡眠
  	__WFI();


	

    //if((Systate&BIT0)==0x01)  // zzp
    //{   
      //  return false;
    //}
/*
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
    */				//zzp
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
void BgCtrFun(void)		//不用
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
void GetExtRTC(void)		//读取RTC时间
{
    volatile uint8 RTCFLAG ;
    RTCFLAG=RTC->LOAD;
    while (RTC->CE & RTC_CE_BSY);			//等待BSY清零 约100us
    gs_ClkTmp.ucSecond  = RTC->SEC;			
    gs_ClkTmp.ucMinute  = RTC->MIN;
    gs_ClkTmp.ucHour    = RTC->HOUR;
	gs_ClkTmp.ucDay     = RTC->DAY;	
    gs_ClkTmp.ucWeek    = RTC->WEEK;
    gs_ClkTmp.ucMonth   = RTC->MON;
    gs_ClkTmp.ucYear    = RTC->YEAR;
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
void SetExtRTC(void)	//设置RTC时间
{
    //RTC允许写 
   	RTC->PWD = 0X5AA55AA5;
   	RTC->CE = 0XA55AA55B;
    //写RTC时间
    RTC->YEAR   = gs_DateTime.ucYear;
    RTC->MON  = gs_DateTime.ucMonth;
	RTC->WEEK   = gs_DateTime.ucWeek;
    RTC->DAY   = gs_DateTime.ucDay;
    RTC->HOUR   = gs_DateTime.ucHour;
    RTC->MIN  = gs_DateTime.ucMinute;
    RTC->SEC   = gs_DateTime.ucSecond;

	RTC->PWD = 0X5AA55AA5;
	RTC->CE = 0XA55AA55A;
    while (RTC->CE & RTC_CE_BSY);			//等待BSY清零 约100us
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
/*void Check_Sfr(void)		//不用
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
/*void EventOutProc(void)			//不用
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
*///zzp181120