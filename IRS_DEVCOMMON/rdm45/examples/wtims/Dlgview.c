/******************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1992, Raima Corporation, All Rights Reserved

\******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define DLGVIEW_C
#include "timsdec.h"

BOOL bViewByKeyWord;
short nScrollPos;

/**** Exported ****************************************************************\

fcn:  ViewInfoDlgProc() ;

for:

arg:

ret:

rev:

\******************************************************************************/

BOOL FAR PASCAL ViewInfoDlgProc(hDlg, message, wParam, lParam)
HWND hDlg;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   switch (message)
   {
      case WM_INITDIALOG:

         /* is a keyword or author selected? */

         if (SendMessage(GetDlgItem(GetParent(hDlg), IDI_LIST), LB_GETCURSEL,
                         (DB_WPARAM) 0, (DB_LPARAM) 0L) == LB_ERR)
         {
            PostMessage(hDlg, WM_COMMAND, (DB_WPARAM) IDI_OK, (DB_LPARAM) 0L);
            return TRUE;
         }

         nScrollPos = 0;               /* used to keep track of
                                        * IDI_NEXT/PREVIOUS */

         /* initialize dialog box controls */

         ViewInfoInit(hDlg);
         break;

      case WM_COMMAND:
         switch (DB_GET_WM_COMMAND_ID(wParam, lParam))
         {
            case IDI_OK:
               EndDialog(hDlg, TRUE);
               break;

            case IDI_PREVIOUS:

               nScrollPos--;

               /* re-initialize dialog box controls */

               ViewInfoInit(hDlg);
               break;

            case IDI_NEXT:

               nScrollPos++;

               /* re-initialize dialog box controls */

               ViewInfoInit(hDlg);
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

/**** PrivateFunction *********************************************************\

fcn:  ViewInfoInit() ;

for:

arg:

ret:

rev:

\******************************************************************************/

void FAR ViewInfoInit(hDlg)
HWND hDlg;
{
   struct info InfoRec;
   char szKeyWord[LSZKEYWORD + 1];
   char szAuthor[LSZAUTHOR + 1];
   char szName[LSZNAME + 1];
   char szLine[LSZLINE + 1];
   char szHalfLine[LSZLINE / 2 + 1];
   register int i;
   register int j;
   register int n;
   DB_ADDR dba;
   long count;
   short nSel;
   int stat;

   /* zero intialize IDI_AUTHOR, IDI_TITLE, IDI_IDCODE */

   SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   SendMessage(GetDlgItem(hDlg, IDI_IDCODE), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   /* zero intialize IDI_PUBLISHER, IDI_PUBLISHED */

   SendMessage(GetDlgItem(hDlg, IDI_PUBLISHER), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   SendMessage(GetDlgItem(hDlg, IDI_PUBLISHED), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   /* zero intialize IDI_ABSTRACT, IDI_LIST */

   SendMessage(GetDlgItem(hDlg, IDI_ABSTRACT), LB_RESETCONTENT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_RESETCONTENT,
               (DB_WPARAM) 0, (DB_LPARAM) 0L);

   /* disable IDI_PREVIOUS, IDI_NEXT */

   EnableWindow(GetDlgItem(hDlg, IDI_PREVIOUS), FALSE);
   EnableWindow(GetDlgItem(hDlg, IDI_NEXT), FALSE);

   if (bViewByKeyWord == TRUE)
   {

      /* get keyword from IDD_KEYWORDS listbox */

      nSel = (short) SendMessage(GetDlgItem(GetParent(hDlg), IDI_LIST),
                                 LB_GETCURSEL, (DB_WPARAM) 0, (DB_LPARAM) 0L);

      SendMessage(GetDlgItem(GetParent(hDlg), IDI_LIST), LB_GETTEXT,
                  (DB_WPARAM) nSel, (DB_LPARAM) (LPSTR) szKeyWord);

      /* find key word record */

      if (d_keyfind(KWORD, szKeyWord, CURR_DB) == S_NOTFOUND)
      {
         MessageBox(hDlg, "There are no records for this keyword.", szAppName,
                    MB_OK | MB_ICONEXCLAMATION);
         return;
      }

      /* scan thru key_to_info set */

      d_setor(KEY_TO_INFO, CURR_DB);

      i = 0;
      for (stat = d_findfm(KEY_TO_INFO, CURR_DB); stat == S_OKAY;
           stat = d_findnm(KEY_TO_INFO, CURR_DB))
         if (i++ == nScrollPos)
            break;

      /* enable IDI_PREVIOUS, IDI_NEXT? */

      if ((stat = d_findpm(KEY_TO_INFO, CURR_DB)) != S_EOS)
      {
         stat = d_findnm(KEY_TO_INFO, CURR_DB); /* restore currrency */
         EnableWindow(GetDlgItem(hDlg, IDI_PREVIOUS), TRUE);
      }
      else
      {
         stat = d_findfm(KEY_TO_INFO, CURR_DB); /* restore currrency */
      }
      if ((stat = d_findnm(KEY_TO_INFO, CURR_DB)) != S_EOS)
      {
         stat = d_findpm(KEY_TO_INFO, CURR_DB); /* restore currrency */
         EnableWindow(GetDlgItem(hDlg, IDI_NEXT), TRUE);
      }
      else
      {
         stat = d_findlm(KEY_TO_INFO, CURR_DB); /* restore currrency */
      }

      if (stat == S_OKAY)
      {

         /* find current owner (info) of current record (intersect) */

         d_findco(INFO_TO_KEY, CURR_DB);

         /* read contents of info record */

         d_recread(&InfoRec, CURR_DB);

         /* find author of info record */

         d_findco(HAS_PUBLISHED, CURR_DB);

         d_crread(NAME, szAuthor, CURR_DB);
      }
   }
   else
   {

      /* get author from IDD_AUTHORS listbox */

      nSel = (short) SendMessage(GetDlgItem(GetParent(hDlg), IDI_LIST),
                                 LB_GETCURSEL, (DB_WPARAM) 0, (DB_LPARAM) 0L);

      SendMessage(GetDlgItem(GetParent(hDlg), IDI_LIST), LB_GETTEXT,
                  (DB_WPARAM) nSel, (DB_LPARAM) (LPSTR) szAuthor);

      /* find author record */

      for (stat = d_findfm(AUTHOR_LIST, CURR_DB); stat == S_OKAY;
           stat = d_findnm(AUTHOR_LIST, CURR_DB))
      {

         d_crread(NAME, szName, CURR_DB);

         if (strcmp(szAuthor, szName) == 0)
            break;
         else if (strcmp(szAuthor, szName) < 0)
         {
            MessageBox(hDlg, "There are no records for this author.", szAppName,
                       MB_OK | MB_ICONEXCLAMATION);
            return;
         }
      }

      d_setor(HAS_PUBLISHED, CURR_DB);

      i = 0;
      for (stat = d_findfm(HAS_PUBLISHED, CURR_DB); stat == S_OKAY;
           stat = d_findnm(HAS_PUBLISHED, CURR_DB))
         if (i++ == nScrollPos)
            break;

      /* enable IDI_PREVIOUS, IDI_NEXT? */

      if ((stat = d_findpm(HAS_PUBLISHED, CURR_DB)) != S_EOS)
      {
         stat = d_findnm(HAS_PUBLISHED, CURR_DB);       /* restore currrency */
         EnableWindow(GetDlgItem(hDlg, IDI_PREVIOUS), TRUE);
      }
      else
      {
         stat = d_findfm(HAS_PUBLISHED, CURR_DB);       /* restore currrency */
      }
      if ((stat = d_findnm(HAS_PUBLISHED, CURR_DB)) != S_EOS)
      {
         stat = d_findpm(HAS_PUBLISHED, CURR_DB);       /* restore currrency */
         EnableWindow(GetDlgItem(hDlg, IDI_NEXT), TRUE);
      }
      else
      {
         stat = d_findlm(HAS_PUBLISHED, CURR_DB);       /* restore currrency */
      }

      if (stat == S_OKAY)
      {

         /* read contents of info record */

         d_recread(&InfoRec, CURR_DB);
      }
   }

   /* intialize IDI_AUTHOR, IDI_TITLE, IDI_IDCODE */

   SendMessage(GetDlgItem(hDlg, IDI_AUTHOR), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szAuthor);

   SendMessage(GetDlgItem(hDlg, IDI_TITLE), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.info_title);

   SendMessage(GetDlgItem(hDlg, IDI_IDCODE), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.id_code);

   /* intialize IDI_PUBLISHER, IDI_PUBLISHED */

   SendMessage(GetDlgItem(hDlg, IDI_PUBLISHER), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.publisher);

   SendMessage(GetDlgItem(hDlg, IDI_PUBLISHED), WM_SETTEXT,
               (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) InfoRec.pub_date);

   /* intialize IDI_ABSTRACT */

   d_setom(ABSTRACT, HAS_PUBLISHED, CURR_DB);

   d_members(ABSTRACT, &count, CURR_DB);

   if (count > 0)
   {

      /* find, read and add each abstract text line to IDI_ABSTRACT */

      while (d_findnm(ABSTRACT, CURR_DB) != S_EOS)
      {

         d_csmread(ABSTRACT, LINE, szLine, CURR_DB);

         i = 0;
         j = 0;
         for (; szLine[i] != 0 && j != sizeof(szHalfLine) - 1;)
         {
            szHalfLine[j++] = szLine[i++];
         }
         szHalfLine[j++] = 0;

         if (szHalfLine[0] != 0)
         {

            /* back up to first space ' ' char? */

            if (j == sizeof(szHalfLine) && szLine[j - 2] != ' ' &&
                szLine[j - 1] != ' ')
            {

               --j;
               --j;

               for (n = j; n >= 0; n--)
               {
                  if (szHalfLine[n] == ' ')
                  {
                     szHalfLine[n] = 0;
                     i = n + 1;
                     break;
                  }
               }
            }

            SendMessage(GetDlgItem(hDlg, IDI_ABSTRACT), LB_INSERTSTRING,
                        (DB_WPARAM) - 1, (DB_LPARAM) (LPSTR) szHalfLine);
         }

         j = 0;
         for (; szLine[i] != 0 && j != sizeof(szHalfLine) - 1;)
         {
            szHalfLine[j++] = szLine[i++];
         }
         szHalfLine[j++] = 0;

         if (szHalfLine[0] != 0)
         {
            SendMessage(GetDlgItem(hDlg, IDI_ABSTRACT), LB_INSERTSTRING,
                        (DB_WPARAM) - 1, (DB_LPARAM) (LPSTR) szHalfLine);
         }
      }
   }

   /* intialize IDI_LIST with keywords */

   d_setom(INFO_TO_KEY, HAS_PUBLISHED, CURR_DB);

   d_members(INFO_TO_KEY, &count, CURR_DB);

   /* list the key words, if any */

   if (count > 0L)
   {

      d_csmget(KEY_TO_INFO, &dba, CURR_DB);

      /* find each intersect member record */

      while (d_findnm(INFO_TO_KEY, CURR_DB) == S_OKAY)
      {

         d_findco(KEY_TO_INFO, CURR_DB);

         d_crread(KWORD, szKeyWord, CURR_DB);

         SendMessage(GetDlgItem(hDlg, IDI_LIST), LB_ADDSTRING,
                     (DB_WPARAM) 0, (DB_LPARAM) (LPSTR) szKeyWord);
      }

      /* reset key_to_info current member and owner */

      if (dba)
         d_csmset(KEY_TO_INFO, &dba, CURR_DB);
   }
}

/* $Revision:   1.3  $ */

/* vpp DLGVIEW.C (Fri Sep 19 11:28:41 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

