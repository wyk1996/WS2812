
#ifndef __LCDSHOW_H__
#define __LCDSHOW_H__

#ifndef LCDSHOWEXT
    #define LCDSHOWEXT extern
#else
    #define LCDSHOWSELF
#endif



#define Const_MaxDispTabNum (sizeof(gCs_DispTab)/sizeof(S_DISPCONT))


#define DEFAULT_CLOCK_NO    68          //默认的时间显示在显示表中的位置
#define DEFAULT_ENERGY_NO   1           //默认的当前电量在显示表中的位置

#define Const_Disp_Neg      0x00        //负数
#define Const_Disp_Pos      0x01        //正数
LCDSHOWEXT uint8 guc_ShowStCom;
LCDSHOWEXT uint8 guc_ShowEnergyFlag;

LCDSHOWEXT uint32   gul_ShowState;      // 显示系统状态
/*#define flgShowSt_Fuh       0x10000000  
#define flgShowSt_FZ        0x8000000   
#define flgShowSt_TJ        0x4000000  
#define flgShowSt_TZ        0x2000000   
#define flgShowSt_LZ        0x1000000   
#define flgShowSt_QGD       0x800000   
#define flgShowSt_SB        0x400000    
#define flgShowSt_CG        0x200000    
#define flgShowSt_Zhong     0x100000    
#define flgShowSt_DK        0x80000     
#define flgShowSt_SJ2       0x20000    
#define flgShowSt_SJ1       0x10000    
#define flgShowSt_Fk4       0x8000     
//#define flgShowSt_Fk3       0x4000      
//#define flgShowSt_Fk2       0x2000      
//#define flgShowSt_Fk1       0x1000     
//#define flgShowSt_Lock      0x0800 
#define flgShowSt_PRvs     0x4000      
#define flgShowSt_Bat       0x2000
#define flgShowSt_Pgm        0x1000 
#define flgShowSt_Com       0x0800  
//#define flgShowSt_PRvs      0x0400   
#define flgShowSt_Lock     0x0400   
//#define flgShowSt_Bat        0x0200     
//#define flgShowSt_Pgm       0x0100     
//#define flgShowSt_ZB         0x0080      
//#define flgShowSt_Com       0x0040     
#define flgShowSt_YQ2       0x0020      
#define flgShowSt_YQ1       0x0010      
#define flgShowSt_Fee4      0x0008      
#define flgShowSt_Fee3      0x0004      
#define flgShowSt_Fee2      0x0002      
#define flgShowSt_Fee1      0x0001
*/
#define flgShowSt_RPvs       0x8000  
#define flgShowSt_PRvs       0x4000  
#define flgShowSt_Bat          0x2000
#define flgShowSt_Pgm        0x01000 
#define flgShowSt_Com        0X0800
#define flgShowSt_Lock       0X0400
#define flgShowSt_Reserved   0X0200
#define flgShowSt_UC          0x0100      
#define flgShowSt_UB          0x0080  
#define flgShowSt_UA          0x0040    
#define flgShowSt_NIC        0x0020      
#define flgShowSt_NIB        0x0010  
#define flgShowSt_NIA        0x0008 
#define flgShowSt_IC           0x0004      
#define flgShowSt_IB           0x0002  
#define flgShowSt_IA           0x0001      
//LCDSHOWEXT  uint32 gul_ShowId;

enum
{
    Const_NoNeedRefresh,    //不需要调用驱动刷新
    Const_NeedRefresh,      //需要调用驱动刷新
};

enum
{
    Const_DispMode_LX=0,        //轮显
    Const_DispMode_JX,          //键显
    Const_DispMode_TX,          //停显
    Const_DispMode_Full,        //全屏显示
    Const_DispMode_IRDA,        //红外通讯显示
    Const_DispMode_LXGB,        //自动轮显一轮结束后关闭显示状态
    Const_DispMode_TXGB,        //停显一轮结束后关闭显示状态
    Const_DispMode_PwOff,       //停电N天以后的状态
    Const_DispModeOff,          //凌晨x点到第二天y点关闭lcd显示的状态
    Const_DispMode_AdvSysClr,
    Const_DispMode_AC,          //自动校表特殊显示
    Const_DispMode_RMTLX,       //遥控器轮显键加速轮显内容显示
    Const_DispMode_Err,         //错误显示模式
};

