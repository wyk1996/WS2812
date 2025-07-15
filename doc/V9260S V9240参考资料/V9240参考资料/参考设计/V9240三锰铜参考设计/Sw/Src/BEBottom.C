#include "Include.h"


//底层驱动函数
uint8 BE_I_ReadB(void);         //读一个字节，主机的应答不在该程序中完成
uint8 BE_I_WriteB(uint8);       //写入一个字节，同时返回从机是否有应答信号
void BE_I_Start(void);          //I2C驱动的起始位时序控制
void BE_I_Stop(void);           //I2C驱动的停止位时序控制
void BE_I_NAK(void);            //I2C驱动NAK时序控制
void BE_I_ACK(void);            //I2C驱动ACK时序控制

/*=========================================================================================\n
* @function_name: Delay_Unit5us
* @function_file: BEBottom.C
* @描述: I2C驱动时序延时程序
* 
* @参数: 
* @返回: 
* @作者: Lwb (2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:  
===========================================================================================*/
//void Delay_Unit5us(void)
//{
//    nop();
//    nop();
//}

#define Delay_Unit5us() {nop();nop();}
/*=========================================================================================\n
* @function_name: Stop_Delay
* @function_file: BEBottom.C
* @描述: 
* 
* @参数: 
* @返回: 
* @作者: Lwb (2011-7-27)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
void Stop_Delay(void) 
{ 
    uint8 DelayCount=BE_Const_DyWrite;           //这里延时30就会错误,31就可以正确。目前延时38，保证正确性
    uint8 i;
    while (DelayCount--)
    {
        for(i=0;i<30;i++)
        {
            nop();
        }
    }
} 

//uint8 gui_BE2ErrCnt;
//以下为提供给应用程序的一个接口函数
/*=========================================================================================\n
* @function_name: BE_WriteP
* @function_file: BEBottom.C
* @描述: 
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* @param:Len  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb (2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
uint8 BE_WriteP(uint32 address,uint8 *buff,uint8 Len)
{
    uint8 i;
    uint8 uc_sublen;                                // 每次写入字节数
    uint8 uc_retrycnt;                              // 重试次数 
    uint8 uc_QjAddress;                             // 存储器件地址，包含块号的信息
    uint16 ui_E2Address;                            // 具体地址，指一块内 
    uint16 ui_tmp1;                                 //
    uint16 ui_tmp2;                                 //
    uint32 ulAddress;

   // uint8 ucg_cmpbuf[BE_Const_Page];              // 比对数据缓存
    uint8 * ucp_buff2;                              //
    BE_WP_0();                                      //E2 WP=0
    // 首先根据不同的器件获取地址的器件地址         //
#if(_SW_BE_Type==Const_AT24C04_08_16)               // 以下为24C16的器件驱动
    ui_E2Address = (uint16)address;                 // 如果选择了小容量E2，不考虑多个小容量E2的情况，只有一个
//  ui_E2Address &=0x7ff;                           // 地址保护
//  uc_QjAddress = (uint8)(ui_E2Address>>8);        // 取地址的高位
//  uc_QjAddress <<= 1;                             // 乘 2，
//  uc_QjAddress &= 0x0e;                           //
//  uc_QjAddress += BE_Const_ID;                    // 器件地址，包含了地址的高字节

#elif(_SW_BE_Type==Const_AT24C64)                   // 以下为24LC64
    ui_E2Address = (uint16)address;                 // 取具体的一块中的地址
    ui_E2Address &= 0x1fff;                         //
    uc_QjAddress = BE_Const_ID;                     // 器件地址，包含了地址的高字节 

#elif(_SW_BE_Type==Const_AT24C256)                  //
    ui_E2Address = (uint16)address;                 // 取具体的一块中的地址
    ui_E2Address &= 0x7fff;                         //

//  uc_QjAddress =(uint8)(address>>15);             // 24C256，考虑多个E2连在一起的情况
//  uc_QjAddress <<=1;                              // 取块号
//  uc_QjAddress += BE_Const_ID;                    // 确定器件地址
    uc_QjAddress  = BE_Const_ID;                    // 确定器件地址
#elif(_SW_BE_Type==Const_AT24C512)                  //
    ui_E2Address = (uint16)address;                 // 取具体的一块中的地址
//  uc_QjAddress =(uint8)(address>>16);             // 24C512，考虑多个E2连在一起的情况
//  uc_QjAddress <<=1;                              // 取块号
//  uc_QjAddress += BE_Const_ID;                    // 确定器件地址
    uc_QjAddress = BE_Const_ID;                    // 确定器件地址
#else
    return 1;                                       // 返回 
#endif 

    uc_retrycnt = 0;                                // 重试次数清零

    ui_tmp1 = ~((uint16)BE_Const_Page - 1);         // 获取E2地址的最低几位
    ui_tmp1 &= ui_E2Address;                        //

    ui_tmp2 = ~((uint16)BE_Const_Page - 1);         //
    ui_tmp2 &= (ui_E2Address + Len -1);             //

    if (ui_tmp1 != ui_tmp2)                          // 判别是否有夸页产生
    {
        // 跨页, 获得第一页的写入字节数 
        uc_sublen = BE_Const_Page - (ui_E2Address & ((uint16)BE_Const_Page - 1));       
    }                                               //
    else                                            //
    {
        uc_sublen = Len;                            // 未跨页 
    }

    for (;;)
    {
        Feed_WDT();
        if (uc_retrycnt >= 3)                       // 重试达到3次
        {
            gut_SysWarn.SysWarn.EEPROMERROR=true;   //E2错误
            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt == false      //状态标志未置
               && gs_ReportWord.ui_ReportWord1.ReportWord1.EepErrRpt == 1)           //需要主动上报
            {
                gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = true;         //状态置位
            }
            return 2;                               // 写入失败
        }
#if(_SW_BE_Type==Const_AT24C04_08_16)               // 如果是AT24C16采用如下的程序 
        ui_E2Address &=0x7ff;                       // 地址保护
        uc_QjAddress = (uint8)(ui_E2Address>>8);    // 取地址的高位
        uc_QjAddress <<= 1;                         // 乘 2，
        uc_QjAddress &= 0x0e;                       //
        uc_QjAddress += BE_Const_ID;                // 器件地址，包含了地址的高字节

        BE_I_Start();                               // 

        if(BE_I_WriteB(uc_QjAddress))               // 写入器件地址和地址的高位
        {
            uc_retrycnt++;
            continue;                  
        }

        if(BE_I_WriteB((uint8)ui_E2Address))        // 写入地址低字节
        {
            uc_retrycnt++;
            continue;          
        }

#elif((_SW_BE_Type==Const_AT24C256)||(_SW_BE_Type==Const_AT24C64)||(_SW_BE_Type==Const_AT24C512))                                   
        BE_I_Start();                               //

        if(BE_I_WriteB(uc_QjAddress))               // 写入器件地址
        {
            uc_retrycnt++;
            continue; 
        }
        if(BE_I_WriteB((uint8)(ui_E2Address>>8)))   // 写入地址高字节
        {
            uc_retrycnt++;
            continue;      
        }
        if(BE_I_WriteB((uint8)ui_E2Address))        // 写入地址低字节
        {
            uc_retrycnt++;
            continue; 
        }
#else 
        return 1; 
#endif 

        ucp_buff2 = buff;                           
        for (i=0; i<uc_sublen; i++)                 // 通过循环将数据写入到器件中去
        {
            if(BE_I_WriteB(*buff))                  // 写入1个字节
            {
                break;
            }
            buff ++;                                // 指向下一个字节
        }

        if(i<uc_sublen)
        { 
            buff=ucp_buff2;                         //重试
            uc_retrycnt++;
            continue;   
        }
                                           
        BE_I_Stop();                                // 结束

        Stop_Delay();
      //Delay_Unit5us(BE_Const_DyWrite);            // 延时等待5ms的时间

      /*BE_ReadP(address, ucg_cmpbuf, uc_sublen);   // 读出刚写入的数据
       buff -= uc_sublen;                          // 进行比对
        for (i=0; i<uc_sublen; i++)                  //
        {
            if ( (*buff) != ucg_cmpbuf[i] )         // 不同
            {
                //
                break;                              // 提前跳出循环
            }                                       //
            buff ++;                                //
        }

        if (i < uc_sublen)                           // 不相同==>中间比对中有不正确,说明是提前跳出
        {
            if (uc_retrycnt >= 3)                    // 重试达到3次
            {
                gui_BE2ErrCnt++;                    //失败次数累加
                return 2;                           // 写入失败
            }
            buff = ucp_buff2;                       // 重试
            uc_retrycnt ++;                         //
            continue;                               // 重新开始写入数据
        }
        */
        if (uc_sublen >= Len)                       // 已写完
        {
            break;                                  // 退出
        }                                           //

        //uc_retrycnt = 0;                          // 操作下一页
        ulAddress = (uint32)ui_E2Address + uc_sublen;
        ui_E2Address = (uint16)ulAddress;               // 地址指向下一页

