/***************************************************************************
 *                                - io8052.h -
 *
 * Special header for the _generic 8052 Microcontroller.
 *
 ***************************************************************************/

#ifndef IO8052_H
#define IO8052_H

#define __8052__

#ifdef __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
   #pragma system_include
#endif
#pragma language=extended

/*-------------------------------------------------------------------------
 *   MCS-51 Core
 *-------------------------------------------------------------------------*/

__sfr __no_init volatile union
{
  unsigned char ACC; /* Accumulator */
  struct /* Accumulator */
  {
    unsigned char ACC0 : 1;
    unsigned char ACC1 : 1;
    unsigned char ACC2 : 1;
    unsigned char ACC3 : 1;
    unsigned char ACC4 : 1;
    unsigned char ACC5 : 1;
    unsigned char ACC6 : 1;
    unsigned char ACC7 : 1;
  } ACC_bit;
} @ 0xE0;


#define ACC0  ACC_bit.ACC0
#define ACC1  ACC_bit.ACC1
#define ACC2  ACC_bit.ACC2
#define ACC3  ACC_bit.ACC3
#define ACC4  ACC_bit.ACC4
#define ACC5  ACC_bit.ACC5
#define ACC6  ACC_bit.ACC6
#define ACC7  ACC_bit.ACC7

__sfr __no_init volatile union
{
  unsigned char PSW; /* Program Status word */
  struct /* Program Status word */
  {
    unsigned char P : 1;
    unsigned char F1 : 1;
    unsigned char OV : 1;
    unsigned char RS0 : 1;
    unsigned char RS1 : 1;
    unsigned char F0 : 1;
    unsigned char AC : 1;
    unsigned char CY : 1;
  } PSW_bit;
} @ 0xD0;

#define P PSW_bit.P
#define F1 PSW_bit.F1
#define OV PSW_bit.OV
#define RS0 PSW_bit.RS0
#define RS1 PSW_bit.RS1
#define F0 PSW_bit.F0
#define AC PSW_bit.AC
#define CY PSW_bit.CY


__sfr __no_init volatile unsigned char SP @ 0x81; /* Stack Pointer */
__sfr __no_init volatile unsigned char DPL @ 0x82; /* Data Pointer Low byte, LSB of DPTR */
__sfr __no_init volatile unsigned char DPH @ 0x83; /* Data Pointer High byte, MSB of DPTR */
__sfr __no_init volatile union
{
  unsigned char B; /* B Register */
  struct /* B Register */
  {
    unsigned char B0 : 1;
    unsigned char B1 : 1;
    unsigned char B2 : 1;
    unsigned char B3 : 1;
    unsigned char B4 : 1;
    unsigned char B5 : 1;
    unsigned char B6 : 1;
    unsigned char B7 : 1;
  } B_bit;
} @ 0xF0;

#define B0  B_bit.B0
#define B1  B_bit.B1
#define B2  B_bit.B2
#define B3  B_bit.B3
#define B4  B_bit.B4
#define B5  B_bit.B5
#define B6  B_bit.B6
#define B7  B_bit.B7


/*-------------------------------------------------------------------------
 *   Timers
 *-------------------------------------------------------------------------*/

__sfr __no_init volatile unsigned char TL2 @ 0xCC; /* Timer2 Low */

__sfr __no_init volatile unsigned char TH0 @ 0x8C; /* Timer0 High */
__sfr __no_init volatile union
{
  unsigned char TCON; /* Timer Control */
  struct /* Timer Control */
  {
    unsigned char IT0 : 1;
    unsigned char IE0 : 1;
    unsigned char IT1 : 1;
    unsigned char IE1 : 1;
    unsigned char TR0 : 1;
    unsigned char TF0 : 1;
    unsigned char TR1 : 1;
    unsigned char TF1 : 1;
  } TCON_bit;
} @ 0x88;


