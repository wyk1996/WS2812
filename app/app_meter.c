#include "app_meter.h"
#include "IndependentDataType.h"
#include "systick.h"
#include <stdio.h>
#include "hw_api_usart.h"


static INT8U recv_buf_temp[OLED_RX_MAX_LEN] = {0};


static void meter_9260_read(void)
{
    INT8U meter_recv_buff[100]={0x00}, i=0, j =0,recv_count = 0,recv_nub = 0;

    if (!hw_api_uart_check_seqeue_empty(UART_SN_0))//检查接收队列是否为空
	{
	// log_i("------- ok ok \n");
	


		recv_nub = hw_api_uart_get_recv_data_len(UART_SN_0);//获取接收数据长度
      
        if( recv_nub < 7)
			#if 1
			return;
			#endif


			#if 0
			for(i=0; i<recv_nub; i++)
			{
				meter_recv_buff[i] = hw_api_uart_dequeue(UART_SN_0);
				log_d("\r");
				log_d("meter_recv_buff[%d] = 0x%x", i, meter_recv_buff[i]);
				log_d("\n");
			}
			drv_common_usart0_send(meter_recv_buff, recv_nub);	
			#endif
			

        for(i=0; i<100; i++)
        {
			if(TRUE == hw_api_uart_check_seqeue_empty(UART_SN_0))//检查接收队列是否为空
			{
				// log_i("------- ok ok \n");
				break;
			}
			{
				break;
			}
			
			meter_recv_buff[i] = hw_api_uart_dequeue(UART_SN_0);//获取接收数据

            #if 0
            log_i("meter_recv_buff[%d] = 0x%x", i, meter_recv_buff[i]);
            #endif


			
			if((meter_recv_buff[i] == OLED_HELD_0)&&(recv_count == 0)) 
				{
					
				  recv_count = 1;  ///OLED_HELD_0
				  delay_1ms(2);
				  
			    }

			if((recv_count == 1)&&(meter_recv_buff[i] == OLED_HELD_1)&&(meter_recv_buff[i-1] == OLED_HELD_0))
				{
			
				 recv_count = 2;  ///OLED_HELD_1
				 memset(recv_buf_temp, 0, sizeof(recv_buf_temp));
				 recv_buf_temp[0] = OLED_HELD_0;
				 recv_buf_temp[1] = OLED_HELD_1;
				 j =2;
				// log_i("ok ok \n");
				

			   }
			else if(recv_count == 2)
				{
				  
                    recv_buf_temp[j] = meter_recv_buff[i];
				    j++;
				}
			else
				{
			       if(j>10)
				   	break;

			    }


		}
    }
}

