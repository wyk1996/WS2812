#ifndef __Uart4_H__
#define __ComBottom_Uart4_H__


#ifndef UART4_EXT
    #define UART4_EXT extern
#else
    #define UART4_SELF 
#endif

#define Const_DyUart4Over   0xff                        // 模拟口的溢出保护，在10ms中执行，等于0xff则初始化
//#define RS485_TWOLINE

//#define Uart4_RevEn()       {P9OE&=~BIT5;P9IE&=~BIT5;P9OD|=BIT5;}
//#define Uart4_SendEn()      {P9OE&=~BIT5;P9IE&=~BIT5;P9OD&=~BIT5;}

//#define Uart4_CtrIoIdle()   {P9OE|=BIT5;P9IE&=~BIT5;}

#define Uart4_RevEnable()        {}//{P2OE |= BIT1;P2IE &=(~BIT1);P21FS=0;ExInt2IE &=(~BIT2);\
                                  P2OE |= BIT0;P2IE |= BIT0;P20FS=2;ExInt2IE |= BIT3;}

#define Uart4_RevDisable()       {}//{P2OE |= BIT0;P2IE &=(~BIT0);P20FS=0;ExInt2IE &=(~BIT3);\
                                  P2OE &=(~BIT1);P2IE &=(~BIT1);P21FS=2;ExInt2IE |= BIT2;}

#define Uart4_SendEn()      {}

#define Uart4_CtrIoIdle()   {}


#define CONST_UART4_DEFBPS  Const_BPS_2400
 
UART4_EXT uint8 offtim1;
UART4_EXT uint8 offtim2;
UART4_EXT uint8 offtim3;
        
void Init_Uart4(uint8 ucBode);
void Uart4_Dy10ms(void);
void Uart4_Receive(void);
void Uart4_Transmit(void);
unsigned char check_bitnum(unsigned char value);




#endif
