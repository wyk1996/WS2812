#define COMBOTTOMEXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: ComBom_Init
* @function_file: ComBottom.C
* @描述: 指定COM口进行初始化
*
*
* @参数:
* @param:ucNo
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ComBom_Init(uint8 ucNo)
{
    //清空接收缓存
    if(ucNo < Const_MaxComNum)                          // 增加长度保护限制
    {
        gs_ComGroup[ucNo].ucPort=Port_Idle;
        gs_ComGroup[ucNo].ucLen = 0;
        gs_ComGroup[ucNo].ucPos = 0;
        gs_ComGroup[ucNo].ucFrmHeadCnt=0;
        //初始化通讯状态
        gs_ComGroup[ucNo].ucStt = ComStt_Idle;
        gs_ComGroup[ucNo].ucRecvTmr = 0;                // 定时器无效状态
        gs_ComGroup[ucNo].ucOverTmr = 0;                // 定时器无效状态
    }
}

/*=========================================================================================\n
* @function_name: ComBom_InitAll
* @function_file: ComBottom.C
* @描述: 把所有的COM都初始化
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ComBom_InitAll(void)
{
    uint8 i;

    for(i=0;i<Const_MaxComNum;i++)
    {
        ComBom_Init(i);
    }
}
/*=========================================================================================\n
* @function_name: ComBom_StartSend
* @function_file: ComBottom.C
* @描述: 根据入口参数，确定启动哪个端口的发送和把那个buff对应的数据发送出去
*
*
* @参数:
* @param:ucNo
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ComBom_StartSend(uint8 ucNo)
{
    gs_ComGroup[ucNo].ucStt = ComStt_Send;              // 将该COM口设置成发送转换
    gs_ComGroup[ucNo].ucFrmHeadCnt=0;
    if(gs_ComGroup[ucNo].ucPort == Port_Uart4)
    {
        Uart4_Transmit();                               // uart6 启动发送
    }
    else if(gs_ComGroup[ucNo].ucPort == Port_Uart2)
    {
        //EXIF|=BIT4;
        //ExInt2IFG|=BIT0;
        //P25FS=2;
        //P2OE&=(~BIT5);
        //ExInt2IE|=(BIT0);
        Uart2_Transmit();                              //// 模拟串口启动发送
    }
    else
    {
        ComBom_Init(ucNo);                              // 无效，初始化COM
    }
}
/*=========================================================================================\n
* @function_name: ComBom_1msDy
* @function_file: ComBottom.C
* @描述: 根据入口参数，实现不同的延时，提供给1ms中断调用
*       通过该实现是否一个数据包结束标志
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ComBom_10msDy(void)//最小延时10ms
{
    uint8 i;
    for(i=0; i<Const_MaxComNum; i++)
    {                                                       //对COM进行延时
        if(gs_ComGroup[i].ucRecvTmr > 0)                    // 判断接收超时
        {
            gs_ComGroup[i].ucRecvTmr--;
            if(0 == gs_ComGroup[i].ucRecvTmr)               // 如果间隔一定时间内未收到有效数据，则表示一个数据包已经收到
            {
                gs_ComGroup[i].ucStt = ComStt_WaitProc;     // 处于等待处理模式，提交给应用层
                gui_SystemEvent |= flgEtGlb_NeedPrtclChk;   // 设置标志
            }
        }
    }
}


