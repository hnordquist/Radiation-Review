/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/
#include <stdio.h>
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define DLGLOANS_C
#include "timsdec.h"

short nCurScrollPos;
short nNewScrollPos;

/**** Exported ****************************************************************\

fcn:  BorrowingsDlgProc() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL BorrowingsDlgProc(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   struct borrower BorrowerRec;
   char szTitle[LSZTITLE + 1];
   char szDate[LSZDATE + 1];
   register int i;
   int stat;

   switch (message)
   {
      case WM_INITDIALOG:

         nCurScrollPos = 0;
         nNewScrollPos = 0;

         SetScrollRange(GetDlgItem(hDlg, IDI_SCROLL), SB_CTL, 0, 100, FALSE);
         SetScrollPos(GetDlgItem(hDlg, IDI_SCROLL), SB_CTL, 0, TRUE);

         /* initialize IDI_TITLE, IDI_BORROWER, IDI_BORROWED controls */

         for (stat = d_findfm(LOAN_HISTORY, CURR_DB); stat == S_OKAY;
              stat = d_findnm(LOAN_HISTORY, CURR_DB))
         {

            d_recread(&BorrowerRec, CURR_DB);

            if (BorrowerRec.date_returned == 0L)
            {

               d_findco(LOANED_BOOKS, CURR_DB);

               d_crread(INFO_TITLE, szTitle, CURR_DB);

               sprintf(szDate, "%ld", BorrowerRec.date_borrowed);

               SendMessage(GetDlgItem(hDlg, IDI_BORROWER), WM_SETTEXT,
                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) BorrowerRec.friend);

               SendMessage(GetDlgItem(hDlg, IDI_BORROWED), WM_SETTEXT,
                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szDate);

               SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szTitle);
               break;
            }
         }
         break;

      case WM_COMMAND:
         switch (DB_GET_WM_COMMAND_ID(wParam, lParam))
         {
            case IDI_OK:
               EndDialog(hDlg, TRUE);
               break;

            default:
               return FALSE;
         }
         break;

      case WM_VSCROLL:
         switch (wParam)
         {
            case SB_LINEUP:
               nNewScrollPos = nCurScrollPos - 1;
               break;
            case SB_LINEDOWN:
               nNewScrollPos = nCurScrollPos + 1;
               break;
            case SB_PAGEUP:
               nNewScrollPos = nCurScrollPos - (5 - 1);
               break;
            case SB_PAGEDOWN:
               nNewScrollPos = nCurScrollPos + (5 - 1);
               break;
            case SB_TOP:
               nNewScrollPos = 0;
               break;
            case SB_BOTTOM:
               nNewScrollPos = 100;       /* assumes at most 100 borrowed
                                        * items */
               break;
            case SB_THUMBPOSITION:
               nNewScrollPos = (short)DB_GET_WM_COMMAND_HWND(wParam, lParam);
               break;
            default:
               return 0L;
         }

         /* cannot scroll too far */

         nNewScrollPos = max(nNewScrollPos, 0);
         nNewScrollPos = min(nNewScrollPos, 100);

         if (nNewScrollPos == nCurScrollPos)
         {
            return 0L;
         }
         else
         {
            nCurScrollPos = nNewScrollPos;
         }

         /* initialize IDI_SCROLL control */

         SetScrollPos((HWND)DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam), SB_CTL, nCurScrollPos, TRUE);

         /* initialize IDI_BORROWED, IDI_BORROWER, IDI_TITLE controls */

         i = 0;

         SendMessage(GetDlgItem(hDlg, IDI_BORROWER), WM_SETTEXT,
                     (DB_WPARAM) 0, (DB_LPARAM) 0);

         SendMessage(GetDlgItem(hDlg, IDI_BORROWED), WM_SETTEXT,
                     (DB_WPARAM) 0, (DB_LPARAM) 0);

         SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                     (DB_WPARAM) 0, (DB_LPARAM) 0);

         for (stat = d_findfm(LOAN_HISTORY, CURR_DB); stat == S_OKAY;
              stat = d_findnm(LOAN_HISTORY, CURR_DB))
         {

            if (i++ != nCurScrollPos)
            {
               continue;
            }

            d_recread(&BorrowerRec, CURR_DB);

            if (BorrowerRec.date_returned == 0L)
            {

               d_findco(LOANED_BOOKS, CURR_DB);

               d_crread(INFO_TITLE, szTitle, CURR_DB);

               sprintf(szDate, "%ld", BorrowerRec.date_borrowed);

               SendMessage(GetDlgItem(hDlg, IDI_BORROWER), WM_SETTEXT,
                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) BorrowerRec.friend);

               SendMessage(GetDlgItem(hDlg, IDI_BORROWED), WM_SETTEXT,
                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szDate);

               SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szTitle);
               break;
            }
         }
         break;

      default:
         return FALSE;
   }

   return TRUE;
}

/* $Revision:   1.2  $ */

/* vpp DLGLOANS.C (Fri Sep 19 11:28:41 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

