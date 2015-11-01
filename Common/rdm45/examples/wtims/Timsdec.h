#define WDB_PASCAL PASCAL
#define WDB_FAR DB_FAR

#define DB_WPARAM     WPARAM
#define DB_LPARAM     LPARAM
#define DB_GET_WM_COMMAND_ID(w, l)       GET_WM_COMMAND_ID(w, l)
#define DB_GET_WM_PARENTNOTIFY_ID(w, l)  GET_WM_PARENTNOTIFY_ID(w, l)
#define DB_GET_WM_COMMAND_HWND(w, l)     GET_WM_COMMAND_HWND(w, l)

void WDB_PASCAL WDB_FAR UpdateViewMenu(void);
void WDB_PASCAL WDB_FAR UpdateEditMenu(void);
void WDB_PASCAL WDB_FAR UpdateLoanMenu(void);
int WDB_PASCAL WDB_FAR Cmd_View_ItemsByKeyword(void);
int WDB_PASCAL WDB_FAR Cmd_View_ItemsByAuthor(void);
int WDB_PASCAL WDB_FAR Cmd_Edit_InsertInfo(void);
int WDB_PASCAL WDB_FAR Cmd_Edit_DeleteInfo(void);
int WDB_PASCAL WDB_FAR Cmd_Loan_BorrowBook(void);
int WDB_PASCAL WDB_FAR Cmd_Loan_ReturnBook(void);
int WDB_PASCAL WDB_FAR Cmd_Loan_Borrowings(void);
void WDB_FAR ViewInfoInit(HWND hDlg);
BOOL WDB_PASCAL WDB_FAR InsertInfoDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
void WDB_FAR InsertInfoAdd(HWND hDlg);
void WDB_FAR InsertInfoChange(HWND hDlg);
void WDB_FAR InsertInfoDelete(HWND hDlg);
BOOL WDB_PASCAL WDB_FAR DeleteInfoDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR BorrowBookDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR ReturnBookDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
long WDB_PASCAL WDB_FAR MainWndProc(HWND hWnd, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR AboutDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR BorrowingsDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR KeyWordsDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR AuthorsDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
BOOL WDB_PASCAL WDB_FAR ViewInfoDlgProc(HWND hDlg, unsigned message, DB_WPARAM wParam, DB_LPARAM lParam);
LPSTR WDB_PASCAL WDB_FAR TString(WORD wResource);

#ifdef CMDEDIT_C
#pragma alloc_text (SG00003, Cmd_Edit_DeleteInfo, Cmd_Edit_InsertInfo)
#endif

#ifdef CMDLOAN_C
#pragma alloc_text (SG00003, Cmd_Loan_BorrowBook, Cmd_Loan_Borrowings, Cmd_Loan_ReturnBook)
#endif

#ifdef CMDVIEW_C
#pragma alloc_text (SG00002, Cmd_View_ItemsByAuthor, Cmd_View_ItemsByKeyword)
#endif

#ifdef DLGABOUT_C
#pragma alloc_text (SG00003, AboutDlgProc)
#endif

#ifdef DLGAUTH_C
#pragma alloc_text (SG00003, AuthorsDlgProc)
#endif

#ifdef DLGBOR_C
#pragma alloc_text (SG00003, BorrowBookDlgProc)
#endif

#ifdef DLGDEL_C
#pragma alloc_text (SG00004, DeleteInfoDlgProc)
#endif

#ifdef DLGINS_C
#pragma alloc_text (SG00002, InsertInfoAdd, InsertInfoChange, InsertInfoDelete)
#pragma alloc_text (SG00004, InsertInfoDlgProc)
#endif

#ifdef DLGKEYS_C
#pragma alloc_text (SG00003, KeyWordsDlgProc)
#endif

#ifdef DLGLOANS_C
#pragma alloc_text (SG00003, BorrowingsDlgProc)
#endif

#ifdef DLGRET_C
#pragma alloc_text (SG00005, ReturnBookDlgProc)
#endif

#ifdef DLGVIEW_C
#pragma alloc_text (SG00002, ViewInfoInit)
#pragma alloc_text (SG00003, ViewInfoDlgProc)
#endif

#ifdef TIMS_C
#pragma alloc_text (SG00002, MainWndProc, Note, String, UpdateEditMenu, UpdateLoanMenu)
#pragma alloc_text (SG00002, UpdateViewMenu, WinMain)
#endif

/* $Revision:   1.1  $ */

/* vpp TIMSDEC.H (Fri Sep 19 11:28:42 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

