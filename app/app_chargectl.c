#include "app.h"
#include "systick.h"
#include "app_realcheck.h"
#include "app_chargectl.h"
#include "hw_api_usart.h"







//CP连接OK的判断电压值-欧标需要通风
#define AC_CP_CNOK_NEED_VENT_VOL	3.0f


// CP连接OK的判断电压值
#define AC_CP_CNOK_VOL		6.0f
// CP半连接的判断电压值
#define AC_CP_SMCN_VOL		9.0f
// CP断开的判断电压值
#define AC_CP_BK_VOL		  12.0f
// CP判断的回差  标准规定是0.8V
//#define AC_CP_VOL_DIFF		0.8f

#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	// CP判断的回差  欧标规定是1.0V
	#define AC_CP_VOL_DIFF		1.0f
#else
	// 默认为国标0.8V
	#define AC_CP_VOL_DIFF		0.8f
#endif


/// 充电核查状态类型定义
#define CHECKSTATETYPE_SEMICN		     	0x00000001		///< 半连接-9V
#define CHECKSTATETYPE_CNOK			     	0x00000002		///< 连接好-6V
#define CHECKSTATETYPE_EMERSTOP		     	0x00000004		///< 急停按下
#define CHECKSTATETYPE_OVP			     	0x00000008		///< OVP
#define CHECKSTATETYPE_OCP			     	0x00000010		///< OCP
#define CHECKSTATETYPE_UVP			     	0x00000020		///< UVP
#define CHECKSTATETYPE_TEMPOVER     	 	0x00000040		///< 过温
#define CHECKSTATETYPE_EARTHING			 	0x00000080		///< 接地
#define CHECKSTATETYPE_HANDOVERTIME      	0x00000100		///< 握手超时,车端S2闭合时间
#define CHECKSTATETYPE_METER_COMM			0x00000200		///< 计量通讯失败
#define CHECKSTATETYPE_LEAKCURR			 	0x00000400		///< 漏电
#define CHECKSTATETYPE_SUSPENDOVERTIME   	0x00000800		///< 暂停充电超过10min
#define CHECKSTATETYPE_METER_ENERGY_FAULT	0x00001000		///< 电表读数异常

// 在准备启动充电阶段,需要核查的状态
#define CHECKTYPE_PREPARE	(CHECKSTATETYPE_SEMICN | CHECKSTATETYPE_EMERSTOP | CHECKSTATETYPE_TEMPOVER| CHECKSTATETYPE_EARTHING | \
							CHECKSTATETYPE_HANDOVERTIME | CHECKSTATETYPE_METER_COMM | CHECKSTATETYPE_LEAKCURR)
// 在充电中,需要核查的状态
#define CHECKTYPE_CHARGING	(CHECKSTATETYPE_CNOK | CHECKSTATETYPE_EMERSTOP | CHECKSTATETYPE_OVP | CHECKSTATETYPE_OCP | \
							CHECKSTATETYPE_UVP | CHECKSTATETYPE_TEMPOVER | CHECKSTATETYPE_EARTHING | CHECKSTATETYPE_METER_COMM| \
							CHECKSTATETYPE_LEAKCURR | CHECKSTATETYPE_SUSPENDOVERTIME | CHECKSTATETYPE_METER_ENERGY_FAULT)


#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
#define CHARGE_S2_OPEN_SUSPEND		60			//s2断开暂停充电等待时间,默认60S
#else
#define CHARGE_S2_OPEN_SUSPEND		60			//s2断开暂停充电等待时间,默认300S
#endif



//静态变量
static _t_ac_charge_info s_ac_charge_info;
static _t_ac_charge_system_para t_ac_charge_system_para;
static INT64U s_cp_value_fault_start_ms = 0;
//static INT64U s_cp_6v_to_9v_start_ms = 0;
static INT64U s_cp_6v_start_ms = 0;
//static INT8U s_cp_9v_start_nub = 0;
static BOOLEAN s_cp_value_flag = FALSE;
static BOOLEAN s_charge_end_flag = FALSE;
static BOOLEAN s_send_pwm_again_flag=FALSE;
//static BOOLEAN s_cp_6v_to_9v=FALSE;
static BOOLEAN s_cp_6v_start_flag=FALSE;

