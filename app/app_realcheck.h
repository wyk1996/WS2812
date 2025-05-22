#ifndef __APP_REALCHECK_H__
#define __APP_REALCHECK_H__

#include "gd32e23x.h"
#include "IndependentDataType.h"







typedef enum
{
	INPUT_IO_SIM_forward_CP = 0,	                        //正向CP检测
    INPUT_IO_SIM_negative_CP,	                            //负向CP检测
    INPUT_IO_SIM_short_L,                                    //L相短路检测  
    INPUT_IO_SIM_short_N,                                    //N相短路检测    
    INPUT_IO_SIM_board_version,	                            //硬件版本检测
    INPUT_IO_SIM_l_conglutination,                          //L相粘连检测
    INPUT_IO_SIM_n_conglutination,                          //N相粘连检测
    INPUT_IO_SIM_charging_pile_tempdetection,	            //充电桩温度检测
    INPUT_IO_SIM_relay_tempdetection,	                    //继电器温度检测
    INPUT_IO_SIM_reservation_1,	                            //预留1
	INPUT_IO_SIM_EARTH,		                                //接地检测
	INPUT_IO_SIM_TEMP,
	INPUT_IO_SIM_MAX
}_e_input_io_simulation_typedef;










void Realcheck_Handler(void);
void detection_charge_fault(void);
void Realcheck_Init(void);
FP32 app_get_input_io_simulation_value(_e_input_io_simulation_typedef io_simulation);

#endif


