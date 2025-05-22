#include "app_common.h"
#include "Includes.h"

/**  @brief    ȥ��ָ�������������Сֵ��ȡ����ƽ��ֵ  
	@param    pData		����ָ��  
	@param    Len			���ݳ���  
	@param    AbdMaxNo	��Ҫȥ�������ֵ����  
	@param    AbdMinNo	��Ҫȥ������Сֵ����  
	@return   INT16U		ȡƽ����Ľ��  
	������ʵ�����Ѵ�����������buf��������������У���С�ŵ��� 
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
	// ð�ݷ�����,����Ӧ�ò��ṩ��buf
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

//��ȡtext�Ƿ����ַ�����
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