enum
{
    Const_Disp_RAM = 0,
    Const_Disp_E2,
};

/*ucSpecialMode的定义*/
#define Const_AdvSysClr     0x80    //
#define Const_AdvAC         0x40    //自动校表

#define Cosnt_RefreshTime   0x01    //停电显示时仅显示数据需要更新需要刷新,本表仅为时间
#define Const_RefreshAll    0x02    //停电时所有显示都更新

typedef struct s_DispStt
{
    uint8   ucMode;         //显示模式,轮显,键显,停显等
    uint8   ucTmr;          //显示的定时时间，或关闭显示时间
    uint8   ucPwrOffRefresh;//停电时显示是否需要刷新
    uint16  uiIndex;        //当前显示查的表中的序号,不管是轮显或是键显等
    uint32  ulPwrOffTmr;    //停电下显示的定时时间
}S_DISPSTT;

#define ConstDayMin (1*24*60) //1day=24hour=1*24*60min

/*
*定义的这个变量内的数据是上面设置下来的,以后不设置则不会变化,时间的单位是秒,或许需要
*定时从E2中更新数据
*/
typedef struct 
{
    uint8 ucLxCnt;          //循显屏数
    uint8 ucLxTm;           //轮显显示时间
    uint8 ucEnyDoc;         //显示电能小数位数
    uint8 ucPowDoc;         //显示功率(最大需量显示位数)
    uint8 ucJxCnt;          //按键循环显示屏数
    uint8 uc_ShowErrPst;    //ERR错误字显示位置
    uint8 ucJxTm;           //有电下键显显示时间(自扩)
    uint8 ucJxPowOffTm;     //停电下键显显示时间(自扩)
    uint16 ulPowOffShowTm ; //停电显示时间(自扩)
    uint8  ucFullShowTm;    //上电全屏时间（自扩）
    uint8  ucPowUpBgTm;     //上电背光亮时间(自扩)
    uint8  ucIrBgTm;        //红外触发背光亮时间(自扩)
    uint8  ucKeyBgTm;       //按键触发背光亮时间(自扩)
    uint8  ucEsamBgTm;      //ESAM错误背光亮时间(自扩)
    uint8  ucPowUpLedTm;    //LED自检时间(自扩)
    uint16 ul_CRC;          //CRC保护
}S_DISPCONTENT;



typedef struct 
{
    uint16  uiStr1;         //用于显示的str区内容
    uint16  uiUnitPt1;      //单位和小数点符号显示内容
    uint32  ulNum1;         //显示num区的内容
    uint32  ulNum1Msk;      //num区显示时,消隐或特殊字符显示信息
}S_LCDDISP;

typedef struct 
{
    uint8   ucPort;                 //为了兼容通讯
    uint8   ucCmd;                  //规约命令字
    uint8   ucLen;                  //收发数据的长度
    uint8   ucLevel;                //权限等级
    uint8   ucReply;                //是否需要回复
    Word16  ucErr;                  //错误状态字
    uint8   ucAtb;                  //数据属性,包括地址信息,帧判别状态
    Word32  uiID;                   //数据ID
    Word32  uiUserCode;             //操作者代码
    uint8   *ucbuf;               //接收的数据区首地址指针
}S_LCDDATEINFO;

LCDSHOWEXT  S_DISPCONTENT gs_DispCont;
LCDSHOWEXT S_DISPSTT   gs_DispStt ;        //状态转换中用到的结构
LCDSHOWEXT S_LCDDISP   gs_LcdDisp ;
LCDSHOWEXT uint8 guc_IrShowLcdId[5];
LCDSHOWEXT uint8 guc_ErrEsamMacSwTm; 
LCDSHOWEXT uint8 guc_ErrEsamKetSwTm;
LCDSHOWEXT uint8 guc_SwErrCode[8];
LCDSHOWEXT uint8 guc_ErrCodeCnt;
LCDSHOWEXT uint8 guc_ErrBtnFlag;
LCDSHOWEXT uint8 guc_ShowErrEndFlag;

LCDSHOWEXT uint32 gul_ShowId;
LCDSHOWEXT uint8 guc_ErrSwPt;       //错误显示指针


