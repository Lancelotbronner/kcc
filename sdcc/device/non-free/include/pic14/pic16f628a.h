//
// Register Declarations for Microchip 16F628A Processor
//
//
// This header file was automatically generated by:
//
//	inc2h.pl V4850
//
//	Copyright (c) 2002, Kevin L. Pauba, All Rights Reserved
//
//	SDCC is licensed under the GNU Public license (GPL) v2.  Note that
//	this license covers the code to the compiler and other executables,
//	but explicitly does not cover any code or objects generated by sdcc.
//	We have not yet decided on a license for the run time libraries, but
//	it will not put any requirements on code linked against it. See:
// 
//	http://www.gnu.org/copyleft/gpl/html
//
//	See http://sdcc.sourceforge.net/ for the latest information on sdcc.
//
// 
#ifndef P16F628A_H
#define P16F628A_H

//
// Register addresses.
//
#define INDF_ADDR	0x0000
#define TMR0_ADDR	0x0001
#define PCL_ADDR	0x0002
#define STATUS_ADDR	0x0003
#define FSR_ADDR	0x0004
#define PORTA_ADDR	0x0005
#define PORTB_ADDR	0x0006
#define PCLATH_ADDR	0x000A
#define INTCON_ADDR	0x000B
#define PIR1_ADDR	0x000C
#define TMR1L_ADDR	0x000E
#define TMR1H_ADDR	0x000F
#define T1CON_ADDR	0x0010
#define TMR2_ADDR	0x0011
#define T2CON_ADDR	0x0012
#define CCPR1L_ADDR	0x0015
#define CCPR1H_ADDR	0x0016
#define CCP1CON_ADDR	0x0017
#define RCSTA_ADDR	0x0018
#define TXREG_ADDR	0x0019
#define RCREG_ADDR	0x001A
#define CMCON_ADDR	0x001F
#define OPTION_REG_ADDR	0x0081
#define TRISA_ADDR	0x0085
#define TRISB_ADDR	0x0086
#define PIE1_ADDR	0x008C
#define PCON_ADDR	0x008E
#define PR2_ADDR	0x0092
#define TXSTA_ADDR	0x0098
#define SPBRG_ADDR	0x0099
#define EEDATA_ADDR	0x009A
#define EEADR_ADDR	0x009B
#define EECON1_ADDR	0x009C
#define EECON2_ADDR	0x009D
#define VRCON_ADDR	0x009F

//
// Memory organization.
//



//         LIST
// P16F628A.INC  Standard Header File, Version 1.10    Microchip Technology, Inc.
//         NOLIST

// This header file defines configurations, registers, and other useful bits of
// information for the PIC16F628A microcontroller.  These names are taken to match
// the data sheets as closely as possible.

// Note that the processor must be selected before this file is
// included.  The processor may be selected the following ways:

//       1. Command line switch:
//               C:\ MPASM MYFILE.ASM /PIC16F628A
//       2. LIST directive in the source file
//               LIST   P=PIC16F628A
//       3. Processor Type entry in the MPASM full-screen interface

//==========================================================================
//
//       Revision History
//
//==========================================================================

//Rev:   Date:    Reason:
//1.01   14 Nov 2002 Updated to reflect BOD terminology changed to BOR
//1.00   22 Aug 2002 Initial Release

//==========================================================================
//
//       Verify Processor
//
//==========================================================================

//        IFNDEF __16F628A
//            MESSG "Processor-header file mismatch.  Verify selected processor."
//         ENDIF

//==========================================================================
//
//       Register Definitions
//
//==========================================================================

#define W                    0x0000
#define F                    0x0001

//----- Register Files------------------------------------------------------

