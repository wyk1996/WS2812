#ifndef __COMAPPLICATION_H__
#define __COMAPPLICATION_H__

#define RECEIVE   0x55
#define SEND      0xAA

typedef  struct 
{   
    uint8  ucPort;
    uint8* SendBuf;
    uint8* RevBuf;
    uint16 ucLen;
}GsComProc;

//函数声明列表
void ComApp_Proc(void);                     //COM口的处理
void Uart_Proc(uint8 port);


#endif