/*S_DISPCONT显示表成员结构体的定义
*uiInID为核心表内部ID,用uiInID可以查核心表
*ucStr用于lcd驱动中查中文字符串表
*ucAtb为属性,其中:
    bit7: 0不消隐/1消隐
    bit6: 0无/1有 符号数
    bit5: 0不要/1要 Hex2BCD
    bit4-bit0:用于查单位小数点格式表
*/
#define Const_DispNeedMsk   0x80                        //需要消隐
#define Const_DispSigned    0x40                        //有符号数显示
#define Const_DisppHEX2BCD  0x20                        //需要HEX转BCD显示
#define Const_DispUnitPt    (0x10+0x08+0x04+0x02+0x01)  //用于查小数点和单位符号表

typedef struct s_dispcont
{
//  uint32  ulInID;         //内部ID,当为0xFFFF表示无效屏,0xFFFE表示全屏
    uint16  uiStr;          //用于lcd驱动中查中文字符串表
    uint8   ucAtb;          //属性
//  uint8   ucExt;          //扩展
//  uint8   ucOffset;       //用于扩展规约的项
//  uint8   reserved;       //保留
}S_DISPCONT;

typedef struct s_idtab
{
    uint32 ulInID;
    uint8 uiNumb;
}S_IDTAB;

//[显示表]
/*
*这里的屏号是从0开始分别为1,2,3,...,屏号0无效
*在code区显示此值时需要在g_sDispStt.uiIndex的基础上加一才可以看到实际的屏号
*设置在E2中的屏号为大于0的整数
*注意:
    1.此表中内部核心ID不需要排序,而由屏号隐含顺序排列
    2.其中这个表里面的核心ID中,0xFFFF为无效屏号,仍然去数据处理中作用,0xFFFE为全屏显示
*/

#ifdef LCDSHOWSELF

