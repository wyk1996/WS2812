#define  ENERGYBOTTOMEXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: EnyB_Open_Cf
* @function_file: EnergyBottom.c
* @描述: 开启CF脉冲
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_Open_Cf(void)
{   

    if(guc_OpenCfTmCnt<3)
    {
        guc_OpenCfTmCnt++;
    }else
    {
        guc_ABCreadbegian =1;
       if( guc_CfOpenFlg==true)
        {
            EnyB_SetMeterCfgACK(0xf8,PMCtrl4);  //打开计量,保持潜动开启
            EnyB_InitCFInt();                   //开启CF脉冲中断
            guc_CfOpenFlg = false;
        }
        //EnyB_SetMeterCfgACK( 0xA4A04D,0x10ed);
        //EnyB_SetMeterCfgACK( 0x7B7839C,0x10ed);
    }
}
/*========================================================
* @function_name: EnyB_InitCFInt
* @function_file: EnergyBottom.c
* @描述: 初始化CF中断
* 
* @作者: lwb (2012/3/3)
* @备注: 
*----------------------------------------------------------
* @修改人: 
* @修改内容:  
============================================================*/
void EnyB_InitCFInt(void)
{
    EIE|=BIT0;
    ExInt2IE|=BIT7;
    P9OE&=(~BIT6);                          //打开CF IO输出
    P9FC|=(BIT6);

    EIE|=BIT1;
    ExInt3IE|=BIT7;
    P9OE&=(~BIT5);                          //打开CF2 IO输出
    P9FC|=(BIT5);

}
/*=========================================================================================\n
* @function_name: EnyB_SetMeterRule
* @function_file: EnergyBottom.c
* @描述: 设置计量模块流程
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_SetMeterRule(void)
{
    ADCInit();              //初始化ADC，设置增益和通道
    PMDspInit();            //PM DSP init，设置计量值，防止乱出脉冲
    EnyB_SetMeterCfgACK(0x58C745,0x10F5);   //E1潜动
    EnyB_SetMeterCfgACK(0x58C745,0x10FB);   //E2潜动
    EnyB_SetMeterCfgACK(0,0x10e8);  //E1比差
    EnyB_SetMeterCfgACK(0,0x10e9);  //E2比差
    EnyB_PurInit();         //计量模块上电初始化
    
}
/*=========================================================================================\n
* @function_name: EnyB_PurInit
* @function_file: EnergyBottom.c
* @描述: 上电初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_PurInit(void)                                 
{
    //CLRWDT();                         //喂狗
//    BroadcastWriteRaccoon(0xE0038002,RegMTPARA0);
    CLRWDT(); 
//    BroadcastWriteRaccoon(0x01FF0008,RegSysCtrl);
//    BroadcastWriteRaccoon(0x2C064000,SysCtrl);  //广播写系统配置寄存器 U: x1  IA: x32  IB: x32  +50 PPM
    CLRWDT(); 
    //BroadcastWriteRaccoon(0x01FF0008,RegSysCtrl);       //广播写系统配置寄存器
   //CLRWDT(); 
   // BroadcastWriteRaccoon(0x01FF0008,RegSysCtrl);       //广播写系统配置寄存器
   // CLRWDT(); 
    Init_Uart4(3);
    CLRWDT();                          //喂狗
    Eny_RefVar();                                       //计量参数刷新
    CLRWDT();                          //喂狗
    Eny_RefCalcType();                                  //计量方式字刷新
    CLRWDT();                         //喂狗
    Eny_PurCheckRAMEny();                               //检测RAM电量
    CLRWDT();                          //喂狗
}
extern const uint8 code InitPara5[sizeof(S_JBPM)-2];
/*=========================================================================================\n
* @function_name: EnyB_JbPm_Init
* @function_file: EnergyBottom.c
* @描述: 校表参数的初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_JbPm_Init(void)                                
{                                                        
    FCpyTMem((uint8*)&gs_JbPm.ui_MeterC,InitPara5,sizeof(S_JBPM)-2);  //拷贝Flash数据到RAM中
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
//  V9281_InitDefaultPara();
}

/*=========================================================================================\n
* @function_name: EnyB_IntCF
* @function_file: EnergyBottom.c
* @描述: 脉冲中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_IntCF(uint8 phadd,uint8 power)                       
{
    uint8 *pCfZ;
    uint8 *pEyZ;
    if(power == active)
    {
        if(gs_MeterState.ui_MeterState2.RunCode2Word.PADIR)      //判断是否是反向
        {
            pEyZ=&gs_Energy.ucNz[phadd];
            pCfZ=&gs_PowerCf.uc_Nz[phadd];
        }else
        {
            pEyZ=&gs_Energy.ucPz[phadd];                                   //正向能量
            pCfZ=&gs_PowerCf.uc_Pz[phadd];                                 //正向脉冲数
        }


        (*pCfZ)++;                                                  //总能量累加
        if(*pCfZ>=guc_GateCF)                                       //判别是否需要走字
        {
            *pCfZ -=guc_GateCF;                                     //对CF脉冲进行分频，将分频结果进行能量累加
            *pEyZ +=guc_UnitCF; 
            gui_SystemEvent |= flgEtGlb_EnergyCalc;                 //设置能量处理事件
        }
    }
    else
    {
         if(gs_MeterState.ui_MeterState2.RunCode2Word.PADIR)      //判断是否是反向
        {
            pEyZ=&gs_NEnergy.ucNz[phadd];
            pCfZ=&gs_PowerCfN.uc_Nz[phadd];
        }else
        {
            pEyZ=&gs_NEnergy.ucPz[phadd];                                   //正向能量
            pCfZ=&gs_PowerCfN.uc_Pz[phadd];                                 //正向脉冲数
        }


        (*pCfZ)++;                                                  //总能量累加
        if(*pCfZ>=guc_GateCF)                                       //判别是否需要走字
        {
            *pCfZ -=guc_GateCF;                                     //对CF脉冲进行分频，将分频结果进行能量累加
            *pEyZ +=guc_UnitCF; 
            gui_SystemEvent |= flgEtGlb_EnergyCalcN;                 //设置能量处理事件
        }
    }
}

/*=========================================================================================\n
* @function_name: ReadRMSPara
* @function_file: EnergyBottom.c
* @描述: 读取显示比例系数k
* 
* 
* @参数: 
* @param:ucpos  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-05-29)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 ReadRMSPara(uint8 ucpos)
{
    uint32 TempValue;
    TempValue=0x1000;

    BE_ReadP(EEP_JBTOTAL+sizeof(S_JBPM)-19+ucpos*4,(uint8*)&TempValue,4);
    TempValue/=65536;
    return TempValue;
}
/*=========================================================================================\n
* @function_name: CalRMS
* @function_file: EnergyBottom.c
* @描述: 计算瞬时量
* 
* 
* @参数: 
* @param:addr  寄存器地址
* 
* @返回: 
* @return: uint32 返回有效值
* @作者:   lwb (2012-05-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  hjj
* @修改内容: 添加phadd, 添加相位选择
===========================================================================================*/
uint32 CalRMS(uint16 addr,uint8 phyadd)
{
    Word32 TempValue,TempValue3;
    uint32 TempPara,TempValue1,TempValue2;

    if(ReadRaccoon(addr,2,phyadd))
    {
        memcpy((uint8*)TempValue.byte,gs_RacCtrl.ucBuf+3,4);
        memcpy((uint8*)TempValue3.byte,gs_RacCtrl.ucBuf+7,4);
       
    }else
    {
        TempValue.lword=0;
        TempValue3.lword=0;
    }

    switch (addr)
    {
      case PAAVG:
          if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
          {
              TempValue.lword=~TempValue.lword+1;
          }

          TempPara=gs_JbPm.ul_PG;        //读取比例系数

          if(TempPara==0)
          {
              TempValue1=0;
          }
          else
          {
               TempValue.lword=((TempValue.lword)/TempPara);
          }
          if(TempValue.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
          {
               TempValue1=0;
          }
          else
          {
               TempValue1=Hex2BCD(TempValue.lword);

               if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
               {
                   TempValue1|=0x800000;
               }
          }
          break;
      case UAVG:
           TempPara=gs_JbPm.ul_URmG;   //ReadRMSPara(1);
           if(TempPara==0)
           {
              return 0;
           }
           else
           {
               TempValue.lword=TempValue.lword/TempPara;
           }

           if(TempValue.lword<ULEVEL)
           {
               return 0;
           }else
           {
               TempValue1=Hex2BCD(TempValue.lword);
           }
          // break;
      case IAAVG:
          //电流的计算
          TempPara=gs_JbPm.ul_I1RmG;    //ReadRMSPara(2);
          if(TempPara==0)
          {
              return 0;
          }
          else
          {
              TempValue3.lword=TempValue3.lword/TempPara;
          }

          if(TempValue3.lword<((gs_JbPm.ui_Ib/1000)*2))
          {
              return 0;
          }else
          {

              TempValue2=Hex2BCD(TempValue.lword);
              //功率的计算
              /*if(ReadRaccoon(DATAP,1,phyadd))
              {
                  memcpy((uint8*)TempValue2.byte,gs_RacCtrl.ucBuf+3,4);
              }else
              {
                  TempValue2.lword=0;
              }

              if(TempValue2.byte[3]>0x7f)                         //如果是负数的数，先变成正数
              {
                  TempValue2.lword=~TempValue2.lword+1;
              }

              TempPara=gs_JbPm.ul_PG;        //读取比例系数

              if(TempPara==0)
              {
                  TempValue2.lword=0;
              }
              else
              {
                  TempValue2.lword=((TempValue2.lword)/TempPara);
              }

              if(TempValue2.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
              {
                  //如果功率小于启动功率，这是电流小于0.2A,电流清零
                  if(TempValue1<0x200)
                  {
                      TempValue1=0;
                  }*/


              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
              {
                  TempValue2|=0x800000;
              }
            gs_UI.ul_I[phyadd >> 2] =  TempValue2;  
          }
          break;
//      case RMSI2:
//          TempPara=gs_JbPm.ul_I2RmG;    //ReadRMSPara(3);
//          if(TempPara==0)
//          {
//              return 0;
//          }
//          else
//          {
//              TempValue.lword=TempValue.lword/TempPara;
//          }
//
//          if(TempValue.lword<IBLEVEL)
//          {
//              return 0;
//          }else
//          {
//              TempValue1=Hex2BCD(TempValue.lword);
//
//              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
//              {
//                  TempValue1|=0x800000;
//              }
//          }
//          break;
        case DATAFREQ:
            TempValue.lword=(81920000)/TempValue.lword;

            if(TempValue.lword>4950&&TempValue.lword<5050)
            {
                TempValue.lword=5000;
            }else if(TempValue.lword<4500)
            {
                TempValue.lword=4500;
            }else if(TempValue.lword>5500)
            {
                TempValue.lword=5500;
            }
            TempValue1=Hex2BCD(TempValue.lword);
           break;
//    case RMSIU:
//         TempPara=gs_JbPm.ul_URmG/65536;   //ReadRMSPara(1);
//         if(TempPara==0)
//         {
//            return 0;
//         }
//         else
//         {
//             TempValue.lword=TempValue.lword/TempPara;
//         }
//
//         if(TempValue.lword<ULEVEL)
//         {
//             return 0;
//         }else
//         {
//             TempValue1=TempValue.lword;
//         }
//         break;
      default:
          TempValue1=0;
          break;
    }
    return TempValue1;
}
/*=========================================================================================\n
* @function_name: CalRMS2
* @function_file: EnergyBottom.c
* @描述: 
* 
* 
* @参数: 
* @param:addr  
* @param:value  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2013-09-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 CalRMS2(uint16 addr,uint32 value)
{
    uint32 TempPara,TempValue1;
    switch (addr)
    {
        case PAAVG:
        case DATAQ:
          TempPara=gs_JbPm.ul_PG;        //读取比例系数
          if(TempPara==0)
          {
              TempValue1=0;
          }
          else
          {
               value=((value*10)/TempPara);
          }
#if (SysMode_TEST != 1)
          if(value<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*3/200)) //0.15%  16.5W
          {
              TempValue1=0;
          }
          else
#endif            
          {
              TempValue1=Hex2BCD(value);
              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
              {
                  TempValue1|=0x800000;
              }
          }
          break;
      case UAVG:
           TempPara=gs_JbPm.ul_URmG;   //ReadRMSPara(1);
           if(TempPara==0)
           {
              return 0;
           }
           else
           {
               value=value/TempPara;
           }

           if(value<ULEVEL)
           {
               return 0;
           }else
           {
               TempValue1=Hex2BCD(value);
           }
           break;
      case IAAVG:
          //电流的计算
          TempPara=gs_JbPm.ul_I1RmG;    //ReadRMSPara(2);
          if(TempPara==0)
          {
              return 0;
          }
          else
          {
              value=value/TempPara;
          }
#if (SysMode_TEST != 1)
          if(value<((gs_JbPm.ui_Ib/1000)*3))
          {
              return 0;
          }else
#endif            
          {
              TempValue1=Hex2BCD(value);
              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
              {
                  TempValue1|=0x800000;
              }
          }
          break;
          case DATAFREQ:
#if (SysMode_TEST != 1)            
          if(value>4950&&value<5050)
          {
              value=5000;
          }else if(value>5950&&value<6050)
          {
              value=6000;
          }else 
#endif            
          if(value<4500)
          {    
              value=4500;
          }else if(value>6500)
          {    
              value=6500;
          }
          TempValue1=Hex2BCD(value);
         break;
          default:
              break;
    }
    return TempValue1;
}
/*=========================================================================================\n
* @function_name: CalCos
* @function_file: EnergyBottom.c
* @描述: 计算功率因数
* 
* @参数: 
* 
* @返回: 
* @return: uint16 
* @作者:   lwb (2012-06-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint16 CalCos(uint32 *Power,uint32 *Voltage,uint32 *Current,uint8 phy)
{
    uint8 flag;
    uint32 temp_p,temp_i,temp_u,temp_ui;
    Word32 temp_res;
    if(phy<4) //单相
    {
        temp_p = Power[phy];
        temp_u = Voltage[phy];
        temp_i = Current[phy];      

        if(temp_p&0x80000000)     //负功率
        {
            temp_p=~temp_p+1;   //功率取正
            flag=1;
        }
        
        temp_p=(temp_p*10/gs_JbPm.ul_PG);
        temp_u=(temp_u/(gs_JbPm.ul_URmG*10));
        //temp_i=(temp_i/(gs_JbPm.ul_I1RmG*100)); 
        temp_i=(temp_i/(gs_JbPm.ul_I1RmG*10));      
        
        temp_res.lword=(temp_p*10000)/(temp_u*temp_i);
    }else       //合相
    {
        temp_p = (Power[0]/gs_JbPm.ul_PG) + (Power[1]/gs_JbPm.ul_PG) + (Power[2]/gs_JbPm.ul_PG);
        temp_ui = 0;
        for( uint8 j= 0; j < 3; j++)
        {
             temp_ui += (Voltage[j]/gs_JbPm.ul_URmG)*(Current[j]/gs_JbPm.ul_I1RmG);
        }
        temp_ui/=1000;
        temp_res.lword=(temp_p*10000)/(temp_ui);
    }
    
    temp_res.lword = Hex2BCD(temp_res.lword);
    
    //功率因数处理
    if(temp_res.lword>=0x990)              //大于0.99的话直接显示1.0
    {    
        temp_res.lword=0x1000;
    }else if((temp_res.lword>=0x480)&&(temp_res.lword<=0x520))        //在0.48和0.52之间直接显示0.5
    {    
        temp_res.lword=0x500;
    }else if((temp_res.lword>=0x780)&&(temp_res.lword<=0x820))        //在0.78和0.82之间直接显示0.8
    {    
        temp_res.lword=0x800;
    }else if(temp_res.lword<=0x100)                                //小于0.1情况下,根据有功功率与电流显示
    {
        if(temp_i>=3)                           //有电流 
        {    
            if(temp_p<0x500)                       //且没有有功功率，显示0.0 
            {
                temp_res.lword=0x0000;
            }
        }
        else
        {                                           //没电流，显示1.0
            temp_res.lword=0x1000; 
        }
    }
                                                  //其余根据计算显示
    if(flag==1&&temp_res.lword!=0)
    {
        temp_res.word[0]|=0x8000;
    }
    return (uint16)temp_res.word[0];    
}
/*=========================================================================================\n
* @function_name: EnyB_Check_PowRev
* @function_file: EnergyBottom.c
* @描述: 检测功率是否反向
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_Check_PowRev(void)
{
    int32 l_pa;
    uint32 TempPara;
    uint32 ulPower;
//
    l_pa = EnyB_ReadMeterParaACK(PAAVG);
    if(l_pa>0)
    {                                       //功率反向
        TempPara=gs_JbPm.ul_PG>>8;          //读取比例系数
        if(TempPara==0)
        {                                   //校表参数功率计算值为0的话，直接判断功率原始值
            if(l_pa>0x2000)
            {
                gs_PDirCnt.ucRevDirCnt++;
                gs_PDirCnt.ucDirCnt=0;
                if(gs_PDirCnt.ucRevDirCnt>=gs_PDirCtr.ucDelayTm)
                {
                    gs_MeterState.ui_MeterState1.RunCode1Word.PDir=true;
                }
            }
            else
            {
                gs_PDirCnt.ucRevDirCnt=0;
                gs_PDirCnt.ucDirCnt++;
                if(gs_PDirCnt.ucDirCnt>=gs_PDirCtr.ucDelayTm)
                {
                    gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
                }
            }
        }else
        {
             ulPower=((l_pa*32)/TempPara)*8;
             if(ulPower>gs_PDirCtr.ul_PDirGate
                /*(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100)*/)
             {
                 gs_PDirCnt.ucRevDirCnt++;
                 gs_PDirCnt.ucDirCnt=0;
                 if(gs_PDirCnt.ucRevDirCnt>=gs_PDirCtr.ucDelayTm)
                 {
                     gs_MeterState.ui_MeterState1.RunCode1Word.PDir=true;
                 }
             }else
             {
                 gs_PDirCnt.ucRevDirCnt=0;
                 gs_PDirCnt.ucDirCnt++;
                 if(gs_PDirCnt.ucDirCnt>=gs_PDirCtr.ucDelayTm)
                 {
                     gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
                 }
             }
        }
    }else
    {
        gs_PDirCnt.ucRevDirCnt=0;
        gs_PDirCnt.ucDirCnt++;
        if(gs_PDirCnt.ucDirCnt>=gs_PDirCtr.ucDelayTm)
        {
            gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
        }
    }

    if(gs_PDirCtr.ul_PDirGate == 0)
    {
        gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
    }

    if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir)
    {
//     gs_MeterState.ui_MeterState1.RunCode1Word.PDir = true;               //置功率反向标志
       gs_MeterState.ui_MeterState2.RunCode2Word.PADIR= true;
       if(gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt == false    // 状态标志未置
          && gs_ReportWord.ui_ReportWord3.ReportWord3.PDirRpt == 1)         // 需要主动上报
       {
           gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = true;       // 状态置位
       }
       if(gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt == false   // 状态标志未置
          && gs_ReportWord.ui_ReportWord2.ReportWord2.PDirRpt == 1)         // 需要主动上报
       {
           gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = true;      // 状态置位
       }
//     gut_SysWarn.SysWarn.REVPOWER=true;
       if(gs_SysRunSt.PDirFR==false
//        &&(gs_DispStt.ucMode!=Const_DispMode_Full)
          )
       {

//         gs_DispStt.ucMode=Const_DispMode_Err;
//         gui_SystemEvent |= flgEtGlb_RefreshLcd;

//         #ifdef FRCom
//              EVENTOUTHIGH();
//              gs_FREventOut.FREventOutStruct.PDirFR=true;
//         #endif
           gs_SysRunSt.PDirFR=true;
       }
    }
    else
    {
//     gut_SysWarn.SysWarn.REVPOWER=false;
//     gs_MeterState.ui_MeterState1.RunCode1Word.PDir = false;      //清功率反向标志
       gs_MeterState.ui_MeterState2.RunCode2Word.PADIR= false;
       gs_SysRunSt.PDirFR=false;
       if(gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt == true)     // 判断状态标志
       {
           gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = false;
       }
       if(gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt == true)     // 判断状态标志
       {
           gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = false;
       }
    }
}
/*=========================================================================================\n
* @function_name: EnyB_Cal_Cos
* @function_file: EnergyBottom.c
* @描述: 计算功率因数
* 
* @参数: 
* 
* @返回: 
* @return: int16 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//int16 EnyB_Cal_Cos(void)
//{
//    uint32 ul_PS;
//    int16 CosA = 0;
//    if(gs_EnyInsData.ul_IARms < 40)
//    {
//        return 1000;
//    }
//    ul_PS = gs_EnyInsData.ul_URms*gs_EnyInsData.ul_IARms;
//    ul_PS /= 10000;
//    CosA = (gs_EnyInsData.l_PowerPA * 1000)/ (int32)ul_PS;
//    return CosA;
//}
 
/*=========================================================================================\n
* @function_name: EnyB_JbPm_Updata
* @function_file: EnergyBottom.c
* @描述: 刷新校表参数到SFR中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_JbPm_Updata(void)
{
    //把RAM中的校表参数跟新到SFR中
    EnyB_SetMeterCfgACK(gs_JbPm.RacEGYTH/3,0x10F4); //E1门限
    EnyB_SetMeterCfgACK(gs_JbPm.RacREEGYTH/3,0x10FA); //E2门限
    Clashin();
    UpdateRaccoonPara();
    BroadcastWriteRaccoon( AnaCtrl1_Val, 0x0183); //CSEL<1：0>设置为01
    CheckRegSum();
    CLRWDT(); 
//    CheckSysErr();  
    Clashout();
    CLRWDT(); 
}

/*=========================================================================================\n
* @function_name: EnyB_JbPm_Updata
* @function_file: EnergyBottom.c
* @描述: 刷新校表参数到SFR中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RecoverPowDirConfig(void)
{
    gs_PDirCnt.ucDirCnt=0;
    gs_PDirCnt.ucRevDirCnt=0;
    SysE2ReadData(EEP_PDIRTIME,(uint8*)&gs_PDirCtr.ucDelayTm,4);    //跟新反向功率判断延时和阀值 
    gs_PDirCtr.ucDelayTm=BCD2Hex(gs_PDirCtr.ucDelayTm);
    gs_PDirCtr.ul_PDirGate=BCD2Hex(gs_PDirCtr.ul_PDirGate&0x00ffffff);   
}

/*=============================================================================================\n
Function：自动校正算法
 ================================================================================================*/

