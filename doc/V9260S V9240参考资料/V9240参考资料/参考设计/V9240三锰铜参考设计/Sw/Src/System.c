#define SYSTEMEXT

#include "Include.h"
/*=========================================================================================\n
* @function_name: EXINT0
* @function_file: System.c
* @描述: 外部中断0
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=2
#pragma vector=0x03               //中断号0,外部中断0
__interrupt void EXINT0(void)
{
//   uint16 tmp;
//  if(!gs_RacPhy.uc_Powstat&power_c &&!gs_RacPhy.uc_Powstat&power_a)
//    {
//        TR0 = 0;
//        tmp = TH0 <<8;
//        gs_phytim.CPhytim = tmp + TL0;
//        gs_phyint.Cint = 10; 
//    }
}

/*=========================================================================================\n
* @function_name: Timer0Interrupt
* @function_file: System.c
* @描述: 定时器0中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=3
#pragma vector=0x0b               //中断号1,  Timer0
__interrupt void Timer0Interrupt(void)
{
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
    if(guc_80msCntCF2>0)
    {
      guc_80msCntCF2--;
      if(guc_80msCntCF2 == 0)
      {
         P9OD &=(~BIT7);    //P9.7 无功CF关闭
         TR0 = 0;           //关定时器0
      }
    }
}
/*=========================================================================================\n
* @function_name: EXINT1
* @function_file: System.c
* @描述: 外部中断1
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=2
#pragma vector=0x13               //中断号2,  外部中断1
__interrupt void EXINT1(void)
{
  
}

/*=========================================================================================\n
* @function_name: Timer1Interrupt
* @function_file: System.c
* @描述: 定时器1中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=3
#pragma vector=0x1b               //中断号3,  Timer1
__interrupt void Timer1Interrupt(void)
{
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
    if(guc_80msCntCF1>0)
    {
      guc_80msCntCF1--;
      if(guc_80msCntCF1 == 0)
      {
         P9OD &=(~BIT3);    //P9.3 有功CF关闭
         TR1 = 0;  //关定时器1
      }
    }
}

/*=========================================================================================\n
* @function_name: Timer2Interrupt
* @function_file: System.c
* @描述: 定时器2中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=2
#pragma vector=0x2b               //中断号5,  Timer2
__interrupt void Timer2Interrupt(void)
{
    TF2=0;
    //TH2 = 0xD5;             //定时10ms
    //TL2 = 0x56;             //启动第一次进中断的时间设定
     
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
    if(guc_ABCreadbegian ==1)
    {
        guc_1280msCnt++;
    }
#if (CHECK_RevPhase == 1)    
    if(guc_1280msCnt == 45)
    {
      guc_CheckABCPhase_Cnt++;
      if(( guc_CheckABCPhase_Cnt > 10) && ( guc_CheckABCPhase_Dly == 0)) //每11次检测一下,写带通滤波器之后延时时间到
      {
        guc_CheckABCPhase_Cnt = 0;
        gui_SystemEvent |= flgEtGlb_ABC_Phase;
      }
    }else if( guc_1280msCnt > 50) //时间太长没响应关闭相位测量
    {
      gui_SystemEvent&=~flgEtGlb_ABC_Phase;
    }
#endif    
    if(guc_1280msCnt >= 64)
    {
      guc_1280msCnt = 0;
      if(gui_ReadABCPhy == 0x00 && gs_RacPhy.uc_Physendtim == 0)
//      if(guc_StartReadABC_Flg == 0x00)
      {
        gui_ReadABCPhy |= flgEtPhy_A;  
        gui_SystemEvent |= gui_ReadABCPhy<<8;
        
        guc_ReadingABC_Flg = 1;  //读有效值开始标志
//        guc_StartReadABC_Flg = 1;
        EnyB_SetMeterCfgACK((gs_power_enerycal.ul_power[0] + gs_power_enerycal.ul_power[1] + gs_power_enerycal.ul_power[2]),0x10ed);//E1功率二次补偿值
        EnyB_SetMeterCfgACK((gs_power_enerycal.ul_Npower[0] + gs_power_enerycal.ul_Npower[1] + gs_power_enerycal.ul_Npower[2]),0x10ee);//E2功率二次补偿值
      }
    }
    
//    if(gs_RacPhy.uc_Physendtim > 0) //raccoon 通信超时计数
//    {
//        gs_RacPhy.uc_Physendtim--;
//        if(gs_RacPhy.uc_Physendtim == 0)
//        {
//          gui_SystemEvent |= flgEtGlb_RacTimeOut;
//        }
//    }
    Uart4_Dy10ms();       //raccoon
    Uart2_Dy10ms();         //485
    KeyScan();              //按键
    ComBom_10msDy();

    if(guc_ComSendDy!=0)
    {
        guc_ComSendDy--;
    }

    guc_SecCnt++;
    if(guc_SecCnt>=100)
    {
        guc_SecCnt=0;
        guc_TimeEvent |= flgEtTim_Second;
    }
    if(guc_CommDelayTime>0)
    {
        guc_CommDelayTime--;
    }
    
    PowDownChk();
   //掉电检测
//    if(gs_phyint.Aint >0 )
//    {
//        gs_phyint.Aint --;
//    }
//    if(gs_phyint.Bint >0 )
//    {
//        gs_phyint.Bint --;
//    }
//    if(gs_phyint.Cint >0 )
//    {
//        gs_phyint.Cint --;
//    }
    
//    Accumulate_abc();    
    Accumulate_abc_times++;  //分电量累加定时
}

/*=========================================================================================\n
* @function_name: UATR1Interrupt
* @function_file: System.c
* @描述: UART1发送接收中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=2
#pragma vector=0x3b               //中断号7,  UATR1，发送和接收中断
__interrupt void UATR1Interrupt(void)
{
    if(SCON1&BIT0)
    { 
        SCON1&=~BIT0;
    }

    if(SCON1&BIT1)
    {    
        SCON1&=~BIT1;
    }
}
/*=========================================================================================\n
* @function_name: UATRAndCFInterrupt
* @function_file: System.c
* @描述: UATR2、UATR4、CF中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=3
#pragma vector=0x43               //中断号8,  UATR2、UATR4、CF中断
__interrupt void UATRAndCFInterrupt(void)
{
  uint8 ucTemp;
    ucTemp  = ExInt2IFG;
    ucTemp &= ExInt2IE;
//   if(EXIF&BIT4)
//    {
        if(ucTemp&BIT7)          //脉冲中断
        {
            //用户代码
          ExInt2IFG&=(~BIT7);
          P9OD|=BIT3;
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
          TR1 = 1;              // 定时1ms,开定时器1 
          if( guc_80msCntFLG1)
          {
#if ( CF_TimeBase == CF_TimeBase_1MS)            
            guc_80msCntCF1=40;
#elif ( CF_TimeBase == CF_TimeBase_20MS)            
            guc_80msCntCF1=2;
#else
    #error "CF_TimeBase is error"
#endif            
          }else
          {
#if ( CF_TimeBase == CF_TimeBase_1MS)            
            guc_80msCntCF1=80;
#elif ( CF_TimeBase == CF_TimeBase_20MS)            
            guc_80msCntCF1=4;
#else
    #error "CF_TimeBase is error"
#endif            
          }       
          EnyB_IntCF(ABCPhy,active);
        }
        
        if(ucTemp&BIT4)          //UART2普通定时器中断
        {
            //用户代码
            ExInt2IFG&=(~BIT4);
        }
        
        if(ucTemp&BIT5)          //UART4普通定时器中断
        {
            //用户代码
            ExInt2IFG&=(~BIT5);
        }
       
        if(ucTemp&BIT2)          //UART4 发送中断
        {
            ExInt2IFG&=(~BIT2);
            SCON4&=(~BIT1);
            Uart4_Transmit();
        }
        
        if(ucTemp&BIT3)          //UART4 接收中断
        {
            SCON4&=(~BIT0);
            ExInt2IFG&=(~BIT3);
            Uart4_Receive();
        }

        if(ucTemp&BIT0)          //UART2 发送中断
        {
            SCON2&=(~BIT1);
            ExInt2IFG&=(~BIT0);
            Uart2_Transmit(); 
        }
        
        if(ucTemp&BIT1)          //UART2 接收中断
        {
            SCON2&=(~BIT0);
            ExInt2IFG&=(~BIT1);
            Uart2_Receive();
        }
        EXIF&=(~BIT4);
//    }

}
/*=========================================================================================\n
* @function_name: UATRAndRTCInterrupt
* @function_file: System.c
* @描述: UATR5、RTC、CF2中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=3
#pragma vector=0x4b               //中断号9,  UATR5、RTC、CF2中断
__interrupt void UATRAndRTCInterrupt(void)
{
  uint8 ucTemp;
    ucTemp  = ExInt3IFG;
    ucTemp &= ExInt3IE;
//   if(EXIF&BIT5)
//    {

        if(ucTemp&BIT7)          //CF2中断
        {
            ExInt3IFG&=(~BIT7);
            P9OD|=BIT7;
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
            TR0 = 1;        // 定时1ms,开定时器0 
            if( guc_80msCntFLG2)
            {
#if ( CF_TimeBase == CF_TimeBase_1MS)            
              guc_80msCntCF2=40;
#elif ( CF_TimeBase == CF_TimeBase_20MS)            
              guc_80msCntCF2=2;
#else
  #error "CF_TimeBase is error"
#endif            
            }else
            {
#if ( CF_TimeBase == CF_TimeBase_1MS)              
              guc_80msCntCF2=80;
#elif ( CF_TimeBase == CF_TimeBase_20MS)              
              guc_80msCntCF2=4;
#else
  #error "CF_TimeBase is error"
#endif              
            } 
            EnyB_IntCF(ABCPhy,reactive);
        }

        if(ucTemp&BIT6)      //RTC
        {
//          guc_TimeEvent|= flgEtTim_Second;
            ExInt3IFG&=(~BIT6);
        }
       
        if(ucTemp&BIT2)      //UART5发送
        {
            //Uart5_Transmit();
            SCON5&=(~BIT1);
            ExInt3IFG&=(~BIT2);
        }
        
        if(ucTemp&BIT3)      //UART5接收
        {
            //Uart5_Receive();
            SCON5&=(~BIT0);
            ExInt3IFG&=(~BIT3);
        }
        
        
        if(ucTemp&BIT4)          //UART3普通定时器中断
        {
            ExInt3IFG&=(~BIT4);
        }
        
        if(ucTemp&BIT5)          //UART5普通定时器中断
        {
            ExInt3IE&=~BIT5;        //关中断
            TCON5 &=~BIT0;          //停止定时
            ExInt3IFG&=(~BIT5);
        }         
                
        if(ucTemp&BIT0)          //UART3发送
        {
            //Uart3_Transmit();
            SCON3&=(~BIT1);
            ExInt3IFG&=(~BIT0);
        }
        
        if(ucTemp&BIT1)
        {
            //Uart3_Receive();
            SCON3&=(~BIT0);            
            ExInt3IFG&=(~BIT1);
        }
        EXIF&=(~BIT5);
//    }
}
/*=========================================================================================\n
* @function_name: PLLAndExINT3
* @function_file: System.c
* @描述: 外部中断3、PLL频率锁定中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=2
#pragma vector=0x53               //中断号10,  外部中断3、PLL频率锁定中断

__interrupt void PLLAndExINT3(void)
{

//    uint16 tmp;
    if(EXIF&BIT6)
    {
        if(ExInt4IFG&BIT2)
        {
//            TH0 = 0x00;             //
//            TL0 = 0x00;             //启动第一次进中断的时间设定
//            gs_phyint.Aint = 10;
//            TR0 = 1;                // 开定时器0
            ExInt4IFG&=(~BIT2 );
        }
          if(ExInt4IFG&BIT3)
        {
//            if(!gs_RacPhy.uc_Powstat&power_b &&!gs_RacPhy.uc_Powstat&power_a)
//            {
//                TR0 = 0; 
//                tmp = TH0<<8;
//                gs_phytim.BPhytim =tmp+TL0;
//                gs_phyint.Bint = 10;
//                TR0 = 1; 
//            }           
            ExInt4IFG&=(~BIT3 );
        }
        EXIF&=(~BIT6);
    }
  
}


#pragma register_bank=2
#pragma vector=0x5B               //中断号11

__interrupt void TimerA(void)
{
  
 
  
}

/*=========================================================================================\n
* @function_name: PowerOffInterrupt
* @function_file: System.c
* @描述: 外部中断3、PLL频率锁定中断
* 
* @参数: 
* 
* @返回: 
* @return: __interrupt void 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
#pragma register_bank=2
#pragma vector=0x63               //中断号10,  外部中断3、PLL频率锁定中断
__interrupt void PowerOffInterrupt(void)
{
  
}
 
 



 
