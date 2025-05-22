
#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__

#include "IndependentDataType.h"
#include "bsp_led.h"


//input IO
#if 0
#define BL0939_I_LINK_PIN			GPIO_PIN_1
#define BL0939_I_LINK_GPIO_PORT		GPIOF

#define EMERGENCY_STOP_PIN			GPIO_PIN_13
#define EMERGENCY_STOP_GPIO_PORT	GPIOA
#endif




//output IO
#define RELAY_L_CTRL_PIN				                    GPIO_PIN_10                                     //继电器L相控制引脚
#define RELAY_L_CTRL_GPIO_PORT		                        GPIOB

#define RELAY_N_CTRL_PIN				                    GPIO_PIN_11                                     //继电器N相控制引脚
#define RELAY_N_CTRL_GPIO_PORT		                        GPIOB

#define POWER_CTL_PORT                                      GPIOA
#define POWER_CTL_PIN                                       GPIO_PIN_12
#define POWER_CTL_ON                                        gpio_bit_set(POWER_CTL_PORT, POWER_CTL_PIN)	    //5V电源控制 
#define POWER_CTL_OFF                                       gpio_bit_reset(POWER_CTL_PORT, POWER_CTL_PIN)	 
	

#define RELAY_L_SYNNCEHIAN_PORT                             GPIOF
#define RELAY_L_SYNNCEHIAN_PIN                              GPIO_PIN_6
#define RELAY_L_SYNNCEHIAN_ON                               gpio_bit_set(RELAY_L_SYNNCEHIAN_PORT, RELAY_L_SYNNCEHIAN_PIN)	// 继电器L相粘连使能
#define RELAY_L_SYNNCEHIAN_OFF                              gpio_bit_reset(RELAY_L_SYNNCEHIAN_PORT, RELAY_L_SYNNCEHIAN_PIN)	


#define RELAY_N_SYNNCEHIAN_PORT                             GPIOF
#define RELAY_N_SYNNCEHIAN_PIN                              GPIO_PIN_7
#define RELAY_N_SYNNCEHIAN_ON                               gpio_bit_set(RELAY_N_SYNNCEHIAN_PORT, RELAY_N_SYNNCEHIAN_PIN)	// 继电器N相粘连使能
#define RELAY_N_SYNNCEHIAN_OFF                              gpio_bit_reset(RELAY_N_SYNNCEHIAN_PORT, RELAY_N_SYNNCEHIAN_PIN)	

#define CP_PWM_OUTPUT_PIN				                    GPIO_PIN_8                                                      //CP PWM输出引脚
#define CP_PWM_OUTPUT_PORT		                            GPIOA

#define Out_Short_DET_PORT		                            GPIOB							//短路检测引脚
#define Out_Short_DET_PIN		                            GPIO_PIN_9				
#define Out_Short_DET_ON		                            gpio_bit_set(Out_Short_DET_PORT, Out_Short_DET_PIN)	//短路检测引脚使能
#define Out_Short_DET_OFF		                            gpio_bit_reset(Out_Short_DET_PORT, Out_Short_DET_PIN)	//短路检测引脚禁止使能		


//IO口状态
typedef enum 
{
	GPIO_STATUS_RESET = 0,
	GPIO_STATUS_SET
}_e_gpio_status;

//IO输出定义
typedef enum 
{
	OUTPUT_IO_L_RELAY_OPEN = 0,
	OUTPUT_IO_N_RELAY_OPEN,
	OUTPUT_IO_L_RELAY_COLSE,
	OUTPUT_IO_N_RELAY_COLSE,
	OUTPUT_IO_LCD_POWER_CTRL,
	OUTPUT_IO_POWER_CTRL,
	OUTPUT_IO_L_SYNNCEHIAN_CTRL,
	OUTPUT_IO_N_SYNNCEHIAN_CTRL,
	OUTPUT_IO_MAX
}_e_output_io_digital_typedef;



//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
void drv_common_usart0_init(INT32U baud_rate);
// void drv_common_usart1_init(INT32U baud_rate);
void drv_common_usart0_send(INT8U *p_buff, INT16U data_len);
// void drv_common_usart1_send(INT8U *p_buff, INT16U data_len);
// void drv_common_adc1_init(void);
FP32 drv_common_cp_vol_get(void);
FP32 drv_common_earth_vol_get(void);
FP32 drv_common_temperature_vol_get(void);
void drv_common_gpio_init(void);
void drv_common_set_gpio_status(_e_output_io_digital_typedef output_gpio, _e_gpio_status gpio_status);
void drv_common_toggle_gpio_status(_e_output_io_digital_typedef output_gpio);
void drv_common_timer_init(void);
void drv_common_set_cp_pwm(INT16U maxcurr);

#if 0
#if((CUSTOMER_BOARD_SELECT ==JYC_GD_A10L)||(CUSTOMER_BOARD_SELECT ==JYC_GD_A02))
void drv_common_gpio_init_mcu_led_stop_led(void);
#endif
#endif

void WriteFlashTest(INT32U L,INT32U Data[],INT32U addr);

INT16U adc_channel_sample(INT8U channel);
void usart_send_string(const char *str);

#endif

