#ifndef __FUNCFG_H__
#define __FUNCFG_H__

#ifndef FUNCFGEXT
    #define FUNCFGEXT extern
#endif

typedef struct
{
    uint8   uc_CfSaveCfg;          //CF保存配置项（自扩）
    uint8   uc_CbrCfg;             //抄表日补冻配置项（自扩）
    uint8   uc_DJCfg;              //冻结补冻配置项（自扩）
    uint16  uc_PWLCfg;             //密码等级有效配置项（自扩）
    uint8   uc_SQSDCfg;            //时区和时段切换时间设置判别配置项（自扩）
    uint8   uc_ENSHOWCfg;          //电量显示格式配置项（自扩）
    uint8   uc_TMSHOWCfg;          //时间显示格式配置项（自扩）
    uint8   uc_PWOFFSHOWMDCfg;     //停电显示模式配置项（自扩）
    uint8   uc_JDQCKCfg;           //继电器检测方式配置项（自扩）
    uint8   uc_ClrMeterCntCfg;     //电表清零次数（自扩）
    uint32  ul_ClrMeterEnyValCfg;  //电表清零电量阀值（自扩）
    uint16  ul_CRC;                //配置项crc
}GSFUNCFG;


FUNCFGEXT GSFUNCFG   gs_FunCfg;


void RecoverCfg(void);
void Up_FunCfg(void);
void CfgCheck(void);

#define _SQ_
#define _JR_
#define _ZXR_     //农网无周休日
 
 





#endif