static INT8U s_check_err_nub=0;

_e_oled_max_curr_type s_oled_max_curr;

static FP32 total_kwh_old=0;	//上一次的充电电量

#define PREPARE_CHARGE_CLOSE_PWM_TIME	30000		//30S
#define PREPARE_CHARGE_RETRY_OPEN_PWM_TIME	32000		//32S

#define PREPARE_CHARGE_RETRY_6V_TO_9V_CLOSE_PWM_TIME	1000		
#define PREPARE_CHARGE_RETRY_CP_6V_TIME	100  //100ms	

//获取充电信息，提供为其他文件使用
_t_ac_charge_info* app_chargectl_get_charge_info(void)
{
	return &s_ac_charge_info;
}

//获取充电桩设备信息
_t_ac_charge_system_para* app_chargectl_get_system_para(void)
{
	return &t_ac_charge_system_para;
}

INT8U get_oled_keypad_curr_flashdata(void)
{

	INT8U max_curr =10;

	switch (s_oled_max_curr)
		{
			case MAX_CURR_10:
                 max_curr = 10;			
				break;
				
			case MAX_CURR_12:
				max_curr = 12;
				break;
				
			case MAX_CURR_14:
				max_curr = 14;
				break;
				
			case MAX_CURR_16:
				max_curr = 16;
				break;
			case MAX_CURR_ERROR:
				break;
			default:
			    max_curr = 10;
				break;
		}
		
		// log_d("max_curr22 = %d\r\n",max_curr);//10
    return  max_curr;
}



