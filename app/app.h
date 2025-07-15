#ifndef _APP_H_
#define _APP_H_

#include "gd32e23x.h"
#include "TimeOut.h"

#define POLL_TIME                      150


void App_Init(void);
void App_Handler(void);

typedef struct
{
    uint8_t relay_open_flag;	//继电器开标志位
    uint8_t flag_vlaue;
    uint8_t timer_cnt;
    uint8_t cp_flag;
    uint32_t oled_ulavlue;
    struct {
        TIMEOUT_PARA poll;
        TIMEOUT_PARA relay_poll;
        TIMEOUT_PARA poll_meter;
    } tout;

} app_para_t;

extern app_para_t app_para;



#endif

