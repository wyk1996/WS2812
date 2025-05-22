
#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include "IndependentDataType.h"

INT16U app_common_get_avg_abandon_some_extr(INT16U *pData, INT16U Len, INT16U AbdMaxNo, INT16U AbdMinNo);
INT16U app_common_text_in_str(INT8U *text, INT16U iLen, INT8U *str, INT8U sLen);

#endif

