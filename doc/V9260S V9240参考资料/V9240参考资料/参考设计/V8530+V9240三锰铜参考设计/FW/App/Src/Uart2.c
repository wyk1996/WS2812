#include "Include.h"
#include "lib_uart.h"

//变量声明
//uint8   guc_DyUart2Over;        //模拟串口超时保护
/*=========================================================================================\n
* @function_name: Init_Uart2
* @function_file: Uart2.c
* @描述: uart2的初始化
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Init_Uart2(uint8 ucBode)	//8530对应uart0  485通信 上位机偶校验
{    

    if(ucBode>=5)
        {
            ucBode=2;                           //默认2400
        }
    	/*
        TMOD2 = 0x20;                               // 8-bit counter with auto-reload
        TCON2 =BaudRateTable[ucBode].Type;          //时钟选择CLK  clear SMOD  SET  T1M,TR1
        TL21=TH21=BaudRateTable[ucBode].THValue;    //波特率设置
        SCON2=0xD0;                                 //数据位9位,8位数据位+1校验位
    
        P2OE &= ~BIT5;                      //Uart4发送口输出允许
        P2IE &= ~BIT5;                      //Uart4发送口输入禁止
        P2OE |= BIT4;                       //Uart4接收口输出禁止
        P2IE |= BIT4;                       //Uart4接收口输入允许
        P24FS=2;                            //P2.0配置成RXD
        P25FS=2;                            //P2.1配置成TXD
        
        ExInt2IE|=BIT0;						//使能发送中断
        ExInt2IE|=BIT1;						//使能接收中断
        EIE|=BIT0;							//中断向量8使能
  		*/

        PMU->IOAOEN |=BIT12;	//oen bit1置1禁止ioA12输出功能 rx
        PMU->IOAIE |= BIT12;	//ie  bit1置1使能ioA12输入功能 rx

        GPIOB->OEN &=~BIT2; //使能tx脚输出功能 IOB2
        GPIOB->IE &=~ BIT2; //禁止tx脚输入功能 IOB2

        UART0->CTRL2 |= 0X02;//带奇偶校验，LSB优先 1+8+1+1 模式，奇偶校验位由 PMODE控制 
        //0x02改0x06 zzp1227上位机接收不对
        //UART0->CTRL2 |= 0x04;//V9240uart支持1bit奇校验，设置pmode为01奇校验
        UART0->BAUDDIV = (BaudRateTable[ucBode].THValue)/(BaudRateTable[ucBode].Type);
        
        UART0->INTSTS |= BIT1+BIT5;//清除接收发送中断
        UART0->CTRL |= 0x03;	//uart接收发送使能 
        UART0->CTRL |= 0X108;	//BIT8发送完成中断 BIT3接收中断 使能
        
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART0_IRQn, 0);	//设置uart5中断优先级为0

	
}

