/***************************************************************************\

                            TIMS SAMPLE APPLICATION

         Copyright (c) 1989-1993, Raima Corporation, All Rights Reserved

\***************************************************************************/

#include <stdio.h>

#if 0
#define ALIGNMENT_CHECK
#define ALIGNMENT_PROC_NAME CalcAlignment
#endif
#include "vista.h"
#include "timsres.h"
#include "tims.h"

#define TIMS_C
#include "timsdec.h"

DB_TASK Currtask;
GLOBALHANDLE hInst;                    /* to module instance */
GLOBALHANDLE hAccelerators;            /* to accelerators resource */
GLOBALHANDLE hIcon;                    /* to icon resource */
GLOBALHANDLE hMenu;                    /* to menu resource */
HWND hWndMain;                         /* to window of class szMainClass */
char szMainClass[11];
char szAppName[15];

/*** Exported **************************************************************\

fcn:  WinMain() ;

for:

arg:

ret:

rev:

\***************************************************************************/

BOOL FAR PASCAL WinMain(hInstance, hPrevInstance, lpszCmdLine, cmdShow)
HANDLE hInstance;
HANDLE hPrevInstance;
LPSTR lpszCmdLine;
int cmdShow;
{
   WNDCLASS WndClass;
   MSG msg;
   HWND WndPrev;
   FARPROC fp;

   if ((WndPrev = FindWindow("TIMS_MAIN", NULL)))
   {
      BringWindowToTop(WndPrev);
      return 0;
   }

   hInst = hInstance;

   /* open a single DB_TASK */
   if (dt_opentask(&Currtask) != S_OKAY)
   {

      LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName));
      MessageBox(0, TString(IDS_CANTOPENDATABASE), szAppName,
                 MB_OK | MB_ICONASTERISK | MB_SYSTEMMODAL);
      return FALSE;
   }

   /* open the database */
#if 0
   fp = MakeProcInstance((FARPROC)ALIGNMENT_PROC_NAME, hInstance);
   dt_set_alignment_proc((ALIGNMENT_PROC)fp, &Currtask);
#endif
   if (d_open("tims", "o") != S_OKAY)
   {

      LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName));
      MessageBox(0, TString(IDS_CANTOPENDATABASE), szAppName,
                 MB_OK | MB_ICONASTERISK | MB_SYSTEMMODAL);
      return FALSE;
   }
   FreeProcInstance(fp);

   /* load resources */
   if (!LoadString(hInst, IDS_APPNAME, szAppName, sizeof(szAppName)) ||
       !LoadString(hInst, IDS_MAINCLASS, szMainClass, sizeof(szMainClass)) ||
       !(hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU))) ||
       !(hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDR_ICON))) ||
       !(hAccelerators =
            LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATORS))))
   {
      if (MessageBox(0, TString(IDS_OUTOFMEMORY), szAppName, MB_OUTOFMEMORY)
            == IDCANCEL)
         return FALSE;
   }

   WndClass.lpszClassName = "TIMS_MAIN";
   WndClass.lpszMenuName = (LPSTR) szMainClass;
   WndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
   WndClass.hCursor = LoadCursor(0, IDC_ARROW);
   WndClass.hIcon = hIcon;
   WndClass.hInstance = hInst;
   WndClass.cbWndExtra = 0;
   WndClass.cbClsExtra = 0;
   WndClass.lpfnWndProc = MainWndProc;
   WndClass.style = 0;

   while (RegisterClass((LPWNDCLASS) & WndClass) == 0)
   {
      if (MessageBox(0, TString(IDS_OUTOFMEMORY), szAppName,
                     MB_OUTOFMEMORY) == IDCANCEL)
         return FALSE;
   }

   while ((hWndMain = CreateWindow(szMainClass,
                                   TString(IDS_MAINCAPTION),
                                   WS_OVERLAPPEDWINDOW | WS_SYSMENU |
                                   WS_CLIPSIBLINGS     | WS_CLIPCHILDREN |
                                   WS_MINIMIZEBOX      | WS_MAXIMIZEBOX,
                                   0,
                                   0,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   0,
                                   hMenu,
                                   hInst,
                                   0)) == 0)
   {
      if (MessageBox(0, TString(IDS_OUTOFMEMORY), szAppName,
                     MB_OUTOFMEMORY) == IDCANCEL)
         return FALSE;
   }

   /* display ADK_MAIN window */
   ShowWindow(hWndMain, cmdShow);
   UpdateWindow(hWndMain);

   /* add SEPARATOR and "About..." to System Menu */
   ChangeMenu(GetSystemMenu(hWndMain, FALSE), 0, 0,
              IDM_SEPARATOR, MF_APPEND | MF_SEPARATOR);
   ChangeMenu(GetSystemMenu(hWndMain, FALSE), 0, TString(IDS_ABOUT),
              IDM_ABOUT, MF_APPEND | MF_STRING);

   /* message polling loop */
   while (GetMessage(&msg, 0, 0, 0))
   {
      if (!TranslateAccelerator(hWndMain, hAccelerators, &msg))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   return msg.wParam;
}

