#include "Includes.h"
#include "IndependentDataType.h"
#include "Includes.h"
#include "drv_common.h"
#include "app_common.h"
#include "app.h"
#include "bsp_oled.h"


#define ADC_SAMP_COUNTS 		10		//adc采样次数
#define ADC_ABD_MAX_NUM			2		// 去除的大值数量
#define ADC_ABD_MIN_NUM			2		// 去除的小值数量

#define AC_CP_ADC_VREF	     ((FP32)(3.3/4096))  //待调，根据芯片文档调整

//串口0初始化
void drv_common_usart0_init(INT32U baud_rate)
{
	/* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);

	/* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* USART configure */
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_9BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_ODD);
    usart_baudrate_set(USART0, baud_rate);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_enable(USART0);

	nvic_irq_enable(USART0_IRQn,0);//涓?鏂?閰嶇疆

	usart_interrupt_enable(USART0 , USART_INT_RBNE);

	
}
#if 0
//串口1初始化
void drv_common_usart1_init(INT32U baud_rate)
{
	/* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* USART configure */
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, baud_rate);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
		
	nvic_irq_enable(USART1_IRQn,0);//涓?鏂?閰嶇疆

	usart_interrupt_enable(USART1 , USART_INT_RBNE);

	/* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
}
#endif



//串口0发送函数
#if 1
void drv_common_usart0_send(INT8U *p_buff, INT16U data_len)
{
	INT16U i=0;
	while(data_len--)
	{
		usart_data_transmit(USART0, p_buff[i]);
		while(usart_flag_get(USART0,USART_FLAG_TBE) == RESET);
		i++;		
    }
}
#endif

// 串口发送字符串函数
void usart_send_string(const char *str)
{
    while (*str) {
        usart_data_transmit(USART0, (uint8_t)(*str)); // 假设 USART0 是你的串口
        while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET); // 等待发送完成
        str++;
    }
}


#if 0
//串口1发送函数
void drv_common_usart1_send(INT8U *p_buff, INT16U data_len)
{
	INT16U i=0;
	while(data_len--)
	{
		usart_data_transmit(USART1, p_buff[i]);
		while(usart_flag_get(USART1,USART_FLAG_TBE) == RESET);
		i++;		
    }
}
#endif

//adc1初始化
#if 0
void drv_common_adc1_init(void)
{
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);

    /* config the GPIO as analog mode */
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_PULLDOWN, GPIO_PIN_1|GPIO_PIN_4);

   /* config the GPIO as analog mode */
    gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_PULLDOWN, GPIO_PIN_1);

    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1U);

    /* ADC trigger config */
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger config */
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable();
    delay_ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
}
#endif

//adc获取通道电压
INT16U adc_channel_sample(INT8U channel)
{
	INT16U adc_value_array[ADC_SAMP_COUNTS];
	INT16U adc_avg_value = 0;
    INT8U i = 0;
 
	for(i = 0; i< ADC_SAMP_COUNTS; i++)
	{
		adc_resolution_config(ADC_RESOLUTION_12B);
		/* ADC regular channel config */
		adc_regular_channel_config(0U, channel, ADC_SAMPLETIME_7POINT5);
		/* ADC software trigger enable */
		adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
		 
		/* wait the end of conversion flag */
		while(!adc_flag_get(ADC_FLAG_EOC));
		/* clear the end of conversion flag */
		adc_flag_clear(ADC_FLAG_EOC);

		adc_value_array[i] = adc_regular_data_read();
			 
		if(i == (ADC_SAMP_COUNTS/2))
		{
			delay_ms(1);
		}
	}

 	adc_avg_value = app_common_get_avg_abandon_some_extr(adc_value_array, ADC_SAMP_COUNTS, ADC_ABD_MAX_NUM, ADC_ABD_MIN_NUM);
	return adc_avg_value;
 }

//获取到CP电压值
FP32 drv_common_cp_vol_get(void)
{   
    INT16U cp_adc_v;
    FP32 cp_vol;
    cp_adc_v= adc_channel_sample(ADC_CHANNEL_0);
	
    cp_vol = (FP32)AC_CP_ADC_VREF*cp_adc_v;
	// printf("cp_vol_AD = %f\r\n", cp_vol);
    return (cp_vol*161.0f/39.0f + 0.2f);
}

//获取接地电压值  输入没接地电压是0.6-1.1v   输入接地电压是0.6v    
FP32 drv_common_earth_vol_get(void)
{   
    INT16U earth_adc_v;
    FP32 earth_vol;
	
    earth_adc_v= adc_channel_sample(ADC_CHANNEL_9);
    earth_vol = (FP32)AC_CP_ADC_VREF*earth_adc_v;

	return earth_vol;
}

