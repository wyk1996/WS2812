
#ifndef __APP_CHARGECTL_H__
#define __APP_CHARGECTL_H__

#include "IndependentDataType.h"
#include "customer_config.h"
#include "gd32e23x.h"




typedef enum
{
	MAX_CURR_10 = 0,
	MAX_CURR_12,
	MAX_CURR_14,
	MAX_CURR_16,
	MAX_CURR_ERROR
} _e_oled_max_curr_type;



/// �˲�״̬ʧ�ܹ��϶���
typedef enum
{
	AC_CHARGE_ERR_OK = 0,
	AC_CHARGE_ERR_CNFAIL,				// δ��ȫ���Ӻ�
	AC_CHARGE_ERR_SEMICNFAIL,			// δ�����Ӻ�
	AC_CHARGE_ERR_EMERSTOPPUSH,			// ��ͣ��ť����
	AC_CHARGE_ERR_OVP,					// ��ѹ
	AC_CHARGE_ERR_OCP,					// ����
	AC_CHARGE_ERR_UVP,					// Ƿѹ
	AC_CHARGE_ERR_TEMPOVER,        		// ����
	AC_CHARGE_ERR_EARTHING,				// �ӵ�
	AC_CHARGE_ERR_HANDOVERTIME,     	// ���ֳ�ʱ
	AC_CHARGE_ERR_METER_COMMFAIL,		// ���ͨѶ����
	AC_CHARGE_ERR_LEAKCURR,				// ©��
	AC_CHARGE_ERR_SUSPENDOVERTIME,  	//��ͣ��糬��10min
	AC_CHARGE_ERR_METER_ENERGYFAULT,	// �������쳣	
	AC_CHARGE_ERR_MAX
}_e_ac_charge_err;

//���״̬
typedef enum
{
	AC_CHARGE_STATE_IDLE_GUN_NOLINK	= 0,		//ǹδ����
	AC_CHARGE_STATE_START_PREPARE,			    // ���׼���У���pwm���պ�����̵���-�ȴ�6V
	AC_CHARGE_STATE_CHARGING,			        // ����У�ѭ��ˢ��δ�����˵���¼
	AC_CHARGE_STATE_STOP_ACT,			        // ���ֹͣ�������Ͽ�����̵�����ˢ��δ�����˵���¼��ֹͣpwm
	AC_CHARGE_STATE_STOP_COMPLETE			    // ���ֹͣ��ɣ��ȴ���ǹ
}_e_ac_charge_state;

//cp״̬
typedef enum
{
	AC_CP_BREAK = 0,	//12V��δ����
	AC_CP_SEMICONNECT,	//9V��������
	AC_CP_CONNECTOK,	//6V��������
	#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	AC_CP_CONNECTOK_NEED_VENT,	//3V,�����ӣ�ŷ�꣬��Ҫͨ��
	#endif
	AC_CP_ERR			//����6/9/12V��Χ��
}_e_ac_cp_state;

//��������Ϣ
typedef union
{
	INT8U	u8data[2];
	INT16U	u16data;
	struct
	{
		INT16U over_voltage		:	1;			// ��ѹ����
		INT16U over_current		:	1;			// ��������
		INT16U under_voltage	:	1;			// Ƿѹ����
		INT16U earthing_fault	:	1;			// �ӵع���	
		INT16U env_temper_over	:	1;			// over temp	
		INT16U handshake_timeout	:	1;		// ���ֳ�ʱ,����S2�պ�ʱ��
		INT16U meter_comm_fault	:	1;			// ����ͨѶ����			
 		INT16U leak_curr_fault	:	1;	        // ©��������	
		INT16U suspend_over_10min_fault	:	1;	// ��ͣ��糬��10min����ʱ��Ϊ�������С��0.5AΪ��ͣ��磩 
		INT16U meter_energy_fault	:	1;	    // �������쳣
		INT16U cp_fault	:	1;	                // cp��ѹ�쳣������6/9/12V��Χ�ڣ�
		INT16U emergency_stop		:	1;
		INT16U card_comm_fault	:	1;		// rf��ͨѶ����	
		
		INT16U rsv		:	3;					//Ԥ��
	}bits;
}_u_ac_charge_err;

//�豸����
typedef  struct  
{
	FP32 volt_over;				//��ѹֵ
	FP32 volt_under;			//Ƿѹֵ
	INT16U curr_max;				//������
	INT8U env_temper_over;      //��������ֵ
	INT8U handshake_overtime;    //���ֳ�ʱʱ��
}_t_ac_charge_system_para;