//检查充电异常状态，不同充电阶段，异常检测项目不同
static _e_ac_charge_err charge_ctl_check_err(INT32U check_type)
{
	//检测9V连接状态
	if(check_type & CHECKSTATETYPE_SEMICN)
	{
	   #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	    if((s_ac_charge_info.cp_state!=AC_CP_SEMICONNECT)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK_NEED_VENT))
	   #else
		if((s_ac_charge_info.cp_state!=AC_CP_SEMICONNECT)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK))
	   #endif
		{
			//delay_ms(15);	//等待20ms再次判断cp
			//if((s_ac_charge_info.cp_state!=AC_CP_SEMICONNECT)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK))
			//if((get_os_ticks_ms()-s_ocp_fault_start_ms)>100)
			if(s_cp_value_flag == 1)
			{
			   if((get_os_ticks_ms()-s_cp_value_fault_start_ms)>25)
				return AC_CHARGE_ERR_SEMICNFAIL;
			}
			else
			{
			   s_cp_value_flag = 1;
			   s_cp_value_fault_start_ms = get_os_ticks_ms();

			}
		}
		else
			{
			   s_cp_value_flag = 0;

		    }
	}
	//检测6V连接状态/3v
	if(check_type & CHECKSTATETYPE_CNOK)
	{
      #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
        if(s_ac_charge_info.need_vent_flag==1)
        	{
			  	if(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK_NEED_VENT)
				{
					//delay_ms(15);	//等待25ms再次判断cp
					//if(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK_NEED_VENT)
					//{
					//	return AC_CHARGE_ERR_CNFAIL;
					//}

					if(s_ac_charge_info.detect_cp_3v_flag == 1)
					{
					   if((get_os_ticks_ms()-s_ac_charge_info.detect_cp_3v_time)>25)
					   	{
						  return AC_CHARGE_ERR_CNFAIL;
					   	}
					}
					else
					{
					   s_ac_charge_info.detect_cp_3v_flag = 1;
					   s_ac_charge_info.detect_cp_3v_time = get_os_ticks_ms();

					}
				}
				else
					{
				       s_ac_charge_info.detect_cp_3v_flag = 0;

				   }
        	}
		else
			{
				if(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK)
	        	{
					
					if(s_ac_charge_info.detect_cp_6v_flag == TRUE)
					{
					   if((get_os_ticks_ms()-s_ac_charge_info.detect_cp_6v_time)>25)
						{
						  return AC_CHARGE_ERR_CNFAIL;
						}
					}
					else
					{
					   s_ac_charge_info.detect_cp_6v_flag = TRUE;
					   s_ac_charge_info.detect_cp_6v_time = get_os_ticks_ms();
					
					}

		         }
				else
					{
				    s_ac_charge_info.detect_cp_6v_flag = FALSE;

				}


		  }


	  #else
		if(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK)
		{
			//delay_ms(15);	//等待25ms再次判断cp

			if(s_ac_charge_info.detect_cp_6v_flag == TRUE)
			{
			   if((get_os_ticks_ms()-s_ac_charge_info.detect_cp_6v_time)>25)
			   	{
				  return AC_CHARGE_ERR_CNFAIL;
			   	}
			}
			else
			{
			   s_ac_charge_info.detect_cp_6v_flag = TRUE;
			   s_ac_charge_info.detect_cp_6v_time = get_os_ticks_ms();

			}
			
			//if(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK)
			//{
			//	return AC_CHARGE_ERR_CNFAIL;
			//}
		}
		else
		{
		    s_ac_charge_info.detect_cp_6v_flag = FALSE;

		}
	  #endif
	}
    #if(CUSTOMER_BOARD_SELECT ==JYC_GD_A10L)

	//检测急停状态
	if(check_type & CHECKSTATETYPE_EMERSTOP)
	{
		if(s_ac_charge_info.charge_err.bits.emergency_stop==TRUE)
		{
			return AC_CHARGE_ERR_EMERSTOPPUSH;
		}
	}
	#endif 
	//检测过压故障
	if(check_type & CHECKSTATETYPE_OVP)
	{
		if(s_ac_charge_info.charge_err.bits.over_voltage==TRUE)
		{
			return AC_CHARGE_ERR_OVP;
		}
	}
	//检测过流故障
	if(check_type & CHECKSTATETYPE_OCP)
	{
		if(s_ac_charge_info.charge_err.bits.over_current==TRUE)
		{
			return AC_CHARGE_ERR_OCP;
		}
	}
	//检测欠压故障
	if(check_type & CHECKSTATETYPE_UVP)
	{
		if(s_ac_charge_info.charge_err.bits.under_voltage==TRUE)
		{
			return AC_CHARGE_ERR_UVP;
		}
	}
	#if 1
	//温度故障
	if(check_type & CHECKSTATETYPE_TEMPOVER)
	{
		if(s_ac_charge_info.charge_err.bits.env_temper_over==TRUE)
		{
			return AC_CHARGE_ERR_TEMPOVER;
		}
	}
	#endif
	//接地故障
	if(check_type & CHECKSTATETYPE_EARTHING)
	{
		if(s_ac_charge_info.charge_err.bits.earthing_fault==TRUE)
		{
			return AC_CHARGE_ERR_EARTHING;
		}
	}
	//握手超时
	if(check_type & CHECKSTATETYPE_HANDOVERTIME)
	{
		if(s_ac_charge_info.charge_err.bits.handshake_timeout==TRUE)
		{
			return AC_CHARGE_ERR_HANDOVERTIME;
		}
	}
	//计量通讯故障
	if(check_type & AC_CHARGE_ERR_METER_COMMFAIL)
	{
		if(s_ac_charge_info.charge_err.bits.meter_comm_fault==TRUE)
		{
			return AC_CHARGE_ERR_METER_COMMFAIL;
		}
	}
	//检测漏电故障
	if(check_type & CHECKSTATETYPE_LEAKCURR)
	{
		if(s_ac_charge_info.charge_err.bits.leak_curr_fault==TRUE)
		{
			return AC_CHARGE_ERR_LEAKCURR;
		}
	}
	//暂停充电超过10分钟
	if(check_type & CHECKSTATETYPE_SUSPENDOVERTIME)
	{
		if(s_ac_charge_info.charge_err.bits.suspend_over_10min_fault==TRUE)
		{
			return AC_CHARGE_ERR_SUSPENDOVERTIME;
		}
	}
	//电表读数异常-主要是电量读取异常
	if(check_type & CHECKSTATETYPE_METER_ENERGY_FAULT)
	{
		if(s_ac_charge_info.charge_err.bits.meter_energy_fault==TRUE)
		{
			return AC_CHARGE_ERR_METER_ENERGYFAULT;
		}
	}
	
	return AC_CHARGE_ERR_OK;
}
//刷新充电信息
static void charge_ctl_refresh_charge_data(void)
{
	s_ac_charge_info.duration_s = (get_os_ticks_ms() - s_ac_charge_info.prepare_start_time)/1000;	//充电时长
	s_ac_charge_info.kwh = s_ac_charge_info.energy-s_ac_charge_info.start_kwh;
}

