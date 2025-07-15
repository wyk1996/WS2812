#ifndef _INITPARA_H_
#define _INITPARA_H_

//#define  INIT_EXT  extern

#ifndef INIT_EXT
    #define INIT_EXT extern
#endif


INIT_EXT   uint8 guc_RTCSave;
void InitE2Data(uint8 uc_RTCSave);
INIT_EXT   const uint8  InitPara38[];
INIT_EXT   const uint8  InitPara3[];

#endif
