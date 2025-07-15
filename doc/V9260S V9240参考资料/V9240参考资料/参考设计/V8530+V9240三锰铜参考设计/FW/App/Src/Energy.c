#define AP_ENERGYEXT
#include "Include.h"

/*=========================================================================================\n
* @function_name: Eny_JbPm_GetE2
* @function_file: EnergyBottom.c
* @描述: 获取E2中的数据，同时进行相关的处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_JbPm_GetE2(void)
{
    uint8 ucjb[sizeof(S_JBPM)];

    BE_ReadP(EEP_JBTOTAL,ucjb,sizeof(S_JBPM));

    if(Uint8_To_Uint16(ucjb+sizeof(S_JBPM)-2)==do_CRC(ucjb,sizeof(S_JBPM)-2))
    {
        Eny_JbPm_UpdateRam(ucjb);
        EnyB_JbPm_Updata();
    }
    else
    {
    #if (_SW_SE_Type > 0)
        SE_ReadP(EEP_JBTOTAL,ucjb,sizeof(S_JBPM));
    #else
        BE_ReadP(EEP_JBTOTAL+EEP_BAK_LEN,ucjb,sizeof(S_JBPM));
    #endif
        if(Uint8_To_Uint16(ucjb+sizeof(S_JBPM)-2)==do_CRC(ucjb,sizeof(S_JBPM)-2))
        {
            Eny_JbPm_UpdateRam(ucjb);
            //PMU->IOADAT &=~ BIT7;////////////////////////////zzp0128
            EnyB_JbPm_Updata();
        }
        else
        {
            if(gs_JbPm.ui_JbCRC == do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2))  //RAM中的数据是否完整
            {                                           //因为以下参数要求定义在两个结构体里面
                EnyB_JbPm_Updata();
            }
            else
            {
                EnyB_JbPm_Init();       //1122
                EnyB_JbPm_Updata();
            }
        }
    }
}
                                                     
 
/*=========================================================================================\n
* @function_name: Eny_JbPm_UpdateRam
* @function_file: EnergyBottom.c
* @描述: 校表参数的更新，从E2中更新
* 
* 
* @参数: 
* @param:pjb  
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_JbPm_UpdateRam(uint8 *pjb)                  
{                                                       // 
    uint8 i;                                            //
    uint8 *p;                                           //
    p = (uint8 *)&gs_JbPm;                              //
    for(i=0;i<sizeof(S_JBPM);i++)                       //
    {                                                   //
        *p = * pjb;                                     //
        p++;                                            //
        pjb++;                                          //
    }                                                   //
} 
/*=========================================================================================\n
* @function_name: Eny_RefVar
* @function_file: EnergyBottom.c
* @描述: 从E2中读出校表参数，同时进行完整性校验，如果正确，则进行刷新，不正确则用初始化数据
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_RefVar(void)
{
    uint16 uitmp;
    uint16 uitmp2;
  //***************
    BE_ReadW(EEP_JBTOTAL+sizeof(S_JBPM)-2, &uitmp);//读E2中的CRC校验值
//    uitmp2 = uitmp<<16;
//    BE_ReadW(EEP_JBTOTAL+sizeof(S_JBPM)-4, &uitmp);
//    uitmp = uitmp&0x0000ffff;
//    uitmp2 += uitmp;
  //**************************zzp1201修改，之前BE_ReadW(EEP_JBTOTAL+sizeof(S_JBPM)-2, &uitmp);  
    //gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);//zzp1201

    if(uitmp == gs_JbPm.ui_JbCRC)                                 //判断他们E2中的数据是否与RAM中的相等
    {                                                            //RAM中的数据与E2中的是一样的
        if(gs_JbPm.ui_JbCRC == do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2))//RAM中的数据是否完整
        {                                               //因为以下参数要求定义在两个结构体里面
            EnyB_JbPm_Updata();
        }
        else
        {
            Eny_JbPm_GetE2();   // RAM中的数据要不通不过CRC校验，要不与E2中CRC对不起来
        }
    }
    else
    {
        Eny_JbPm_GetE2();      //RAM中的数据要不通不过CRC校验，要不与E2中CRC对不起来
        
    }
    //IWDG_ReloadCounter();    
    Eny_JbPm_MeterConst();                                                     //表常数及其的更新
} 
/*=========================================================================================\n
* @function_name: Eny_RefCalcType
* @function_file: Energy.c
* @描述: 刷新计量方式字
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_RefCalcType(void)
{
    BE_ReadB(EEP_YGCALC,&guc_EPCalcType);               //刷新计量方式字    
}
/*=========================================================================================\n
* @function_name: Eny_JbPm_MeterConst
* @function_file: EnergyBottom.c
* @描述: 脉冲常数
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_JbPm_MeterConst(void)                           
{                                                        
    uint8 i;
    for(i=0;i<MeterConstLen;i++)                                // 根据脉冲才常数实现走字控制
    {                                                           // 根据表常数，确定CF脉冲分频系数和单位能量大小
        if(gs_MeterConstT[i].ui_MC==gs_JbPm.ui_MeterC)
        {                                                       // 根据脉冲常数来确定CF分频系数和单位能量大小
            guc_GateCF = gs_MeterConstT[i].uc_CFGate;           // 出的脉冲个数
            guc_UnitCF = gs_MeterConstT[i].uc_CFUinit;          // 单位大小
            break;
        }
    }

    if(i>=MeterConstLen)                                        // 查找表格中，发现没有对应的常数的处理
    {                                                           // 如果表格中没有对应项，则设置状态，报警
        guc_GateCF = 10;                                        // 用一个比较大的数据，容易看出
        guc_UnitCF = 1;
    }
}
 
/*=========================================================================================\n
* @function_name: Eny_EnergyClrCur
* @function_file: Energy.c
* @描述: 清除当前正向有功电量
*        清除RAM中的数据，就是结构体中的数据，最大是2kWh 
*        清除E2中的数据，是当前正向有功总电量数据
* @参数: 
* @param:ucType  
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_EnergyClrCur(uint8 ucType,uint8 power)
{                       
   uint16 *p,*q;
   uint32 *r;
    if(power == active)
    {
        p = (uint16*)&gs_EnergyData;
        q = (uint16*)&gs_EnergyData_bak;
        r = (uint32*)&gs_EnergyA;
    }
    else
    {
        p = (uint16*)&gs_NEnergyData;
        q = (uint16*)&gs_NEnergyData_bak;
        r = (uint32*)&gs_NEnergyA;
    }

    if(ucType & ENERGY_CLR_RAM)
    {
        memset((uint8*)p, 0, sizeof(ENERGYDATA)-2);    //电量增量
        p[40] = do_CRC((uint8 *)p, sizeof(ENERGYDATA)-2);
        memcpy((uint8*)q, (uint8*)p, sizeof(ENERGYDATA));
    }

    if(ucType & ENERGY_CLR_E2)
    {
        memset((uint8*)r, 0, sizeof(ENERGYACTIVE));
        r[21] = do_CRC((uint8*)r, sizeof(ENERGYACTIVE)- 2);
        BE_WriteP(EEP_EPS,(uint8*)r, sizeof(ENERGYACTIVE));

    #if (_SW_SE_Type > 0)
        SE_WriteP(EEP_EPS,(uint8*)&r, sizeof(ENERGYACTIVE));
    #else
        BE_WriteP(EEP_EPS+EEP_BAK_LEN,(uint8*)r, sizeof(ENERGYACTIVE));
    #endif
    }
}
 
/*=========================================================================================\n
* @function_name: Eny_PurCheckRAMEny
* @function_file: Energy.c
* @描述: 上电检测RAM电量是否正确
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_PurCheckRAMEny(void)
{
    ENERGYACTIVE sEA;
    if(ENERGY_ERR_RAM==Eny_CheckEnergyActiveRAM(active))
    {                                                                       //RAM电量不正确
        BE_ReadP(EEP_EPS,(uint8*)&sEA,sizeof(ENERGYACTIVE));                //读取E2中的电量
        if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE)- 2) != sEA.uiCRC)      //检查E2中电量CRC是否正确
        {                                                                   //主E2电量不正确
        #if (_SW_SE_Type > 0)
            SE_ReadP(EEP_EPS+EEP_BAK_LEN,(uint8*)&sEA,sizeof(ENERGYACTIVE));
        #else
            BE_ReadP(EEP_EPS+EEP_BAK_LEN,(uint8*)&sEA,sizeof(ENERGYACTIVE)); //读取E2中的电量
        #endif 
            if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE) - 2) == sEA.uiCRC)
            {                                                               //备份E2电量正确
                memcpy((uint8*)&gs_EnergyA, (uint8*)&sEA, sizeof(ENERGYACTIVE));            //拷贝E2备份电量到RAM中
            }
        }
        else
        {
            memcpy((uint8*)&gs_EnergyA, (uint8*)&sEA, sizeof(ENERGYACTIVE));                //拷贝主E2电量到RAM中
        }
    }
}

/*=========================================================================================\n
* @function_name: Eny_CheckEnergy
* @function_file: Energy.c
* @描述: 检查结构体中的增量是否正确
*       电量增量crc校验出错次数标记,0必定主校验正确,1主校验错而备份对,2都错
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_CheckEnergy(uint8 power)
{
    uint8   ucErr = 0;
    uint16  uiCRC;
    uint16 *p,*q;
    if(power == active)
    {
        p =(uint16*)& gs_EnergyData;
        q = (uint16*)&gs_EnergyData_bak;
    }
    else
    {
        p = (uint16*)&gs_NEnergyData;
        q = (uint16*)&gs_NEnergyData_bak;
    }
    uiCRC = do_CRC((uint8 *)p, sizeof(ENERGYDATA)-2);          // 电量增量CRC校验
    if (uiCRC != p[40])                                       
    {                                                                       
        ucErr++;                                                            
        uiCRC = do_CRC((uint8 *)q, sizeof(ENERGYDATA)-2);  // 备份电量增量CRC校验
        if (uiCRC != q[40])                               
        {                                                                   
            ucErr++;                                                        
        }                                                                   
    }                                                                       

    if (ucErr & 1)                                                          
    {                                                                       // 第一块出错
        memcpy((uint8*)p, (uint8*)q, sizeof(ENERGYDATA));     // 更新数据
    }
    else if(ucErr >= 2)                                                     // 主区域和备份区域都出错
    {                                                                       // 都出错                                                               
        Eny_EnergyClrCur(ENERGY_CLR_RAM,power);                                       // 清零
    }                                                                       
}
 
/*=========================================================================================\n
* @function_name: Eny_CheckEnergyActiveRAM
* @function_file: Energy.c
* @描述: 检查RAM电量是否正确
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 Eny_CheckEnergyActiveRAM(uint8 power)
{
    //验证数据CRC校验
    uint32 *p;
    if(power == active)
    {
        p = (uint32*)&gs_EnergyA;
    }
    else
    {
        p = (uint32*)&gs_NEnergyA;
    }
    if(do_CRC((uint8*)p, sizeof(ENERGYACTIVE)- 2) != p[21])
    {
        return ENERGY_ERR_RAM;
    }
    
    return ENERGY_ERR_NONE;
}
 
/*=========================================================================================\n
* @function_name: Eny_GetEp1
* @function_file: Energy.c
* @描述: 计算当前正、反有功总电能增量（分费率)
* 
* 
* @参数: 
* @param:index  0-组合（计量状态字决定）, 1-原正, 2-反
* @param:fee   计费的费率
* 
* @返回: 
* @return: int16 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  hjj 
* @修改内容: 添加三相表相位参数phyadd
===========================================================================================*/
int16 Eny_GetEp1(uint8 index, uint8 fee,uint8 phyadd,uint8 power)                 
{
    int16 iRet=0;
    uint16 *pEyP;
    uint16 *pEyN;

//   if((gs_FunCfg.uc_CfSaveCfg&0x0f)!=0x05)                             //CF脉冲分费率计数
//    {
//        if((fee != 0) && ((fee - 1) != gs_FeeData.uc_FeeNo))            //如果传入的费率参数不等于当前费率或者不等于总电量的话返回0
//        {
//            return 0;
//        }
//    }
    if(power == active)
    {
        Eny_CheckEnergy(active);                                                  //检查电量增量有效性，是否通的过CRC校验
        switch(phyadd)
            {
                case APhy:
                    pEyP=&gs_EnergyData.uiEPZ[phyadd];           //正向总脉冲
                    pEyN=&gs_EnergyData.uiENZ[phyadd];           //反向总脉冲
                    break;
                case BPhy:
                    pEyP=&gs_EnergyData.uiEPZ[phyadd>>2];           //正向总脉冲
                    pEyN=&gs_EnergyData.uiENZ[phyadd>>2];           //反向总脉冲
                    break;
                case CPhy:
                    pEyP=&gs_EnergyData.uiEPZ[phyadd>>2];           //正向总脉冲
                    pEyN=&gs_EnergyData.uiENZ[phyadd>>2];           //反向总脉冲
                    break;
                case ABCPhy:
                default:
                   /* if((gs_FunCfg.uc_CfSaveCfg&0x0f)==0x05)                            //CF脉冲分费率计数
                    {
                        if(fee == 0)
                        {
                            pEyP=&gs_EnergyData.uiEPZ[phyadd];           //正向总脉冲
                            pEyN=&gs_EnergyData.uiENZ[phyadd];           //反向总脉冲
                        }
                        else
                        {
                            pEyP=(&gs_EnergyData.uiEP1[phyadd] + (fee - 1));
                            pEyN=(&gs_EnergyData.uiEN1[phyadd] + (fee - 1));
                        }       
                    }*/
                   // else
                    {
                        pEyP=&gs_EnergyData.uiEPZ[phyadd];           //正向总脉冲
                        pEyN=&gs_EnergyData.uiENZ[phyadd];           //反向总脉冲
                    }
                    break;
            }

    }
    else
    {
        Eny_CheckEnergy(reactive);                                                  //检查电量增量有效性，是否通的过CRC校验
        switch(phyadd)
            {
                case APhy:
                    pEyP=&gs_NEnergyData.uiEPZ[phyadd];           //正向总脉冲
                    pEyN=&gs_NEnergyData.uiENZ[phyadd];           //反向总脉冲
                    break;
                case BPhy:
                    pEyP=&gs_NEnergyData.uiEPZ[phyadd>>2];           //正向总脉冲
                    pEyN=&gs_NEnergyData.uiENZ[phyadd>>2];           //反向总脉冲
                    break;
                case CPhy:
                    pEyP=&gs_NEnergyData.uiEPZ[phyadd>>2];           //正向总脉冲
                    pEyN=&gs_NEnergyData.uiENZ[phyadd>>2];           //反向总脉冲
                    break;
                case ABCPhy:
                default:
                    {
                        pEyP=&gs_NEnergyData.uiEPZ[phyadd];           //正向总脉冲
                        pEyN=&gs_NEnergyData.uiENZ[phyadd];           //反向总脉冲
                    }
                    break;
            }
    }
    
    if(index==1)
    {
        iRet = *pEyP;                               //返回正向电量增量
    }else if(index==0)
    {   
                                                    //下面是正向有功按照计量方式字来处理
        //正向增量处理
        if(guc_EPCalcType&BIT0)
        {
            iRet = *pEyP;
        }
        else if(guc_EPCalcType&BIT1)
        {
            iRet -= *pEyP;
        }
        //反向增量处理
        if(guc_EPCalcType&BIT2)
        {
            iRet += *pEyN;
        }
        else if(guc_EPCalcType&BIT3)
        {
            iRet -= *pEyN;
        }
                       
    }else if(index==2)
    {
        iRet =*pEyN;                                             //返回反向电流增量
    }
    return iRet;
}
 