//�����Ϣ
// ʹ�� pragma pack ȷ���ṹ����գ���ѡ������ƽ̨����
#pragma pack(push, 1)  // ��1�ֽڶ��룬�����������
typedef struct
{
    // ����1������Ա��С�������У�8�ֽ� > 4�ֽ� > 1�ֽڣ�
    // --- 8�ֽڳ�Ա ---
    INT64U start_time_ms;          // �����ʼʱ��
    INT64U prepare_start_time;     // ���׼����ʼʱ��
    INT64U detect_cp_9v_time;      // ���CP_9V��ѹ
    INT64U detect_cp_9v_to_6v_time;// ���CP_9V-6v��ѹ
    INT64U detect_cp_6v_time;      // ���CP_6V��ѹ
    INT64U s2_open_time;           // S2���ضϿ�ʱ��
    #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
    INT64U detect_cp_3v_time;      // ���CP_3V��ѹ���������룩
    #endif

    // --- 4�ֽڳ�Ա ---
    FP32 vol;                      // ����ѹ
    FP32 curr;                     // ������
    FP32 energy;                   // ������
    FP32 start_kwh;                // �����ʼ����
    FP32 kwh;                      // ������
    FP32 temperature;              // �����¶�

    // --- 4�ֽ�ö��/������ ---
    _e_ac_charge_state charge_state;
    _e_ac_cp_state cp_state;
    _u_ac_charge_err charge_err;

    // --- 1�ֽڱ�־λ���д�� ---
    INT8U detect_cp_9v_flag;        // CP_9V��־
    INT8U detect_cp_9v_to_6v_flag;  // CP_9V-6V��־
    INT8U detect_cp_6v_flag;        // CP_6V��־
    INT8U s2_open_flag;             // S2�Ͽ���־
    BOOLEAN charge_end;             // ��������־
    #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
    INT8U detect_cp_3v_flag;        // CP_3V��־���������룩
    INT8U need_vent_flag;           // ͨ���־���������룩
    #endif

    // --- �����ֶ��Ż� ---
    // ԭ duration_s ���� start_time_ms �͵�ǰʱ����㣬�ɿ����Ƴ�
    INT64U duration_s;           // ���ʱ������ע�ͣ����豣����
} _t_ac_charge_info;
#pragma pack(pop)  // �ָ�Ĭ�϶���


#if 0
//�����Ϣ
typedef struct
{
	_e_ac_charge_state	charge_state;
	_e_ac_cp_state	cp_state;
	_u_ac_charge_err	charge_err;
	
	FP32 vol;					//����ѹ
	FP32 curr;					//������
	FP32 energy;				//������
	INT64U start_time_ms;		//�����ʼʱ��
	INT64U duration_s;			//���ʱ��
	FP32 start_kwh;				//�����ʼ����
	FP32 kwh;					//������

	INT64U prepare_start_time;	//���׼����ʼʱ��
	FP32 temperature;    		//�����¶�
	INT64U detect_cp_9v_time;		//���CP_9V��ѹ
	INT8U detect_cp_9v_flag;		//���CP_9V��ѹ��־
	INT64U detect_cp_9v_to_6v_time;		//���CP_9V-6v��ѹ
	INT8U detect_cp_9v_to_6v_flag;		//���CP_9V-6v��ѹ��־
	INT64U detect_cp_6v_time;		//���CP_6V��ѹ
	INT8U detect_cp_6v_flag;		//���CP_6V��ѹ��־
	INT64U s2_open_time;		//��������S2���ضϿ�
	INT8U s2_open_flag;			//�����S2�Ͽ���־
	BOOLEAN charge_end;    		//�����ʽ����
	#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	INT64U detect_cp_3v_time;		//���CP_3V��ѹ
	INT8U detect_cp_3v_flag;		//���CP_3V��ѹ��־
	INT8U need_vent_flag;		//ŷ�����Ƿ���Ҫͨ��
	#endif
}_t_ac_charge_info;
#endif

//��������
_t_ac_charge_info* app_chargectl_get_charge_info(void);
_t_ac_charge_system_para* app_chargectl_get_system_para(void);

void app_charge_ctl_process(void);
BOOLEAN app_chargectl_startcharge(void);
BOOLEAN app_chargectl_stopcharge(void);
INT8U get_oled_keypad_curr_flashdata(void);
void app_charge_ctl_handle(void);

#endif

