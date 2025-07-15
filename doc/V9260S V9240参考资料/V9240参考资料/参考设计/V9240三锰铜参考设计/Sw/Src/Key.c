#define KEY_EXT
#include "Include.h"

/*=========================================================================================\n
* @function_name: KeyInit
* @function_file: Key.c
* @描述: 按键初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-14)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void KeyInit(void)
{
    uint8 i;
    KeyPortIn();                                        // 按键端口设置成输入模式
    for(i=0;i<KeyNums;i++)
    {
        gs_KeyCtr[i].KeyThd=0;
        gs_KeyCtr[i].LgKeyThd=200;
    }
}

/*=========================================================================================\n
* @function_name: KeyScan
* @function_file: Key.c
* @描述: 按键扫描
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void KeyScan(void)
{
    if(KeyStUp())                                   //上翻键延时开启
    {
        if((gs_KeyCtr[UpKey].DlyCnt>=gs_KeyCtr[UpKey].KeyThd)&&
           gs_KeyCtr[UpKey].Status==KeyRls)
        {
            //OpenBeiGuang();
            guc_DyBgLed = gs_DispCont.ucKeyBgTm;     // 设置背光延时时间
            guc_KeyEvent|=flgEtKey_Up;              // 置上翻键标记
            gs_KeyCtr[UpKey].Status=KeyDw;          //上翻键为按下状态
        }
        if( gs_KeyCtr[UpKey].Status !=KeyDWLong)
        {
            gs_KeyCtr[UpKey].DlyCnt++;              //延时计数器加1
        }
        if((gs_KeyCtr[UpKey].DlyCnt>=gs_KeyCtr[UpKey].LgKeyThd)&&
                 (gs_KeyCtr[UpKey].Status==KeyDw))
        {                                           //如果键状态为按下，并且延时计数大于2s为长按
            gs_KeyCtr[UpKey].Status=KeyDWLong;      //按键状态为长按
            //LongKeyProc();                          //长按键处理
        }
    }
    else
    {
        gs_KeyCtr[UpKey].DlyCnt=0;                  //延时计数清零
        gs_KeyCtr[UpKey].Status=KeyRls;             //按键为释放
    }

//    if(KeyStPr())                                   //上翻键延时开启
//    {
//        if((gs_KeyCtr[PrKey].DlyCnt>=gs_KeyCtr[PrKey].KeyThd)&&
//           gs_KeyCtr[PrKey].Status==KeyRls)
//        {
//            guc_KeyEvent|=flgEtKey_Pr;              // 置上翻键标记
//            gs_KeyCtr[PrKey].Status=KeyDw;          //上翻键为按下状态
//        }
//
//        if( gs_KeyCtr[PrKey].Status !=KeyDw)
//        {
//            gs_KeyCtr[PrKey].DlyCnt++;              //延时计数器加1
//        }
//
////      if((gs_KeyCtr[PrKey].DlyCnt>=gs_KeyCtr[PrKey].LgKeyThd)&&
////               (gs_KeyCtr[PrKey].Status==KeyDw))
////      {                                           //如果键状态为按下，并且延时计数大于2s为长按
////          gs_KeyCtr[PrKey].Status=KeyDWLong;      //按键状态为长按
////          //LongKeyProc();                          //长按键处理
////      }
//    }
//    else
//    {
//        gs_KeyCtr[PrKey].DlyCnt=0;                  //延时计数清零
//        gs_KeyCtr[PrKey].Status=KeyRls;             //按键为释放
//    }

}

/*=========================================================================================\n
* @function_name: LongKeyProc
* @function_file: Key.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void LongKeyProc(void)
{
/*    if(gs_LoadSwichStatus.SwitchActionFlag)
    {
        if(gs_LoadSwichStatus.SwitchAction==0)
        { 
            guc_LongKeyFlg=0x55;
        }
    }
*/
}
 
