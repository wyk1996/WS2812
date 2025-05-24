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

    TimeOutDet_Init();
    

    Uart1_Init();
    Adc_Init();
    drv_common_usart0_init(115200);
    hw_api_usart_data_init(UART_SN_0);
    Timer5_Base();
    TimerPwm_Config();
    ws2812_init();
    
    #if 1
    drv_common_set_cp_pwm(0xFFFF);	
    #endif		

    #if 0
    Meter_V9260_Init();
    Key_Touch_Init();
    drv_common_gpio_init();
    spi_send_to_qg2864_init();
    Realcheck_Init();

    #endif

    #if 0
    Meter_V9260_Port_Init();
    V9260_InitDefaultPara();
    UpdateRaccoonPara();
    #endif
    
   
    #if 0
    rcu_config_SPI();
    gpio_config_SPI();
    spi_config();
    #endif
   
    app_target_sys_para_init();
   


    #if 1
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


    #if 0
    Meter_parameter_update();
    app_charge_ctl_process();
    app_mainctl_process();
    Realcheck_Handler();
    Key_Touch_Handler();
    Oled_Handler();
    #endif



    Led_Handler();
    

    
    

    
    

 
    

   
  
    
   

   
}





/*!
    \brief      this function handles TIMER5 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER5_IRQHandler(void)
{ 
    //20us进一次中断
    if(SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);

        //1ms
        if(++app_para.timer_cnt > 50){
            app_para.timer_cnt = 0;
            TimeOutDet_DecHandle();
        }
        

        #if 1
        if(app_para.relay_open_flag == 1)
        {
            drv_common_toggle_gpio_status(OUTPUT_IO_L_RELAY_OPEN);
        } 
       
        #endif
        
       
    }
}



