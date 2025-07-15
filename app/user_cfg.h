#ifndef _USER_CFG_H_
#define _USER_CFG_H_


#include "gd32e23x.h"




#define COM_GPIO_PORT                   GPIOA
#define TX_PIN                          GPIO_PIN_9
#define COM_GPIO_PORT                   GPIOA
#define RX_PIN                          GPIO_PIN_10


#define COM1_GPIO_PORT                  GPIOA
#define RX1_PIN                         GPIO_PIN_15
#define COM1_GPIO_PORT                  GPIOA
#define TX1_PIN                         GPIO_PIN_2

#define SPI_CRC_ENABLE           1
#define ARRAYSIZE                10






void Uart1_Init(void);
// void Spi_Config(void);
void Fwdgt_Config(void);
void Fwdgt_Free(void);
void Timer5_Base(void);
void TimerPwm_Config(void);
void Adc_Init(void);
// void Uart0_Dma_Config(void);

#endif