/*=========================================================================================\n
* @function_name: Eny_EnergyActiveSave
* @function_file: Energy.c
* @描述: 当前电量写入E2中
*           这里还存在一个问题,就是数据分费率和与总电量不相等的话,将不再写数据到E2中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_EnergyActiveSave(uint8 power)
{
    uint8 ucFlg = 0;
    //uint8 i = 0;
    //int16 iDelta;
    uint32 *p;
    uint16 *q;
    uint32 e2_addr,e2_addr_bak;
    ENERGYACTIVE sEA;
    if(power == active)
    {
        e2_addr = EEP_EPS;
        e2_addr_bak = EEP_EPS + EEP_BAK_LEN;
        p = (uint32*)&gs_EnergyA;
        q = (uint16*)&gs_EnergyData;
    }
    else
    {
        e2_addr = EEP_NEPS;
        e2_addr_bak = EEP_EPS + EEP_BAK_LEN + 100;
        p = (uint32*)&gs_NEnergyA;
        q = (uint16*)&gs_NEnergyData;
    }
    //先验证E2数据CRC是否正确,如果有一个正确,则认为这部分内容有效
    BE_ReadP(e2_addr,(uint8*)&sEA,sizeof(ENERGYACTIVE));

    if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE)- 2) != sEA.uiCRC)
    {
    #if (_SW_SE_Type > 0)
        SE_ReadP(e2_addr, (uint8*)&sEA, sizeof(ENERGYACTIVE));
    #else
        BE_ReadP(e2_addr_bak, (uint8*)&sEA, sizeof(ENERGYACTIVE));
    #endif 
 
        if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE) - 2) == sEA.uiCRC)
        {
            ucFlg = 1;
        }
    }
    else
    {
        ucFlg = 1;
    }
    
    //如果E2数据CRC有效,则用E2数据覆盖RAM数据,当E2数据CRC不正确时,RAM数据CRC正确,那么相信RAM数据
    //如果都不正确,都将会用主E2的数据覆盖RAM数据
    if(ucFlg == 0)
    {
        if(do_CRC ((uint8*)p, sizeof(ENERGYACTIVE) - 2) != p[21])
        {
            //如果电量都不正确，则都不处理，直接返回。
            return;
        }
    }

    if(ucFlg == 1)
    {
        memcpy((void*)p, (void*)&sEA, sizeof(ENERGYACTIVE));
    }



   /* if((gs_FunCfg.uc_CfSaveCfg&0x0f)==0x05)                            //CF脉冲分费率计数
    {
        for(i=0; i<5; i++)
        {
            iDelta = Eny_GetEp1(1, i);
            gs_EnergyA.ulP[i] += iDelta; 
            iDelta = Eny_GetEp1(0, i);
            gs_EnergyA.lCP[i] += iDelta; 
            iDelta = Eny_GetEp1(2, i);
            gs_EnergyA.ulN[i] += iDelta; 
        } 
    }*///else
    {
          //正向有功
       // iDelta = Eny_GetEp1(1, 0);
       // gs_EnergyA.ulP[0] += iDelta;
       // gs_EnergyA.ulP[gs_FeeData.uc_FeeNo+1] += iDelta;
       p[15]   += q[0];
       p[16]   += q[1];
       p[17]   += q[2];
       p[5]     += q[3];
        //组合有功
       // iDelta = Eny_GetEp1(0, 0);
       // gs_EnergyA.lCP[0] += iDelta;
       // gs_EnergyA.lCP[gs_FeeData.uc_FeeNo+1] += iDelta;
        //反向有功
       // iDelta = Eny_GetEp1(2, 0);
       // gs_EnergyA.ulN[0] += iDelta;
       // gs_EnergyA.ulN[gs_FeeData.uc_FeeNo+1] += iDelta;     
    }

    //计算CRC
    p[21] = do_CRC((uint8*)p, sizeof(ENERGYACTIVE) - 2);
    //需要RAM电量合法才能写E2
    if(Eny_CheckEnergyActiveRAM(power) == ENERGY_ERR_NONE)
    {
        BE_WriteP(e2_addr,(uint8*)p,sizeof(ENERGYACTIVE));
    }
    else
    {
        return;
    }
    //这个判断加上,而不是沿用上面的判断是因为防错处理
    if(Eny_CheckEnergyActiveRAM(power) == ENERGY_ERR_NONE)
    {
        Eny_EnergyClrCur(ENERGY_CLR_RAM,power);
        
    #if (_SW_SE_Type > 0)
        SE_WriteP(e2_addr,(uint8*)p, sizeof(ENERGYACTIVE));
    #else
        BE_WriteP(e2_addr_bak,(uint8*)p, sizeof(ENERGYACTIVE));
    #endif
    }
}
 
