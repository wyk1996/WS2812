#define FUNCFGEXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: RecoverCfg
* @function_file: FunCfg.c
* @描述: 恢复配置字
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-01-23)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RecoverCfg(void)
{
    /*SysE2ReadData(EEP_CFSAVECFG,&gs_FunCfg.uc_CfSaveCfg,sizeof(GSFUNCFG)-2);   //读取配置项
    if((gs_FunCfg.uc_PWOFFSHOWMDCfg!=0x09)
       &&(gs_FunCfg.uc_PWOFFSHOWMDCfg!=0x90))
    {
        gs_FunCfg.uc_PWOFFSHOWMDCfg=0;
    }    
    gs_FunCfg.ul_CRC=do_CRC(&gs_FunCfg.uc_CfSaveCfg,sizeof(GSFUNCFG)-2);       //计算配置项的crc
    SysE2ReadData(EEP_DBYXTZZ,(uint8*)&gs_MeterMode,1);                        //刷新电表运行特征字1
    SysE2ReadData(EEP_ZDSBMSZ,(uint8*)&gs_ReportWord,sizeof(REPORTWORD));      //刷新主动上报模式字
    */
}
/*=========================================================================================\n
* @function_name: Up_FunCfg
* @function_file: FunCfg.c
* @描述: 跟新配置寄存器
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Up_FunCfg(void)
{
//    uint8 i;
//    RecoverCfg();                                   //恢复配置
//    
//    //读取计算方式字
//    if((gs_FunCfg.uc_CfSaveCfg&0xf0)==0x50)         //需要重新计算
//    {
//        BE_ReadB(EEP_YGCALC,&guc_EPCalcType);       //刷新计量方式字   
//        //重新计算组合电量
//        //正向增量处理
//        for(i=0;i<5;i++)
//        {
//            gs_EnergyA.lCP[i]=0;
//            if(guc_EPCalcType&BIT0)
//            {
//                gs_EnergyA.lCP[i]+=gs_EnergyA.ulP[i];
//            }
//            else if(guc_EPCalcType&BIT1)
//            {
//                gs_EnergyA.lCP[i]-=gs_EnergyA.ulP[i];
//            }
//
//
//            //反向增量处理
//            if(guc_EPCalcType&BIT2)
//            {
//                gs_EnergyA.lCP[i] +=gs_EnergyA.ulN[i];
//            }
//            else if(guc_EPCalcType&BIT3)
//            {
//                gs_EnergyA.lCP[i] -=gs_EnergyA.ulN[i];
//            } 
//        }
//
//        gs_EnergyA.uiCRC = do_CRC((uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE) - 2);
//        if(Eny_CheckEnergyActiveRAM() == ENERGY_ERR_NONE)
//        {
//            BE_WriteP(EEP_EPS,(uint8*)&gs_EnergyA,sizeof(ENERGYACTIVE));
//        }
//        //这个判断加上,而不是沿用上面的判断是因为防错处理
//
//        if(Eny_CheckEnergyActiveRAM() == ENERGY_ERR_NONE)
//        {
//        #if (_SW_SE_Type > 0)
//            SE_WriteP(EEP_EPS,(uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
//        #else
//            BE_WriteP(EEP_EPS+EEP_BAK_LEN,(uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
//        #endif
//        }
//
//    }//不然 就对原来的电量不需要处理
//    BE_ReadB(EEP_YGCALC,&guc_EPCalcType);               //刷新计量方式字   
//    Eny_EnergyActiveSave();
 
}
/*=========================================================================================\n
* @function_name: CfgCheck
* @function_file: FunCfg.c
* @描述: 定时检测配置项是否正确
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-28)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CfgCheck(void)
{
   // if(gs_FunCfg.ul_CRC!=do_CRC(&gs_FunCfg.uc_CfSaveCfg,sizeof(GSFUNCFG)-2))       //配置项CRC不正确，则跟新配置项
  //  {
   //     Up_FunCfg();   //跟新配置项 
   // }

    if(gs_DispCont.ul_CRC!=do_CRC(&gs_DispCont.ucLxCnt,sizeof(S_DISPCONTENT)-2))
    {
        ShowInitial();
    }
}
