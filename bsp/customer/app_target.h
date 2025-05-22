
#ifndef __APP_TARGET_H__
#define __APP_TARGET_H__

#include "customer_config.h"
#include "customer_device_type.h"
#include "IndependentDataType.h"

//����汾���� ��Ӳ���汾-����汾��
#define HARDWARE_VERSION_MAIN			0			//Ӳ�����汾��-�����
#define HARDWARE_VERSION_SUB			1
#define SOFTWARE_VERSION_MAIN			0			//������汾��-�����
#define SOFTWARE_VERSION_SUB			2

//�������崦
#if(CUSTOMER_MAX_CURRENT_SELECT==CUSTOMER_MAX_CURRENT_TYPE_8A)
	#define AC_MAX_CURRENT				8						//Maximum output current definition
#elif(CUSTOMER_MAX_CURRENT_SELECT==CUSTOMER_MAX_CURRENT_TYPE_13A)
	#define AC_MAX_CURRENT				13						//Maximum output current definition
#else
	#define AC_MAX_CURRENT				16						//Maximum output current definition
#endif

typedef enum
{
	WORK_MODE_OFFICIAL = 0,
	WORK_MODE_FACTORY,
	WORK_MODE_MAX
}_e_target_work_mode;

//��������
void app_target_set_work_mode(_e_target_work_mode target_work_mode);
_e_target_work_mode app_target_get_work_mode(void);
void app_target_sys_para_init(void);

#endif