//获取继电器温度电压值
FP32 drv_common_temperature_vol_get(void)
{   
    INT16U temp_adc_v;
    FP32 temp_vol;
	
    temp_adc_v= adc_channel_sample(ADC_CHANNEL_7);
    temp_vol = (FP32)AC_CP_ADC_VREF*temp_adc_v;
	// printf("temp_vol = %f\r\n", temp_vol);
	return temp_vol;
}

//获取继环境度电压值
FP32 drv_common_temp_environment_vol_get(void)
{   
    INT16U temp_adc_v;
    FP32 temp_vol;
	
    temp_adc_v= adc_channel_sample(ADC_CHANNEL_6);
    temp_vol = (FP32)AC_CP_ADC_VREF*temp_adc_v;
	// printf("temp_vol = %f\r\n", temp_vol);
	return temp_vol;
}
//获取车端二极管电压
FP32 drv_common_diode_vol_get(void)
{
	INT16U diode_adc_v;
    FP32 diode_vol;
	
    diode_adc_v= adc_channel_sample(ADC_CHANNEL_1);
    diode_vol = (FP32)AC_CP_ADC_VREF*diode_adc_v;
	// printf("diode_vol = %f\r\n", diode_vol);

	return ((diode_vol/15)*61.0f+0.2f);
}

//获取继电器L相粘连电压
FP32 drv_common_synncehianL_vol_get(void)
{
	INT16U synncehian_adc_v;
    FP32 synncehian_vol;

	synncehian_adc_v= adc_channel_sample(ADC_CHANNEL_4);
    synncehian_vol = (FP32)AC_CP_ADC_VREF*synncehian_adc_v;
	// printf("synncehianL_vol = %f\r\n", synncehian_vol);
	return (synncehian_vol*161.0f/39.0f + 0.2f);
}

//获取继电器N相粘连电压
FP32 drv_common_synncehianN_vol_get(void)
{
	INT16U synncehian_adc_v;
    FP32 synncehian_vol;

	synncehian_adc_v= adc_channel_sample(ADC_CHANNEL_5);
    synncehian_vol = (FP32)AC_CP_ADC_VREF*synncehian_adc_v;
	// printf("synncehianN_vol = %f\r\n", synncehian_vol);
	return (synncehian_vol*161.0f/39.0f + 0.2f);
}

//获取输出短路电压
FP32 drv_common_short_vol_get(void)
{
	INT16U short_adc_v;
    FP32 short_vol;

	short_adc_v= adc_channel_sample(ADC_CHANNEL_3);
    short_vol = (FP32)AC_CP_ADC_VREF*short_adc_v;
	printf("diode_vol = %f\r\n", short_vol);
	return (short_vol*161.0f/39.0f + 0.2f);
}

//gpio初始化
void drv_common_gpio_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOF);

	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_bit_set(GPIOA, GPIO_PIN_8);

	//继电器
	gpio_mode_set(RELAY_L_CTRL_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RELAY_L_CTRL_PIN);
	gpio_output_options_set(RELAY_L_CTRL_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RELAY_L_CTRL_PIN);
	gpio_bit_reset(RELAY_L_CTRL_GPIO_PORT, RELAY_L_CTRL_PIN);

	gpio_mode_set(RELAY_N_CTRL_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RELAY_N_CTRL_PIN);
	gpio_output_options_set(RELAY_N_CTRL_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RELAY_N_CTRL_PIN);
	gpio_bit_reset(RELAY_N_CTRL_GPIO_PORT, RELAY_N_CTRL_PIN);
	//继电器粘连
	gpio_mode_set(RELAY_L_SYNNCEHIAN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RELAY_L_SYNNCEHIAN_PIN);
	gpio_output_options_set(RELAY_L_SYNNCEHIAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RELAY_L_SYNNCEHIAN_PIN);
	RELAY_L_SYNNCEHIAN_OFF;
	gpio_mode_set(RELAY_N_SYNNCEHIAN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RELAY_N_SYNNCEHIAN_PIN);
	gpio_output_options_set(RELAY_N_SYNNCEHIAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RELAY_N_SYNNCEHIAN_PIN);
	RELAY_N_SYNNCEHIAN_OFF;
	//5V电源控制
	gpio_mode_set(POWER_CTL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, POWER_CTL_PIN);
	gpio_output_options_set(POWER_CTL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, POWER_CTL_PIN);
	POWER_CTL_ON;


	//短路检测引脚
	gpio_mode_set(Out_Short_DET_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Out_Short_DET_PIN);
	gpio_output_options_set(Out_Short_DET_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Out_Short_DET_PIN);
	Out_Short_DET_ON;
	//B漏输入检测
	#if 0
	gpio_mode_set(RCD_DETECT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, RCD_DETECT_PIN);
	gpio_output_options_set(RCD_DETECT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCD_DETECT_PIN);
	#endif
	//B漏校零
	gpio_mode_set(RCD_ZERO_CALI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, RCD_ZERO_CALI_PIN);
	gpio_output_options_set(RCD_ZERO_CALI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCD_ZERO_CALI_PIN);
	gpio_bit_reset(RCD_ZERO_CALI_PORT, RCD_ZERO_CALI_PIN);
	//B漏校准
	#if 0
	gpio_mode_set(RCD_CALIBRATION_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, RCD_CALIBRATION_PIN);
	gpio_output_options_set(RCD_CALIBRATION_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCD_CALIBRATION_PIN);
	gpio_bit_reset(RCD_CALIBRATION_PORT, RCD_CALIBRATION_PIN);
	#endif
	//B漏自检
	gpio_mode_set(RCD_TEST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, RCD_TEST_PIN);
	gpio_output_options_set(RCD_TEST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RCD_TEST_PIN);
	gpio_bit_reset(RCD_TEST_PORT, RCD_TEST_PIN);



	#if 0
	//BL0939 I_link  >30mA
	rcu_periph_clock_enable(RCU_GPIOF);
	gpio_mode_set(BL0939_I_LINK_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BL0939_I_LINK_PIN);
	#endif
}