/*=========================================================================================\n
* @function_name: CalculateSC
* @function_file: EnergyBottom.c
* @描述: 计算公式
* 
* 
* @参数: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*uint32 CalculateSC(int32 CurValue,int32 TarValue,int32 LastSC)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;

    //公式说明 计算误差公式 S=2^31(1/(1+e)-1)+S1(1/(1+e)),如果e扩大1000倍后，
    //化解公式得:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //下面计算  2^31(e/(1000+e))
    Error=(CurValue-TarValue)*1000/TarValue;            //计算误差值。。扩大1000
    Stemp0=0x80000000/(1000+Error);                     //先保留整数部分 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //保留余数
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //计算小数部分，小数部分扩大1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //把计算的整数和小数部分都乘以ERROR，得2^31(e/(1000+e))

    //下面计算S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //计算整数部分
    Stemp2remd=LastSC%(1000+Error);                     //计算余数部分
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //小数部分
    Stemp3=Stemp2*1000+Stemp2remd;                      //整数部分加小数部分  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}*/


/*=========================================================================================\n
* @function_name: CalculateAC
* @function_file: EnergyBottom.c
* @描述: 计算公式
* 
* 
* @参数: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-06-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*uint8 CalculateAC(int32 CurValue,int32 TarValue)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint8 Result;

    Error=(CurValue-TarValue)*1000/TarValue;            //计算误差值。。扩大1000
    Result = (int32)3011*Error/2000;

    return Result; 
}*/
uint8 CalculateAC( int16 Error)
{
    //计算公式更换            
    uint8 Result;
    Result = (int32)3011*Error*4/2000;
    return Result; 
}

 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkPW
