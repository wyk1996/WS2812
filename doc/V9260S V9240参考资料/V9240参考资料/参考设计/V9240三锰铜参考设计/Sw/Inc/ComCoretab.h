#ifndef _AP_CORETAB_H_
#define _AP_CORETAB_H_

#ifndef AP_CORETAB_EXT
#define AP_CORETAB_EXT extern
#endif


#define Data_RO         0x08        //是否只读
#define Data_WO         0x10        //是否只写
#define Data_Factor     0x20        //厂内操作
#define Data_BCD        0x40        //是否需要BCD转换
#define Data_RF         0x80        //是否需要刷新数据

/*******************************************************************************
**定义核心表菜单的数据结构
*******************************************************************************/
typedef struct s_coretab
{
    uint32  uiID;                   //数据ID
    uint8   ucAtb;                  //数据属性
    uint8   ucLen;                  //单项数据长度
    uint8   ucTotal1;               //D1的同类项总数,假设同类项连自己有N项,那么ucTotal=N-1
    uint8   ucTotal0;               //D0的同类项总数,假设同类项连自己有N项,那么ucTotal=N-1
    uint8   ucFunNo;                //查特殊处理函数族的标号
    uint16  pAddr;                  //数据的存储地址
    uint16  uiOther;                //其他的一些待扩展的东西
}S_CORETAB;

extern const   S_CORETAB code gs_CoreTab0[];
extern const   S_CORETAB code gs_CoreTab2[];
extern const   S_CORETAB code gs_CoreTab3[];
extern const   S_CORETAB code gs_CoreTab4[];
extern const   S_CORETAB code gs_CoreTab5[] ;
extern const   S_CORETAB code gs_CoreTab6[] ;
extern const   S_CORETAB code gs_CoreTab1D[] ;
extern const   S_CORETAB code gs_CoreTab1E[] ;
extern const   S_CORETAB code gs_CoreTab4F[] ;








#define FindType        0x03        //查找表格的方式，0：顺序查找法  1：中值查找法
/*******************************************************************************
**定义核心表与数据ID3的关系
*******************************************************************************/
typedef struct s_coreID
{
    uint8   uiID3;                  //数据ID3
    S_CORETAB code *pTab;            //数据表格
    uint8   ucNum;                  //表格里面的数据项个数
}S_COREID;
extern const   S_COREID code gs_CoreID[];         //核心数据表与ID的对应关系
extern const   uint8  code NumofCoreID;

/*******************************************************************************
**定义特殊处理函数的数据结构
*******************************************************************************/
typedef uint8 (*PFUNC)(S_FRAMEINFO*, S_CORETAB code*);

extern const PFUNC code FunList[];


#endif

