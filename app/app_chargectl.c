#include "app.h"
#include "systick.h"
#include "app_realcheck.h"
#include "app_chargectl.h"
#include "hw_api_usart.h"







//CP����OK���жϵ�ѹֵ-ŷ����Ҫͨ��
#define AC_CP_CNOK_NEED_VENT_VOL	3.0f


// CP����OK���жϵ�ѹֵ
#define AC_CP_CNOK_VOL		6.0f
// CP�����ӵ��жϵ�ѹֵ
#define AC_CP_SMCN_VOL		9.0f
// CP�Ͽ����жϵ�ѹֵ
#define AC_CP_BK_VOL		  12.0f
// CP�жϵĻز�  ��׼�涨��0.8V
//#define AC_CP_VOL_DIFF		0.8f

#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	// CP�жϵĻز�  ŷ��涨��1.0V
	#define AC_CP_VOL_DIFF		1.0f
#else
	// Ĭ��Ϊ����0.8V
	#define AC_CP_VOL_DIFF		0.8f
#endif


/// ���˲�״̬���Ͷ���
#define CHECKSTATETYPE_SEMICN		     	0x00000001		///< ������-9V
#define CHECKSTATETYPE_CNOK			     	0x00000002		///< ���Ӻ�-6V
#define CHECKSTATETYPE_EMERSTOP		     	0x00000004		///< ��ͣ����
#define CHECKSTATETYPE_OVP			     	0x00000008		///< OVP
#define CHECKSTATETYPE_OCP			     	0x00000010		///< OCP
#define CHECKSTATETYPE_UVP			     	0x00000020		///< UVP
#define CHECKSTATETYPE_TEMPOVER     	 	0x00000040		///< ����
#define CHECKSTATETYPE_EARTHING			 	0x00000080		///< �ӵ�
#define CHECKSTATETYPE_HANDOVERTIME      	0x00000100		///< ���ֳ�ʱ,����S2�պ�ʱ��
#define CHECKSTATETYPE_METER_COMM			0x00000200		///< ����ͨѶʧ��
#define CHECKSTATETYPE_LEAKCURR			 	0x00000400		///< ©��
#define CHECKSTATETYPE_SUSPENDOVERTIME   	0x00000800		///< ��ͣ��糬��10min
#define CHECKSTATETYPE_METER_ENERGY_FAULT	0x00001000		///< �������쳣

// ��׼���������׶�,��Ҫ�˲��״̬
#define CHECKTYPE_PREPARE	(CHECKSTATETYPE_SEMICN | CHECKSTATETYPE_EMERSTOP | CHECKSTATETYPE_TEMPOVER| CHECKSTATETYPE_EARTHING | \
							CHECKSTATETYPE_HANDOVERTIME | CHECKSTATETYPE_METER_COMM | CHECKSTATETYPE_LEAKCURR)
// �ڳ����,��Ҫ�˲��״̬
#define CHECKTYPE_CHARGING	(CHECKSTATETYPE_CNOK | CHECKSTATETYPE_EMERSTOP | CHECKSTATETYPE_OVP | CHECKSTATETYPE_OCP | \
							CHECKSTATETYPE_UVP | CHECKSTATETYPE_TEMPOVER | CHECKSTATETYPE_EARTHING | CHECKSTATETYPE_METER_COMM| \
							CHECKSTATETYPE_LEAKCURR | CHECKSTATETYPE_SUSPENDOVERTIME | CHECKSTATETYPE_METER_ENERGY_FAULT)


#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
#define CHARGE_S2_OPEN_SUSPEND		60			//s2�Ͽ���ͣ���ȴ�ʱ��,Ĭ��60S
#else
#define CHARGE_S2_OPEN_SUSPEND		60			//s2�Ͽ���ͣ���ȴ�ʱ��,Ĭ��300S
#endif



//��̬����
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

static FP32 total_kwh_old=0;	//��һ�εĳ�����

#define PREPARE_CHARGE_CLOSE_PWM_TIME	30000		//30S
#define PREPARE_CHARGE_RETRY_OPEN_PWM_TIME	32000		//32S

#define PREPARE_CHARGE_RETRY_6V_TO_9V_CLOSE_PWM_TIME	1000		
#define PREPARE_CHARGE_RETRY_CP_6V_TIME	100  //100ms	

//��ȡ�����Ϣ���ṩΪ�����ļ�ʹ��
_t_ac_charge_info* app_chargectl_get_charge_info(void)
{
	return &s_ac_charge_info;
}

