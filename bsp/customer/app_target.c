
#include "Includes.h"
#include "app_target.h"
#include "app_chargectl.h"

//��̬����������
static _e_target_work_mode s_target_work_mode;

//�豸����Ĭ��ֵ
static const _t_ac_charge_system_para _t_ac_charge_system_para_default = {
#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	276.0f,			//ŷ���ѹֵ
#else
	264.0f, 		//�����ѹֵ
#endif
#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	184.0f, 		//ŷ��Ƿѹֵ
#else
	176.0f, 		//����Ƿѹֵ
#endif

	AC_MAX_CURRENT, //������
	95, 			//����ֵ
	120				//���ֳ�ʱʱ�� 
};

//�����豸����ģʽ
void app_target_set_work_mode(_e_target_work_mode target_work_mode)
{
	s_target_work_mode = target_work_mode;
}

//��ȡ�豸����ģʽ
_e_target_work_mode app_target_get_work_mode(void)
{
	return s_target_work_mode;
}

//�豸������ʼ��
void app_target_sys_para_init(void)
{
	_t_ac_charge_system_para *ptr_ac_charge_system_para = app_chargectl_get_system_para();
	memcpy(ptr_ac_charge_system_para, &_t_ac_charge_system_para_default, sizeof(_t_ac_charge_system_para));
}


