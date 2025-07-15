#define LCDMIDDLEEXT
#include "Include.h"

/*=========================================================================================\n
* @function_name: Disp_Numb
* @function_file: Lcdmiddle.c
* @描述:
*
*
* @参数:
* @param:data1  需要显示的数据，十六进制传送
* @param:type  显示方式，具体参照显示驱动接口
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Disp_Numb(uint32 data1, uint32 type)
{
    uint8 tmpdata;
    uint8 tmptype;
    uint8 tmpdisph,tmpdispl;
    uint8 *p;
    uint8 *q;

    for(uint8 i=0; i<4; i++)
    {
        tmpdata = (uint8)(data1 & 0x0F);        //取最后一位
        tmptype = (uint8)(type & 0x0F);
        data1 >>= 4;                            //数据处理
        type >>= 4;

        tmpdisph =NUMTABLE_H1[tmpdata];          //获取高位显示编码
        tmpdispl = NUMTABLE_L1[tmpdata];         //获取低位显示编码

        p = guc_LCDRAM + NumAddr[i*2];          //数字显示缓存
        q = guc_LCDRAM + NumAddr[i*2+1];        //数字显示缓存
        *p &= N_Resdh1;                          //保留不是数字位
        *q &= N_Resdl1;                          //保留不是数字位


        switch(tmptype)                         //根据参数二散转
        {
            case 0:
                *p |= tmpdisph<<4;         //显示数据
                *q |= tmpdispl;         //显示数据
                break;
            case 1:
                *p |= LCD_CHARH1_G;
                *q |= LCD_CHARL1_G;      //显示"-"号
                break;
            case 2:                     //显示'L'
                *p |= LCD_CHARH1_L;
                *q |= LCD_CHARL1_L;
                break;
            case 3:                     //显示'r'
                *p |= LCD_CHARH1_r;
                *q |= LCD_CHARL1_r;
                break;
            case 4:                     //显示'n'
                *p |= LCD_CHARH1_n<<4;
                *q |= LCD_CHARL1_n;
                break;
            case 5:                     //显示字母'o'
                *p |= LCD_CHARH1_o<<4;
                *q |= LCD_CHARL1_o;
                break;
            case 6: //显示字母‘E’
                *p |= LCD_CHARH1_E<<4;
                *q |= LCD_CHARL1_E;
                break;
            case 7: //显示字母‘C’
                *p |= LCD_CHARH1_C<<4;
                *q |= LCD_CHARL1_C;
                break;
            case 0x0a:
                *p |= 0x00<<4;             //消隐
                *q |= 0x00;
                break;
            default:
                break;
        }
    }

    for(uint8 i=4; i<8; i++)
    {
        tmpdata = (uint8)(data1 & 0x0F);        //取最后一位
        tmptype = (uint8)(type & 0x0F);
        data1 >>= 4;                            //数据处理
        type >>= 4;

        tmpdisph =NUMTABLE_H1[tmpdata];          //获取高位显示编码
        tmpdispl = NUMTABLE_L1[tmpdata];         //获取低位显示编码

        p = guc_LCDRAM + NumAddr[i*2];          //数字显示缓存
        q = guc_LCDRAM + NumAddr[i*2+1];        //数字显示缓存
        *p &= N_Resdh1;                          //保留不是数字位
        *q &= N_Resdl1;                          //保留不是数字位

        switch(tmptype)                         //根据参数二散转
        {
            case 0:
                *p |= tmpdisph<<4;         //显示数据
                *q |= tmpdispl;         //显示数据
                break;
            case 1:
                *p |= LCD_CHARH1_G;
                *q |= LCD_CHARL1_G;      //显示"-"号
                break;
            case 2:                     //显示'L'
                *p |= LCD_CHARH1_L;
                *q |= LCD_CHARL1_L;
                break;
            case 3:                     //显示'r'
                *p |= LCD_CHARH1_r;
                *q |= LCD_CHARL1_r;
                break;
            case 4:                     //显示'n'
                *p |= LCD_CHARH1_n<<4;
                *q |= LCD_CHARL1_n;
                break;
            case 5:                     //显示字母'o'
                *p |= LCD_CHARH1_o<<4;
                *q |= LCD_CHARL1_o;
                break;
            case 6: //显示字母‘E’
                *p |= LCD_CHARH1_E<<4;
                *q |= LCD_CHARL1_E;
                break;
            case 7: //显示字母‘C’
                *p |= LCD_CHARH1_C<<4;
                *q |= LCD_CHARL1_C;
                break;
            case 0x0a:
                *p |= 0x00<<4;             //消隐
                *q |= 0x00;
                break;
            default:
                break;
        }
    }
}
/*=========================================================================================\n
* @function_name: Disp_String
* @function_file: Lcdmiddle.c
* @描述:
*
*
* @参数:
* @param:para1
* @param:para2
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Disp_String(uint16 para)
{
    uint16 tmp;
    uint32 data1;
    uint8 i;
    uint8* p;

    tmp = para&0x00ff;                             // 取参数的低8位
    if(tmp>=Len_LCDChinaCode)
    {
        tmp=0;
    }
    data1 = LCD_ChinaCode[tmp];                     // 由此查表取出需要显示的数据
    for(i=0; i<Len_ChineseTable; i++)
    {//ChineseTable的使用是，每一个表值的对应一个位的位置定义，高5位是在显示缓存的地址偏移量，
     //低三位表示在对应地址字节的第几位
        p = ( guc_LCDRAM + ((ChineseTable[i])>>3) );// 找到显存地址
        if(data1 & 0x0001)                          //
        {
            *p |= BIT8MAP[ChineseTable[i] & 0x07];      //
        }                                           //
        else                                        //
        {
            *p &= ~BIT8MAP[ChineseTable[i] & 0x07];     //
        }
        data1 >>= 1;                                    //
    }

    tmp = (para&0xff00)>>8;                             // 取参数的低8位
    //汉字区数字8显示
   /* data1 = Strnum[tmp];
    for(i=0;i<Len_Strnum;i++)
    {
        p = ( guc_LCDRAM + ((BITStrnum[i])>>3) );           // 找到显存地址
        if(data1 & 0x0001)
        {
            *p |= BIT8MAP[BITStrnum[i] & 0x07];             //
        }else
        {
            *p &= ~BIT8MAP[BITStrnum[i] & 0x07];            //
        }
        data1 >>= 1;
    }*/

}

