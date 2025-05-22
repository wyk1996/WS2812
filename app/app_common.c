#include "app_common.h"
#include "Includes.h"

/**  @brief    去掉指定个数的最大最小值后，取数据平均值  
	@param    pData		数据指针  
	@param    Len			数据长度  
	@param    AbdMaxNo	需要去掉的最大值个数  
	@param    AbdMinNo	需要去掉的最小值个数  
	@return   INT16U		取平均后的结果  
	函数的实现里，会把传进来的数据buf里的数据重新排列，从小排到大 
*/
INT16U app_common_get_avg_abandon_some_extr(INT16U *pData, INT16U Len, INT16U AbdMaxNo, INT16U AbdMinNo)
{
	INT32U Result;
	INT16U i,j,Temp;

	if(Len == 0)
	{
		return 0;
	}
	else if(Len == 1)
	{
		return *pData;
	}
	else if(Len == 2)
	{
		Result = *pData++;
		Result += *pData;
		return Result/2;
	}
	if(AbdMaxNo > Len/2)
	{
		AbdMaxNo = Len/2;
	}
	if(AbdMinNo > Len/2)
	{
		AbdMinNo = Len/2;
	}
	if(AbdMaxNo + AbdMinNo >= Len)
	{
		AbdMaxNo -= 1;
	}
	// 冒泡法排序,就用应用层提供的buf
	for(i=0; i<Len; i++)
	{
		for(j=i; j<Len; j++)
		{
			if(*(pData+i) > *(pData+j))
			{
				Temp = *(pData+i);
				*(pData+i) = *(pData+j);
				*(pData+j) = Temp;
			}
		}
	}
	Result = 0;
	for(i=AbdMinNo; i<Len-AbdMaxNo; i++)
	{
		Result += *(pData+i);
	}
	return (INT16U)(Result/(Len-AbdMinNo-AbdMaxNo));
}

//获取text是否在字符串中
INT16U app_common_text_in_str(INT8U *text, INT16U iLen, INT8U *str, INT8U sLen)
{
	INT16U i=0, j=0;

    for(j=0; j<iLen; j++)
	{
		if((j+sLen) > iLen)
			return 0;

        for(i=0; i<sLen; i++)
		{
            if(text[j+i] != str[i])
            {
            	break;
            }
        }
        if(i>=sLen)
        {
			return (j+1);
        }
    }
    return 0;
}

