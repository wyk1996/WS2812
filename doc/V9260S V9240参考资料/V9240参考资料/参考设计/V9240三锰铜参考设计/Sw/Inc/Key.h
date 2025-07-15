#ifndef __KEY_H__
    #define __KEY_H__


#ifndef KEY_EXT
    #define KEY_EXT extern
#else 
    #define KEY_SELF
#endif

#define KeyNums     2
enum
{
  UpKey=0,
  PrKey,
};
enum
{
    TmOff=0,
    TmOn,
};

enum 
{
    KeyRls=0,
    KeyDw,
    KeyDWLong,
};

typedef struct
{
    uint8  Status;       //按键状态，按下，释放
    uint8  TmStu;        //定时器状态
    uint16 DlyCnt;       //延时计数
    uint16 KeyThd;       //按键门限值
    uint16 LgKeyThd;     //长按键门限值
}KEYCTR;

KEY_EXT KEYCTR  idata gs_KeyCtr[KeyNums];

KEY_EXT uint8       guc_DyBgLed;        // 背光延时控制寄存器
//KEY_EXT uint8       guc_DyLed;          // LED灯控制
KEY_EXT uint16      gui_DyPgmOut;
KEY_EXT uint16      gui_DyProDmOut;
KEY_EXT uint8       guc_PrOutTm;        //编程闭锁等退出时间

KEY_EXT uint8   guc_PrKeyNewDown;       // 编程键是否新按下
KEY_EXT uint8   guc_CoverStatus;        // 表盖状态
KEY_EXT uint8   guc_PoweroffCoverJl;    // 有电下是否开表盖
KEY_EXT uint8   guc_CoverKeyDwCnt;
KEY_EXT uint8   guc_CoverKeyUpCnt;





#define Const_DyBLed   20

void KeyScan(void);
void LongKeyProc(void);
void KeyProc(void);
void KeyInit(void);
//void CoverScanSleep(void);
void CoverKeyScan(void);
void KeyDispSleepDown(void);
void KeyPgm_DyOut(void);


#endif