/*=========================================================================================\n
* @function_name: CoverKeyScan
* @function_file: Key.c
* @描述: 开盖键检测
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*void CoverKeyScan(void)
{
    if(KeyStBg())
    {
        guc_CoverKeyUpCnt=0;
        guc_CoverKeyDwCnt++;
        if(guc_CoverKeyDwCnt>2)
        {
            if(guc_CoverStatus==0)
            {
                CoverOpenLog();
                gul_CoverUpCnt++;
            }
            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=true;
            guc_CoverStatus=1;
            guc_PoweroffCoverJl=0x50;                                               //有电下开表盖
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == false      //状态标志未置
               && gs_ReportWord.ui_ReportWord1.ReportWord1.CoverRpt == 1)           //需要主动上报
            {
                gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = true;         //状态置位
            }
        
        #ifdef FRCom
            if(gs_SysRunSt.OpenGapFRFlag==false)                        //有事件发生，但没有超收
            {
//              EVENTOUTHIGH();
                gs_SysRunSt.OpenGapFRFlag=true;
//              gs_FREventOut.FREventOutStruct.OpenGapFRFlag=true;
            }
        #endif    
        }
    }
    else
    {
        guc_CoverKeyUpCnt++;
        guc_CoverKeyDwCnt=0;
        if(guc_CoverKeyUpCnt>2)
        {
            if(guc_CoverStatus==1)
            {
                CoverCloseLog();
            }
    
            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=false;
            guc_PoweroffCoverJl=0x55;           //有电下关表盖
    
            guc_CoverStatus=0;
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == true)     //判断状态标志
            {
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = false;
            }
            gs_SysRunSt.OpenGapFRFlag=false;
        }
    }
}*/
/*=========================================================================================\n
* @function_name: CoverScanSleep
* @function_file: Key.c
* @描述: 掉电检测开表盖
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*void CoverScanSleep(void)
{
    if(KeyStBg())
    {
        if(guc_PoweroffCoverJl==0x55)   //有电下是关表盖的则记录
        {
            if(guc_CoverStatus==0)      //有电下是关表盖的则只要记录开表盖
            {
                CoverOpenLog();
            }
//          RTCWakeUpTm(RTC_MIN);      //切换到min唤醒

            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=true;
            guc_PoweroffCoverJl=0x50;
            guc_CoverStatus=1;
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == false      //状态标志未置
               && gs_ReportWord.ui_ReportWord1.ReportWord1.CoverRpt == 1)           //需要主动上报
            {
                gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = true;         //状态置位
            }
        }
    }
    else    
    {
        if(guc_PoweroffCoverJl==0x50)   //有电下是关表盖，并且停电下开了表盖
        {
            if(guc_CoverStatus==1)
            {    
                CoverCloseLog();
            }

            if(!guc_PowOffRuning)
            {
                RTCWakeUpTm(RTC_HOUR);      //切换到小时唤醒
            }

            guc_CoverStatus=0;
            guc_PoweroffCoverJl=0;      //不在记录开表盖事件

            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=false;
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == true)     //判断状态标志
            {
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = false;
            }
        }
    } 
}*/
/*=========================================================================================\n
* @function_name: KeyDispSleepDown
* @function_file: Key.c
* @描述: 休眠时显示按键扫描
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void KeyDispSleepDown(void)
{  
    if(gs_KeyCtr[UpKey].Status==KeyRls)
    {
        if(KeyStUp())
        {
            gs_KeyCtr[UpKey].Status=KeyDw;
            ShowKeyManage(Const_Disp_KeyUp);            // 上翻阅按键
        }
    }
    else    
    {
        if(!KeyStUp())
        {
            gs_KeyCtr[UpKey].Status=KeyRls;
            gs_KeyCtr[UpKey].DlyCnt=0;
        }
    }
}
/*=========================================================================================\n
* @function_name: KeyProc
* @function_file: Key.c
* @描述: 按键处理函数
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-02-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void KeyProc(void)
{

    if(guc_KeyEvent & flgEtKey_Up)
    {
        ShowKeyManage(Const_Disp_KeyUp);            // 上翻阅按键
        guc_KeyEvent &=~ flgEtKey_Up;               // 清除标志
    }
//    else if(guc_KeyEvent & flgEtKey_Pr)                  // 编程状态考虑
//    {                                               //
//        if(gui_SystemState & flgStSys_Program)      // 首先判别是否处于编程状态
//        {
//            guc_PrKeyNewDown=0;
//            gui_SystemState &= ~flgStSys_Program;   // 已经处于编程状态，则清除标志
//            gui_DyPgmOut = 0x00;                     // 编程超时退出时间清零
//            gs_MeterState.ui_MeterState3.RunCode3Word.Program=false;
//        }                                           //
//        else                                        //
//        {                                           //
//            gui_SystemState |= flgStSys_Program;    // 设置编程状态标志
//            guc_PrKeyNewDown=0x55;
//            BE_ReadP(EEP_PROTIME,(uint8*)&gui_DyPgmOut,0x02);       // 编程超时退出时间设置
//            if(gui_DyPgmOut==0xffff)                                // 如果E2没有初始化的话，默认为240分钟
//            {
//                gui_DyPgmOut=240;
//            }
//            gs_MeterState.ui_MeterState3.RunCode3Word.Program=true;
//        }                                           //
//        guc_KeyEvent &= ~flgEtKey_Pr;               //
//
//
//        BE_WriteP(EEP_PRGLEFTTM,(uint8*)&gui_DyPgmOut,0x02);    //写入编程剩余时间
//    }                                               //
    else                                            //
    {                                               //
        guc_KeyEvent =0x0;                          //  防止其他位设置起来，频繁进入该事件
    }                                               //
}
/*=========================================================================================\n
* @function_name: KeyPgm_DyOut
* @function_file: Key.c
* @描述: 编程时间退出计算
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-06)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void KeyPgm_DyOut(void)
{
    if(gui_DyPgmOut>0)
    {
        gui_DyPgmOut--;
        if(gui_DyPgmOut==0)
        {
            gui_SystemState &= ~flgStSys_Program;                   // 设置编程状态标志
            BE_WriteP(EEP_PRGLEFTTM,(uint8*)&gui_DyPgmOut,0x02);    //写入编程剩余时间
            gs_MeterState.ui_MeterState3.RunCode3Word.Program=false;
        }
    }

    if(gui_DyProDmOut>0)
    {
        gui_DyProDmOut--;
        if(gui_DyProDmOut==0)
        {
            guc_FactoryType=0;                                          // 退出厂内状态
            BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);  //厂内剩余时间
            SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);  //写入厂内状态           
        }
    }

}
