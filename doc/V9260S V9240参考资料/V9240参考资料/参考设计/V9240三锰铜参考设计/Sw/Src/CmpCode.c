#define CMPCODE_EXT
#include "Include.h"

/*=========================================================================================\n
* @function_name: ReadCodeData
* @function_file: CmpCode.c
* @描述: 
* 
* 
* @参数: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @返回: 
* @return: uint8 
* @作者:   lwb (2012-11-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 ReadCodeData(S_FRAMEINFO *s_FrmInfo, S_CORETAB code* CTable)
{
//    uint8 len;
//    //uint8 rlen;
//    Word32 addr;
//
//    if(s_FrmInfo->ucCmd == 0x11)                //读数据
//    {
//        addr.byte[3] = 0;
//        addr.byte[0] = s_FrmInfo->ucbuf[1];     //读写的地址
//        addr.byte[1] = s_FrmInfo->ucbuf[2];
//        addr.byte[2] = s_FrmInfo->ucbuf[3];
//
//        len = s_FrmInfo->ucbuf[4];              //读写的长度
//
//        /* 
//        if((addr.lword+len)>=0x18000)           //需要bank1区域读取代码
//        {
//            if(addr.lword>=0x18000)             //直接读取bank2
//            {
//                addr.lword&=0xffff;
//                addr.lword|=0x8000;
//                FCpyTMem2(s_FrmInfo->ucbuf,(const uint8 code *)addr.word[0],len);
//            }else
//            {
//                rlen=0x18000-addr.lword;
//                addr.lword&=0xffff;
//                addr.lword|=0x8000;
//                FCpyTMem1(s_FrmInfo->ucbuf,(const uint8 code *)addr.word[0],rlen);
//                FCpyTMem2(s_FrmInfo->ucbuf+rlen,(const uint8 code *)0x8000,len-rlen);
//            }
//        }
//        else if((addr.lword+len)>=0x10000)
//        {
//            if(addr.lword>=0x10000)             //直接读取bank1
//            {
//                addr.lword&=0xffff;
//                addr.lword|=0x8000;
//                FCpyTMem1(s_FrmInfo->ucbuf,(const uint8 code *)addr.word[0],len);
//            }else
//            {
//                rlen=0x10000-addr.lword;
//                FCpyTMem(s_FrmInfo->ucbuf,(const uint8 code *)addr.word[0],rlen);
//                FCpyTMem1(s_FrmInfo->ucbuf+rlen,(const uint8 code *)0x8000,len-rlen);
//            }
//        }else
//        {
//            FCpyTMem(s_FrmInfo->ucbuf,(const uint8 code *)addr.word[0],len);
//        } 
//        */ 
//        ReadFlash(addr.lword,s_FrmInfo->ucbuf,len);
//        s_FrmInfo->ucLen = len;
//    }
//    else                                        //否则就是写数据
//    {
//        s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//        s_FrmInfo->ucLen = 1;
//    }
    return Ret_OK;

}
/*=========================================================================================\n
* @function_name: ReadFlash
* @function_file: CmpCode.c
* @描述: 
* 
* 
* @参数: 
* @param:ulAddr  
* @param:buff  
* @param:len  
* @返回: 
* @作者:   lwb (2012-11-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void ReadFlash(uint32 ulAddr,uint8* buff,uint8 len)
{
    Word32 addr;
    uint8 rlen;
    addr.lword=ulAddr;
    
    if((addr.lword+len)>=0x20000)           //超出code区 补结束符+00
    {
        if(addr.lword<0x20000)
        {
            rlen=0x20000-addr.lword;
            addr.lword&=0xffff;
            addr.lword|=0x8000;
            FCpyTMem2(buff,(const uint8 code *)addr.word[0],rlen);
            MemSet(buff+rlen,0x00,len-rlen);
            MemSet(buff+rlen,0x80,1);
        }else
        {
            MemSet(buff,0x00,len);
        }
    }
    else if((addr.lword+len)>=0x18000)           //需要bank1区域读取代码
    {
        if(addr.lword>=0x18000)             //直接读取bank2
        {
            addr.lword&=0xffff;
            addr.lword|=0x8000;
            FCpyTMem2(buff,(const uint8 code *)addr.word[0],len);
        }else
        {
            rlen=0x18000-addr.lword;
            addr.lword&=0xffff;
            addr.lword|=0x8000;
            FCpyTMem1(buff,(const uint8 code *)addr.word[0],rlen);
            FCpyTMem2(buff+rlen,(const uint8 code *)0x8000,len-rlen);
        }
    }
    else if((addr.lword+len)>=0x10000)
    {
        if(addr.lword>=0x10000)             //直接读取bank1
        {
            addr.lword&=0xffff;
            addr.lword|=0x8000;
            FCpyTMem1(buff,(const uint8 code *)addr.word[0],len);
        }else
        {
            rlen=0x10000-addr.lword;
            FCpyTMem(buff,(const uint8 code *)addr.word[0],rlen);
            FCpyTMem1(buff+rlen,(const uint8 code *)0x8000,len-rlen);
        }
    }else
    {
        FCpyTMem(buff,(const uint8 code *)addr.word[0],len);
    }    

}

/*=========================================================================================\n
* @function_name: GetFactor
* @function_file: CmpCode.c
* @描述: 
* 
* 
* @参数: 
* @param:encryptbuff  
* @param:flashbuff   
* @返回: 
* @作者:   lwb (2012-01-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 GetFactor(uint8* encryptbuff, uint8* flashbuff)
{
    uint16 i;
    uint8 ucCount;
    
    ucCount = 0;    
//    if( ApiBufCmp(flashbuff,(flashbuff+8),8) == CMP_EQU 
//     && ApiBufCmp(flashbuff,(flashbuff+4),4) == CMP_EQU
//     && ApiBufCmp(flashbuff,(flashbuff+2),2) == CMP_EQU
//     && *flashbuff == *(flashbuff+1))                             //比对因子起始地址指向后续16字节为相同数据返回错
//    {
//        return Ret_Err;
//    }
    
    for(i=0;i<255;i++)
    {
        if(*(flashbuff+i)==*(flashbuff+i+1))
        {
            ucCount++;
        }
        else
        {
            ucCount = 0;
        }
        
        if(ucCount >= 15)
        {
            return Ret_Err;
        }
    }    
    
    for(i=0;i<64;i++)
    {
        *(encryptbuff+i) = *(flashbuff+i)^*(flashbuff+64+i)^*(flashbuff+128+i)^*(flashbuff+192+i);
    }


//    for(i=0;i<63;i++)
//    {
//        if(*(encryptbuff+i)==*(encryptbuff+i+1))
//        {
//            ucCount++;
//        }
//        else
//        {
//            ucCount = 0;
//        }
//        
//        if(ucCount >= 15)
//        {
//            return Ret_Err;
//        }
//    }
    
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: FlashEncrypt
* @function_file: CmpCode.c
* @描述: 
* 
* 
* @参数: 
* @param:ulAddr  
* @param:buff  
* @param:len  
* @返回: 
* @作者:   lwb (2012-11-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void CodeEncrypt(uint8* factorbuff, uint8* buff,uint8* Combuf)
{
    uint8 i;
    
    for(i=0;i<64;i++)
    {
        *(Combuf+63-i) = *(factorbuff+i)^*(buff+i)^*(buff+64+i)^*(buff+128+i)^*(buff+192+i);
    }
}
