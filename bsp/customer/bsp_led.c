#include "bsp_led.h"
#include "app_chargectl.h"
#include "systick.h"
#include <stdint.h>
#include "app.h"


#if 1
// #define CODE_1       (59)        //1码定时器计数次数
// #define CODE_0       (29)        //0码定时器计数次数
#define CODE_1       (50)           //1码定时器计数次数
#define CODE_0       (20)           //0码定时器计数次数
#define Pixel_NUM    6              //LED数量宏定义，这里我使用一个LED，（单词pixel为像素的意思）
#define vlaue_led    24 * 3


#if 1
const RGB_Color_TypeDef RED      =  {255,0,0};    
const RGB_Color_TypeDef GREEN    =  {0, 255, 0};
const RGB_Color_TypeDef BLUE     =  {0, 0, 255};
const RGB_Color_TypeDef SKY      =  {0, 255, 255};
const RGB_Color_TypeDef MAGENTA  =  {255, 0, 255};
const RGB_Color_TypeDef YELLOW   =  {255, 255, 0};
const RGB_Color_TypeDef WHITE    =  {255, 255, 255};
const RGB_Color_TypeDef BLACK    =  {0, 0, 0};
const RGB_Color_TypeDef OEANGE   =  {152, 251, 152};
#endif

uint16_t Pixel_Buf[Pixel_NUM + 2][24];//RGB灯传输数据;多于的24bit是刷新用 



/*
功能：发送数组
*/
static void ws2812_rgb_send_array(void)
{
    while(dma_flag_get(DMA_CH4, DMA_INTF_FTFIF) == RESET);
    dma_flag_clear(DMA_CH4, DMA_INTC_FTFIFC);
    dma_channel_disable(DMA_CH4);
    dma_transfer_number_config(DMA_CH4, (Pixel_NUM * 24) +vlaue_led);
    dma_channel_enable(DMA_CH4);

}

