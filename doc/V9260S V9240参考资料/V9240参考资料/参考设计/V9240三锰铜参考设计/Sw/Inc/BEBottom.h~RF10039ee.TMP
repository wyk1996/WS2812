#ifndef __BEBottom_H__
#define __BEBottom_H__

#define _SW_SE_Type     0

#define _SW_BE_Type  Const_AT24C256

#define Const_AT24C04_08_16     16
#define Const_AT24C64           64
#define Const_AT24C256          256
#define Const_AT24C512          512

//常量定义
#if(_SW_BE_Type==Const_AT24C04_08_16)
    #define BE_Const_DyClk      0       // 延时10us的时间（每个单位是5us）
    #define BE_Const_DyWrite    10      // 延时2ms的时间（每个单位是5us）
    #define BE_Const_Page       16      // 大容量E2的一页里面字节数
    #define BE_Const_ID         0xa0    // 大容量E2的器件地址
#elif(_SW_BE_Type==Const_AT24C64)
    #define BE_Const_DyClk      0       // 延时10us的时间（每个单位是5us）
    #define BE_Const_DyWrite    40      // 延时2ms的时间（每个单位是5us）
    #define BE_Const_Page       32      // 大容量E2的一页里面字节数
    #define BE_Const_ID         0xa0    // 大容量E2的器件地址
#else
    #define BE_Const_DyClk      1       // 延时10us的时间（每个单位是5us）
    #define BE_Const_DyWrite    40      // 延时5ms的时间（每个单位是5us） 
    #define BE_Const_Page       64      // 大容量E2的一页里面字节数
    #define BE_Const_ID         0xa0    // 大容量E2的器件地址
#endif



//端口定义
#define BE_I2C_CLK_1()      {P8OD |=BIT1; P8IE &=~BIT1;P8OE &=~BIT1; }   //CLK=1   P8.1 scl zyj 11.2
#define BE_I2C_CLK_0()      {P8OD &=~BIT1;P8IE &=~BIT1;P8OE &=~BIT1; }   //CLK=0


#define BE_I2C_SDA_1()      {P8OD |=BIT2; P8OE &=~BIT2; }   //SDA=1 P9IE &=~BIT1;  P8.2 sda zyj 11.2
#define BE_I2C_SDA_0()      {P8OD &=~BIT2;P8OE &=~BIT2; }   //SDA=0 P9IE &=~BIT1;

#define BE_I2C_SDA_IN()     {P8OE |=BIT2;P8IE |=BIT2;}                  //设置成输入模式
#define BE_I2C_SDA_ST       (P8ID&BIT2)                                 //读取数据线上的信号


#define BE_WP_1()           {BE_I2C_SDA_1();BE_I2C_CLK_1();}
#define BE_WP_0()           {}

uint8 BE_WriteP(uint32 address,uint8 *buff,uint8 Len);



       
uint8 BE_ReadP(uint32 address,uint8 *buff,uint8 Len);
uint8 BE_ReadB(uint32 address,uint8 *buff);
uint8 BE_ReadW(uint32 address,uint16 *buff);


#define Feed_WDT()     ;//CLRWDT() 


#endif


