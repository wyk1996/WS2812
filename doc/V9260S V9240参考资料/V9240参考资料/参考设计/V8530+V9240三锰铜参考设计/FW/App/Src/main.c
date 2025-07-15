
#define __MAIN__
#define VAREXT

#include "Include.h"
#include "main.h"
#include "lib_uart.h"
__IO uint8_t adc_done;
unsigned long abc;
uint8 tx_buf[4] = {0x12,0x34,0x56,0x78};
extern uint8 ktx_count;

void Init_Var(void);
void TimeEventProc(void);
void SysEventProc(void);
uint8 IOWakeUPFunc(void);
uint8 RTCWakeUPFunc(void);

/*=========================================================================================\n
* @function_name: main
* @function_file: main.c
* @描述: 入口主函数
*
* @参数:
* @返回:
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-7-20)
* @修改内容:
===========================================================================================*/
int main(void)
{
    /*
    uint8 temp;
    MAINSTART:
    if(POWERSTATE())		//没电
    {
        if(Rtcwkup_flag == 1)	//睡眠状态下被rtc唤醒             中断置标志位
        {
          
            if(RTCWakeUPFunc())         //1122 调用的唤醒时间设置函数有影响 与下面一样
            {
                SleepRTC();
            }
            else
            {
                goto MAINSTART;
            }
        }

        if(Ioawkup_flag == 1)	//睡眠状态下被io口唤醒
        {
            if(IOWakeUPFunc())
            {
            }
            else
            {
                goto MAINSTART;
            }
        }
        
        SetPLL800K();
        Eny_PurCheckRAMEny();		
        //Init_CfG();		//清零配置项 CF保存配置项         zzp
        IOOFF();                            //关闭所有额外的IO
        MCUForPowerOff();//关闭adc和BGP
        IntOFF();                           //关闭中断，清除中断标记
        UARTOFF();                          //关闭所有UART
        if(guc_PoweroffCoverJl!=0x50)
        {
            RTCWakeUpTm(Init_RTC_HOUR);                          //1hour唤醒
        }else
        {
            RTCWakeUpTm(InitPara38[1]);                     //1Min唤醒
        }
        if(Sleep())
        {
        }
        else
        {
            goto MAINSTART;
        }
    }

    //FWC=0;	//zzp
    //FSC=0;
    SetPLL800K();
    temp=SetPLL(Fmcu_Val);    //26MHz

    if(temp==0)
    {
        goto MAINSTART;
    }

    for(temp=0; temp<20; temp++)
    {
        CLRWDT();               //喂狗
        DelayOSC(187);         //20ms延时
        DelayOSC(187);
        if(POWERSTATE())	//系统没电
        {
            IOOFF();                            //关闭所有额外的IO
            MCUForPowerOff();
            IntOFF();                           //关闭中断，清除中断标记
            UARTOFF();                          //关闭所有UART
            if(guc_PoweroffCoverJl!=0x50)	//0x55？？？
            {
                RTCWakeUpTm(Init_RTC_HOUR);                          //1hour唤醒
            }else
            {
                RTCWakeUpTm(InitPara38[1]);                     //1Min唤醒
            }
            if(Sleep())
            {
            }
            else
            {
                goto MAINSTART;
            }
        }        
    }
    */
  

    __disable_irq();	
    
    SetPLL(Fmcu_Val);    //26MHz  zzp
    
    CPUInit();              //系统初始化
    Disp_Full();            //显示全屏
    SLPWDT();
    Init_Var();             //初始化参数
    /****************************/

    SLPWDT();
    Init_RacCtrl();
    SLPWDT();
    ShowSysStart();         //显示上电初始化
     
    SLPWDT();
    __enable_irq();

    GetBat();               //得到电池电压
    RxReset_V9260(); 
    
    //EnyB_SetMeterRule();
    EnyB_PurInit();         //计量模块上电初始化  zzp1203
    Init_Timer1();
    
    
    /*
    memset((uint8*)&gs_JbPm.ui_MeterC,0,sizeof(S_JBPM));
    abc=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    abc=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    abc=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    abc=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-4);
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);
    
    
    
    abc=do_CRC((uint8*)&XDATA_RAMZONE2,sizeof(S_JBPM)-2);
    abc=do_CRC((uint8*)&XDATA_RAMZONE2,sizeof(S_JBPM)-2);
    abc=do_CRC((uint8*)&XDATA_RAMZONE2,sizeof(S_JBPM)-2);
    abc=do_CRC((uint8*)&XDATA_RAMZONE2,sizeof(S_JBPM)-2);
    
    
    
    //BE_WriteP(0x1b8,XDATA_RAMZONE1,4);
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));
    BE_WriteP(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));
   
    BE_ReadP(0x1b8, XDATA_RAMZONE,4);
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));
    BE_WriteP(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));
    BE_ReadP(0x1b8, XDATA_RAMZONE,4);
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));
    BE_WriteP(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));*/
    BE_ReadP(EEP_JBTOTAL, XDATA_RAMZONE,125);
    BE_ReadP(EEP_JBTOTAL+EEP_BAK_LEN, XDATA_RAMZONE2,125);
    
    //WriteRaccoon(3,0x0180,APhy);
//    ReadRaccoon(0x00F6,1,APhy);
//    ReadRaccoon(0x0180,1,BPhy);
//    ReadRaccoon(0x00CA,1,BPhy);
//    ReadRaccoon(0x00F6,1,CPhy);
    
    

//    WriteRaccoon(3,0x0180,APhy);
//    ReadRaccoon(PAAVG,1,APhy);
//    WriteRaccoon(3,0x0180,APhy);
//    ReadRaccoon(PAAVG,5,APhy);
    

    
    //Disp_Clr();
//    Disp_Clear();
//    Disp_Numb(0x12345678, 0);
//    //Disp_String(0x1);
//    LCD_RAMUpdata();
//    
//    PMU->IOADAT &=~ BIT7;
    while (1)
    {

      /*
        CLRWDT();               //喂狗       
        Disp_Full();
       */
      CLRWDT();               //喂狗   
      
      //Accumulate_abc();      //累加分电量
      
      if(0x00 != gui_SystemEvent)                      //
        {                                               //
            SysEventProc();   // 系统事件   //1122PowerDnPro();里的RTCWakeUpTm
        }   
      else if(0x00!= guc_TimeEvent)                   //
        {                                               //
            TimeEventProc();                            // 时间处理流程
        }
      else if(0x0000 != gui_RefreshEvent)             //
        {                                               //
            RefreshPara();                              // 参数刷新
        } 
      else if(0x00 != guc_KeyEvent)                    //
        {                                               //
            KeyProc();                                  // 按键处理
            
            PMU->IOADAT |= BIT7;
            ///*****************************************************************
            abc = gs_JbPm.gs_JBB.RacPADCC;
            abc = gs_JbPm.gs_JBB.RacUC;

            gs_ComGroup[ComIndex_Uart2].ucPort = Port_Uart1;
            
            
              ktx_count = 1;
              UART_SendData( UART0, (abc&0xff));//485
              //UART_SendData( UART0, tx_buf[i]);//485
                        
            gs_ComGroup[ComIndex_Uart2].ucPort = Port_Idle;
            //*****************************************************************/
//            ReadRaccoon(0x0180,1,APhy);
//            ReadRaccoon(0x00d0,1,APhy);
//            ReadRaccoon(0x00d0,1,BPhy);
//            ReadRaccoon(0x00d0,1,CPhy);
//            ReadRaccoon(0x00F8,1,APhy);
//            ReadRaccoon(0x00F8,1,BPhy);
//            ReadRaccoon(0x00F8,1,CPhy);
        }
       if(gui_SystemSleepState==0x55aa)
        {
            //goto MAINSTART;
        }

     }
}