#define IT0 TCON_bit.IT0
#define IE0 TCON_bit.IE0
#define IT1 TCON_bit.IT1
#define IE1 TCON_bit.IE1
#define TR0 TCON_bit.TR0
#define TF0 TCON_bit.TF0
#define TR1 TCON_bit.TR1
#define TF1 TCON_bit.TF1



__sfr __no_init volatile unsigned char TH2 @ 0xCD; /* Timer2 High */
__sfr __no_init volatile unsigned char TH1 @ 0x8D; /* Timer1 High */
__sfr __no_init volatile union
{
  unsigned char TMOD; /* Timer Mode */
  struct /* Timer Mode */
  {
    unsigned char M00 : 1;
    unsigned char M10 : 1;
    unsigned char C_T0 : 1;
    unsigned char GATE0 : 1;
    unsigned char M01 : 1;
    unsigned char M11 : 1;
    unsigned char C_T1 : 1;
    unsigned char GATE1 : 1;
  } TMOD_bit;
} @ 0x89;

#define  M00   TMOD_bit.M00
#define  M10   TMOD_bit.M10
#define  C_T0   TMOD_bit.C_T0
#define  GATE0   TMOD_bit.GATE0
#define  M01   TMOD_bit.M01
#define  M11   TMOD_bit.M11
#define  C_T1   TMOD_bit.C_T1
#define  GATE1   TMOD_bit.GATE1







__sfr __no_init volatile unsigned char RACAP2L @ 0xCA; /* Timer 2 Capture Low */
__sfr __no_init volatile unsigned char RACAP2H @ 0xCB; /* Timer 2 Capture High */
__sfr __no_init volatile unsigned char TL0 @ 0x8A; /* Timer0 Low */
//__sfr __no_init volatile unsigned char EIE @ 0xe8;
//__sfr __no_init volatile unsigned char CKCON @ 0x8e;
//__sfr __no_init volatile unsigned char EICON @ 0xd8;
//__sfr __no_init volatile unsigned char EXIF @ 0x91;
//__sfr __no_init volatile unsigned char EIP @ 0xf8;
//__sfr __no_init volatile unsigned char SCON1 @ 0xc0;
//__sfr __no_init volatile unsigned char SBUF1 @ 0xc1;
//__sfr __no_init volatile unsigned char DPL1 @ 0x84;
//__sfr __no_init volatile unsigned char DPH1 @ 0x85;
//__sfr __no_init volatile unsigned char isfr @ 0xf1;
//__sfr __no_init volatile unsigned char debug @ 0xff;
//__sfr __no_init volatile unsigned char SPC_FNC @ 0x8f;


__sfr __no_init volatile union
{
  unsigned char T2CON; /* Timer 2 Control */
  struct /* Timer 2 Control */
  {
    unsigned char CP_RL2 : 1;
    unsigned char C_T2 : 1;
    unsigned char TR2 : 1;
    unsigned char EXEN2 : 1;
    unsigned char TCLK : 1;
    unsigned char RCLK : 1;
    unsigned char EXF2 : 1;
    unsigned char TF2 : 1;
  } T2CON_bit;
} @ 0xC8;

#define CP_RL2 T2CON_bit.CP_RL2
#define C_T2 T2CON_bit.C_T2
#define TR2 T2CON_bit.TR2
#define EXEN2 T2CON_bit.EXEN2
#define TCLK T2CON_bit.TCLK
#define RCLK T2CON_bit.RCLK
#define EXF2 T2CON_bit.EXF2
#define TF2 T2CON_bit.TF2


__sfr __no_init volatile unsigned char TL1 @ 0x8B; /* Timer1 Low */




/*-------------------------------------------------------------------------
 *   Interrupt
 *-------------------------------------------------------------------------*/

