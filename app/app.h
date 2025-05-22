#ifndef _APP_H_
#define _APP_H_

#include "gd32e23x.h"





void App_Init(void);
void App_Handler(void);

typedef struct
{
    uint8_t relay_open_flag;	//继电器开标志位
    uint8_t flash_vlaue;

} app_para_t;

extern app_para_t app_para;



#endif

