#include "bsp_meter_v9260.h"
#include "drv_common.h"
#include <stdio.h>
#include "Includes.h"
#include "IndependentDataType.h"




static _t_v9260s_info s_v9260s_info;

const uint32_t SysCtrl_Value = SysCtrl_ValueA;  //ϵͳ���ƼĴ���ֵ


#if 1
typedef struct {
    //����
    uint32_t uc_Physendtim;
    uint32_t uc_flapara;
    uint8_t uc_Powstat[16];
    uint8_t uc_Uartstat[8];
    //����
    uint32_t uc_Sendstat;
    uint32_t uc_Phystat;
    uint8_t ucStatus[16];               
    uint8_t ucSendLen[8];
    //��ѹ
    uint32_t ucSendPoint;
    uint32_t ucRevLen;
    uint8_t ucRevPoint[16];
    uint8_t ucBuf[8];
} S_RacCtrl;
 S_RacCtrl gs_RacCtrl;

typedef struct {
    uint32_t  RacUC;              //
    uint32_t  RacIAC;             //
    uint32_t  RacPAC;             //
    uint32_t  RacPADCC;           //             
    uint32_t  RacQADCC;           //
    uint32_t  RacIADCC;           //
} S_JBRE;

typedef struct {
    // uint16_t ui_Un;               // 
    // uint16_t ui_Ib;               // 
    // uint16_t ui_Resve1;           //
    // uint32_t  RacEGYTH;           //
    // uint32_t  RacREEGYTH;         //
    uint32_t  RacSysCtrl;         //
    uint32_t  RacBPFPARA;         //
    S_JBRE  gs_JBA;               //
    // uint32_t  RacMTPARA0;         //MTPARA0�Ĵ���
    uint32_t ul_PG;               //
    uint32_t ul_URmG;             //
    uint32_t ul_I1RmG;            //
    // uint16_t ui_Resve2;           //
    // uint16_t ui_JbCRC;            // 
} S_JBPM;
 S_JBPM gs_JbPm;                   //
#endif



//��ȡ������?���ṩΪ�����ļ�ʹ��
_t_v9260s_info* app_v9260s_get_meter_info(void)
{
	return &s_v9260s_info;
}



// ��������:����У���??
uint8_t CalculateChecksum(uint8_t *data, uint8_t length) 
{
    uint8_t sum = 0;
    for(uint8_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return ~sum + 0x33;
}



void Meter_V9260_Port_Init(void)
{
    //meter�ж�����
	gpio_mode_set(RCD_INT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, RCD_INT_PIN);
	gpio_output_options_set(RCD_INT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCD_INT_PIN);
}

void Meter_V9260_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_9);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

	gpio_bit_reset(GPIOA, GPIO_PIN_9);   
    delay_ms(70);
    gpio_bit_set(GPIOA, GPIO_PIN_9);
    delay_ms(50);
}


