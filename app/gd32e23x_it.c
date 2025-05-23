/*!
    \file    gd32e23x_it.c
    \brief   interrupt service routines

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

#include "gd32e23x_it.h"
#include "main.h"
#include "systick.h"
#include "app.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
//void NMI_Handler(void)
//{
//    /* if NMI exception occurs, go to infinite loop */
//    while(1) {
//    }
//}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void SVC_Handler(void)
// {
//     /* if SVC exception occurs, go to infinite loop */
//     while(1) {
//     }
// }

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void PendSV_Handler(void)
// {
//     /* if PendSV exception occurs, go to infinite loop */
//     while(1) {
//     }
// }

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    #if 1
    delay_decrement();
    delay_increment();
    #endif
}







/*!
    \brief      this function handles DMA channel1 and channel2 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA_Channel1_2_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA_CH1, DMA_INT_FLAG_FTF)){
        dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
        #if 0
        g_transfer_complete = SET;
        #endif
    }
    if(RESET != dma_interrupt_flag_get(DMA_CH2, DMA_INT_FLAG_FTF)){
        dma_interrupt_flag_clear(DMA_CH2, DMA_INT_FLAG_G);
        #if 0
        g_transfer_complete = SET;
        #endif
    }
}