/*** Exported **************************************************************\

fcn:  MainWndProc() ;

for:

arg:

ret:

rev:

\***************************************************************************/

LONG FAR PASCAL MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;
unsigned message;
DB_WPARAM wParam;
DB_LPARAM lParam;
{
   /* WM_INITMEMUPOPUP processing functions */

   static void (FAR PASCAL * lpfnInitMenuPopup[]) (void) =
   {
      UpdateViewMenu,
      UpdateEditMenu,
      UpdateLoanMenu
   };

   /* WM_COMMAND processing functions */

   static BOOL(FAR PASCAL * lpfnCommand[]) (void) =
   {
      Cmd_View_ItemsByKeyword,
      Cmd_View_ItemsByKeyword,
      Cmd_View_ItemsByAuthor,
      Cmd_Edit_InsertInfo,
      Cmd_Edit_DeleteInfo,
      Cmd_Loan_BorrowBook,
      Cmd_Loan_ReturnBook,
      Cmd_Loan_Borrowings
   };

   FARPROC lpDlgProc;

   switch (message)
   {
      case WM_CREATE:
         break;

      case WM_INITMENUPOPUP:
         (*lpfnInitMenuPopup[LOWORD(lParam)]) ();
         break;

      case WM_COMMAND:
         (*lpfnCommand[DB_GET_WM_COMMAND_ID(wParam, lParam)]) ();
         break;

      case WM_SYSCOMMAND:
         switch (wParam)
         {
            case IDM_ABOUT:

               /* display IDD_ABOUT dialog box */
               lpDlgProc = MakeProcInstance(AboutDlgProc, hInst);
               while (DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd,
                                lpDlgProc) == -1)
               {
                  if (MessageBox(hWnd, TString(IDS_OUTOFMEMORY), szAppName,
                                 MB_OUTOFMEMORY) == IDCANCEL)
                  {
                     FreeProcInstance(lpDlgProc);
                     return 0L;
                  }
               }

               FreeProcInstance(lpDlgProc);
               break;

            default:
               return DefWindowProc(hWnd, message, wParam, lParam);
         }
         break;

      case WM_CLOSE:
         DestroyWindow(hWnd);
         break;

      case WM_DESTROY:                 /* shut down in an orderly manner */

         /* close the database */
         d_close();

         /* close single DB_TASK */
         dt_closetask(&Currtask);

         /* tell Windows I'm quitting */
         PostQuitMessage(0);
         break;

      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0L;
}

/**** Private Function *****************************************************\

fcn:  UpdateViewMenu() ;
fcn:  UpdateEditMenu() ;
fcn:  UpdateLoanMenu() ;

for:

arg:

ret:

rev:

\***************************************************************************/

void FAR PASCAL UpdateViewMenu()
{
   return;
}

void FAR PASCAL UpdateEditMenu()
{
   return;
}

void FAR PASCAL UpdateLoanMenu()
{
   return;
}

/*** Private Function ******************************************************\
*
* fcn: TString() ;
*
* for:
*
* arg:
*
* ret: (near) ptr to null-terminated string ;
*
* rev: 1/1/89 sls ;
*
\***************************************************************************/

LPSTR FAR PASCAL TString(wResource)
WORD wResource;
{
   static char szResource[256];

   switch (wResource)
   {
      case IDS_OUTOFMEMORY:
         return "Not enough memory for this task.";

      default:
         if (LoadString(hInst, wResource, szResource, sizeof(szResource)))
         {
            return szResource;
         }
         break;
   }

   return 0;
}

/* //////////////////////// USED FOR  DEBUGGING  ///////////////////////// */
/* //////////////////////// USED FOR  DEBUGGING  ///////////////////////// */

BOOL FAR PASCAL Note(pStr)
LPSTR pStr;
{
   MessageBox(hWndMain, pStr, szAppName, MB_OK | MB_ICONEXCLAMATION);
   return TRUE;
}

/* //////////////////////// USED FOR  DEBUGGING  ///////////////////////// */
/* //////////////////////// USED FOR  DEBUGGING  ///////////////////////// */

/* $Revision:   1.4  $ */

/* vpp WTIMS.C (Fri Sep 19 11:28:42 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

