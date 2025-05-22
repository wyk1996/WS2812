#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e23x.h"



//LED PWM输出引脚
#define LED_PWM_OUTPUT_PORT		                    GPIOA
#define LED_PWM_OUTPUT_PIN				            GPIO_PIN_11      



/*建立一个定义单个LED三原色值大小的结构体*/
typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_Color_TypeDef;


typedef enum{
    RED_COLOR = 0,
    GREEN_COLOR,
    BLUE_COLOR,
    SKY_COLOR,
    MAGENTA_COLOR,

    YELLOW_COLOR,
    BLACK_COLOR,
    WHITE_COLOR,

    OEANGE_COLOR,
    Max_COLOR
}rgb_color;





void Led_Handler(void);
/* 函数声明 */
void ws2812_init(void);









void ws2812_display_color(uint16_t color);//显示颜色
void ws2812_display_blink_light(uint16_t blink_num, uint16_t color,uint32_t time_out);//闪烁灯模式设置
void ws2812_display_breathe_light(uint16_t color,uint32_t time_out);//呼吸灯模式设置









#ifdef __cplusplus
}
#endif

#endif



// end of file






