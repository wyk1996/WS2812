#include "app.h"
#include "bsp_led.h"
#include "bsp_flash.h"
#include "bsp_oled.h"
#include "app_realcheck.h"
#include "key_touch_process.h"
#include "user_cfg.h"
#include "hw_api_usart.h"
#include "drv_common.h"
#include "app_chargectl.h"
#include "app_meter.h"
#include "bsp_meter_v9260.h"
#include "app_mainctl.h"
#include "gd32e23x_it.h"
#include "systick.h"
#include <stdio.h>
#include "gd32e230c_eval.h"
#include "app_target.h"





app_para_t app_para;




void App_Init(void)
{
    systick_config();

    Uart1_Init();
    Meter_V9260_Port_Init();

    #if 1
    Key_Touch_Init();
    drv_common_gpio_init();
    #endif

    Adc_Init();

    TimeOutDet_Init();
   
    hw_api_usart_data_init(UART_SN_0);
    drv_common_usart0_init(9600);
    

    Realcheck_Init();

    ws2812_init();

    #if 0
    TimerPwm_Config();
    drv_common_set_cp_pwm(0xFFFF);	
    #endif	
    
    
    Timer5_Base();
    
	
   
    
    

   

    #if 1
    rcu_config_SPI();
    gpio_config_SPI();
    spi_config();
    spi_send_to_qg2864_init();
    #endif




    #if 1
    V9260_InitDefaultPara();
    UpdateRaccoonPara();
    app_target_sys_para_init();
    #endif

   
    
    
    

    #if 0
    printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));
    #endif


    Fwdgt_Config();
    memset(&app_para, 0, sizeof(app_para_t));
    
    

    

   
}




void App_Handler(void)
{
    Fwdgt_Free();
    

    Oled_Handler(); 
    Led_Handler();


    #if 1
    Key_Touch_Handler();
    Realcheck_Handler();
    app_charge_ctl_process();
    app_mainctl_process();
    Meter_parameter_update();   
    #endif

}






void TIMER5_IRQHandler(void)
{ 
    static uint8_t relay_pwm_cnt = 0;
    static uint8_t cp_cnt = 0;
    static uint16_t relay_cnt = 0;
    //10us进一次中断
    if(SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);

        //1ms
        if(++app_para.timer_cnt > 100){
            app_para.timer_cnt = 0;
            TimeOutDet_DecHandle();

          

            if(app_para.flag_vlaue){
                if(++relay_cnt > 5000){
                    relay_cnt = 0;
                    app_para.flag_vlaue = 0;
                    app_para.relay_open_flag = 1;
                    #if 1
                    printf("555555\r\n");
                    #endif
                }
            }
        }

        //占空比27%
        if(app_para.cp_flag){
            if (++cp_cnt >= 100) cp_cnt = 0;

            if (cp_cnt < 27) {
                gpio_bit_set(GPIOA, GPIO_PIN_8);    // 高电平 27*10us=270us
            } else {
                gpio_bit_reset(GPIOA, GPIO_PIN_8);  // 低电平 73*10us=730us
            }
        } 
        
        if(app_para.flag_vlaue){
            gpio_bit_set(RELAY_N_CTRL_GPIO_PORT, RELAY_N_CTRL_PIN);
            gpio_bit_set(RELAY_L_CTRL_GPIO_PORT, RELAY_L_CTRL_PIN);  
        }

        

        #if 1
        if(app_para.relay_open_flag == 1)
        {
            relay_pwm_cnt++;
            if(relay_pwm_cnt < 3){
                gpio_bit_set(RELAY_N_CTRL_GPIO_PORT, RELAY_N_CTRL_PIN);
                gpio_bit_set(RELAY_L_CTRL_GPIO_PORT, RELAY_L_CTRL_PIN);     
            } else if(relay_pwm_cnt < 5){
                gpio_bit_reset(RELAY_N_CTRL_GPIO_PORT, RELAY_N_CTRL_PIN);
                gpio_bit_reset(RELAY_L_CTRL_GPIO_PORT, RELAY_L_CTRL_PIN);
            } else{
                relay_pwm_cnt = 0;
                gpio_bit_set(RELAY_N_CTRL_GPIO_PORT, RELAY_N_CTRL_PIN);
                gpio_bit_set(RELAY_L_CTRL_GPIO_PORT, RELAY_L_CTRL_PIN);  
            }
        } else{
            
        }
        #endif
        
       
    }
}




