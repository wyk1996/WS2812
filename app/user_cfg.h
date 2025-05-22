#ifndef _USER_CFG_H_
#define _USER_CFG_H_


#include "gd32e23x.h"




#define COM_GPIO_PORT                   GPIOA
#define TX_PIN                          GPIO_PIN_9
#define COM_GPIO_PORT                   GPIOA
#define RX_PIN                          GPIO_PIN_10


#define COM1_GPIO_PORT                  GPIOA
#define RX1_PIN                         GPIO_PIN_2
#define COM1_GPIO_PORT                  GPIOA
#define TX1_PIN                         GPIO_PIN_15

#define SPI_CRC_ENABLE           1
#define ARRAYSIZE                10

#if 0
uint8_t spi0_send_array[ARRAYSIZE] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t spi1_send_array[ARRAYSIZE] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t spi0_receive_array[ARRAYSIZE];
uint8_t spi1_receive_array[ARRAYSIZE];
uint32_t send_n = 0, receive_n = 0;
#endif




void Uart1_Init(void);
// void Spi_Config(void);
void Fwdgt_Config(void);
void Fwdgt_Free(void);
void Timer5_Base(void);
void TimerPwm_Config(void);
void Adc_Init(void);
// void Uart0_Dma_Config(void);

#endif




