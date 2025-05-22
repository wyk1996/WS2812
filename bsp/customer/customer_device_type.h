/******************************************************************************
*
*  				Copyright (C), 2023-2050, jyc Tech. Co., Ltd.
*
* File Name     : customer_device_type.h
* Description   : Device peripheral definition
*  
* History       :
* Version   Date         Author    Description
* 0.01      2023-08-16   jyc.rd    Create this file.
*
******************************************************************************/

#ifndef __CUSTOMER_DEVICE_TYPE_H__
#define __CUSTOMER_DEVICE_TYPE_H__

//屏型号定义
#define CUSTOMER_HMI_TYPE_NONE								0
#define CUSTOMER_HMI_TYPE_TJC3224T128   			1
//最大电流定义
#define CUSTOMER_MAX_CURRENT_TYPE_16A						0
#define CUSTOMER_MAX_CURRENT_TYPE_13A						1
#define CUSTOMER_MAX_CURRENT_TYPE_8A						2
#define CUSTOMER_MAX_CURRENT_TYPE_32A						4


//log输出等级
#define LOG_LEVEL_DEBUG 									1
#define LOG_LEVEL_INFO 										2
#define LOG_LEVEL_WARNING 									3
#define LOG_LEVEL_ERROR 									4
#define LOG_LEVEL_NULL										5


#define JYC_GD_A01						0
#define JYC_GD_A10L					 	1
#define JYC_GD_A02					    2


#define LED_TYPE_DEFUAL				    0
#define LED_TYPE_CHENGGONG			    1

//充电标准定义
#define CUSTOMER_CHARGE_STANDARD_GB							0			//国标
#define CUSTOMER_CHARGE_STANDARD_OB							1			//欧标

//刷新充电数据
#define CHARGING_DATA_DOES_NOT_ACCUMULATION	   		    0  //在原因基础上进行累加
#define CHARGING_DATA_ACCUMULATION				    1 //不累加数据

#define CUSTOMER_CARD_TYPE_NONE								0
#define CUSTOMER_CARD_TYPE_MS523							1
#define CUSTOMER_CARD_TYPE_MT626							2



//卡密钥选择
#define CUSTOMER_CARD_KEY_COMMON							0			//通用密钥
#define CUSTOMER_CARD_KEY_UNIQUE							1			//唯一密钥


//串口定义
#define CUSTOMER_UART_TYPE_NONE								0xFF
#define CUSTOMER_UART_TYPE_0								0
#define CUSTOMER_UART_TYPE_1								1
#define CUSTOMER_UART_TYPE_2								2
#define CUSTOMER_UART_TYPE_3								3

#endif

