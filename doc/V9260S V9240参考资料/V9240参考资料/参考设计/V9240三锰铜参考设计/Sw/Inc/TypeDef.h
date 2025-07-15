#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__


#define  nop()  __no_operation()
//#define                        __disable_interrupt()
//#define                       __enable_interrupt()
#define  idata  __idata
#define  pdata  __pdata
#define  xdata  __xdata
#define  code   __code
#define  data   __data
#define  idata  __idata
#define  bit    __bit
#define  bdata  __bdata
#define  interrupt __interrupt


#define CPU_BIT  8

#if CPU_BIT == 8
typedef unsigned char data  udata8;
//-------------------------------
typedef unsigned char       uint8;
//------------------------------
typedef unsigned char xdata uint8x;
//------------------------------
typedef signed char         int8;
//------------------------------
typedef signed char xdata   int8x;
//------------------------------
typedef unsigned int        uint16;
//------------------------------
typedef unsigned int xdata  uint16x;
//------------------------------
typedef signed int          int16;
//------------------------------
typedef signed int xdata    int16x;
//------------------------------
typedef unsigned long       uint32;
//------------------------------
typedef unsigned long xdata uint32x;
//------------------------------
typedef signed long         int32;
//------------------------------
typedef signed long xdata   int32x;
//------------------------------

typedef void* Object;           //定义任意类型为Object
typedef const char * string;    //定义字符串类型


//------------------------------
//typedef signed long long  int64;

//typedef unsigned long long uint64;

//------------------------------
typedef union
{
    uint16 word;
    uint8  byte[2];
}Word16;

typedef union
{
    uint32 lword;
    uint16 word[2];
    uint8  byte[4]; 
}Word32;

typedef union
{
    Word32 word;
    uint8  byte;
}Word40;


struct bit_def 
{
    char    b0:1;
    char    b1:1;
    char    b2:1;
    char    b3:1;
    char    b4:1;
    char    b5:1;
    char    b6:1;
    char    b7:1;
};

union byte_def 
{
    struct bit_def bite;
    unsigned char  byte;
};

#ifndef true
    #define true 1==1
#endif

#ifndef false
    #define false 0==1
#endif

/* another normal use */
#define     BIT0        0x01
#define     BIT1        0x02
#define     BIT2        0x04
#define     BIT3        0x08
#define     BIT4        0x10
#define     BIT5        0x20
#define     BIT6        0x40
#define     BIT7        0x80

#define     EBIT0       0x0001
#define     EBIT1       0x0002
#define     EBIT2       0x0004
#define     EBIT3       0x0008
#define     EBIT4       0x0010
#define     EBIT5       0x0020
#define     EBIT6       0x0040
#define     EBIT7       0x0080
#define     EBIT8       0x0100
#define     EBIT9       0x0200
#define     EBIT10      0x0400
#define     EBIT11      0x0800
#define     EBIT12      0x1000
#define     EBIT13      0x2000
#define     EBIT14      0x4000
#define     EBIT15      0x8000
#define     EBIT16      0x00010000
#define     EBIT17      0x00020000
#define     EBIT18      0x00040000
#define     EBIT19      0x00080000
#define     EBIT20      0x00100000
#define     EBIT21      0x00200000
#define     EBIT22      0x00400000
#define     EBIT23      0x00800000
#define     EBIT24      0x01000000
#define     EBIT25      0x02000000
#define     EBIT26      0x04000000
#define     EBIT27      0x08000000
#define     EBIT28      0x10000000
#define     EBIT29      0x20000000
#define     EBIT30      0x40000000
#define     EBIT31      0x80000000

#ifdef __MAIN__
const uint8 code BIT8MAP[]=
{
    BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7
};

const uint32 code BIT32MAP[]=
{
    EBIT0,EBIT1,EBIT2,EBIT3,EBIT4,EBIT5,EBIT6,EBIT7,
    EBIT8,EBIT9,EBIT10,EBIT11,EBIT12,EBIT13,EBIT14,EBIT15,
    EBIT16,EBIT17,EBIT18,EBIT19,EBIT20,EBIT21,EBIT22,EBIT23,
    EBIT24,EBIT25,EBIT26,EBIT27,EBIT28,EBIT29,EBIT30,EBIT31,       
};
#else
extern const uint8 code  BIT8MAP[];
extern const uint32 code BIT32MAP[];
#endif

#define     NONE    0



#define dim(x) (sizeof(x)/sizeof(x[0]))     //求一个数组的维度


#define ERR_OK      (uint8)0
#define ERR_VAR     (uint8)1
#define ERR_FT      (uint8)2

#define SA_VAR_CHECK(X)  {if(X){return ERR_VAR;}}
#define SA_OK()      {return ERR_OK;}

 


#endif







#endif