const S_IDTAB gCs_TabConvent[]=
{   
    {0x00000000, 0x01}, //2,  当前组合有功电量总            (N)
    {0x00000100, 0x02}, //3,  当前组合有功电量尖            (N)
    {0x00000200, 0x03}, //4,  当前组合有功电量峰            (N)
    {0x00000300, 0x04}, //5,  当前组合有功电量平            (N)
    {0x00000400, 0x05}, //6,  当前组合有功电量谷            (N)

    {0x00010000, 0x06}, //2,  当前正有功电量总              (N)
    {0x00010100, 0x07}, //3,  当前正有功电量尖              (N)
    {0x00010200, 0x08}, //4,  当前正有功电量峰              (N)
    {0x00010300, 0x09}, //5,  当前正有功电量平              (N)
    {0x00010400, 0x0A}, //6,  当前正有功电量谷              (N)

    {0x00020000, 0x0B}, //2,  当前反有功电量总              (N)
    {0x00020100, 0x0C}, //3,  当前反有功电量尖              (N)
    {0x00020200, 0x0D}, //4,  当前反有功电量峰              (N)
    {0x00020300, 0x0E}, //5,  当前反有功电量平              (N)
    {0x00020400, 0x0F}, //6,  当前反有功电量谷              (N)

    {0x00000001, 0x10}, //7,  上1月组合有功电量总           (N)
    {0x00000101, 0x11}, //8,  上1月组合有功电量尖           (N)
    {0x00000201, 0x12}, //9,  上1月组合有功电量峰           (N)
    {0x00000301, 0x13}, //10, 上1月组合有功电量平           (N)
    {0x00000401, 0x14}, //11, 上1月组合有功电量谷           (N)

    {0x00010001, 0x15}, //7,  上1月正有功电量总             (N)
    {0x00010101, 0x16}, //8,  上1月正有功电量尖             (N)
    {0x00010201, 0x17}, //9,  上1月正有功电量峰             (N)
    {0x00010301, 0x18}, //10, 上1月正有功电量平             (N)
    {0x00010401, 0x19}, //11, 上1月正有功电量谷             (N)

    {0x00020001, 0x1A}, //7,  上1月反有功电量总             (N)
    {0x00020101, 0x1B}, //8,  上1月反有功电量尖             (N)
    {0x00020201, 0x1C}, //9,  上1月反有功电量峰             (N)
    {0x00020301, 0x1D}, //10, 上1月反有功电量平             (N)
    {0x00020401, 0x1E}, //11, 上1月反有功电量谷             (N)

    {0x00000002, 0x1F}, //12, 上2月组合有功电量总           (N)
    {0x00000102, 0x20}, //13, 上2月组合有功电量尖           (N)
    {0x00000202, 0x21}, //14, 上2月组合有功电量峰           (N)
    {0x00000302, 0x22}, //15, 上2月组合有功电量平           (N)
    {0x00000402, 0x23}, //16, 上2月组合有功电量谷           (N)

    {0x00010002, 0x24}, //12, 上2月正有功电量总             (N)
    {0x00010102, 0x25}, //13, 上2月正有功电量尖             (N)
    {0x00010202, 0x26}, //14, 上2月正有功电量峰             (N)
    {0x00010302, 0x27}, //15, 上2月正有功电量平             (N)
    {0x00010402, 0x28}, //16, 上2月正有功电量谷             (N)


    {0x00020002, 0x29}, //12, 上2月反有功电量总             (N)
    {0x00020102, 0x2A}, //13, 上2月反有功电量尖             (N)
    {0x00020202, 0x2B}, //14, 上2月反有功电量峰             (N)
    {0x00020302, 0x2C}, //15, 上2月反有功电量平             (N)
    {0x00020402, 0x2D}, //16, 上2月反有功电量谷             (N)

    {0x00000003, 0x2E}, //17, 上3月组合有功电量总           (N)
    {0x00000103, 0x2F}, //18, 上3月组合有功电量尖           (N)
    {0x00000203, 0x30}, //19, 上3月组合有功电量峰           (N)
    {0x00000303, 0x31}, //20, 上3月组合有功电量平           (N)
    {0x00000403, 0x32}, //21, 上3月组合有功电量谷           (N)

    {0x00010003, 0x33}, //17, 上3月正有功电量总             (N)
    {0x00010103, 0x34}, //18, 上3月正有功电量尖             (N)
    {0x00010203, 0x35}, //19, 上3月正有功电量峰             (N)
    {0x00010303, 0x36}, //20, 上3月正有功电量平             (N)
    {0x00010403, 0x37}, //21, 上3月正有功电量谷             (N)

    {0x00020003, 0x38}, //17, 上3月反有功电量总             (N)
    {0x00020103, 0x39}, //18, 上3月反有功电量尖             (N)
    {0x00020203, 0x3A}, //19, 上3月反有功电量峰             (N)
    {0x00020303, 0x3B}, //20, 上3月反有功电量平             (N)
    {0x00020403, 0x3C}, //21, 上3月反有功电量谷             (N)


    {0x000C0000, 0x3D}, //20, 当前周期电量                  (N)
    {0x000C0001, 0x3E}, //21, 上1周期电量                   (N)
    {0x00150000, 0x55}, //A相正向电量
    {0x00290000, 0x56}, //B相正向电量
    {0x003D0000, 0x57}, //C相正向电量

    {0x00150001, 0x58}, //上月A相正向电量
    {0x00290001, 0x59}, //上月B相正向电量
    {0x003D0001, 0x5A}, //上月C相正向电量
    {0x00150002, 0x5B}, //上上月A相正向电量
    {0x00290002, 0x5C}, //上上月B相正向电量
    {0x003D0002, 0x5D}, //上上月C相正向电量

    {0x00030000, 0x68}, //总无功正向电量
    {0x00170000, 0x69}, //A相无功正向电量
    {0x002b0000, 0x6A}, //B相无功正向电量
    {0x003f0000, 0x6B}, //C相无功正向电量  
  
    

};
const   uint8  guc_LenOfTabConvent = sizeof(gCs_TabConvent) / sizeof(S_IDTAB);


