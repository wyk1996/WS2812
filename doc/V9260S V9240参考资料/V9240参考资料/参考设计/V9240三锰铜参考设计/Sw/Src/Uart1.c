#include "Include.h"
//初始化
uint8   guc_DyUart1Over;        //模拟串口超时保护
/*=========================================================================================\n
* @function_name: Init_Uart1
* @function_file: Uart1.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-01-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Init_Uart1(void)
{ //UART1 波特率发生器初始化
    TL1=TH1=0x55;
 
    CKCON|=BIT4;
    EICON&=~BIT7;                       //不翻倍
     
    TMOD&=0x0F;                     //AUTO LOAD
    TMOD|=0x20;                     //AUTO LOAD
    
    SCON1=0xD0;                     //9 BIT UART
    
    //TR1=1;                        //开启波特率发生器
    TCON|=BIT6;                     //开启波特率发生器
    P1OE|=(BIT1);
    P1IE|=(BIT1);
    P1OE&=(~BIT2);
    P1IE&=(~BIT2);
    P11FS=2;
    P12FS=2;
    ES=1;
    IE|=BIT6;
}
/*=========================================================================================\n
* @function_name: Uart1_Dy10ms
* @function_file: Uart3.c
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
void Uart1_Dy10ms(void)
{
    guc_DyUart1Over--;
    if(0x00== guc_DyUart1Over)
    {
        Init_Uart1();
        ComBom_Init(ComIndex_Uart1);    //将uart6 端口对应的 COM初始化
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
* @function_name: Uart3_Receive
* @function_file: Uart3.c
* @描述: UART3接收,在中断中调用该函数
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart1_Receive(void)
{

    uint8 temp,temp1;
    guc_DyUart1Over = Const_DyUart1Over;//端口超时保护
    //这里可以做奇偶校验判断
    ACC=SBUF1;                          //ACC 奇校验
    temp=P;
    temp1=(SCON1>>2)&BIT0;              //偶校验

    if(temp!=temp1)                     //奇偶校验位不正确
    {
        return;
    }

    //处于空闲状态或已经处于uart接收状态
    gs_ComGroup[ComIndex_Uart1].ucPort   = Port_Uart1;

    gs_ComGroup[ComIndex_Uart1].ucRecvTmr = Const_DyRvPack;     //设置数据包接收超时时间

    if(gs_ComGroup[ComIndex_Uart1].ucStt == ComStt_Idle)        //当前是空闲的状态的话，判断是否是帧头68
    {
        if(SBUF1==COMHEAD)               //接受到起始字节
        {
            gs_ComGroup[ComIndex_Uart1].ucStt = ComStt_Recv;
        }
    }

    if(gs_ComGroup[ComIndex_Uart1].ucStt == ComStt_Recv)            //这个判断是防止发送的时候有接收中断进入的可能
    {
        if(gs_ComGroup[ComIndex_Uart1].ucLen < Const_MaxBufLen)     //判断 com中的buf是否溢出
        {                                                           //防止缓存溢出
            gs_ComGroup[ComIndex_Uart1].ucBuf[gs_ComGroup[ComIndex_Uart1].ucLen++] = SBUF1;//数据存入缓冲区，指针加加

            if(gs_ComGroup[ComIndex_Uart1].ucLen>COMINITLEN)
            {
                if(gs_ComGroup[ComIndex_Uart1].ucBuf[7]!=COMHEAD)           //如果第7个不是68返回错误
                {
                    gs_ComGroup[ComIndex_Uart1].ucStt=ComStt_Idle;
                }else if(gs_ComGroup[ComIndex_Uart1].ucLen>=(gs_ComGroup[ComIndex_Uart1].ucBuf[9]+COMINITLEN+2))   //长度符合要求
                {
                    if(SBUF1==COMTAIL)
                    {
                        guc_ComSendDy=3;                                        //延时30ms发送
                        gs_ComGroup[ComIndex_Uart1].ucStt=ComStt_WaitProc;
                        gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // 设置标志
                        gs_ComGroup[ComIndex_Uart1].ucRecvTmr=0;
                        //关闭接收
                    }
                }
            }
        }
    }
}
/*=========================================================================================\n
* @function_name: Uart3_Transmit
* @function_file: Uart3.c
* @描述: UART3发送,中断中调用
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart1_Transmit(void)
{
    guc_DyUart1Over = Const_DyUart1Over;//端口超时保护

    if(gs_ComGroup[ComIndex_Uart1].ucPort   == Port_Uart1)
    {   
        //处于空闲状态或已经处于uart接收状态
        if(gs_ComGroup[ComIndex_Uart1].ucStt == ComStt_Send)
        {
            //STAOUTHIGH();
            if(gs_ComGroup[ComIndex_Uart1].ucPos >= gs_ComGroup[ComIndex_Uart1].ucLen)
            {   
                //发送完毕,初始化通讯控制状态
                Uart1_RevEn();
                ComBom_Init(ComIndex_Uart1);//将uart2 端口对应的 COM初始化
            }else
            {
                if(gs_ComGroup[ComIndex_Uart1].ucFrmHeadCnt<4)
                {                    
                    SCON1|=(BIT3);
                    SBUF1=0xfe;
                    gs_ComGroup[ComIndex_Uart1].ucFrmHeadCnt++;
                }else
                {
                    //这里可以做奇偶校验运算
                    ACC = gs_ComGroup[ComIndex_Uart1].ucBuf[gs_ComGroup[ComIndex_Uart1].ucPos]; //计算校验位
                    if(P==0)                                        //校验位
                    {
                        SCON1&=(~BIT3);
                    }
                    else
                    {
                        SCON1|=(BIT3);
                    }
                    Uart1_SendEn();//控制端口设置为发送模式（三线制通讯使用）  //还有未发送数据则写入外设发送缓存
                    SBUF1 = gs_ComGroup[ComIndex_Uart1].ucBuf[gs_ComGroup[ComIndex_Uart1].ucPos++];
                }
            }
        }else
        {   //如果在发送中断标记中，进入但是状态不正确，则初始化
            ComBom_Init(ComIndex_Uart1);//将uart3 端口对应的 COM初始化
            Uart1_RevEn();
        }
    }
}
