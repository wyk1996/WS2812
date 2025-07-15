#define LCDSHOWEXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: ShowSysStart
* @function_file: Lcdshow.c
* @描述: 显示上电初始化
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowSysStart(void)
{
    ShowInitial();                  //显示参数初始化
    guc_DyBgLed = gs_DispCont.ucFullShowTm;         //上电背光时间跟全屏时间一样
    gul_ShowState= 0x00;
    gs_DispStt.uiIndex = 1;         //全屏显示
    gs_DispStt.ucMode = Const_DispMode_Full;
    gs_DispStt.ucTmr = gs_DispCont.ucFullShowTm;    //持续时间5s
}

/*=========================================================================================\n
* @function_name: ShowInitial
* @function_file: Lcdshow.c
* @描述: 显示初始化
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowInitial(void)
{
    SysE2ReadData(EEP_MAX_LX_NUM,&gs_DispCont.ucLxCnt,sizeof(S_DISPCONTENT)-2);//读取显示项

//    FCpyTMem( &gs_DispCont.ucLxCnt, InitPara3+33, sizeof(S_DISPCONTENT)-2);
    gs_DispCont.ul_CRC=do_CRC(&gs_DispCont.ucLxCnt,sizeof(S_DISPCONTENT)-2);       //计算显示项的crc
}
/*=========================================================================================\n
* @function_name: ShowKeyManage
* @function_file: Lcdshow.c
* @描述: 按键数据显示处理
* 
* 
* @参数: 
* @param:ucType  显示类别, Const_Disp_KeyUp-上翻, Const_Disp_KeyDn-下翻
* @返回: 
* @作者:   lwb (2012-05-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ShowKeyManage(uint8 ucType)
{
    //如果是停电下，就不需要显示错误字
    if(!_SYS_IS_PWR_OFF)                            //有电下进行错误字显示
    {
//      if(guc_ErrBtnFlag)                          //需要显示错误状态字
//      {
//          guc_ErrSwPt++;
//          if(guc_ErrSwPt>=guc_ErrCodeCnt)
//          {
//              guc_ErrSwPt=0;
//              guc_ErrBtnFlag=false;
//              guc_ShowErrEndFlag=true;
//
//          }else
//          {
//              gs_DispStt.ucMode=Const_DispMode_Err;
//              gs_DispStt.ucTmr = gs_DispCont.ucJxTm;
//              gs_DispStt.ucPwrOffRefresh |= Const_RefreshAll;
//              ShowRefresh();                                  //立即刷新
//              return;
//          }
//      }
        gs_DispStt.ucTmr = gs_DispCont.ucJxTm;
    }
    else
    {
        gs_DispStt.ucTmr = gs_DispCont.ucJxPowOffTm;
    }
    
    if(gs_DispCont.ucJxCnt == 0)
    {                                       //键显表为0
        gs_DispCont.ucJxCnt = 1;
    }

    if(gs_DispStt.ucMode != Const_DispMode_JX)
    {                                       //由非键显状态进入键显状态
        gs_DispStt.ucMode = Const_DispMode_JX;
        gs_DispStt.uiIndex = 0;
//      if(guc_ShowErrEndFlag!=true)
//      {
//          gs_DispStt.uiIndex = 0;
//
//          if(gut_SysWarn.byte)                                //如果告警字不为0的话
//          {
//              if(gs_DispStt.uiIndex == gs_DispCont.uc_ShowErrPst)
//              {
//                  gs_DispStt.ucMode=Const_DispMode_Err;
//                  guc_ErrSwPt=0;
//                  guc_ErrBtnFlag=true;
//
//              }
//          }
//      }
    }
    else
    {                                                               //本来就在键显状态
        gs_DispStt.uiIndex++;
        if(gs_DispStt.uiIndex >= gs_DispCont.ucJxCnt)
        {
//          if(gs_DispCont.uc_ShowErrPst>=gs_DispCont.ucJxCnt
//             || gs_DispCont.uc_ShowErrPst==0)
//          {
//              if(gut_SysWarn.byte)                               //如果告警字不为0的话
//              {
//                  gs_DispStt.ucMode=Const_DispMode_Err;
//                  guc_ErrSwPt=0;
//                  guc_ErrBtnFlag=true;
//              }
//          }
            gs_DispStt.uiIndex = 0;
//          guc_ShowErrEndFlag=false;
        }
//      else if(gs_DispStt.uiIndex == gs_DispCont.uc_ShowErrPst)
//      {
//          if(gut_SysWarn.byte)                                //如果告警字不为0的话
//          {
//              gs_DispStt.ucMode=Const_DispMode_Err;
//              guc_ErrSwPt=0;
//              guc_ErrBtnFlag=true;
//          }
//      }
    }

    gs_DispStt.ucPwrOffRefresh |= Const_RefreshAll;
    ShowRefresh();                                  //立即刷新
}
/*=========================================================================================\n
* @function_name: CheckErrorCode
* @function_file: Lcdshow.c
* @描述: 检测当前是否需要显示错误字
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-19)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CheckErrorCode(void)
{
    if(gut_SysWarn.byte==0)                            //如果告警字为0的话,直接返回
    {
        return;
    } 

//  if(gut_SysWarn.SysWarn.REVPOWER)
//  {
//      guc_ErrRevPowerSwTm++;
//      if(guc_ErrRevPowerSwTm>9)
//      {
//          gut_SysWarn.SysWarn.REVPOWER=false;
//          guc_ErrRevPowerSwTm=0;
//          gs_DispStt.ucTmr=1;             //下秒切换到轮显
//      }
//  }

    guc_ErrCodeCnt=0;
 
    if(gut_SysWarn.SysWarn.BatLow)
    {
        guc_SwErrCode[guc_ErrCodeCnt]=0x04;
        guc_ErrCodeCnt++;
    }
    if(gut_SysWarn.SysWarn.EEPROMERROR)
    {
        guc_SwErrCode[guc_ErrCodeCnt]=0x06;
        guc_ErrCodeCnt++;
    }
 
    if(gut_SysWarn.SysWarn.TimeError)
    {
        guc_SwErrCode[guc_ErrCodeCnt]=0x08;
        guc_ErrCodeCnt++;
    }
}



/*=========================================================================================\n
* @function_name: ShowSecondProc
* @function_file: Lcdshow.c
* @描述: 一秒钟调用一次,分系统掉电N天以后、系统掉电N天之内、系统有电三种情况处理,它们之间的
*       关系是互斥的这里的状态切换中有一个问题,就是当在凌晨x时到y时之间如果按键的话,当按键
*       退出时,会进入到轮显状态或停显状态,但是最多过1min就会又进入关闭显示状态.
* @参数:
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowSecondProc(void)
{

    gui_SystemEvent |= flgEtGlb_RefreshLcd;             //设置LCD刷新

    guc_PrOutTm++;
    if(guc_PrOutTm>=60)
    {
        guc_PrOutTm=0;
        KeyPgm_DyOut();                                 //编程按键的退出
//      PwBs_DyOut();
    }
    
        
//  CheckErrorCode();
//    ShowStateFlashManage();

    if(guc_ShowStCom>0)
    {
        guc_ShowStCom--;
        if(guc_ShowStCom==0)
        {
          gul_ShowState &= ~flgShowSt_Com;
        }
    }
    
    if(gs_DispStt.ucTmr != 0)
    {
        gs_DispStt.ucTmr--;
        if(gs_DispStt.ucTmr>0)                          //判断各显示状态定时是否到,需要进行状态切换
        {
            return; 
        }
    }

//  if(gut_SysWarn.byte)                            //如果告警字不为0的话,显示错误字
//  {
//      if(gs_DispStt.ucMode!=Const_DispMode_Err)   //如果不在错误模式的话，切换到错误模式
//      {
//          gs_DispStt.ucMode=Const_DispMode_Err;
//          guc_ErrSwPt=0;                          //显示第一个错误字
//      }else
//      {
//          guc_ErrSwPt++;                          //已经在错误模式的话，显示下一个错误字
//          if(guc_ErrSwPt>=guc_ErrCodeCnt)
//          {
//              guc_ErrSwPt=0;
//          }
//      }
//
//      gs_DispStt.ucTmr = gs_DispCont.ucLxTm;
//      return;
//  }
                                             //系统有电状态下工作
    switch(gs_DispStt.ucMode)
    {
    case Const_DispMode_LX:
        gs_DispStt.ucTmr = gs_DispCont.ucLxTm;
        gs_DispStt.uiIndex++;
        if(gs_DispStt.uiIndex >= gs_DispCont.ucLxCnt)
        {
            gs_DispStt.uiIndex = 0;                     //屏号复归到0
        }
        break;
    default:
                                                        //复位到轮显状态
        gs_DispStt.ucTmr = gs_DispCont.ucLxTm;
        gs_DispStt.ucMode = Const_DispMode_LX;
        gs_DispStt.uiIndex = 0;
        break;
    }
    return;
}
 
/*=========================================================================================\n
* @function_name: ShowStateManage
* @function_file: Lcdshow.c
* @描述: 各类状态位的显示准备
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowStateManage(void)
{

    if(!_SYS_IS_PWR_OFF)
    {
        /*gul_ShowState &= ~(flgShowSt_UA+flgShowSt_UB//先清除当前状态，停电下也不显示
                       +flgShowSt_UC+flgShowSt_IA
                       +flgShowSt_IB+flgShowSt_IC
                       +flgShowSt_RPvs+flgShowSt_PRvs
                       +flgShowSt_NIA+flgShowSt_Pgm);*/
           gul_ShowState = 0;
       /* gul_ShowState |=BIT8MAP[gs_FeeData.uc_FeeNo];       //费率显示
        if(gs_FeeData.uc_FeeRSDN == 2)                      //时段表显示
        {
            gul_ShowState |= flgShowSt_YQ2;
        }else
        {
            gul_ShowState |= flgShowSt_YQ1;
        }*/
       if(gs_phytim.RevPhaseFlg)
