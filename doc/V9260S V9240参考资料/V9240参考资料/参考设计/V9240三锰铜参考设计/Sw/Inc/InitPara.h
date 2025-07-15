#ifndef _INITPARA_H_
#define _INITPARA_H_

#ifndef INIT_EXT
    #define INIT_EXT extern
#endif

INIT_EXT uint8 guc_RTCSave;
void InitE2Data(uint8 uc_RTCSave);
INIT_EXT __root const uint8 code InitPara38[];
INIT_EXT __root const uint8 code InitPara3[];
#endif