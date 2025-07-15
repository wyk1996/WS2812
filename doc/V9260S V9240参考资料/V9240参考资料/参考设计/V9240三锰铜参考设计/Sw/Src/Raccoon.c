#define  RACCOONEXT
#include"Include.h"

/*=========================================================================================\n
* @function_name: Init_RacCtrl
* @function_file: ComRaccoon.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   (2013-04-15)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Init_RacCtrl(void)
{
    gs_RacCtrl.ucSendLen=0;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=0;
    gs_RacCtrl.ucRevPoint=0;
    gs_RacCtrl.ucStatus=Rac_Idle;
}
/*=========================================================================================\n
* @function_name: RxReset_V9281
* @function_file: Raccoon.c
* @描述: 低电平有效 P2.3输出低电平 持续40ms
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RxReset_V9260(void)
{
    PHACtrlEnable();
    PHBCtrlEnable();
    PHCCtrlEnable();
    
    P21FS=0; 
    P20FS=0; 
    P2OE&=(~BIT1);       //TXD 允许输出
    P2IE&=(~BIT1);       //TXD禁止输出
    
    P2OD |=(BIT1);

    P2OD&=(~BIT1);  
    guc_CommDelayTime = 10;
    while(guc_CommDelayTime >0);
    //DelayXms(200);
     P2OD |=(BIT1);

    PHACtrlDisable();
    PHBCtrlDisable();
    PHCCtrlDisable();
    
    P21FS=2;  
    P21FS=2; 
}
/*=========================================================================================\n
* @function_name: WriteRaccoon
* @function_file: Raccoon.h
* @描述: 向raccoon发送数据
* 
* 
* @参数: 
* @param:Data  写入数据
*               字节1：帧头0xFE    字节2：高4位地址高4bit，低4位控制
*               字节3：地址低8bit  字节4~7：数据   字节8：校验
* @param:addr  写入地址
* 控制
* @返回: 
* @return: uint8 
* @作者:   lwb (2013-07-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  hjj
* @修改内容: 添加Raccoon物理地址，三相表一共使用3个Raccoon
* @param:Phyadd Raccoon地址

===========================================================================================*/
uint8  WriteRaccoon(uint32 Data,uint16 Addr,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
    while(gs_RacPhy.uc_Uartstat != 0)
    {
        return false;
    }   
    
//     ExInt2IE|=(BIT2);             //开启发送使能
//     ExInt2IE&=(~BIT3);            //关闭接收使能
     
     
//    gs_RacCtrl.ucBuf[0] = 0xfe;    //V9240帧头 0x7D
    gs_RacCtrl.ucBuf[0] = 0x7D; 
//    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+RacWrite+ Phyadd;
    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+RacWrite; //V9240没有片选地址，默认0；zyj 11.4
    gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
    
    gs_RacCtrl.ucBuf[3] = (uint8)(Data&0x000000ff);
    gs_RacCtrl.ucBuf[4] = (uint8)((Data&0x0000ff00)>>8);
    gs_RacCtrl.ucBuf[5] = (uint8)((Data&0x00ff0000)>>16);
    gs_RacCtrl.ucBuf[6] = (uint8)((Data&0xff000000)>>24);
    //计算校验和
    gs_RacCtrl.ucBuf[7] =  gs_RacCtrl.ucBuf[0];
    for(i=1;i<7;i++)
    {
        gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
    gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;

    
    switch(Phyadd)          //GPIO控制V9240通信 开启 zyj 11.4
    {
      case APhy:
          PHACtrlEnable();
          break;
      case BPhy:
          PHBCtrlEnable();
          break;  
      case CPhy:
          PHCCtrlEnable();
          break;
    }
    
    gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态
    gs_RacCtrl.ucSendLen=8;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=4;
    gs_RacCtrl.ucRevPoint=0;
    gs_RacPhy.uc_Uartstat |= flgEtwrite;
    //V9260Start_Send();                  //开启发送
    Uart4_Transmit();
    
//     ExInt2IE&=(~BIT2);         //关闭发送使能  是否来得及？？？ zyj 11.4
//     ExInt2IE|=BIT3;            //开启接收使能
     
     
//    guc_CommDelayTime=7;                  //接收4个字节，按最低1200bps，需要4*11/1200=37ms
    guc_CommDelayTime = (uint8)((BAUDRate_1Byte_OverTime * 12 +10)/10);//发送8个字节,接收4个字节 单位:10ms 进一制
    while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
    {
   
        if(guc_CommDelayTime==0)
        {
            gs_RacPhy.uc_Uartstat &= ~flgEtwrite;
            return false;                   //如果超时 
          
        }
    }  
    gs_RacPhy.uc_Uartstat &= ~flgEtwrite;
    ucSum=0;
    for(i=0;i<3;i++)
    {
        ucSum += gs_RacCtrl.ucBuf[i]; 
    }

    ucSum=~ucSum;
    ucSum+=0x33;
    DelayXms(5);
    
    switch(Phyadd)          //GPIO控制V9240通信 关闭 zyj 11.4
    {
      case APhy:
          PHACtrlDisable();
          break;
      case BPhy:
          PHBCtrlDisable();
          break;  
      case CPhy:
          PHCCtrlDisable();
          break;
    }
    
    if(gs_RacCtrl.ucBuf[3]==ucSum)
    {
        return true;
    }else
    {
        return false;
    }
    
}
/*=========================================================================================\n
* @function_name: BroadcastWriteRaccoon
* @function_file: Raccoon.c
* @描述: 
* 
* 
* @参数: 
* @param:Data  
* @param:Addr  
 
* @返回: 
* @作者:   lwb (2013-09-25)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/ 
void BroadcastWriteRaccoon(uint32 Data,uint16 Addr)
{
    uint8 i;
    volatile uint8 ucSum;
    Clashin();
    while(gs_RacPhy.uc_Uartstat != 0)
    {
        return;
    }
    
//     ExInt2IE|=(BIT2);             //开启发送使能
//     ExInt2IE&=(~BIT3);            //关闭接收使能
     
     
//    gs_RacCtrl.ucBuf[0] = 0xfe;
    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+RacBroadcast;
    gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
    
    gs_RacCtrl.ucBuf[3] = (uint8)(Data&0x000000ff);
    gs_RacCtrl.ucBuf[4] = (uint8)((Data&0x0000ff00)>>8);
    gs_RacCtrl.ucBuf[5] = (uint8)((Data&0x00ff0000)>>16);
    gs_RacCtrl.ucBuf[6] = (uint8)((Data&0xff000000)>>24);
    //计算校验和
    gs_RacCtrl.ucBuf[7] =  gs_RacCtrl.ucBuf[0];
    for(i=1;i<7;i++)
    {
        gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
    gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;

    PHACtrlEnable();                //3路发送均开启 zyj 11.4
    PHBCtrlEnable();
    PHCCtrlEnable();

    gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态
    gs_RacCtrl.ucSendLen=8;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=0;
    gs_RacCtrl.ucRevPoint=0;
    gs_RacPhy.uc_Uartstat |= flgEtBwrite;
    //V9260Start_Send();                  //开启发送
    Uart4_Transmit();
    
//    ExInt2IE&=(~BIT2);         //关闭发送使能  是否来得及？？？ zyj 11.4
//    ExInt2IE|=BIT3;            //开启接收使能
     
//    guc_CommDelayTime=5;
    guc_CommDelayTime = (uint8)((BAUDRate_1Byte_OverTime * 8 +10)/10);//发送8个字节,接收0个字节 单位:10ms 进一制
    while(gs_RacCtrl.ucStatus!=Rac_Rev)
    {
        if(guc_CommDelayTime==0)
        {
             gs_RacPhy.uc_Uartstat &= ~flgEtBwrite;
             return;
           
        }
    }
//    guc_CommDelayTime=4;                //等待最后一个字节发送完
//    while(guc_CommDelayTime>0)
//    {
//    }   
    gs_RacPhy.uc_Uartstat &= ~flgEtBwrite;
    Clashout();
    
    PHACtrlDisable();
    PHBCtrlDisable();
    PHCCtrlDisable();
}
/*=========================================================================================\n
* @function_name: ReadRaccoon
* @function_file: Raccoon.h
* @描述: 向raccoon读取数据
* 字节0：帧头0xFE    字节1：高4位地址高4bit，低4位控制
* 字节2：地址低8bit  字节3：word个数 字节4~6：无意义 字节7：校验
* @参数: 
* @param:Addr  读取起始地址
* @param:num  读取word个数，32bit
* 
* @返回: 
*        
* @return: uint8 
* @作者:   lwb (2013-07-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  hjj
* @修改内容: 添加Raccoon物理地址，三相表一共使用3个Raccoon
* @param:Phyadd Raccoon地址

==========================================================================================*/
uint8 ReadRaccoon(uint16 Addr,uint8 num,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
    Clashin();
    while(gs_RacPhy.uc_Uartstat != 0) 
    {
        return false;
    }
    
//     ExInt2IE|=(BIT2);             //开启发送使能
//     ExInt2IE&=(~BIT3);            //关闭接收使能
    
    gs_RacCtrl.ucBuf[0] = 0x7d;
    
//    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ Phyadd;
    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead;  //V9240没有片选地址，默认0；zyj 11.4
    gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
    
    gs_RacCtrl.ucBuf[3] = num;                   //num：需要读取多少字的内容
    gs_RacCtrl.ucBuf[4] = 0;
    gs_RacCtrl.ucBuf[5] = 0;
    gs_RacCtrl.ucBuf[6] = 0;
    gs_RacCtrl.ucBuf[7] = 0;                    //sum 清零
    for(i=0;i<7;i++)
    {
        gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
    gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
    
    switch(Phyadd)          //GPIO控制V9240通信 开启 zyj 11.4
    {
      case APhy:
          PHACtrlEnable();
          break;
      case BPhy:
          PHBCtrlEnable();
          break;  
      case CPhy:
          PHCCtrlEnable();
          break;
    }
    
    gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态
    
    gs_RacCtrl.ucSendLen=8;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=(num*4)+4;           
    gs_RacCtrl.ucRevPoint=0;
//    guc_CommDelayTime=17; 
    guc_CommDelayTime = (uint8)((BAUDRate_1Byte_OverTime * (4*num+12) +10)/10);//发送8个字节,接收4*N+4个字节 单位:10ms 进一制
    gs_RacPhy.uc_Uartstat |= flgEtread;
   //V9260Start_Send();                  //开启发送
    Uart4_Transmit();
    
//    ExInt2IE&=(~BIT2);         //关闭发送使能  是否来得及？？？ zyj 11.4
//    ExInt2IE|=BIT3;            //开启接收使能
    
    while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
    {
        if(guc_CommDelayTime==0)
        {
            gs_RacPhy.uc_Uartstat &= ~flgEtread;
            return false;           //如果超时 
            
        }
    }
   gs_RacPhy.uc_Uartstat &= ~flgEtread;
    ucSum=0;
    for(i=0;i<(num*4)+3;i++)               //读取不超过255个字节
    {
        ucSum += gs_RacCtrl.ucBuf[i]; 
    }
    
    ucSum=~ucSum;
    ucSum+=0x33;
    Clashout();
    
    switch(Phyadd)          //GPIO控制V9240通信 关闭 zyj 11.4
    {
      case APhy:
          PHACtrlDisable();
          break;
      case BPhy:
          PHBCtrlDisable();
          break;  
      case CPhy:
          PHCCtrlDisable();
          break;
    }
    
    if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
    {
        return true;
    }else
    {
        return false;
    }         
}

void ReadRaccoonInt(uint16 Addr,uint8 num,uint8 Phyadd)
{
     uint8 i;
    while(gs_RacPhy.uc_Uartstat != 0)
    {
        return;
    }
    
//    ExInt2IE|=(BIT2);             //开启发送使能
//    ExInt2IE&=(~BIT3);            //关闭接收使能
     
//    gs_RacCtrl.ucBuf[0] = 0xfe;
    gs_RacCtrl.ucBuf[0] = 0x7d;
//    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ Phyadd;
    gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead;  //V9240没有片选地址，默认0；zyj 11.4
    gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
    
    gs_RacCtrl.ucBuf[3] = num;                   //num：需要读取多少字的内容
    gs_RacCtrl.ucBuf[4] = 0;
    gs_RacCtrl.ucBuf[5] = 0;
    gs_RacCtrl.ucBuf[6] = 0;
    gs_RacCtrl.ucBuf[7] = 0;                    //sum 清零
    for(i=0;i<7;i++)
    {
        gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
    gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
    
    switch(Phyadd)          //GPIO控制V9240通信 开启 zyj 11.4
    {
      case APhy:
          PHACtrlEnable();
          break;
      case BPhy:
          PHBCtrlEnable();
          break;  
      case CPhy:
          PHCCtrlEnable();
          break;
    }
    
    gs_RacCtrl.ucStatus=Rac_Send;       //进入发送状态
    gs_RacCtrl.ucSendLen=8;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=(num*4)+4;           
    gs_RacCtrl.ucRevPoint=0;
//    gs_RacPhy.uc_Physendtim= 0x50;//Const_DyUart2Over; //端口超时保护
    gs_RacPhy.uc_Physendtim= (uint8)((BAUDRate_1Byte_OverTime * (4*num+12) +10)/10);//发送8个字节,接收4*N+4个字节 单位:10ms 进一制
    gs_RacPhy.uc_Phystat = Phyadd;
    gs_RacPhy.uc_Sendstat = true;
    gs_RacPhy.uc_Uartstat |= flgEtreadint;
    //V9260Start_Send();                  //开启发送 
    Uart4_Transmit();
#if (Disp_ErrState == 2)    
    gul_dispbuf_total++;
#endif    
//    switch(Phyadd)          //GPIO控制V9240通信 关闭 zyj 11.4
//    {
//      case APhy:
//          PHACtrlDisable();
//          break;
//      case BPhy:
//          PHBCtrlDisable();
//          break;  
//      case CPhy:
//          PHCCtrlDisable();
//          break;
//    }
}

const uint8 flgphy[]={flgEtPhy_B,flgEtPhy_C,0,0};
const uint8 phy[] = {0,2,1};
/*=========================================================================================\n
* @function_name: ReadbackABCPhy
* @function_file: Raccoon.c
* @描述: 
* 
* 
* @参数: 
* @param:Phyadd  
* @返回: 
* @作者:   lwb (2013-09-25)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ReadbackABCPhy(uint8 Phyadd)
{
    uint8 i;    
    volatile uint8 ucSum;
    Word32 TempValue,TempValue1,TempValue2,TempValue3,TempValue4,TempValue5;
#if (SysMode_TEST != 1)    
    static uint8 offtim = 0;
#endif    
    ucSum=0;
    gs_RacPhy.uc_Physendtim = 0;  //读ABC三相超时关闭
    for(i=0;i<(5*4)+3;i++)               //读取不超过255个字节
    {
        ucSum += gs_RacCtrl.ucBuf[i]; 
    }

    ucSum=~ucSum;
    ucSum+=0x33;

    if(gs_RacCtrl.ucBuf[5*4+3]==ucSum)
    {
        if((gs_RacCtrl.ucBuf[1]&0x03) != RacRead)
        {
            return;
        }
        else
        {
            if((gs_RacCtrl.ucBuf[1]&0x0c) != 0)//Phyadd) 
            {
                return;
            }
            else
            {
                memcpy((uint8*)TempValue.byte,gs_RacCtrl.ucBuf+3,4); //有功秒平均功率
                if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
                {
                    TempValue.lword=~TempValue.lword+1;
                }
                memcpy((uint8*)TempValue1.byte,gs_RacCtrl.ucBuf+7,4);//无功
                 if(TempValue1.byte[3]>0x7f)          //如果是负数的数，先变成正数
                {
                    TempValue1.lword=~TempValue1.lword+1;
                }
                memcpy((uint8*)TempValue2.byte,gs_RacCtrl.ucBuf+11,4);//频率
                memcpy((uint8*)TempValue3.byte,gs_RacCtrl.ucBuf+15,4);//电流
                memcpy((uint8*)TempValue4.byte,gs_RacCtrl.ucBuf+19,4);//电压
#if (SysMode_TEST != 1)                
//                if(offtim >0)
//                {
//                    offtim--;
//                }
                offtim = 0;
                switch(Phyadd)
                {
                  case APhy:
                    offtim1 = 0;
                    break;
                  case BPhy:
                    offtim2 = 0;
                    break;
                  case CPhy:
                    offtim3 = 0;
                    break;
                  default:
                    break;
                }
#endif                
            }
        }
    }
    else
    {    
#if (SysMode_TEST != 1)      
        offtim++;
#endif        
        TempValue.lword = 0;
        TempValue1.lword = 0;
        TempValue2.lword = 1;
        TempValue3.lword = 0;
        TempValue4.lword = 0;

#if (Disp_ErrState == 1)        
        switch(Phyadd)
        {
          case APhy:
            gui_DispErrState &= 0xf0ff; 
            gui_DispErrState |= 0x0100;
            break;
          case BPhy:
            gui_DispErrState &= 0xff0f; 
            gui_DispErrState |= 0x0010;
            break;
          case CPhy:
            gui_DispErrState &= 0xfff0; 
            gui_DispErrState |= 0x0001;
            break;
          default:
            break;
        }
#elif (Disp_ErrState == 2)        
        switch(Phyadd)
        {
          case APhy:
            gul_dispbuf_crcerrA++;
            break;
          case BPhy:
            gul_dispbuf_crcerrB++;
            break;
          case CPhy:
            gul_dispbuf_crcerrC++;
            break;
          default:
            break;
        }
#endif        
    }
  /*  if(Phyadd ==CPhy)
    {
        gs_RacPhy.uc_Phystat = 0x0c;
    }
    gui_ReadABCPhy |= flgphy[Phyadd >>2];
    gs_RacPhy.uc_Sendstat = false;
    gs_RacPhy.uc_Uartstat &= ~flgEtreadint;*/
    
     switch(Phyadd)
     {
         case APhy:
#if (SysMode_TEST != 1)           
              if(offtim > 0 && offtim < 3)
            {
                gs_RacPhy.uc_Powstat &= ~power_a;  //有电标志
                gs_power.ul_power[0] =  gs_power.ul_power[0] ; 
                gs_power.ul_Npower[0] = gs_power.ul_Npower[0]; 
                gs_UI.ul_U[0] = gs_UI.ul_U[0];
                gs_UI.ul_I[0] =  gs_UI.ul_I[0];
                gs_UI.ul_Hz[0] = gs_UI.ul_Hz[0];
            }
            else
#endif              
            {
                gs_RacPhy.uc_Powstat &= ~power_a;  //有电标志
                gs_power.ul_power[0] = TempValue.lword; 
                gs_power.ul_Npower[0] = TempValue1.lword;
                
                if(ReadRaccoon(T8BAUD,1,APhy))  // k x N/ASFREQ
                {
                  memcpy((uint8*)TempValue5.byte,gs_RacCtrl.ucBuf+3,4);
                }
                if( guc_CheckABCPhase_Dly == 0)
                {
                  if( gul_FRQAVG_Value[0] == 0)   //取平均值
                  {
                    gul_FRQAVG_Value[0] = TempValue2.lword;
                  }else
                  {
                    gul_FRQAVG_Value[0] = (gul_FRQAVG_Value[0] + TempValue2.lword)/2;
                  }
                }
                gs_UI.ul_Hz[0] = (uint32)(K_BAUDRate * 100 *((float)(TempValue5.lword) / (float)TempValue2.lword));
//                gs_UI.ul_Hz[0] =(uint32)HZCONST/TempValue2.lword;
                gs_UI.ul_I[0] =  TempValue3.lword;
                gs_UI.ul_U[0] = TempValue4.lword;
            }            
            if(((float)gs_power.ul_power[0]/gs_JbPm.ul_PG) < (((float)((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100)))/1000*1.5))  //最低1.1W(0.1%) *1.5
            {
                gs_power_enerycal.ul_power[0]  = 0;
                
            }else
            {
                gs_power_enerycal.ul_power[0] = gs_power.ul_power[0];
            }
            if(((float)gs_power.ul_Npower[0]/gs_JbPm.ul_PG) < (((float)((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100)))/1000*1.5))
            {
                gs_power_enerycal.ul_Npower[0]  = 0;
            }else
            {
                gs_power_enerycal.ul_Npower[0] = gs_power.ul_Npower[0];
            }      
            break;
        case BPhy:
#if (SysMode_TEST != 1)          
            if(offtim > 0 && offtim < 3)
            {
                gs_RacPhy.uc_Powstat &= ~power_b;  //有电标志
                gs_power.ul_power[1] =  gs_power.ul_power[1] ; 
                gs_power.ul_Npower[1] = gs_power.ul_Npower[1]; 
                gs_UI.ul_U[1] = gs_UI.ul_U[1];
                gs_UI.ul_I[1] =  gs_UI.ul_I[1];
                gs_UI.ul_Hz[1] = gs_UI.ul_Hz[1];
            }
            else
#endif              
            {
                gs_RacPhy.uc_Powstat &= ~power_b;  //有电标志 
                gs_power.ul_power[1] = TempValue.lword; 
                gs_power.ul_Npower[1] = TempValue1.lword;
                
                if(ReadRaccoon(T8BAUD,1,BPhy))  // k x N/ASFREQ
                {
                  memcpy((uint8*)TempValue5.byte,gs_RacCtrl.ucBuf+3,4);
                }
                if( guc_CheckABCPhase_Dly == 0)
                {
                  if( gul_FRQAVG_Value[1] == 0)
                  {
                    gul_FRQAVG_Value[1] = TempValue2.lword;
                  }else
                  {
                    gul_FRQAVG_Value[1] = (gul_FRQAVG_Value[1] + TempValue2.lword)/2;
                  }
                }
                gs_UI.ul_Hz[1] = (uint32)(K_BAUDRate * 100 *((float)(TempValue5.lword) / (float)TempValue2.lword));
//                gs_UI.ul_Hz[1] =TempValue2.lword;//(uint32)HZCONST/TempValue2.lword;
                gs_UI.ul_I[1] =  TempValue3.lword;
                gs_UI.ul_U[1] = TempValue4.lword;
            }
            if(((float)gs_power.ul_power[1]/gs_JbPm.ul_PG) < (((float)((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100)))/1000*1.5))  //最低1.1W(0.1%) *1.5            
            {
                gs_power_enerycal.ul_power[1]  = 0;
            }else
            {
                gs_power_enerycal.ul_power[1] = gs_power.ul_power[1];
            }            
            if(((float)gs_power.ul_Npower[1]/gs_JbPm.ul_PG) < (((float)((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100)))/1000*1.5))
            {
                gs_power_enerycal.ul_Npower[1]  = 0;
            }else
            {
                gs_power_enerycal.ul_Npower[1] = gs_power.ul_Npower[1];
            }            
            break;
         case CPhy:
#if (SysMode_TEST != 1)           
            if(offtim > 0 && offtim < 3)
            {
                gs_RacPhy.uc_Powstat &= ~power_c;  //有电标志
                gs_power.ul_power[2] =  gs_power.ul_power[2] ; 
                gs_power.ul_Npower[2] = gs_power.ul_Npower[2]; 
                gs_UI.ul_U[2] = gs_UI.ul_U[2];
                gs_UI.ul_I[2] =  gs_UI.ul_I[2];
                gs_UI.ul_Hz[2] = gs_UI.ul_Hz[2];
            }
            else
#endif              
            {
                gs_RacPhy.uc_Powstat &= ~power_c;  //有电标志
                gs_power.ul_power[2] = TempValue.lword; 
                gs_power.ul_Npower[2] = TempValue1.lword; 
                
                if(ReadRaccoon(T8BAUD,1,CPhy))  // k x N/ASFREQ
                {
                  memcpy((uint8*)TempValue5.byte,gs_RacCtrl.ucBuf+3,4);
                }
                if( guc_CheckABCPhase_Dly == 0)
                {
                  if( gul_FRQAVG_Value[2] == 0)
                  {
                    gul_FRQAVG_Value[2] = TempValue2.lword;
                  }else
                  {
                    gul_FRQAVG_Value[2] = (gul_FRQAVG_Value[2] + TempValue2.lword)/2;
                  }
                }
                gs_UI.ul_Hz[2] = (uint32)(K_BAUDRate * 100 *((float)(TempValue5.lword) / (float)TempValue2.lword));
//                gs_UI.ul_Hz[2] =TempValue2.lword;//(uint32)HZCONST/TempValue2.lword;
                gs_UI.ul_I[2] =  TempValue3.lword;
                gs_UI.ul_U[2] = TempValue4.lword;
            }            
//            if((gs_power.ul_power[2]/gs_JbPm.ul_PG) < ((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))/1000 )
            if(((float)gs_power.ul_power[2]/gs_JbPm.ul_PG) < (((float)((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100)))/1000*1.5))  //最低1.1W(0.1%) *1.5                        
            {
                gs_power_enerycal.ul_power[2]  = 0;
            }else
            {
                gs_power_enerycal.ul_power[2] = gs_power.ul_power[2];
            }
//            if((gs_power.ul_Npower[2]/gs_JbPm.ul_PG) < ((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))/1000 )
            if(((float)gs_power.ul_Npower[2]/gs_JbPm.ul_PG) < (((float)((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100)))/1000*1.5))            
            {
                gs_power_enerycal.ul_Npower[2]  = 0;
            }else
            {
                gs_power_enerycal.ul_Npower[2] = gs_power.ul_Npower[2];
            }            
            break;
        default:
            break;
    }
    if(Phyadd ==CPhy)
    {
       gs_RacPhy.uc_Phystat = 0x0c;
       guc_ReadingABC_Flg = 0;  //读有效值结束标志
//       guc_StartReadABC_Flg = 0;
    }
    gui_ReadABCPhy |= flgphy[Phyadd >>2];
    gui_SystemEvent |= gui_ReadABCPhy<<8;
    gs_RacPhy.uc_Sendstat = false;
    gs_RacPhy.uc_Uartstat &= ~flgEtreadint;
}

/*=========================================================================================\n
* @function_name: CheckRegSum
* @function_file: Raccoon.c
* @描述: 检测校验和
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CheckRegSum(void)
{
    uint32 ucSum=0;
    ucSum = 
          + SysCtrl_ValueA  //RacSysCtrl
          + gs_BPFPARA_Value.BPFPARA_Value[0]  //gs_JbPm.RacBPFPARA
          + gs_JbPm.gs_JBA.RacIAC
          + gs_JbPm.gs_JBA.RacUC 
          + gs_JbPm.gs_JBA.RacPAC
          + gs_JbPm.gs_JBA.RacPAC //无功比差
          + gs_JbPm.gs_JBA.RacPADCC
          + gs_JbPm.gs_JBA.RacQADCC
          + gs_JbPm.gs_JBA.RacPHC
          + AnaCtrl1_Val;
    
    ucSum = 0xFFFFFFFF-ucSum;
    WriteRaccoon(ucSum,CKSUM,APhy);

     ucSum=0;
    ucSum = 
          + SysCtrl_ValueB  //RacSysCtrl
          + gs_BPFPARA_Value.BPFPARA_Value[1]  //gs_JbPm.RacBPFPARA
          + gs_JbPm.gs_JBB.RacIAC 
          + gs_JbPm.gs_JBB.RacUC 
          + gs_JbPm.gs_JBB.RacPAC
          + gs_JbPm.gs_JBB.RacPAC //无功比差
          + gs_JbPm.gs_JBB.RacPADCC
          + gs_JbPm.gs_JBB.RacQADCC
          + gs_JbPm.gs_JBB.RacPHC
          + AnaCtrl1_Val;
  
    ucSum = 0xFFFFFFFF-ucSum;
    WriteRaccoon(ucSum,CKSUM,BPhy);

     ucSum=0;
    ucSum = 
          + SysCtrl_ValueC  //RacSysCtrl
          + gs_BPFPARA_Value.BPFPARA_Value[2]  //gs_JbPm.RacBPFPARA
          + gs_JbPm.gs_JBC.RacIAC 
          + gs_JbPm.gs_JBC.RacUC 
          + gs_JbPm.gs_JBC.RacPAC
          + gs_JbPm.gs_JBC.RacPAC //无功比差
          + gs_JbPm.gs_JBC.RacPADCC
          + gs_JbPm.gs_JBC.RacQADCC
          + gs_JbPm.gs_JBC.RacPHC
          + AnaCtrl1_Val;
   
    ucSum = 0xFFFFFFFF-ucSum;
    WriteRaccoon(ucSum,CKSUM,CPhy);

    
}

void CheckRegSumABC(uint8 phyadd)
{
    uint32 ucSum;
    switch(phyadd)   //角差 
    {
    case APhy:
      ucSum=0;
      ucSum = 
            + SysCtrl_ValueA  //RacSysCtrl
            + gs_BPFPARA_Value.BPFPARA_Value[0]  //gs_JbPm.RacBPFPARA
            + gs_JbPm.gs_JBA.RacIAC
            + gs_JbPm.gs_JBA.RacUC 
            + gs_JbPm.gs_JBA.RacPAC
            + gs_JbPm.gs_JBA.RacPAC //无功比差
            + gs_JbPm.gs_JBA.RacPADCC
            + gs_JbPm.gs_JBA.RacQADCC
            + gs_JbPm.gs_JBA.RacPHC
            + AnaCtrl1_Val;
    
      ucSum = 0xFFFFFFFF-ucSum;
      WriteRaccoon(ucSum,CKSUM,APhy);
      break;
    case BPhy:
      ucSum=0;
      ucSum = 
            + SysCtrl_ValueB  //RacSysCtrl
            + gs_BPFPARA_Value.BPFPARA_Value[1]  //gs_JbPm.RacBPFPARA
            + gs_JbPm.gs_JBB.RacIAC 
            + gs_JbPm.gs_JBB.RacUC 
            + gs_JbPm.gs_JBB.RacPAC
            + gs_JbPm.gs_JBB.RacPAC //无功比差
            + gs_JbPm.gs_JBB.RacPADCC
            + gs_JbPm.gs_JBB.RacQADCC
            + gs_JbPm.gs_JBB.RacPHC
            + AnaCtrl1_Val;
    
      ucSum = 0xFFFFFFFF-ucSum;
      WriteRaccoon(ucSum,CKSUM,BPhy);
      break;  
    case CPhy:
      ucSum=0;
      ucSum = 
            + SysCtrl_ValueC  //RacSysCtrl
            + gs_BPFPARA_Value.BPFPARA_Value[2]  //gs_JbPm.RacBPFPARA
            + gs_JbPm.gs_JBC.RacIAC 
            + gs_JbPm.gs_JBC.RacUC 
            + gs_JbPm.gs_JBC.RacPAC
            + gs_JbPm.gs_JBC.RacPAC //无功比差
            + gs_JbPm.gs_JBC.RacPADCC
            + gs_JbPm.gs_JBC.RacQADCC
            + gs_JbPm.gs_JBC.RacPHC
            + AnaCtrl1_Val;
     
      ucSum = 0xFFFFFFFF-ucSum;
      WriteRaccoon(ucSum,CKSUM,CPhy);
      break;
    }   
} 
/*=========================================================================================\n
* @function_name: CheckSysErr
* @function_file: Raccoon.c
* @描述: RACCOON的系统自检校验  CHSUM
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//void CheckSysErr(void)
//{
//   // uint32 ulTemp,ulTemp1;
//    //ulTemp = 0x01FF0008&0x000001ff;
//    //ulTemp = ~ulTemp;
//    //ulTemp &= 0x000001ff;
//    //ulTemp1 |= (ulTemp<<16);
//    uint8 i;
//    for(i = 0; i <3; i++)
//    {
//        WriteRaccoon( 0x01F70008,RegSysCtrl,phadd[i]);
//    }
//    //WriteRaccoon(0x01F70008,RegSysCtrl,APhy);
//}
/*=========================================================================================\n
* @function_name: V9281_InitDefaultPara
* @function_file: Raccoon.c
* @描述: 初始化默认参数
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//void V9260_InitDefaultPara(void)
//{
//    gs_JbPm.ui_MeterC=1200;             // 表常数
//    gs_JbPm.ui_Un=22000;                // 标称电压
//    gs_JbPm.ui_Ib=5000;                 // 标称电流
//    gs_JbPm.ui_Resve1=0;
//    gs_JbPm.RacEGYTH   = 0x067215D8;    //0x0181 能量累加门限值 6400 5A 0x013A1A40
//    gs_JbPm.RacREEGYTH = 0x067215D8;    //0x0181 能量累加门限值 6400 5A 0x013A1A40
//    //gs_JbPm.RacCTH     = 0x00000000;    //0x0182 起动，潜动门限值
//    //gs_JbPm.RacRECTH   = 0x00000000;    //0x0182 起动，潜动门限值
////    gs_JbPm.RacANCtrl0 = 0xDf0004C0;    //0x185 增益I:32，U:1，M:4 温度系数，全局电流偏置
////    gs_JbPm.RacANCtrl1 = 0x30000000;    //0x186 
////    gs_JbPm.RacANCtrl2 = 0x0000000F;    //0x187 
//    gs_JbPm.RacSysCtrl = 0x00000000;  //0x180 不参与校验 时钟3.2MHz
//    gs_JbPm.RacBPFPARA = 0x811D2BA7;    //0x0125 带通滤波器系数 0x811D2BA7
//
////    gs_JbPm.RacMTPARA0 = 0xE0000002;    //0x183 CF工作时钟3.2MHz 使能CF输出，能量累加，起动潜动，能量来源：平均全波有功   
//    
//    gs_JbPm.gs_JBA.RacPHC = 0x00000000;      //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差   
//    gs_JbPm.gs_JBA.RacUC = 0x00000000;        //全波电压有效值比差   
//    gs_JbPm.gs_JBA.RacIAC = 0x00000000;        //全波电流有效值比差
//    gs_JbPm.gs_JBA.RacPAC = 0x00000000;         //全波有功功率比差
//    gs_JbPm.gs_JBA.RacPADCC = 0x00000000;        //全波有功功率二次补偿值
//    gs_JbPm.gs_JBA.RacQADCC = 0x00000000;      //全波无功功率二次补偿值
//
//    gs_JbPm.gs_JBB.RacPHC = 0x00000000;      //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差   
//    gs_JbPm.gs_JBB.RacUC = 0x00000000;        //全波电压有效值比差   
//    gs_JbPm.gs_JBB.RacIAC = 0x00000000;        //全波电流有效值比差
//    gs_JbPm.gs_JBB.RacPAC = 0x00000000;         //全波有功功率比差
//    gs_JbPm.gs_JBB.RacPADCC = 0x00000000;        //全波有功功率二次补偿值
//    gs_JbPm.gs_JBB.RacQADCC = 0x00000000;       //全波无功功率二次补偿值
//    
//    gs_JbPm.gs_JBC.RacPHC = 0x00000000;      //0x184 M通道关闭，仅进行配置自检，使能去直高通滤波器，使能（U/I）输入，角差   
//    gs_JbPm.gs_JBC.RacUC = 0x00000000;        //全波电压有效值比差   
//    gs_JbPm.gs_JBC.RacIAC = 0x00000000;        //全波电流有效值比差
//    gs_JbPm.gs_JBC.RacPAC = 0x00000000;         //全波有功功率比差
//    gs_JbPm.gs_JBC.RacPADCC = 0x00000000;        //全波有功功率二次补偿值
//    gs_JbPm.gs_JBC.RacQADCC = 0x00000000;       //全波无功功率二次补偿值
//
//    gs_JbPm.ui_Resve2=0;
//    gs_JbPm.ul_PG=1;               //功率比例系数
//    gs_JbPm.ul_URmG=1;             //电压通道比例系数
//    gs_JbPm.ul_I1RmG=1;            //电流通道1比例系数
//    
//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
//   SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM)); //写入E2中
//
//
//}

/*=========================================================================================\n
* @function_name: RecoverBPFPARA_Value
* @function_file: Raccoon.c
* @描述: 上电恢复带通滤波器系数值
* 
* @参数: 
* 
* @返回: 
* @return: void 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RecoverBPFPARA_Value(void)
{
    BE_ReadP(EEP_BPFPARA_VALUE,(uint8 *)&gs_BPFPARA_Value,sizeof(S_BPFPARA_Value));
    
    if(gs_BPFPARA_Value.ui_CRC != do_CRC((uint8 *)&gs_BPFPARA_Value,sizeof(S_BPFPARA_Value)-2))  //RAM中的数据是否完整
    {                                          
        gs_BPFPARA_Value.BPFPARA_Value[0] = BPFPARA_ValueDef; //默认参数
        gs_BPFPARA_Value.BPFPARA_Value[1] = BPFPARA_ValueDef;
        gs_BPFPARA_Value.BPFPARA_Value[2] = BPFPARA_ValueDef;
        gs_BPFPARA_Value.ui_CRC=do_CRC((uint8*)&gs_BPFPARA_Value,sizeof(S_BPFPARA_Value)-2);     // CRC
    }
}

/*=========================================================================================\n
* @function_name: Cal_BPFPARA_Value
* @function_file: Raccoon.c
* @描述: 计算带通滤波器系数值
* 
* @参数: 
* 
* @返回: 
* @return: void 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Cal_BPFPARA_Value(void)
{
    static uint8 cnt = 0;
    
    cnt++;
    if( guc_CheckABCPhase_Dly > 0)   //延时  计算相位
    {
      guc_CheckABCPhase_Dly--;
    }
    if(( cnt >= guc_CalBPFValue_DelayTm) && (guc_ReadingABC_Flg == 0))
    {
        guc_CheckABCPhase_Dly = 5;      //延时5S  计算相位
        guc_CalBPFValue_DelayTm = 15;   //间隔15s 计算带通滤波器 
        cnt = 0;
        
        if((!(gs_RacPhy.uc_Powstat&power_a)) && ( gul_FRQAVG_Value[0] != 0))  //有电，且数据正常
        {
          gs_BPFPARA_Value.BPFPARA_Value[0] = (int32)((-2143289344) * cos(100.5309632/gul_FRQAVG_Value[0]));
        }
//        else
//        {
//          gs_BPFPARA_Value.BPFPARA_Value[0] = BPFPARA_ValueDef; //默认参数
//        }
        
        if((!(gs_RacPhy.uc_Powstat&power_b)) && ( gul_FRQAVG_Value[1] != 0))
        {
          gs_BPFPARA_Value.BPFPARA_Value[1] = (int32)((-2143289344) * cos(100.5309632/gul_FRQAVG_Value[1]));
        }
//        else
//        {
//          gs_BPFPARA_Value.BPFPARA_Value[1] = BPFPARA_ValueDef;
//        }
        
        if((!(gs_RacPhy.uc_Powstat&power_c)) && ( gul_FRQAVG_Value[2] != 0))
        {
          gs_BPFPARA_Value.BPFPARA_Value[2] = (int32)((-2143289344) * cos(100.5309632/gul_FRQAVG_Value[2]));
        }
//        else
//        {
//          gs_BPFPARA_Value.BPFPARA_Value[2] = BPFPARA_ValueDef;
//        }

        gs_BPFPARA_Value.ui_CRC=do_CRC((uint8*)&gs_BPFPARA_Value,sizeof(S_BPFPARA_Value)-2);
//        gui_RefreshEvent |= flgEtPara_EnyBottom;
        if(!(gs_RacPhy.uc_Powstat&power_a))  //有电
        {
          WriteRaccoon(gs_BPFPARA_Value.BPFPARA_Value[0],BPFPARA, APhy);
          CheckRegSumABC(APhy);
        }
        if(!(gs_RacPhy.uc_Powstat&power_b))  //有电
        {
          WriteRaccoon(gs_BPFPARA_Value.BPFPARA_Value[1],BPFPARA, BPhy);
          CheckRegSumABC(BPhy);
        }
        if(!(gs_RacPhy.uc_Powstat&power_c))  //有电
        {
          WriteRaccoon(gs_BPFPARA_Value.BPFPARA_Value[2],BPFPARA, CPhy);
          CheckRegSumABC(CPhy);
        }
    }
}
/*=========================================================================================\n
* @function_name: UpdateRaccoonPara
* @function_file: Raccoon.c
* @描述: 
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
extern const uint8 phadd[] = {0x00,0x04,0x08};           //zyj 1118 从机地址改动
const uint32 SysCtrl_Value[] = {SysCtrl_ValueA,SysCtrl_ValueB,SysCtrl_ValueC};

uint8 UpdateRaccoonPara(void)
{
    uint8 i,j,tmp;
//    uint32 ucSum=0;
    uint32 *ul_pRacReg;
    for(j=0;j<3;j++)
    {
        ul_pRacReg=(uint32*)&gs_JbPm.RacSysCtrl;
        for(i=0;i<sizeof(RegAddr)/sizeof(uint16) ;i++)
        {
           CLRWDT(); 

            if(i < 2)  //SysCtrl,BPFPARA
            {
              if( i == 0) //SysCtrl
              {
                tmp = WriteRaccoon(SysCtrl_Value[j],RegAddr[i], phadd[j]);//U: x1  IA: x32  IB: x32  +50 PPM
                if(tmp ==false)
                {
                    WriteRaccoon(SysCtrl_Value[j],RegAddr[i], phadd[j]);  
                }
//                ucSum += SysCtrl_Value;
                CLRWDT();
              }else     //BPFPARA
              {
//                tmp = WriteRaccoon(ul_pRacReg[i],RegAddr[i], phadd[j]);
                tmp = WriteRaccoon(gs_BPFPARA_Value.BPFPARA_Value[j],RegAddr[i], phadd[j]);
                if(tmp ==false)
                {
//                    WriteRaccoon(ul_pRacReg[i],RegAddr[i], phadd[j]); 
                    WriteRaccoon(gs_BPFPARA_Value.BPFPARA_Value[j],RegAddr[i], phadd[j]);
                }
//                ucSum += ul_pRacReg[i];
                CLRWDT();  
              }
            }
            else
            {
                if(i == 2)
                {
                    switch(phadd[j])   //RacUC,RacIAC,RacPAC,RacPADCC,RacQADCC
                    {
                        case APhy:
                            ul_pRacReg = (uint32*)&gs_JbPm.gs_JBA.RacUC;
                            break;
                        case BPhy:
                            ul_pRacReg = (uint32*)&gs_JbPm.gs_JBB.RacUC;
                            break;  
                        case CPhy:
                            ul_pRacReg = (uint32*)&gs_JbPm.gs_JBC.RacUC;
                            break;
                    }
                    
                }
                if(i == 4) //RacPAC
                {
                    tmp = WriteRaccoon(ul_pRacReg[i-2],RegAddr[i], phadd[j]);
                    if(tmp == false)
                    {
                        WriteRaccoon(ul_pRacReg[i-2],RegAddr[i], phadd[j]);
                    }
//                    ucSum += ul_pRacReg[i-2];
                    CLRWDT(); 
                    WriteRaccoon(ul_pRacReg[i-2], QAC, phadd[j]);  //A路无功比差 = 有功比差
//                    ucSum += ul_pRacReg[i-2];
                }
                else
                {
                    tmp = WriteRaccoon(ul_pRacReg[i-2],RegAddr[i], phadd[j]);
                    if(tmp ==false)
                    {
                        WriteRaccoon(ul_pRacReg[i-2],RegAddr[i], phadd[j]);
                    }
//                    ucSum += ul_pRacReg[i-2];
                 CLRWDT(); 
                }
            }
        }
        switch(phadd[j])   //角差 
        {
            case APhy:
                ul_pRacReg = (uint32*)&gs_JbPm.gs_JBA.RacPHC;
                break;
            case BPhy:
                ul_pRacReg = (uint32*)&gs_JbPm.gs_JBB.RacPHC;
                break;  
            case CPhy:
                ul_pRacReg = (uint32*)&gs_JbPm.gs_JBC.RacPHC;
                break;
        }
        tmp = WriteRaccoon(*ul_pRacReg, PHC, phadd[j]); 
        if(tmp ==false)
        {
          WriteRaccoon(*ul_pRacReg, PHC, phadd[j]);  
        }
        CLRWDT();
//        ucSum += *ul_pRacReg;
//    
//        ucSum = 0xFFFFFFFF-ucSum;
//        WriteRaccoon( ucSum, CKSUM, phadd[j]);
     }
    return true;
}


/*=========================================================================================\n
* @function_name: InitRaccoon
* @function_file: Raccoon.c
* @描述: 初始化9281
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-10)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void InitRaccoon(void)
{
    Init_RacCtrl();
//  EnyB_PurInit();         //计量模块上电初始化
//  V9281_InitDefaultPara();
//
//  if(UpdateRaccoonPara())
//  {
//      CheckRegSum();
//      CheckSysErr();
//  }
}

/*=========================================================================================\n
* @function_name: Read_Power
* @function_file: EnergyBottom.c
* @描述:读取功率值
* 
* @参数: 
* @返回: 
* @作者:   hjj (2013-09-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 Read_Power(uint16 reg,uint8 phyadd)
{
    Word32 TempValue;
    if(ReadRaccoon(reg,1,phyadd))
    {
        memcpy((uint8*)TempValue.byte,gs_RacCtrl.ucBuf+3,4);
    }else
    {
        TempValue.lword=0;
    }    
    return TempValue.lword;
}

/*=========================================================================================\n
* @function_name: UpdateRaccoonParABC
* @function_file: EnergyBottom.c
* @描述更新校表参数，相位通电情况
* 
* @参数: phyadd  相位 pow 有电标志
* @返回: 
* @作者:   hjj (2013-10-17)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void UpdateRaccoonParABC(uint8 phyadd)
{
    uint8 i;
    uint32 ucSum=0;
    uint32 *ul_pRacReg;
    Clashin();    
    ul_pRacReg=(uint32*)&gs_JbPm.RacSysCtrl;
    for(i=0;i<sizeof(RegAddr)/sizeof(uint16) ;i++)
    {
       CLRWDT(); 
        if(i  < 2)   //SysCtrl,BPFPARA
        {
            if( i == 0) //SysCtrl
            {
//               WriteRaccoon(ul_pRacReg[i],RegAddr[i], phyadd);  //U: x1  IA: x32  IB: x32  +50 PPM
               WriteRaccoon(SysCtrl_Value[phyadd/4],RegAddr[i], phyadd);
               CLRWDT();  
               ucSum += SysCtrl_Value[phyadd/4];//ul_pRacReg[i];
            }else     //BPFPARA
            {
//               WriteRaccoon(ul_pRacReg[i],RegAddr[i], phyadd);  
               WriteRaccoon(gs_BPFPARA_Value.BPFPARA_Value[phyadd/4],RegAddr[i], phyadd);
               CLRWDT();  
               ucSum += gs_BPFPARA_Value.BPFPARA_Value[phyadd/4];//ul_pRacReg[i]; 
            }
        }
        else
        {
            if(i == 2)  //RacUC,RacIAC,RacPAC,RacPADCC,RacQADCC
            {
                switch(phyadd)
                {
                    case APhy:
                        ul_pRacReg = (uint32*)&gs_JbPm.gs_JBA.RacUC;
                        break;
                    case BPhy:
                        ul_pRacReg = (uint32*)&gs_JbPm.gs_JBB.RacUC;
                        break;  
                    case CPhy:
                        ul_pRacReg = (uint32*)&gs_JbPm.gs_JBC.RacUC;
                        break;
                }
            }
            if(i == 4)  //RacPAC
            {
                WriteRaccoon(ul_pRacReg[i-2],RegAddr[i], phyadd);
                CLRWDT(); 
                ucSum += ul_pRacReg[i-2];
                WriteRaccoon(ul_pRacReg[i-2], QAC, phyadd);  //A路无功比差 = 有功比差
                ucSum += ul_pRacReg[i-2];

            }
            else
            {

                WriteRaccoon(ul_pRacReg[i-2],RegAddr[i], phyadd);
                CLRWDT(); 
                ucSum += ul_pRacReg[i-2];
            }
        }
    }
    switch(phyadd)   //角差 
    {
    case APhy:
      ul_pRacReg = (uint32*)&gs_JbPm.gs_JBA.RacPHC;
      break;
    case BPhy:
      ul_pRacReg = (uint32*)&gs_JbPm.gs_JBB.RacPHC;
      break;  
    case CPhy:
      ul_pRacReg = (uint32*)&gs_JbPm.gs_JBC.RacPHC;
      break;
    }
    WriteRaccoon(*ul_pRacReg, PHC, phyadd); 
    CLRWDT();
    ucSum += *ul_pRacReg;
    
    ucSum += AnaCtrl1_Val;
    WriteRaccoon( AnaCtrl1_Val, 0x0183, phyadd); //CSEL<1：0>设置为11
    
    ucSum = 0xFFFFFFFF-ucSum;
    WriteRaccoon(ucSum,CKSUM,phyadd);

    Clashout();
}

/*=========================================================================================\n
* @function_name: Accumulate_abc
* @function_file: EnergyBottom.c
* @描述分相电量累加100MS
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-1-7)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Accumulate_abc(void)
{
    uint8 i;
    uint32 tmp_egyth;
    
    if(Accumulate_abc_times > 3) 
    {
        Accumulate_abc_times -= 4;      
         /*各相有功能量*/
       /*    tmp_egyth =  0x2BD85D;              //0x2BD85D80
            gs_power.ul_power_all[0] += (0x7B7839C >> 8);
            gs_power.ui_power_rem[0] += (0x7B7839C & 0x000000ff);
            if(gs_power.ui_power_rem[0] > 255)
            {
                gs_power.ul_power_all[0]++;
                gs_power.ui_power_rem[0] -= 255;
            }
            if(gs_power.ul_power_all[0] >= tmp_egyth)
            {
                gs_power.ul_power_all[0] -= tmp_egyth;
                guc_80msCnt = 8;
                P9OD &=~ (BIT3);
                EnyB_IntCF(0,active);               
            }*/
        tmp_egyth =  (gs_JbPm.RacEGYTH*4/3) >>8; 
       for(i =0;i <3;i++)
        {
            gs_power.ul_power_all[i] += (gs_power_enerycal.ul_power[i] >> 8);
            gs_power.ui_power_rem[i] += (gs_power_enerycal.ul_power[i] & 0x000000ff);
            if(gs_power.ui_power_rem[i] > 255)
            {
                gs_power.ul_power_all[i]++;
                gs_power.ui_power_rem[i] -= 255;
            }
            if(gs_power.ul_power_all[i] >= tmp_egyth)
            {
                gs_power.ul_power_all[i] -= tmp_egyth;
                EnyB_IntCF(i,active);
            }
        }  
         /*各相无功能量*/
        tmp_egyth =  (gs_JbPm.RacREEGYTH*4/3) >>8;
        for(i =0;i <3;i++)
        {
            gs_power.ul_Npower_all[i] += (gs_power_enerycal.ul_Npower[i] >> 8);
            gs_power.ui_Npower_rem[i] += (gs_power_enerycal.ul_Npower[i] & 0x000000ff);
            if(gs_power.ui_Npower_rem[i] > 255)
            {
                gs_power.ul_Npower_all[i]++;
                gs_power.ui_Npower_rem[i] -= 255;
            }

            if(gs_power.ul_Npower_all[i] >= tmp_egyth)
            {
                gs_power.ul_Npower_all[i] -= tmp_egyth;
                EnyB_IntCF(i,reactive);
            }
        } 
    }
    
}

