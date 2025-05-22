#ifndef __UART_DECODE_H_
#define __UART_DECODE_H_

// define decode num
#define UART_DEV_NUM                    2
// uart communication 
#define UART_SIZE                       7
#define UART_START_FILED                0XA5
#define UART_END_FILED                  0XFB

#define UART_CS_EN

typedef struct {
    union {
        unsigned char buf[UART_SIZE];
        struct {
            unsigned char start;
            unsigned char dev_id;
            unsigned char cmd_type;
            unsigned int cmd;
            unsigned char cs;
            unsigned char stop;
        } filed;
    } rev;
    unsigned char len;
} uart_para_t;


extern uart_para_t uart_para[UART_DEV_NUM]; 

void Uart_Decode_Init(void);

unsigned char Calc_CheckSum(unsigned char *buf, unsigned char len);

unsigned char Uart_Decode_Rev(unsigned char dev, unsigned char rev_data);

#endif

