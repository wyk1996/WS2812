
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



/// 核查状态失败故障定义
typedef enum
{
	AC_CHARGE_ERR_OK = 0,
	AC_CHARGE_ERR_CNFAIL,				// 未完全连接好
	AC_CHARGE_ERR_SEMICNFAIL,			// 未半连接好
	AC_CHARGE_ERR_EMERSTOPPUSH,			// 急停按钮按下
	AC_CHARGE_ERR_OVP,					// 过压
	AC_CHARGE_ERR_OCP,					// 过流
	AC_CHARGE_ERR_UVP,					// 欠压
	AC_CHARGE_ERR_TEMPOVER,        		// 过温
	AC_CHARGE_ERR_EARTHING,				// 接地
	AC_CHARGE_ERR_HANDOVERTIME,     	// 握手超时
	AC_CHARGE_ERR_METER_COMMFAIL,		// 电表通讯故障
	AC_CHARGE_ERR_LEAKCURR,				// 漏电
	AC_CHARGE_ERR_SUSPENDOVERTIME,  	//暂停充电超过10min
	AC_CHARGE_ERR_METER_ENERGYFAULT,	// 电表读数异常	
	AC_CHARGE_ERR_MAX
}_e_ac_charge_err;

//充电状态
typedef enum
{
	AC_CHARGE_STATE_IDLE_GUN_NOLINK	= 0,		//枪未插入
	AC_CHARGE_STATE_START_PREPARE,			    // 充电准备中；发pwm、闭合输出继电器-等待6V
	AC_CHARGE_STATE_CHARGING,			        // 充电中；循环刷新未结算账单记录
	AC_CHARGE_STATE_STOP_ACT,			        // 充电停止动作；断开输出继电器、刷新未结算账单记录、停止pwm
	AC_CHARGE_STATE_STOP_COMPLETE			    // 充电停止完成；等待拔枪
}_e_ac_charge_state;

//cp状态
typedef enum
{
	AC_CP_BREAK = 0,	//12V，未连接
	AC_CP_SEMICONNECT,	//9V，半连接
	AC_CP_CONNECTOK,	//6V，已连接
	#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	AC_CP_CONNECTOK_NEED_VENT,	//3V,已连接，欧标，需要通风
	#endif
	AC_CP_ERR			//不在6/9/12V范围内
}_e_ac_cp_state;

//充电故障信息
typedef union
{
	INT8U	u8data[2];
	INT16U	u16data;
	struct
	{
		INT16U over_voltage		:	1;			// 过压保护
		INT16U over_current		:	1;			// 过流保护
		INT16U under_voltage	:	1;			// 欠压保护
		INT16U earthing_fault	:	1;			// 接地故障	
		INT16U env_temper_over	:	1;			// over temp	
		INT16U handshake_timeout	:	1;		// 握手超时,车端S2闭合时间
		INT16U meter_comm_fault	:	1;			// 计量通讯故障			
 		INT16U leak_curr_fault	:	1;	        // 漏电流故障	
		INT16U suspend_over_10min_fault	:	1;	// 暂停充电超过10min（暂时认为连续充电小于0.5A为暂停充电） 
		INT16U meter_energy_fault	:	1;	    // 电表读数异常
		INT16U cp_fault	:	1;	                // cp电压异常（不在6/9/12V范围内）
		INT16U emergency_stop		:	1;
		INT16U card_comm_fault	:	1;		// rf卡通讯故障	
		
		INT16U rsv		:	3;					//预留
	}bits;
}_u_ac_charge_err;

//设备参数
typedef  struct  
{
	FP32 volt_over;				//过压值
	FP32 volt_under;			//欠压值
	INT16U curr_max;				//最大电流
	INT8U env_temper_over;      //环境过温值
	INT8U handshake_overtime;    //握手超时时间
}_t_ac_charge_system_para;