//设置gpio电平
void drv_common_set_gpio_status(_e_output_io_digital_typedef output_gpio, _e_gpio_status gpio_status)
{
	switch(output_gpio)
	{
		case OUTPUT_IO_LCD_POWER_CTRL:
			gpio_bit_write(LCD_POWER_CTL_PORT, LCD_POWER_CTL_PIN, (FlagStatus)gpio_status);
			break;
		case OUTPUT_IO_POWER_CTRL:
			gpio_bit_write(POWER_CTL_PORT, POWER_CTL_PIN, (FlagStatus)gpio_status);
			break;
		case OUTPUT_IO_L_SYNNCEHIAN_CTRL:
			gpio_bit_write(RELAY_L_SYNNCEHIAN_PORT, RELAY_L_SYNNCEHIAN_PIN, (FlagStatus)gpio_status);
			break;
		case OUTPUT_IO_N_SYNNCEHIAN_CTRL:
			gpio_bit_write(RELAY_N_SYNNCEHIAN_PORT, RELAY_N_SYNNCEHIAN_PIN, (FlagStatus)gpio_status);
			break;
		case OUTPUT_IO_CP_HIGLT_OFF:
			gpio_bit_write(GPIOA, GPIO_PIN_8, (FlagStatus)gpio_status);
			break;
			
		
		default:
			break;
	}
}








//根据电流设置pwm
void drv_common_set_cp_pwm(INT16U maxcurr)
{
	INT16U k=10;
	INT32U u32duty;

	if(maxcurr==0)
	{
		// printf("aaaa\r\n");
		u32duty = 0;
	}
	else if(maxcurr==5)	// 5表示需要数字通信
	{
		// printf("bbbb\r\n");
		u32duty = 5*k;
	}
	else if(maxcurr >= 0xFFFF)	// 0xFFFF表示直接输出12V
	{
		// printf("cccc\r\n");
		u32duty = 100*k;
	}
	else if(maxcurr < 5)
	{
		// printf("dddd\r\n");
		return;
	}
	else if(maxcurr <= 51)
	{
		// printf("eeee\r\n");
		u32duty = (1000*maxcurr/6)*k/100;
	}
	else if(maxcurr <= 63)
	{
		// printf("ffff\r\n");
		u32duty = (40*maxcurr + 6400)*k/100;
	}
	else
	{
		// printf("gggg\r\n");
		u32duty = 90*k;
	}

	if (u32duty > 1000) {
        u32duty = 1000;
		// printf("hhhhh\r\n");
    }
	#if 1
	// printf("u32duty = %d\r\n", u32duty);
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, u32duty);
	#endif
}



#if 1
//read write in-flash
///*FLASH鍐欏叆绋嬪簭*/
void WriteFlashTest(INT32U L,INT32U Data[],INT32U addr)
{
    INT32U i=0;

	//  /* 1/4解锁FLASH*/
	///* unlock the flash program/erase controller */
	fmc_unlock();//解锁Flash操作
	ob_unlock();//解锁选项字节,先决条件fmc_unlock
	
	//清除标志位  

   fmc_flag_clear(FMC_FLAG_BUSY);
   fmc_flag_clear(FMC_FLAG_PGERR);
   fmc_flag_clear(FMC_FLAG_WPERR);        
   fmc_flag_clear(FMC_FLAG_END);        
       

	///  /* 2/4擦除FLASH*/
	//擦除页
	fmc_page_erase(addr);
	
	////  /* 3/4对FLASH烧写*/

    for(i=0;i<L;i++)
    {
        fmc_word_program(addr+4*i, Data[i]);
    }    

  //// /* 4/4锁住FLASH*/   

	ob_lock();
    fmc_lock();    

}
#endif









