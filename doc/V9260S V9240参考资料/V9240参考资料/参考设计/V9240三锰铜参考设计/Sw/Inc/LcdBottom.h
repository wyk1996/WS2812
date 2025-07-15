#ifndef __LCDBOTTOM_H__
#define __LCDBOTTOM_H__

#ifndef LCDBOTTOMEXT
    #define LCDBOTTOMEXT extern
#endif


#define Const_LCDRAMLen 40

LCDBOTTOMEXT uint8 guc_LCDRAM[Const_LCDRAMLen];

void Disp_Full(void);
void Disp_Clear(void);
void Dsip_Clr(void);
void RefreshLCDRAM(uint8 *p);
void LCD_RAMUpdata(void);
void LCD_Off(void);
void LCDInitSleep(void);
void InitLCD(void);


#endif