/*=========================================================================================\n
* @function_name: Disp_UnitPoint
* @function_file: Lcdmiddle.c
* @描述:
*
*
* @参数:
* @param:para
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Disp_UnitPoint(uint16 para)                        //
{
    uint16 tmp;
    uint16 data1;
    uint8 i;
    uint8* p;

    // 单位符号的显示
    tmp = para & 0x000F;                                //低半字节用于符号显示定义
    if(tmp>=UnitTableLen)
    {
        tmp=0;
    }
    data1 = UnitTable[tmp];                             // 由此查表取出需要显示的数据
    for(i=0; i<Len_UnitBit; i++)
    {
        p = ( guc_LCDRAM + ((UnitBitTable[i])>>3) );    // 找到显存地址
        if(data1 & 0x0001)
        {
            *p |= BIT8MAP[UnitBitTable[i] & 0x07];      //
        }else
        {
            *p &= ~BIT8MAP[UnitBitTable[i] & 0x07];     //
        }
        data1 >>= 1;
    }

    // 小数点的显示
    data1 = para>>4;            //低半字节用于符号显示定义
    for(i=0; i<PointLen; i++)
    {
        p = ( guc_LCDRAM + ((PointTable[i])>>3) );          // 找到显存地址
        if(data1 & 0x0001)
        {
            *p |= BIT8MAP[PointTable[i] & 0x07];//
        }else
        {
            *p &= ~BIT8MAP[PointTable[i] & 0x07];//
        }
        data1 >>= 1;
    }

}

/*=========================================================================================\n
* @function_name: Disp_State
* @function_file: Lcdmiddle.c
* @描述:
*
*
* @参数:
* @param:data1
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Disp_State(uint32 data1)
{
    uint8 i,*p;

    for(i=0; i<St_Len; i++)
    {
        p = ( guc_LCDRAM + ((St_Table[i])>>3) );       // 找到显存地址
        if(data1 & 0x0001)
        {
            *p |= BIT8MAP[St_Table[i] & 0x07];
        }
        else
        {
            *p &= ~BIT8MAP[St_Table[i] & 0x07];
        }
        data1 >>= 1;
    }
}