const   S_IDTAB  gCs_TabConvCS[]=
{
    {0x04000101, 0x3F}, //日期星期
    {0x04000102, 0x40}, //时间
    {0x04000401, 0x41}, //通讯地址 占用2屏
    {0x04000402, 0x43}, //表号 占用2屏
    {0x0400040E, 0x45}, //客户编号 占用2屏
    {0x04000409, 0x47}, //常数
    {0x04000B01, 0x48}, //结算日1
    {0x04000B02, 0x49}, //结算日2
    {0x04000B03, 0x4A}, //结算日3

};
const   uint8  guc_LenOfTabConvCS = sizeof(gCs_TabConvCS) / sizeof(S_IDTAB);
const   S_IDTAB  gCs_TabConvBL[]=
{
    {0x02010100, 0x4B}, // A电压
    {0x02010200, 0x5E}, // B相电压  // hjj add
    {0x02010300, 0x5F}, // C相电压
    {0x02020100, 0x4C}, // A电流
    {0x02020200, 0x60}, // B相电流  //hjj add
    {0x02020300, 0x61}, // C相电流   
    {0x02030000, 0x4D}, // 总功率
    {0x02030100, 0x62}, // A功率
    {0x02030200, 0x63}, // B功率
    {0x02030300, 0x64}, // C功率
    {0x02060000, 0x4E}, // 功率因素
    {0x02060100, 0x65}, // A功率因素
    {0x02060200, 0x66}, // B功率因素
    {0x02060300, 0x67}, // C功率因素
    {0x02070100, 0x6C}, // A相角
    {0x02070200, 0x6D}, // B相角
    {0x02070300, 0x6E}, // C相角
    
    {0x02040000, 0x6F}, // 总无功功率
    {0x02040100, 0x70}, // A无功功率
    {0x02040200, 0x71}, // B无功功率
    {0x02040300, 0x72}, // C无功功率
    
    {0x02800008, 0x50}, // 电池电压
    
    {0x02800002, 0x50}, //A频率
    {0x0280000C, 0x73}, // B频率
    {0x0280000D, 0x74}, // C频率
};
const   uint8  guc_LenOfTabConvBL = sizeof(gCs_TabConvBL) / sizeof(S_IDTAB);

