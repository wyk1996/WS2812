/**
  * @file    target_isr.c
  * @author  Application Team
  * @version V4.3.0
  * @date    2018-09-04
  * @brief   Main Interrupt Service Routines.
******************************************************************************/
#include "include.h"
#include "target_isr.h"
#include "main.h"
uint8_t j = 0;
uint8_t uart;
uint32_t tim_buf[2] = {0,0};
uint32_t tim_mbuf[1024];
uint32_t x=0,y=0;
uint8_t Flag_reac=3;
uint8_t Cf_status = 2;

uint8 ktx_count = 0;
extern unsigned long abc;

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  while(1)
  {}
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  while(1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/**
  * @brief  This function handles PMU interrupt request.
  * @param  None
  * @retval None
  */
void PMU_IRQHandler(void)
{
	if((*PMU_IOAINTSTS)&BIT4)	//产生ioa4外部中断
	{
		*PMU_IOAINTSTS |= BIT4;		//写1清零
		Ioawkup_flag = 1;
	}

}

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  while(1);
}

/**
  * @brief  This function handles U32K0 interrupt request.
  * @param  None
  * @retval None
  */
void U32K0_IRQHandler(void)
{
}

/**
  * @brief  This function handles U32K1 interrupt request.
  * @param  None
  * @retval None
  */
void U32K1_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C interrupt request.
  * @param  None
  * @retval None
  */
void I2C_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART0 interrupt request.
  * @param  None
  * @retval None
  */
void UART0_IRQHandler(void)
{
	
    if(UART0->INTSTS&BIT5)	//发送完成
	{
          UART0->INTSTS |= BIT5;	//写1清零
          Uart2_Transmit();
          
          if(ktx_count <4)
          {
            UART_SendData( UART0, ((abc>>8*ktx_count)&0xff));
            ktx_count ++;
            if(ktx_count > 3)
            {
                UART0->INTSTS |= BIT5;	//写1清零
            }
          }
	}
	
	if(UART0->INTSTS&BIT1)	//接收完成
	{
          UART0->INTSTS |= BIT1;	//写1清零                
//                uart = UART5->DATA;
//                UART5->DATA = 0;                
          Uart2_Receive();
	}
  
}

/**
  * @brief  This function handles UART1 interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART2 interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART3 interrupt request.
  * @param  None
  * @retval None
  */
void UART3_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
    
    if(UART5->INTSTS&BIT5)	//发送完成
	{
		UART5->INTSTS = BIT5;	//写1清零
		Uart4_Transmit();
	}
	
	if(UART5->INTSTS&BIT1)	//接收完成
	{
		UART5->INTSTS = BIT1;	//写1清零
                
//                uart = UART5->DATA;
//                UART5->DATA = 5;
                
		Uart4_Receive();
	}
}
  


/**
  * @brief  This function handles ISO78160 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78160_IRQHandler(void)
{
}

/**
  * @brief  This function handles ISO78161 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78161_IRQHandler(void)
{
}

/**
  * @brief  This function handles TMR0 interrupt request.
  * @param  None
  * @retval None
  */
void TMR0_IRQHandler(void)
{
  if(TMR0->INT&BIT0)
  {
    TMR0->INT = 1;
    if(Flag_reac!=3&&Cf_status!=4)
    {
      EnyB_IntCF(Flag_reac,Cf_status);
      Flag_reac = 3;
      Cf_status = 4;
    }
  }
  
  
}

/**
  * @brief  This function handles TMR1 interrupt request.
  * @param  None
  * @retval None
  */
