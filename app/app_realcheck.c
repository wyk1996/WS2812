#include "app_realcheck.h"
#include "Includes.h"
#include "hw_api_usart.h"
#include "Includes.h"
#include "drv_common.h"
#include "customer_config.h"
#include "app_chargectl.h"
#include "bsp_meter_v9260.h"
#include "systick.h"

static FP32 s_input_io_simulation_value[INPUT_IO_SIM_MAX];  //模拟量io值,校准后的


//故障检测使用
static BOOLEAN s_ovp_fault_flag = 0;
static BOOLEAN s_uvp_fault_flag = 0;
static BOOLEAN s_ocp_fault_flag = 0;
static BOOLEAN s_scp_fault_flag	= 0;


static INT64U s_ovp_fault_start_ms = 0;
static INT64U s_uvp_fault_start_ms = 0;
static INT64U s_ocp_fault_start_ms = 0;
static INT64U s_scp_fault_start_ms = 0;

static INT64U s_leak_curr_fault_start_ms = 0;

static INT8U  s_cp_value_fault_conut = 0;
static INT8U  s_leak_curr_fault_conut = 0;

#if(CUSTOMER_BOARD_SELECT ==JYC_GD_A10L)

static INT8U  s_stop_conut = 0;

#endif
#define THERMISTOR_SDNT1608X103_MIN	(-40)
#define THERMISTOR_SDNT1608X103_MAX	(125)

#define CHECK_SCP_FAULT_TIME		600		 // 600s
#define CHARGING_MIN_CURR_FAULT  0.8f 

static const FP32 table_thermistor_SDNT1608X103[THERMISTOR_SDNT1608X103_MAX-THERMISTOR_SDNT1608X103_MIN+1]=
{
 //-40~~-31
 352760, 330100, 308770, 289370, 271290, 254100, 238090, 223390, 209860, 196990,
 //-30~~-21
 185630, 173770, 163390, 153600, 144370, 135920, 127980, 120620, 113500, 106380,
 //-20~~-11
 100350, 94710, 89460, 84480, 79770, 75440, 71230, 67400, 63760, 60270,
 //-10~~-1
 57100, 54020, 51210, 48510, 45940, 43600, 41350, 39270, 37230, 35370,
 //0~~9
 33570, 31890, 30420, 28320, 26770, 25580, 24330, 23020, 21920, 20920,
 //10~~19
 20140, 19210, 18290, 17430, 16610, 15870, 15110, 14430, 13770, 13140,
 //20~~29
 12560, 11990, 11450, 10950, 10350, 10000, 9570, 9160, 8770, 8390,
 //30~~39
 8040, 7700, 7380, 7060, 6780, 6500, 6230, 5980, 5730, 5500,
 //40~~49
 5290, 5080, 4870, 4680, 4500, 4330, 4160, 4000, 3850, 3710,
 //50~~59
 3570, 3430, 3310, 3180, 3070, 2960, 2850, 2740, 2650, 2550,
 //60~~69
 2460, 2370, 2290, 2210, 2130, 2060, 1990, 1992, 1850, 1790,
 //70~~79
 1730, 1670, 1620, 1570, 1520, 1470, 1420, 1370, 1330, 1290,
 //80~~89
 1250, 1210, 1170, 1130, 1090, 1060, 1030, 1000, 970, 940,
 //90~~99
 910, 880, 860, 830, 810, 780, 760, 740, 720, 700,
 //100~~109
 680, 650, 630, 610, 600, 580, 560, 550, 530, 520,
 //110~~119
 500, 490, 480, 460, 450, 440, 430, 420, 410, 400,
 //120~~125 
 390, 380, 370, 360, 350, 340
};

static FP32 app_thermistor_temperature_get(FP32 res)
{
	INT32U i;

	for(i = 0; i < (THERMISTOR_SDNT1608X103_MAX-THERMISTOR_SDNT1608X103_MIN+1); i++)
	{
		if(res >= table_thermistor_SDNT1608X103[i])
		{
			break;
		}
	}
	return ((FP32)i + THERMISTOR_SDNT1608X103_MIN);
}

