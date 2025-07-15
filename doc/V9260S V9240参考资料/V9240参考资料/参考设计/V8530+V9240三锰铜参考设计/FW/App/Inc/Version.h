/*
*版本控制
*每添加一个版本请注明当前版本与上一版本不同之处,在哪几个源文件中有变动,具体指明
*
*/
#ifndef __VERSION_H__
#define __VERSION_H__


#define CHECK_RevPhase    1              //是否判断逆相序  0-不判断, 1-判断

#define SysMode_TEST      0              //是否处于测试模式下  0-否, 1-是

#define Rec_PwrDown       1              //是否能够掉相恢复  0-否, 1-是

#define Disp_ErrState     0              //是否显示错误状态  0-否, 1-显示错误码, 2-显示通信次数
//-----------------------------------------------------//
#define CF_TimeBase_1MS        1               //1MS 时基
#define CF_TimeBase_20MS       20              //20MS 时基

#define CF_TimeBase       CF_TimeBase_20MS      //CF脉宽对应定时器的时基
//-----------------------------------------------------//
#define Fmcu_3M2        0               //Fmcu = 3.2M
#define Fmcu_6M5        1               //Fmcu = 6.5M
#define Fmcu_13M        2               //Fmcu = 13M
#define Fmcu_26M        3               //Fmcu = 26M

#define Fmcu_Val        Fmcu_26M
//-----------------------------------------------------//

//表计类型
#define CONST_A_METER   0               //三相液晶表
#define CONST_B_METER   1               //单相表
#define CONST_C_METER   2


#define __T26_S01__                     //单相多费率表,浙江需求

/**表计选择和版本控制
版本定义
*/

#if defined(__T26_S01__)
    #define __METER_TYPE__ CONST_B_METER
#endif

#if (__METER_TYPE__ != CONST_B_METER)
    #error "VersionExt.h: system not define B meter type!!!"
#endif

#if (SysMode_TEST != 1)
    #define __VER_SOFT_B__  "M001.01A.XXX000101.11" //"S308.9240_18"      //当前软件版本
#else
    #define __VER_SOFT_B__  "T308.9240_18"      //当前软件版本
#endif

#define __VER_HARD_B__  0x01            //当前硬件版本

extern   uint8  gCuc_VerSoft[];
extern const  uint8  gCuc_VerHard[];


#endif

