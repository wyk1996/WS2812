#ifndef __CMPCODE_H__
#define __CMPCODE_H__


#ifndef CMPCODE_EXT
    #define CMPCODE_EXT  extern
#endif


uint8 ReadCodeData(S_FRAMEINFO *s_FrmInfo, S_CORETAB code* CTable);
void ReadFlash(uint32 ulAddr,uint8* buff,uint8 len);
uint8 GetFactor(uint8* encryptbuff, uint8* flashbuff);
void CodeEncrypt(uint8* factorbuff, uint8* buff,uint8* Combuf);
void GetKeyCMD(void);

#endif