/*=========================================================================================\n
* @function_name: Init_Var
* @function_file: main.c
* @描述: 初始化系统全局变量和参数
*
* @参数:
* @返回:
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-8-9)
* @修改内容:
===========================================================================================*/
void Init_Var(void)
{
    uint8 i;

	Ioawkup_flag = 0;
	Rtcwkup_flag = 0;
        
        
/*********************************************************************/	
//    guc_1msCnt=0;
    gui_SystemEvent=0;
    guc_TimeEvent=0;
    gui_RefreshEvent=0;
    
    //gui_RefreshEvent |= flgEtPara_Init;//zzp1127
    
    guc_KeyEvent=0;
    guc_DjCheckFlg=0;
//    guc_CheckBatFlg=false;
    guc_RtcConflict=0;
    guc_BodeDely=0;
//    guc_SwichPluse=0;
//    guc_LongKeyFlg=0;
//    guc_PluseCount=0;
//    guc_LoopChk1Cnt=0;
//    guc_LoopChk2Cnt=0;
   // guc_SleepFlg=true;
//    guc_CfOpenFlg=true;
    guc_OpenCfTmCnt=0;
    guc_CoverKeyDwCnt=0;
    guc_ShowStCom=0;
    gs_SysRunSt.PluseType=0;
    gut_SysWarn.byte=0;
    gs_MeterState.ui_MeterState1.Word.word=0;
    gs_MeterState.ui_MeterState2.Word.word=0;
    gs_MeterState.ui_MeterState3.Word.word&=0x0021;
    gs_MeterState.ui_MeterState4.Word.word=0;
    gs_MeterState.ui_MeterState5.Word.word=0;
    gs_MeterState.ui_MeterState6.Word.word=0;
    gs_MeterState.ui_MeterState7.Word.word=0;
    guc_ComSendDy=0;
//    guc_FrsCalRTCDelay=5;
    gui_SystemSleepState=0;
    guc_SecCnt= 0;
    guc_PrOutTm= 0;
//    guc_Timechange=0x55;
  //  memcpy(XDATA_RAMZONE,   (uint8*)&gs_ReportState,   10);        //主动上报状态字1
    memcpy(XDATA_RAMZONE+10,   (uint8*)&gs_ReportStateFlg,10);        //当前电量增量
   // if(gui_ReportCRC != do_CRC(XDATA_RAMZONE, 20))
  //  {
 //       memset((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //主动上报状态清零
  //      memset((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //主动上报状态标志清零
  //  }
    
    guc_SleepFlg=false;
    gs_RacPhy.uc_Sendstat =0;
    gs_RacPhy.uc_Powstat = 0;
    gs_RacPhy.uc_flapara=0;
    gs_RacPhy.uc_Physendtim = 0;
     gui_SystemState &= ~flgStSys_PowOff;
   // gs_power.ul_underpow = 0; //潜动计数值
   // gs_power.ul_Nunderpow = 0;
    gs_RacPhy.uc_Uartstat  = 0;
    for(i = 0; i < 4; i++)
    {
        gs_UI.ul_P[i] = 0;
        gs_power.ul_power[i] = 0;
        gs_power.ul_power_all[i] = 0;
        gs_power.ui_power_rem[i]=0;
    }
    for(i = 0; i < 4; i++)
    {
        gs_power.ul_Npower[i] = 0;
        gs_power.ul_Npower_all[i] = 0;
        gs_power.ui_Npower_rem[i]=0;
    }
    for(i = 0; i < 3; i++)
    {
        gs_UI.ul_U[i] = 0;
        gs_UI.ul_I[i] = 0;
        gs_UI.ul_Hz[i] = 0;
        
        gs_power_enerycal.ul_power[i] = 0;
        gs_power_enerycal.ul_Npower[i] = 0;
        gul_FRQAVG_Value[i] = 0;
    }
    guc_1280msCnt =0;
    gui_ReadABCPhy = 0;
    guc_ABCreadbegian =0;
    gs_RacPhy.uc_Uartstat = 0;
    gs_phytim.RevPhaseFlg = 0;
    gs_phytim.APhytim = 0;
    gs_phytim.BPhytim = 0;
    gs_phytim.CPhytim = 0;
    guc_80msCntCF1 = 0;
    guc_80msCntCF2 = 0;
    
    guc_80msCntFLG1 = 0;
    guc_80msCntFLG1 = 0;
    
    guc_CheckABCPhase_Cnt = 0;
    guc_CheckABCPhase_Dly = 0;
    guc_CalBPFValue_DelayTm = 15;  //间隔15s 计算带通滤波器 
    
    CLRWDT();
    RecoverBPFPARA_Value(); //恢复带通滤波器系数
    CLRWDT();
    
#if (SysMode_TEST != 1)    
    offtim1 = 0;
    offtim2 = 0;
    offtim3 = 0;
#endif

#if (Disp_ErrState == 1)        
    gui_DispErrState = 0;
    guc_RevCntA = 0;
    guc_RevCntB = 0;
    guc_RevCntC = 0;
#elif (Disp_ErrState == 2)    
    gul_dispbuf_total = 0;
    gul_dispbuf_timeoutA = 0;
    gul_dispbuf_timeoutB = 0;
    gul_dispbuf_timeoutC = 0;
    gul_dispbuf_crcerrA = 0;
    gul_dispbuf_crcerrB = 0;
    gul_dispbuf_crcerrC = 0;
#endif

    guc_ReadingABC_Flg = 1;  // 上电不去做相位和掉相判断 
//    guc_StartReadABC_Flg = 0;
    
    Accumulate_abc_times = 0;  //分电量累加定时
}

