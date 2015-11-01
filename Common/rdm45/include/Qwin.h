/*----------------------------------------------------------------------
   qwin.h - Microsoft Windows includes.

   Copyright (c) 1995, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef _INC_WINDOWS    /* #defined if windows.h has been included */

#ifndef NO_WIN_H

#include "vwin.h"       /* will include windows.h */

#else    /* only define what we need in order to miminize compile time */

#define PM_REMOVE   0x0001

typedef struct tagMSG
{
   unsigned int   hwnd;
   unsigned int   message;
   unsigned int   wParam;
   long           lParam;
   unsigned long  time;
   struct {short x, y;} pt;
} MSG;

unsigned int far pascal PeekMessage(MSG far *, unsigned, unsigned, unsigned,
                                    unsigned);
unsigned int far pascal TranslateMessage(MSG far *);
long         far pascal DispatchMessage(MSG far *);

int _far _cdecl wsprintf(char _far *, char _far *,...);
int _far _pascal MessageBox(unsigned int, char _far *, char _far *,
                           unsigned int);
unsigned int _far _pascal GetFocus(void);
int _far _pascal wvsprintf(char _far *, char _far *, char _far *);

#ifndef OF_SHARE_EXCLUSIVE 
#define OF_SHARE_EXCLUSIVE    0x0010
#endif
#ifndef OF_SHARE_DENY_WRITE 
#define OF_SHARE_DENY_WRITE   0x0020
#endif
#ifndef OF_SHARE_DENY_READ 
#define OF_SHARE_DENY_READ    0x0030
#endif
#ifndef OF_SHARE_DENY_NONE 
#define OF_SHARE_DENY_NONE    0x0040
#endif

#endif

#endif  /* _INC_WINDOWS */

/* $Revision:   1.3  $ */

/* vpp QWIN.H (Fri Sep 19 11:56:12 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