/*=========================================================================================\n
* @function_name: Eny_EnergyProc
* @function_file: Energy.c
* @描述: 电量处理，接收到底层能量数据后，转存入RAM，同时增加CRC校验
*       同时判别是否需要把RAM中的电量转存入E2中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Eny_EnergyProc(uint8 power)
{
    uint8   i;                                           
    uint8   ucDelta;  
    uint8*  p;                                          // 合相有功能量的指针
    uint16*  p1;
    uint16*  p2;
    uint8 ucSaveflg;
                                             
    ucSaveflg=0;                                                    
    Eny_CheckEnergy(power);                                  // 检查电量有效性
    if(power == active)
    {
        p = (uint8*)&gs_Energy;                             // 能量数据;
        p1 = (uint16*)&gs_EnergyData;
    }
    else
    {
        p = (uint8*)&gs_NEnergy;                             // 能量数据;
        p1 = (uint16*)&gs_NEnergyData;
    }                       
    for (i=0; i<sizeof(S_ENERGY); i++,p++,p1++)
    {                                                   // 检查有功脉冲标志（电量达到MAX_E，向EEPROM中写一次）
        if ( *p > 0)                                    
        {                                               // 有电量脉冲
            __disable_irq();                      // 数据保护
            ucDelta = *p;                               // 把中断中产生的脉冲移动到RAM增量寄存器中
            *p = 0;                                     
            __enable_irq();                           
            *p1 += ucDelta;                             // 将中断产生的能量数据累加到RAM中
        }                                               
                                                        
        if (*p1 > MAX_E)                                // RAM中的增量数据为无效数据
        {                                               
            Eny_CheckEnergy(power);                              
        }                                               

        if (*p1 > SAVEVALUE)                            // 判别是否需要写入
        { 
            ucSaveflg=0x55;                                              
        }                                               
    }                                                   
    if(power == active)
    {
        p2 = (uint16*)&gs_EnergyData_bak;                             // 能量数据;
        p1 = (uint16*)&gs_EnergyData;
    }
    else
    {
        p2 = (uint16*)&gs_NEnergyData_bak;                             // 能量数据;
        p1 = (uint16*)&gs_NEnergyData;
    }
    p1[40] = do_CRC((uint8 *)p1, sizeof(ENERGYDATA)-2);  // 电量增量CRC校验
    memcpy((void*)p2, (void*)p1, sizeof(ENERGYDATA));
    if(ucSaveflg==0x55)
    {
        Eny_EnergyActiveSave(power);
    }
}
/*=========================================================================================\n
* @function_name: WriteHisEP
* @function_file: Energy.c
* @描述: 写入历史电量
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void WriteHisEP(void)
{
    if(Eny_CheckEnergyActiveRAM(active) != ENERGY_ERR_NONE)
    {
        BE_ReadP(EEP_EPS, (uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
        if(Eny_CheckEnergyActiveRAM(active) != ENERGY_ERR_NONE)
        {
        #if (_SW_SE_Type > 0)
            SE_ReadP(EEP_EPS, (uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
        #else
            BE_ReadP(EEP_EPS+EEP_BAK_LEN, (uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
        #endif
        }
    }
    DataProcWriteRound2(R2_MOVE,LSDLID,(uint8*)&gs_EnergyA);    //写入历史电量
}

/*=========================================================================================\n
* @function_name: MonthLYDL
* @function_file: Energy.c
* @描述: 月度组合电量过月
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-03-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void MonthLYDL(uint8 uc_Save)
{
    uint32 ulData;
    uint8  ucPt;
    int16 iDelta[2];
    Word32 JSSJTime;
    
    BE_ReadB(EEP_SYZHLYDL_PT, (uint8*)&ucPt);       //指针
    if(ucPt == 0)
    {
        ucPt = 1;
    }
    else
    {
        ucPt--;
    }

    iDelta[0] = Eny_GetEp1(1, 0,ABCPhy,active);                   //当前正向电量增量
    iDelta[1] = Eny_GetEp1(2, 0,ABCPhy,active);                   //当前反向电量增量
    ulData=gs_EnergyA.ulP[0]+gs_EnergyA.ulN[0]+iDelta[0]+iDelta[1];
    BE_WriteP(EEP_SYZHLYDLADDR+ucPt*4,(uint8*)&ulData,4);
    BE_WriteP(EEP_SYZHLYDL_PT, (uint8*)&ucPt,1);    //写入指针数据  
   
    if(uc_Save == 1)
    {
        JSSJTime.byte[0]=gs_DateTime.ucHour;
        JSSJTime.byte[1]=gs_DateTime.ucDay;
        JSSJTime.byte[2]=gs_DateTime.ucMonth;
        JSSJTime.byte[3]=gs_DateTime.ucYear;
        Eny_EnergyActiveSave(active);
        WriteHisEP();
        SaveJSTime(JSSJTime.lword);
    }
}

/*=========================================================================================\n
* @function_name: EnergyCBRManage
* @function_file: Energy.c
* @描述: 抄表日处理,一小时调用本函数判断抄表日时间一次
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnergyCBRManage(void)
{
    uint8 i,j;
    Word32 JSSJTime;
    Word16 W16_JSR[3];

    BE_ReadP(EEP_CBR1, W16_JSR[0].byte,6);              // 得到抄表日的日期和小时
                                                    
    for(i=0;i<2;i++)                                    //去除相同的抄表日
    {
        for(j=i+1;j<3;j++)
        {
            if(W16_JSR[i].word==W16_JSR[j].word)
            {
                W16_JSR[j].word=0x9999;
            }
        }
    }


    for(i=0;i<3;i++)
    {
        if(W16_JSR[i].word==0x9999)
        {
            continue;
        }
        //需要需要检测结算日是否合法

        if(W16_JSR[i].byte[0] == gs_DateTime.ucHour && W16_JSR[i].byte[1]  == gs_DateTime.ucDay)
        {
            if(i==0)                                            //过第一个结算日，存组合有功总累用电量
            {
                MonthLYDL(1);
            }
            else
            {
            //抄表日时间到,开始数据转存
                JSSJTime.byte[0]=W16_JSR[i].byte[0];
                JSSJTime.byte[1]=W16_JSR[i].byte[1];
                JSSJTime.byte[2]=gs_DateTime.ucMonth;
                JSSJTime.byte[3]=gs_DateTime.ucYear;
                Eny_EnergyActiveSave(active);
                WriteHisEP();
                SaveJSTime(JSSJTime.lword);
            }
            break;                                  //同一个时刻不可能转存好几次           
        }
    }
}
/*=========================================================================================\n
* @function_name: SaveJSTime
* @function_file: Energy.c
* @描述: 保存结算时间
* 
* 
* @参数: 
* @param:time  
* @返回: 
* @作者:   xuqf (2013-1-5)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SaveJSTime(uint32 time)
{
    DataProcWriteRound2(R2_MOVE,JSSJJLID,(uint8*)&time);
}
