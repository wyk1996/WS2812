#include <stddef.h>
#include <string.h>
#include "uart_decode.h"

uart_para_t uart_para[UART_DEV_NUM]; 

void Uart_Decode_Init(void)
{
    memset((void*)&uart_para[0], 0, sizeof(uart_para_t)*UART_DEV_NUM);
}

unsigned char Calc_CheckSum(unsigned char *buf, unsigned char len)
{
    unsigned char cs = 0, i;
    for (i = 0; i < len; i++) {
        cs += *buf++;
    }
    return cs;
}

unsigned char Uart_Decode_Rev(unsigned char dev, unsigned char rev_data)
{
    uart_para[dev].rev.buf[uart_para[dev].len++] = rev_data;
    
    if (UART_START_FILED != uart_para[dev].rev.buf[0]) {
        uart_para[dev].len = 0; 
        return 0;
    }
    
    if (UART_SIZE <= uart_para[dev].len) {
        if (UART_END_FILED != uart_para[dev].rev.buf[UART_SIZE-1]) {
            uart_para[dev].len = 0; 
            return 0;
        }
#ifdef UART_CS_EN
        if (Calc_CheckSum(&uart_para[dev].rev.buf[0], 5) != uart_para[dev].rev.filed.cs) {
            uart_para[dev].len = 0;
            return 0;
        }
#endif
        uart_para[dev].len = 0;
        return 1;
    }
    return 0;
}