__sfr __no_init volatile union
{
  unsigned char IP; /* Interrupt Priority */
  struct /* Interrupt Priority */
  {
    unsigned char PX0 : 1;
    unsigned char PT0 : 1;
    unsigned char PX1 : 1;
    unsigned char PT1 : 1;
    unsigned char PS : 1;
    unsigned char PT2 : 1;
    unsigned char  : 1;
    unsigned char  : 1;
  } IP_bit;
} @ 0xB8;

#define PX0  IP_bit.PX0
#define PT0  IP_bit.PT0
#define PX1  IP_bit.PX1
#define PT1  IP_bit.PT1
#define PS   IP_bit.PS
#define PT2  IP_bit.PT2




__sfr __no_init volatile union
{
  unsigned char IE; /* Interrupt Enable */
  struct /* Interrupt Enable */
  {
    unsigned char EX0 : 1;
    unsigned char ET0 : 1;
    unsigned char EX1 : 1;
    unsigned char ET1 : 1;
    unsigned char ES : 1;
    unsigned char ET2 : 1;
    unsigned char  : 1;
    unsigned char EA : 1;
  } IE_bit;
} @ 0xA8;


#define EX0  IE_bit.EX0
#define ET0  IE_bit.ET0
#define EX1  IE_bit.EX1
#define ET1  IE_bit.ET1
#define ES   IE_bit.ES
#define ET2  IE_bit.ET2
#define EA   IE_bit.EA

/*-------------------------------------------------------------------------
 *   I/O Port
 *-------------------------------------------------------------------------*/

//__sfr __no_init volatile union
//{
//  unsigned char P1; /* Port 1 */
//  struct /* Port 1 */
//  {
//    unsigned char T2 : 1;
 //   unsigned char T2EX : 1;
 //   unsigned char  : 1;
//    unsigned char  : 1;
//    unsigned char  : 1;
//    unsigned char  : 1;
 //   unsigned char  : 1;
 //   unsigned char  : 1;
 // } P1_bit;
//} @ 0x90;
//__sfr __no_init volatile union
//{
 // unsigned char P0; /* Port 0 */
 // struct /* Port 0 */
 // {
 //   unsigned char P00 : 1;
 //   unsigned char P01 : 1;
 //   unsigned char P02 : 1;
 //   unsigned char P03 : 1;
 //   unsigned char P04 : 1;
 //   unsigned char P05 : 1;
//    unsigned char P06 : 1;
//    unsigned char P07 : 1;
//  } P0_bit;
//} @ 0x80;
//__sfr __no_init volatile union
//{
//  unsigned char P3; /* Port 3 */
//  struct /* Port 3 */
//  {
//    unsigned char RxD : 1;
 //   unsigned char TxD : 1;
  //  unsigned char INT0 : 1;
 //   unsigned char INT1 : 1;
//    unsigned char T0 : 1;
//    unsigned char T1 : 1;
//    unsigned char WR : 1;
//    unsigned char RD : 1;
//  } P3_bit;
//} @ 0xB0;
//__sfr __no_init volatile union
//{
//  unsigned char P2; /* Port 2 */
//  struct /* Port 2 */
//  {
//    unsigned char AD8 : 1;
//    unsigned char AD9 : 1;
//    unsigned char AD10 : 1;
//    unsigned char AD11 : 1;
//    unsigned char AD12 : 1;
//    unsigned char AD13 : 1;
//    unsigned char AD14 : 1;
//    unsigned char AD15 : 1;
//  } P2_bit;
//} @ 0xA0;

/*-------------------------------------------------------------------------
 *   Serial I/O Port
 *-------------------------------------------------------------------------*/

__sfr __no_init volatile unsigned char SBUF @ 0x99; /* Serial Data Buffer */
__sfr __no_init volatile union
{
  unsigned char SCON; /* Serial Control */
  struct /* Serial Control */
  {
    unsigned char RI : 1;
    unsigned char TI : 1;
    unsigned char RB8 : 1;
    unsigned char TB8 : 1;
    unsigned char REN : 1;
    unsigned char SM2 : 1;
    unsigned char SM1 : 1;
    unsigned char SM0 : 1;
  } SCON_bit;
} @ 0x98;