const S_DISPCONT  gCs_DispTab[]=
{
    { 0x0000, 0x00,}, //0,  全屏显示
    { 0x0004, 0x43,}, //1,  当前组合正有功电量总                (N)
    { 0x0004, 0x43,}, //2,  当前组合正有功电量尖                (N)
    { 0x0004, 0x43,}, //3,  当前组合正有功电量峰                (N)
    { 0x0004, 0x43,}, //4,  当前组合正有功电量平                (N)
    { 0x0004, 0x43,}, //5,  当前组合正有功电量谷                (N)

    { 0x0004, 0x43,}, //6,  当前正有功电量总                (N)
    { 0x0004, 0x43,}, //7,  当前正有功电量尖                (N)
    { 0x0004, 0x43,}, //8,  当前正有功电量峰                (N)
    { 0x0004, 0x43,}, //9,  当前正有功电量平                (N)
    { 0x0004, 0x43,}, //10, 当前正有功电量谷                (N)

    { 0x0004, 0x43,}, //11,  当前反有功电量总                (N)
    { 0x0004, 0x43,}, //12,  当前反有功电量尖                (N)
    { 0x0004, 0x43,}, //13,  当前反有功电量峰                (N)
    { 0x0004, 0x43,}, //14,  当前反有功电量平                (N)
    { 0x0004, 0x43,}, //15,  当前反有功电量谷                (N)

    { 0x0108, 0x43,}, //16,  上1月组合有功电量总               (N)
    { 0x0108, 0x43,}, //17,  上1月组合有功电量尖               (N)
    { 0x0108, 0x43,}, //18,  上1月组合有功电量峰               (N)
    { 0x0108, 0x43,}, //19, 上1月组合有功电量平               (N)
    { 0x0108, 0x43,}, //20, 上1月组合有功电量谷               (N)

    { 0x0108, 0x43,}, //21,  上1月正有功电量总               (N)
    { 0x0108, 0x43,}, //22,  上1月正有功电量尖               (N)
    { 0x0108, 0x43,}, //23,  上1月正有功电量峰               (N)
    { 0x0108, 0x43,}, //24, 上1月正有功电量平               (N)
    { 0x0108, 0x43,}, //25, 上1月正有功电量谷               (N)

    { 0x0108, 0x43,}, //26,  上1月反有功电量总               (N)
    { 0x0108, 0x43,}, //27,  上1月反有功电量尖               (N)
    { 0x0108, 0x43,}, //28,  上1月反有功电量峰               (N)
    { 0x0108, 0x43,}, //29, 上1月反有功电量平               (N)
    { 0x0108, 0x43,}, //30, 上1月反有功电量谷               (N)

    { 0x020C, 0x43,}, //31, 上2月组合有功电量总               (N)
    { 0x020C, 0x43,}, //32, 上2月组合有功电量尖               (N)
    { 0x020C, 0x43,}, //33, 上2月组合有功电量峰               (N)
    { 0x020C, 0x43,}, //34, 上2月组合有功电量平               (N)
    { 0x020C, 0x43,}, //35, 上2月组合有功电量谷               (N)

    { 0x020C, 0x43,}, //36, 上2月正有功电量总               (N)
    { 0x020C, 0x43,}, //37, 上2月正有功电量尖               (N)
    { 0x020C, 0x43,}, //38, 上2月正有功电量峰               (N)
    { 0x020C, 0x43,}, //39, 上2月正有功电量平               (N)
    { 0x020C, 0x43,}, //40, 上2月正有功电量谷               (N)

    { 0x020C, 0x43,}, //41, 上2月反有功电量总               (N)
    { 0x020C, 0x43,}, //42, 上2月反有功电量尖               (N)
    { 0x020C, 0x43,}, //43, 上2月反有功电量峰               (N)
    { 0x020C, 0x43,}, //44, 上2月反有功电量平               (N)
    { 0x020C, 0x43,}, //45, 上2月反有功电量谷               (N)

    { 0x030b, 0x43,}, //46, 上3月组合有功电量总               (N)
    { 0x030c, 0x43,}, //47, 上3月组合有功电量尖               (N)
    { 0x030d, 0x43,}, //48, 上3月组合有功电量峰               (N)
    { 0x030e, 0x43,}, //49, 上3月组合有功电量平               (N)
    { 0x030f, 0x43,}, //50, 上3月组合有功电量谷               (N)

    { 0x032c, 0x43,}, //51, 上3月正有功电量总               (N)
    { 0x032d, 0x43,}, //52, 上3月正有功电量尖               (N)
    { 0x032e, 0x43,}, //53, 上3月正有功电量峰               (N)
    { 0x032f, 0x43,}, //54, 上3月正有功电量平               (N)
    { 0x0330, 0x43,}, //55, 上3月正有功电量谷               (N)

    { 0x0331, 0x43,}, //56, 上3月反有功电量总               (N)
    { 0x0332, 0x43,}, //57, 上3月反有功电量尖               (N)
    { 0x0333, 0x43,}, //58, 上3月反有功电量峰               (N)
    { 0x0334, 0x43,}, //59, 上3月反有功电量平               (N)
    { 0x0335, 0x43,}, //60, 上3月反有功电量谷               (N)

    { 0x0001, 0x43,}, //61当前周期电量               (N)
    { 0x010b, 0x43,}, //62上月周期电量            (N)
    
    { 0x001F, 0x01,}, //63, 日期及星期  //hjj add
    { 0x001F, 0x02,}, //64, 时间

    { 0x0000, 0x13,}, //65, 通讯地址高4位
    { 0x0000, 0x00,}, //66, 通讯地址低8位

    { 0x0000, 0x13,}, //67, 表号高4位 //hjj add
    { 0x0000, 0x00,}, //68, 表号低8位

    { 0x0020, 0x13,}, //69, 客户编号高4位
    { 0x0020, 0x00,}, //70, 客户编号低8位


    {0x0000, 0x80},   //71常数
    {0x0000, 0x0b},   //72结算日1
    {0x0000, 0x0b},   //73结算日2
    {0x0000, 0x0b},   //74结算日3

    { 0x001C, 0x8E,}, //75, 电压 //hjj add
    { 0x0019, 0x8F,}, //76, 电流
    { 0x0000, 0x86,}, //77, 总功率
    { 0x0000, 0x87,}, //78, 功率因素
    { 0x0000, 0x00,}, //79, 零线电流
    { 0x0021, 0x88,}, //80, 频率
//    { 0x0000, 0x85,}, //80, 电池电压
    { 0x0000, 0x12,}, //81, 编程日期
    { 0x0000, 0x02,}, //82, 编程时间
    { 0x0000, 0x12,}, //83, 校表日期
    { 0x0000, 0x11,}, //84, 校表时间
    { 0x0001, 0x43,}, //85 A相正向电量 //hjj add
    { 0x0002, 0x43,}, //86 B相正向电量
    { 0x0003, 0x43,}, //87 C相正向电量
    { 0x0005, 0x43,}, //88 A相正向电量 //hjj add
    { 0x0006, 0x43,}, //89 B相正向电量
    { 0x0007, 0x43,}, //90 C相正向电量
    { 0x0009, 0x43,}, //91 A相正向电量 //hjj add
    { 0x000A, 0x43,}, //92 B相正向电量
    { 0x000B, 0x43,}, //93 C相正向电量
    { 0x001D, 0x8E,}, //94 B相电压
    { 0x001E, 0x8E,}, //95 C相电压
    { 0x001A, 0x8F,}, //96 B相电流
    { 0x001B, 0x8F,}, //97 C相电流

    { 0x0021, 0x86,}, //98, A总功率
    { 0x0022, 0x86,}, //99, B总功率
    { 0x0023, 0x86,}, //100,C总功率

    { 0x0021, 0x87,}, //101, A功率因素
    { 0x0022, 0x87,}, //102, B功率因素
    { 0x0023, 0x87,}, //103, C功率因素
    { 0x0004, 0x4D,}, //104总无功
    { 0x0001, 0x4D,}, //105A无功
    { 0x0002, 0x4D,}, //106B无功
    { 0x0003, 0x4D}, //107C无功

    { 0x0021, 0x84,}, //108, A相角
    { 0x0022, 0x84,}, //109, B相角
    { 0x0023, 0x84,}, //110, C相角

    { 0x0000, 0x8C,}, //111, 总无功功率
    { 0x0021, 0x8C,}, //112, A相无功功率
    { 0x0022, 0x8C,}, //113, B相无功功率
    { 0x0023, 0x8C,}, //114, C相无功功率
    
    { 0x0022, 0x88,}, //115, B频率
    { 0x0023, 0x88,}, //116, C频率
};


