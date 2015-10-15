/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define DLGRET_C
#include "timsdec.h"

/**** Exported ****************************************************************\

fcn:  ReturnBookDlgProc() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL ReturnBookDlgProc(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   struct info InfoRec;
   struct borrower BorrowerRec;
   char szIDCode[LSZIDCODE + 1];
   char szAuthor[LSZAUTHOR + 1];
   char szBorrower[LSZNAME + 1];
   char szBorrowed[LSZDATE + 1];
   char szReturned[LSZDATE + 1];
   long bdate;
   short nSel;
   int stat;

   switch (message)
   {
      case WM_INITDIALOG:

         /* initialize IDI_LIST control */

         for (stat = d_keyfrst(ID_CODE, CURR_DB); stat == S_OKAY;
              stat = d_keynext(ID_CODE, CURR_DB))
         {

            d_recread(&InfoRec, CURR_DB);

            SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_ADDSTRING,
                        (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.id_code);
         }

         /* disable IDI_OK */

         EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);
         break;

      case WM_COMMAND:
         switch (DB_GET_WM_COMMAND_ID(wParam, lParam))
         {
            case IDI_OK:

               /* get listbox selection */

               nSel = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCURSEL,
                                          (DB_WPARAM) 0, (DB_LPARAM) 0L);
               if (nSel == LB_ERR)
                  return TRUE;

               SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETTEXT,
                           (DB_WPARAM) nSel, (DB_LPARAM) (LPSTR) szIDCode);

               /* is the selected item in the database? */

               if (d_keyfind(ID_CODE, szIDCode, CURR_DB) == S_NOTFOUND)
               {
                  MessageBox(hDlg, "Selected item is not on file.", szAppName,
                             MB_OK | MB_ICONASTERISK);
                  return TRUE;
               }

               d_setor(LOANED_BOOKS, CURR_DB);

               /* get IDI_BORROWER && IDI_RETURNED */

               SendMessage(GetDlgItem(hDlg, IDI_BORROWER), WM_GETTEXT,
                           (DB_WPARAM) sizeof(szBorrower), (DB_LPARAM) (LPSTR) szBorrower);

               SendMessage(GetDlgItem(hDlg, IDI_RETURNED), WM_GETTEXT,
                           (DB_WPARAM) sizeof(szReturned), (DB_LPARAM) (LPSTR) szReturned);

               while (d_findnm(LOANED_BOOKS, CURR_DB) == S_OKAY)
               {

                  d_recread(&BorrowerRec, CURR_DB);

                  if (!strcmp(BorrowerRec.friend, szBorrower))
                  {

                     if (BorrowerRec.date_returned == 0L)
                     {

                        sscanf(szReturned, "%ld", &bdate);

                        d_crwrite(DATE_RETURNED, &bdate, CURR_DB);

                        EndDialog(hDlg, TRUE);
                        return TRUE;
                     }
                  }
               }

               MessageBox(hDlg, "Borrower not on file.", szAppName,
                          MB_OK | MB_ICONASTERISK);
               break;

            case IDI_CANCEL:
               EndDialog(hDlg, FALSE);
               break;

            case IDI_RETURNED:
            case IDI_BORROWER:
               if (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam) == EN_CHANGE)
               {
                  if ((BOOL) SendMessage(GetDlgItem(hDlg, IDI_BORROWER),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (BOOL) SendMessage(GetDlgItem(hDlg, IDI_RETURNED),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (BOOL) SendMessage(GetDlgItem(hDlg, IDI_LIST),
                                         LB_GETCURSEL, (DB_WPARAM) 0, (DB_LPARAM) 0L) != LB_ERR)
                  {
                     EnableWindow(GetDlgItem(hDlg, IDI_OK), TRUE);
                  }
                  else
                  {
                     EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);
                  }
               }
               break;

            case IDI_LIST:
               switch (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam))
               {
                  case LBN_DBLCLK:
                     break;

                  default:

                     /* get listbox selection */

                     nSel = (short) SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam), LB_GETCURSEL,
                                                (DB_WPARAM) 0, (DB_LPARAM) 0L);
                     if (nSel == LB_ERR)
                     {

                        /* clear IDI_AUTHOR, IDI_TITLE */

                        SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_SETTEXT,
                                    (DB_WPARAM) 0, (DB_LPARAM) 0L);
                        SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                                    (DB_WPARAM) 0, (DB_LPARAM) 0L);

                        /* disable IDI_OK */

                        EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);

                        return TRUE;
                     }

                     SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam), LB_GETTEXT,
                                 (DB_WPARAM) nSel, (DB_LPARAM) (LPSTR) szIDCode);

                     if (d_keyfind(ID_CODE, szIDCode, CURR_DB) == S_NOTFOUND)
                     {
                        MessageBox(hDlg, "Selected item is not on file.", szAppName,
                                   MB_OK | MB_ICONASTERISK);
                        return TRUE;
                     }

                     d_recread(&InfoRec, CURR_DB);
                     d_setor(LOANED_BOOKS, CURR_DB);

                     /* get author name: use to initialize IDI_AUTHOR */

                     d_findco(HAS_PUBLISHED, CURR_DB);
                     d_crread(NAME, szAuthor, CURR_DB);

                     SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_SETTEXT,
                                 (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szAuthor);

                     /* get books title: use to initialize IDI_TITLE */

                     SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                                 (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.info_title);

                     /* fill in IDI_BORROWED if name of borrower has been
                      * entered */

                     if (SendMessage(GetDlgItem(hDlg, IDI_BORROWER), WM_GETTEXT,
                          (DB_WPARAM) sizeof(szBorrower), (DB_LPARAM) (LPSTR) szBorrower))
                     {

                        while (d_findnm(LOANED_BOOKS, CURR_DB) == S_OKAY)
                        {

                           d_recread(&BorrowerRec, CURR_DB);

                           if (!strcmp(BorrowerRec.friend, szBorrower))
                           {

                              if (BorrowerRec.date_returned == 0L)
                              {

                                 sprintf(szBorrowed, "%ld", BorrowerRec.date_borrowed);

                                 SendMessage(GetDlgItem(hDlg, IDI_BORROWED), WM_SETTEXT,
                                           (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szBorrowed);
                                 break;
                              }
                           }
                        }
                     }

                     /* enable IDI_OK? */

                     if ((BOOL) SendMessage(GetDlgItem(hDlg, IDI_BORROWER),
                                            WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                       (BOOL) SendMessage(GetDlgItem(hDlg, IDI_RETURNED),
                                          WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L))
                     {
                        EnableWindow(GetDlgItem(hDlg, IDI_OK), TRUE);
                     }
                     else
                     {
                        EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);
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

/* $Revision:   1.2  $ */

/* vpp DLGRET.C (Fri Sep 19 11:28:42 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