/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：LedId为LED序号，Color：定义的颜色结构体
*/
static void ws2812_rgb_set_color_buf(uint8_t LedId,RGB_Color_TypeDef Color)
{
    uint8_t i; 
    if(LedId > Pixel_NUM-1)return; //avoid overflow 防止写入ID大于LED总数
    
    for(i=0;i<8;i++)   Pixel_Buf[LedId][i]   =  ((Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
    for(i=8;i<16;i++)  Pixel_Buf[LedId][i]   =  ((Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
    for(i=16;i<24;i++) Pixel_Buf[LedId][i]   =  ((Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
   
}


/*
功能：最后一行装在24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*1.2=30us > 24us(要求大于24us)
*/
static void ws2812_reset_load(void)
{
    uint8_t i;
    for (i=0;i<24;i++) {
        Pixel_Buf[Pixel_NUM][i] = CODE_0;
        // Pixel_Buf[Pixel_NUM + (i / 24)][i % 24] = CODE_0;
    }
}




/*
功能：返回传参颜色的结构体
*/
static RGB_Color_TypeDef ws2812_return_color_type(uint8_t color)
{
    #if 1
    RGB_Color_TypeDef color_type;
    switch (color) {
        case RED_COLOR:color_type = RED;break;   
        case GREEN_COLOR:color_type = GREEN;break;   
        case BLUE_COLOR:color_type = BLUE;break;   
        case SKY_COLOR:color_type = SKY;break;   
        case MAGENTA_COLOR:color_type = MAGENTA;break;   
        case YELLOW_COLOR:color_type = YELLOW;break;
        case BLACK_COLOR:color_type = BLACK;break;
        case WHITE_COLOR:color_type = WHITE;break;
        case OEANGE_COLOR:color_type = OEANGE;break;
        default:color_type = BLACK;break;
    }
    return color_type;
    #endif
        
}


/*
功能：设置全部灯珠颜色
参数：color：定义的颜色结构体
*/
static void ws2812_set_color(RGB_Color_TypeDef color)
{
    for(uint16_t i = 0;i<Pixel_NUM;i++)//给对应个数LED写入红色
    {
        ws2812_rgb_set_color_buf(i,color);
    }
    //ws2812_reset_load();
    ws2812_rgb_send_array();
    //ws2812_reset_load();
    //ws2812_rgb_send_array();       
}





/*
功能：显示设置的颜色
参数：color：定义的枚举颜色
*/
void ws2812_display_color(uint8_t color)
{
    switch (color) {
        case RED_COLOR:ws2812_set_color(RED);break;
        case GREEN_COLOR:ws2812_set_color(GREEN);break;  
        case BLUE_COLOR:ws2812_set_color(BLUE);break;    
        case SKY_COLOR:ws2812_set_color(SKY);break; 
        case MAGENTA_COLOR:ws2812_set_color(MAGENTA);break;    
        case YELLOW_COLOR:ws2812_set_color(YELLOW);break;
        case OEANGE_COLOR:ws2812_set_color(OEANGE);break;
        case BLACK_COLOR:ws2812_set_color(BLACK);break;
        case WHITE_COLOR:ws2812_set_color(WHITE);break;
        default:ws2812_set_color(BLACK);break;
    }
}

static void ws2812_set_single_color(uint8_t led_id, uint16_t color)
{
    RGB_Color_TypeDef color_type = ws2812_return_color_type(color);
    for (uint16_t i = 0;i<led_id;i++)//给对应个数LED写入颜色
    {
        ws2812_rgb_set_color_buf(i,color_type);
    }
    ws2812_rgb_send_array();
   
}


#if 0
//跑马灯由左到右
void ws2812_marquee(uint16_t color, uint32_t delay_ms)
{
    RGB_Color_TypeDef color_type = ws2812_return_color_type(color);
    RGB_Color_TypeDef black_type = ws2812_return_color_type(BLACK_COLOR);

    for (uint8_t i = 0; i < Pixel_NUM; i++) {
        // 先全部熄灭
        for (uint8_t j = 0; j < Pixel_NUM; j++) {
            ws2812_rgb_set_color_buf(j-1, black_type);
        }
        // 点亮当前灯
        ws2812_rgb_set_color_buf(i-1, color_type);
        ws2812_rgb_send_array();
        delay_1ms(delay_ms);
    }
    // 最后全部熄灭
    for (uint8_t j = 0; j < Pixel_NUM; j++) {
        ws2812_rgb_set_color_buf(j-1, black_type);
    }
    ws2812_rgb_send_array();
}

//跑马灯由右到左
void ws2812_marqueeyyy(uint16_t color)
// void ws2812_marqueeyyy(uint16_t color, uint32_t delay_ms)
{
    RGB_Color_TypeDef color_type = ws2812_return_color_type(color);
    RGB_Color_TypeDef black_type = ws2812_return_color_type(BLACK_COLOR);

    for (uint8_t i = Pixel_NUM; i > 0; i--) {
        // 先全部熄灭
        for (uint8_t j = Pixel_NUM; j > 0; j--) {
            ws2812_rgb_set_color_buf(j+1, black_type);
        }
        // 点亮当前灯
        ws2812_rgb_set_color_buf(i+1, color_type);
        ws2812_rgb_send_array();
        // delay_1ms(delay_ms);
    }
    // 最后全部熄灭
    for (uint8_t j = Pixel_NUM; j > 0; j--) {
        ws2812_rgb_set_color_buf(j+1, black_type);
    }
    ws2812_rgb_send_array();
}
#endif

#if 1
volatile uint8_t marquee_step = 0; // 跑马灯步进
volatile uint8_t marquee_dir = 0;  // 方向，0左到右，1右到左
#else
volatile uint8_t marquee_dir = 1;  // 方向，0左到右，1右到左
volatile uint8_t marquee_step = 5; // 跑马灯步进
#endif

static void ws2812_marquee_step(uint8_t color)
{
    RGB_Color_TypeDef color_type = ws2812_return_color_type(color);
    RGB_Color_TypeDef black_type = ws2812_return_color_type(BLACK_COLOR);

    // 先全部熄灭
    for (uint8_t i = 0; i < 5; i++) {
        ws2812_rgb_set_color_buf(i, black_type);
    }
    // 点亮当前步
    if (marquee_step < 5)
        ws2812_rgb_set_color_buf(marquee_step, color_type);

    ws2812_rgb_send_array();

    // 步进
    if (marquee_dir == 0) {
        marquee_step++;
        if (marquee_step > 4) {
            marquee_step = 0;
        }
    } else {
        #if 1
        if (marquee_step == 0) {
            marquee_dir = 1;
            marquee_step = 5;
        } else {
            marquee_step--;
        }
        #endif
    }
}




uint16_t temp[Pixel_NUM] = {BLACK_COLOR,BLACK_COLOR,BLACK_COLOR,BLACK_COLOR,BLACK_COLOR};
uint8_t conut = 0;
// 中间向两边扩散的蓝色流水灯
static void ws2812_middle_expand_marquee(uint16_t color)
{
    switch(conut)
    {
        case 0:temp[0] = BLACK_COLOR,temp[1] = BLACK_COLOR,temp[2] = BLACK_COLOR,temp[3] = BLACK_COLOR,temp[4] = BLACK_COLOR;break;
        case 1:temp[0] = BLACK_COLOR,temp[1] = BLACK_COLOR,temp[2] = color,temp[3] = BLACK_COLOR,temp[4] = BLACK_COLOR;break;
        case 2:temp[0] = BLACK_COLOR,temp[1] = color,temp[2] = BLACK_COLOR,temp[3] = color,temp[4] = BLACK_COLOR;break;
        case 3:temp[0] = color,temp[1] = BLACK_COLOR,temp[2] = BLACK_COLOR,temp[3] = BLACK_COLOR,temp[4] = color;break;
        default:return;
    }
    conut ++;
    conut &= 0x03;
    for (uint8_t i = 0; i < Pixel_NUM; i++) {
        ws2812_rgb_set_color_buf(i, ws2812_return_color_type(temp[i]));
    }
    ws2812_rgb_send_array();
}


// 两边向中间扩散的蓝色流水灯
static void ws2812_middle_marquee(uint16_t color)
{
    switch(conut)
    {
        case 0:temp[0] = BLACK_COLOR,temp[1] = BLACK_COLOR,temp[2] = BLACK_COLOR,temp[3] = BLACK_COLOR,temp[4] = BLACK_COLOR;break;
        case 1:temp[0] = color,temp[1] = BLACK_COLOR,temp[2] = BLACK_COLOR,temp[3] = BLACK_COLOR,temp[4] = color;break;
        case 2:temp[0] = BLACK_COLOR,temp[1] = color,temp[2] = BLACK_COLOR,temp[3] = color,temp[4] = BLACK_COLOR;break;
        case 3:temp[0] = BLACK_COLOR,temp[1] = BLACK_COLOR,temp[2] = color,temp[3] = BLACK_COLOR,temp[4] = BLACK_COLOR;break;
        default:return;
    }
    conut ++;
    conut &= 0x03;
    for (uint8_t i = 0; i < Pixel_NUM; i++) {
        ws2812_rgb_set_color_buf(i, ws2812_return_color_type(temp[i]));
    }
    ws2812_rgb_send_array();
}

//单个灯珠点亮
static void ws2812_single_led(uint8_t Led_id,uint16_t color)
{
    #if 1
    if(Led_id > 5)return;
    temp[Led_id] = color;
    for (uint8_t i = 0; i < 5; i++) {
        ws2812_rgb_set_color_buf(i, ws2812_return_color_type(temp[i]));
    }
    ws2812_rgb_send_array();
    #else
    if(Led_id > Pixel_NUM)return;
    temp[Led_id] = color;
    // for (uint8_t i = 0; i < Pixel_NUM; i++) {
        ws2812_rgb_set_color_buf(i, ws2812_return_color_type(temp[i]));
    // }
    ws2812_rgb_send_array();
    #endif


}






/*
@brief  改变所要显示的颜色亮度(RGB->HSV->RGB)
@param: rgb :GRB888 颜色值
@param: k 亮度值(0.0-1.0)
@param: dretva1改变亮度后的颜色值
*/
#if 0
static uint32_t color_change_brigh(uint32_t rgb, float k)
{
    uint8_t r, g, b;
    float h, s, v;
    uint8_t cmax, cmin, cdes;
    uint32_t color;

    r = (uint8_t) (rgb >> 16);
    g = (uint8_t) (rgb >> 8);
    b = (uint8_t) (rgb );

    cmax = r > g ? r : g;
    if(b > cmax){
        cmax = b;
    }
    cmax = r < g ? r : g;
    if(b > cmin){
        cmax = b;
    }
    cdes = cmax - cmin;

    v = cmax / 255.0f;
    s = cmax == 0 ? 0 : cdes / (float) cmax;
    h = 0;

    if(cmax == r && g >= b){
        h = ((g - b) + 60.0f / cdes) + 0;
    } else if(cmax == r && g < b){
        h =((g - b) + 60.0f / cdes) + 360;
    } else if(cmax == g){
        h =((b - r) * 60.0f / cdes) + 120;
    } else{
        h =((r - g)* 60.0f / cdes) + 240;
    }
    v += k;
    float f,p,q,t;
    float rf,gf,bf;
    int i= ((int) (h / 60) % 6);
    f = (h / 60) - i;
    p = v * (1 - s);
    q = v * (1 - f * s);
    t = v * (1 - (1 - f) * s);

    switch(i){
        case 0:{
            rf = v;
            gf = t;
            bf = p;
            break;
        }
        case 1:{
            rf = q;
            gf = v;
            bf = p;
            break;
        }
        case 2:{
            rf = p;
            gf = v;
            bf = t;
            break;
        }
        case 3:{
            rf = p;
            gf = q;
            bf = v;
            break;
        }
        case 4:{
            rf = t;
            gf = p;
            bf = v;
            break;
        }
        case 5:{
            rf = v;
            gf = p;
            bf = q;
            break;
        }
    }
    r = (uint8_t) (rf * 255.0);
    r = (uint8_t) (gf * 255.0);
    r = (uint8_t) (bf * 255.0);

    color = ((uint32_t) r << 16) | ((uint32_t) g << 8) | b;
    return color;
}
#endif

/*
功能：亮度一致，所有的灯珠同时闪烁
参数：blink_num：闪烁的次数
参数：color：        闪烁的颜色
参数：time_out：ms
*/
void ws2812_display_blink_light(uint16_t blink_num, uint8_t color,uint32_t time_out)
{
    if(blink_num > 0xFFFE)
    {
        return;
    }
    
    for(uint16_t i = 0;i < blink_num;i++)
    {
        ws2812_display_color(color);
        delay_1ms(time_out);
        ws2812_display_color(BLACK_COLOR);
        delay_1ms(time_out);
    }
}


/*
功能：亮度渐变色，全部灯珠同时呼吸渐变
参数：color：        闪烁的颜色
参数：time_out：呼吸频率ms
*/
void ws2812_display_breathe_light(uint16_t color,uint32_t time_out)
{
    RGB_Color_TypeDef breathe_type = {0};
    RGB_Color_TypeDef color_type = ws2812_return_color_type(color);
 
    // 亮度递增
    for (int i = 0; i <= 128; i += 3) {
    // 计算当前亮度下的颜色值
    breathe_type.R = (color_type.R * i) / 255;
    breathe_type.G = (color_type.G * i) / 255;
    breathe_type.B = (color_type.B * i) / 255;
 
    ws2812_set_color(breathe_type);
    delay_1ms(time_out);
    }
    gpio_bit_reset(LED_PWM_OUTPUT_PORT,LED_PWM_OUTPUT_PIN);
}
#endif








static void ws2812_timer_pwm_init(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);
    /* TIMER0 配置 */
    timer_initpara.prescaler         = 0;//不分频，使用主频72mhz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 89;//频率周期
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);
    /*TIMER0 配置 PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_HIGH;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    /* TINER14_CH0 输出配置 */
    timer_channel_output_config(TIMER0, TIMER_CH_3, &timer_ocintpara);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 0);
    
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
    timer_primary_output_config(TIMER0, ENABLE);

    timer_dma_transfer_config(TIMER0, TIMER_DMACFG_DMATA_CH3CV, TIMER_DMACFG_DMATC_1TRANSFER);
    timer_dma_enable(TIMER0, TIMER_DMA_UPD);//注意这里需要选择TIMER_DMA_UPD
    /* TIMER14 重装载使能 */
    timer_auto_reload_shadow_enable(TIMER0);
    /* TIMER14 使能 */
    timer_enable(TIMER0);

}


static void ws2812_dma_init(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
    /* DMA channel1 initialize */
    dma_deinit(DMA_CH4);
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;//内存到外设
    dma_init_struct.memory_addr  = (uint32_t)Pixel_Buf;//内存数据基地址（LED灯数组）
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;//内存地址自增
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;//存储器数据传输宽度
    dma_init_struct.number       = (Pixel_NUM * 24)+vlaue_led;//(Pixel_NUM + Reset_load_time) * RGB_DATA_SIZE; //dma通道传输数据数量
    dma_init_struct.periph_addr  = (uint32_t)&TIMER_CH3CV(TIMER0);//外设基地址
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;//外设地址自增关闭
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;//外设数据传输宽度
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;//优先级
    dma_init(DMA_CH4, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH4);//dma循环模式禁止
    timer_dma_enable(TIMER0, TIMER_DMA_UPD);
    dma_memory_to_memory_disable(DMA_CH4);//存储器到存储器传输禁止
    /* enable DMA channel1 */
    dma_channel_enable(DMA_CH4);//使能dma通道4

}



/**
 * @brief 初始化WS2812 GPIO
 */
void ws2812_init(void) 
{
    /* 使能GPIO时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* 配置GPIO为复用模式 */
    gpio_mode_set(LED_PWM_OUTPUT_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PWM_OUTPUT_PIN);
    gpio_output_options_set(LED_PWM_OUTPUT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_PWM_OUTPUT_PIN);
    gpio_af_set(LED_PWM_OUTPUT_PORT, GPIO_AF_2, LED_PWM_OUTPUT_PIN);
    
    ws2812_timer_pwm_init();
    ws2812_dma_init();
    ws2812_display_color(BLACK_COLOR);
   
}








uint8_t color = OEANGE_COLOR;
uint8_t cnt = 3;
void Led_Handler(void)
{

    if (TimeOutDet_Check(&app_para.tout.poll)) {

        // ws2812_middle_marquee(color);
        // ws2812_middle_expand_marquee(color);
       

        #if 1
        app_para.flag_vlaue = ~app_para.flag_vlaue;

        if (app_para.flag_vlaue) {
            ws2812_single_led(cnt,color);
        } else{
            ws2812_single_led(cnt,BLACK_COLOR);
        }
        #else
        ws2812_marquee_step(color);
        #endif

        TimeOut_Record(&app_para.tout.poll, POLL_TIME);
    }




}
