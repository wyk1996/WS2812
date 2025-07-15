#ifndef _DW8051_H_
#define _DW8051_H_

/* STANDARD 8052 */
#include "reg52.h"



/* NEWADD FOR ZEBOR */
/* DW8051 NEW_REG */


 __sfr __no_init volatile unsigned char CKCON   @0x8e; 
 __sfr __no_init volatile unsigned char EICON   @0xd8;

 __sfr __no_init volatile unsigned char EIE     @0xE8;
 __sfr __no_init volatile unsigned char EXIF    @0x91;
 __sfr __no_init volatile unsigned char EIP     @0xf8;

 __sfr __no_init volatile unsigned char SCON1   @0xC0;
 __sfr __no_init volatile unsigned char SBUF1   @0xC1;

 __sfr __no_init volatile unsigned char DPL1    @0x84;
 __sfr __no_init volatile unsigned char DPH1    @0x85;
 __sfr __no_init volatile unsigned char isfr    @0xf1;
 __sfr __no_init volatile unsigned char debug   @0xff;
 __sfr __no_init volatile unsigned char SPC_FNC @0x8F; //wrs


#endif