extern __sfr  __at (INDF_ADDR)                    INDF;
extern __sfr  __at (TMR0_ADDR)                    TMR0;
extern __sfr  __at (PCL_ADDR)                     PCL;
extern __sfr  __at (STATUS_ADDR)                  STATUS;
extern __sfr  __at (FSR_ADDR)                     FSR;
extern __sfr  __at (PORTA_ADDR)                   PORTA;
extern __sfr  __at (PORTB_ADDR)                   PORTB;
extern __sfr  __at (PCLATH_ADDR)                  PCLATH;
extern __sfr  __at (INTCON_ADDR)                  INTCON;
extern __sfr  __at (PIR1_ADDR)                    PIR1;
extern __sfr  __at (TMR1L_ADDR)                   TMR1L;
extern __sfr  __at (TMR1H_ADDR)                   TMR1H;
extern __sfr  __at (T1CON_ADDR)                   T1CON;
extern __sfr  __at (TMR2_ADDR)                    TMR2;
extern __sfr  __at (T2CON_ADDR)                   T2CON;
extern __sfr  __at (CCPR1L_ADDR)                  CCPR1L;
extern __sfr  __at (CCPR1H_ADDR)                  CCPR1H;
extern __sfr  __at (CCP1CON_ADDR)                 CCP1CON;
extern __sfr  __at (RCSTA_ADDR)                   RCSTA;
extern __sfr  __at (TXREG_ADDR)                   TXREG;
extern __sfr  __at (RCREG_ADDR)                   RCREG;
extern __sfr  __at (CMCON_ADDR)                   CMCON;

extern __sfr  __at (OPTION_REG_ADDR)              OPTION_REG;
extern __sfr  __at (TRISA_ADDR)                   TRISA;
extern __sfr  __at (TRISB_ADDR)                   TRISB;
extern __sfr  __at (PIE1_ADDR)                    PIE1;
extern __sfr  __at (PCON_ADDR)                    PCON;
extern __sfr  __at (PR2_ADDR)                     PR2;
extern __sfr  __at (TXSTA_ADDR)                   TXSTA;
extern __sfr  __at (SPBRG_ADDR)                   SPBRG;
extern __sfr  __at (EEDATA_ADDR)                  EEDATA;
extern __sfr  __at (EEADR_ADDR)                   EEADR;
extern __sfr  __at (EECON1_ADDR)                  EECON1;
extern __sfr  __at (EECON2_ADDR)                  EECON2;
extern __sfr  __at (VRCON_ADDR)                   VRCON;

//----- STATUS Bits --------------------------------------------------------


//----- INTCON Bits --------------------------------------------------------


//----- PIR1 Bits ----------------------------------------------------------


//----- T1CON Bits ---------------------------------------------------------

//----- T2CON Bits ---------------------------------------------------------

//----- CCP1CON Bits ---------------------------------------------------------

//----- RCSTA Bits ---------------------------------------------------------

//----- CMCON Bits ---------------------------------------------------------


//----- OPTION Bits --------------------------------------------------------


//----- PIE1 Bits ----------------------------------------------------------


//----- PCON Bits ----------------------------------------------------------


//----- TXSTA Bits ----------------------------------------------------------

//----- EECON1 Bits ---------------------------------------------------------

//----- VRCON Bits ---------------------------------------------------------


//==========================================================================
//
//       RAM Definition
//
//==========================================================================

//     __MAXRAM H'01FF'
//     __BADRAM H'07'-H'09', H'0D', H'13'-H'14', H'1B'-H'1E'
//     __BADRAM H'87'-H'89', H'8D', H'8F'-H'91', H'93'-H'97', H'9E'
//     __BADRAM H'105', H'107'-H'109', H'10C'-H'11F', H'150'-H'16F'
//     __BADRAM H'185', H'187'-H'189', H'18C'-H'1EF'

//==========================================================================
//
//       Configuration Bits
//
//==========================================================================

