
#ifndef _POWMANGE_H_
#define _POWMANGE_H_

#ifndef POWMANGE_EXT
#define POWMANGE_EXT extern
#endif


POWMANGE_EXT uint16 gs_powerErr;

void PowDownChk(void);
uint8 PowerDnPro(void);
uint8 JudgePowerDnValue(uint32 RMSUValue);

#endif