#define RI SCON_bit.RI
#define TI SCON_bit.TI
#define RB8 SCON_bit.RB8
#define TB8 SCON_bit.TB8
#define REN SCON_bit.REN
#define SM2 SCON_bit.SM2
#define SM1 SCON_bit.SM1

#define SM0 SCON_bit.SM0


/*-------------------------------------------------------------------------
 *   System Management
 *-------------------------------------------------------------------------*/

__sfr __no_init volatile union
{
  unsigned char PCON; /* Power Control */
  struct /* Power Control */
  {
    unsigned char IDL : 1;
    unsigned char PD : 1;
    unsigned char GF0 : 1;
    unsigned char GF1 : 1;
    unsigned char POF : 1;
    unsigned char  : 1;
    unsigned char SMOD0 : 1;
    unsigned char SMOD1 : 1;
  } PCON_bit;
} @ 0x87;

#define IDL  PCON_bit.IDL
#define PD  PCON_bit.PD
#define GF0  PCON_bit.GF0
#define GF1  PCON_bit.GF1
#define POF  PCON_bit.POF
#define SMOD0  PCON_bit.SMOD0
#define SMOD1  PCON_bit.SMOD1

/*  8052 Extensions  */
__sfr __no_init volatile unsigned char RCAP2L @ 0xCA;
__sfr __no_init volatile unsigned char RCAP2H @ 0xCB;




/*
 * Interrupt Vectors
 */
//#define extern0 0x03 /* External interrupt 0 */
//#define IE0_int 0x03 /* External interrupt 0 */
//#define timer0 0x0B /* Timer 0 Interrupt */
//#define TF0_int 0x0B /* Timer 0 Interrupt */
//#define extern1 0x13 /* External interrupt 1 */
//#define IE1_int 0x13 /* External interrupt 1 */
//#define timer1 0x1B /* Timer 1 Interrupt */
//#define TF1_int 0x1B /* Timer 1 Interrupt */
//#define sio_ti 0x23 /* Serial Port Interrupt */
//#define sio_ri 0x23 /* Serial Port Interrupt */
//#define TI_int 0x23 /* Serial Port Interrupt */
//#define RI_int 0x23 /* Serial Port Interrupt */

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */

/***************************************************************************
 *   Assembler definitions
 *
 *   The following SFRs are built in in the a8051.exe and can not be
 *   defined explicitly in this file:
 *     ACC,B,PSW,SP,DPL,DPH
 *   The PSW-bits are built-in in the a8051.exe
 *     OV,AC,F0,RS0,RS1,P
 *
 ***************************************************************************/

#ifdef __IAR_SYSTEMS_ASM__


/*-------------------------------------------------------------------------
 *   Timers
 *-------------------------------------------------------------------------*/

TCON DEFINE 0x88 /* Timer Control */
TCON_IT0 DEFINE 0x88.0
TCON_IE0 DEFINE 0x88.1
TCON_IT1 DEFINE 0x88.2
TCON_IE1 DEFINE 0x88.3
TCON_TR0 DEFINE 0x88.4
TCON_TF0 DEFINE 0x88.5
TCON_TR1 DEFINE 0x88.6
TCON_TF1 DEFINE 0x88.7
TMOD DEFINE 0x89 /* Timer Mode */
TL0 DEFINE 0x8A /* Timer0 Low */
TL1 DEFINE 0x8B /* Timer1 Low */
TH0 DEFINE 0x8C /* Timer0 High */
TH1 DEFINE 0x8D /* Timer1 High */
T2CON DEFINE 0xC8 /* Timer 2 Control */
T2CON_CP_RL2 DEFINE 0xC8.0
T2CON_C_T2 DEFINE 0xC8.1
T2CON_TR2 DEFINE 0xC8.2
T2CON_EXEN2 DEFINE 0xC8.3
T2CON_TCLK DEFINE 0xC8.4
T2CON_RCLK DEFINE 0xC8.5
T2CON_EXF2 DEFINE 0xC8.6
T2CON_TF2 DEFINE 0xC8.7
RACAP2L DEFINE 0xCA /* Timer 2 Capture Low */
RACAP2H DEFINE 0xCB /* Timer 2 Capture High */
TL2 DEFINE 0xCC /* Timer2 Low */
TH2 DEFINE 0xCD /* Timer2 High */

