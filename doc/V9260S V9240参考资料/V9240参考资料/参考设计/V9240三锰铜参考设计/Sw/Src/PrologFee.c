#define AP_PROLOGFEE_EXT
#include "Include.h"

const uint8 code RSBJLID[]= {0x00,
                    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                    0x00,
                    0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
/*=========================================================================================\n
* @function_name: RSDJLlog
* @function_file: PrologFee.c
* @描述: 记录时段或者时区更改记录
* 
* 
* @参数: 
* @param:usecode  操作者代码
* @param:DataID  操作id
* @返回: 
* @作者:   lwb (2011-12-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  xuqf (2012-12-20)
* @修改内容: 
===========================================================================================*/
#ifdef SQSDPRLOG
void RSDJLlog(uint32 usecode,uint8 DataID)
{
    uint8 RsdjlPt; 
    uint8 SqOffset;                                        //日时段表记录指针

    if(DataID>=18)                                          //做参数保护
    {
        return ;
    }

    if(DataID==0||DataID==9)                                //第1套时区更改 //第2套时区表更改
    {
        BE_ReadP(EEP_SQXGJL_PT,&RsdjlPt,1);                   //读取偏移指针

        if(RsdjlPt==0)
        {
            RsdjlPt=9;
        }else
        {
            RsdjlPt--;
        }
        RsdjlPt%=10;                                        //指针保护

        BE_WriteP(EEP_SQXGJL_PT,&RsdjlPt,1);                  //回写指针

        if(DataID==0)                                       //编号   
        {
            SqOffset=0x01;
        }else
        {
            SqOffset=0x02;
        }
        
        XDATA_RAMZONE[0]=SqOffset;                              //编号
        MemCpy(XDATA_RAMZONE+1,(uint8*)&gs_DateTime.ucSecond,6);//写入日期时间
        MemCpy(XDATA_RAMZONE+7,(uint8*)&usecode,4);             //操作代码


        //XRAMZ_ZONE1从偏移量11开始在更改前的时段费率调用函数前已经写入
        //日时段更改记录最近10次首地址
        BE_WriteP(EEP_SQXGJL+RsdjlPt*53,XDATA_RAMZONE,53);      //写入最近更改记录     

        DataProcEvCountAdd(SQNUMID);                            //时区表编程更改次数累加1次

    }else                                                       //时段表更改记录
    {
        DataProcEvCountAdd(RSDNUMID);                           //时段表编程更改次数累加1次
        BE_ReadP(EEP_RSDXGJL_PT,&RsdjlPt,1);                    //读取偏移指针

        if(RsdjlPt==0)
        {
            RsdjlPt=9;
        }else
        {
            RsdjlPt--;
        }
         
        RsdjlPt%=10;                                            //指针保护
        BE_WriteP(EEP_RSDXGJL_PT,&RsdjlPt,1);                   //回写指针
    
        XDATA_RAMZONE[0]=RSBJLID[DataID];                       //编号        
        MemCpy(XDATA_RAMZONE+1,(uint8*)&gs_DateTime.ucSecond,6);//写入日期时间
        MemCpy(XDATA_RAMZONE+7,(uint8*)&usecode,4);             //操作代码

   
        //更改前的时段费率调用函数前已经写入XRAMZ_ZONE1
        //日时段更改记录最近10次首地址
        BE_WriteP(EEP_RSDXGJL+RsdjlPt*53,XDATA_RAMZONE,53);    //写入最近更改记录     
    }
}
#endif
/*=========================================================================================\n
* @function_name: ProJL
* @function_file: PrologFee.c
* @描述: 
* 
* 
* @参数: 
* @param:usecode  操作者代码
* @param:Type  事件记录类型 
* @返回: 
* @作者:   lwb (2012-02-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  xuqf (2012-12-20)
* @修改内容: 
===========================================================================================*/
void ProJL(uint32 usecode,uint8 Type)
{
#ifdef ZXRPRLOG
    if(Type == FLG_ZXRJL)
    {
        DataProcEvCountAdd(ZXRNUMID);                        //周休日编程更改次数累加1次
        MemCpy(XDATA_RAMZONE,(uint8*)&gs_DateTime.ucSecond,6);//写入日期时间
        MemCpy(XDATA_RAMZONE+6,(uint8*)&usecode,4);           //操作代码
        BE_ReadP(EEP_ZXSD,XDATA_RAMZONE+10,1);  
        DataProcWriteRound2(R2_MOVE,ZXRJLID,XDATA_RAMZONE);
    }else 
#endif
#ifdef YGZHZPRLOG   
    if(Type == FLG_YGZHJL)
    {
        DataProcEvCountAdd(YGZHNUMID);                        //有功组合方式编程更改次数累加1次
        MemCpy(XDATA_RAMZONE,(uint8*)&gs_DateTime.ucSecond,6);//写入日期时间
        MemCpy(XDATA_RAMZONE+6,(uint8*)&usecode,4);           //操作代码
        BE_ReadP(EEP_YGCALC,XDATA_RAMZONE+10,1);  
        DataProcWriteRound2(R2_MOVE,YGZHJLID,XDATA_RAMZONE);
    }else 
#endif 
#ifdef JSRPRLOG     
    if(Type == FLG_JSRJL)
    {
        DataProcEvCountAdd(JSRNUMID);                        //结算日编程更改次数累加1次
        MemCpy(XDATA_RAMZONE,(uint8*)&gs_DateTime.ucSecond,6);//写入日期时间
        MemCpy(XDATA_RAMZONE+6,(uint8*)&usecode,4);           //操作代码
        BE_ReadP(EEP_CBR1,XDATA_RAMZONE+10,6);  
        DataProcWriteRound2(R2_MOVE,JSRJLID,XDATA_RAMZONE);
    }
#endif
    {
    }
}
/*=========================================================================================\n
* @function_name: ReadRSDLogItem
* @function_file: PrologFee.c
* @描述: 分帧回复日时段表更改记录
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2011-12-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  xuqf (2012-12-20)
* @修改内容: 
===========================================================================================*/
#ifdef SQSDPRLOG
uint8 ReadRSDLogItem(S_FRAMEINFO *s_FrmInfo, S_CORETAB code* CTable)
{
    uint8 FrameId;
    uint8 RsdjlPt;
    uint8 i,j,k;
    uint8 cnt;
    uint8 CurPos;
    Word32 RsdXgCnt;
    uint8 ucLoop;
    RsdXgCnt.lword=0;
    BE_ReadP(EEP_RSDXGNO,RsdXgCnt.byte,3);                      //读取时段表编程次数
    if(s_FrmInfo->ucCmd==0x11)                                  //首帧
    {
        if(RsdXgCnt.lword<s_FrmInfo->uiID.byte[0])
        {
            //MemSet(XDATA_RAMZONE,0,64);
            //MemCpy(s_FrmInfo->ucbuf, XDATA_RAMZONE, 52);
            MemSet(s_FrmInfo->ucbuf,0,178);
            s_FrmInfo->ucCmd|=0x20;
            s_FrmInfo->ucLen = CTable->ucLen;
            return Ret_OK;
        }

        BE_ReadP(EEP_RSDXGJL_PT,&RsdjlPt,1);                    //读取存储偏移指针
        CurPos=(RsdjlPt+s_FrmInfo->uiID.byte[0])-1;
        CurPos%=10;
        RsdJLIndex.cnt=s_FrmInfo->uiID.byte[0];                 //获取
        for(i=0;i<RsdJLIndex.cnt;i++)
        {
            //读取记录指针和存储地址
            BE_ReadP(EEP_RSDXGJL+RsdjlPt*53,&RsdJLIndex.POS[i],1);    //
            RsdJLIndex.E2Addr[i].word=EEP_RSDXGJL+RsdjlPt*53;         //地址
            RsdjlPt++;
            RsdjlPt%=10;
        }

        cnt=0;
        for(i=0;i<RsdJLIndex.cnt;i++)
        {
            //保证下一个处理是有效的
            if(RsdJLIndex.POS[i]==0)
            {
                for(k=i;k<RsdJLIndex.cnt;k++)
                {
                    if(RsdJLIndex.POS[k]!=0)
                    {
                        RsdJLIndex.POS[i]=RsdJLIndex.POS[k];
                        RsdJLIndex.E2Addr[i].word=RsdJLIndex.E2Addr[k].word;
                        RsdJLIndex.POS[k]=0;
                        RsdJLIndex.E2Addr[k].word=0;
                    }
                }
            }
            //排除重复的项
            for(j=i+1;j<RsdJLIndex.cnt;j++)
            {
                if(RsdJLIndex.POS[j]==0)
                {
                    continue;
                }

                if(RsdJLIndex.POS[i]==RsdJLIndex.POS[j])    //如果更改同一个时段多次的话，返回最新最靠近这次的一次
                {
                    RsdJLIndex.POS[i]=RsdJLIndex.POS[j];
                    RsdJLIndex.E2Addr[i].word=RsdJLIndex.E2Addr[j].word;
                    RsdJLIndex.POS[j]=0;
                    RsdJLIndex.E2Addr[j].word=0;
                    cnt++;
                }
            }
        }

        RsdJLIndex.cnt-=cnt;

        BE_ReadP(EEP_RSDXGJL+CurPos*53+1,s_FrmInfo->ucbuf,10);  //时间和操作者代码
        for(ucLoop=0;ucLoop<4;ucLoop++)
        {
            j=0xff;
            for(i=0;i<RsdJLIndex.cnt;i++)
            {
                if(RsdJLIndex.POS[i]==(ucLoop+1))
                {
                    j=i;                                            //需要回复最新的
                    break;
                }
            }
            //BE_ReadP(EEP_RSDXGJL+CurPos*53+1,XDATA_RAMZONE1,10);      //时间和操作者代码
            //MemCpy(s_FrmInfo->ucbuf, XDATA_RAMZONE1, 10);
            if(j!=0xff)
            {
                BE_ReadP(RsdJLIndex.E2Addr[j].word+11,s_FrmInfo->ucbuf+10+ucLoop*42,42);
            }else
            {
                BE_ReadP(EEP_1RSDS+ucLoop*E2_PROTECT_LEN,s_FrmInfo->ucbuf+10+ucLoop*42,42);
            }
        }

//      MemCpy(s_FrmInfo->ucbuf+10, XDATA_RAMZONE, 42);
        s_FrmInfo->ucCmd|=0x20;                                     //与数据建构 0x80配合 有后续帧
        s_FrmInfo->ucLen = CTable->ucLen;
        return Ret_OK;

    }else
    {
        FrameId=s_FrmInfo->ucbuf[0];
        if(FrameId == 4 )   //主动上报状态字
        {
            if(gs_MeterMode.ReportMode == 0)
            {
                s_FrmInfo->ucErr.word=ComErrBit_Other;
                s_FrmInfo->ucLen = 1;  
            }
            else
            {
                s_FrmInfo->uiID.lword = 0x04001501;
                MemCpy(s_FrmInfo->ucbuf, (uint8*)&gs_ReportState, sizeof(REPORTSTATE));       //
                
                if(gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = true;
                }
                if(gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = true;
                }
                if(gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = true;
                }
                if(gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt)
                {
                    gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = false;
                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = true;            
                }
                if(gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt)
                {
                    gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = false;
                    gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = true;
                }
                if(gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt)
                {
                    gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = false;
                    gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = true;
                }  
                s_FrmInfo->ucbuf[10]=FrameId;             
                s_FrmInfo->ucLen = 11;           
            }
            return Ret_OK;
        }
        else if(RsdXgCnt.lword<s_FrmInfo->uiID.byte[0])              //如果没有记录返回为0
        {
//          MemSet(XDATA_RAMZONE,0,64);
//          XDATA_RAMZONE[42]=FrameId;
//          MemCpy(s_FrmInfo->ucbuf, XDATA_RAMZONE, 43);
            MemSet(s_FrmInfo->ucbuf,0,168);
            s_FrmInfo->ucbuf[168]=FrameId;  //帧序号
            
            if((FrameId==3))          //帧序号
            {
                if(gs_MeterMode.ReportMode == 1)
                {
                    //最后一帧cmd=0x92;
                    if((gs_ReportState.ui_ReportState1.Word.word&0x528) != 0 
                       || (gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt != 0)  
                       || (gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt != 0))//静止式多费率判断
                    {
                        s_FrmInfo->ucCmd|=0x20;
                    }
                }
            }else
            {
                s_FrmInfo->ucCmd|=0x20;
            }

            s_FrmInfo->ucLen = CTable->ucLen-9;
            return Ret_OK;
        }
        
        //有数据记录则回复数据
        for(ucLoop=0;ucLoop<4;ucLoop++)
        {
            j=0xff;
            for(i=0;i<RsdJLIndex.cnt;i++)
            {
                if(RsdJLIndex.POS[i]==(FrameId*4+ucLoop+1))                    
                {
                    j=i;                                            //需要回复最新的
                    break;
                }
            }
    
            if(j!=0xff)
            {
                BE_ReadP(RsdJLIndex.E2Addr[j].word+11,s_FrmInfo->ucbuf+ucLoop*42,42);
            }else
            {
                if(FrameId==1)
                {
                    BE_ReadP(EEP_1RSDS+(ucLoop+4)*E2_PROTECT_LEN,s_FrmInfo->ucbuf+ucLoop*42,42);
                }else
                {
                    BE_ReadP(EEP_1RSDS+(FrameId*4+ucLoop+1)*E2_PROTECT_LEN,s_FrmInfo->ucbuf+ucLoop*42,42);
                }
            }
        }

        s_FrmInfo->ucbuf[168]=FrameId;

//      MemCpy(s_FrmInfo->ucbuf, XDATA_RAMZONE, 43);
        if(FrameId==3)          //帧序号
        {
            //最后一帧*cmd=0x92;
            if(gs_MeterMode.ReportMode == 1)
            {
                if(gs_ReportState.ui_ReportState1.Word.word != 0 
                   || gs_ReportState.ui_ReportState3.Word.word != 0)  //静止式多费率判断)
                {
                    s_FrmInfo->ucCmd|=0x20;
                }
            }
            
        }else
        {
            //*cmd=0xb2;
          s_FrmInfo->ucCmd|=0x20;
        }
        s_FrmInfo->ucLen = CTable->ucLen-9;
        return Ret_OK;
    }
}
#endif
/*=========================================================================================\n
* @function_name: ReadSQLogItem
* @function_file: PrologFee.c
* @描述: 
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2011-12-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  xuqf (2012-12-20)
* @修改内容: 
===========================================================================================*/
#ifdef SQSDPRLOG
uint8 ReadSQLogItem(S_FRAMEINFO *s_FrmInfo, S_CORETAB code* CTable)
{
    uint8 SQlPt;
    uint8 i,j,k;
    uint8 cnt;
    uint8 CurPos;
    Word32 SQXgCnt;

    if(s_FrmInfo->ucCmd == 0x12)        //指令写
    {
        if(gs_MeterMode.ReportMode == 0)
        {
            s_FrmInfo->ucErr.word=ComErrBit_Other;
            s_FrmInfo->ucLen = 1;  
        }
        else
        {
            s_FrmInfo->uiID.lword = 0x04001501;
            MemCpy(s_FrmInfo->ucbuf, (uint8*)&gs_ReportState, sizeof(REPORTSTATE));       //
            
            if(gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt)
            {
                gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = false;
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = true;
            }
            if(gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt)
            {
                gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = false;
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = true;
            }
            if(gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt)
            {
                gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = false;
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = true;
            }
            if(gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt)
            {
                gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = false;
                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = true;            
            }
            if(gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt)
            {
                gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = false;
                gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = true;
            }
            if(gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt)
            {
                gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = false;
                gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = true;
            }  
            s_FrmInfo->ucbuf[10] = 0x01;             
            s_FrmInfo->ucLen = 11;           
        }
        return Ret_OK;
    }


    SQXgCnt.lword=0;
    BE_ReadP(EEP_SQXGNO,SQXgCnt.byte,3);           //读取时区修改次数
    if(SQXgCnt.lword<s_FrmInfo->uiID.byte[0])    //如果修改的次数小于读取最近的记录次数的话，返回全0
    {
        MemSet(s_FrmInfo->ucbuf,0,94);
//      MemSet(XDATA_RAMZONE,0,64);
//      MemSet(XDATA_RAMZONE1,0,64);
//      MemCpy(s_FrmInfo->ucbuf, XDATA_RAMZONE, 64);
//      MemCpy(s_FrmInfo->ucbuf+64, XDATA_RAMZONE1, 30);
        s_FrmInfo->ucLen = CTable->ucLen;
        return Ret_OK;
    }else
    {
        BE_ReadP(EEP_SQXGJL_PT,&SQlPt,1);          //读取存储偏移指针
        CurPos=(SQlPt+s_FrmInfo->uiID.byte[0])-1;
        CurPos%=10;
        SQJLIndex.cnt=s_FrmInfo->uiID.byte[0];   //获取

        for(i=0;i<SQJLIndex.cnt;i++)                //首先获取要读取的最近N次记录信息和地址
        {
            //读取记录指针和存储地址
            BE_ReadP(EEP_SQXGJL+SQlPt*53,&SQJLIndex.POS[i],1);
            SQJLIndex.E2Addr[i].word=EEP_SQXGJL+SQlPt*53;
            SQlPt++;
            SQlPt%=10;
        }

        cnt=0;
        for(i=0;i<SQJLIndex.cnt;i++)                //去除对同一更改项
        {
            //保证下一个处理是有效的
            if(SQJLIndex.POS[i]==0)
            {
                for(k=i;k<SQJLIndex.cnt;k++)
                {
                    if(SQJLIndex.POS[k]!=0)
                    {
                        SQJLIndex.POS[i]=SQJLIndex.POS[k];
                        SQJLIndex.E2Addr[i].word=SQJLIndex.E2Addr[k].word;
                        SQJLIndex.POS[k]=0;
                        SQJLIndex.E2Addr[k].word=0;
                    }
                }
            }
            //排除重复的项
            for(j=i+1;j<SQJLIndex.cnt;j++)
            {
                if(SQJLIndex.POS[j]==0)
                {
                    continue;
                }

                if(SQJLIndex.POS[i]==SQJLIndex.POS[j])    //如果更改同一个时段多次的话，返回最新最靠近这次的一次
                {
                    SQJLIndex.POS[i]=SQJLIndex.POS[j];
                    SQJLIndex.E2Addr[i].word=SQJLIndex.E2Addr[j].word;
                    SQJLIndex.POS[j]=0;
                    SQJLIndex.E2Addr[j].word=0;
                    cnt++;
                }
            }
        }

        SQJLIndex.cnt-=cnt;

        BE_ReadP(EEP_SQXGJL+CurPos*53+1,s_FrmInfo->ucbuf,10);        //时间和操作者代码
//      BE_ReadP(EEP_SQXGJL+CurPos*53+1,XDATA_RAMZONE1,10);        //时间和操作者代码
//      MemCpy(s_FrmInfo->ucbuf, XDATA_RAMZONE1, 10);
        j=0xff;
        for(i=0;i<SQJLIndex.cnt;i++)
        {
            if(SQJLIndex.POS[i]==0x01)
            {
                j=i;                                            //需要回复最新的
                break;
            }
        }


        if(j!=0xff)
        {
            BE_ReadP(SQJLIndex.E2Addr[j].word+11,s_FrmInfo->ucbuf+10,42);
//          BE_ReadP(SQJLIndex.E2Addr[j].word+11,XDATA_RAMZONE,42);
        }else
        {
            BE_ReadP(EEP_1SQ,s_FrmInfo->ucbuf+10,42);
//          BE_ReadP(EEP_1SQ,XDATA_RAMZONE,42);
        }

        j=0xff;
        for(i=0;i<SQJLIndex.cnt;i++)
        {
            if(SQJLIndex.POS[i]==0x02)
            {
                j=i;                                                //需要回复最新的
                break;
            }
        }

        if(j!=0xff)                                                 //判断第2套时区是否从最近更改记录里面读取
        {
            BE_ReadP(SQJLIndex.E2Addr[j].word+11,s_FrmInfo->ucbuf+52,42);
//          BE_ReadP(SQJLIndex.E2Addr[j].word+11,XDATA_RAMZONE1,42);
        }else
        {
            BE_ReadP(EEP_2SQ,s_FrmInfo->ucbuf+52,42);
//          BE_ReadP(EEP_2SQ,XDATA_RAMZONE1,42);
        }

//      MemCpy(s_FrmInfo->ucbuf+10, XDATA_RAMZONE, 42);
//      MemCpy(s_FrmInfo->ucbuf+52, XDATA_RAMZONE1, 42);
        s_FrmInfo->ucLen = CTable->ucLen;

    }

    return Ret_OK;

}
#endif
/*=========================================================================================\n
* @function_name: SeekDataID
* @function_file: PrologFee.c
* @描述: 根据数据标识计算id号  0/9  第一/二套时区    1~8    第一套第1~8日时段
*                                                    10~17  第二套第1~8日时段
* 
* @参数: 
* @param:uiID  
* 
* @返回: 
* @return: uint8 
* @作者:   xuqf (2012-12-20)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 SeekDataID(Word32 uiID)
{
    uint8 ui_DataID;
    
    if(uiID.byte[2]==0x01)                                          //根据数据标识判断是哪个时区。0x01  第一套时区表  
    {
        ui_DataID = uiID.byte[2]+uiID.byte[0]-1;                    //计算id号,0~8
        if(ui_DataID==0)                                            //如果是第一套时区表的id，
        {
            BE_ReadP(EEP_1SQ,XDATA_RAMZONE+11,42);                  
        }
        else                                                            //第一套时区第一时段表
        {   
            BE_ReadP(EEP_1RSDS1+(ui_DataID-1)*E2_PROTECT_LEN,XDATA_RAMZONE+11,42);      
        }
    }
    else                                                           //0x02 第二套时区表
    {
        ui_DataID = uiID.byte[2]+uiID.byte[0]+7;                    //id  9~17
        if(ui_DataID==9)                                            //如果是第二套时区表的id，
        {
            BE_ReadP(EEP_2SQ,XDATA_RAMZONE+11,42);
        }
        else                                                        //第二套时区第一时段表
        {
            BE_ReadP(EEP_2RSDS1+(ui_DataID-10)*E2_PROTECT_LEN,XDATA_RAMZONE+11,42);
        }
    }
    return ui_DataID;
}
