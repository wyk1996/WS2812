#include "key_touch_process.h"
#include "systick.h"
#include <stdio.h>
#include "customer_config.h"
#include "customer_device_type.h"


#define KEY_TOUCH_PORT    GPIOB
#define KEY_TOUCH_PIN     GPIO_PIN_2

typedef struct
{
    uint8_t variaddr;                  //变量地址
    uint8_t (*Fun)(uint8_t value, uint8_t addr, uint8_t addr1);    //对应的处理函数
} KEY_ADDR_FRAME;

#if 0
static void Open_Handle(uint8_t value, uint8_t addr, uint8_t addr1)
{
 
}

static void Close_Handle(uint8_t value, uint8_t addr, uint8_t addr1)
{

}

static const KEY_ADDR_FRAME key_func[] = {
    {1, Open_Handle},
    {2, Close_Handle},
};
#endif




void Key_Touch_Init(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    
    /* configure KEY_TOUCH_PIN as input floating */
    gpio_mode_set(KEY_TOUCH_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_TOUCH_PIN);
    
    /* configure KEY_TOUCH_PIN as input pull-up */
    gpio_mode_set(KEY_TOUCH_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_TOUCH_PIN);
    gpio_output_options_set(KEY_TOUCH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, KEY_TOUCH_PIN);

}


void Key_Touch_Handler(void)
{
    static uint64_t key_touch_time = 0;
    static uint8_t key_touch_cnt = 0;
    // static uint8_t touch_cnt = 1;
    // static uint8_t touch_cnt1 = 1;
    // static uint8_t variaddr = 1;
    if(get_os_ticks_ms() - key_touch_time > 30){
        key_touch_time = get_os_ticks_ms();

        if(gpio_input_bit_get(KEY_TOUCH_PORT, KEY_TOUCH_PIN) == FALSE)
        {
            if(key_touch_cnt == FALSE){
                key_touch_cnt = TRUE;
             
               
            }

        } else{
        
            if(key_touch_cnt == TRUE){
                key_touch_cnt = FALSE;
    
                
            }
           
        }

    }
    
    #if 0
    for(uint8_t i = 0; i < (sizeof(key_func)/sizeof(KEY_ADDR_FRAME)); i++) {
        if (key_func[i].variaddr == touch_cnt1) {
            if (key_func[i].Fun) {
                key_func[i].Fun(key_touch_cnt, touch_cnt, touch_cnt1);
            }
            break;
        }
    }
    #endif
}


