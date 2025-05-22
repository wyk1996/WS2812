
#ifndef __HW_API_USART_H__
#define __HW_API_USART_H__


#include "gd32e23x.h"
#include "systick.h"
#include "Includes.h"
#include "IndependentDataType.h"

#define UART0_RX_BUFF_MAX   128
#define UART1_RX_BUFF_MAX   128

//���??���
typedef enum
{
	UART_SN_0 = 0,
	UART_SN_1,
	UART_SN_MAX
}_e_uart_sn;

void hw_api_uart_clear_rcv_data(_e_uart_sn uart_sn);
BOOLEAN hw_api_uart_check_seqeue_empty(_e_uart_sn uart_sn);
INT8U hw_api_uart_dequeue(_e_uart_sn uart_sn);
INT8U hw_api_uart_get_recv_data_len(_e_uart_sn uart_sn);
void hw_api_usart_data_init(_e_uart_sn uart_sn);

#endif

