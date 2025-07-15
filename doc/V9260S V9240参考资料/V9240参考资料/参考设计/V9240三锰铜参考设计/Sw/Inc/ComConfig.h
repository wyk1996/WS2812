#ifndef __ComConfig_H__
#define __ComConfig_H__


#define Const_BPS_600       0               // bps设置
#define Const_BPS_1200      1               //
#define Const_BPS_2400      2               //
#define Const_BPS_4800      3               //
#define Const_BPS_9600      4               //

#define Const_MaxComNum     3               // 以下为对应的buf序号，如果三个共用一个buf，则全部为0，如果分开，则根据实际分开
#define ComIndex_Uart4      0               //  485
#define ComIndex_Uart2      1               //  红外
#define ComIndex_Uart1      2               //  载波
//端口
#define Const_MaxPortNum    3               // 总的端口数
#define Port_Uart4          0               // 端口与COM的对应关系
#define Port_Uart2          1               //
#define Port_Uart1          2               //
#define Port_Idle           0xff            // 该COM目前没有使用，可以允许数据写入到该COM对应的buf中

//超时时间
#define Const_DyUartOut     0xff            // 超时保护
#define Const_DyRvPack      50              // 500ms,接收超时时间，溢出表示一个数据包接结束标志

//COM口的状态
#define ComStt_Recv         01              // COM处于接收状态，在中断中处理
#define ComStt_Send         02              // COM处于发送状态，在中断中处理
#define ComStt_Idle         03              // COM处于空闲状态
#define ComStt_WaitProc     04              // COM处于等待处于状态，在主程序中处理

//与串口BUFF相关
#define Const_MaxBufLen     255             // 缓冲区容量

#endif