void TMR1_IRQHandler(void)
{
  
  
  uint8 i;
  static uint8 act,react;
  if (TMR1->INT&BIT0)  //检查TIM2更新中断发生与否
  {
    
//    TMR0->CTRL |= 0X01;//zzp
//    tim_buf[0] = TMR0->VALUE;
    
    //****************************************************************    
      
//    tim_buf[1] = TMR0->VALUE;
//    TMR0->CTRL &=~ 0X01;
//    TMR0->VALUE = 0;
//    
//    tim_mbuf[x]=tim_buf[0]-tim_buf[1];
//    tim_buf[0] = 0;
//    tim_buf[1] = 0;
//    x++;
//    while(x>1023);
    
//    if(j%2 == 0)
//    {
//      GPIOB->DAT &=~ BIT6;
//    }
//    else
//    {
//      GPIOB->DAT |= BIT6;
//    }
//    j++;
//    if(j >11)
//    {
//      j = 0;
//    }
    
    
//**************************************************************** 
    
    //*************************
       if(gs_ipmtim.uc_160msCnt > 0)
        {
            gs_ipmtim.uc_160msCnt --;
            if(gs_ipmtim.uc_160msCnt  == 0)
            {
                gs_ipmtim.uc_160msCnt  = 0;
            }
        }
        
        if(gs_ipmtim.uc_160msCntRE> 0)
        {
            gs_ipmtim.uc_160msCntRE--;
            if(gs_ipmtim.uc_160msCntRE== 0)
            {
                gs_ipmtim.uc_160msCntRE= 0;
            }
        }
        
        if(gs_ipmtim.uc_80msCnt>0)
        {
            gs_ipmtim.uc_80msCnt--;
            if(gs_ipmtim.uc_80msCnt == 0)
            {
                GPIOB->DAT |= BIT0;    //有功 低电平亮
            }
        }
        if(gs_ipmtim.uc_80msCntRE>0)
        {
            gs_ipmtim.uc_80msCntRE--;
            if(gs_ipmtim.uc_80msCntRE == 0)
            {
                GPIOB->DAT |= BIT6;    //无功 低电平亮
            }
        }
    
    //**************************
    /*各相有功能量*/
    for(i =0;i <3;i++)
   // for(i = 3;i>0;i--)
    {
      gs_power.ul_power_all[i] += (gs_power_enerycal.ul_power[i] >> 8);
      gs_power.ui_power_rem[i] += (gs_power_enerycal.ul_power[i] & 0x000000ff);
      if(gs_power.ui_power_rem[i] > 255)//4930 200
      {
        gs_power.ul_power_all[i]++;
        gs_power.ui_power_rem[i] -= 255;
      }
      
      if(gs_power.ul_power_all[i] >= gs_JbPm.RacEGYTH)
      {
        gs_power.ul_power_all[i] -= (gs_JbPm.RacEGYTH);
        EnyB_IntCF(i,active);
//        Flag_reac |= i;
//        Cf_status |= active; 
      }
    }  
    /* 总有功能量*/
    gs_power.ul_power_all[3]  += (gs_power_enerycal.ul_power[0] >> 8) + (gs_power_enerycal.ul_power[1] >> 8)
      + (gs_power_enerycal.ul_power[2] >> 8);
    gs_power.ui_power_rem[3] += (gs_power_enerycal.ul_power[0] & 0x000000ff) + (gs_power_enerycal.ul_power[1] & 0x000000ff)
      + (gs_power_enerycal.ul_power[2] & 0x000000ff);
//    
//     gs_power.ul_power_all[3] += 14180;
//     gs_power.ui_power_rem[3] += 200;
    /*
    if( gs_power.ui_power_rem[3]  > 255)//14810  200
    {
      gs_power.ul_power_all[3]++;
      gs_power.ui_power_rem[3] -= 255;
    }
    //*///ZZP0212
    ///***********************************************
    if( gs_power.ui_power_rem[3]  > 765)
    {
      gs_power.ul_power_all[3]+=3;
      gs_power.ui_power_rem[3] -= 765;
    }
    if( gs_power.ui_power_rem[3]  > 510)
    {
      gs_power.ul_power_all[3]+=2;
      gs_power.ui_power_rem[3] -= 510;
    }
    if( gs_power.ui_power_rem[3]  > 255)//14810  200
    {
      gs_power.ul_power_all[3]++;
      gs_power.ui_power_rem[3] -= 255;
    }
    //***********************************************ZZP0212*/
    if(gs_power.ul_power_all[3] >= gs_JbPm.RacEGYTH)
    {
      if(act == 0)
      {
        gs_ipmtim.uc_160msCnt = 800;//16;
        gs_ipmtim.uc_80msCnt=400;//8; 
        act = 1;
      }
      else
      {
        if(gs_ipmtim.uc_160msCnt < 800 && gs_ipmtim.uc_160msCnt > 0)//160毫秒未到 160毫秒监控大电流情况
        {
          gs_ipmtim.uc_80msCnt=400-(gs_ipmtim.uc_160msCnt/2);
        }
        else if(gs_ipmtim.uc_160msCnt ==0 )                                    //大于等于160毫秒
        {
          gs_ipmtim.uc_80msCnt=400;//8;      
        }
        gs_ipmtim.uc_160msCnt = 800;//16;
        
      }
      GPIOB->DAT &=~ BIT0;    //有功 低电平亮
      //gs_power.ul_underpow = 0;
      gs_power.ul_power_all[3] -= (gs_JbPm.RacEGYTH);
      EnyB_IntCF(ABCPhy,active); 
      //y=x;
//      Flag_reac |= ABCPhy;
//      Cf_status |= active;
      
    }
    
    ///*各相无功能量*/
    for(i =0;i <3;i++)
    {
      gs_power.ul_Npower_all[i] += (gs_power_enerycal.ul_Npower[i] >> 8);
      gs_power.ui_Npower_rem[i] += (gs_power_enerycal.ul_Npower[i] & 0x000000ff);
      if(gs_power.ui_Npower_rem[i] > 255)
      {
        gs_power.ul_Npower_all[i]++;
        gs_power.ui_Npower_rem[i] -= 255;
      }
      
      if(gs_power.ul_Npower_all[i] >= gs_JbPm.RacREEGYTH)
      {
        gs_power.ul_Npower_all[i] -= (gs_JbPm.RacREEGYTH);
        EnyB_IntCF(i,reactive);
        
//        Flag_reac |= i;
//        Cf_status |= reactive;
        
      }
    }  
   // /* 总无功能量*/
    gs_power.ul_Npower_all[3]  += (gs_power_enerycal.ul_Npower[0] >> 8) + (gs_power_enerycal.ul_Npower[1] >> 8)
      + (gs_power_enerycal.ul_Npower[2] >> 8);
    gs_power.ui_Npower_rem[3] += (gs_power_enerycal.ul_Npower[0] & 0x000000ff) + (gs_power_enerycal.ul_Npower[1] & 0x000000ff)
      + (gs_power_enerycal.ul_Npower[2] & 0x000000ff);
    /************************************************
    if( gs_power.ui_Npower_rem[3]  > 255)
    {
      gs_power.ul_Npower_all[3]++;
      gs_power.ui_Npower_rem[3] -= 255;
    } 
    /************************************************///zzp0212
    //************************************************
    if( gs_power.ui_Npower_rem[3]  > 765)
    {
      gs_power.ul_Npower_all[3]+=3;
      gs_power.ui_Npower_rem[3] -= 765;
    }
    if( gs_power.ui_Npower_rem[3]  > 510)
    {
      gs_power.ul_Npower_all[3]+=2;
      gs_power.ui_Npower_rem[3] -= 510;
    }
    if( gs_power.ui_Npower_rem[3]  > 255)
    {
      gs_power.ul_Npower_all[3]++;
      gs_power.ui_Npower_rem[3] -= 255;
    }
    //************************************************
    if(gs_power.ul_Npower_all[3] >=gs_JbPm.RacREEGYTH)
    {
      if(react== 0)
      {
        gs_ipmtim.uc_160msCntRE= 800;//16;
        gs_ipmtim.uc_80msCntRE=400;//8; 
        react = 1;
      }
      else
      {
        if(gs_ipmtim.uc_160msCntRE< 16 && gs_ipmtim.uc_160msCntRE> 0)      //160毫秒未到 160毫秒监控大电流情况
        {
          gs_ipmtim.uc_80msCntRE=8-(gs_ipmtim.uc_160msCntRE/2);
        }
        else if(gs_ipmtim.uc_160msCntRE==0 )                                    //大于等于160毫秒
        {
          gs_ipmtim.uc_80msCntRE=400;//8;      
        }
        gs_ipmtim.uc_160msCntRE= 800;//16;
      }
      GPIOB->DAT &=~ BIT6;    //无功 低电平亮
      //gs_power.ul_Nunderpow = 0;
      gs_power.ul_Npower_all[3] -=gs_JbPm.RacREEGYTH;
//      Flag_reac |= ABCPhy;
//      Cf_status |= reactive;
      EnyB_IntCF(ABCPhy,reactive);
    }

   
    TMR1->INT |= 1;	//写1清零标志
  }
  
  
}