static void meter_9260_receive_number_handle(void)
{
    INT8U nub=0,curr =0 ;
    char str[10] = {0};
    static INT32U  s_oled_energy =0;
    static INT8U  s_start_charge =0;
    static INT8U  s_oled_curr =0;

    INT32U data=0 ;
 
    _e_oled_send_type sta;
    static INT8U cnt =0;
    if((recv_buf_temp[0] ==OLED_HELD_0)&&(recv_buf_temp[1] ==OLED_HELD_1))
   	{

		nub = (recv_buf_temp[0] +recv_buf_temp[1]+recv_buf_temp[2]+recv_buf_temp[5])&0xff ;

		// log_i("nub = %0x\r\n",nub);
        cnt = nub;
		if(nub == cnt)
		{

		    sta = (_e_oled_send_type)recv_buf_temp[2];
		    // log_i("sta = %d\r\n",sta);
		    switch(sta)
			{
                case OLED_TYPE_VOL:
                // log_i("111111\r\n");
                    break;
                    
                case OLED_TYPE_CURR:
            
                    break;
                    
                case OLED_TYPE_ENERGY:
                
                    break;
                    
                case OLED_TYPE_TIME:
                    break;
                case OLED_TYPE_VOL_NUM:	

				    data = (recv_buf_temp[3]<<16)+(recv_buf_temp[4]<<8) + (recv_buf_temp[5]);
                    #if 0
                    if(data >1000)
                    {
                        sprintf(str,"%d.%d",(char)(data/100),(char)(data%100)); 
                        OLED_ShowString(50,0,str,12);
                    }
                    else
                    {
                        // OLED_ShowString(60,0,"0.0000",12);
                        OLED_ShowString(50,0,"0.000  ",12);
                    }
                    OLED_Refresh();
                    #endif

				    break;
			    case OLED_TYPE_CURR_NUM:			     
			     
                    data = (recv_buf_temp[3]<<16)+(recv_buf_temp[4]<<8) + (recv_buf_temp[5]);
                    #if 0
                    sprintf(str,"%d.%d",(char)(data/100),(char)(data%100)); 
                    OLED_ShowString(50,16,str,12);
                    OLED_Refresh();
                    #endif
                    break;
                case OLED_TYPE_ENERGY_NUM:
                    
                    data = (recv_buf_temp[3]<<16)+(recv_buf_temp[4]<<8) + (recv_buf_temp[5]);

                    s_oled_energy = data;
                    sprintf(str,"%d.%02d",(char)(s_oled_energy/100),(char)(s_oled_energy%100)); 
                    // sprintf(str,"%d.%02d",(INT16U)(data/100),(INT16U)(data%100)); 
                    // OLED_ShowString(50,32,str,12);
                    // OLED_Refresh();
				    break;
			case OLED_TYPE_TIME_NUM:
			  	break;
			case OLED_TYPE_START_CHARG:
				s_start_charge =1;
			  	break;
			case OLED_TYPE_STOP_CHARG:
				s_start_charge = 0;
			  	break;
			case OLED_TYPE_KEYPAD_CURR:
			
				 curr = (INT8U)(recv_buf_temp[5]);

				//  log_i("curr = %d",curr);

			     if(curr == 0)//10A
			     { 
  			       s_oled_curr = 10;
  			    //    OLED_ShowString(100,32,"10A",12);
			     }
			     else if(curr == 1) //12A
			     {
			      s_oled_curr = 12;
			    //   OLED_ShowString(100,32,"12A",12);
			     }
			     else if(curr == 2) //14A
			     {
			     s_oled_curr = 14;
			    //   OLED_ShowString(100,32,"14A",12);
			     }
			     else if(curr == 3) //16A
			     {
			     s_oled_curr = 16;
			    //   OLED_ShowString(100,32,"16A",12);
			     }
			     else
			     {
			      s_oled_curr = 32;
				//   OLED_ShowString(100,32,"10A",12);
			     }

				//  OLED_Refresh();
			    break;

			case OLED_TYPE_OVER_VOLTAGE:
				//  OLED_Clear();
				//  OLED_ShowString(2,32,"over voltage",12);
				//  OLED_Refresh();
			  	break;
				
			case OLED_TYPE_OVER_CURRENT:
				//  OLED_Clear();
				//  OLED_ShowString(2,32,"over current",12);
				//  OLED_Refresh();
			  	break;
				
			case OLED_TYPE_UNDER_VOLTAGE:
				// OLED_Clear();
				// OLED_ShowString(2,32,"under voltage",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_EARTHING_FAULT:
				//  OLED_Clear();
				//  OLED_ShowString(2,32,"earthing fault",12);
				//  OLED_Refresh();
			  	break;
				
			case OLED_TYPE_ENV_TEMPER_OVER:
				// OLED_Clear();
				// OLED_ShowString(2,32,"temper over",12);
				// OLED_Refresh();

			  	break;
				
			case OLED_TYPE_HANDSHAKE_TIMEOUT:
				// OLED_Clear();
				// OLED_ShowString(2,32,"handshake timeout",12);
				// OLED_Refresh();

			  	break;
				
			case OLED_TYPE_METER_COMM_FAULT:
				// OLED_Clear();
				// OLED_ShowString(2,32,"meter fault",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_LEAK_CURR_FAULT:
				// OLED_Clear();
				// OLED_ShowString(2,32,"leakage of electricity",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_SUSPEND_OVER_10MIN_FAULT:
				// OLED_Clear();
				// OLED_ShowString(2,32,"over10min fault",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_METER_ENERGY_FAULT:
				// OLED_Clear();
				// OLED_ShowString(2,32,"meter energy fault",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_EMERGENCY_STOP:
				// OLED_Clear();
				// OLED_ShowString(2,32,"emergency stop",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_CP_FAULT:
				// OLED_Clear();
				// OLED_ShowString(2,32,"cp fault",12);
				// OLED_Refresh();
			  	break;
				
			case OLED_TYPE_INIT:
                 #if 0
				OLED_Clear();
				OLED_ShowString(2,0,"vol/V:",12);
				OLED_ShowString(50,0,"0.000",12);
				
				OLED_ShowString(2,16,"cur/A:",12);
				OLED_ShowString(50,16,"0.000",12);

				
				OLED_ShowString(2,32,"kWh :",12);
				sprintf(str,"%d.%02d",(char)(s_oled_energy/100),(char)(s_oled_energy%100)); 
				OLED_ShowString(50,32,str,12);
				//OLED_ShowString(50,32,"0.000",12);

				
				OLED_ShowString(2,48,"time:",12);
				//OLED_ShowString(50,48,"00:00:00",12);			
				sprintf(str,"%02d:%02d:%02d",(char)(s_oled_charge_time_old/3600),(char)((s_oled_charge_time_old%3600)/60),(char)(s_oled_charge_time_old%60)); 
				OLED_ShowString(50,48,str,12);

				sprintf(str,"%dA",s_oled_curr); 
				OLED_ShowString(100,32,str,12);	
				
				OLED_Refresh();		
                #endif		
			  	break;
			case OLED_TYPE_ERROR:
				// OLED_Clear();
				// OLED_ShowString(2,32,"error !!!",12);
				// OLED_Refresh();
			  	break;
			default:
				break;
			}

		 memset(recv_buf_temp, 0, sizeof(recv_buf_temp));
		}

    }
}







void Meter_parameter_update(void)
{
    meter_9260_read();
    meter_9260_receive_number_handle();
	
}







