#include "hw_api_usart.h"





typedef struct {
    volatile INT8U rx_byte;		//��������λ
    INT8U *rx_buff_ptr;     	//�����ݶ��д洢��ָ��
    INT16U front;           	//���ӱ��
    INT16U rear;            	//��ӱ��
    INT16U max_len;         	//������ݳ���
} _s_uart_rcv_buff;

static INT8U s_rx_buff0[UART0_RX_BUFF_MAX] = {0};
static INT8U s_rx_buff1[UART1_RX_BUFF_MAX] = {0};
static _s_uart_rcv_buff s_uart_rcv_buff[UART_SN_MAX];

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (INT8U)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}

//�����������
void hw_api_uart_clear_rcv_data(_e_uart_sn uart_sn)
{
    s_uart_rcv_buff[uart_sn].rx_byte = 0;
    s_uart_rcv_buff[uart_sn].front = 0;
    s_uart_rcv_buff[uart_sn].rear = 0;
}

//�жϻ����������Ƿ�Ϊ��
static BOOLEAN check_seqeue_empty(_s_uart_rcv_buff *sq)
{
    if(sq->front == sq->rear)
    {
        return 1;  
    }
    else
    {
        return 0;
    }
}

//�жϴ��ڻ����������Ƿ�Ϊ��
BOOLEAN hw_api_uart_check_seqeue_empty(_e_uart_sn uart_sn)
{
    return check_seqeue_empty(&s_uart_rcv_buff[uart_sn]);
}

//���������
static BOOLEAN enqueue(_s_uart_rcv_buff *sq, INT8U val)
{
    /* �ж϶����Ƿ�������������һ���洢�ռ䲻�� */
    if((sq->rear + 1)%sq->max_len == sq->front)
    {
        return ERROR;
    }
    sq->rx_buff_ptr[sq->rear] = val;
    sq->rear = (sq->rear+1) % sq->max_len;    //ѭ���洢
    return SUCCESS;
}

//ѭ���洢
static BOOLEAN uart_enqueue(INT8U val, _e_uart_sn uart_sn)
{
    if(ERROR == enqueue(&s_uart_rcv_buff[uart_sn],val))
    {
    	if(uart_sn!=UART_SN_0)
    	{
        	//log_e("uart:uart%d enq err!\n", uart_sn);
    	}
    }
    return 1;
}

//����������
static INT8U dequeue(_s_uart_rcv_buff *sq)
{
    INT8U val = 0;
    if( 1 == check_seqeue_empty(sq))
    {
        return ERROR;
    }
    val = sq->rx_buff_ptr[sq->front] ;
    sq->front = (sq->front+1) % sq->max_len;
    return val;
}

//���ڻ���������
INT8U hw_api_uart_dequeue(_e_uart_sn uart_sn)
{
    return dequeue(&s_uart_rcv_buff[uart_sn]);
}

//��ʼ�����ݻ�����
void hw_api_usart_data_init(_e_uart_sn uart_sn)
{
	s_uart_rcv_buff[uart_sn].rx_byte = 0;
    s_uart_rcv_buff[uart_sn].front = 0;
    s_uart_rcv_buff[uart_sn].rear = 0;
	switch(uart_sn)
	{
		case UART_SN_0:
			s_uart_rcv_buff[uart_sn].max_len = UART1_RX_BUFF_MAX;
			s_uart_rcv_buff[uart_sn].rx_buff_ptr = &s_rx_buff0[0];
			break;
		case UART_SN_1:
			s_uart_rcv_buff[uart_sn].max_len = UART1_RX_BUFF_MAX;
        	s_uart_rcv_buff[uart_sn].rx_buff_ptr = &s_rx_buff1[0];
			break;
		default:
			break;
	}
}

INT8U hw_api_uart_get_recv_data_len(_e_uart_sn uart_sn)
{
	if(s_uart_rcv_buff[uart_sn].front>s_uart_rcv_buff[uart_sn].rear)
	{
		return (s_uart_rcv_buff[uart_sn].rear+s_uart_rcv_buff[uart_sn].max_len-s_uart_rcv_buff[uart_sn].front);
	}
	else
	{
		return s_uart_rcv_buff[uart_sn].rear-s_uart_rcv_buff[uart_sn].front;
	}
}

//����0�жϷ�����
void USART0_IRQHandler(void)
{
	INT8U recv_temp=0;

	if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE_ORERR) != RESET)
	{
		recv_temp = (INT8U)usart_data_receive(USART0);
		uart_enqueue(recv_temp, UART_SN_0);
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE_ORERR);
		return;
	}
	
	if(usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE) != RESET)
	{
		recv_temp = (INT8U)usart_data_receive(USART0);
		uart_enqueue(recv_temp, UART_SN_0);
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);  
	}
}

#if 0
//����1�жϷ�����
void USART1_IRQHandler(void)
{
	INT8U recv_temp=0;
	
	if(usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE_ORERR) != RESET)
	{
		recv_temp = (INT8U)usart_data_receive(USART1);
		uart_enqueue(recv_temp, UART_SN_1);
		usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE_ORERR);
		return;
	}
	
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET)
	{
		recv_temp = (INT8U)usart_data_receive(USART1);
		uart_enqueue(recv_temp, UART_SN_1);
		usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
	}
}
#endif