//根据adc序号读取值
FP32 app_get_input_io_simulation_value(_e_input_io_simulation_typedef io_simulation)
{
	FP32 temp_value=0.0f;
	switch(io_simulation)
	{
		case INPUT_IO_SIM_forward_CP:
			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_forward_CP];
			break;
		case INPUT_IO_SIM_EARTH:
			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_EARTH];
			break;
		case INPUT_IO_SIM_TEMP:
			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_TEMP];
			break;
		case INPUT_IO_SIM_short_L:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_short_L];
			break;
		case INPUT_IO_SIM_short_N:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_short_N];
			break;
		case INPUT_IO_SIM_board_version:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_board_version];
			break;
		case INPUT_IO_SIM_l_conglutination:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_l_conglutination];
			break;
		case INPUT_IO_SIM_n_conglutination:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_n_conglutination];
			break;
		case INPUT_IO_SIM_charging_pile_tempdetection:
	
			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_charging_pile_tempdetection];
			break;
		case INPUT_IO_SIM_relay_tempdetection:
	
			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_relay_tempdetection];
			break;
		case INPUT_IO_SIM_reservation_1:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_reservation_1];
			break;
		case INPUT_IO_SIM_negative_CP:

			temp_value = s_input_io_simulation_value[INPUT_IO_SIM_negative_CP];
			break;	
		case INPUT_IO_SIM_MAX:
			break;
		default:

			break;
	}

	return temp_value;
}

static void detection_alloc_input_port(void)
{
    
}

static void detection_adc_input(void)
{
    s_input_io_simulation_value[INPUT_IO_SIM_forward_CP] = drv_common_cp_vol_get();
	s_input_io_simulation_value[INPUT_IO_SIM_negative_CP] = drv_common_diode_vol_get();
	s_input_io_simulation_value[INPUT_IO_SIM_l_conglutination] = drv_common_synncehianL_vol_get();
	s_input_io_simulation_value[INPUT_IO_SIM_l_conglutination] = drv_common_synncehianN_vol_get();
	#if 1
	s_input_io_simulation_value[INPUT_IO_SIM_EARTH] = drv_common_earth_vol_get();
	//3.3/(Vadc / R1604)-R1604 == 3.3*R1604
	s_input_io_simulation_value[INPUT_IO_SIM_TEMP] = app_thermistor_temperature_get((6600/drv_common_temperature_vol_get())-2000);
	s_input_io_simulation_value[INPUT_IO_SIM_charging_pile_tempdetection] = app_thermistor_temperature_get((33000/drv_common_temp_environment_vol_get())-10000);
	#endif
	// delay_ms(500);
	// printf("s_input_io_simulation_value[INPUT_IO_SIM_forward_CP] = %f \r\n",s_input_io_simulation_value[INPUT_IO_SIM_forward_CP]);
	// printf("INPUT_IO_SIM_EARTH = %f \n",s_input_io_simulation_value[INPUT_IO_SIM_EARTH]);
	// printf("INPUT_IO_SIM_TEMP = %f \n",s_input_io_simulation_value[INPUT_IO_SIM_TEMP]);
	// printf("INPUT_IO_SIM_charging_pile_tempdetection = %f \n",s_input_io_simulation_value[INPUT_IO_SIM_charging_pile_tempdetection]);
	// printf("INPUT_IO_SIM_negative_CP = %f \n",s_input_io_simulation_value[INPUT_IO_SIM_negative_CP]);
	// printf("INPUT_IO_SIM_l_conglutination = %f \n",s_input_io_simulation_value[INPUT_IO_SIM_l_conglutination]);
}