#define _BODEN_ON            0x3FFF //Backwards compatability to 16F62X
#define _BODEN_OFF           0x3FBF //Backwards compatability to 16F62X
#define _BOREN_ON            0x3FFF
#define _BOREN_OFF           0x3FBF
#define _CP_ON               0x1FFF
#define _CP_OFF              0x3FFF
#define _DATA_CP_ON          0x3EFF
#define _DATA_CP_OFF         0x3FFF
#define _PWRTE_OFF           0x3FFF
#define _PWRTE_ON            0x3FF7
#define _WDT_ON              0x3FFF
#define _WDT_OFF             0x3FFB
#define _LVP_ON              0x3FFF
#define _LVP_OFF             0x3F7F
#define _MCLRE_ON            0x3FFF
#define _MCLRE_OFF           0x3FDF
#define _RC_OSC_CLKOUT       0x3FFF
#define _RC_OSC_NOCLKOUT     0x3FFE
#define _ER_OSC_CLKOUT       0x3FFF //Backwards compatability to 16F62X
#define _ER_OSC_NOCLKOUT     0x3FFE //Backwards compatability to 16F62X
#define _INTOSC_OSC_CLKOUT   0x3FFD
#define _INTOSC_OSC_NOCLKOUT 0x3FFC    
#define _INTRC_OSC_CLKOUT    0x3FFD //Backwards compatability to 16F62X
#define _INTRC_OSC_NOCLKOUT  0x3FFC //Backwards compatability to 16F62X
#define _EXTCLK_OSC          0x3FEF
#define _HS_OSC              0x3FEE
#define _XT_OSC              0x3FED
#define _LP_OSC              0x3FEC

//         LIST


// ----- CCP1CON bits --------------------
typedef union {
  struct {
    unsigned char CCP1M0:1;
    unsigned char CCP1M1:1;
    unsigned char CCP1M2:1;
    unsigned char CCP1M3:1;
    unsigned char CCP1Y:1;
    unsigned char CCP1X:1;
    unsigned char :1;
    unsigned char :1;
  };
} __CCP1CONbits_t;
extern volatile __CCP1CONbits_t __at(CCP1CON_ADDR) CCP1CONbits;

// ----- CMCON bits --------------------
typedef union {
  struct {
    unsigned char CM0:1;
    unsigned char CM1:1;
    unsigned char CM2:1;
    unsigned char CIS:1;
    unsigned char C1INV:1;
    unsigned char C2INV:1;
    unsigned char C1OUT:1;
    unsigned char C2OUT:1;
  };
} __CMCONbits_t;
extern volatile __CMCONbits_t __at(CMCON_ADDR) CMCONbits;

// ----- EECON1 bits --------------------
typedef union {
  struct {
    unsigned char RD:1;
    unsigned char WR:1;
    unsigned char WREN:1;
    unsigned char WRERR:1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
  };
} __EECON1bits_t;
extern volatile __EECON1bits_t __at(EECON1_ADDR) EECON1bits;

// ----- INTCON bits --------------------
typedef union {
  struct {
    unsigned char RBIF:1;
    unsigned char INTF:1;
    unsigned char T0IF:1;
    unsigned char RBIE:1;
    unsigned char INTE:1;
    unsigned char T0IE:1;
    unsigned char PEIE:1;
    unsigned char GIE:1;
  };
} __INTCONbits_t;
extern volatile __INTCONbits_t __at(INTCON_ADDR) INTCONbits;

// ----- OPTION_REG bits --------------------
typedef union {
  struct {
    unsigned char PS0:1;
    unsigned char PS1:1;
    unsigned char PS2:1;
    unsigned char PSA:1;
    unsigned char T0SE:1;
    unsigned char T0CS:1;
    unsigned char INTEDG:1;
    unsigned char NOT_RBPU:1;
  };
} __OPTION_REGbits_t;
extern volatile __OPTION_REGbits_t __at(OPTION_REG_ADDR) OPTION_REGbits;

// ----- PCON bits --------------------
typedef union {
  struct {
    unsigned char NOT_BO:1;
    unsigned char NOT_POR:1;
    unsigned char :1;
    unsigned char OSCF:1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
  };
  struct {
    unsigned char NOT_BOR:1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
  };
  struct {
    unsigned char NOT_BOD:1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
    unsigned char :1;
  };
} __PCONbits_t;
extern volatile __PCONbits_t __at(PCON_ADDR) PCONbits;