/*=========================================================================================\n
* @function_name: Clashin
* @function_file: EnergyBottom.c
* @描述串口保护进入，防止三相周期读与刷校表参数冲突
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-1-7)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Clashin(void)
{
    ExInt2IE&=~BIT3;   //关中断接收；
    gs_RacPhy.uc_Physendtim = 0; //读ABC三相超时关闭；
    guc_ABCreadbegian = 0; //ABC三相周期读定时器关闭;
    gui_ReadABCPhy = 0;
    gs_RacPhy.uc_Sendstat = false;
    guc_CommDelayTime=8;
    while(guc_CommDelayTime);
    ExInt2IE |= BIT3; 
    gs_RacPhy.uc_Uartstat = 0;
   
}

/*=========================================================================================\n
* @function_name: Clashin
* @function_file: EnergyBottom.c
* @描述串口保护结束，防止三相周期读与刷校表参数冲突
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-1-7)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Clashout(void)
{
    guc_ABCreadbegian = 1;
}

/*=========================================================================================\n
* @function_name: Clashin
* @function_file: EnergyBottom.c
* @描述通过中断输出检测校表参数，如果校验不对，刷校表参数
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-2-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//void RaccoonPara_Check(void)
//{
//    if(gs_phyint.Aint == 0 && !gs_RacPhy.uc_Powstat&power_a) //A相无过零中断并且A相有电
//    {
//        UpdateRaccoonParABC(APhy);
//    } 
//    
//    if(gs_phyint.Bint == 0 && !gs_RacPhy.uc_Powstat&power_b)
//    {
//         UpdateRaccoonParABC(BPhy);
//    }
//    
//    if(gs_phyint.Cint == 0 && !gs_RacPhy.uc_Powstat&power_c)
//    {
//         UpdateRaccoonParABC(CPhy);
//    }
//}

/*=========================================================================================\n
* @function_name: Clashin
* @function_file: EnergyBottom.c
* @描述  系统控制寄存器自检错误中断标志位 检测
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-2-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CHKERR_Bit_scan(void)
{
  Word32 TempValue;
  static uint8 ChkErrCnt = 0;
  
  if(( ChkErrCnt > 4) && (guc_ReadingABC_Flg == 0))
  {
    ChkErrCnt = 0;
    
    if(!(gs_RacPhy.uc_Powstat&power_a))  //A有电，且能正常通信
    {
      if(ReadRaccoon(SysSts,1,APhy))
      {
        memcpy((uint8*)TempValue.byte,gs_RacCtrl.ucBuf+3,4);
        if( TempValue.lword & EBIT2)  //CHKERR位
        {
          gui_RefreshEvent |= flgEtPara_InitA;
        }
      }
    }
    
    if(!(gs_RacPhy.uc_Powstat&power_b))  //B有电，且能正常通信
    {
      if(ReadRaccoon(SysSts,1,BPhy))
      {
        memcpy((uint8*)TempValue.byte,gs_RacCtrl.ucBuf+3,4);
        if( TempValue.lword & EBIT2)  //CHKERR位
        {
          gui_RefreshEvent |= flgEtPara_InitB;
        }
      }
    }
    
    if(!(gs_RacPhy.uc_Powstat&power_c))  //C有电，且能正常通信
    {
      if(ReadRaccoon(SysSts,1,CPhy))
      {
        memcpy((uint8*)TempValue.byte,gs_RacCtrl.ucBuf+3,4);
        if( TempValue.lword & EBIT2)  //CHKERR位
        {
          gui_RefreshEvent |= flgEtPara_InitC;
        }
      }
    }
  }else
  {
    ChkErrCnt++;
  }
  
}

/*=========================================================================================\n
* @function_name: Clashin
* @function_file: EnergyBottom.c
* @描述断相上电刷校表参数
* 
* @参数: 
* @返回: 
* @作者:   hjj (2015-2-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Recover_from_powerdown_scan(void)
{
    static uint8 Powstat_tmp = 0;
    
    if((!(gs_RacPhy.uc_Powstat&power_a)) && ((gs_RacPhy.uc_Powstat&power_a) != (Powstat_tmp&power_a)))
    {

        gui_RefreshEvent |= flgEtPara_InitA;
    }

    if((!(gs_RacPhy.uc_Powstat&power_b)) && ((gs_RacPhy.uc_Powstat&power_b) != (Powstat_tmp&power_b)))
    {
       gui_RefreshEvent |= flgEtPara_InitB;
    }
         
    if((!(gs_RacPhy.uc_Powstat&power_c)) && ((gs_RacPhy.uc_Powstat&power_c) != (Powstat_tmp&power_c)))
    {
       gui_RefreshEvent |= flgEtPara_InitC;   
    }
    
    Powstat_tmp = gs_RacPhy.uc_Powstat;   //保存上一次的状态
}



/*=========================================================================================\n
* @function_name: Cal_RealPHDat
* @function_file: EnergyBottom.c
* @描述  计算实际相位数据
* 
* @参数: 
* @返回: 采样点对应的时间
* @作者:
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
float Cal_RealPHDat(uint32 ul_ZXDatReg, uint32 ul_ZXDat, uint32 ul_PHDat, uint32 T8Baud)
{
  static float tmp;
  
  tmp = (float)ul_PHDat - ((float)ul_ZXDat*512 / (float)((int32)ul_ZXDat - (int32)ul_ZXDatReg));
   
//  tmp = tmp * 20 / ((float)T8Baud / 5376 * 128);
  tmp = tmp / ((float)T8Baud * Actual_BAUDRate/ 26214400);
  
  return tmp;
}

float Cal_RealSample(uint32 ul_hz)
{
  float tmp;
  
  tmp = 327680000 / (float)ul_hz;
  
  return tmp;
}
/*float Cal_CycPHDat(uint32 T8Baud)
{
  float tmp;
  
  //tmp = (float)T8Baud / T8_BAUDRate * 128;//zhoub
   tmp = (float)T8Baud / T8_BAUDRate *65536;
  return tmp;
}*/
//uint32 inst_frqA,inst_frqB,inst_frqC;
/*=========================================================================================\n
* @function_name: Check_ABC_Phase
* @function_file: EnergyBottom.c
* @描述  相位测量
* 
* @参数: 
* @返回: 
* @作者:
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Check_ABC_Phase(void)
{
  static float tmp1,tmp2,tmp3;
  
  if((guc_ReadingABC_Flg == 0))
  {
    BroadcastWriteRaccoon( 0x01, 0x0198);  //写入任意非零值，相位测量命令
    DelayXms(30);  //MCU: 26MHz  30ms
    
    if(!(gs_RacPhy.uc_Powstat&power_a))  //有电，且能正常通信
    {
      if(ReadRaccoon(ZXDATREG,5,APhy))
      {
        MemCpy((uint8*)&gs_phytim.gs_APhaseDat.ZXDatReg, gs_RacCtrl.ucBuf+3, 20);
      }else
      {
        MemSet((uint8*)&gs_phytim.gs_APhaseDat.ZXDatReg, 0, 20);
      }
    }else
    {
      MemSet((uint8*)&gs_phytim.gs_APhaseDat.ZXDatReg, 0, 20);
    }
    
    if(!(gs_RacPhy.uc_Powstat&power_b)) //有电，且能正常通信
    {
      if(ReadRaccoon(ZXDATREG,5,BPhy))
      {
        MemCpy((uint8*)&gs_phytim.gs_BPhaseDat.ZXDatReg, gs_RacCtrl.ucBuf+3, 20);
      }else
      {
        MemSet((uint8*)&gs_phytim.gs_BPhaseDat.ZXDatReg, 0, 20);
      }
    }else
    {
      MemSet((uint8*)&gs_phytim.gs_BPhaseDat.ZXDatReg, 0, 20);
    }
    
    if(!(gs_RacPhy.uc_Powstat&power_c)) //有电，且能正常通信
    {
      if(ReadRaccoon(ZXDATREG,5,CPhy))
      {
        MemCpy((uint8*)&gs_phytim.gs_CPhaseDat.ZXDatReg, gs_RacCtrl.ucBuf+3, 20);
      }
    }else
    {
      MemSet((uint8*)&gs_phytim.gs_CPhaseDat.ZXDatReg, 0, 20);
    }
      
    
    if(gs_phytim.gs_APhaseDat.PHDat == 0)  //A=0
    {
      gs_phytim.APhytim = 0;
      gs_phytim.BPhytim = 0;
      gs_phytim.CPhytim = 0;
      gs_phytim.RevPhaseFlg = 0;
    }else
    {
      if(gs_phytim.gs_BPhaseDat.PHDat == 0)
      {
        if(gs_phytim.gs_CPhaseDat.PHDat == 0) //A
        {
          gs_phytim.APhytim = 0;
          gs_phytim.BPhytim = 0;
          gs_phytim.CPhytim = 0;
          gs_phytim.RevPhaseFlg = 0;
        }else       //A,C
        {
          gs_phytim.APhytim = 0;
          gs_phytim.BPhytim = 0;
          
          tmp1 = Cal_RealPHDat(gs_phytim.gs_APhaseDat.ZXDatReg, gs_phytim.gs_APhaseDat.ZXDat, gs_phytim.gs_APhaseDat.PHDat, gs_phytim.gs_APhaseDat.T8Baud);
          tmp3 = Cal_RealPHDat(gs_phytim.gs_CPhaseDat.ZXDatReg, gs_phytim.gs_CPhaseDat.ZXDat, gs_phytim.gs_CPhaseDat.PHDat, gs_phytim.gs_CPhaseDat.T8Baud);
          
          if( gs_phytim.gs_APhaseDat.PHDat < gs_phytim.gs_CPhaseDat.PHDat) //A在前、C在后
          {
            gs_phytim.CPhytim = (uint32)((tmp3 - tmp1)/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);//42.667   21845.333*1200
            
            if((gs_phytim.gs_CPhaseDat.PHDat - gs_phytim.gs_APhaseDat.PHDat) < (Cal_RealSample(gs_UI.ul_Hz[0])/2))// <180° 逆相序64
            {
              gs_phytim.RevPhaseFlg = 0;
            }else
            {
              gs_phytim.RevPhaseFlg = 0;
            }
            
          }else //C在前、A在后
          {
      //      tmp3 += Cal_CycPHDat( gs_phytim.gs_CPhaseDat.T8Baud);
            gs_phytim.CPhytim = (uint32)((tmp3 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            
            if((gs_phytim.gs_APhaseDat.PHDat - gs_phytim.gs_CPhaseDat.PHDat) > (Cal_RealSample(gs_UI.ul_Hz[0])/2))// >180° 逆相序
            {
              gs_phytim.RevPhaseFlg = 0;
            }else
            {
              gs_phytim.RevPhaseFlg = 0;
            }
            
          }
        }
      }else
      {
        if(gs_phytim.gs_CPhaseDat.PHDat == 0)   //A,B
        {
          gs_phytim.APhytim = 0;
          gs_phytim.CPhytim = 0;
          
          tmp1 = Cal_RealPHDat(gs_phytim.gs_APhaseDat.ZXDatReg, gs_phytim.gs_APhaseDat.ZXDat, gs_phytim.gs_APhaseDat.PHDat, gs_phytim.gs_APhaseDat.T8Baud);
          tmp2 = Cal_RealPHDat(gs_phytim.gs_BPhaseDat.ZXDatReg, gs_phytim.gs_BPhaseDat.ZXDat, gs_phytim.gs_BPhaseDat.PHDat, gs_phytim.gs_BPhaseDat.T8Baud);

          if( gs_phytim.gs_APhaseDat.PHDat < gs_phytim.gs_BPhaseDat.PHDat) //A在前、B在后
          {
            gs_phytim.BPhytim = (uint32)((tmp2 - tmp1)/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            
            if((gs_phytim.gs_BPhaseDat.PHDat - gs_phytim.gs_APhaseDat.PHDat) >(Cal_RealSample(gs_UI.ul_Hz[0])/2))// >180° 逆相序
            {
              gs_phytim.RevPhaseFlg = 0;
            }else
            {
              gs_phytim.RevPhaseFlg = 0;
            }
            
          }else //B在前、A在后
          {
      //      tmp2 += Cal_CycPHDat( gs_phytim.gs_BPhaseDat.T8Baud);
            gs_phytim.BPhytim = (uint32)((tmp2 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            
            if((gs_phytim.gs_APhaseDat.PHDat - gs_phytim.gs_BPhaseDat.PHDat) <(Cal_RealSample(gs_UI.ul_Hz[0])/2))// <180° 逆相序
            {
              gs_phytim.RevPhaseFlg = 0;
            }else
            {
              gs_phytim.RevPhaseFlg = 0;
            }
          }
        }else    //A,B,C
        {
          tmp1 = Cal_RealPHDat(gs_phytim.gs_APhaseDat.ZXDatReg, gs_phytim.gs_APhaseDat.ZXDat, gs_phytim.gs_APhaseDat.PHDat, gs_phytim.gs_APhaseDat.T8Baud);
          tmp2 = Cal_RealPHDat(gs_phytim.gs_BPhaseDat.ZXDatReg, gs_phytim.gs_BPhaseDat.ZXDat, gs_phytim.gs_BPhaseDat.PHDat, gs_phytim.gs_BPhaseDat.T8Baud);
          tmp3 = Cal_RealPHDat(gs_phytim.gs_CPhaseDat.ZXDatReg, gs_phytim.gs_CPhaseDat.ZXDat, gs_phytim.gs_CPhaseDat.PHDat, gs_phytim.gs_CPhaseDat.T8Baud);

          if(( gs_phytim.gs_APhaseDat.PHDat < gs_phytim.gs_BPhaseDat.PHDat) //A,(B,C)或A,(C,B)
             &&( gs_phytim.gs_APhaseDat.PHDat < gs_phytim.gs_CPhaseDat.PHDat))
          {
            gs_phytim.APhytim = 0;            
            gs_phytim.BPhytim = (uint32)((tmp2 - tmp1)/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            gs_phytim.CPhytim = (uint32)((tmp3 - tmp1)/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            
            if( gs_phytim.gs_BPhaseDat.PHDat < gs_phytim.gs_CPhaseDat.PHDat)
            {
              gs_phytim.RevPhaseFlg = 0;
            }else   //逆相序
            {
              gs_phytim.RevPhaseFlg = 1;
            }
          }
          else if(( gs_phytim.gs_BPhaseDat.PHDat < gs_phytim.gs_APhaseDat.PHDat)//B,(C,A)或B,(A,C)
                   &&( gs_phytim.gs_BPhaseDat.PHDat < gs_phytim.gs_CPhaseDat.PHDat))
          {
            if( gs_phytim.gs_CPhaseDat.PHDat < gs_phytim.gs_APhaseDat.PHDat)
            {
              gs_phytim.RevPhaseFlg = 0;
              
         //     tmp2 += Cal_CycPHDat( gs_phytim.gs_BPhaseDat.T8Baud);
         //     tmp3 += Cal_CycPHDat( gs_phytim.gs_CPhaseDat.T8Baud);
              gs_phytim.BPhytim = (uint32)((tmp2 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
              gs_phytim.CPhytim = (uint32)((tmp3 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            }else   //逆相序
            {
              gs_phytim.RevPhaseFlg = 1;
              
        //      tmp2 += Cal_CycPHDat( gs_phytim.gs_BPhaseDat.T8Baud);
              gs_phytim.BPhytim = (uint32)((tmp2 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
              gs_phytim.CPhytim = (uint32)((tmp3 - tmp1)/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            }
            gs_phytim.APhytim = 0;              
          }
          else if(( gs_phytim.gs_CPhaseDat.PHDat < gs_phytim.gs_APhaseDat.PHDat)//C,(A,B)或C,(B,A)
                   &&( gs_phytim.gs_CPhaseDat.PHDat < gs_phytim.gs_BPhaseDat.PHDat))
          {
            if( gs_phytim.gs_APhaseDat.PHDat < gs_phytim.gs_BPhaseDat.PHDat)
            {
              gs_phytim.RevPhaseFlg = 0;
              
        //      tmp3 += Cal_CycPHDat( gs_phytim.gs_CPhaseDat.T8Baud);
              gs_phytim.BPhytim = (uint32)((tmp2 - tmp1)/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
              gs_phytim.CPhytim = (uint32)((tmp3 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            }else   //逆相序
            {
              gs_phytim.RevPhaseFlg = 1;
              
           //   tmp2 += Cal_CycPHDat( gs_phytim.gs_BPhaseDat.T8Baud);
           //   tmp3 += Cal_CycPHDat( gs_phytim.gs_CPhaseDat.T8Baud);
              gs_phytim.BPhytim = (uint32)((tmp2 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
              gs_phytim.CPhytim = (uint32)((tmp3 - (tmp1-Cal_RealSample(gs_UI.ul_Hz[0])))/(Cal_RealSample(gs_UI.ul_Hz[0])/3)*1200);
            }
            gs_phytim.APhytim = 0;
          }
        }
      }      
    }      
  }
}


