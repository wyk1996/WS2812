#ifndef __Uart2_H__
#define __ComBottom_Uart2_H__

#define Const_DyUart2Over   0xff                     // 模拟口的溢出保护，在10ms中执行，等于0xff则初始化

#define Uart2_RevEn()
#define Uart2_SendEn()

#define CONST_UART2_DEFBPS  Const_BPS_1200


void Init_Uart2(uint8 ucBode);
void Uart2_Dy10ms(void);
void Uart2_Receive(void);
void Uart2_Transmit(void);



#endif