//       if((gs_phytim.CPhytim *3600/43690)<2000 && (gs_phytim.CPhytim *3600/43690)>1000) //逆相序判断
       {        
            gul_ShowState |= flgShowSt_RPvs;
        }
       else
       {
            gul_ShowState &= ~ flgShowSt_RPvs;
        }
       
       if(guc_ShowStCom)       //通讯标志
       {
            gul_ShowState |= flgShowSt_Com;
        }
       else
       {
            gul_ShowState &= ~ flgShowSt_Com;
        }
       
        if(CalRMS2(UAVG,gs_UI.ul_U[0]) >0x0500)
        {
            gul_ShowState |= flgShowSt_UA;
        }
        else
        {
            gul_ShowState &= ~flgShowSt_UA;
        }
        if(CalRMS2(UAVG,gs_UI.ul_U[1]) >0x0500)
        {
            gul_ShowState |= flgShowSt_UB;
        }
        else
        {
            gul_ShowState &= ~flgShowSt_UB;
        }
        if(CalRMS2(UAVG,gs_UI.ul_U[2])>0x0500)
        {
            gul_ShowState |= flgShowSt_UC;
        }
        else
        {
            gul_ShowState &= ~flgShowSt_UC;
        }
        
        if((CalRMS2(IAAVG,gs_UI.ul_I[0]) >0x0020)
           &&(gul_ShowState&flgShowSt_UA))
        {
            gul_ShowState |= flgShowSt_IA;
        }
        else
        {
            gul_ShowState &= ~flgShowSt_IA;
        }
        if((CalRMS2(IAAVG,gs_UI.ul_I[1]) >0x0020)
            &&(gul_ShowState&flgShowSt_UB))
        {
            gul_ShowState |= flgShowSt_IB;
        }
        else
        {
            gul_ShowState &= ~flgShowSt_IB;
        }

        if((CalRMS2(IAAVG,gs_UI.ul_I[2]) >0x0020)
           &&(gul_ShowState&flgShowSt_UC))
        {
            gul_ShowState |= flgShowSt_IC;
        }
        else
        {
            gul_ShowState &= ~flgShowSt_IC;
        }
       
        if(guc_batstate ==BATDOWN)
        {
             gul_ShowState|=flgShowSt_Bat;
        }
        else
        {
            gul_ShowState&=~flgShowSt_Bat;
        }
        
        if(guc_FactoryType == FactorMd)                     // 是否生产模式
        {
            //gul_ShowState ^= flgShowSt_Pgm;                 // 生产模式中编程显示取反
             gul_ShowState |= flgShowSt_Pgm;  
        }
       // else if(gui_SystemState&flgStSys_Program)         // 是否编程状态
       // {
       //     gul_ShowState |= flgShowSt_Pgm;                 // 编程显示
       // }
        else                                                // 不是编程状态
        {
            gul_ShowState &= ~flgShowSt_Pgm;                // 取消编程显示
        }

        if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir)  //功率反向显示
        {
            gul_ShowState|=flgShowSt_PRvs;
        }

