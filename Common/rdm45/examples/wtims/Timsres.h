/******************************************************************************\

                            TIMS SAMPLE APPLICATION

              COPYRIGHT (C) 1989 RAIMA CORP. ALL RIGHTS RESERVED.

\******************************************************************************/

/******************************************************************************\

                          RESOURCE CONSTANTS (_IDR)

\******************************************************************************/

#define IDR_ACCELERATORS      1
#define IDR_ICON              2
#define IDR_MENU              3

/******************************************************************************\

                   MENU COMMAND RESOURCE CONSTANTS (_IDM)

\******************************************************************************/

#define IDM_SEPARATOR             0
#define IDM_VIEW_ITEMSBYKEYWORD   1
#define IDM_VIEW_ITEMSBYAUTHOR    2
#define IDM_EDIT_INSERTINFO       3
#define IDM_EDIT_DELETEINFO       4
#define IDM_LOAN_BORROWBOOK       5
#define IDM_LOAN_RETURNBOOK       6
#define IDM_LOAN_BORROWINGS       7
#define IDM_ABOUT                 99

/******************************************************************************\

                      STRING RESOURCE CONSTANTS (_IDS)

\******************************************************************************/

#define IDS_MAINCLASS         0
#define IDS_ABOUT             1
#define IDS_ADDEDINSTANCE     2
#define IDS_OUTOFMEMORY       3
#define IDS_APPNAME           4
#define IDS_MAINCAPTION       5
#define IDS_CANTOPENDATABASE  6
#define IDS_SELECTKEYWORD     7
#define IDS_SELECTAUTHOR      8

/******************************************************************************\

                    DIALOG BOX RESOURCE CONSTANTS (_IDD)

\******************************************************************************/

#define IDD_ABOUT         1
#define IDD_INSERTINFO    2
#define IDD_DELETEINFO    3
#define IDD_BORROWBOOK    4
#define IDD_RETURNBOOK    5
#define IDD_BORROWINGS    6
#define IDD_KEYWORDS      7
#define IDD_AUTHORS       8
#define IDD_VIEWINFO      9

/******************************************************************************\

                       DIALOG CONTROL CONSTANTS (_IDI)

\******************************************************************************/

#define IDI_OK          100
#define IDI_VIEW        100
#define IDI_CANCEL      101
#define IDI_ADD         102
#define IDI_CHANGE      103
#define IDI_DELETE      104
#define IDI_CLEAR       105
#define IDI_NEXT        106
#define IDI_PREVIOUS    107
#define IDI_LIST        108
#define IDI_AUTHOR      109
#define IDI_TITLE       110
#define IDI_PUBLISHER   111
#define IDI_PUBLISHED   112
#define IDI_IDCODE      113
#define IDI_ABSTRACT    114
#define IDI_ARTICLE     115            /* 115 - 117 are sequenced */
#define IDI_BOOK        116            /* 115 - 117 are sequenced */
#define IDI_MAGAZINE    117            /* 115 - 117 are sequenced */
#define IDI_KEYWORD     118
#define IDI_BORROWER    119
#define IDI_BORROWED    120
#define IDI_RETURNED    121
#define IDI_SCROLL      122

/******************************************************************************\

                               GLOBAL HANDLES

\******************************************************************************/

extern GLOBALHANDLE hInst;             /* handle to module instance */
extern GLOBALHANDLE hAccelerators;       /* handle to accelerators resource */
extern GLOBALHANDLE hIcon;             /* handle to icon resource */
extern GLOBALHANDLE hMenu;             /* handle to menu resource */
extern HWND hWndMain;                  /* handle to window of class
                                        * szMainClass */

/******************************************************************************\

                               GLOBAL STRINGS

\******************************************************************************/

extern char szMainClass[11];
extern char szAppName[15];

#define LSZTITLE    80
#define LSZAUTHOR   32
#define LSZIDCODE   16
#define LSZKEYWORD  32
#define LSZLINE     80
#define LSZNAME     32
#define LSZDATE     12

/******************************************************************************\

                             MessageBox() FLAGS

\******************************************************************************/

#define MB_OUTOFMEMORY        MB_RETRYCANCEL | MB_ICONHAND

/******************************************************************************\

                         UTILITY FUNCTION PROTOTYPES

\******************************************************************************/

PSTR FAR PASCAL String(WORD);

/* ///////////////////////// USED FOR  DEBUGGING  /////////////////////////// */
/* ///////////////////////// USED FOR  DEBUGGING  /////////////////////////// */

BOOL FAR PASCAL Note(PSTR);

/* ///////////////////////// USED FOR  DEBUGGING  /////////////////////////// */
/* ///////////////////////// USED FOR  DEBUGGING  /////////////////////////// */

/* $Revision:   1.1  $ */

/* vpp TIMSRES.H (Fri Sep 19 11:28:42 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

