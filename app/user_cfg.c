#include "user_cfg.h"
#include "systick.h"
#include "drv_common.h"


void Uart1_Init(void)
{
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, TX1_PIN);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, RX1_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, TX1_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, TX1_PIN);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RX1_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, RX1_PIN);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    usart_enable(USART1);
}

#if 0
void Spi_Config(void)
{

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_DMA);
    rcu_periph_clock_enable(RCU_SPI0);
    rcu_periph_clock_enable(RCU_SPI1);

    /* configure SPI0 GPIO: SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    /* configure SPI1 GPIO: SCK/PB13, MISO/PB14, MOSI/PB15 */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15); 
    
    dma_parameter_struct  dma_init_struct;
    dma_struct_para_init(&dma_init_struct);

    /* configure SPI0 transmit DMA: DMA_CH2 */
    dma_deinit(DMA_CH2);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)spi0_send_array;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_init_struct.number       = ARRAYSIZE;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA_CH2, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH2);
    dma_memory_to_memory_disable(DMA_CH2);

    /* configure SPI1 transmit DMA: DMA_CH4 */
    dma_deinit(DMA_CH4);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory_addr  = (uint32_t)spi1_send_array;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH ;
    dma_init(DMA_CH4, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH4);
    dma_memory_to_memory_disable(DMA_CH4);

    /* configure SPI0 receive DMA: DMA_CH1 */
    dma_deinit(DMA_CH1);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)spi0_receive_array;
    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_LOW;
    dma_init(DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH1);
    dma_memory_to_memory_disable(DMA_CH1);

    /* configure SPI1 receive DMA: DMA_CH3 */
    dma_deinit(DMA_CH3);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory_addr  = (uint32_t)spi1_receive_array;
    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_MEDIUM;
#if SPI_CRC_ENABLE
    dma_init_struct.number       = ARRAYSIZE + 1;
#endif /* enable CRC function */
    dma_init(DMA_CH3, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH3);
    dma_memory_to_memory_disable(DMA_CH3);

    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI0);
    spi_i2s_deinit(SPI1);
    spi_struct_para_init(&spi_init_struct);

    /* configure SPI0 parameter */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* configure SPI1 parameter */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_SLAVE;
    spi_init(SPI1, &spi_init_struct);

    /* configure SPI1 byte access to FIFO */
    spi_fifo_access_size_config(SPI1, SPI_BYTE_ACCESS);

#if SPI_CRC_ENABLE
    /* configure SPI CRC function */
    spi_crc_length_set(SPI1, SPI_CRC_8BIT);
    spi_crc_polynomial_set(SPI0, 7);
    spi_crc_polynomial_set(SPI1, 7);
    spi_crc_on(SPI0);
    spi_crc_on(SPI1);
#endif /* enable CRC function */

    spi_enable(SPI1);
    spi_enable(SPI0);

    /* enable DMA channel */
    /* SPI1_Tx DMA channel */
    dma_channel_enable(DMA_CH4);
    /* SPI0_Tx DMA channel */
    dma_channel_enable(DMA_CH2);
    /* SPI1_Rx DMA channel */
    dma_channel_enable(DMA_CH3);
    /* SPI0_Rx DMA channel */
    dma_channel_enable(DMA_CH1);

    /* enable SPI DMA */
    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI1, SPI_DMA_RECEIVE);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI0, SPI_DMA_RECEIVE);
}
#endif

void Fwdgt_Config(void)
{
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
    /* wait till IRC40K is ready */
    rcu_osci_stab_wait(RCU_IRC40K);

    /* configure and enable the FWDGT */
    /* enable write access to FWDGT_PSC and FWDGT_RLD registers.
    FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(625, FWDGT_PSC_DIV64);
    fwdgt_enable();
}

void Fwdgt_Free(void)
{
    fwdgt_counter_reload();
}


void Timer5_Base(void)
{
    /* ----------------------------------------------------------------------------
    TIMER2 Configuration: 
    TIMER2CLK = SystemCoreClock/7200 = 10KHz,the period is 1s(10000/10000 = 1s).
    ---------------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER5);

    /* deinit a TIMER */
    timer_deinit(TIMER5);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 71; /* 7200-1 */
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 19;//25KHz
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER5, &timer_initpara);

    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
    /* enable a TIMER */
    timer_enable(TIMER5);     
    
    nvic_irq_enable(TIMER5_IRQn, 0);
}