* @function_file: EnergyBottom.c
* @描述: 校正功率和误差
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_AutoChkPW(void)
{
//    uint8 ucData[12];                        //三相标准功率值
//    uint8 i;
//    int32 l_SdPower[3];                        //标准功率值
//    int16  err[3];
//    int32 pa[3];
//   BE_ReadP(EEP_AUTOCAL,ucData,12);         //读取标准功率值 
//   for(i=0;i<3;i++)
//   {
//       l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //获得标准功率值
//       pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //实际功率值
//       pa[i] =BCD2Hex(pa[i]);
//       err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //计算误差
//       *ul_pRacReg[i] = CalculateSC(err[i],*ul_pRacReg[i]); //计算新的比差值并存入校表尝试
//   }
   
////  pa = EnyB_ReadMeterParaACK(DATAP);        //读取功率值做方向判断
////
////  if(pa>0)           //首先把表计读取的功率值转存正值
////  {
////     pa= -BCD2Hex(CalRMS(DATAP));//gs_EnyInsData.l_PowerPA;
////  }else
////  {
////     pa= BCD2Hex(CalRMS(DATAP));//-gs_EnyInsData.l_PowerPA;
////  }
//
//    pa=CalRMS(DATAP);               //读取功率
//    if(pa&0x800000)                 //判断方向
//    {
//        pa&=~0x800000;               
//    } 
//
//    pa =BCD2Hex(pa);
//    gs_JbPm.ul_SCP = CalculateSC(pa, l_SdPower, ul_Scp);   //计算比差值
// 
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);                                 //设置新比差值
}

