#ifndef _APP_H_
#define _APP_H_

#include "gd32e23x.h"
#include "TimeOut.h"

#define POLL_TIME                      500


void App_Init(void);
void App_Handler(void);

typedef struct
{
    uint8_t relay_open_flag;	//继电器开标志位
    uint8_t flag_vlaue;
    uint8_t timer_cnt;
    struct {
        TIMEOUT_PARA poll;
    } tout;

} app_para_t;

extern app_para_t app_para;



#endif