//进入充电初始化参数
static void charge_ctl_preparecharge_init_data(void)
{
	s_send_pwm_again_flag = FALSE;
	s_cp_6v_start_flag = FALSE; 
//	s_cp_6v_to_9v = FALSE;
	s_ac_charge_info.prepare_start_time = get_os_ticks_ms();
	
	s_ac_charge_info.start_kwh = s_ac_charge_info.energy; //总的充电电量
	
}



//获取CP状态
static void ac_charge_cp_status_get(void)
{
	FP32 cp_vol;

	cp_vol = app_get_input_io_simulation_value(INPUT_IO_SIM_forward_CP);

	if(FLOAT_GREATER_EQUAL(cp_vol, AC_CP_CNOK_VOL-AC_CP_VOL_DIFF)&&FLOAT_LESS_EQUAL(cp_vol, AC_CP_CNOK_VOL+AC_CP_VOL_DIFF))	//5.2V-6.8V
	{
		s_ac_charge_info.cp_state = AC_CP_CONNECTOK;
		
	}
	else if(FLOAT_GREATER_EQUAL(cp_vol, AC_CP_SMCN_VOL-AC_CP_VOL_DIFF)&&FLOAT_LESS_EQUAL(cp_vol, AC_CP_SMCN_VOL+AC_CP_VOL_DIFF))	//8.2V-9.8V
	{
		s_ac_charge_info.cp_state = AC_CP_SEMICONNECT;

	}
	else if(FLOAT_GREATER_EQUAL(cp_vol, AC_CP_BK_VOL-AC_CP_VOL_DIFF)&&FLOAT_LESS_EQUAL(cp_vol, AC_CP_BK_VOL+AC_CP_VOL_DIFF))	//11.2V-12.8V
	{
		s_ac_charge_info.cp_state = AC_CP_BREAK;

	}	
#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	else if(FLOAT_GREATER_EQUAL(cp_vol, AC_CP_CNOK_NEED_VENT_VOL-AC_CP_VOL_DIFF)&&FLOAT_LESS_EQUAL(cp_vol, AC_CP_CNOK_NEED_VENT_VOL+AC_CP_VOL_DIFF))			//3V
	{
		s_ac_charge_info.cp_state = AC_CP_CONNECTOK_NEED_VENT;
	}
#endif
	else
	{
		s_ac_charge_info.cp_state = AC_CP_ERR;

	}
}