/*=========================================================================================\n
* @function_name: SysEventProc
* @function_file: main.c
* @描述: 系统事件处理流程
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-09)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysEventProc(void)
{
    if(gui_SystemEvent & flgEtGlb_PowDown)              
    {                                                   
        PowerDnPro();                  //掉电处理      //1122
        gui_SystemEvent &=~ flgEtGlb_PowDown;           
        gui_SystemSleepState=0x55aa;
                               //睡眠不成功，等待开门狗复位
    } 
    else if(gui_SystemEvent & flgEtGlb_ABCPhy)
    {        
        ABCPhyProc();                               //三相电量读取  
        gui_SystemEvent&=~flgEtGlb_ABCPhy;
    }
    else  if(gui_SystemEvent & flgEtGlb_ReadPower)
    {
        ReadbackABCPhy(gs_RacPhy.uc_Phystat);                  //电量获得
        gui_SystemEvent&=~flgEtGlb_ReadPower;   
        
    }
    else if(gui_SystemEvent & flgEtGlb_EnergyCalc)      
    {                                                   
        Eny_EnergyProc(active);                               // 有功能量事件
        gui_SystemEvent &=~ flgEtGlb_EnergyCalc;        
    } 
     else if(gui_SystemEvent & flgEtGlb_EnergyCalcN)      
    {                                                   
        Eny_EnergyProc(reactive);                               // 无功能量事件
        gui_SystemEvent &=~ flgEtGlb_EnergyCalcN;        
    } 
    else if(gui_SystemEvent & flgEtGlb_RefreshLcd)      
    {    
        ShowRefresh();                                  // 显示刷新
        gui_SystemEvent &=~ flgEtGlb_RefreshLcd;        
    }                                                   
    else if(gui_SystemEvent & flgEtGlb_NeedPrtclChk)    
    {                                                   
        ComApp_Proc();                                  // 通讯事件
        gui_SystemEvent &=~flgEtGlb_NeedPrtclChk;       
    }
    else if(gui_SystemEvent&flgEtGlb_ClearMeter)
    {
        Data_ClearMeter();            //zzp1122
        gui_SystemEvent&=~flgEtGlb_ClearMeter;
    }
    else if(gui_SystemEvent&flgEtGlb_ABC_Phase)
    {
#if (CHECK_RevPhase == 1)         
        Check_ABC_Phase();                   //相位测量
#endif
      gui_SystemEvent&=~flgEtGlb_ABC_Phase;
    }
 
    else
    {
        gui_SystemEvent = 0;                            //防止出错时,整个循环下面的处理永远被挂起
    }
}
/*=========================================================================================\n
* @function_name: TimeEventProc
* @function_file: main.c
* @描述: 系统时间事件处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-09)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
 uint32 * const ul_pRacReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacPAC,(uint32*)&gs_JbPm.gs_JBB.RacPAC,
                                        (uint32*)&gs_JbPm.gs_JBC.RacPAC};
void TimeEventProc(void)
{

    if(guc_TimeEvent & flgEtTim_Second)                 // 秒处理
    {    

        TimerProc();                         // 在停电和有电下均处理  
       EnyB_Open_Cf();//ZZP181121
       
#if (CHECK_RevPhase == 1)        
        Cal_BPFPARA_Value();                 //计算带通滤波器系数值
#endif
#if (Rec_PwrDown == 1) 
       CHKERR_Bit_scan(); //检测配置
#endif        

        ShowSecondProc();
        /////////////////////EnyB_Check_PowRev();                            //功率方向判断
        if(OSCSTATE())                //check crystal state 
        {
            guc_ShowErr = 1;
        }
        else
        {
            guc_ShowErr = 0;
        }

        guc_TimeEvent |= flgEtTim_SecSoft;
        guc_TimeEvent &= ~flgEtTim_Second;              //
    }
    else if(guc_TimeEvent&flgEtTim_Minute)
    {
         Data_MinProc();
         guc_TimeEvent&=~flgEtTim_Minute;
    }                                                   //
    else if(guc_TimeEvent & flgEtTim_Hour)              // 小时事件
    {                                                   //
        Data_HourProc();
        guc_TimeEvent &= ~flgEtTim_Hour;                //
    }else if(guc_TimeEvent & flgEtTim_Day)
    {
        Data_DayProc();
        guc_TimeEvent &=~flgEtTim_Day;
    }
    else if(guc_TimeEvent & flgEtTim_SecSoft)
    {   

        CF_TimCtrl();     //CF脉冲时间控制
        guc_TimeEvent &= ~flgEtTim_SecSoft;
    }
    else                                                //
    {                                                   //
        guc_TimeEvent = 0;                              //  
    }
}


/*=========================================================================================\n
* @function_name: RTCWakeUPFunc
* @function_file: main.c
* @描述: RTC唤醒处理
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 RTCWakeUPFunc(void)
{
    CLRWDT();
    SetPLL800K();
    //CoverScanSleep();                   //开盖检测 
    if(guc_PowOffRuning)                //唤醒状态
    {
        gs_DispStt.ulPwrOffTmr=0;       //停显时间清零
        
        if(SleepDisp())                 //休眠显示
        {
        }
        else
        {
            if(guc_PoweroffCoverJl==0x50)
            {
                //1122RTCWakeUpTm(InitPara38[1]);      //有电下是关表盖，停电开了表盖没有闭合，则分钟唤醒
            }else
            {
                //1122RTCWakeUpTm(Init_RTC_HOUR);      //1hour唤醒
            }
            SleepDispInit();            //关闭屏幕初始化显示指针
            LCD_Off();
            Sleep();
        }
    }                                   //小时唤醒
    else                                //普通休眠状态
    {
        if(guc_PoweroffCoverJl==0x50)
        {
            //1122RTCWakeUpTm(InitPara38[1]);      //有电下是关表盖，停电开了表盖没有闭合，则分钟唤醒
            //这里考虑一下是否要RTC补偿
            gui_WakeUpCnt++;
            if(gui_WakeUpCnt>=60)
            {
                gui_WakeUpCnt=0;
							/*
                CtrlADC6=0x08;	//开启M通道adc
                CtrlADC5=0x81;	//测温度
                EnyB_SetMeterCfgACK(0x18,PMCtrl1);
				//使能控制有效值计算及M通道数字信号输入
                DelayXms(15);
							*///zzp181120
                //CalRTC();
            }
        }else
        {
            //1122RTCWakeUpTm(Init_RTC_HOUR);          //1hour唤醒
					/*
            CtrlADC6=0x08;
            CtrlADC5=0x81;
            EnyB_SetMeterCfgACK(0x18,PMCtrl1);
            DelayXms(15);
					*///zzp181120
           // CalRTC();
        }
    }

    if(SleepRTC())
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*=========================================================================================\n
* @function_name: IOWakeUPFunc
* @function_file: main.c
* @描述: IO口唤醒处理
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 IOWakeUPFunc(void)
{
    CLRWDT();                           //喂狗
    Ioawkup_flag = 0;
    SetPLL800K();
    if(KeyStUp())
    {
        LCDInitSleep();                     //初始化LCD显示
        SleepTimeProc();                    //读取RTC时钟到RAM中,       
        if(guc_PowOffRuning==true)          //如果已经处在唤醒状态下
        {
            KeyDispSleepDown();             //检测按键
        }
        else
        {
            //唤醒后，轮显一轮
            gs_DispStt.ucMode = Const_DispMode_LX;
            gs_DispStt.uiIndex = 0;
            gs_DispStt.ucTmr=gs_DispCont.ucLxTm;
            gs_DispStt.ucPwrOffRefresh|=Const_RefreshAll;
            ShowRefresh();                  //立即刷新
            guc_PowOffRuning=true;          //当前在唤醒状态
        }
    }
    RTCWakeUpTm(Init_RTC_SEC);               //初始化为1S RTC唤醒
   // CoverScanSleep();                   //开盖检测
    if(SleepRTC())
    {
        return true;
    }
    else
    {    
        return false;
    }
}