/**
  * @brief  This function handles TMR2 interrupt request.
  * @param  None
  * @retval None
  */
void TMR2_IRQHandler(void)
{
    
  if (TMR2->INT&BIT0)           //10ms中断
  {     
    TMR2->INT |= 1;	//写1清零
    //***********************************************************
 
    //***********************************************************
    
//    if(Flag_reac!=3&&Cf_status!=4)
//    {
//      EnyB_IntCF(Flag_reac,Cf_status);
//      Flag_reac = 3;
//      Cf_status = 4;
//    }
    
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
        //EnyB_SetMeterCfgACK((gs_power_enerycal.ul_power[0] + gs_power_enerycal.ul_power[1] + gs_power_enerycal.ul_power[2]),0x10ed);//E1功率二次补偿值
        //EnyB_SetMeterCfgACK((gs_power_enerycal.ul_Npower[0] + gs_power_enerycal.ul_Npower[1] + gs_power_enerycal.ul_Npower[2]),0x10ee);//E2功率二次补偿值
      }
    }
    
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
    //****************************************************************
    
    
    
    //PowDownChk();
    //掉电检测 
    //zzp1130	Accumulate_abc_times++;  //分电量累加定时
  }
  
}

/**
  * @brief  This function handles TMR3 interrupt request.
  * @param  None
  * @retval None
  */
void TMR3_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM0 interrupt request.
  * @param  None
  * @retval None
  */
void PWM0_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM1 interrupt request.
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM2 interrupt request.
  * @param  None
  * @retval None
  */
void PWM2_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM3 interrupt request.
  * @param  None
  * @retval None
  */
void PWM3_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{
}

/**
  * @brief  This function handles FLASH interrupt request.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler(void)
{
}

/**
  * @brief  This function handles ANA interrupt request.
  * @param  None
  * @retval None
  */
void ANA_IRQHandler(void)
{
/*
  if (ADC_GetAutoDoneFlag())
  {
    /* Clear ADC auto done interrupt flag 
    ADC_ClearAutoDoneFlag();
    adc_done = 1;
  }*/
	
  if (ANA->INTSTS & ANA_INTSTS_INTSTS0)		//adc手动转换中断标志
  {
    /* Clear ADC auto done interrupt flag */
    ANA->INTSTS = ANA_INTSTS_INTSTS0;
    adc_done = 1;
  }

  if ((*ANA_INTSTS)&BIT8)
  {
    *ANA_INTSTS |= BIT8;		//VDCin上电或掉电中断 写1清零
    Rtcwkup_flag = 1;
  }
	
}

/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
}

/*********************************** END OF FILE ******************************/