//#if(_SW_BE_Type==Const_AT24C256)                        // 如果是AT24C256采用如下的程序
//        if (ulAddress >= 0x8000)
//        {
//            uc_QjAddress += 2;                          // 片号加
//        }
//#elif(_SW_BE_Type==Const_AT24C512)                      // 如果是AT24C512采用如下的程序
//        if (ulAddress >= 0x10000)
//        {
//            uc_QjAddress += 2;                          // 片号加
//        }
//#endif

        //address += uc_sublen;                       // 读数据的时候，地址也增加
        Len -= uc_sublen;                           // 总长度减少
        if (Len > BE_Const_Page)                    // 重试次数清零
        {
            uc_sublen = BE_Const_Page;              //
        }                                           //
        else                                        //
        {
            uc_sublen = Len;                        //
        }                                           //
    }                                               //
    BE_WP_1();
    gut_SysWarn.SysWarn.EEPROMERROR=false;          //E2正确
    if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt == true)     //判断状态标志
    {
        gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = false;
    }
    return 0;                                       // 正确返回
}
/*=========================================================================================\n
* @function_name: BE_WriteL
* @function_file: BEBottom.C
* @描述: 写一个4字节的数据到E2中
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
//uint8 BE_WriteL(uint32 address,uint32 buff)
//{
//    return(BE_WriteP(address,(uint8 *)(&buff),4));
//}
/*=========================================================================================\n
* @function_name: BE_WriteW
* @function_file: BEBottom.C
* @描述: 写一个字到E2中
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
//uint8 BE_WriteW(uint32 address,uint16 buff)
//{
//    return(BE_WriteP(address,(uint8 *)(&buff),2));
//}
/*=========================================================================================\n
* @function_name: BE_WriteB
* @function_file: BEBottom.C
* @描述: 写一个字节到E2中
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
//uint8 BE_WriteB(uint32 address,uint8 buff)
//{
//    return(BE_WriteP(address,&buff,1));
//}

/*=========================================================================================\n
* @function_name: BE_ReadP
* @function_file: BEBottom.C
* @描述: 读取E2数据
* 
* 
* @参数: 
* @param:address  读取E2数据的首地址
* @param:buff     读取数据存放的缓存
* @param:Len      读取数据的长度
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
uint8 BE_ReadP(uint32 address,uint8 *buff,uint8 Len)
{
    uint8 i;
    uint16 ui_E2Address;
    uint8 uc_QjAddress; 
    uint8 uc_ErrCnt;
    BE_WP_0();                                              //E2 WP=0
    uc_ErrCnt=0;
    for(i=0;i<3;i++)
    {
        uc_ErrCnt++;
        #if(_SW_BE_Type==Const_AT24C04_08_16)               // 以下为24C16的器件驱动
            ui_E2Address = (uint16)address;                 // 如果选择了小容量E2，不考虑多个小容量E2的情况，只有一个
            ui_E2Address &=0x7ff;                           // 地址保护
            uc_QjAddress = (uint8)(ui_E2Address>>8);        // 取地址的高位
            uc_QjAddress <<= 1;                             // 乘 2，
            uc_QjAddress &= 0x0e;                           //
            uc_QjAddress += BE_Const_ID;                    // 器件地址，包含了地址的高字节
        
            BE_I_Start();                                   // start 
            if(BE_I_WriteB(uc_QjAddress))                   // 写入器件地址和地址的高位
            {
                continue;
            }
            if(BE_I_WriteB((uint8)ui_E2Address))            // 写入地址低字节
            {
                continue;
            }
            BE_I_Start();                                   // restart
            if(BE_I_WriteB(uc_QjAddress+1))                 // 写入器件地址，读数据
            {
                continue;                    
            }
        
        #elif(_SW_BE_Type==Const_AT24C64)                   //
            ui_E2Address = (uint16)address;                 // 取具体的一块中的地址
            ui_E2Address &= 0x1fff;                         //
            uc_QjAddress = BE_Const_ID;                     // 确定器件地址
        
            BE_I_Start();                                   // start

            if(BE_I_WriteB(uc_QjAddress))                   // 写入器件地址，同时把块号也写入
            {    
                continue;                      
            }
            if(BE_I_WriteB((uint8)(ui_E2Address>>8)))       // 写入地址高字节
            {
                continue;          
            }
            if(BE_I_WriteB((uint8)ui_E2Address))            // 写入地址低字节
            {
                continue;               
            }
        
            BE_I_Start();                                   // restart
            if(BE_I_WriteB(uc_QjAddress+1))                 // 写入器件地址，读数据
            {
                continue;                    
            }
        
        #elif(_SW_BE_Type==Const_AT24C256)                  //
            ui_E2Address = (uint16)address;                 // 取具体的一块中的地址
            ui_E2Address &= 0x7fff;                         //
            //uc_QjAddress =(uint8)(address>>15);             // 24C256，考虑多个E2连在一起的情况
            //uc_QjAddress <<=1;                              // 取块号
            //uc_QjAddress += BE_Const_ID;                    // 确定器件地址
            uc_QjAddress =BE_Const_ID;
            BE_I_Start();                                   // start

            if(BE_I_WriteB(uc_QjAddress))                   // 写入器件地址，同时把块号也写入
            {
                continue;                      
            }
            if(BE_I_WriteB((uint8)(ui_E2Address>>8)))       // 写入地址高字节
            {
                continue;          
            }

            if(BE_I_WriteB((uint8)ui_E2Address))            // 写入地址低字节
            {
                continue;               
            }
        
            BE_I_Start();                                   // restart
            if(BE_I_WriteB(uc_QjAddress+1))                 // 写入器件地址，读数据
            {
                continue;                    
            }
        
        #elif(_SW_BE_Type==Const_AT24C512)                  //
            ui_E2Address = (uint16)address;                 // 取具体的一块中的地址
        
            //uc_QjAddress =(uint8)(address>>16);             // 24C512，考虑多个E2连在一起的情况
            //uc_QjAddress <<=1;                              // 取块号
            //uc_QjAddress += BE_Const_ID;                    // 确定器件地址
            uc_QjAddress =BE_Const_ID;
            BE_I_Start();                                   // start
            if(BE_I_WriteB(uc_QjAddress))                   // 写入器件地址，同时把块号也写入
            {
                continue;   
            }

            if(BE_I_WriteB((uint8)(ui_E2Address>>8)))       // 写入地址高字节
            {
                continue;   
            }

            if(BE_I_WriteB((uint8)ui_E2Address))            // 写入地址低字节
            {
                continue;               
            }
        
            BE_I_Start();                                   // restart
            if(BE_I_WriteB(uc_QjAddress+1))                 // 写入器件地址，读数据
            {
                continue;                                   
            }
        #else
            return 1;                                       // 返回 
        #endif                                              //

            break;                                          //没有错误退出循环
    }

    //for (i=0;i<Len;i++,ui_E2Address++)                      
    for (i=0;i<Len;i++)                      
    {
         *buff = BE_I_ReadB();                              //读取数据

         if (i==(Len-1)                                     //最后一个字节，或者地址大于E2地址时
//#if(_SW_BE_Type==Const_AT24C256)
//            || (ui_E2Address >= 0x7FFF)
//#elif(_SW_BE_Type==Const_AT24C512)
//            || (ui_E2Address >= 0xFFFF)
//#endif
           )                                         
        {
            BE_I_NAK();                             // 返回NAK
        }                                           
        else                                        
        {
            BE_I_ACK();                             // 返回ACK
        }                                           

        buff++;                                     
//
//#if(_SW_BE_Type==Const_AT24C256)
//        if (ui_E2Address >= 0x7FFF)
//        {
//            BE_I_Stop();                            // 片切换
//            Stop_Delay();
//            ui_E2Address = 0xFFFF;                  // 下个循环+1后归零
//
//            uc_QjAddress += 2;
//            BE_I_Start();                           // start
//            BE_I_WriteB(uc_QjAddress);              // 写入器件地址，同时把块号也写入
//            BE_I_WriteB(0);                         // 写入地址高字节
//            BE_I_WriteB(0);                         // 写入地址低字节
//
//            BE_I_Start();                           // restart
//            BE_I_WriteB(uc_QjAddress+1);            // 写入器件地址，读数据
//        }
//#elif(_SW_BE_Type==Const_AT24C512)                  //
//        if (ui_E2Address >= 0xFFFF)
//        {
//            BE_I_Stop();                            // 片切换
//            Stop_Delay();
//            ui_E2Address = 0xFFFF;                  // 下个循环+1后归零
//
//            uc_QjAddress += 2;
//            BE_I_Start();                           // start
//            BE_I_WriteB(uc_QjAddress);              // 写入器件地址，同时把块号也写入
//            BE_I_WriteB(0);                         // 写入地址高字节
//            BE_I_WriteB(0);                         // 写入地址低字节
//
//            BE_I_Start();                           // restart
//            BE_I_WriteB(uc_QjAddress+1);            // 写入器件地址，读数据
//        }
//#endif

    }                                               //
    BE_I_Stop();                                    //
    Stop_Delay(); 
    BE_WP_1();                                      //E2 WP=0
    if(uc_ErrCnt>=3)
    {
        gut_SysWarn.SysWarn.EEPROMERROR=true;   //E2错误
        if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt == false      //状态标志未置
           && gs_ReportWord.ui_ReportWord1.ReportWord1.EepErrRpt == 1)           //需要主动上报
        {
            gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = true;         //状态置位
        }
        return 2;                               // 写入失败
    }else
    {
        gut_SysWarn.SysWarn.EEPROMERROR=false;   //E2正确
        if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt == true)     //判断状态标志
        {
            gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = false;
        }
    }
    return 0;

}

/*=========================================================================================\n
* @function_name: BE_ReadB
* @function_file: BEBottom.C
* @描述: 从指定的地址中读一个字节的数据
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
uint8 BE_ReadB(uint32 address,uint8 *buff)
{
    BE_ReadP(address,buff,1);
    return 0;
}

/*=========================================================================================\n
* @function_name: BE_ReadW
* @function_file: BEBottom.C
* @描述: 
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
uint8 BE_ReadW(uint32 address,uint16 *buff)
{
    BE_ReadP(address,(uint8*)buff,2);
    return 0;
}
/*=========================================================================================\n
* @function_name: BE_ReadL
* @function_file: BEBottom.C
* @描述: 
* 
* 
* @参数: 
* @param:address  
* @param:buff  
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
//uint8 BE_ReadL(uint32 address,uint32 *buff)
//{
//    BE_ReadP(address,(uint8*)buff,4);
//    return 0;
//}

uint8 uc_BeRdData;      // 为保证以下运行速度不受影响，定义一个全局变量在读数据的时候使用
/*=========================================================================================\n
* @function_name: BE_I_ReadB
* @function_file: BEBottom.C
* @描述: 
* 
* @参数: 
* 
* @返回: 
* @return: uint8 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
uint8 BE_I_ReadB(void)
{
    uint8 i;                            //
    BE_I2C_CLK_0();                     // sclk=0 
    BE_I2C_SDA_IN();                    //
    uc_BeRdData =0;                     //
    for (i=0;i<8;i++)                    //
    {
        BE_I2C_CLK_0();                 // sclk=0
        Delay_Unit5us();
        //Delay_Unit5us(BE_Const_DyClk);// Clock low time
        BE_I2C_CLK_1();                 // sclk=1

        uc_BeRdData <<= 1;              // Clock high time
        if (BE_I2C_SDA_ST)               // 接收到1
        {
            uc_BeRdData |= 0x01;        // 数据末位置1 
        }                               //
    }                                   //
    BE_I2C_CLK_0();                     // sclk=0
    return uc_BeRdData;                //
}

uint8 uc_BeWrData;                      // 为保证以下运行速度不受影响，定义一个全局变量在读数据的时候使用
/*=========================================================================================\n
* @function_name: BE_I_WriteB
* @function_file: BEBottom.C
* @描述: 
* 
* 
* @参数: 
* @param:data1  
* 
* @返回: 
* @return: uint8 1：表示从设备没有响应，返回0：表示从设备响应正常
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
uint8 BE_I_WriteB(uint8 data1)
{
    uint8 i;
    uint8 errFg;

    uc_BeWrData = data1;                //
    BE_I2C_CLK_0();                     // SCK=0
    BE_I2C_SDA_0();                     // SDA=0 

    for (i=0; i<8; i++)                  // send One byte
    {
        BE_I2C_CLK_0();                 // SCK=0
        if (uc_BeWrData & 0x80)          //
        {
            BE_I2C_SDA_1();             // DA=1 
        }                               //
        else                            //
        {
            BE_I2C_SDA_0();             // DA=0 
        }                               //
        uc_BeWrData <<= 1;
        Delay_Unit5us();
        BE_I2C_CLK_1();                 // SCK=1
        //Delay_Unit5us(BE_Const_DyClk);// 延时 
        Delay_Unit5us();
    }

    //receive ack
    BE_I2C_CLK_0();                     // SCK=0 

    Delay_Unit5us();
    //Delay_Unit5us(BE_Const_DyClk);    // 延时
    BE_I2C_SDA_IN();                    // DA<-输入
    BE_I2C_CLK_1();                     // SCK=1

    Delay_Unit5us();
    //Delay_Unit5us(BE_Const_DyClk);    // 延时

    if (BE_I2C_SDA_ST)                  //读取ACK
    {
        errFg = 1;                      // 返回是1，为NAK
    }                                   //
    else                                //
    {
        errFg = 0;                      // 返回0为ACK
    }                                   //
    BE_I2C_CLK_0();                     // SCK=0
    return errFg;
}

/*=========================================================================================\n
* @function_name: BE_I_Start
* @function_file: BEBottom.C
* @描述: 
* 
* @参数: 
* @返回: 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
void BE_I_Start(void)
{   
    BE_I2C_CLK_0();                 // 在CLK=0的时候，改变数据线的状态
    BE_I2C_SDA_1();                 // SDA=1
    BE_I2C_CLK_1();                 // CLK=1
    //Delay_Unit5us(BE_Const_DyClk); // start setup 建立时间，必须大于5us
    Delay_Unit5us();
    BE_I2C_SDA_0();                 // SDA=0
    Delay_Unit5us();
    //Delay_Unit5us(BE_Const_DyClk); // start hold 保持时间，必须大于5us
    BE_I2C_CLK_0();                 // CLK=0
    Delay_Unit5us();
}

/*=========================================================================================\n
* @function_name: BE_I_Stop
* @function_file: BEBottom.C
* @描述: 
* 
* @参数: 
* @返回: 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
void BE_I_Stop(void)
{
    BE_I2C_CLK_0();                 // 在CLK=0的时候，改变数据线的状态
    BE_I2C_SDA_0();                 // SDA=0
    Delay_Unit5us();
    BE_I2C_CLK_1();                 // CLK=1
    Delay_Unit5us();
//  Delay_Unit5us(BE_Const_DyClk); // stop setup 建立时间，必须大于4us 
    BE_I2C_SDA_1();                 // SDA=1
    Delay_Unit5us();
    //Delay_Unit5us(BE_Const_DyClk); // stop hold 保持时间，必须大于4us 
    BE_I2C_SDA_IN();                // 为低功耗设计需要，将SDA设计成输入模式
    BE_I2C_CLK_0();                 // CLK=0 

}

/*=========================================================================================\n
* @function_name: BE_I_NAK
* @function_file: BEBottom.C
* @描述: 
* 
* @参数: 
* @返回: 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
void BE_I_NAK(void)
{
    BE_I2C_CLK_0();                 // SCK=0
    BE_I2C_SDA_1();                 // SDA=1
    BE_I2C_CLK_1();                 // SCK=1
    //Delay_Unit5us(BE_Const_DyClk); // 保持时间，必须大于4us 
    Delay_Unit5us();
    BE_I2C_CLK_0();                 // SCK=0
}
/*=========================================================================================\n
* @function_name: BE_I_ACK
* @function_file: BEBottom.C
* @描述: 
* 
* @参数: 
* @返回: 
* @作者: Lwb(2011-7-25)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: 
* @修改内容:  
===========================================================================================*/
void BE_I_ACK(void)
{
    BE_I2C_CLK_0();                 // SCK=0
    BE_I2C_SDA_0();                 // SDA=0
    BE_I2C_CLK_1();                 // SCK=1
    Delay_Unit5us();
    //Delay_Unit5us(BE_Const_DyClk); // 必须大于4us 
    BE_I2C_CLK_0();                 // SCK=0
}