/*=========================================================================================\n
* @function_name: Uart2_Dy10ms
* @function_file: Uart2.c
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
void Uart2_Dy10ms(void)
{
    guc_DyUart2Over--;
    if(0x00== guc_DyUart2Over)
    {
        Init_Uart2(guc_485Bode);                   // 初始化模拟串口
        ComBom_Init(ComIndex_Uart2);    //将uart6 端口对应的 COM初始化
    }

    if(guc_BodeDely>0)                  //波特率切换
    {
        guc_BodeDely--;
        if(guc_BodeDely==0)
        {
            gui_RefreshEvent|=flgEtPara_Bode;
        }
    }
}
/*=========================================================================================\n
* @function_name: Uart2_Receive
* @function_file: Uart2.c
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
//void Uart2_Receive(void)
//{
//	
//    uint8 temp,temp1;
//    uint8 rec_buf2;
//    rec_buf2 = UART0->DATA;
//    
//    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护
//    //这里可以做奇偶校验判断
////    ACC=SBUF2;                          //ACC 奇校验
////    temp=P;
////    temp1=(SCON2>>2)&BIT0;              //偶校验
////
////    if(temp!=temp1)                     //奇偶校验位不正确
////    {
////        return;
////    }
////    if( UART_GetFlag( UART0, UART_STATE_RXPE))     //奇偶校验位不正确
////    {
////        UART_ClearFlag( UART0, UART_STATE_RXPE); //清除错误标志位
////        return;
////    }
//	
//    //处于空闲状态或已经处于uart接收状态
//    gs_ComGroup[ComIndex_Uart2].ucPort   = Port_Uart2;
//
//    gs_ComGroup[ComIndex_Uart2].ucRecvTmr = Const_DyRvPack;     //设置数据包接收超时时间
//
//
//    if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle)        //当前是空闲的状态的话，判断是否是帧头68
//    {
//
//        if(UART0->DATA==COMHEAD)               //接受到起始字节
//        {
//            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Recv;
//            guc_DyBgLed=2*gs_DispCont.ucLxCnt*gs_DispCont.ucLxTm;
//        }
//    }
// 
//    //处于空闲状态或已经处于uart接收状态
//    if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Recv)            //这个判断是防止发送的时候有接收中断进入的可能
//    {
//        if(gs_ComGroup[ComIndex_Uart2].ucLen < Const_MaxBufLen)     //判断 com中的buf是否溢出
//        {                                                           //防止缓存溢出
//            gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucLen++] = rec_buf2;//UART0->DATA;//数据存入缓冲区，指针加加
//
//            if(gs_ComGroup[ComIndex_Uart2].ucLen>COMINITLEN)
//            {
//                if(gs_ComGroup[ComIndex_Uart2].ucBuf[7]!=COMHEAD)           //如果第7个不是68返回错误
//                {
//                    gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_Idle;
//                }else if(gs_ComGroup[ComIndex_Uart2].ucLen>=(gs_ComGroup[ComIndex_Uart2].ucBuf[9]+COMINITLEN+2))   //长度符合要求
//                {
//                    if(UART0->DATA==COMTAIL)
//                    {
////                        guc_ComSendDy=3;                                        //延时30ms发送
//                        guc_ComSendDy=0;
//                        gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_WaitProc;
//                        gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // 设置标志
//                        gs_ComGroup[ComIndex_Uart2].ucRecvTmr=0;
//                        //关闭接收
//                    }
//                }
//            }
//        }
//    }
//}


void Uart2_Receive(void)
{
    uint8_t Uart_buf;
    
    Uart_buf = UART_ReceiveData( UART0);
    
    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护
    //这里可以做奇偶校验判断
    if( UART_GetFlag( UART0, UART_FLAG_RXPE))     //奇偶校验位不正确
    {
        UART_ClearFlag( UART0, UART_FLAG_RXPE); //清除错误标志位
        return;
    }

    //处于空闲状态或已经处于uart接收状态
    gs_ComGroup[ComIndex_Uart2].ucPort   = Port_Uart2;

    gs_ComGroup[ComIndex_Uart2].ucRecvTmr = Const_DyRvPack;     //设置数据包接收超时时间

    if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle)        //当前是空闲的状态的话，判断是否是帧头68
    {
        if(Uart_buf==COMHEAD)               //接受到起始字节
        {
            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Recv;
        }
    }

    if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Recv)            //这个判断是防止发送的时候有接收中断进入的可能
    {
        if(gs_ComGroup[ComIndex_Uart2].ucLen < Const_MaxBufLen)     //判断 com中的buf是否溢出
        {
            //防止缓存溢出
            gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucLen++] = Uart_buf;//数据存入缓冲区，指针加加

            if(gs_ComGroup[ComIndex_Uart2].ucLen>COMINITLEN)
            {
                if(gs_ComGroup[ComIndex_Uart2].ucBuf[7]!=COMHEAD)           //如果第7个不是68返回错误
                {
                    gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_Idle;
                }else if(gs_ComGroup[ComIndex_Uart2].ucLen>=(gs_ComGroup[ComIndex_Uart2].ucBuf[9]+COMINITLEN+2))   //长度符合要求
                {
                    if(Uart_buf==COMTAIL)
                    {
                        guc_ComSendDy=3;                                        //延时30ms发送
                        gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_WaitProc;
                        gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // 设置标志
                        gs_ComGroup[ComIndex_Uart2].ucRecvTmr=0;
                        //关闭接收
                    }
                }
            }
        }
    }
}


/*=========================================================================================\n
* @function_name: Uart2_Transmit
* @function_file: Uart2.c
* @描述: UART2发送,中断中调用
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart2_Transmit(void)
{
    uint8 ucbuf;
    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护

    if(gs_ComGroup[ComIndex_Uart2].ucPort   == Port_Uart2)
    {   //处于空闲状态或已经处于uart接收状态
        if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Send)
        {
            if(gs_ComGroup[ComIndex_Uart2].ucPos >= gs_ComGroup[ComIndex_Uart2].ucLen)
            {   //发送完毕,初始化通讯控制状态
//                GPIOB->OEN &=~BIT2; //使能tx脚输出功能 IOB2
//                GPIOB->IE &=~ BIT2; //禁止tx脚输入功能 IOB2
//                UART0->CTRL &= 0xEFF;	//关闭发送中断 bit8
		
                ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
            }
            else
            {
                if(gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt<4)
                {
                    //zzp 181119
                    //UART0->DATA = 0XFE;	
                    UART_SendData( UART0, 0xfe);
                    gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt++;
                }
                else
                {
                  //
                  ucbuf = gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucPos++];
                  //UART0->DATA = ucbuf;
                  UART_SendData( UART0, ucbuf);
                }

            }
        }
	else
        {   //如果在发送中断标记中，进入但是状态不正确，则初始化
            ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
            //Uart2_RevEn();
        }
    }
}

//void Uart2_Transmit(void)
//{
//    uint8 ucbuf;
//    
//    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护
//
//    if(gs_ComGroup[ComIndex_Uart2].ucPort   == Port_Uart2)
//    {   //处于空闲状态或已经处于uart接收状态
//        if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Send)
//        {
//            if(gs_ComGroup[ComIndex_Uart2].ucPos >= gs_ComGroup[ComIndex_Uart2].ucLen)
//            {   //发送完毕,初始化通讯控制状态
//           
//                ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
//            }else
//            {
//
//                  if(gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt<4)
//                  {
//                      UART_SendData( UART0, 0xfe);
//                      gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt++;
//                  }else
//                  {
//                      //这里可以做奇偶校验运算
//                      ucbuf = gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucPos++];
//                      UART_SendData( UART0, ucbuf);
//                  }
//            }
//        }else
//        {   //如果在发送中断标记中，进入但是状态不正确，则初始化
//            ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
//
//        }
//    }
//}
