#ifndef _APP_METER_H_
#define _APP_METER_H_

#include "gd32e23x.h"







#define OLED_HELD_0		                                0x55
#define OLED_HELD_1		                                0xA5
#define OLED_RX_MAX_LEN	                                50


typedef enum
{
	OLED_TYPE_VOL = 0,
	OLED_TYPE_CURR,
	OLED_TYPE_ENERGY,
	OLED_TYPE_TIME,
    OLED_TYPE_INIT,  //INIT,	
	OLED_TYPE_VOL_NUM,
	OLED_TYPE_CURR_NUM,
	OLED_TYPE_ENERGY_NUM,
	OLED_TYPE_TIME_NUM,
	OLED_TYPE_START_CHARG,
	OLED_TYPE_STOP_CHARG,
	OLED_TYPE_KEYPAD_CURR,
	OLED_TYPE_OVER_VOLTAGE, //over_voltage,
	OLED_TYPE_OVER_CURRENT,  //over_current,
	OLED_TYPE_UNDER_VOLTAGE,  //under_voltage,
	OLED_TYPE_EARTHING_FAULT,  //earthing_fault,
	OLED_TYPE_ENV_TEMPER_OVER,  //env_temper_over,
	OLED_TYPE_HANDSHAKE_TIMEOUT,  //handshake_timeout,
	OLED_TYPE_METER_COMM_FAULT,  //meter_comm_fault,
	OLED_TYPE_LEAK_CURR_FAULT,  //leak_curr_fault,
	OLED_TYPE_SUSPEND_OVER_10MIN_FAULT,  //suspend_over_10min_fault,
	OLED_TYPE_METER_ENERGY_FAULT,  //meter_energy_fault,
	OLED_TYPE_CP_FAULT,  //cp_fault,
	OLED_TYPE_EMERGENCY_STOP,  //emergency_stop,
	OLED_TYPE_ERROR
} _e_oled_send_type;


void Meter_parameter_update(void);


#endif




