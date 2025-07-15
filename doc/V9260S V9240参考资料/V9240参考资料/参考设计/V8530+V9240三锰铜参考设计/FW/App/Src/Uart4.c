#define UART4_EXT
#include "Include.h"

//变量声明
uint8   guc_DyUart4Over;        //模拟串口超时保护
/*=========================================================================================\n
* @function_name: Init_Uart4
* @function_file: Uart4.c
* @描述: uart4的初始化
* 
* 
* @参数: 
* @param:ucBode  波特率  0-600  1-1200 2-2400 3-4800 4-9600
* @返回: 
* @作者:   lwb (2012-05-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Init_Uart4(uint8 ucBode)//计量芯片支持奇校验
{

    while(gs_RacPhy.uc_Uartstat != 0)
    {
        return;
    }
    if(ucBode>=5)
    {
        ucBode=2;                           //默认2400
    }

    //TMOD4 = 0x20;                               // 8-bit counter with auto-reload
    //TCON4 =BaudRateTable[ucBode].Type;     //时钟选择CLK  clear SMOD  SET  T1M,TR1 0X22
    //TL41=TH41=BaudRateTable[ucBode].THValue;//波特率设置							0XD5				
    //SCON4=0xD0;                                 //数据位9位,8位数据位+1校验位
    
    //P2OE &= ~BIT1;                      //Uart4发送口输出允许
    //P2IE &= ~BIT1;                      //Uart4发送口输入禁止
    //P2OE |= BIT0;                       //Uart4接收口输出禁止
    //P2IE |= BIT0;                       //Uart4接收口输入允许
    //P20FS=2;                            //P2.0配置成RXD
    //P21FS=2;                            //P2.1配置成TXD

	GPIOB->OEN |=BIT1;	//oen bit1置1禁止iob1输出功能 rx
	GPIOB->IE |= BIT1;	//ie  bit1置1使能iob1输入功能 rx
	GPIOB->OEN &=~BIT7; //使能tx脚输出功能
	GPIOB->IE &=~ BIT7; //禁止tx脚输入功能

	UART5->CTRL2 |= 0X02;//带奇偶校验，LSB优先 1+8+1+1 模式，奇偶校验位由 PMODE控制 
	UART5->CTRL2 |= 0x04;//V9240uart支持1bit奇校验，设置pmode为01奇校验
	UART5->BAUDDIV = (BaudRateTable[ucBode].THValue)/(BaudRateTable[ucBode].Type);
        
        UART5->INTSTS |= BIT1+BIT5;//清除接收发送中断标志
	UART5->CTRL |= 0x03;	//uart接收发送使能 

	UART5->CTRL |= 0X108;	//BIT8发送完成中断 BIT3接收中断 使能
	
	CORTEX_SetPriority_ClearPending_EnableIRQ(UART5_IRQn, 2);	//设置uart5中断优先级为0
    
    //ExInt2IE|=BIT2;             //发送中断使能
    //ExInt2IE|=BIT3;            //接收中断使能
////    Uart4_RevDisable();       //关闭接收中断
//    EIE|=BIT0;	//中断向量8使能 uart4收发中断包含于此

}

/*=========================================================================================\n
* @function_name: Uart4_Dy10ms
* @function_file: Uart4.c
* @描述: 模拟串口的超时保护
*       为防止锁死，定时保护，当没有任何数据发送和接收的时候，初始化
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart4_Dy10ms(void)
{
//#if (SysMode_TEST != 1) 
//        static uint8 offtim1 = 0;
//        static uint8 offtim2 = 0;
//        static uint8 offtim3 = 0;
//#endif
       //if(gs_RacCtrl.ucStatus==Rac_Send)
//    {
        if(gs_RacPhy.uc_Physendtim > 0)
        {
            gs_RacPhy.uc_Physendtim--;
            if(gs_RacPhy.uc_Physendtim == 0)
            {
                switch(gs_RacPhy.uc_Phystat)
                {
                    case APhy:
#if (Disp_ErrState == 1)        
                        gui_DispErrState &= 0xf0ff;
                        if(guc_RevCntA == 0x01)
                        {
                            gui_DispErrState |= 0x0200; 
                        }else
                        {
                            gui_DispErrState |= 0x0300;
                        }                        
#elif (Disp_ErrState == 2)
                        gul_dispbuf_timeoutA++;
#endif                      
                        gs_RacPhy.uc_Powstat |= power_a;
#if (SysMode_TEST != 1) 
                        offtim1++;
                        if(offtim1 >3)
                        {
#endif
                            gs_power.ul_power[0] = 0;
                            gs_power.ul_Npower[0] = 0;
                            gs_UI.ul_U[0] = 0;
                            gs_UI.ul_I[0] =  0;
#if (SysMode_TEST != 1)                            
                            offtim1 = 0;
                        }
#endif                        
                        break;
                     case BPhy:
#if (Disp_ErrState == 1)        
                        gui_DispErrState &= 0xff0f; 
                        if(guc_RevCntB == 0x01)
                        {
                            gui_DispErrState |= 0x0020; 
                        }else
                        {
                            gui_DispErrState |= 0x0030;
                        }
#elif (Disp_ErrState == 2)
                        gul_dispbuf_timeoutB++;                       
#endif                       
                        gs_RacPhy.uc_Powstat |= power_b;
#if (SysMode_TEST != 1) 
                        offtim2++;
                        if(offtim2 >3)
                        {
#endif                          
                            gs_power.ul_power[1] = 0; 
                            gs_power.ul_Npower[1] = 0;
                            gs_UI.ul_U[1] = 0;
                            gs_UI.ul_I[1] =  0;
#if (SysMode_TEST != 1)                            
                            offtim2 = 0;
                        }
#endif                        
                        break;
                     case CPhy:
#if (Disp_ErrState == 1)        
                        gui_DispErrState &= 0xfff0; 
                        if(guc_RevCntC == 0x01)
                        {
                            gui_DispErrState |= 0x0002; 
                        }else
                        {
                            gui_DispErrState |= 0x0003;
                        }
#elif (Disp_ErrState == 2)
                        gul_dispbuf_timeoutC++;                        
#endif                       
                        gs_RacPhy.uc_Powstat |=  power_c;
#if (SysMode_TEST != 1)                        
                        offtim3++;
                        if(offtim3 >3)
                        {
#endif                          
                            gs_power.ul_power[2] = 0;
                            gs_power.ul_Npower[2] = 0;
                            gs_UI.ul_U[2] = 0;
                            gs_UI.ul_I[2] =  0;
#if (SysMode_TEST != 1)                            
                            offtim3 = 0;
                         }
#endif                        
                        break;
                    default:
                        break;
                }
                if(gs_RacPhy.uc_Phystat ==CPhy)
                {
                    guc_ReadingABC_Flg = 0;  //读有效值结束标志
//                    guc_StartReadABC_Flg = 0;
                }
                gui_ReadABCPhy |= flgphy[(gs_RacPhy.uc_Phystat)>>2];
                gui_SystemEvent |= gui_ReadABCPhy<<8;
                gs_RacPhy.uc_Uartstat &= ~flgEtreadint;
                gs_RacPhy.uc_Sendstat = false;
            }
        }
//    }

}
/*=========================================================================================\n
* @function_name: Uart4_Receive
* @function_file: Uart4.c
* @描述: UART4接收,在中断中调用该函数
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart4_Receive(void)
{
    //uint8 temp,temp1;
    //guc_DyUart4Over = Const_DyUart4Over;//端口超时保护
    //这里可以做奇偶校验判断
  //  ACC=SBUF4;                          //ACC 奇校验
 //   temp=P;
 //   temp1=(SCON4>>2)&BIT0;              //偶校验

 //   if(temp!=temp1)                     //奇偶校验位不正确
 //   {
  //      return;
 //   }
    if(gs_RacCtrl.ucStatus==Rac_Rev)
    {
        if(gs_RacCtrl.ucRevPoint<gs_RacCtrl.ucRevLen)
        {
            gs_RacCtrl.ucBuf[gs_RacCtrl.ucRevPoint++]=UART5->DATA;

#if (Disp_ErrState == 1)        
            switch(gs_RacPhy.uc_Phystat)
            {
              case APhy:
                guc_RevCntA = 0x01;
                break;
              case BPhy:
                guc_RevCntB = 0x01;
                break;
              case CPhy:
                guc_RevCntC = 0x01;
                break;
              default:
                break;
            }
#endif            
            if(gs_RacCtrl.ucRevPoint==gs_RacCtrl.ucRevLen)
           //     if(gs_RacCtrl.ucRevPoint==3)
            {
//                Uart4_RevDisable();//接收完关闭接收中断
                
                gs_RacCtrl.ucStatus=Rac_WaitPro;
                if( gs_RacPhy.uc_Sendstat == true && gs_RacPhy.uc_Uartstat == flgEtreadint) 
               //if( gs_RacPhy.uc_Sendstat == true)
                {
//                    gs_RacPhy.uc_Physendtim = 0;  //读ABC三相超时关闭
                    gui_SystemEvent |= flgEtGlb_ReadPower;  //电量读取事件
                }
            }
        }
    }

}
/*=========================================================================================\n
* @function_name: Uart4_Transmit
* @function_file: Uart4.c
* @描述: UART4发送,中断中调用
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
    /**
     * @brief  检测数据中1的个数，返回0表示含有有偶数个数的1，返回1表示有奇数个数的1
     * @param  value 待检测的数据
     * @return  0---带检测数据中有偶数个1
     *          1---带检测的数据中奇数个1
     */
