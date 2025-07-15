#ifndef _BSP_METER_V9260_H_
#define _BSP_METER_V9260_H_


#include "gd32e23x.h"
#include "IndependentDataType.h"




#define RCD_INT_PORT                           			    GPIOB                    	  //计量中断输出
#define RCD_INT_PIN                            			    GPIO_PIN_8                 	//计量中断输出

#define     SysCtrl   	        0x0180 
#define     BPFPARA 		        0x0107  
#define     UC      		        0x00FF 
#define     IAC     		        0x00FD    
#define     PAC     		        0x00F6
#define     PADCC   		        0x00F8
#define     QADCC   		        0x00FB

#define     QAC     		        0x00F9      //A·�޹����ʱȲ�У��
#define     PHC     		        0x00F7      //�й����ʽǲ�У��
#define     IADCC   		        0x00FE      //A·������ЧֵС�ź�У��
#define     PAAVG               0x00D0      //A·ƽ���й�����ֵ�Ĵ���
#define     UAVG                0x00D4      //ƽ����ѹֵ�Ĵ���
#define     IAAVG               0x00D3      //A·ƽ������ֵ�Ĵ���

#define     SysCtrl_ValueA      0x39C54001   // +40 PPM
#define     BPFPARA_ValueDef    0x806764B6 //��ͨ�˲��������Ĵ���ֵ
#define     AnaCtrl1_Val        0x10000000 //ģ�������ƼĴ���1
#define     RacWrite            0x02 //д������
#define     RacRead             0x01 //��ȡ����


                //


typedef enum
{
	V9260S_NULL = 0,
	V9260S_ONLINE,
	V9260S_OFFLINE
}_e_v9260s_comm_status;

typedef struct
{
	_e_v9260s_comm_status comm_status;	//通讯状态
	FP32 voltage;		//电压值
	FP32 current;		//电流值
	FP32 energy;		//电量
}_t_v9260s_info;





_t_v9260s_info* app_bl0939_get_meter_info(void);


void Meter_V9260_Init(void);
void Meter_V9260_Port_Init(void);
uint8_t CalculateChecksum(uint8_t *data, uint8_t length) ;
void V9260_InitDefaultPara(void);
uint8_t Meter_V9260_write(uint32_t Data, uint16_t Addr);

uint8_t UpdateRaccoonPara(void) ;


uint32_t Meter_V9260_read_ulevel(uint16_t Addr, uint8_t num);
uint32_t Meter_V9260_read_currentevel(uint16_t Addr, uint8_t num);
uint32_t Meter_V9260_read_energyevel(uint16_t Addr, uint8_t num);

#endif