//充电流程处理
void app_charge_ctl_handle(void)
{

	_e_ac_charge_err ac_charge_err=AC_CHARGE_ERR_OK;


	switch(s_ac_charge_info.charge_state)
	{
		case AC_CHARGE_STATE_IDLE_GUN_NOLINK:			//枪未插入
		
         #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
			if((s_ac_charge_info.cp_state==AC_CP_SEMICONNECT)||(s_ac_charge_info.cp_state==AC_CP_CONNECTOK)||(s_ac_charge_info.cp_state==AC_CP_CONNECTOK_NEED_VENT))
		 #else
			if((s_ac_charge_info.cp_state==AC_CP_SEMICONNECT)||(s_ac_charge_info.cp_state==AC_CP_CONNECTOK))
		 #endif 
			{

			    #if((CUSTOMER_SELECT == CHENGGONG_JYC_GD_A01_NONE_16A_KEYPAD)||(CUSTOMER_SELECT == NENGSHENG_K046_23110007_JYC_GD_A02_NONE_16A_KEYPAD)\
					||(CUSTOMER_SELECT == CUSTOMER_JYC_GB_A10L_NONE_NONE_32A)||(CUSTOMER_SELECT == JYC_K046_24030002_JYC_GB_A10L_MS523_BL0939_32A))
				//log_d("\n waiting for keypad start ...\n");

				
				#else
				s_ac_charge_info.charge_state = AC_CHARGE_STATE_START_PREPARE;
				#endif
				//drv_common_set_cp_pwm(AC_MAX_CURRENT);			//发出pwm
			#if((CUSTOMER_SELECT == CUSTOMER_JYC_GB_A01_FC01_ONEKEY_16A))
				drv_common_set_cp_pwm(get_oled_keypad_curr_flashdata());			//发出pwm
				// log_d("111111\r\n");
			#else
		     	drv_common_set_cp_pwm(AC_MAX_CURRENT);
			#endif

				charge_ctl_preparecharge_init_data();
				//初始化充电前参数
			}

			if(s_charge_end_flag == TRUE)
			{
				s_charge_end_flag = FALSE;
				s_ac_charge_info.charge_end = FALSE;

			}
				
			break;
		case AC_CHARGE_STATE_START_PREPARE:			    // 充电准备中；闭合输出继电器-等待6V
			ac_charge_err=charge_ctl_check_err(CHECKTYPE_PREPARE);
			if(ac_charge_err != AC_CHARGE_ERR_OK)
			{

			    s_check_err_nub ++;
				if(s_check_err_nub >3)
					{
					s_check_err_nub = 0;
					// log_d("charge ctl prepare err==%d\n", ac_charge_err);
					drv_common_set_cp_pwm(0xFFFF);
					// log_d("2222222\r\n");
					s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
					}
			}
			else 
			{
				s_check_err_nub = 0;

	            #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)

				  if((s_ac_charge_info.cp_state == AC_CP_CONNECTOK)&&(s_cp_6v_start_flag == FALSE))
					{
	                    s_cp_6v_start_flag = TRUE;
				        s_cp_6v_start_ms = get_os_ticks_ms();				
					}
					else if((s_ac_charge_info.cp_state == AC_CP_CONNECTOK)&&(s_cp_6v_start_flag == TRUE)) 
					{
					    if((get_os_ticks_ms()-s_cp_6v_start_ms)>PREPARE_CHARGE_RETRY_CP_6V_TIME)
					    {
					    	s_ac_charge_info.need_vent_flag = FALSE;
					    	s_cp_6v_start_flag = FALSE;
							s_ac_charge_info.charge_state = AC_CHARGE_STATE_CHARGING;
							#if 1
							app_para.relay_open_flag = TRUE;	//继电器开标志位
							// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);	//开继电器
							#endif
							//初始化充电参数
							s_charge_end_flag = TRUE;	
					    }
					}				
				   else if((s_ac_charge_info.cp_state == AC_CP_CONNECTOK_NEED_VENT)&&(s_cp_6v_start_flag == FALSE))
					{
	                    s_cp_6v_start_flag = TRUE;
				        s_cp_6v_start_ms = get_os_ticks_ms();				
					}
					else if((s_ac_charge_info.cp_state == AC_CP_CONNECTOK_NEED_VENT)&&(s_cp_6v_start_flag == TRUE)) 
					{
					    if((get_os_ticks_ms()-s_cp_6v_start_ms)>PREPARE_CHARGE_RETRY_CP_6V_TIME)
					    {
					    	s_ac_charge_info.need_vent_flag = TRUE;
					    	s_cp_6v_start_flag = FALSE;
							s_ac_charge_info.charge_state = AC_CHARGE_STATE_CHARGING;
							#if 1
							app_para.relay_open_flag = TRUE;	//继电器开标志位
							// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);	//开继电器
							#endif
							
							//初始化充电参数
							s_charge_end_flag = TRUE;	
					    }
					}

				#else
		    	if((s_ac_charge_info.cp_state == AC_CP_CONNECTOK)&&(s_cp_6v_start_flag == FALSE))
				{
                    s_cp_6v_start_flag = TRUE;
			        s_cp_6v_start_ms = get_os_ticks_ms();				
				}
				else if((s_ac_charge_info.cp_state == AC_CP_CONNECTOK)&&(s_cp_6v_start_flag == TRUE)) 
				{
				    if((get_os_ticks_ms()-s_cp_6v_start_ms)>PREPARE_CHARGE_RETRY_CP_6V_TIME)
				    	{
				    	s_cp_6v_start_flag = FALSE;
						s_ac_charge_info.charge_state = AC_CHARGE_STATE_CHARGING;
						app_para.relay_open_flag = TRUE;	//继电器开标志位
						// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);	//开继电器
						//初始化充电参数
						s_charge_end_flag = TRUE;	
				    	}
				}
			#endif
				else if((get_os_ticks_ms()-s_ac_charge_info.prepare_start_time)>=PREPARE_CHARGE_RETRY_OPEN_PWM_TIME)
				{
					
					if(s_send_pwm_again_flag==TRUE)
					{
					//	drv_common_set_cp_pwm(AC_MAX_CURRENT);						
				    #if((CUSTOMER_SELECT == CUSTOMER_JYC_GB_A01_FC01_ONEKEY_16A))
				       drv_common_set_cp_pwm(get_oled_keypad_curr_flashdata());			//发出pwm
					//    log_d("3333333\r\n");
			        #else
		     	       drv_common_set_cp_pwm(AC_MAX_CURRENT);
			        #endif
						s_send_pwm_again_flag = FALSE;
					}


				}
				else if((get_os_ticks_ms()-s_ac_charge_info.prepare_start_time)>=PREPARE_CHARGE_CLOSE_PWM_TIME)
				{

			     	if(s_send_pwm_again_flag==FALSE)
					{
						drv_common_set_cp_pwm(0xFFFF);		
						// log_d("444444\r\n");
						s_send_pwm_again_flag = TRUE;
					}

				}
				

			}
			break;
		case AC_CHARGE_STATE_CHARGING:			        // 充电中；循环刷新未结算账单记录
		
			//刷新充电数据		
			#if(DATA_COUNTING_MODE_SELECT==CHARGING_DATA_ACCUMULATION)
			charge_ctl_refresh_charge_data();
			#else
			s_ac_charge_info.duration_s =s_ac_charge_info.start_time_ms/1000 + (get_os_ticks_ms() - s_ac_charge_info.prepare_start_time)/1000;
			s_ac_charge_info.kwh = total_kwh_old + (s_ac_charge_info.energy-s_ac_charge_info.start_kwh);
			#endif
			
			ac_charge_err = charge_ctl_check_err(CHECKTYPE_CHARGING);			
			if((ac_charge_err != AC_CHARGE_ERR_OK))
			{
			  //  s_cp_6v_to_9v = FALSE;
				//s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
				
				#if 1
				drv_common_toggle_gpio_status(OUTPUT_IO_L_RELAY_COLSE);	//关继电器
				app_para.relay_open_flag = FALSE;	//继电器开标志位
				// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_RESET);	//关继电器
				#endif
				
				//drv_common_set_cp_pwm(0xFFFF);											//关pwm

				
				if((ac_charge_err==AC_CHARGE_ERR_CNFAIL))	//9V状态下，超时后再停止充电
				{

				   if((s_ac_charge_info.cp_state==AC_CP_SEMICONNECT))
				   	{
						if(s_ac_charge_info.s2_open_flag==TRUE)
						{
							if((get_os_ticks_s()-s_ac_charge_info.s2_open_time) >=CHARGE_S2_OPEN_SUSPEND)
							{
								s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
								drv_common_set_cp_pwm(0xFFFF);
								// log_d("555555\r\n");
							}
						}
						else		//FALSE
						{
						    s_ac_charge_info.detect_cp_9v_to_6v_flag = FALSE;
							s_ac_charge_info.s2_open_flag = TRUE;
							s_ac_charge_info.s2_open_time = get_os_ticks_s();
						}
				   	}
				   else if((s_ac_charge_info.cp_state==AC_CP_BREAK)||(s_ac_charge_info.cp_state==AC_CP_ERR))
				   	{

					  if(s_ac_charge_info.detect_cp_9v_to_6v_flag == TRUE)
					  	{

						  if((get_os_ticks_ms()-s_ac_charge_info.detect_cp_9v_to_6v_time) >=1000)
						  	{

							  s_ac_charge_info.detect_cp_9v_to_6v_flag = FALSE;
							  s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
							  drv_common_set_cp_pwm(0xFFFF);	
							//   log_d("66666\r\n");
							}
					  	}
					  else
					  	{
	

						    s_ac_charge_info.detect_cp_9v_to_6v_flag = TRUE;
							s_ac_charge_info.detect_cp_9v_to_6v_time = get_os_ticks_ms();

					    }

				    } 
				   else
				   	{
					  s_ac_charge_info.detect_cp_9v_to_6v_flag = FALSE;

			        }
				}
				else
				{   
				    s_ac_charge_info.s2_open_flag = FALSE;
			    	s_ac_charge_info.detect_cp_9v_to_6v_flag = FALSE;
				    s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
				    drv_common_set_cp_pwm(0xFFFF);
					// log_d("777777\r\n");

				}

				
			}			
			else
			{
			    if(s_ac_charge_info.s2_open_flag==TRUE)
		    	{
				    s_ac_charge_info.s2_open_flag = FALSE;
					#if 1
					app_para.relay_open_flag = TRUE;	//继电器开标志位
					// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);
					#endif
		    	}
			}
				
			break;
		case AC_CHARGE_STATE_STOP_ACT:			        // 充电停止动作；断开输出继电器、刷新未结算账单记录、停止pwm
			#if 1
			drv_common_toggle_gpio_status(OUTPUT_IO_L_RELAY_COLSE);	//关继电器
			app_para.relay_open_flag = FALSE;	//继电器开标志位
			#endif
		
			drv_common_set_cp_pwm(0xFFFF);											//关pwm

			// log_d("888888\r\n");
			s_ac_charge_info.s2_open_flag = FALSE;
			if(s_charge_end_flag == TRUE)
				{
				  s_ac_charge_info.charge_end = TRUE;

			    }
			if(s_ac_charge_info.cp_state == AC_CP_BREAK)
			{
				s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_COMPLETE;
			}

			
			#if(DATA_COUNTING_MODE_SELECT!=CHARGING_DATA_ACCUMULATION)
			s_ac_charge_info.start_time_ms = s_ac_charge_info.duration_s*1000;
			total_kwh_old = s_ac_charge_info.kwh;
			#endif
			s_check_err_nub = 0;

			
			break;
		case AC_CHARGE_STATE_STOP_COMPLETE:			    // 充电停止完成；等待拔枪
			//等待2秒钟
			s_check_err_nub = 0;
			s_ac_charge_info.charge_state = AC_CHARGE_STATE_IDLE_GUN_NOLINK;
			if(s_charge_end_flag == TRUE)
				{
				
			     s_charge_end_flag = FALSE;
			     s_ac_charge_info.charge_end = FALSE;

			  }
			break;
		default:
			break;
	}
}

//停止充电
BOOLEAN app_chargectl_stopcharge(void)
{
	s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
	return TRUE;
}

//启动充电
BOOLEAN app_chargectl_startcharge(void)
{
	if(charge_ctl_check_err(CHECKTYPE_PREPARE)==AC_CHARGE_ERR_OK)
	{
		s_ac_charge_info.charge_state = AC_CHARGE_STATE_START_PREPARE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



//充电主流程
void app_charge_ctl_process(void)
{
	ac_charge_cp_status_get();
	app_charge_ctl_handle();
}