void detection_charge_fault(void)
{
	_t_ac_charge_info* ptr_ac_charge_info = app_chargectl_get_charge_info();
	_t_ac_charge_system_para *ptr_ac_charge_system_para = app_chargectl_get_system_para();	
	INT32U earthing =0;

	//过压检测,电压大于volt_over并保持5s为异常
	if(FLOAT_GREATER(ptr_ac_charge_info->vol, ptr_ac_charge_system_para->volt_over))	//实际值大于设定值
	{
		if(s_ovp_fault_flag==TRUE)
		{
			if((get_os_ticks_ms()-s_ovp_fault_start_ms)>5000)
			{
				ptr_ac_charge_info->charge_err.bits.over_voltage = TRUE;
			}
		}
		else
		{
			s_ovp_fault_flag = TRUE;
			s_ovp_fault_start_ms = get_os_ticks_ms();
		}
	}
	else if(FLOAT_LESS((ptr_ac_charge_info->vol+10), ptr_ac_charge_system_para->volt_over))		//实际值小于设定值-10，恢复正常
	{
		//s_ovp_fault_flag = FALSE;
		//ptr_ac_charge_info->charge_err.bits.over_voltage = FALSE;
		if(s_ovp_fault_flag ==TRUE){
			s_ovp_fault_flag = FALSE;
			s_ovp_fault_start_ms = get_os_ticks_ms();
		}
		else if((get_os_ticks_ms()-s_ovp_fault_start_ms)>7000){
			ptr_ac_charge_info->charge_err.bits.over_voltage = FALSE;
		}
	}
	else
	{
	    s_ovp_fault_flag = FALSE;
		if((get_os_ticks_ms()-s_ovp_fault_start_ms)>7000)
		{
			ptr_ac_charge_info->charge_err.bits.over_voltage = FALSE;
		}
			
		

	}
	//过流检测,按照国标处理(curr_max大于20A,电流大于1.1*curr_max且保持5秒；curr_max不大于20A,电流大于2A+curr_max且保持5秒)
	//if(FLOAT_GREATER((FP32)ptr_ac_charge_system_para->curr_max, 20.0f))	//桩最大电流大于20A
#if((CUSTOMER_SELECT == CUSTOMER_JYC_GB_A01_FC01_ONEKEY_16A))
	if(FLOAT_GREATER((FP32)get_oled_keypad_curr_flashdata(), 20.0f))	//桩最大电流大于20A
	
#else	
	if(FLOAT_GREATER((FP32)ptr_ac_charge_system_para->curr_max, 20.0f))	//桩最大电流大于20A
#endif
	{
		// printf("ddddd\r\n");
		//if(FLOAT_GREATER(ptr_ac_charge_info->curr, ((FP32)ptr_ac_charge_system_para->curr_max)*((FP32)1.1)))
		#if((CUSTOMER_SELECT == CUSTOMER_JYC_GB_A01_FC01_ONEKEY_16A))
		if(FLOAT_GREATER(ptr_ac_charge_info->curr, ((FP32)get_oled_keypad_curr_flashdata())*((FP32)1.1)))
		
		#else
		if(FLOAT_GREATER(ptr_ac_charge_info->curr, ((FP32)ptr_ac_charge_system_para->curr_max)*((FP32)1.1)))
		#endif
		{
			if(s_ocp_fault_flag==TRUE)
			{
				
				if((get_os_ticks_ms()-s_ocp_fault_start_ms)>5000)
				{
					ptr_ac_charge_info->charge_err.bits.over_current = TRUE;
					
				}
			}
			else
			{
				
				s_ocp_fault_flag = TRUE;
				s_ocp_fault_start_ms = get_os_ticks_ms();
			}
		}
		else
		{
			
			s_ocp_fault_flag = FALSE;
			ptr_ac_charge_info->charge_err.bits.over_current = FALSE;
		}
	}
	else
	{
		
	//	if(FLOAT_GREATER(ptr_ac_charge_info->curr, ((FP32)ptr_ac_charge_system_para->curr_max+2.0f)))
    #if((CUSTOMER_SELECT == CUSTOMER_JYC_GB_A01_FC01_ONEKEY_16A))
		if(FLOAT_GREATER(ptr_ac_charge_info->curr, ((FP32)get_oled_keypad_curr_flashdata()+2.0f)))
		
	#else	
		if(FLOAT_GREATER(ptr_ac_charge_info->curr, ((FP32)ptr_ac_charge_system_para->curr_max+2.0f)))
	#endif
		{
			if(s_ocp_fault_flag==TRUE)
			{
				
				if((get_os_ticks_ms()-s_ocp_fault_start_ms)>5000)
				{
					
					ptr_ac_charge_info->charge_err.bits.over_current = TRUE;
				}
			}
			else
			{
				
				s_ocp_fault_flag = TRUE;
				s_ocp_fault_start_ms = get_os_ticks_ms();
			}
		}
		else
		{
			
			if(s_ocp_fault_flag ==TRUE){
				s_ocp_fault_flag = FALSE;
				s_ocp_fault_start_ms = get_os_ticks_ms();
			}
			else if((get_os_ticks_ms()-s_ocp_fault_start_ms)>6000){
			//s_ocp_fault_flag = FALSE;
			ptr_ac_charge_info->charge_err.bits.over_current = FALSE;
			}
			
		}
	}
	//欠压，暂时按照电压小于volt_under且保持5秒处理；高于于10V后恢复
	if(AC_CHARGE_STATE_CHARGING == ptr_ac_charge_info->charge_state)
    {
    	
    	if((FLOAT_LESS(ptr_ac_charge_info->vol, ptr_ac_charge_system_para->volt_under))&&(ptr_ac_charge_info->s2_open_flag==FALSE))
        {
           
            if(s_uvp_fault_flag==TRUE)
            {
                if(get_os_ticks_ms() - s_uvp_fault_start_ms >= 5000)
                {
                    ptr_ac_charge_info->charge_err.bits.under_voltage = TRUE;
                }
            }
            else
            {
                s_uvp_fault_flag = TRUE;
                s_uvp_fault_start_ms = get_os_ticks_ms();
            }
        }
        else if(FLOAT_GREATER(ptr_ac_charge_info->vol, (ptr_ac_charge_system_para->volt_under+10.0f)))
        {
        	
            s_uvp_fault_flag = FALSE;
            ptr_ac_charge_info->charge_err.bits.under_voltage = FALSE;
            
        }
    }
    else
    {
    	
        
		/*
		s_uvp_fault_flag = FALSE;
		if(get_os_ticks_ms() - s_uvp_fault_start_ms >= 7000)
        {
            ptr_ac_charge_info->charge_err.bits.under_voltage = FALSE;
        }
        */
        //ptr_ac_charge_info->charge_err.bits.under_voltage = FALSE;

		
            if(s_uvp_fault_flag ==TRUE){
				s_uvp_fault_flag = FALSE;
				s_uvp_fault_start_ms = get_os_ticks_ms();
			}
			else if(get_os_ticks_ms() - s_uvp_fault_start_ms >= 8500)
	        {
	            ptr_ac_charge_info->charge_err.bits.under_voltage = FALSE;
	        
	        }
    }
	//接地故障,小于0.65V故障
	
	
    if((FLOAT_LESS(app_get_input_io_simulation_value(INPUT_IO_SIM_EARTH), 0.65f))) 
	{
	    earthing = get_flashdata_earthing();
		
	    if( earthing  > 1 )
		ptr_ac_charge_info->charge_err.bits.earthing_fault = TRUE;
		else
		ptr_ac_charge_info->charge_err.bits.earthing_fault = FALSE;
			
	}
	else
	{
		ptr_ac_charge_info->charge_err.bits.earthing_fault = FALSE;
	}

	#if 1
	//握手超时
	if((AC_CHARGE_STATE_START_PREPARE == ptr_ac_charge_info->charge_state) && \
		((ptr_ac_charge_system_para->handshake_overtime*1000)<(get_os_ticks_ms()-ptr_ac_charge_info->prepare_start_time)))
	{
		ptr_ac_charge_info->charge_err.bits.handshake_timeout = TRUE;
	}
	else
	{
		ptr_ac_charge_info->charge_err.bits.handshake_timeout = FALSE;
	}

	#endif
	//计量芯片故障，待写
	#if 0
	if(app_bl0939_get_meter_info()->comm_status==BL0939_OFFLINE)
	{
		ptr_ac_charge_info->charge_err.bits.meter_comm_fault = TRUE;
	}
	else
	{
		ptr_ac_charge_info->charge_err.bits.meter_comm_fault = FALSE;
	}
	#endif
 // temperature
 	
	if(FLOAT_GREATER_EQUAL(app_get_input_io_simulation_value(INPUT_IO_SIM_TEMP), (FP32)ptr_ac_charge_system_para->env_temper_over))
	{
		ptr_ac_charge_info->charge_err.bits.env_temper_over = TRUE;
	}
	else if(FLOAT_LESS_EQUAL(app_get_input_io_simulation_value(INPUT_IO_SIM_TEMP), (FP32)(ptr_ac_charge_system_para->env_temper_over-10)))		//锟斤拷锟斤拷锟借定值10锟饺恢革拷
	{
		ptr_ac_charge_info->charge_err.bits.env_temper_over = FALSE;
	}


    // CP 电压异常
	if(ptr_ac_charge_info->cp_state == AC_CP_ERR)
	{
		s_cp_value_fault_conut++;

	    if(s_cp_value_fault_conut > 100)    
	    	{
		      ptr_ac_charge_info->charge_err.bits.cp_fault = TRUE;
		      s_cp_value_fault_conut = 0;

		   }
	}
	else
	{

	    s_cp_value_fault_conut = 0;
		ptr_ac_charge_info->charge_err.bits.cp_fault = FALSE;
	}
#if 1
if(AC_CHARGE_STATE_CHARGING == ptr_ac_charge_info->charge_state)
	{
		if(FLOAT_LESS_EQUAL(ptr_ac_charge_info->curr, CHARGING_MIN_CURR_FAULT))		//鍏呯數涓?灏戜??.8A
		{
			if(s_scp_fault_flag==TRUE)
			{
				if((get_os_ticks_s()-s_scp_fault_start_ms)>CHECK_SCP_FAULT_TIME)
				{

					ptr_ac_charge_info->charge_err.bits.suspend_over_10min_fault = TRUE;
				}
			}
			else
			{
				s_scp_fault_flag = TRUE;
				s_scp_fault_start_ms = get_os_ticks_s();
			}
		}
		else
		{
			s_scp_fault_flag = FALSE;
			ptr_ac_charge_info->charge_err.bits.suspend_over_10min_fault = FALSE;
		}
	}
	else
	{
		s_scp_fault_flag = FALSE;
		ptr_ac_charge_info->charge_err.bits.suspend_over_10min_fault = FALSE;
	}

#endif
	// BL0939 I_LINK  >30mA  漏电中断 
	// if(gpio_input_bit_get(BL0939_I_LINK_GPIO_PORT,BL0939_I_LINK_PIN) == SET)
	if(gpio_input_bit_get(RCD_INT_PORT,RCD_INT_PIN) == SET)
	{
		s_leak_curr_fault_conut++;
	    if(s_leak_curr_fault_conut > 3)    
	    {

		      ptr_ac_charge_info->charge_err.bits.leak_curr_fault = TRUE;

			  s_leak_curr_fault_start_ms = get_os_ticks_ms();
		      s_leak_curr_fault_conut = 0;

		}
	}
	else
	{

	    s_leak_curr_fault_conut = 0;

		if((get_os_ticks_ms()-s_leak_curr_fault_start_ms)>5000)
		{
			ptr_ac_charge_info->charge_err.bits.leak_curr_fault = FALSE;
		}
	}

	//emergency-stop
#if(CUSTOMER_BOARD_SELECT ==JYC_GD_A10L)

	if(gpio_input_bit_get(EMERGENCY_STOP_GPIO_PORT,EMERGENCY_STOP_PIN) == RESET)
	{
		s_stop_conut ++;
       #if(CUSTOMER_CARD_SELECT!=CUSTOMER_CARD_TYPE_NONE)
	   if(s_stop_conut>2)
	   #else
	   if(s_stop_conut>8)
	   #endif
			{
		
	          ptr_ac_charge_info->charge_err.bits.emergency_stop = TRUE;
			}

	}
	else
	{
	   s_stop_conut = 0;
	   ptr_ac_charge_info->charge_err.bits.emergency_stop = FALSE;

	}
#endif

#if(CUSTOMER_CARD_SELECT!=CUSTOMER_CARD_TYPE_NONE)

	//m1鍗￠€氳??寮傚父
	if((app_card_get_card_info()->comm_status==CARD_OFFLINE))
	{
		ptr_ac_charge_info->charge_err.bits.card_comm_fault = TRUE;
	}
	else
	{
		ptr_ac_charge_info->charge_err.bits.card_comm_fault = FALSE;
	}
#endif
}

//电表数据获取
static void meter_value_get(void)
{
	#if 1
	_t_ac_charge_info* ptr_ac_charge_info;
	_t_v9260s_info* ptr_meter_info;

	ptr_ac_charge_info = app_chargectl_get_charge_info();
    ptr_meter_info = app_v9260s_get_meter_info();

	ptr_ac_charge_info->vol = ptr_meter_info->voltage;
    ptr_ac_charge_info->curr = ptr_meter_info->current;
	ptr_ac_charge_info->energy = ptr_meter_info->energy;
	#endif
}


void Realcheck_Init(void)
{

}



void Realcheck_Handler(void)
{
    detection_alloc_input_port();
	meter_value_get();

	#if 1
    detection_adc_input();
	#endif

	#if 0
    detection_charge_fault();
	#endif
	
}





