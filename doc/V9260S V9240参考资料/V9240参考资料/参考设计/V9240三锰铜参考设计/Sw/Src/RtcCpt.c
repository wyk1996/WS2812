#define RTCCPTEXT
#include "Include.h"
/*=========================================================================================\n
* @function_name: RtcParaFsToE2
* @function_file: RtcCpt.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-07-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RtcParaFsToE2(uint8* ucData)
{
    uint8 i;
    uint8 code* p;
    uint8 CalSum;
    //CalCRC;
    uint8 Sum;
    int16 tempoffset;

    //读取错误的话,从FLASH中读取数据
    //判断FLASH是否是正确的,正确的话，写入E2中
    p=(uint8 code*)0x480;
    for(i=0;i<3;i++)                        //读取温度偏移量
    {
        FCpyTMem(XDATA_RAMZONE,p+i*4,4);
        CalSum=DoSum(XDATA_RAMZONE,2)+0x33;
        Sum=XDATA_RAMZONE[2];
        if(CalSum==Sum)             //Crc正确
        {
            //tempoffset=*((int16*)(XDATA_RAMZONE));
            MemInvertCpy((uint8*)&tempoffset,XDATA_RAMZONE,2);
    
            if(tempoffset<0)
            {
                tempoffset-=5;
                tempoffset/=10;
            }else
            {
                tempoffset+=5;       //四舍五入
                tempoffset/=10;
            }
            ucData[0]=(uint8)tempoffset;
            //1个字节，唉！！！！
            break;
        }
    }
    
     //读取晶体常温偏移量
    
    p=(uint8 code*)0x48c;
    
    for(i=0;i<3;i++)
    {
    
        FCpyTMem(XDATA_RAMZONE,p+i*4,4);
        CalSum=DoSum(XDATA_RAMZONE,2)+0x33;
        Sum=XDATA_RAMZONE[2];
    
        //判断FLASH是否是正确的,正确的话，写入E2中
        if(CalSum==Sum)             //Crc正确
        {   //4个字节
            XDATA_RAMZONE[3]=XDATA_RAMZONE[1];
            XDATA_RAMZONE[2]=XDATA_RAMZONE[0];
            if(XDATA_RAMZONE[2]>=0x80)
            {
                XDATA_RAMZONE[0]=0xff;
                XDATA_RAMZONE[1]=0xff;
            }else
            {
                XDATA_RAMZONE[0]=0x00;
                XDATA_RAMZONE[1]=0x00;
            }
    
            MemCpy(ucData+1,XDATA_RAMZONE,4);
            break;
        }
    }
    //重新计算CRC,写入E2中
    //CalCRC=do_CRC(ucData,sizeof(ucData)-2);
    //Uint16_To_Uint8(CalCRC,ucData+sizeof(ucData)-2);
    //BE_WriteP(EEP_RTCTEMPOFFSET,(uint8*)ucData,28);     //  读取RTC参数
    SysE2ParaSetManage(EEP_RTCTEMPOFFSET, (uint8*)ucData,5);//写入常温偏移和RTC常温偏移

}
/*=========================================================================================\n
* @function_name: RecoverRtcParaFrmFalsh
* @function_file: RtcCpt.c
* @描述: 恢复RTC参数从FLASH中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-07-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RecoverRtcParaFrmFalsh(void)
{
    //首先读取RTC参数,计算CRC是否正确
    uint8 ucData[28];
    BE_ReadP(EEP_RTCTEMPOFFSET,(uint8*)ucData,28);     //  读取RTC参数
    RtcParaFsToE2(ucData);
}

/*=========================================================================================\n
* @function_name: RecoverRtcPara
* @function_file: RtcCpt.c
* @描述: 恢复RTC参数
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RecoverRtcPara(void)
{

    //首先读取RTC参数,计算CRC是否正确
    uint8 ucData[28];
//  uint8 i;
//  uint8 code* p;
//  uint16 CalSum;
//  //CalCRC;
//  uint16 Sum;
//  int16 tempoffset;


    BE_ReadP(EEP_RTCTEMPOFFSET,(uint8*)ucData,28);     //  读取RTC参数

    if(Uint8_To_Uint16(ucData+sizeof(ucData)-2)!=do_CRC(ucData,sizeof(ucData)-2))
    {
        RtcParaFsToE2(ucData);

//      //读取错误的话,从FLASH中读取数据
//      //判断FLASH是否是正确的,正确的话，写入E2中
//      p=(uint8 code*)0x480;
//      for(i=0;i<3;i++)                        //读取温度偏移量
//      {
//          FCpyTMem(XDATA_RAMZONE,p+i*4,4);
//          CalSum=DoSum(XDATA_RAMZONE,2)+0x33;
//          Sum=XDATA_RAMZONE[2];
//          if(CalSum==Sum)             //Crc正确
//          {
//              //tempoffset=*((int16*)(XDATA_RAMZONE));
//              MemInvertCpy((uint8*)&tempoffset,XDATA_RAMZONE,2);
//
//              if(tempoffset<0)
//              {
//                  tempoffset-=5;
//                  tempoffset/=10;
//              }else
//              {
//                  tempoffset+=5;       //四舍五入
//                  tempoffset/=10;
//              }
//              ucData[0]=(uint8)tempoffset;
//              //1个字节，唉！！！！
//              break;
//          }
//      }
//
//       //读取晶体常温偏移量
//
//      p=(uint8 code*)0x48c;
//
//      for(i=0;i<3;i++)
//      {
//
//          FCpyTMem(XDATA_RAMZONE,p+i*4,4);
//          CalSum=DoSum(XDATA_RAMZONE,2)+0x33;
//          Sum=XDATA_RAMZONE[2];
//
//          //判断FLASH是否是正确的,正确的话，写入E2中
//          if(CalSum==Sum)             //Crc正确
//          {   //4个字节
//              XDATA_RAMZONE[3]=XDATA_RAMZONE[1];
//              XDATA_RAMZONE[2]=XDATA_RAMZONE[0];
//              if(XDATA_RAMZONE[2]>=0x80)
//              {
//                  XDATA_RAMZONE[0]=0xff;
//                  XDATA_RAMZONE[1]=0xff;
//              }else
//              {
//                  XDATA_RAMZONE[0]=0x00;
//                  XDATA_RAMZONE[1]=0x00;
//              }
//
//              MemCpy(ucData+1,XDATA_RAMZONE,4);
//              break;
//          }
//      }
//      //重新计算CRC,写入E2中
//      //CalCRC=do_CRC(ucData,sizeof(ucData)-2);
//      //Uint16_To_Uint8(CalCRC,ucData+sizeof(ucData)-2);
//      //BE_WriteP(EEP_RTCTEMPOFFSET,(uint8*)ucData,28);     //  读取RTC参数
//      SysE2ParaSetManage(EEP_RTCTEMPOFFSET, (uint8*)ucData,5);//写入常温偏移和RTC常温偏移
    }
}
/*=========================================================================================\n
* @function_name: Correct_Rtc
* @function_file: RtcCpt.c
* @描述: 校正RTC
*
*
* @参数:
* @param:Hvalue
* @param:Lvalue
* @返回:
* @作者:   lwb (2012-03-24)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void SetRTCNormal(int16 offset)
{
    Word32 temp1,temp2;
    int16 temp3;
    temp2.lword=0;
    temp1.lword=(uint16)offset;
    if((temp1.byte[1]>0x80))
    { 
        temp1.word[0]=(~temp1.word[0]);
        temp1.word[0]&=0x1fff;
        temp1.lword=temp1.lword*20/30;                      //晶振偏移精度为0.1ppm
        temp2.lword=6553600-1;
        temp2.lword=temp2.lword+temp1.lword;
    }
    else
    {
        temp1.lword=(temp1.lword)*20/30;
        temp2.lword=6553600;
        temp2.lword=temp2.lword-temp1.lword;
    }
    temp3=offset/10;
    temp1.lword=(uint16)temp3;
    RTCPEN=0x96;                        //1S唤醒
    RTCPWD=0x57;
    DelayOSC(5);
    RTCCH=temp1.byte[1];
    RTCCL=temp1.byte[0];
    DIVTHH=temp2.byte[2];
    DIVTHM=temp2.byte[1];
    DIVTHL=temp2.byte[0];
    DelayOSC(5);
    RTCPEN=0x96;
    RTCPWD=0x56;
}
const uint32 code CtrBgpTab[]=
{
    1000,
    1008,
    1015,
    1022,
    974,
    980,
    985,
    993,
};
/*=========================================================================================\n
* @function_name: CalRTC
* @function_file: RtcCpt.h
* @描述: 计算RTC偏移ppm
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-22)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void CalRTC(void)
{
    uint8 ucdata[4];
    Word32 temp1;
    int32 delta;
    float temperature; //当前温度
    int8  tempoffset;
    int16  OSC;
    uint32 BparaTemp[5];
    int32 Bpara;
    uint32  MAdcValue;
    uint8 Ti;
    uint32  temp;
    S_TEMPPARA s_TempPara;
    S_CTRLBGP  s_CtrlBgp;
    uint8 code *p;
    uint8 TempParaSum;
    uint8 CtrlBagSum;

    p=(uint8 code*)0x420;
    for(uint8 i=0;i<3;i++)
    {
        FCpyTMem((uint8*)&s_TempPara.ul_TempParaA,p+i*24,sizeof(S_TEMPPARA));
        TempParaSum=DoSum((uint8*)&s_TempPara.ul_TempParaA,sizeof(S_TEMPPARA)-1)+0x33;
        if(TempParaSum==s_TempPara.uc_Add33)
        {
            break;
        }
    }

     p=(uint8 code*)0x414;
     for(uint8 i=0;i<3;i++)
     {
         FCpyTMem((uint8*)&s_CtrlBgp.ucVersion,p+i*4,sizeof(S_CTRLBGP));
         CtrlBagSum=DoSum((uint8*)&s_CtrlBgp.ucVersion,sizeof(S_CTRLBGP)-1)+0x33;
         if(CtrlBagSum==s_CtrlBgp.uc_Add33)
         {
             break;
         }
     }

    if(_SYS_IS_PWR_OFF)
    {    
        MAdcValue=EnyB_ReadMeterParaACK(DATAOM);//读M通道原始值
    }
    else
    {    
        MAdcValue=EnyB_ReadMeterParaACK(DATAADM);//读M通道原始值
    }

    temp=MAdcValue>>16;

    if(CtrlBagSum==s_CtrlBgp.uc_Add33)
    {
        if(s_CtrlBgp.ucVersion>0xA2)
        {
            temp=(temp*CtrBgpTab[s_CtrlBgp.ucCtrlBGP])/1000;
        }
        CtrlBGP=(s_CtrlBgp.ucCtrlBGP<<1);
    }else
    {
        CtrlBGP=InitPara38[0];
    }

    if(TempParaSum==s_TempPara.uc_Add33)
    {
        temperature=(s_TempPara.ul_TempParaB*sqrt(s_TempPara.ul_TempParaC+s_TempPara.ul_TempParaD*temp)-s_TempPara.ul_TempParaA)/s_TempPara.ul_TempParaE;
    }else
    {
        temperature=(-116500+80*sqrt(1042777+60*temp))/99;
    }
    //temperature=(-58.25+sqrt(3396.5584-0.096*(18001.2-(float)temp)))/0.0495;//(sqrt(3396.5584-0.096*(18001.2-(float)temp))/0.048)-58.28;



    BE_ReadP(EEP_RTCTEMPOFFSET,(uint8*)&tempoffset,1);     //  温度常温偏移

    temperature+=tempoffset;

    if(_SYS_IS_PWR_OFF)
    {
        temperature-=6;
    }

    if(temperature>90)
    {   
        temperature=90;
    }else if(temperature<(-43))
    {   
        temperature=-43;
    }

    //保存温度到全局变量中
    gi_Temprature=(uint16)(temperature*10);
    //读取RTCB值
    BE_ReadP(EEP_RTCBVAL, (uint8*)BparaTemp,20);           //RTCB值

    if(temperature>=60)
    {
        Bpara=BparaTemp[4];
    }
    else if((temperature<60)&&(temperature>=40))
    {
        Bpara=BparaTemp[3];
    }
    else if((temperature<40)&&(temperature>=0))
    {
        Bpara=BparaTemp[2];
    }
    else if((temperature<0)&&(temperature>=-20))
    {
        Bpara=BparaTemp[1];
    }
    else if(temperature<-20)
    {
        Bpara=BparaTemp[0];
    }

    //读取顶点温度
    BE_ReadP(EEP_RTCDDTEMP, (uint8*)&Ti,1);  //顶点温度

    //读取晶体常温偏移
    BE_ReadP(EEP_RTCFRQOFFSET, (uint8*)ucdata,4);           // RTC常温偏移
 
    MemInvertCpy(temp1.byte,ucdata,4);

    delta=(int32)temp1.lword;

    OSC=(int16)((((float)Bpara)*(temperature-((float)Ti))*(temperature-((float)Ti)))/1000000+delta);

    SetRTCNormal(OSC);
}
