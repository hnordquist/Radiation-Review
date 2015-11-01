/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/
#include <stdio.h>
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define DLGDEL_C
#include "timsdec.h"

/**** Exported ****************************************************************\

fcn:  DeleteInfoDlgProc() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL DeleteInfoDlgProc(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   struct info InfoRec;
   char szIDCode[LSZIDCODE + 1];
   char szAuthor[LSZAUTHOR + 1];
   long count;
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

               if (d_keyfind(ID_CODE, szIDCode, CURR_DB) == S_NOTFOUND)
               {
                  EndDialog(hDlg, TRUE);
                  return TRUE;
               }

               d_recread(&InfoRec, CURR_DB);

               /* get author name */

               d_findco(HAS_PUBLISHED, CURR_DB);
               d_crread(NAME, szAuthor, CURR_DB);

               /* confirm delete request */

               if (MessageBox(hDlg, "Are you sure you want to delete this item?",
                          szAppName, MB_YESNO | MB_ICONQUESTION) == IDNO)
               {
                  return TRUE;
               }

               /* disconnect any listed articles */

               d_setom(ARTICLE_LIST, HAS_PUBLISHED, CURR_DB);

               for (stat = d_findfm(ARTICLE_LIST, CURR_DB); stat == S_OKAY;
                    stat = d_discon(ARTICLE_LIST, CURR_DB));

               /* disconnect and delete borrowers */

               d_setom(LOANED_BOOKS, HAS_PUBLISHED, CURR_DB);

               while (d_findfm(LOANED_BOOKS, CURR_DB) == S_OKAY)
               {
                  d_discon(LOANED_BOOKS, CURR_DB);
                  d_setmr(LOAN_HISTORY, CURR_DB);
                  d_discon(LOAN_HISTORY, CURR_DB);
                  d_delete(CURR_DB);
               }

               /* disconnect and delete abstract */

               d_setom(ABSTRACT, HAS_PUBLISHED, CURR_DB);

               while (d_findfm(ABSTRACT, CURR_DB) == S_OKAY)
               {
                  d_discon(ABSTRACT, CURR_DB);
                  d_delete(CURR_DB);
               }

               /* disconnect and delete intersect and (possibly) key word */

               d_setom(INFO_TO_KEY, HAS_PUBLISHED, CURR_DB);

               while (d_findfm(INFO_TO_KEY, CURR_DB) == S_OKAY)
               {
                  d_discon(INFO_TO_KEY, CURR_DB);
                  d_setmr(KEY_TO_INFO, CURR_DB);
                  d_discon(KEY_TO_INFO, CURR_DB);
                  d_delete(CURR_DB);
                  d_members(KEY_TO_INFO, &count, CURR_DB);

                  if (count == 0L)
                  {
                     /* delete key word */
                     d_setro(KEY_TO_INFO, CURR_DB);
                     d_delete(CURR_DB);
                  }
               }

               /* disconnect info record from author and delete */

               d_discon(HAS_PUBLISHED, CURR_DB);
               d_delete(CURR_DB);

               /* delete author too, if he has no other pubs */

               d_members(HAS_PUBLISHED, &count, CURR_DB);

               if (count == 0L)
               {
                  d_setmo(AUTHOR_LIST, HAS_PUBLISHED, CURR_DB);
                  d_discon(AUTHOR_LIST, CURR_DB);
                  d_delete(CURR_DB);
               }

               EndDialog(hDlg, TRUE);
               break;

            case IDI_CANCEL:
               EndDialog(hDlg, FALSE);
               break;

            case IDI_LIST:
               switch (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam))
               {
                  case LBN_DBLCLK:
                     PostMessage(hDlg, WM_COMMAND, (DB_WPARAM) IDI_OK, (DB_LPARAM) 0L);
                     break;

                  default:

                     /* get listbox selection */

                     nSel = (short) SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam), LB_GETCURSEL,
                                                (DB_WPARAM) 0, (DB_LPARAM) 0L);
                     if (nSel == LB_ERR)
                     {

                        /* clear IDI_AUTHOR, IDI_TITLE */

                        SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_SETTEXT,
                                    (DB_WPARAM) 0, (DB_LPARAM) 0);
                        SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                                    (DB_WPARAM) 0, (DB_LPARAM) 0);

                        /* disable IDI_OK */

                        EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);

                        return TRUE;
                     }

                     SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam), LB_GETTEXT,
                                 (DB_WPARAM) nSel, (DB_LPARAM) (LPSTR) szIDCode);

                     if (d_keyfind(ID_CODE, szIDCode, CURR_DB) == S_NOTFOUND)
                     {
                        MessageBox(hDlg, "This ID code is not on file.", szAppName,
                                   MB_OK | MB_ICONASTERISK);
                        return TRUE;
                     }

                     d_recread(&InfoRec, CURR_DB);

                     /* get author name: use to initialize IDI_AUTHOR */

                     d_findco(HAS_PUBLISHED, CURR_DB);
                     d_crread(NAME, szAuthor, CURR_DB);

                     SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_SETTEXT,
                                 (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szAuthor);

                     /* get books title: use to initialize IDI_TITLE */

                     SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
                                 (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.info_title);

                     /* enable IDI_OK? */

                     EnableWindow(GetDlgItem(hDlg, IDI_OK),
                        (short) SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam), LB_GETCURSEL,
                                            (DB_WPARAM) 0, (DB_LPARAM) 0L) != LB_ERR);
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

/* vpp DLGDEL.C (Fri Sep 19 11:28:42 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