/*=========================================================================================\n
* @function_name: V9281_InitDefaultPara
* @function_file: Raccoon.c
* @����: ��ʼ��Ĭ�ϲ���
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void V9260_InitDefaultPara(void)
{
    gs_JbPm.RacSysCtrl = SysCtrl_Value;  //0x180 ������У�� ʱ��3.2MHz
    gs_JbPm.RacBPFPARA = BPFPARA_ValueDef;    //0x0125 ��ͨ�˲���ϵ�� 0x811D2BA7
    #if 1
    gs_JbPm.gs_JBA.RacIAC = 0xCCCCCCC;        //ȫ��������Чֵ�Ȳ�
    gs_JbPm.gs_JBA.RacPAC = 0x614240A9;         //ȫ���й����ʱȲ�
    gs_JbPm.gs_JBA.RacPADCC = 0x00000000;        //ȫ���й����ʶ��β���ֵ
    gs_JbPm.gs_JBA.RacQADCC = 0x00000000;      //ȫ���޹����ʶ��β���ֵ
    #endif
    gs_JbPm.ul_PG=1;               //���ʱ���ϵ��
    gs_JbPm.ul_URmG=965870;             //��ѹͨ������ϵ��
    gs_JbPm.ul_I1RmG=2737951;            //����ͨ��1����ϵ��
    gs_JbPm.gs_JBA.RacUC = 0x47C6945; //ȫ����ѹ��Чֵ�Ȳ� 

    

}

uint8_t UpdateRaccoonPara(void) 
{
    
    Meter_V9260_write(gs_JbPm.RacSysCtrl, 0x0180);
    delay_ms(50);    

    Meter_V9260_write(gs_JbPm.RacBPFPARA, 0x0107);
    delay_ms(50); 

    Meter_V9260_write(0x0, 0x0182);
    delay_ms(50); 

    Meter_V9260_write(AnaCtrl1_Val, 0x0183);
    delay_ms(50);

    Meter_V9260_write(gs_JbPm.gs_JBA.RacUC, 0x00FF);
    delay_ms(50);

    Meter_V9260_write(gs_JbPm.gs_JBA.RacIAC, IAC);
    delay_ms(50);
 
    Meter_V9260_write(gs_JbPm.gs_JBA.RacPAC, QAC);
    delay_ms(50);

  
    // if(!WriteWithRetry(gs_JbPm.gs_JBA.RacPADCC, RegAddr[5])) {
    //     success = FALSE;
    // }


    // if(!WriteWithRetry(gs_JbPm.gs_JBA.RacQADCC, RegAddr[6])) {
    //     success = FALSE;
    // }

}



uint8_t Meter_V9260_write(uint32_t Data, uint16_t Addr) 
{
    gs_RacCtrl.ucBuf[0] = 0x7D;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacWrite;
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr & 0x00ff);
    gs_RacCtrl.ucBuf[3] = (uint8_t)(Data & 0xFF);
    gs_RacCtrl.ucBuf[4] = (uint8_t)((Data >> 8) & 0xFF);
    gs_RacCtrl.ucBuf[5] = (uint8_t)((Data >> 16) & 0xFF);
    gs_RacCtrl.ucBuf[6] = (uint8_t)((Data >> 24) & 0xFF);
    gs_RacCtrl.ucBuf[7] =  CalculateChecksum(gs_RacCtrl.ucBuf, 7);
    hw_api_uart_clear_rcv_data(UART_SN_0); 
    drv_common_usart0_send(gs_RacCtrl.ucBuf, 8); 
}




//����ѹ
uint32_t Meter_V9260_read_ulevel(uint16_t Addr, uint8_t num)
{
    uint8_t i, ucSum = 0;

    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacRead;
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr & 0x00ff);
    gs_RacCtrl.ucBuf[3] = num;
    gs_RacCtrl.ucBuf[4] = 0;
    gs_RacCtrl.ucBuf[5] = 0;
    gs_RacCtrl.ucBuf[6] = 0;
    gs_RacCtrl.ucBuf[7] = CalculateChecksum(gs_RacCtrl.ucBuf, 7);
    hw_api_uart_clear_rcv_data(UART_SN_0); 
    drv_common_usart0_send(gs_RacCtrl.ucBuf, 8); 

    delay_ms(20); 

    if(!hw_api_uart_check_seqeue_empty(UART_SN_0)){
        // printf("rx_data:");
        for(i = 0; i < 10; i++){
            if(TRUE == hw_api_uart_check_seqeue_empty(UART_SN_0)){
                break;
            }
            gs_RacCtrl.ucRevPoint[i]= hw_api_uart_dequeue(UART_SN_0);
            // printf(" %02X", gs_RacCtrl.ucRevPoint[i]);
        }
        // printf("\r\n");
    }

    gs_RacCtrl.ucRevLen = ((uint32_t)gs_RacCtrl.ucRevPoint[6] << 24) |
                          ((uint32_t)gs_RacCtrl.ucRevPoint[5] << 16) |
                          ((uint32_t)gs_RacCtrl.ucRevPoint[4] << 8)  |
                          ((uint32_t)gs_RacCtrl.ucRevPoint[3]);

    gs_RacCtrl.ucSendPoint = gs_RacCtrl.ucRevLen / gs_JbPm.ul_URmG;
    
    // printf("gs_RacCtrl.ucSendPoint = %ldV", gs_RacCtrl.ucSendPoint);

    ucSum = CalculateChecksum(gs_RacCtrl.ucBuf, 7);
    if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
    {
        return gs_RacCtrl.ucSendPoint;
    } else{
        return FALSE;
    } 
}



//������
uint32_t Meter_V9260_read_currentevel(uint16_t Addr, uint8_t num)
{
    uint8_t ucSum = 0;

    gs_RacCtrl.ucSendLen[0] = 0x7d;
    gs_RacCtrl.ucSendLen[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacRead;
    gs_RacCtrl.ucSendLen[2] = (uint8_t)(Addr & 0x00ff);
    gs_RacCtrl.ucSendLen[3] = num;
    gs_RacCtrl.ucSendLen[4] = 0;
    gs_RacCtrl.ucSendLen[5] = 0;
    gs_RacCtrl.ucSendLen[6] = 0;
    gs_RacCtrl.ucSendLen[7] = CalculateChecksum(gs_RacCtrl.ucSendLen, 7);
    hw_api_uart_clear_rcv_data(UART_SN_0); 
    drv_common_usart0_send(gs_RacCtrl.ucSendLen, 8); 

    delay_ms(20); 

    if(!hw_api_uart_check_seqeue_empty(UART_SN_0)){
        // printf("rx_data:");
        for(int i = 0; i < 16; i++){
            if(TRUE == hw_api_uart_check_seqeue_empty(UART_SN_0)){
                break;
            }
            gs_RacCtrl.ucStatus[i]= hw_api_uart_dequeue(UART_SN_0);
            // printf(" %02X", gs_RacCtrl.ucStatus[i]);
        }
        // printf("\r\n");
    }

    gs_RacCtrl.uc_Phystat = ((uint32_t)gs_RacCtrl.ucStatus[6] << 24) |
                            ((uint32_t)gs_RacCtrl.ucStatus[5] << 16) |
                            ((uint32_t)gs_RacCtrl.ucStatus[4] << 8)  |
                            ((uint32_t)gs_RacCtrl.ucStatus[3]);

    // printf("gs_RacCtrl.uc_Phystat = %ld\r\n", gs_RacCtrl.uc_Phystat);

    gs_RacCtrl.uc_Sendstat = gs_RacCtrl.uc_Phystat / gs_JbPm.ul_I1RmG;

    // printf("gs_RacCtrl.uc_Sendstat = %ld\r\n", gs_RacCtrl.uc_Sendstat);

    ucSum = CalculateChecksum(gs_RacCtrl.ucSendLen, 7);
    if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
    {
        return gs_RacCtrl.uc_Sendstat;
    } else{
        return FALSE;
    } 
}

//������
uint32_t Meter_V9260_read_energyevel(uint16_t Addr, uint8_t num)
{
    uint8_t i, ucSum = 0;

    gs_RacCtrl.uc_Uartstat[0] = 0x7d;
    gs_RacCtrl.uc_Uartstat[1] = (uint8_t)((Addr & 0x0f00) >> 4) + RacRead;
    gs_RacCtrl.uc_Uartstat[2] = (uint8_t)(Addr & 0x00ff);
    gs_RacCtrl.uc_Uartstat[3] = num;
    gs_RacCtrl.uc_Uartstat[4] = 0;
    gs_RacCtrl.uc_Uartstat[5] = 0;
    gs_RacCtrl.uc_Uartstat[6] = 0;
    gs_RacCtrl.uc_Uartstat[7] = CalculateChecksum(gs_RacCtrl.uc_Uartstat, 7);
    hw_api_uart_clear_rcv_data(UART_SN_0); 
    drv_common_usart0_send(gs_RacCtrl.uc_Uartstat, 8); 

    delay_ms(20); 

    if(!hw_api_uart_check_seqeue_empty(UART_SN_0)){
        // printf("rx_data:");
        for(i = 0; i < 16; i++){
            if(TRUE == hw_api_uart_check_seqeue_empty(UART_SN_0)){
                break;
            }
            gs_RacCtrl.uc_Powstat[i]= hw_api_uart_dequeue(UART_SN_0);
            // printf(" %02X", gs_RacCtrl.uc_Powstat[i]);
        }
        // printf("\r\n");
    }

    gs_RacCtrl.uc_flapara = ((uint32_t)gs_RacCtrl.uc_Powstat[6] << 24) |
                            ((uint32_t)gs_RacCtrl.uc_Powstat[5] << 16) |
                            ((uint32_t)gs_RacCtrl.uc_Powstat[4] << 8)  |
                            ((uint32_t)gs_RacCtrl.uc_Powstat[3]);

    gs_RacCtrl.uc_flapara = (int32_t)gs_RacCtrl.uc_flapara; 

    if(gs_RacCtrl.uc_flapara < 0){
        gs_RacCtrl.uc_flapara = 0;
    }         

    // printf("gs_RacCtrl.uc_flapara = %ld\r\n", gs_RacCtrl.uc_flapara);

    gs_RacCtrl.uc_Physendtim = gs_RacCtrl.uc_flapara / gs_JbPm.ul_PG;

    // printf("gs_RacCtrl.uc_Physendtim = %ld\r\n", gs_RacCtrl.uc_Physendtim);
    
    ucSum = CalculateChecksum(gs_RacCtrl.uc_Uartstat, 7);
    if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
    {
        return gs_RacCtrl.uc_Physendtim;
    } else{
        return FALSE;
    } 
}

















