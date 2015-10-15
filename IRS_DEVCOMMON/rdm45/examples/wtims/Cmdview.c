/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/

#include "vista.h"
#include "timsres.h"

#define CMDVIEW_C
#include "timsdec.h"

/*** Private Function *********************************************************\

fcn: Cmd_View_ItemsByKeyword() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL Cmd_View_ItemsByKeyword()
{
   FARPROC lpDlgProc;
   short nDlg;

   /* display IDD_KEYWORDS dialog box */

   do
   {

      lpDlgProc = MakeProcInstance(KeyWordsDlgProc, hInst);

      nDlg = DialogBox(hInst, MAKEINTRESOURCE(IDD_KEYWORDS), hWndMain,
                       lpDlgProc);
      FreeProcInstance(lpDlgProc);

      if (nDlg == -1)
      {
         if (MessageBox(hWndMain, TString(IDS_OUTOFMEMORY), szAppName,
                        MB_OUTOFMEMORY) == IDCANCEL)
         {
            return FALSE;
         }
      }
   } while (nDlg == -1);

   return TRUE;
}

/*** Private Function *********************************************************\

fcn: Cmd_View_ItemsByAuthor() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL Cmd_View_ItemsByAuthor()
{
   FARPROC lpDlgProc;
   short nDlg;

   /* display IDD_AUTHORS dialog box */

   do
   {

      lpDlgProc = MakeProcInstance(AuthorsDlgProc, hInst);

      nDlg = DialogBox(hInst, MAKEINTRESOURCE(IDD_AUTHORS), hWndMain,
                       lpDlgProc);
      FreeProcInstance(lpDlgProc);

      if (nDlg == -1)
      {
         if (MessageBox(hWndMain, TString(IDS_OUTOFMEMORY), szAppName,
                        MB_OUTOFMEMORY) == IDCANCEL)
         {
            return FALSE;
         }
      }
   } while (nDlg == -1);

   return TRUE;
}

/* $Revision:   1.2  $ */

/* vpp CMDVIEW.C (Fri Sep 19 11:28:41 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

