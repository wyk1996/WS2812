
#include "Includes.h"
#include "app_target.h"
#include "app_chargectl.h"

//静态变量定义区
static _e_target_work_mode s_target_work_mode;

//设备参数默认值
static const _t_ac_charge_system_para _t_ac_charge_system_para_default = {
#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	276.0f,			//欧标过压值
#else
	264.0f, 		//国标过压值
#endif
#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	184.0f, 		//欧标欠压值
#else
	176.0f, 		//国标欠压值
#endif

	AC_MAX_CURRENT, //最大电流
	95, 			//过温值
	120				//握手超时时间 
};

//设置设备工作模式
void app_target_set_work_mode(_e_target_work_mode target_work_mode)
{
	s_target_work_mode = target_work_mode;
}

//获取设备工作模式
_e_target_work_mode app_target_get_work_mode(void)
{
	return s_target_work_mode;
}

//设备参数初始化
void app_target_sys_para_init(void)
{
	_t_ac_charge_system_para *ptr_ac_charge_system_para = app_chargectl_get_system_para();
	memcpy(ptr_ac_charge_system_para, &_t_ac_charge_system_para_default, sizeof(_t_ac_charge_system_para));
}