// ----- PIE1 bits --------------------
typedef union {
  struct {
    unsigned char TMR1IE:1;
    unsigned char TMR2IE:1;
    unsigned char CCP1IE:1;
    unsigned char :1;
    unsigned char TXIE:1;
    unsigned char RCIE:1;
    unsigned char CMIE:1;
    unsigned char EEIE:1;
  };
} __PIE1bits_t;
extern volatile __PIE1bits_t __at(PIE1_ADDR) PIE1bits;

// ----- PIR1 bits --------------------
typedef union {
  struct {
    unsigned char TMR1IF:1;
    unsigned char TMR2IF:1;
    unsigned char CCP1IF:1;
    unsigned char :1;
    unsigned char TXIF:1;
    unsigned char RCIF:1;
    unsigned char CMIF:1;
    unsigned char EEIF:1;
  };
} __PIR1bits_t;
extern volatile __PIR1bits_t __at(PIR1_ADDR) PIR1bits;

// ----- PORTA bits --------------------
typedef union {
  struct {
    unsigned char RA0:1;
    unsigned char RA1:1;
    unsigned char RA2:1;
    unsigned char RA3:1;
    unsigned char RA4:1;
    unsigned char RA5:1;
    unsigned char :1;
    unsigned char :1;
  };
} __PORTAbits_t;
extern volatile __PORTAbits_t __at(PORTA_ADDR) PORTAbits;

// ----- PORTB bits --------------------
typedef union {
  struct {
    unsigned char RB0:1;
    unsigned char RB1:1;
    unsigned char RB2:1;
    unsigned char RB3:1;
    unsigned char RB4:1;
    unsigned char RB5:1;
    unsigned char RB6:1;
    unsigned char RB7:1;
  };
} __PORTBbits_t;
extern volatile __PORTBbits_t __at(PORTB_ADDR) PORTBbits;

// ----- RCSTA bits --------------------
typedef union {
  struct {
    unsigned char RX9D:1;
    unsigned char OERR:1;
    unsigned char FERR:1;
    unsigned char ADEN:1;
    unsigned char CREN:1;
    unsigned char SREN:1;
    unsigned char RX9:1;
    unsigned char SPEN:1;
  };
} __RCSTAbits_t;
extern volatile __RCSTAbits_t __at(RCSTA_ADDR) RCSTAbits;

// ----- STATUS bits --------------------
typedef union {
  struct {
    unsigned char C:1;
    unsigned char DC:1;
    unsigned char Z:1;
    unsigned char NOT_PD:1;
    unsigned char NOT_TO:1;
    unsigned char RP0:1;
    unsigned char RP1:1;
    unsigned char IRP:1;
  };
} __STATUSbits_t;
extern volatile __STATUSbits_t __at(STATUS_ADDR) STATUSbits;

// ----- T1CON bits --------------------
typedef union {
  struct {
    unsigned char TMR1ON:1;
    unsigned char TMR1CS:1;
    unsigned char NOT_T1SYNC:1;
    unsigned char T1OSCEN:1;
    unsigned char T1CKPS0:1;
    unsigned char T1CKPS1:1;
    unsigned char :1;
    unsigned char :1;
  };
} __T1CONbits_t;
extern volatile __T1CONbits_t __at(T1CON_ADDR) T1CONbits;

// ----- T2CON bits --------------------
typedef union {
  struct {
    unsigned char T2CKPS0:1;
    unsigned char T2CKPS1:1;
    unsigned char TMR2ON:1;
    unsigned char TOUTPS0:1;
    unsigned char TOUTPS1:1;
    unsigned char TOUTPS2:1;
    unsigned char TOUTPS3:1;
    unsigned char :1;
  };
} __T2CONbits_t;
extern volatile __T2CONbits_t __at(T2CON_ADDR) T2CONbits;

// ----- TRISA bits --------------------
typedef union {
  struct {
    unsigned char TRISA0:1;
    unsigned char TRISA1:1;
    unsigned char TRISA2:1;
    unsigned char TRISA3:1;
    unsigned char TRISA4:1;
    unsigned char TRISA5:1;
    unsigned char :1;
    unsigned char :1;
  };
} __TRISAbits_t;
extern volatile __TRISAbits_t __at(TRISA_ADDR) TRISAbits;

