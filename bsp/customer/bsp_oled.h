#ifndef _BSP_OLED_H_
#define _BSP_OLED_H_

#include "gd32e23x.h"
#include "Includes.h"







#define OLED_CMD  0	
#define OLED_DATA 1

#define LCD_POWER_CTL_PORT                                  GPIOB
#define LCD_POWER_CTL_PIN                                   GPIO_PIN_6

#define LCD_POWER_CTL_ON                                    gpio_bit_set(LCD_POWER_CTL_PORT, LCD_POWER_CTL_PIN)	// LCD电源控制
#define LCD_POWER_CTL_OFF                                   gpio_bit_reset(LCD_POWER_CTL_PORT, LCD_POWER_CTL_PIN)


// #define SPI_POWER_H        gpio_bit_write(GPIOF, GPIO_PIN_0, SET)
// #define SPI_POWER_L        gpio_bit_write(GPIOF, GPIO_PIN_0, RESET)

#define SPI_POWER_H        gpio_bit_write(LCD_POWER_CTL_PORT, LCD_POWER_CTL_PIN, SET)
#define SPI_POWER_L        gpio_bit_write(LCD_POWER_CTL_PORT, LCD_POWER_CTL_PIN, RESET)

#if 0
#define SPI_NSS_H        gpio_bit_write(GPIOA, GPIO_PIN_10, SET)
#define SPI_NSS_L        gpio_bit_write(GPIOA, GPIO_PIN_10, RESET)
#endif

// #define SPI_RES_H        gpio_bit_write(GPIOA, GPIO_PIN_9, SET)
// #define SPI_RES_L        gpio_bit_write(GPIOA, GPIO_PIN_9, RESET)

#define SPI_RES_H        gpio_bit_write(GPIOB, GPIO_PIN_12, SET)
#define SPI_RES_L        gpio_bit_write(GPIOB, GPIO_PIN_12, RESET)



// #define SPI_DC_H        gpio_bit_write(GPIOA, GPIO_PIN_0, SET)
// #define SPI_DC_L        gpio_bit_write(GPIOA, GPIO_PIN_0, RESET)

#define SPI_DC_H        gpio_bit_write(GPIOB, GPIO_PIN_14, SET)
#define SPI_DC_L        gpio_bit_write(GPIOB, GPIO_PIN_14, RESET)



  

















void OLED_ShowString(INT8U x,INT8U y,char *chr,INT8U size1);
void OLED_DrawPoint(INT8U x,INT8U y);
void OLED_ClearPoint(INT8U x,INT8U y);
void OLED_ShowChar(INT8U x,INT8U y,INT8U chr,INT8U size1);
void OLED_Clear(void);
void OLED_Refresh_2(void);
void OLED_Refresh(void);
void gpio_config_SPI(void);

void rcu_config_SPI(void);
void spi_config(void);
void spi_config_send(INT8U data, INT8U mode);
INT16U spi_config_receive(void);

void spi_config_send_init(INT8U data, INT8U mode);

void spi_config_send_data(INT8U data);
void spi_send_to_qg2864_init(void)  ;
void oled_time_handle(void);
void Oled_Handler(void);
    
#endif