const S_IDTAB  gCs_TabAdd[]=
{
    {0x03300001, 0x51}, //编程时间 占用2屏
    {0x03300401, 0x53}, //校时时间 占用2屏
};
const uint8  guc_LenOfTabAdd = sizeof(gCs_TabAdd) / sizeof(S_IDTAB);

#else
extern const   S_IDTAB  gCs_TabConvent[];
extern const   uint8  guc_LenOfTabConvent;
extern const   S_IDTAB  gCs_TabConvBL[];
extern const   uint8  guc_LenOfTabConvBL;
extern const   S_IDTAB  gCs_TabConvCS[];
extern const   uint8  guc_LenOfTabConvCS;
#endif


// 单位小数点表
/*
*低半字节的：显示符号（等于某个值即显示对应的符号）
*其余几位的显示情况：显示小数点（对应位等于1：显示，否则不显示）
*/
enum
{//单位表
    UNIT_NONE,          //0x00
    UNIT_KWH,           //0x01
    UNIT_KW,            //0x02
    UNIT_V,            //0x03
    UNIT_A,          //0x04
    UNIT_VA,       //0X05
    UNIT_VAH,    //0X06
 //   UNIT_Jine,          //0x04
//  UNIT_BH,            //0x05
//  UNIT_TIME,          //0x07
//  UNIT_SD,            //0x08
//  UNIT_SQ,            //0x09
//  UNIT_SY,            //0x0a
//  UNIT_JT,            //0x0b
//  UNIT_V,             //0x0c
//  UNIT_A,             //0x0d
//  UNIT_PF,            //0x0e
};

/*
1.SDT-M3485-HP-4屏
                                         DP2             DP5
             ____    ____    ____    ____    ____    ____    ____    ____
            |    |  |    |  |    |  |    |  |    |  |    |  |    |  |    |
            |____|  |____|  |____|  |____|  |____|  |____|  |____|  |____|
            |    |  |    |  |    |  |    | .|    |  |    | .|    |  |    |
            |____|  |____| .|____|  |____| .|____|. |____| .|____| .|____|
                         DP1              DP3     DP4     DP6     DP7
*/

//定义实际使用到的小数点,用于建立小数点和单位符号表
#define _P1      0x0010
#define _P2      0x0020
#define _P3      0x0040
#define _P4      0x0080
#define _P5      0x0100
#define _P6      0x0200
#define _P7      0x0400
#define _P8      0x0800
#define _P9      0x1000
#define _P10    0x2000
#define _PFu    0x4000