// ----- TRISB bits --------------------
typedef union {
  struct {
    unsigned char TRISB0:1;
    unsigned char TRISB1:1;
    unsigned char TRISB2:1;
    unsigned char TRISB3:1;
    unsigned char TRISB4:1;
    unsigned char TRISB5:1;
    unsigned char TRISB6:1;
    unsigned char TRISB7:1;
  };
} __TRISBbits_t;
extern volatile __TRISBbits_t __at(TRISB_ADDR) TRISBbits;

// ----- TXSTA bits --------------------
typedef union {
  struct {
    unsigned char TX9D:1;
    unsigned char TRMT:1;
    unsigned char BRGH:1;
    unsigned char :1;
    unsigned char SYNC:1;
    unsigned char TXEN:1;
    unsigned char TX9:1;
    unsigned char CSRC:1;
  };
} __TXSTAbits_t;
extern volatile __TXSTAbits_t __at(TXSTA_ADDR) TXSTAbits;

// ----- VRCON bits --------------------
typedef union {
  struct {
    unsigned char VR0:1;
    unsigned char VR1:1;
    unsigned char VR2:1;
    unsigned char VR3:1;
    unsigned char :1;
    unsigned char VRR:1;
    unsigned char VROE:1;
    unsigned char VREN:1;
  };
} __VRCONbits_t;
extern volatile __VRCONbits_t __at(VRCON_ADDR) VRCONbits;


#ifndef NO_BIT_DEFINES

#define CCP1M0               CCP1CONbits.CCP1M0             /* bit 0 */
#define CCP1M1               CCP1CONbits.CCP1M1             /* bit 1 */
#define CCP1M2               CCP1CONbits.CCP1M2             /* bit 2 */
#define CCP1M3               CCP1CONbits.CCP1M3             /* bit 3 */
#define CCP1Y                CCP1CONbits.CCP1Y              /* bit 4 */
#define CCP1X                CCP1CONbits.CCP1X              /* bit 5 */

#define CM0                  CMCONbits.CM0                  /* bit 0 */
#define CM1                  CMCONbits.CM1                  /* bit 1 */
#define CM2                  CMCONbits.CM2                  /* bit 2 */
#define CIS                  CMCONbits.CIS                  /* bit 3 */
#define C1INV                CMCONbits.C1INV                /* bit 4 */
#define C2INV                CMCONbits.C2INV                /* bit 5 */
#define C1OUT                CMCONbits.C1OUT                /* bit 6 */
#define C2OUT                CMCONbits.C2OUT                /* bit 7 */

#define RD                   EECON1bits.RD                  /* bit 0 */
#define WR                   EECON1bits.WR                  /* bit 1 */
#define WREN                 EECON1bits.WREN                /* bit 2 */
#define WRERR                EECON1bits.WRERR               /* bit 3 */

#define RBIF                 INTCONbits.RBIF                /* bit 0 */
#define INTF                 INTCONbits.INTF                /* bit 1 */
#define T0IF                 INTCONbits.T0IF                /* bit 2 */
#define RBIE                 INTCONbits.RBIE                /* bit 3 */
#define INTE                 INTCONbits.INTE                /* bit 4 */
#define T0IE                 INTCONbits.T0IE                /* bit 5 */
#define PEIE                 INTCONbits.PEIE                /* bit 6 */
#define GIE                  INTCONbits.GIE                 /* bit 7 */

#define PS0                  OPTION_REGbits.PS0             /* bit 0 */
#define PS1                  OPTION_REGbits.PS1             /* bit 1 */
#define PS2                  OPTION_REGbits.PS2             /* bit 2 */
#define PSA                  OPTION_REGbits.PSA             /* bit 3 */
#define T0SE                 OPTION_REGbits.T0SE            /* bit 4 */
#define T0CS                 OPTION_REGbits.T0CS            /* bit 5 */
#define INTEDG               OPTION_REGbits.INTEDG          /* bit 6 */
#define NOT_RBPU             OPTION_REGbits.NOT_RBPU        /* bit 7 */

