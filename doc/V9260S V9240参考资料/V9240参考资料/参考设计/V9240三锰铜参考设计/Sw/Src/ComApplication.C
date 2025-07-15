#include "Include.h"
/*=========================================================================================\n
* @function_name: ComApp_Proc
* @function_file: ComApplication.C
* @描述: 通讯处理函数，主程序调用
*
* @参数:
* @返回:
* @作者: Lwb(2011-8-9)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ComApp_Proc(void)
{
    uint8 i;
    for(i=0; i<Const_MaxComNum; i++)
    {
        if(ComStt_WaitProc == gs_ComGroup[i].ucStt)
        {
            Uart_Proc(i);
        }
    }
}
/*=========================================================================================\n
* @function_name: Uart_RecvProc
* @function_file: ComApplication.C
* @描述:
*
*
* @参数:
* @param:port
* @返回:
* @作者: Lwb(2011-8-24)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart_Proc(uint8 port)
{
    S_COM* psCom = ((S_COM*)&gs_ComGroup + port);
    if(port >= Const_MaxComNum)
    {
        ComBom_InitAll();           //初始化所有COM
        return;
    }

    psCom->ucPort=port;
    
    if(BD_ProcFrame(psCom) == Ret_Err)
    {
        ComBom_Init(port);
    }
}
