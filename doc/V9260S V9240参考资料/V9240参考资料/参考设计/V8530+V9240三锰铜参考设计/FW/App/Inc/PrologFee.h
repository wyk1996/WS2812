#ifndef _AP_PROLOGFEE_H_
#define _AP_PROLOGFEE_H_

#ifndef AP_PROLOGFEE_EXT
    #define AP_PROLOGFEE_EXT extern 
#endif

#define SQSDPRLOG           //时区时段表编程记录
//#define ZXRPRLOG            //周休日编程记录      农网无周休日
#define JSRPRLOG            //结算日编程记录
#define YGZHZPRLOG          //有功组合字编程记录


enum
{
    FLG_ZXRJL = 0,
    FLG_YGZHJL,
    FLG_JSRJL,                         
};


struct RsdXgJL
{
    uint8 cnt;
    uint8 POS[10];
    Word16 E2Addr[10];
};

AP_PROLOGFEE_EXT struct RsdXgJL xdata RsdJLIndex;
AP_PROLOGFEE_EXT struct RsdXgJL xdata SQJLIndex;


void RSDJLlog(uint32 usecode,uint8 DataID);
void ProJL(uint32 usecode,uint8 Type);
uint8 ReadRSDLogItem(S_FRAMEINFO *s_FrmInfo, S_CORETAB code* CTable);
uint8 ReadSQLogItem(S_FRAMEINFO *s_FrmInfo, S_CORETAB code* CTable);
uint8 SeekDataID(Word32 uiID);

#endif