#define NOT_BO               PCONbits.NOT_BO                /* bit 0 */
#define NOT_BOD              PCONbits.NOT_BOD               /* bit 0 */
#define NOT_BOR              PCONbits.NOT_BOR               /* bit 0 */
#define NOT_POR              PCONbits.NOT_POR               /* bit 1 */
#define OSCF                 PCONbits.OSCF                  /* bit 3 */

#define TMR1IE               PIE1bits.TMR1IE                /* bit 0 */
#define TMR2IE               PIE1bits.TMR2IE                /* bit 1 */
#define CCP1IE               PIE1bits.CCP1IE                /* bit 2 */
#define TXIE                 PIE1bits.TXIE                  /* bit 4 */
#define RCIE                 PIE1bits.RCIE                  /* bit 5 */
#define CMIE                 PIE1bits.CMIE                  /* bit 6 */
#define EEIE                 PIE1bits.EEIE                  /* bit 7 */

#define TMR1IF               PIR1bits.TMR1IF                /* bit 0 */
#define TMR2IF               PIR1bits.TMR2IF                /* bit 1 */
#define CCP1IF               PIR1bits.CCP1IF                /* bit 2 */
#define TXIF                 PIR1bits.TXIF                  /* bit 4 */
#define RCIF                 PIR1bits.RCIF                  /* bit 5 */
#define CMIF                 PIR1bits.CMIF                  /* bit 6 */
#define EEIF                 PIR1bits.EEIF                  /* bit 7 */

#define RA0                  PORTAbits.RA0                  /* bit 0 */
#define RA1                  PORTAbits.RA1                  /* bit 1 */
#define RA2                  PORTAbits.RA2                  /* bit 2 */
#define RA3                  PORTAbits.RA3                  /* bit 3 */
#define RA4                  PORTAbits.RA4                  /* bit 4 */
#define RA5                  PORTAbits.RA5                  /* bit 5 */

#define RB0                  PORTBbits.RB0                  /* bit 0 */
#define RB1                  PORTBbits.RB1                  /* bit 1 */
#define RB2                  PORTBbits.RB2                  /* bit 2 */
#define RB3                  PORTBbits.RB3                  /* bit 3 */
#define RB4                  PORTBbits.RB4                  /* bit 4 */
#define RB5                  PORTBbits.RB5                  /* bit 5 */
#define RB6                  PORTBbits.RB6                  /* bit 6 */
#define RB7                  PORTBbits.RB7                  /* bit 7 */

#define RX9D                 RCSTAbits.RX9D                 /* bit 0 */
#define OERR                 RCSTAbits.OERR                 /* bit 1 */
#define FERR                 RCSTAbits.FERR                 /* bit 2 */
#define ADEN                 RCSTAbits.ADEN                 /* bit 3 */
#define CREN                 RCSTAbits.CREN                 /* bit 4 */
#define SREN                 RCSTAbits.SREN                 /* bit 5 */
#define RX9                  RCSTAbits.RX9                  /* bit 6 */
#define SPEN                 RCSTAbits.SPEN                 /* bit 7 */

#define C                    STATUSbits.C                   /* bit 0 */
#define DC                   STATUSbits.DC                  /* bit 1 */
#define Z                    STATUSbits.Z                   /* bit 2 */
#define NOT_PD               STATUSbits.NOT_PD              /* bit 3 */
#define NOT_TO               STATUSbits.NOT_TO              /* bit 4 */
#define RP0                  STATUSbits.RP0                 /* bit 5 */
#define RP1                  STATUSbits.RP1                 /* bit 6 */
#define IRP                  STATUSbits.IRP                 /* bit 7 */

