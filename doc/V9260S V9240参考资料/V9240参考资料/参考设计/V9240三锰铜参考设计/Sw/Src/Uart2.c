#include "Include.h"

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
void Init_Uart2(uint8 ucBode)
{    

    if(ucBode>=5)
        {
            ucBode=2;                           //默认2400
        }
    
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
        
        ExInt2IE|=BIT0;
        ExInt2IE|=BIT1;
        EIE|=BIT0;
  
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
void Uart2_Receive(void)
{

    uint8 temp,temp1;
    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护
    //这里可以做奇偶校验判断
    ACC=SBUF2;                          //ACC 奇校验
    temp=P;
    temp1=(SCON2>>2)&BIT0;              //偶校验

    if(temp!=temp1)                     //奇偶校验位不正确
    {
        return;
    }
    //处于空闲状态或已经处于uart接收状态
    gs_ComGroup[ComIndex_Uart2].ucPort   = Port_Uart2;

    gs_ComGroup[ComIndex_Uart2].ucRecvTmr = Const_DyRvPack;     //设置数据包接收超时时间


    if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle)        //当前是空闲的状态的话，判断是否是帧头68
    {

        if(SBUF2==COMHEAD)               //接受到起始字节
        {
            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Recv;
            guc_DyBgLed=2*gs_DispCont.ucLxCnt*gs_DispCont.ucLxTm;
        }
    }
 
    //处于空闲状态或已经处于uart接收状态
    if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Recv)            //这个判断是防止发送的时候有接收中断进入的可能
    {
        if(gs_ComGroup[ComIndex_Uart2].ucLen < Const_MaxBufLen)     //判断 com中的buf是否溢出
        {                                                           //防止缓存溢出
            gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucLen++] = SBUF2;//数据存入缓冲区，指针加加

            if(gs_ComGroup[ComIndex_Uart2].ucLen>COMINITLEN)
            {
                if(gs_ComGroup[ComIndex_Uart2].ucBuf[7]!=COMHEAD)           //如果第7个不是68返回错误
                {
                    gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_Idle;
                }else if(gs_ComGroup[ComIndex_Uart2].ucLen>=(gs_ComGroup[ComIndex_Uart2].ucBuf[9]+COMINITLEN+2))   //长度符合要求
                {
                    if(SBUF2==COMTAIL)
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
    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护

    if(gs_ComGroup[ComIndex_Uart2].ucPort   == Port_Uart2)
    {   //处于空闲状态或已经处于uart接收状态
        if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Send)
        {
            if(gs_ComGroup[ComIndex_Uart2].ucPos >= gs_ComGroup[ComIndex_Uart2].ucLen)
            {   //发送完毕,初始化通讯控制状态
                //发送完，配置回去
//              P25FS=0;                            //P2.5配置成GPIO,只有在发送的时候切换成TXD
//              P2OE &= ~BIT5;                      //UART2发送口输出允许
//              P2OD&=(~BIT5);
//              P2IE &= ~BIT5;                      //UART2发送口输入禁止
//              ExInt2IE&=(~BIT0);                  //关闭发送中断
                Uart2_RevEn();
                ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
            }else
            {
                if(gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt<4)
                {
                    SCON2|=(BIT3);
                    SBUF2=0xfe;
                    gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt++;
                }else
                {
                    //这里可以做奇偶校验运算
                    ACC = gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucPos]; //计算校验位
                    if(P==0)                                        //校验位
                    {
                        SCON2&=(~BIT3);
                    }
                    else
                    {
                        SCON2|=(BIT3);
                    }
                    Uart2_SendEn();//控制端口设置为发送模式（三线制通讯使用）  //还有未发送数据则写入外设发送缓存
                    SBUF2 = gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucPos++];
                }

            }
        }else
        {   //如果在发送中断标记中，进入但是状态不正确，则初始化
            ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
            Uart2_RevEn();
        }
    }
}

