/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define DLGINS_C
#include "timsdec.h"

#ifdef ZOR
#define strcmpi strcmpl                /* Ask Walter? */
#endif

struct author AuthorRec;
struct info InfoRec;

/**** Exported ****************************************************************\

fcn:  InsertInfoDlgProc() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL InsertInfoDlgProc(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   char szKeyWord[LSZKEYWORD + 1];
   char szName[LSZNAME + 1];
   char szLine[LSZLINE + 1];
   register int i;
   register int j;
   register int n;
   GLOBALHANDLE hAbstract;
   LPSTR lpAbstract;
   int stat;

   switch (message)
   {
      case WM_INITDIALOG:
         /* disable IDI_OK, IDI_ADD, IDI_CHANGE, IDI_DELETE */
         EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDI_ADD), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDI_CHANGE), FALSE);
         EnableWindow(GetDlgItem(hDlg, IDI_DELETE), FALSE);
         break;

      case WM_COMMAND:
         switch (DB_GET_WM_COMMAND_ID(wParam, lParam))
         {
            case IDI_OK:

               /* get IDI_AUTHOR, IDI_TITLE, IDI_IDCODE, etc... */

               SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_GETTEXT,
                  (DB_WPARAM) sizeof(AuthorRec.name), (DB_LPARAM) (LPSTR) AuthorRec.name);

               SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_GETTEXT,
                           (DB_WPARAM) sizeof(InfoRec.info_title), (DB_LPARAM) (LPSTR) InfoRec.info_title);

               SendMessage(GetDlgItem(hDlg, IDI_PUBLISHER), WM_GETTEXT,
                           (DB_WPARAM) sizeof(InfoRec.publisher), (DB_LPARAM) (LPSTR) InfoRec.publisher);

               SendMessage(GetDlgItem(hDlg, IDI_PUBLISHED), WM_GETTEXT,
               (DB_WPARAM) sizeof(InfoRec.pub_date), (DB_LPARAM) (LPSTR) InfoRec.pub_date);

               SendMessage(GetDlgItem(hDlg, IDI_IDCODE), WM_GETTEXT,
                (DB_WPARAM) sizeof(InfoRec.id_code), (DB_LPARAM) (LPSTR) InfoRec.id_code);

               /* get information type */

               if (IsDlgButtonChecked(hDlg, IDI_ARTICLE))
               {
                  InfoRec.info_type = 2;
               }
               if (IsDlgButtonChecked(hDlg, IDI_BOOK))
               {
                  InfoRec.info_type = 0;
               }
               if (IsDlgButtonChecked(hDlg, IDI_MAGAZINE))
               {
                  InfoRec.info_type = 1;
               }

               /* see if author exists */

               for (stat = d_findfm(AUTHOR_LIST, CURR_DB); stat == S_OKAY;
                    stat = d_findnm(AUTHOR_LIST, CURR_DB))
               {

                  stat = d_crread(NAME, szName, CURR_DB);

                  if (strcmp(AuthorRec.name, szName) == 0)
                  {
                     break;
                  }
               }

               if (stat == S_EOS)
               {

                  /* author not on file: create record and connect to
                   * author list */

                  d_fillnew(AUTHOR, &AuthorRec, CURR_DB);

                  d_connect(AUTHOR_LIST, CURR_DB);
               }

               /* make author current owner of has_published set */

               d_setor(HAS_PUBLISHED, CURR_DB);

               /* create new tech. info record */

               if (d_fillnew(INFO, &InfoRec, CURR_DB) == S_DUPLICATE)
               {
                  EndDialog(hDlg, TRUE);
                  return TRUE;
               }
               else
               {

                  /* connect to author record */

                  d_connect(HAS_PUBLISHED, CURR_DB);

                  /* set current owner for key words and abstract */

                  d_setor(INFO_TO_KEY, CURR_DB);
                  d_setor(ABSTRACT, CURR_DB);
               }

               /* for each keyword, create new keyword record */

               n = (int) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCOUNT,
                                     (DB_WPARAM) 0, (DB_LPARAM) 0);

               for (i = 0; i != n; i++)
               {

                  SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETTEXT,
                              (DB_WPARAM) i, (DB_LPARAM) (LPSTR) szKeyWord);

                  /* see if key word record exists */

                  if (d_keyfind(KWORD, szKeyWord, CURR_DB) == S_NOTFOUND)
                  {

                     /* create new key word record */

                     d_fillnew(KEY_WORD, szKeyWord, CURR_DB);
                  }

                  d_setor(KEY_TO_INFO, CURR_DB);

                  /* create intersection record */

                  d_fillnew(INTERSECT, &InfoRec.info_type, CURR_DB);

                  d_connect(KEY_TO_INFO, CURR_DB);
                  d_connect(INFO_TO_KEY, CURR_DB);
               }

               /* get length of abstract text */

               n = (int) SendMessage(GetDlgItem(hDlg, IDI_ABSTRACT), WM_GETTEXTLENGTH,
                                     (DB_WPARAM) 0, (DB_LPARAM) 0);

               while (!(hAbstract = GlobalAlloc(GHND, (DWORD) (n + 1))))
               {
                  if (MessageBox(hDlg, TString(IDS_OUTOFMEMORY), szAppName,
                                 MB_OUTOFMEMORY) == IDCANCEL)
                     return TRUE;
               }

               lpAbstract = (LPSTR) GlobalLock(hAbstract);
               GlobalUnlock(hAbstract);

               /* copy abstract text into buffer; then parse */

               SendMessage(GetDlgItem(hDlg, IDI_ABSTRACT), WM_GETTEXT,
                           (DB_WPARAM) (n + 1), (DB_LPARAM) lpAbstract);

               for (i = 0; lpAbstract[i] != 0;)
               {

                  j = 0;
                  while (lpAbstract[i] != 0 && j != sizeof(szLine) - 1)
                  {
                     szLine[j++] = lpAbstract[i++];
                  }
                  szLine[j++] = 0;

                  if (szLine[0] != 0)
                  {

                     d_fillnew(INFOTEXT, szLine, CURR_DB);

                     d_connect(ABSTRACT, CURR_DB);
                  }
               }

               GlobalFree(hAbstract);

               EndDialog(hDlg, TRUE);
               break;

            case IDI_CANCEL:
               EndDialog(hDlg, FALSE);
               break;

            case IDI_ADD:
               InsertInfoAdd(hDlg);
               break;

            case IDI_CHANGE:
               InsertInfoChange(hDlg);
               break;

            case IDI_DELETE:
               InsertInfoDelete(hDlg);
               break;

            case IDI_ARTICLE:
            case IDI_BOOK:
            case IDI_MAGAZINE:

               /* check/uncheck radio buttons */

               if (!IsDlgButtonChecked(hDlg, wParam))
               {
                  CheckRadioButton(hDlg, IDI_ARTICLE, IDI_MAGAZINE, wParam);
               }

               /* enable/disable IDI_OK ? */

               if ((BOOL) SendMessage(GetDlgItem(hDlg, IDI_AUTHOR),
                                      WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                   (BOOL) SendMessage(GetDlgItem(hDlg, IDI_TITLE),
                                      WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                   (BOOL) SendMessage(GetDlgItem(hDlg, IDI_PUBLISHER),
                                      WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                   (BOOL) SendMessage(GetDlgItem(hDlg, IDI_PUBLISHED),
                                      WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                   (BOOL) SendMessage(GetDlgItem(hDlg, IDI_IDCODE),
                                      WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                   (IsDlgButtonChecked(hDlg, IDI_ARTICLE) ||
                    IsDlgButtonChecked(hDlg, IDI_BOOK) ||
                    IsDlgButtonChecked(hDlg, IDI_MAGAZINE)))
               {
                  EnableWindow(GetDlgItem(hDlg, IDI_OK), TRUE);
               }
               else
               {
                  EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);
               }
               break;

            case IDI_AUTHOR:
            case IDI_TITLE:
            case IDI_PUBLISHER:
            case IDI_PUBLISHED:
            case IDI_IDCODE:

               /* enable/disable IDI_OK ? */

               if (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam) == EN_CHANGE)
               {
                  if ((BOOL) SendMessage(GetDlgItem(hDlg, IDI_AUTHOR),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (BOOL) SendMessage(GetDlgItem(hDlg, IDI_TITLE),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (BOOL) SendMessage(GetDlgItem(hDlg, IDI_PUBLISHER),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (BOOL) SendMessage(GetDlgItem(hDlg, IDI_PUBLISHED),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (BOOL) SendMessage(GetDlgItem(hDlg, IDI_IDCODE),
                                         WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L) &&
                      (IsDlgButtonChecked(hDlg, IDI_ARTICLE) ||
                       IsDlgButtonChecked(hDlg, IDI_BOOK) ||
                       IsDlgButtonChecked(hDlg, IDI_MAGAZINE)))
                  {
                     EnableWindow(GetDlgItem(hDlg, IDI_OK), TRUE);
                  }
                  else
                  {
                     EnableWindow(GetDlgItem(hDlg, IDI_OK), FALSE);
                  }
               }
               break;

            case IDI_KEYWORD:

               /* enable/disable IDI_ADD? */

               if (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam) == EN_CHANGE)
               {
                  EnableWindow(GetDlgItem(hDlg, IDI_ADD),
                               (BOOL) SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam),
                                               WM_GETTEXTLENGTH, (DB_WPARAM) 0, (DB_LPARAM) 0L));
               }
               break;

            case IDI_LIST:
               switch (DB_GET_WM_PARENTNOTIFY_ID(wParam, lParam))
               {
                  case LBN_DBLCLK:
                     PostMessage(hDlg, WM_COMMAND, (DB_WPARAM) IDI_CHANGE, (DB_LPARAM) 0L);
                     break;
                  default:

                     /* enable IDI_CHANGE && IDI_DELETE? */

                     EnableWindow(GetDlgItem(hDlg, IDI_CHANGE),
                        (short) SendMessage(DB_GET_WM_COMMAND_HWND(wParam, lParam), LB_GETCURSEL,
                                            (DB_WPARAM) 0, (DB_LPARAM) 0L) != LB_ERR);
                     EnableWindow(GetDlgItem(hDlg, IDI_DELETE),
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

/**** Private Function ********************************************************\

fcn:  InsertInfoAdd() ;

for:

arg:

ret:

rev:

\******************************************************************************/

void FAR InsertInfoAdd(hDlg)
HWND hDlg;
{
   char szNewKeyWord[LSZKEYWORD + 1];
   char szOldKeyWord[LSZKEYWORD + 1];
   register int i;
   short nCount;

   /* retrieve contents of IDI_KEYWORD */

   SendMessage(GetDlgItem(hDlg, IDI_KEYWORD), WM_GETTEXT,
               (DB_WPARAM) sizeof(szNewKeyWord), (DB_LPARAM) (LPSTR) szNewKeyWord);

   nCount = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCOUNT,
                                (DB_WPARAM) 0, (DB_LPARAM) 0L);

   /* is keyword unique? If not, disable IDI_ADD */

   for (i = 0; i != nCount; i++)
   {

      SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETTEXT,
                  (DB_WPARAM) i, (DB_LPARAM) (LPSTR) szOldKeyWord);

      if (!strcmpi(szNewKeyWord, szOldKeyWord))
      {

         SendMessage(GetDlgItem(hDlg, IDI_KEYWORD), WM_SETTEXT, (DB_WPARAM) 0, (DB_LPARAM) 0L);

         EnableWindow(GetDlgItem(hDlg, IDI_ADD), FALSE);

         return;
      }
   }

   /* keyword is unique: add to IDI_LIST */

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_ADDSTRING,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szNewKeyWord);

   SendMessage(GetDlgItem(hDlg, IDI_KEYWORD), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   /* set focus back to IDI_KEYWORD */

   SetFocus(GetDlgItem(hDlg, IDI_KEYWORD));

   /* disable IDI_ADD button */

   EnableWindow(GetDlgItem(hDlg, IDI_ADD), FALSE);
}

/**** Private Function ********************************************************\

fcn:  InsertInfoChange() ;

for:

arg:

ret:

rev:

\******************************************************************************/

void FAR InsertInfoChange(hDlg)
HWND hDlg;
{
   char szKeyWord[LSZKEYWORD + 1];
   short nSel;

   /* get listbox selection, delete from listbox and unselect */

   nSel = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCURSEL,
                              (DB_WPARAM) 0, (DB_LPARAM) 0L);
   if (nSel == LB_ERR)
      return;

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETTEXT,
               (DB_WPARAM) nSel, (DB_LPARAM) (LPSTR) szKeyWord);

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_DELETESTRING,
               (DB_WPARAM) nSel, (DB_LPARAM) 0L);

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_SETCURSEL,
               (DB_WPARAM) - 1, (DB_LPARAM) 0L);

   /* display keyword in IDI_KEYWORD and enable IDI_ADD */

   SendMessage(GetDlgItem(hDlg, IDI_KEYWORD), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szKeyWord);

   SendMessage(GetDlgItem(hDlg, IDI_KEYWORD), EM_SETSEL,
               (DB_WPARAM)0, (DB_LPARAM) (MAKELONG(0, 32767)));

   SetFocus(GetDlgItem(hDlg, IDI_KEYWORD));

   /* enable/disable IDI_ADD, IDI_CHANGE, IDI_DELETE */

   EnableWindow(GetDlgItem(hDlg, IDI_ADD), TRUE);
   EnableWindow(GetDlgItem(hDlg, IDI_DELETE), FALSE);
   EnableWindow(GetDlgItem(hDlg, IDI_CHANGE), FALSE);
}

/**** Private Function ********************************************************\

fcn:  InsertInfoDelete() ;

for:

arg:

ret:

rev:

\******************************************************************************/

void FAR InsertInfoDelete(hDlg)
HWND hDlg;
{
   short nSel;

   /* get listbox selection */

   nSel = (short) SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_GETCURSEL,
                              (DB_WPARAM) 0, (DB_LPARAM) 0L);
   if (nSel == LB_ERR)
      return;

   /* delete from IDI_LIST and unselect */

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_DELETESTRING,
               (DB_WPARAM) nSel, (DB_LPARAM) 0L);

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_SETCURSEL, (DB_WPARAM) - 1, (DB_LPARAM) 0L);

   /* disable IDI_CHANGE && IDI_DELETE */

   EnableWindow(GetDlgItem(hDlg, IDI_DELETE), FALSE);
   EnableWindow(GetDlgItem(hDlg, IDI_CHANGE), FALSE);
}

/* $Revision:   1.4  $ */

/* vpp DLGINS.C (Fri Sep 19 11:28:41 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

