/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/
#include <stdio.h>
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define DLGKEYS_C
#include "timsdec.h"

extern BOOL bViewByKeyWord;

/**** Exported ****************************************************************\

fcn:  KeyWordsDlgProc() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL KeyWordsDlgProc(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   char szKeyWord[LSZKEYWORD + 1];
   FARPROC lpDlgProc;
   short nDlg;
   short nSel;
   int stat;

   switch (message)
   {
      case WM_INITDIALOG:

         /* initialize IDI_LIST control */

         for (stat = d_keyfrst(KWORD, CURR_DB); stat == S_OKAY;
              stat = d_keynext(KWORD, CURR_DB))
         {

            d_keyread(szKeyWord);

            SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_ADDSTRING,
                        (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szKeyWord);
         }

         /* select first item in the list */

         SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_SETCURSEL,
                     (DB_WPARAM) 0, (DB_LPARAM) 0L);

         /* is a listbox item selected? */

         nSel = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCURSEL,
                                    (DB_WPARAM) 0, (DB_LPARAM) 0L);

         if (nSel == LB_ERR)
         {
            EnableWindow(GetDlgItem(hDlg, IDI_VIEW), FALSE);
         }
         else
         {
            EnableWindow(GetDlgItem(hDlg, IDI_VIEW), TRUE);
         }

         /* set global flag (used by ViewInfoDlgProc()) */

         bViewByKeyWord = TRUE;
         break;

      case WM_COMMAND:
         switch (DB_GET_WM_COMMAND_ID(wParam, lParam))
         {
            case IDI_VIEW:

               /* get listbox selection */

               nSel = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCURSEL,
                                          (DB_WPARAM) 0, (DB_LPARAM) 0L);
               if (nSel == LB_ERR)
                  return TRUE;

               /* display IDD_VIEWINFO dialog box */

               do
               {

                  lpDlgProc = MakeProcInstance(ViewInfoDlgProc, hInst);

                  nDlg = DialogBox(hInst, MAKEINTRESOURCE(IDD_VIEWINFO), hDlg,
                                   lpDlgProc);
                  FreeProcInstance(lpDlgProc);

                  if (nDlg == -1)
                  {
                     if (MessageBox(hDlg, TString(IDS_OUTOFMEMORY), szAppName,
                                    MB_OUTOFMEMORY) == IDCANCEL)
                     {
                        return FALSE;
                     }
                  }
               } while (nDlg == -1);
               break;

            case IDI_CANCEL:
               EndDialog(hDlg, FALSE);
               break;

            case IDI_LIST:
               switch (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam))
               {
                  case LBN_DBLCLK:
                     PostMessage(hDlg, WM_COMMAND, (DB_WPARAM) IDI_VIEW, (DB_LPARAM) 0L);
                     break;

                  default:

                     /* is a listbox item selected? */

                     nSel = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCURSEL,
                                                (DB_WPARAM) 0, (DB_LPARAM) 0L);

                     if (nSel == LB_ERR)
                     {
                        EnableWindow(GetDlgItem(hDlg, IDI_VIEW), FALSE);
                     }
                     else
                     {
                        EnableWindow(GetDlgItem(hDlg, IDI_VIEW), TRUE);
                     }
                     break;
               }
               break;

            default:
               return FALSE;
         }
         break;

      default:
         return FALSE;
   }

   return TRUE;
}

/* $Revision:   1.3  $ */

/* vpp DLGKEYS.C (Fri Sep 19 11:28:41 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

