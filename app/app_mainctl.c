#include "app_mainctl.h"
#include <stdio.h>
#include "hw_api_usart.h"
#include "systick.h"
#include "drv_common.h"
#include "app_chargectl.h"


static BOOLEAN  s_start_earthing  = FALSE;
static INT8U  s_earthing_count  = 0;

static INT64U s_earthing_cp_connect_time=0;
#if 0
static INT64U s_earthing_cp_disconnect_time=0;
#endif


static INT32U earthing_addr_T = 0x0800F800;
static INT32U earthing_flashData_T[1] = {0x00};
static INT8U  s_main_init =0;

#define MAIN_CP_CONNECT_OR_DISCONNECT_TIMEOUT	8000


#if 0
#if((CUSTOMER_BOARD_SELECT ==JYC_GD_A10L)||(CUSTOMER_BOARD_SELECT ==JYC_GD_A02))
static INT64U s_ctl_led_period=0;
static INT64U s_ctl_swo_excepting=0;
static BOOLEAN s_swo_flag	= FALSE;
#endif
#endif

INT32U get_flashdata_earthing(void)
{
    return  earthing_flashData_T[0];
}

#if 1
static void main_change_ground_detection_handle(void)
{
	_t_ac_charge_info* ptr_ac_charge_info = app_chargectl_get_charge_info();


	    if(s_main_init == 0)
    	{
		   s_main_init = 1;
	       earthing_flashData_T[0] = *(__IO INT32U*)(earthing_addr_T);

		  // log_d("earthing_flashData_T[0] = 0x%x \n",earthing_flashData_T[0]);
    	}


	if((s_start_earthing == FALSE )&&(ptr_ac_charge_info->cp_state ==AC_CP_BREAK))
		{
            s_start_earthing = TRUE ;  ///只能从 空枪 12V开始    
			if(s_earthing_count > 0)
				{
                   if((get_os_ticks_ms()-s_earthing_cp_connect_time ) > MAIN_CP_CONNECT_OR_DISCONNECT_TIMEOUT) //
                   	{
					   s_earthing_count  = 0;
				    }

			    }

			if((s_earthing_count>3) && (ptr_ac_charge_info->cp_state ==AC_CP_BREAK))
				{
					s_earthing_count = 0;
					
					if(earthing_flashData_T[0] == 0x00)
					{
						earthing_flashData_T[0] = 0x22;
					}
					else
					{
						earthing_flashData_T[0] = 0x00;
				
					}
					#if 1
					//待定
					WriteFlashTest(1,earthing_flashData_T,earthing_addr_T);
					#endif

					delay_ms(100);
					
					

			    }
	    }
    else if(s_start_earthing == TRUE)
		{
        
	      if ((ptr_ac_charge_info->cp_state ==AC_CP_SEMICONNECT)|| (ptr_ac_charge_info->cp_state == AC_CP_CONNECTOK))
	      	{

	      	    s_earthing_cp_connect_time = get_os_ticks_ms();  // 接枪时间
			    s_start_earthing = FALSE;
				s_earthing_count ++;

		    }

	    }

}


#endif

void app_mainctl_process(void)
{
	
	
	main_change_ground_detection_handle();
}