/*=========================================================================================\n
* @function_name: EnyB_AutoChkPW2
* @function_file: EnergyBottom.c
* @描述: 小信号校正功率和误差
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 * const ul_pRacSReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacPADCC,(uint32*)&gs_JbPm.gs_JBB.RacPADCC,
                                (uint32*)&gs_JbPm.gs_JBC.RacPADCC};
void EnyB_AutoChkPW2(void)
{
    uint8 ucData[12],ucData1[4];                        //三相标准功率值
    uint8 i;
    int32 l_SdPower[3];                        //标准功率值
    int32 l_SdPreg;
    int32  err[3];
    int32 pa[3];
    BE_ReadP(EEP_AUTOCAL,ucData,12);         //读取标准功率值 
    BE_ReadP(EEP_AUTOCAL+24,ucData1,4);         //读取标准功率值 
    l_SdPreg = Uint8_To_Uint32(ucData1);
    for(i=0;i<3;i++)
    {
       l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //获得标准功率值
       
       pa[i] = CalRMS2(PAAVG,gs_power.ul_power[i]);      //实际功率值
       pa[i] =BCD2Hex(pa[i]);
       err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //计算误差
       *ul_pRacSReg[i] = (0-err[i])*l_SdPreg/1000 ;
    }
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
    EnyB_JbPm_Updata();   
//    uint8 ucData[4];                        //单相标准功率值
//    uint8 i;
//    int32 l_SdPower;                        //标准功率值   
//    Word32 TempValue;
//    uint32 TempSum;
//    uint32 pa_sum,TempPara;
//    
//    pa_sum = 0;
//    TempSum = 0;
//    BE_ReadP(EEP_AUTOCAL,ucData,4);         //读取标准功率值  
//
//    l_SdPower = Uint8_To_Uint32(ucData);   //获得标准功率值
//
//    for(i=0;i<16;i++)
//    {
//        TempValue.lword=EnyB_ReadMeterParaACK(DATAIP); 
//        DelayXms(20);
//        CLRWDT();                           //喂狗
//        //DelayXms(255);
//        if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
//        {
//            TempValue.lword=~TempValue.lword+1;
//        }
//        TempSum += TempValue.lword;
//    }   
//    
//    TempSum = TempSum>>4;              //读取功率寄存器值
//    TempPara = gs_JbPm.ul_PG/256;        //读取比例系数
//    pa_sum = (TempSum*2560)/TempPara;  //换算值
//      
//    TempValue.lword = l_SdPower*TempPara/2560;    //理论寄存器值
//    
//    if(l_SdPower > pa_sum)
//      gs_JbPm.ul_PARAPC = (TempValue.lword*(l_SdPower-pa_sum)/l_SdPower);       //计算二次补偿值
//    else
//      gs_JbPm.ul_PARAPC = -(TempValue.lword*(pa_sum-l_SdPower)/l_SdPower);   //计算二次补偿值
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_PARAPC, PARAPC);                                 //设置新比差值
}
 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkIrms
* @function_file: EnergyBottom.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 * const ul_pRacIReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacIAC,(uint32*)&gs_JbPm.gs_JBB.RacIAC,
                                (uint32*)&gs_JbPm.gs_JBC.RacIAC};
void EnyB_AutoChkIrms(void)
{
//    uint8 ucData[2];
//    uint32  ul_SdCurrent;
//    uint32  ul_Irmg;
//    int32  l_IARms;
//
//    BE_ReadP(EEP_AUTOCAL+12,ucData,2);      //读取标准电流值
//    ul_SdCurrent=Uint8_To_Uint16(ucData);    //获得标准电流值
//    ul_Irmg = EnyB_ReadMeterParaACK(SCI1);                    //读取比差值
//
//    l_IARms= BCD2Hex(CalRMS(RMSI1));//gs_EnyInsData.ul_IARms;
//
//    gs_JbPm.ul_SCI1 = CalculateSC(l_IARms, ul_SdCurrent, ul_Irmg);   //计算比差值
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCI1, SCI1);                                 //设置新比差值
    uint8 ucData[6];                        //三相标准功率值
    uint8 i;
    int32 ul_SdCurrent[3];                        //标准电流值
    int16  err[3];
    int32 pa[3];
    BE_ReadP(EEP_AUTOCAL+12,ucData,6);         //读取标准电流值
    for(i=0;i<3;i++)
    {
        ul_SdCurrent[i] = Uint8_To_Uint16(ucData + 2*i);   //获得标准电流值
        pa[i] = CalRMS2(IAAVG,gs_UI.ul_I[i]);      //实际电流
        pa[i] =BCD2Hex(pa[i]);
        err[i] = (pa[i] -  ul_SdCurrent[i])*1000/ ul_SdCurrent[i] ;   //计算误差
        *ul_pRacIReg[i] = CalculateSC(err[i],*ul_pRacIReg[i]); //计算新的比差值并存入校表尝试
    }

}

 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkUrms
* @function_file: EnergyBottom.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 * const ul_pRacUReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacUC,(uint32*)&gs_JbPm.gs_JBB.RacUC,
                                (uint32*)&gs_JbPm.gs_JBC.RacUC};
void EnyB_AutoChkUrms(void)
{
//    uint8 ucData[2];
//    uint32  ul_SdVoltage;
//    uint32  ul_Urmg;
//    int32  l_URms;
//
//    BE_ReadP(EEP_AUTOCAL+18,ucData,2);      //读取标准电压值
//    ul_SdVoltage=Uint8_To_Uint16(ucData);    //获得标准电压值
//    ul_Urmg = EnyB_ReadMeterParaACK(SCU);                    //读取比差值
//
//    l_URms= BCD2Hex(CalRMS(RMSU))*10;//gs_EnyInsData.ul_URms;
//
//    gs_JbPm.ul_SCU = CalculateSC(l_URms, ul_SdVoltage, ul_Urmg);   //计算比差值
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCU, SCU);                                 //设置新比差值
    uint8 ucData[6];                        //三相标准功率值
    uint8 i;
    int32 ul_SdVoltage[3];                        //标准电流值
    int16  err[3];
    int32 pa[3];
    BE_ReadP(EEP_AUTOCAL+18,ucData,6);         //读取标准电流值
    for(i=0;i<3;i++)
    {
        ul_SdVoltage[i] = Uint8_To_Uint16(ucData + 2*i);   //获得标准电流值
        pa[i] = CalRMS2(UAVG,gs_UI.ul_U[i]);      //实际电流
        pa[i] =BCD2Hex(pa[i])*10;
        err[i] = (pa[i] -  ul_SdVoltage[i])*1000/ ul_SdVoltage[i] ;   //计算误差
        *ul_pRacUReg[i] = CalculateSC(err[i],*ul_pRacUReg[i]); //计算新的比差值并存入校表尝试
    }

}

/*=========================================================================================\n
* @function_name: EnyB_AutoChkAngle
* @function_file: EnergyBottom.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 * const ul_pRacACReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacPHC,(uint32*)&gs_JbPm.gs_JBB.RacPHC,
                                (uint32*)&gs_JbPm.gs_JBC.RacPHC};

void EnyB_AutoChkAngle(void)
{
//    uint8 ucData[4];                        //单相标准功率值
//    int32 l_SdPower;                        //标准功率值
//    int32 pa;
//    
//    //EnyB_SetMeterCfgACK(0, PHCCtrl1);       //角差值写0
//    //需要延时等待，故放到通信写入E2前
//    BE_ReadP(EEP_AUTOCAL,ucData,4);         //读取标准功率值  
//   
//    l_SdPower = Uint8_To_Uint32(ucData)/10;   //获得标准功率值
////  pa = EnyB_ReadMeterParaACK(DATAP);        //读取功率值做方向判断
////
////  if(pa>0)           //首先把表计读取的功率值转存正值
////  {
////     pa= -BCD2Hex(CalRMS(DATAP));//gs_EnyInsData.l_PowerPA;
////  }else
////  {
////     pa= BCD2Hex(CalRMS(DATAP));//-gs_EnyInsData.l_PowerPA;
////  }
//
//    pa=CalRMS(DATAP);               //读取功率
//    if(pa&0x800000)                 //判断方向
//    {
//        pa&=~0x800000;               
//        pa =BCD2Hex(pa);
//    }else
//    {
//        pa =BCD2Hex(pa);
//    }
//
    uint8 ucData[12];                        //三相标准功率值
    uint8 i;
    int32 l_SdPower[3];                        //标准功率值
    int16  err[3];
    int32 pa[3];
     uint32 JbJc;
    BE_ReadP(EEP_AUTOCAL,ucData,12);         //读取标准功率值 
    for(i=0;i<3;i++)
    {
        l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //获得标准功率值
        pa[i] = CalRMS2(PAAVG,gs_power.ul_power[i]);      //实际功率值
        pa[i] =BCD2Hex(pa[i]);
        err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //计算误差    
        JbJc = CalculateAC(err[i]); //计算新的角差值并存入校表尝试
        JbJc<<=8;
        (*ul_pRacACReg[i])&=0xfffB00ff;                          //角差位清零
        (*ul_pRacACReg[i])|=JbJc;
    }
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
    EnyB_JbPm_Updata();     

//    
//    gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
//
//    EnyB_JbPm_Updata();                                       //跟新校表参数到计量芯片
}
/*=========================================================================================\n
* @function_name: EnyB_AutoChkInrms
* @function_file: EnergyBottom.c
* @描述: 校正零线电流
* 
* 
* @参数: 
* @param:ui_SdCurrent  
* @返回: 
* @作者:   lwb (2012-10-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_AutoChkInrms(uint16 ui_SdCurrent)
{
//    uint32  ul_Irmg;
//    int32   l_IARms;
//    int32   ul_SdCurrent;
//    
//    ul_SdCurrent=ui_SdCurrent;
//
//    ul_Irmg = EnyB_ReadMeterParaACK(SCI2);                    //读取比差值
//
//    l_IARms= BCD2Hex(CalRMS(RMSI2));//gs_EnyInsData.ul_IARms;
//
//    gs_JbPm.ul_SCI2 = CalculateSC(l_IARms, ul_SdCurrent, ul_Irmg);   //计算比差值
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCI2, SCI2);                     //设置新比差值
//
//
//    gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//    BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));        //写回校表参数
//        
//    #if (_SW_SE_Type > 0)
//        SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));    
//    #else
//        BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
//    #endif     

} 
/*=========================================================================================\n
* @function_name: EnyB_AutoCheck
* @function_file: EnergyBottom.c
* @描述: 自动校表
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_AutoCheck(void)
{
    EnyB_AutoChkPW();                                                   // 校正功率和误差
    EnyB_AutoChkIrms();                                                 // 校正电流有效值
    EnyB_AutoChkUrms();                                                 // 校正电压有效值
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
    EnyB_JbPm_Updata();      
//    //if((0x00 == gs_JbPm.uc_JbOkFlag)&&(guc_FactoryType))                    // 没有禁止写入标志，同时处于生产模式，允许写校表参数
//    //{//写入数据
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));            //写回校表参数
//
////      BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));            //写回校表参数
////
////  #if (_SW_SE_Type > 0)
////      SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
////  #else
////      BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
////  #endif
//             
//        EnyB_JbPm_Updata();                                       //跟新校表参数到计量芯片
//    //}
}
 
/*=========================================================================================\n
* @function_name: EnyB_CloseAutoCheck
* @function_file: EnergyBottom.c
* @描述: 关闭自动校表，一旦校正完成后，需要关闭
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_CloseAutoCheck(void)
{
//    //需要加保护限制，要求在生产模式
//    if(guc_FactoryType==FactorMd)                                                    // 只有在生产模式下，才允许写入数据
//    {
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);      //计算CRC校验值
//        BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
//        
//    #if (_SW_SE_Type > 0)
//        SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));    
//    #else
//        BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
//    #endif 
//    }
}
 
/*=========================================================================================\n
* @function_name: EnyB_OpenAutoCheck
* @function_file: EnergyBottom.c
* @描述: 打开自动校表
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_OpenAutoCheck(void)
{
//    if(guc_FactoryType==FactorMd)// 只有在生产模式下，才允许写入数据
//    {
//        
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//        BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));        //写入数据
//        
//    #if (_SW_SE_Type > 0)
//        SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));    
//    #else
//        BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
//    #endif 
//    }
}
/*=========================================================================================\n
* @function_name: EnyB_Check_RMSI
* @function_file: EnergyBottom.c
* @描述: 检测是否大电流
* 
* @参数: 
* @返回: 
* @作者:   lm (2013-03-21)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_Check_RMSI(void)
{
//    uint32 ul_rmsi;
//
//    if(gs_GiantSCP.ui_CRC == do_CRC((uint8*)&gs_GiantSCP.ul_Adjust,4)                  
//       && gs_GiantSCP.ul_Adjust != 0)                                        //大电流有补偿值需要补偿
//    {
//        ul_rmsi = CalRMS(RMSI1)&0x7fffffff;                                  //读取电流有效值
//        
//        if((ul_rmsi >= 0x048000 && guc_GiantSCPflag==0x55)                    //大于48A & 
//           || ul_rmsi >= 0x052000)                                           //大于52A 
//        {
//            if(guc_GiantSCPcnt>3)
//            {
//                if(gs_GiantSCP.ul_Adjust<0x014AFD6A && gs_GiantSCP.ul_Adjust>-0x014AFD6A) //保护
//                {
//                    ul_rmsi = gs_JbPm.ul_SCP + gs_GiantSCP.ul_Adjust;
//                }
//                else
//                {
//                    ul_rmsi = gs_JbPm.ul_SCP;
//                }
//                EnyB_SetMeterCfgACK(ul_rmsi, SCP);
//                guc_GiantSCPflag = 0x55;
//                guc_GiantSCPcnt = 3;
//            }
//            else
//            {
//                EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);
//                guc_GiantSCPflag = 0xAA;
//            }
//            guc_GiantSCPcnt++;
//        }
//        else
//        {
//            EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);
//            guc_GiantSCPflag = 0xAA;
//            guc_GiantSCPcnt = 0;
//        }
//    }
//    else
//    {
//        EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);
//        guc_GiantSCPflag = 0x00;
//    }
}


void ReadABCPHY(void)
{
//     uint32 temp[3];
//     temp[0] = abs(Read_Power(DATAP,APhy));
//     temp[1] = abs(Read_Power(DATAP,BPhy));
//     temp[2] = abs(Read_Power(DATAP,CPhy));   //秒读功率值
//     
//     __disable_interrupt();  //关闭总中断
//     gs_power.ul_power[0] =  temp[0];
//     gs_power.ul_power[1] =  temp[1];
//     gs_power.ul_power[2] =  temp[2];   //秒读功率值
//     __enable_interrupt();
}


void ABCPhyProc(void)
{
    if(gui_ReadABCPhy &flgEtPhy_A)
    {        
        gui_ReadABCPhy &=~ flgEtPhy_A;
        ReadRaccoonInt(PAAVG,5,APhy); //读取电量，结果在系统事件中获得
    }
    else if(gui_ReadABCPhy &flgEtPhy_B)
    {
        gui_ReadABCPhy &=~ flgEtPhy_B;
        ReadRaccoonInt(PAAVG,5,BPhy);

    }else if(gui_ReadABCPhy &flgEtPhy_C)
    {
        gui_ReadABCPhy &=~ flgEtPhy_C;
        ReadRaccoonInt(PAAVG,5,CPhy);

    }else
    {
            gui_ReadABCPhy=0;
    }
}

/*=========================================================================================\n
* @function_name: CalculateSC
* @function_file: EnergyBottom.c
* @描述: 计算公式
* 
* 
* @参数: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 CalculateSC(int16 error,int32 LastSC)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;
    //公式说明 计算误差公式 S=2^31(1/(1+e)-1)+S1(1/(1+e)),如果e扩大1000倍后，
    //化解公式得:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //下面计算  2^31(e/(1000+e))
    Error=error;                                                               //计算误差值。。扩大1000
    Stemp0=0x80000000/(1000+Error);                     //先保留整数部分 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //保留余数
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //计算小数部分，小数部分扩大1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //把计算的整数和小数部分都乘以ERROR，得2^31(e/(1000+e))
    //下面计算S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //计算整数部分
    Stemp2remd=LastSC%(1000+Error);                     //计算余数部分
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //小数部分
    Stemp3=Stemp2*1000+Stemp2remd;                      //整数部分加小数部分  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}


uint32 EnyB_ReadMeterParaACK(uint16 addr)
{
    udata8 index;
    uint32  u32PMdatal,temp;
    XBYTE[INVD]=0XCC;
    XBYTE[INVD]=0x00;
    index=0;
    while(XBYTE[ACK])
    {
        index++;
        if(index>CFGDELAY)
        {
            return false;
        }
    }

    index=XBYTE[addr];                          //读取真实地址，加载数据到缓冲
//  for(index=0;index<CFGDELAY;index++)
    {   
        nop();
    }
    index=0;
    while(XBYTE[ACK])
    {
        index++;
        if(index>CFGDELAY)
        {
            return false;
        }
    }

    u32PMdatal=(uint32)XBYTE[BUFF0];
    temp=(uint32)XBYTE[BUFF1]<<8;
    u32PMdatal+= temp;
    temp=(uint32)XBYTE[BUFF2];
    temp=temp<<16;
    u32PMdatal+=temp;
    temp=(uint32)XBYTE[BUFF3];
    temp=temp<<24;
    u32PMdatal+=temp;
    return u32PMdatal;
     
}

uint8 EnyB_SetMeterCfgACK(uint32 u32PMdatal ,uint16 addr)
{
   uint8 index;
    volatile uint32 temp;
    index=0;
    
    XBYTE[INVD]=0XCC;
    XBYTE[INVD]=0x00;
    while(XBYTE[ACK])
    {
        index++;
        if(index>CFGDELAY)
        {
            return false;
        }
    }

    XBYTE[BUFF0]=(uint8)(u32PMdatal&0xff);
    XBYTE[BUFF1]=(uint8)((u32PMdatal>>8)&(0xff));
    XBYTE[BUFF2]=(uint8)((u32PMdatal>>16)&(0xff));
    XBYTE[BUFF3]=(uint8)((u32PMdatal>>24)&(0xff));
    XBYTE[addr]=0;                              //向真正的地址写入0，启动缓冲输入到真正的地址

//  for(index=0;index<CFGDELAY;index++)
    {   
        nop();
    }

    index=0;
    while(XBYTE[ACK])
    {
        index++;
        if(index>CFGDELAY)
        {   
            return false;
        }
    }
    
//  temp=EnyB_ReadMeterParaACK(addr);
//  if(temp==u32PMdatal)
//  {
//      return true;
//  }
//  else
//  {
//      return false;
//  }
    return true;
}



/*=========================================================================================\n
* @function_name: ADCInit
* @function_file: EnergyBottom.c
* @描述: 初始化初始化ADC，设置增益和通道
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-12)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ADCInit(void)
{
    CtrlADC5=0x81;                              //默认是测量温度
    CtrlADC6=0;
 //   ADCOPEN(ADCAPDN+ADCUPDN+ADCMPDN);   //OPEN ADC U A B M
//  CtrlADC0=(ADCA16X+ADCB1X+ADCU1X);           //ADC A 32X GAIN  ADC B 1x GAIN ADC U 1X GAIN
}
/*=========================================================================================\n
* @function_name: PMDspInit
* @function_file: EnergyBottom.c
* @描述: 计量寄存器初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-12)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void PMDspInit(void)
{
    uint8 i;
    PMG=0;
    EnyB_SetMeterCfgACK(0x00,PMCtrl4);
    EnyB_SetMeterCfgACK(0x00,PMCtrl1);
    EnyB_SetMeterCfgACK(INITGATEP,GATEP);        //设置计量初始值
    EnyB_SetMeterCfgACK(INITGATEQ,GATEQ);
    for(i=0;i<4;i++)
    {
        EnyB_SetMeterCfgACK(0,0x10f0+i);
    }
//  EnyB_SetMeterCfgACK(0,0x10f0);
//  EnyB_SetMeterCfgACK(0,0x10f1);
//  EnyB_SetMeterCfgACK(0,0x10f2);
//  EnyB_SetMeterCfgACK(0,0x10f3);

    for(i=0;i<4;i++)
    {
        EnyB_SetMeterCfgACK(0,0x10f6+i);
    }
//  EnyB_SetMeterCfgACK(0,0x10f6);
//  EnyB_SetMeterCfgACK(0,0x10f7);
//  EnyB_SetMeterCfgACK(0,0x10f8);
//  EnyB_SetMeterCfgACK(0,0x10f9);
    EnyB_SetMeterCfgACK(0xC8,PMCtrl4);
    EnyB_SetMeterCfgACK(0x08,PMCtrl4);
    EnyB_SetMeterCfgACK(0x00,PMCtrl4);

//  EnyB_SetMeterCfgACK(0,0x10f0);
//  EnyB_SetMeterCfgACK(0,0x10f1);
//  EnyB_SetMeterCfgACK(0,0x10f2);
//  EnyB_SetMeterCfgACK(0,0x10f3);

    for(i=0;i<4;i++)
    {
        EnyB_SetMeterCfgACK(0,0x10f0+i);
    }

//  EnyB_SetMeterCfgACK(0,0x10f6);
//  EnyB_SetMeterCfgACK(0,0x10f7);
//  EnyB_SetMeterCfgACK(0,0x10f8);
//  EnyB_SetMeterCfgACK(0,0x10f9);

    for(i=0;i<4;i++)
    {
        EnyB_SetMeterCfgACK(0,0x10f6+i);
    }
    EnyB_SetMeterCfgACK(0x01,GATECP);       //潜动1
    
    EnyB_SetMeterCfgACK(0x01,GATECQ);       //潜动2
    EnyB_SetMeterCfgACK(0x58,PMCtrl1);      //开启所有通道
    EnyB_SetMeterCfgACK(0x00,PMCtrl2);      //
    //EnyB_SetMeterCfgACK(0x70,PMCtrl3);      //开启两路有功，次谐波抑制
    EnyB_SetMeterCfgACK(0x60,PMCtrl3);      //开启一路有功，一路无功 次谐波抑制
    EnyB_SetMeterCfgACK(0xc0,PMCtrl4);      //等待滤波器稳定后开启
    EnyB_SetMeterCfgACK(0,PHCCtrl1);        //清0
    EnyB_SetMeterCfgACK(0,PHCCtrl2);
    guc_CfOpenFlg=true;
}

/*=========================================================================================\n
* @function_name: CF_TimCtrl
* @function_file: EnergyBottom.c
* @描述: CF脉冲输出高电平时间控制
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-12)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CF_TimCtrl(void)
{
  if( CalRMS2(PAAVG,(gs_power.ul_power[0] + gs_power.ul_power[1] + gs_power.ul_power[2])) > 0x180000) //大于18KW
  {
    guc_80msCntFLG1 = 1;
  }else
  {
    guc_80msCntFLG1 = 0;
  }
  if( CalRMS2(DATAQ,(gs_power.ul_Npower[0] + gs_power.ul_Npower[1] + gs_power.ul_Npower[2])) > 0x180000) //大于18KW
  {
    guc_80msCntFLG2 = 1;
  }else
  {
    guc_80msCntFLG2 = 0;
  }
}