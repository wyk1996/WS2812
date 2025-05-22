/*!
    \file    systick.c
    \brief   the systick configuration file
    
    \version 2024-02-22, V2.1.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/


#include "systick.h"
#include <stdint.h>

volatile static uint32_t delay;
volatile static float count_1us = 0;
volatile static float count_1ms = 0;

static volatile uint64_t s64_system_ticks = 0;
static uint64_t delay_temp=0;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    #if 1
    /* setup systick timer for 1000Hz interrupts */
    if(SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while(1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
    #else
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);
    count_1us = (float)SystemCoreClock/8000000;
    count_1ms = (float)count_1us * 1000;
    #endif


}


void delay_1us(uint32_t count)
{
    uint32_t ctl;
    
    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1us);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do{
        ctl = SysTick->CTRL;
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    #if 1
    delay = count;

    while(0U != delay){
    }
    

    #else
    uint32_t ctl;
    
    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1ms);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do{
        ctl = SysTick->CTRL;
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    #endif
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if(0U != delay){
        delay--;
    }
}


/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_ms(uint32_t count)
{
	delay_temp = s64_system_ticks + count;
	
    while(delay_temp > s64_system_ticks){
		
    }
}

//获取系统tick秒数
uint64_t get_os_ticks_s(void)
{
	return u64_systick_get()/1000;
}

//获取系统tick毫秒数
uint64_t get_os_ticks_ms(void)
{
	return u64_systick_get();
}


//获取当前计数
uint64_t u64_systick_get(void)
{
	return s64_system_ticks;
}


//计数递加
void delay_increment(void)
{
    s64_system_ticks ++;
}