//��ȡ���׮�豸��Ϣ
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



//������쳣״̬����ͬ���׶Σ��쳣�����Ŀ��ͬ
static _e_ac_charge_err charge_ctl_check_err(INT32U check_type)
{
	//���9V����״̬
	if(check_type & CHECKSTATETYPE_SEMICN)
	{
	   #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	    if((s_ac_charge_info.cp_state!=AC_CP_SEMICONNECT)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK_NEED_VENT))
	   #else
		if((s_ac_charge_info.cp_state!=AC_CP_SEMICONNECT)&&(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK))
	   #endif
		{
			//delay_ms(15);	//�ȴ�20ms�ٴ��ж�cp
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
	//���6V����״̬/3v
	if(check_type & CHECKSTATETYPE_CNOK)
	{
      #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
        if(s_ac_charge_info.need_vent_flag==1)
        	{
			  	if(s_ac_charge_info.cp_state!=AC_CP_CONNECTOK_NEED_VENT)
				{
					//delay_ms(15);	//�ȴ�25ms�ٴ��ж�cp
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
			//delay_ms(15);	//�ȴ�25ms�ٴ��ж�cp

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

	//��⼱ͣ״̬
	if(check_type & CHECKSTATETYPE_EMERSTOP)
	{
		if(s_ac_charge_info.charge_err.bits.emergency_stop==TRUE)
		{
			return AC_CHARGE_ERR_EMERSTOPPUSH;
		}
	}
	#endif 
	//����ѹ����
	if(check_type & CHECKSTATETYPE_OVP)
	{
		if(s_ac_charge_info.charge_err.bits.over_voltage==TRUE)
		{
			return AC_CHARGE_ERR_OVP;
		}
	}
	//����������
	if(check_type & CHECKSTATETYPE_OCP)
	{
		if(s_ac_charge_info.charge_err.bits.over_current==TRUE)
		{
			return AC_CHARGE_ERR_OCP;
		}
	}
	//���Ƿѹ����
	if(check_type & CHECKSTATETYPE_UVP)
	{
		if(s_ac_charge_info.charge_err.bits.under_voltage==TRUE)
		{
			return AC_CHARGE_ERR_UVP;
		}
	}
	#if 1
	//�¶ȹ���
	if(check_type & CHECKSTATETYPE_TEMPOVER)
	{
		if(s_ac_charge_info.charge_err.bits.env_temper_over==TRUE)
		{
			return AC_CHARGE_ERR_TEMPOVER;
		}
	}
	#endif
	//�ӵع���
	if(check_type & CHECKSTATETYPE_EARTHING)
	{
		if(s_ac_charge_info.charge_err.bits.earthing_fault==TRUE)
		{
			return AC_CHARGE_ERR_EARTHING;
		}
	}
	//���ֳ�ʱ
	if(check_type & CHECKSTATETYPE_HANDOVERTIME)
	{
		if(s_ac_charge_info.charge_err.bits.handshake_timeout==TRUE)
		{
			return AC_CHARGE_ERR_HANDOVERTIME;
		}
	}
	//����ͨѶ����
	if(check_type & AC_CHARGE_ERR_METER_COMMFAIL)
	{
		if(s_ac_charge_info.charge_err.bits.meter_comm_fault==TRUE)
		{
			return AC_CHARGE_ERR_METER_COMMFAIL;
		}
	}
	//���©�����
	if(check_type & CHECKSTATETYPE_LEAKCURR)
	{
		if(s_ac_charge_info.charge_err.bits.leak_curr_fault==TRUE)
		{
			return AC_CHARGE_ERR_LEAKCURR;
		}
	}
	//��ͣ��糬��10����
	if(check_type & CHECKSTATETYPE_SUSPENDOVERTIME)
	{
		if(s_ac_charge_info.charge_err.bits.suspend_over_10min_fault==TRUE)
		{
			return AC_CHARGE_ERR_SUSPENDOVERTIME;
		}
	}
	//�������쳣-��Ҫ�ǵ�����ȡ�쳣
	if(check_type & CHECKSTATETYPE_METER_ENERGY_FAULT)
	{
		if(s_ac_charge_info.charge_err.bits.meter_energy_fault==TRUE)
		{
			return AC_CHARGE_ERR_METER_ENERGYFAULT;
		}
	}
	
	return AC_CHARGE_ERR_OK;
}
//ˢ�³����Ϣ
static void charge_ctl_refresh_charge_data(void)
{
	s_ac_charge_info.duration_s = (get_os_ticks_ms() - s_ac_charge_info.prepare_start_time)/1000;	//���ʱ��
	s_ac_charge_info.kwh = s_ac_charge_info.energy-s_ac_charge_info.start_kwh;
}

//�������ʼ������
static void charge_ctl_preparecharge_init_data(void)
{
	s_send_pwm_again_flag = FALSE;
	s_cp_6v_start_flag = FALSE; 
//	s_cp_6v_to_9v = FALSE;
	s_ac_charge_info.prepare_start_time = get_os_ticks_ms();
	
	s_ac_charge_info.start_kwh = s_ac_charge_info.energy; //�ܵĳ�����
	
}



//��ȡCP״̬
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

//������̴���
void app_charge_ctl_handle(void)
{

	_e_ac_charge_err ac_charge_err=AC_CHARGE_ERR_OK;


	switch(s_ac_charge_info.charge_state)
	{
		case AC_CHARGE_STATE_IDLE_GUN_NOLINK:			//ǹδ����
		
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
				//drv_common_set_cp_pwm(AC_MAX_CURRENT);			//����pwm
			#if((CUSTOMER_SELECT == CUSTOMER_JYC_GB_A01_FC01_ONEKEY_16A))
				drv_common_set_cp_pwm(get_oled_keypad_curr_flashdata());			//����pwm
				// log_d("111111\r\n");
			#else
		     	drv_common_set_cp_pwm(AC_MAX_CURRENT);
			#endif

				charge_ctl_preparecharge_init_data();
				//��ʼ�����ǰ����
			}

			if(s_charge_end_flag == TRUE)
			{
				s_charge_end_flag = FALSE;
				s_ac_charge_info.charge_end = FALSE;

			}
				
			break;
		case AC_CHARGE_STATE_START_PREPARE:			    // ���׼���У��պ�����̵���-�ȴ�6V
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
							app_para.relay_open_flag = TRUE;	//�̵�������־λ
							// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);	//���̵���
							#endif
							//��ʼ��������
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
							app_para.relay_open_flag = TRUE;	//�̵�������־λ
							// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);	//���̵���
							#endif
							
							//��ʼ��������
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
						app_para.relay_open_flag = TRUE;	//�̵�������־λ
						// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);	//���̵���
						//��ʼ��������
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
				       drv_common_set_cp_pwm(get_oled_keypad_curr_flashdata());			//����pwm
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
		case AC_CHARGE_STATE_CHARGING:			        // ����У�ѭ��ˢ��δ�����˵���¼
		
			//ˢ�³������		
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
				drv_common_toggle_gpio_status(OUTPUT_IO_L_RELAY_COLSE);	//�ؼ̵���
				app_para.relay_open_flag = FALSE;	//�̵�������־λ
				// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_RESET);	//�ؼ̵���
				#endif
				
				//drv_common_set_cp_pwm(0xFFFF);											//��pwm

				
				if((ac_charge_err==AC_CHARGE_ERR_CNFAIL))	//9V״̬�£���ʱ����ֹͣ���
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
					app_para.relay_open_flag = TRUE;	//�̵�������־λ
					// drv_common_set_gpio_status(OUTPUT_IO_RELAY_CTRL, GPIO_STATUS_SET);
					#endif
		    	}
			}
				
			break;
		case AC_CHARGE_STATE_STOP_ACT:			        // ���ֹͣ�������Ͽ�����̵�����ˢ��δ�����˵���¼��ֹͣpwm
			#if 1
			drv_common_toggle_gpio_status(OUTPUT_IO_L_RELAY_COLSE);	//�ؼ̵���
			app_para.relay_open_flag = FALSE;	//�̵�������־λ
			#endif
		
			drv_common_set_cp_pwm(0xFFFF);											//��pwm

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
		case AC_CHARGE_STATE_STOP_COMPLETE:			    // ���ֹͣ��ɣ��ȴ���ǹ
			//�ȴ�2����
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

//ֹͣ���
BOOLEAN app_chargectl_stopcharge(void)
{
	s_ac_charge_info.charge_state = AC_CHARGE_STATE_STOP_ACT;
	return TRUE;
}

//�������
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



//���������
void app_charge_ctl_process(void)
{
	ac_charge_cp_status_get();
	app_charge_ctl_handle();
}