//充电信息
// 使用 pragma pack 确保结构体紧凑（可选，根据平台需求）
#pragma pack(push, 1)  // 按1字节对齐，彻底消除填充
typedef struct
{
    // 规则1：按成员大小降序排列（8字节 > 4字节 > 1字节）
    // --- 8字节成员 ---
    INT64U start_time_ms;          // 充电起始时间
    INT64U prepare_start_time;     // 充电准备起始时间
    INT64U detect_cp_9v_time;      // 检测CP_9V电压
    INT64U detect_cp_9v_to_6v_time;// 检测CP_9V-6v电压
    INT64U detect_cp_6v_time;      // 检测CP_6V电压
    INT64U s2_open_time;           // S2开关断开时间
    #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
    INT64U detect_cp_3v_time;      // 检测CP_3V电压（条件编译）
    #endif

    // --- 4字节成员 ---
    FP32 vol;                      // 电表电压
    FP32 curr;                     // 电表电流
    FP32 energy;                   // 电表电量
    FP32 start_kwh;                // 充电起始电量
    FP32 kwh;                      // 充电电量
    FP32 temperature;              // 环境温度

    // --- 4字节枚举/错误码 ---
    _e_ac_charge_state charge_state;
    _e_ac_cp_state cp_state;
    _u_ac_charge_err charge_err;

    // --- 1字节标志位集中存放 ---
    INT8U detect_cp_9v_flag;        // CP_9V标志
    INT8U detect_cp_9v_to_6v_flag;  // CP_9V-6V标志
    INT8U detect_cp_6v_flag;        // CP_6V标志
    INT8U s2_open_flag;             // S2断开标志
    BOOLEAN charge_end;             // 充电结束标志
    #if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
    INT8U detect_cp_3v_flag;        // CP_3V标志（条件编译）
    INT8U need_vent_flag;           // 通风标志（条件编译）
    #endif

    // --- 冗余字段优化 ---
    // 原 duration_s 可由 start_time_ms 和当前时间计算，可考虑移除
    INT64U duration_s;           // 充电时长（已注释，按需保留）
} _t_ac_charge_info;
#pragma pack(pop)  // 恢复默认对齐


#if 0
//充电信息
typedef struct
{
	_e_ac_charge_state	charge_state;
	_e_ac_cp_state	cp_state;
	_u_ac_charge_err	charge_err;
	
	FP32 vol;					//电表电压
	FP32 curr;					//电表电流
	FP32 energy;				//电表电量
	INT64U start_time_ms;		//充电起始时间
	INT64U duration_s;			//充电时长
	FP32 start_kwh;				//充电起始电量
	FP32 kwh;					//充电电量

	INT64U prepare_start_time;	//充电准备起始时间
	FP32 temperature;    		//环境温度
	INT64U detect_cp_9v_time;		//检测CP_9V电压
	INT8U detect_cp_9v_flag;		//检测CP_9V电压标志
	INT64U detect_cp_9v_to_6v_time;		//检测CP_9V-6v电压
	INT8U detect_cp_9v_to_6v_flag;		//检测CP_9V-6v电压标志
	INT64U detect_cp_6v_time;		//检测CP_6V电压
	INT8U detect_cp_6v_flag;		//检测CP_6V电压标志
	INT64U s2_open_time;		//充电过程中S2开关断开
	INT8U s2_open_flag;			//充电中S2断开标志
	BOOLEAN charge_end;    		//充电正式结束
	#if(CUSTOMER_CHARGE_STANDARD_SELECT==CUSTOMER_CHARGE_STANDARD_OB)
	INT64U detect_cp_3v_time;		//检测CP_3V电压
	INT8U detect_cp_3v_flag;		//检测CP_3V电压标志
	INT8U need_vent_flag;		//欧标中是否需要通风
	#endif
}_t_ac_charge_info;
#endif

//函数声明
_t_ac_charge_info* app_chargectl_get_charge_info(void);
_t_ac_charge_system_para* app_chargectl_get_system_para(void);

void app_charge_ctl_process(void);
BOOLEAN app_chargectl_startcharge(void);
BOOLEAN app_chargectl_stopcharge(void);
INT8U get_oled_keypad_curr_flashdata(void);
void app_charge_ctl_handle(void);

#endif