void TimerPwm_Config(void)
{
    /* -----------------------------------------------------------------------
    TIMER2 configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER2CLK = SystemCoreClock / 72 = 1MHz, the PWM frequency is 62.5Hz.

    TIMER2 channel0 duty cycle = (4000/ 16000)* 100  = 25%
    TIMER2 channel1 duty cycle = (8000/ 16000)* 100  = 50%
    TIMER2 channel2 duty cycle = (12000/ 16000)* 100 = 75%
    ----------------------------------------------------------------------- */
    rcu_periph_clock_enable(RCU_GPIOA);
    
    gpio_mode_set(CP_PWM_OUTPUT_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CP_PWM_OUTPUT_PIN);
    gpio_output_options_set(CP_PWM_OUTPUT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CP_PWM_OUTPUT_PIN);
    #if 0
    gpio_mode_set(LED_PWM_OUTPUT_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PWM_OUTPUT_PIN);
    gpio_output_options_set(LED_PWM_OUTPUT_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_PWM_OUTPUT_PIN);
    #endif

    gpio_af_set(CP_PWM_OUTPUT_PORT, GPIO_AF_2, CP_PWM_OUTPUT_PIN);
    #if 0
    gpio_af_set(LED_PWM_OUTPUT_PORT, GPIO_AF_2, LED_PWM_OUTPUT_PIN);
    #endif

    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);

    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;//1KHz PWM
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
    #if 0
    timer_channel_output_config(TIMER0, TIMER_CH_3, &timer_ocintpara);
    #endif


    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    #if 0
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
    #endif

    timer_primary_output_config(TIMER0, ENABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);
    #if 0
    /* enable the GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /*configure PA6(TIMER2 CH0) as alternate function*/
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_4);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_5);
    
    /* -----------------------------------------------------------------------
    TIMER2 configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER2CLK = SystemCoreClock / 72 = 1MHz, the PWM frequency is 62.5Hz.

    TIMER2 channel0 duty cycle = (4000/ 16000)* 100  = 25%
    TIMER2 channel1 duty cycle = (8000/ 16000)* 100  = 50%
    TIMER2 channel2 duty cycle = (12000/ 16000)* 100 = 75%
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* enable the TIMER clock */
    rcu_periph_clock_enable(RCU_TIMER2);
    /* deinit a TIMER */
    timer_deinit(TIMER2);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* configure TIMER channel output function */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER2, TIMER_CH_0, &timer_ocinitpara);
    timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocinitpara);
   
    /* CH0 configuration in PWM mode0, duty cycle 25% */
    timer_channel_output_mode_config(TIMER2, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    
    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* TIMER2 counter enable */
    timer_enable(TIMER2);
    #endif


    
}



void Adc_Init(void)
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
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_PULLDOWN, GPIO_PIN_4|GPIO_PIN_0|GPIO_PIN_3/*|GPIO_PIN_2*/
    |GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    /* config the GPIO as analog mode */
    gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_PULLDOWN, GPIO_PIN_0|GPIO_PIN_1);

    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1U);

    #if 1
    /* ADC contineous function enable */
    adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
    #endif

    /* ADC trigger config */
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger config */
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable();
    delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable();    
}


#if 0
void Uart0_Dma_Config(void)
{

    #define USART0_TDATA_ADDRESS      ((uint32_t)&USART_TDATA(USART0))  
    #define USART0_RDATA_ADDRESS      ((uint32_t)&USART_RDATA(USART0)) 
    #define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

    __IO FlagStatus g_transfer_complete = RESET;
    uint8_t rxbuffer[10];
    uint8_t txbuffer[] = "\n\rUSART DMA interrupt receive and transmit example, please input 10 bytes:\n\r";

    dma_parameter_struct dma_init_struct;

    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

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
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_enable(USART0);

    nvic_irq_enable(DMA_Channel1_2_IRQn, 0);

    /* initialize DMA channel1 */
    dma_deinit(DMA_CH1);

    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(txbuffer);
    dma_init_struct.periph_addr = USART0_TDATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH1, &dma_init_struct);

    /* initialize DMA channel2 */
    dma_deinit(DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 10;
    dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH2, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA_CH1);
    dma_memory_to_memory_disable(DMA_CH1);
    dma_circulation_disable(DMA_CH2);
    dma_memory_to_memory_disable(DMA_CH2);

    /* USART DMA enable for reception */
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);
    /* enable DMA channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA_CH2, DMA_INT_FTF);
    /* enable DMA channel2 */
    dma_channel_enable(DMA_CH2);
    /* USART DMA enable for transmission */
    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);
    /* enable DMA channel1 transfer complete interrupt */
    dma_interrupt_enable(DMA_CH1, DMA_INT_FTF);
    /* enable DMA channel1 */
    dma_channel_enable(DMA_CH1);

    /* test */
    /* waiting for the transfer to complete*/
    while(RESET == g_transfer_complete);

    g_transfer_complete = RESET;

    /* waiting for the transfer to complete*/
    while(RESET == g_transfer_complete);

    printf("\n\r%s\n\r", rxbuffer);
    while(1);
}
#endif