unsigned char check_bitnum(unsigned char value)
{
      uint8 i,temp_data,re_value;
      temp_data=value;
      re_value=0;
      for(i=0;i<8;i++)
      { 
          if((temp_data&0x01)==1)
          {
              re_value++;  
          }
          temp_data=temp_data>>1;
      }
      re_value=re_value%2;
      return re_value;
}




void Uart4_Transmit(void)
{
    uint8 i;
    //guc_DyUart4Over = Const_DyUart4Over;//端口超时保护
    if(gs_RacCtrl.ucStatus==Rac_Send)
       {
#if (Disp_ErrState == 1)        
            switch(gs_RacPhy.uc_Phystat)
            {
              case APhy:
                guc_RevCntA = 0x00;
                break;
              case BPhy:
                guc_RevCntB = 0x00;
                break;
              case CPhy:
                guc_RevCntC = 0x00;
                break;
              default:
                break;
            }
#endif         
           if(gs_RacCtrl.ucSendPoint<gs_RacCtrl.ucSendLen)   
           {
//               ACC = gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint];  //发送数据
//                if(P==0)                                        //校验位
//                {
//                    SCON4&=(~BIT3);
//                }
//                else
//                {
//                    SCON4|=(BIT3);
//                }
               //i=check_bitnum(gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint]);            //发送奇偶校验位
            //if(i==0)
            //{    
                  //SCON4 |=0x08;                        //偶校验的值  0x1906的bit3 0     
            //}
            //else
           // {    
                  //SCON4 &=0xf7;                       //奇校验的值  0x1906的bit3 1
            //}
                //SBUF4 = gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint++];
                UART5->DATA = gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint++];//发送数据
           }
           else
           {
               gs_RacCtrl.ucStatus=Rac_Rev; 

               PHACtrlDisable();
               PHBCtrlDisable();
               PHCCtrlDisable();
//               Uart4_RevEnable(); //发完开接收中断
           }
       }
}