//        if(gs_MeterState.ui_MeterState1.RunCode1Word.MeterCell==true)   //电池报警
//        {
//            gul_ShowState|=flgShowSt_Bat;
//        }else
//        {
//            gul_ShowState&=~flgShowSt_Bat;
//        }
    }else
    {
        gul_ShowState=0;                    //停电下不显示状态
    }

//    if(guc_EsamSuspend==true)
//    {
//        gul_ShowState|=flgShowSt_Lock;
//    }
//    else
//    {
//        gul_ShowState&=~flgShowSt_Lock;
//    }
//
//    if((gs_EsamKeyUpDateCtrl.W32Status.lword&EsamKeyRevOkTab[gs_EsamKeyUpDateCtrl.ucTotal-1])
//        ==EsamKeyRevOkTab[gs_EsamKeyUpDateCtrl.ucTotal-1])
//    {
//        gul_ShowState&=~flgShowSt_FZ;
//    }else
//    {
//
//        gul_ShowState|=flgShowSt_FZ;
//    }
}
/*=========================================================================================\n
* @function_name: ShowStateFlashManage
* @function_file: Lcdshow.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ShowStateFlashManage(void)
{

//    if(gs_LoadSwichStatus.ControlWarning)               //报警灯
//    {
//        gul_ShowState ^= flgShowSt_QGD;
//    }else
//    {
//        gul_ShowState &=~ flgShowSt_QGD;
//    }
//     //拉合闸相关符号
//    if(gs_LoadSwichStatus.SwitchActionFlag)
//    {
//        if(gs_LoadSwichStatus.SwitchAction)             //拉闸
//        {
//            gul_ShowState ^= flgShowSt_LZ;
//        }else
//        {
//            gul_ShowState&=~flgShowSt_LZ;
//        }
//    }
//    else
//    {
//        if(gs_LoadSwichStatus.SwitchAction)
//        {
//            gul_ShowState|=flgShowSt_LZ;
//        }else
//        {
//            gul_ShowState&=~flgShowSt_LZ;
//        }
//    }


 }
/*=========================================================================================\n
* @function_name: ShowNumManage
* @function_file: Lcdshow.c
* @描述:
*
*
* @参数:
* @param:ucAtb  相当于显示表中关于数据属性中符号,BCD2HEX,消隐部分的内容,其状态位与表中属性状态的定义位一致
* @param:ucType  相当于单位小数点格式表的位置,某种类型的变量其显示格式必定与这个表中的某一个成员对应
* @param:pucData  为显示的数据,有效长度由全局变量ucLen指定
* @param:ucLen
* @返回:
* @作者:   lwb (2012-03-19)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowNumManage(uint8 ucAtb, uint8 ucType, uint8* pucData)
{
    uint32  ulNum;
    uint32  ulMsk;
    uint8 * pucNum;
    uint8 * pucMsk;
    uint8 ucMskLen;
    uint8 i;
    uint8 ucDocNum;

    ulNum = Uint8_To_Uint32(pucData);
    if(ucAtb & Const_DisppHEX2BCD)
    {
        ulNum = Hex2BCD(ulNum);
    }

    ulMsk=0;
    if(ucType==0x01)      //时间
    {
        ulNum>>=8;
        ulMsk = 0xAA000000L;
    }else if(ucType==0x02 || ucType==0x12 || ucType==0x11)//日期
    {
        ulMsk = 0xAA000000L;
    }
    else if(ucType==0x03 ||ucType==0x0D )
    {
        //读取小数点位数
        if(guc_ShowEnergyFlag==0x55)
        {
            if(ulNum&0x80000000)        //负电量
            {
                ulNum&=~0x80000000;
            }
           // gul_ShowState|=flgShowSt_Fuh;
        }

        BE_ReadP(EEP_EDEC,&ucDocNum,1);
        if(ucDocNum==0)
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)
            {
                ulNum>>=8;
            }else
            {
                ulMsk=0x000000AAL;
            }
        }
        else if(ucDocNum==1)
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)
            {
                ulNum>>=4;
                gs_LcdDisp.uiUnitPt1 |= Point_D1;
            }else
            {
                gs_LcdDisp.uiUnitPt1 |= Point_D2;
                ulMsk=0x0000000AL;
            }
        }else
        {
            gs_LcdDisp.uiUnitPt1 |= Point_D2;
        }

        if(gs_FunCfg.uc_ENSHOWCfg&BIT2)                 //电量高位需要消隐
        {
            ucAtb &=~ Const_DispNeedMsk;                //不需要消隐
        }else
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)             //1位小数时需要右移
            {
                if(ucDocNum==0)                         //0位小数的时候，消隐7位
                {
                    ucMskLen=7;
                }else
                if(ucDocNum==1)                         //1位小数的时候，消隐6位
                {
                    ucMskLen=6;
                }else
                {
                    ucMskLen=5;
                }
            }else
            {
                ucMskLen=5;
            }
            ucAtb |= Const_DispNeedMsk;     //需要消隐
       }
    }else if(ucType==0x13||ucType==0x0b)
    {
        ulMsk = 0xAAAA0000L;
    }else if(ucType==0x04  || ucType==0x0E)                  //电压是6
    {
        ucMskLen=6;
    }else if(ucType==0x05  ||ucType==0x0F)                  //电流，零线电流
    {
        if(ulNum&0x800000)                  //负功率
        {
            ulNum&=~0x800000;
            //gul_ShowState|=flgShowSt_Fuh;
        }
        ucMskLen=4;
    }else if(ucType==0x06 ||ucType==0x0C )                  //功率
    {
        if(ulNum&0x800000)                  //负功率
        {
            ulNum&=~0x800000;
            //gul_ShowState|=flgShowSt_Fuh;
        }

        BE_ReadP(EEP_PDEC,&ucDocNum,1);
        if(ucDocNum==0)
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)
            {
                ulNum>>=16;
            }else
            {
                ulMsk=0x0000AAAAL;
            }
        }
        else if(ucDocNum==1)
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)
            {
                ulNum>>=12;
                gs_LcdDisp.uiUnitPt1 |= Point_D1;
            }else
            {
                gs_LcdDisp.uiUnitPt1 |= Point_D4;
                ulMsk=0x00000AAAL;
            }
        }else if(ucDocNum==2)
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)
            {
                ulNum>>=8;
                gs_LcdDisp.uiUnitPt1 |= Point_D2;
            }else
            {
                gs_LcdDisp.uiUnitPt1 |= Point_D4;
                ulMsk=0x000000AAL;
            }
        }else if(ucDocNum==3)
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)
            {
                ulNum>>=4;
                gs_LcdDisp.uiUnitPt1 |= Point_D3;
            }else
            {
                gs_LcdDisp.uiUnitPt1 |= Point_D4;
                ulMsk=0x0000000AL;
            }
        }
        else
        {
            gs_LcdDisp.uiUnitPt1 |= Point_D4;
        }

        if(gs_FunCfg.uc_ENSHOWCfg&BIT2)                 //电量高位需要消隐
        {
            ucAtb &=~ Const_DispNeedMsk;                //不需要消隐
        }else
        {
            if(gs_FunCfg.uc_ENSHOWCfg&BIT1)             //1位小数时需要右移
            {
                if(ucDocNum==0)                         //0位小数的时候，消隐7位
                {
                    ucMskLen=7;
                }else
                if(ucDocNum==1)                         //1位小数的时候，消隐6位
                {
                    ucMskLen=6;
                }else
                if(ucDocNum==2)                         //2位小数的时候，消隐6位
                {
                    ucMskLen=5;
                }else
                if(ucDocNum==3)                         //3位小数的时候，消隐6位
                {
                    ucMskLen=4;
                }
                else
                {
                    ucMskLen=3;
                }
            }else
            {
                ucMskLen=3;
            }
            ucAtb |= Const_DispNeedMsk;     //需要消隐
       }
    }else if(ucType==0x07)                  //功率因数
    {
        if(ulNum&0x8000)                  //负功率
        {
            ulNum&=~0x8000;
           // gul_ShowState|=flgShowSt_Fuh;
        }
        ucMskLen=4;
    }

    if (ucAtb & Const_DispNeedMsk)
    {

        pucNum = ((uint8*)&ulNum)+3;
        pucMsk = ((uint8*)&ulMsk)+3;
        for(i=0; i<ucMskLen; i++)
        {//先判断高位字符,再判断低位字符
            if(i & 1)
            {//奇数,对应字符在字节中的位置为低位
                if(*pucNum & 0x0F)
                {//不为零,则退出
                    break;
                }
                else
                {//为0,则设置消隐
                    *pucMsk |= 0x0A;
                }
                pucNum--;           //过了一个字节
                pucMsk--;
            }
                else
                {//偶数,对应字符在字节的高位
                    if(*pucNum & 0xF0)
                    {//不为零,则退出
                        break;
                    }
                    else
                    {//为0,则设置消隐
                        *pucMsk |= 0xA0;
                    }
                }
        }
    }
    gs_LcdDisp.ulNum1 = ulNum;
    gs_LcdDisp.ulNum1Msk = ulMsk;
}
/*=========================================================================================\n
* @function_name: SearchDispNO
* @function_file: Lcdshow.c
* @描述: 通过数据标识查找屏号
*
*
* @参数:
* @param:ulID
*
* @返回:
* @return: uint16
* @作者:   lwb (2012-03-19)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 SearchDispNO(Word32 ulID)
{
	uint8 i;
	
    if(ulID.byte[3]==0x00)                                  //电量类
    {
        for(i=0;i<guc_LenOfTabConvent;i++)
        {
            if(gCs_TabConvent[i].ulInID==ulID.lword)
            {
                return gCs_TabConvent[i].uiNumb;
            }
        }
    }else if(ulID.byte[3]==0x02)
    {
        for( i=0;i<guc_LenOfTabConvBL;i++)
        {
            if(gCs_TabConvBL[i].ulInID==ulID.lword)
            {
                return gCs_TabConvBL[i].uiNumb;
            }
        }
    }else if(ulID.byte[3]==0x03)
    {
        for( i=0;i<guc_LenOfTabAdd;i++)
        {
            if(gCs_TabAdd[i].ulInID==ulID.lword)
            {
                return gCs_TabAdd[i].uiNumb;
            }
        }

    }else if(ulID.byte[3]==0x04)
    {
        for( i=0;i<guc_LenOfTabConvCS;i++)
        {
            if(gCs_TabConvCS[i].ulInID==ulID.lword)
            {
                return gCs_TabConvCS[i].uiNumb;
            }
        }
    }
    return 0xff;    //无效屏
}

/*=========================================================================================\n
* @function_name: ShowDataProc
* @function_file: Lcdshow.c
* @描述: 显示数据的处理
*
* @参数:
*
* @返回:
* @return: uint8 包括Const_Ret_OK / Const_Ret_Err / Const_Ret_Disp_Off / Const_Ret_DispFull
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
uint8 ShowDataProc(void)
{
    uint8   ucRet = Const_Ret_OK;
    uint8   ucUnitPtPos;
    uint8   ucAtb;
    uint16  uiAddr;                                         //E2中各类显示相关表的地址
    uint8   ucDispNo;                                       //屏号
    uint8   ucShowIdPos;
    S_LCDDATEINFO psRet;                                    //用于接收数据处理返回结果

    memset((uint8*)&psRet, 0, sizeof(S_LCDDATEINFO));
    uiAddr = gs_DispStt.uiIndex *5;                         //根据序号获取屏号地址

    if(gs_DispStt.ucMode==Const_DispMode_LX
       ||gs_DispStt.ucMode==Const_DispMode_TX)
    {
        uiAddr += EEP_LXTABLE;
        BE_ReadP(uiAddr, XDATA_RAMZONE,5);
    }else if(gs_DispStt.ucMode==Const_DispMode_JX)
    {
        uiAddr += EEP_JXTABLE;
        BE_ReadP(uiAddr, XDATA_RAMZONE,5);

    }else if(gs_DispStt.ucMode==Const_DispMode_Full)
    {
        return Const_Ret_DispFull;
    }
    else if(gs_DispStt.ucMode==Const_DispMode_IRDA)
    {
        memcpy(XDATA_RAMZONE,guc_IrShowLcdId,5);       //数据内容
        //通讯ID处理
    }else if(gs_DispStt.ucMode==Const_DispMode_Err)
    {
        ucRet = Const_Ret_DispMode_Err;
    }else
    {
        ucRet = Const_Ret_Err;      //其它状态不用进行显示操作,此时应该是显示被关闭状态
    }

    if(ucRet != Const_Ret_OK)
    {                               //不在下面进行数据组织,直接进入高级显示或特殊显示方式
        return ucRet;
    }

    psRet.uiID.lword=Uint8_To_Uint32(XDATA_RAMZONE);
    gul_ShowId=psRet.uiID.lword;            //停电显示用
    ucDispNo=SearchDispNO(psRet.uiID);
    ucShowIdPos=XDATA_RAMZONE[4];
    if(ucShowIdPos==0x01)
    {
        ucDispNo++;
    }

    if((ucDispNo == 0) || (ucDispNo > Const_MaxDispTabNum))
    {   //屏号超过显示表的容限,显示正向有功总
        ucDispNo = DEFAULT_ENERGY_NO;
        psRet.uiID.lword=0;

    }

    gs_LcdDisp.uiStr1 = gCs_DispTab[ucDispNo].uiStr;
                                    //得到显示的中文字符串信息,用于驱动层查表
    ucUnitPtPos = gCs_DispTab[ucDispNo].ucAtb & Const_DispUnitPt;
                                    //用于查单位和小数点表
    gs_LcdDisp.uiUnitPt1 = gCui_UnitPointTab[ucUnitPtPos];
                                    //得到显示单位和小数点

    ucAtb = gCs_DispTab[ucDispNo].ucAtb & (Const_DispNeedMsk + Const_DispSigned + Const_DisppHEX2BCD);
    psRet.ucCmd = 0x11;
    psRet.ucAtb |= Const_Atb_ShowData;
    psRet.ucbuf=XDATA_RAMZONE;
    memset(XDATA_RAMZONE,0x00,14);
    guc_ShowEnergyFlag=0;
    RWData((S_FRAMEINFO*)&psRet);
                                    //得到显示数据同时得到数据长度,用变量psRet->ucLen传递
    if(psRet.ucErr.word != Const_Data_OK)
    {
        ucRet = Const_Ret_Err;
        return ucRet;
    }
    if(ucUnitPtPos==0x11)           //校表日期
    {
        psRet.ucbuf+=4;
    }else if(ucUnitPtPos==0x13)           //表号，通讯地址，客户编号
    {
        psRet.ucbuf+=4;
    }else if(ucUnitPtPos==0x12)           //编程日期，校表日期
    {
        if(ucDispNo == 0x29)
        {
            psRet.ucbuf+=7;
        }
        else
        {
            psRet.ucbuf+=3;
        }
    }

    ShowNumManage(ucAtb, ucUnitPtPos, psRet.ucbuf);
    return ucRet;
}

/*=========================================================================================\n
* @function_name: ShowRefreshTime
* @function_file: Lcdshow.c
* @描述: 停电情况下显示时间时的直接更新,这里不去数据处理得到时间是出于停电时功耗考虑
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowRefreshTime(void)
{
    GetSysClock((uint8*)&gs_LcdDisp.ulNum1, Const_hhmmss);
    return;
}
extern uint32_t y;
/*=========================================================================================\n
* @function_name: ShowRefresh
* @function_file: Lcdshow.c
* @描述: 显示刷新
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-07)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void ShowRefresh(void)
{
    uint8 ucRet = Const_Ret_OK;
    uint8 ucType = Const_NeedRefresh;

    ShowStateManage();
    if(_SYS_IS_PWR_OFF)
    {                                           //系统掉电
        if(gs_DispStt.ucPwrOffRefresh & Const_RefreshAll)
        {                                       //所有数据需要更新
            ucRet = ShowDataProc();
        }
        else if(gs_DispStt.ucPwrOffRefresh & Cosnt_RefreshTime)
        {                                       //时间需要刷新
            ShowRefreshTime();
        }else
        {
            ucType = Const_NoNeedRefresh;       //不切换屏，不显示时间的换，不需要刷新
        }
    }
    else
    {                                           //有电
        ucRet = ShowDataProc();
    }

    switch(ucRet)
    {
    case Const_Ret_OK:                          //如果上面返回这个值,则显示数据已经组织好
        break;
    case Const_Ret_Err:                         //display error,显示"------"
        gs_LcdDisp.ulNum1Msk = 0x11111111L;
        break;
    case Const_Ret_DispFull:                    //全屏显示
        Disp_Full();
        ucType = Const_NoNeedRefresh;
        break;
    case Const_Ret_DispMode_Err:

        if(gut_SysWarn.SysWarn.EsamJMError
           ||gut_SysWarn.SysWarn.MACError
           )
        {
            gs_LcdDisp.ulNum1=0x10;
        }else if(gut_SysWarn.SysWarn.EsamKeyError)
        {
            gs_LcdDisp.ulNum1=0x16;
        }else
        {
            gs_LcdDisp.ulNum1=guc_SwErrCode[guc_ErrSwPt];
        }
        gs_LcdDisp.ulNum1|=0x00E00000;
        gs_LcdDisp.ulNum1Msk=0xAA033100;
        gul_ShowState=0;
        gs_LcdDisp.uiStr1=0;
        gs_LcdDisp.uiUnitPt1=0;
        break;
    default:
        break;
    }
    if(guc_ShowErr ==1)
    {
        gs_LcdDisp.ulNum1Msk = 0xAAA63353;
    }
    if(ucType == Const_NeedRefresh)
    {
        Disp_State(gul_ShowState);
#if (Disp_ErrState == 1)        
        gs_LcdDisp.ulNum1 = (((uint32)gui_DispErrState) << 20) | (gs_LcdDisp.ulNum1 & 0x000FFFFF);
        gs_LcdDisp.ulNum1Msk = 0;
#elif (Disp_ErrState == 2)        
        if(gul_ShowId == 0x00000000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_total;
          gs_LcdDisp.ulNum1Msk = 0;
        }else if(gul_ShowId == 0x00150000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_timeoutA;
          gs_LcdDisp.ulNum1Msk = 0;
        }else if(gul_ShowId == 0x00290000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_timeoutB;
          gs_LcdDisp.ulNum1Msk = 0;
        }else if(gul_ShowId == 0x003D0000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_timeoutC;
          gs_LcdDisp.ulNum1Msk = 0;
        }else if(gul_ShowId == 0x00170000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_crcerrA;
          gs_LcdDisp.ulNum1Msk = 0;
        }else if(gul_ShowId == 0x002B0000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_crcerrB;
          gs_LcdDisp.ulNum1Msk = 0;
        }else if(gul_ShowId == 0x003F0000)
        {
          gs_LcdDisp.ulNum1 = gul_dispbuf_crcerrC;
          gs_LcdDisp.ulNum1Msk = 0;
        }
#endif           
        
        //gs_LcdDisp.ulNum1=Hex2BCD(y);
        //gs_LcdDisp.ulNum1Msk=0;
        Disp_Numb(gs_LcdDisp.ulNum1, gs_LcdDisp.ulNum1Msk);
        Disp_String(gs_LcdDisp.uiStr1);
        Disp_UnitPoint(gs_LcdDisp.uiUnitPt1);
        
        //guc_LCDRAM[3] = 0x11;
        
        LCD_RAMUpdata();
    }
}
/*=========================================================================================\n
* @function_name: SleepDispInit
* @function_file: Lcdshow.c
* @描述: 睡眠前初始化显示
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SleepDispInit(void)
{
    gs_DispStt.uiIndex = 0;         //显示屏切换到第一屏
    guc_PowOffRuning=false;         //唤醒状态切换到睡眠
}
/*=========================================================================================\n
* @function_name: SleepDisp
* @function_file: Lcdshow.c
* @描述: 掉电显示
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
uint8 SleepDisp(void)
{

    if(gs_DispStt.ucTmr!=0)          //休眠显示
    {
        gs_DispStt.ucTmr--;
    }

    if(gs_DispStt.ucTmr>0)
    {
        //如果是时间那屏需要读取时间
        if(gul_ShowId == 0x04000102) 
        {
            SleepTimeProc();
            gs_DispStt.ucPwrOffRefresh|= Cosnt_RefreshTime;
            ShowRefresh();
        }
        return true;
    }

//  if(gs_DispStt.ulPwrOffTmr!=0)   //停显时间到
//  {
//      if(((gs_FunCfg.uc_PWOFFSHOWMDCfg&0x99)==0x09))  //停电常显
//      {
//          gs_DispStt.uiIndex = 0;                     //屏号复归到0
//          gs_DispStt.ucTmr = 1;                       //时间设置60s
//          gs_DispStt.ucMode = Const_DispMode_TX;
//          gs_DispStt.ucPwrOffRefresh |= Const_RefreshAll;
//          ShowRefresh();
//          return false;
//      }
//  }


    if(gs_DispStt.ucTmr==0)
    {
        if(gs_DispStt.ucMode == Const_DispMode_LX)
        {
            SleepTimeProc();
            gs_DispStt.ucTmr = gs_DispCont.ucLxTm;
            gs_DispStt.uiIndex++;
            gs_DispStt.ucPwrOffRefresh |= Const_RefreshAll;
            if(gs_DispStt.uiIndex >= gs_DispCont.ucLxCnt)
            {
                gs_DispStt.uiIndex = 0;                     //屏号复归到0
                return false;
            }
        }else 
        {
            return false;
        }
        ShowRefresh();
    }
    return true;
}

