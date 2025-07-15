 
#ifndef _API_H_
#define _API_H_

#ifndef API_EXT
#define API_EXT extern
#endif

enum
{
    CMP_EQU = 0,    //相等
    CMP_LG,         //large than 大于
    CMP_LT          //little than 小于
};
 

enum
{//数据处理返回类型定义
    Const_Data_OK = 0,      //数据有效
    Const_Data_Err,         //数据无效
    Const_DataID_Err,       //找不到id
    Const_DataPswd_Err = 4, //密码或权限错误
    Const_DataNot_Ret = 20  //通讯不需要回应
};

 
void Uint16_To_Uint8(uint16 x, uint8 * buff);
uint16 Uint8_To_Uint16(uint8 * buff);
void Uint24_To_Uint8(uint32 x, uint8 * buff);
uint32 Uint8_To_Uint24(uint8 * buff);
void Uint32_To_Uint8(uint32 x, uint8 * buff);
void Int32_To_Uint8(int32 x, uint8 * buff);
uint32 Uint8_To_Uint32(uint8 * buff);
int32 Uint8_To_Int32(uint8 * buff);
uint32 Hex2BIN(uint8 x);
uint16 getPower(uint16 x, uint16 y);
uint32 Hex2BCD(uint32 hex);
uint32 BCD2Hex(uint32);
uint16 IntHex2BCD(uint16 hex);
uint16 do_CRC(uint8 xdata *str,uint8 len);
uint8 ApiBufCmp(uint8* pucCompare, uint8* pucCompared, uint8 ucLen);
uint8 ApiRevBufCmp(uint8* pucCompare, uint8* pucCompared, uint8 ucLen);
uint8 ApiCodeBufCmp(uint8* pucCompare, const uint8 code* pucCompared, uint8 ucLen);
void ApiConData(uint8* buff, uint8 Len);
uint8 JudgeIsNum(uint8* pucBuf, uint8 ucLen);
uint8 APIByteBCD2HEX(uint8 ucBCD);
uint8 APIByteHEX2BCD(uint8 ucHEX);
void SumplementNegtive(uint8 ucLen, uint8* p);
uint16 IntBCD2Hex(uint16 x);
void MoveBuffer(uint8* sbuff, uint8* tbuff, uint16 length);
uint8 CompareTime(uint8  *Tm1,uint8 *Tm2);
void MemInvertCpy(uint8 *des,uint8 *src,uint8 len);
void MemCpy(uint8 *des,uint8 *src,uint8 len);
void MemSet(uint8* des,uint8 value,uint8 len);
__banked_func void FCpyTMem(uint8x *des,const uint8 code *src,uint8 len);
__banked_func void FCpyTMem1(uint8x *des,const uint8 code *src,uint8 len);
__banked_func void FCpyTMem2(uint8x *des,const uint8 code *src,uint8 len);
void DelayXms(uint8 num);
void ApiReverseData(uint8* buff, uint8 Len);
uint8 CalCBRCount(uint8 xdata* str1,uint8 xdata* str2,uint8 xdata ucDay,uint8 xdata ucHour);
uint8 CalRDJCount(uint8 xdata* str1,uint8 xdata* str2,uint8 xdata ucHour,uint8 xdata ucMin);
uint16 HourAndMinToMin(uint8*Data);

#endif