//程序处理用小数点宏定义
#define Point_D1 _P7                //对应第2位数后面的小数点,后面跟1位小数
#define Point_D2 _P6                //对应第3位数后面的小数点,后面跟2位小数
#define Point_D3 _P4                //对应第4位数后面的小数点,后面跟3位小数
#define Point_D4 _P3                //对应第5位数后面的小数点,后面跟4位小数
#define Point_D6 _P1                //对应第7位数后面的小数点,后面跟6位小数
#define Point_Fu _PFu                //数字前面的负号

#ifdef LCDSHOWSELF
const   uint16  gCui_UnitPointTab[]=
{
    0x0000,                 //0x00.不显示,如显示表号等无单位小数点的信息时使用
    _P6+_P3,                //0x01.日期(XX.XX.XX  X)
    _P3+_P6+_P2+_P5,        //0x02.时间(XX:XX:XX)
    UNIT_KWH,               //0x03.正向电量类,默认不显示小数点,由E2中读取的小数点位数决定,在显示处理函数中组织完成
    _P7,                    //0x04.电压(X.X  V)
    _P4,                    //0x05.电流(X.XXX A)
    UNIT_KW,                //0x06.有功功率或需量(X.XXXXX kW)
    //_P5+UNIT_CS,            //0x07.功率因素(X.XXX,cos￠)
    _P4,
    _P6,                    //0x08.二位小数(XX.XX)
    _P3+_P6,                //0x09.XX.XX.XX 年月日 或 日月时
    _P3,                    //0x0A.XXXX.XXXX
    _P6,                    //0x0b 结算日 XX.XX
    UNIT_VA,                 //0x0c
    UNIT_VAH,                 //0x0d
    UNIT_V + _P7,                 //0x0e
    UNIT_A + _P4,                 //0x0f
    0x0000,                 //0x10
    0x0000,                 //0x11
    0x0000,                 //0x12
    0x0000,                 //0x13
};
#endif

#define MacIsDispEnerg ((ucType == 0x16) || (ucType == 0x03))
#define _SW_ISNEEDNEGDISP   0       //是否需要负号显示功能

typedef struct s_ptmsk
{
    uint16  uiPtMsk;    //对应小数点的屏蔽位
    uint8   ucNum;      //小数点对应第几个位置
    uint8   reserved;   //保留
}S_PTMSK;

#ifdef LCDSHOWSELF
const S_PTMSK  gCs_szPtMskTab[] =
{
    {Point_D6, 6, 0x00},
    {Point_D4, 4, 0x00},
    {Point_D3, 3, 0x00},
    {Point_D2, 2, 0x00},
    {Point_D1, 1, 0x00},
};

#endif

typedef struct s_dispsttbit
{
    uint8*  pucDst;     //目的状态字
    uint8*  pucSrc;     //源状态字
    uint8   ucDstMsk;   //目的状态字的位,其值大小为BIT0 - BIT7
    uint8   ucSrcMsk;   //源状态字的位定义,其值大小为BIT0 - BIT7
    uint8   ucAtb;      //属性
    uint8   ucOffset;   //偏移,用于查子表时的偏移指针
}S_DISPSTTBIT;


#define __SW_BLINK_DISP__  1            //闪烁位控制

enum
{
    Const_Disp_KeyUp = 0,
    Const_Disp_KeyDn,
    Const_Disp_RmtUp,       //遥显上翻屏
    Const_Disp_RmtDn,       //遥显下翻屏
};

enum
{
    Const_Ret_OK = 0,
    Const_Ret_Err,
    Const_Ret_Disp_Off,             //关闭显示
    Const_Ret_DispFull,             //全屏显示
    Const_Ret_DispMode_Err,         //错误报警
};

void ShowSysStart(void);
void ShowInitial(void);
void ShowKeyManage(uint8 ucType);
void RefreshDisplayPara(void); //没用到
void TaskLcdShow(void);         //没用到
void ShowSecondProc(void);
uint8 ShowDataProc(void);
void ShowRefresh(void);
void ShowAdvEnterAC(void);      //没有
void ShowAdvCancelAC(void);     //没有
void SleepDispInit(void);
uint8 SleepDisp(void);
uint8 SearchDispNO(Word32 ulID);
void ShowStateManage(void);
void ShowStateFlashManage(void);//没有

#endif