/*-------------------------------------------------------------------------
 *   Interrupt
 *-------------------------------------------------------------------------*/

IE DEFINE 0xA8 /* Interrupt Enable */
IE_EX0 DEFINE 0xA8.0
IE_ET0 DEFINE 0xA8.1
IE_EX1 DEFINE 0xA8.2
IE_ET1 DEFINE 0xA8.3
IE_ES DEFINE 0xA8.4
IE_ET2 DEFINE 0xA8.5
IE_EA DEFINE 0xA8.7
IP DEFINE 0xB8 /* Interrupt Priority */
IP_PX0 DEFINE 0xB8.0
IP_PT0 DEFINE 0xB8.1
IP_PX1 DEFINE 0xB8.2
IP_PT1 DEFINE 0xB8.3
IP_PS DEFINE 0xB8.4
IP_PT2 DEFINE 0xB8.5

/*-------------------------------------------------------------------------
 *   I/O Port
 *-------------------------------------------------------------------------*/

P0 DEFINE 0x80 /* Port 0 */
P0_P00 DEFINE 0x80.0
P0_P01 DEFINE 0x80.1
P0_P02 DEFINE 0x80.2
P0_P03 DEFINE 0x80.3
P0_P04 DEFINE 0x80.4
P0_P05 DEFINE 0x80.5
P0_P06 DEFINE 0x80.6
P0_P07 DEFINE 0x80.7
P1 DEFINE 0x90 /* Port 1 */
P1_T2 DEFINE 0x90.0
P1_T2EX DEFINE 0x90.1
P2 DEFINE 0xA0 /* Port 2 */
P2_AD8 DEFINE 0xA0.0
P2_AD9 DEFINE 0xA0.1
P2_AD10 DEFINE 0xA0.2
P2_AD11 DEFINE 0xA0.3
P2_AD12 DEFINE 0xA0.4
P2_AD13 DEFINE 0xA0.5
P2_AD14 DEFINE 0xA0.6
P2_AD15 DEFINE 0xA0.7
P3 DEFINE 0xB0 /* Port 3 */
P3_RxD DEFINE 0xB0.0
P3_TxD DEFINE 0xB0.1
P3_INT0 DEFINE 0xB0.2
P3_INT1 DEFINE 0xB0.3
P3_T0 DEFINE 0xB0.4
P3_T1 DEFINE 0xB0.5
P3_WR DEFINE 0xB0.6
P3_RD DEFINE 0xB0.7

/*-------------------------------------------------------------------------
 *   Serial I/O Port
 *-------------------------------------------------------------------------*/

SCON DEFINE 0x98 /* Serial Control */
SCON_RI DEFINE 0x98.0
SCON_TI DEFINE 0x98.1
SCON_RB8 DEFINE 0x98.2
SCON_TB8 DEFINE 0x98.3
SCON_REN DEFINE 0x98.4
SCON_SM2 DEFINE 0x98.5
SCON_SM1 DEFINE 0x98.6
SCON_SM0 DEFINE 0x98.7
SBUF DEFINE 0x99 /* Serial Data Buffer */

/*-------------------------------------------------------------------------
 *   System Management
 *-------------------------------------------------------------------------*/

PCON DEFINE 0x87 /* Power Control */



#endif /* __IAR_SYSTEMS_ASM__*/
#endif /* IO8052_H */