#define TMR1ON               T1CONbits.TMR1ON               /* bit 0 */
#define TMR1CS               T1CONbits.TMR1CS               /* bit 1 */
#define NOT_T1SYNC           T1CONbits.NOT_T1SYNC           /* bit 2 */
#define T1OSCEN              T1CONbits.T1OSCEN              /* bit 3 */
#define T1CKPS0              T1CONbits.T1CKPS0              /* bit 4 */
#define T1CKPS1              T1CONbits.T1CKPS1              /* bit 5 */

#define T2CKPS0              T2CONbits.T2CKPS0              /* bit 0 */
#define T2CKPS1              T2CONbits.T2CKPS1              /* bit 1 */
#define TMR2ON               T2CONbits.TMR2ON               /* bit 2 */
#define TOUTPS0              T2CONbits.TOUTPS0              /* bit 3 */
#define TOUTPS1              T2CONbits.TOUTPS1              /* bit 4 */
#define TOUTPS2              T2CONbits.TOUTPS2              /* bit 5 */
#define TOUTPS3              T2CONbits.TOUTPS3              /* bit 6 */

#define TRISA0               TRISAbits.TRISA0               /* bit 0 */
#define TRISA1               TRISAbits.TRISA1               /* bit 1 */
#define TRISA2               TRISAbits.TRISA2               /* bit 2 */
#define TRISA3               TRISAbits.TRISA3               /* bit 3 */
#define TRISA4               TRISAbits.TRISA4               /* bit 4 */
#define TRISA5               TRISAbits.TRISA5               /* bit 5 */

#define TRISB0               TRISBbits.TRISB0               /* bit 0 */
#define TRISB1               TRISBbits.TRISB1               /* bit 1 */
#define TRISB2               TRISBbits.TRISB2               /* bit 2 */
#define TRISB3               TRISBbits.TRISB3               /* bit 3 */
#define TRISB4               TRISBbits.TRISB4               /* bit 4 */
#define TRISB5               TRISBbits.TRISB5               /* bit 5 */
#define TRISB6               TRISBbits.TRISB6               /* bit 6 */
#define TRISB7               TRISBbits.TRISB7               /* bit 7 */

#define TX9D                 TXSTAbits.TX9D                 /* bit 0 */
#define TRMT                 TXSTAbits.TRMT                 /* bit 1 */
#define BRGH                 TXSTAbits.BRGH                 /* bit 2 */
#define SYNC                 TXSTAbits.SYNC                 /* bit 4 */
#define TXEN                 TXSTAbits.TXEN                 /* bit 5 */
#define TX9                  TXSTAbits.TX9                  /* bit 6 */
#define CSRC                 TXSTAbits.CSRC                 /* bit 7 */

#define VR0                  VRCONbits.VR0                  /* bit 0 */
#define VR1                  VRCONbits.VR1                  /* bit 1 */
#define VR2                  VRCONbits.VR2                  /* bit 2 */
#define VR3                  VRCONbits.VR3                  /* bit 3 */
#define VRR                  VRCONbits.VRR                  /* bit 5 */
#define VROE                 VRCONbits.VROE                 /* bit 6 */
#define VREN                 VRCONbits.VREN                 /* bit 7 */
#endif /* NO_BIT_DEFINES */

#ifndef NO_LEGACY_NAMES
#define CCP1CON_bits         CCP1CONbits
#define CMCON_bits           CMCONbits
#define EECON1_bits          EECON1bits
#define INTCON_bits          INTCONbits
#define OPTION_REG_bits      OPTION_REGbits
#define PCON_bits            PCONbits
#define PIE1_bits            PIE1bits
#define PIR1_bits            PIR1bits
#define PORTA_bits           PORTAbits
#define PORTB_bits           PORTBbits
#define RCSTA_bits           RCSTAbits
#define STATUS_bits          STATUSbits
#define T1CON_bits           T1CONbits
#define T2CON_bits           T2CONbits
#define TRISA_bits           TRISAbits
#define TRISB_bits           TRISBbits
#define TXSTA_bits           TXSTAbits
#define VRCON_bits           VRCONbits
#endif /* NO_LEGACY_NAMES */

#endif
