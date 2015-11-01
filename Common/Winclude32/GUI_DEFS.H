
#if !defined(GUI_DEFS_H)
	#define GUI_DEFS_H
	
	#ifdef WIN32
		#ifndef RW_NO_STL
			#define RW_NO_STL		// RogueWave NO Std Template Lib.
		#endif
		#define __export
		#define huge
		#define halloc	calloc
		#define hfree	free
	#else
		#define GUI_DATA_TABLE
	#endif


	const COLORREF GUI_BLACK = RGB(0, 0, 0);
	const COLORREF GUI_BLUE = RGB(0, 0, 255);
	const COLORREF GUI_BLUE_DARK = RGB(0, 0, 128);
	const COLORREF GUI_BLUE_LIGHT = RGB(0, 0, 192);
	const COLORREF GUI_CYAN	= RGB(255, 255, 0);
	const COLORREF GUI_GRAY = RGB(128, 128, 128);
	const COLORREF GUI_GRAY_DARK = RGB(64, 64, 64);
	const COLORREF GUI_GRAY_LIGHT = RGB(192, 192, 192);
	const COLORREF GUI_GREEN = RGB(0, 255, 0);
	const COLORREF GUI_GREEN_DARK = RGB(0, 128, 0);
	const COLORREF GUI_RED = RGB(255, 0, 0);
	const COLORREF GUI_RED_DARK	= RGB(128, 0, 0);
	const COLORREF GUI_MAGENTA = RGB(255, 0, 255);
	const COLORREF GUI_MAGENTA_DARK	= RGB(128, 0, 128);
	const COLORREF GUI_WHITE = RGB(255, 255, 255);
	const COLORREF GUI_YELLOW = RGB(255, 255, 0);
		
	typedef unsigned int       RESOURCE_ID;
	typedef unsigned char	   UCHAR;
	typedef unsigned short	   USHORT;
//	typedef unsigned int       UINT;		// Done in windows.h
	typedef unsigned long      ULONG;

	#ifdef WIN_NT
		// Next lines added 18-Mar-96 for NT compile	
		#define huge 
		#define _export
		#define Z_NP 1
		#define DDE 1
		#define CTRL3D	1
		#define TOM_ONLY
		#define hfree	free
		#define halloc	calloc
		#define __CTL3D__
		#define Z_DB	TRUE
		#define	Z_DLLVSPEC
		#define Z_DLLVBSPEC
		#define Z_DLLSPEC
	#endif
	
	const USHORT GUI_LIB_NO_MAX_NUM_CHAR = 0;

	#if defined(TOM_ONLY)
		struct GEN_TIME_STRUCT {
				    int hr;
				    int min;
				    int sec;
				    int msec;
				   };
		
		struct GEN_DATE_STRUCT {
				    int yr;
				    int mon;
				    int day;
				   };
	#endif
		
	// Cursor types.
	typedef enum
	{
		GUI_CURSOR_ARROW,
		GUI_CURSOR_HOURGLASS,
		GUI_CURSOR_IBEAM,
	} GUI_CURSOR;

	// Icons for message boxes.
	typedef enum
	{
		GUI_ICON_NONE,
		GUI_ICON_EXCLAMATION,
		GUI_ICON_QUESTION,
		GUI_ICON_INFO,
		GUI_ICON_WARN,
		GUI_ICON_ERROR
	} GUI_ICON;

	// States for buttons in button arrays.
	const uiNUM_BUTTON_ARRAY_STATES = 3;	// Used in CCtrlPushBtnArray.
	typedef enum
	{
		GUI_BUTTON_ARRAY_OUT = 0,
		GUI_BUTTON_ARRAY_FLUSH = 1,
		GUI_BUTTON_ARRAY_IN = 2,
	} GUI_BUTTON_ARRAY_STATE;

	// GUI actions.
	typedef enum
	{
		GUI_NONE,
		GUI_OK,
		GUI_CANCEL,
		GUI_CANCEL_WITH_VERIFY,			// GUI_CANCEL_WITH_CONFIRM is probably better 8/29/94 (RAC).
		GUI_TIME_OUT,
		GUI_HELP,
		GUI_YES,
		GUI_NO,
		GUI_DEFAULT,
		GUI_YESNO,	// Yes is default.
		GUI_NOYES,	// No is default.
		GUI_YESNOCANCEL,
		GUI_OKCANCEL,
		GUI_MDI_CASCADE,
		GUI_MDI_TILE,
		GUI_MDI_ARRANGE_ICONS,
		GUI_MDI_CLOSE_ALL,
		GUI_MDI_CLOSE_ALL_WITH_VERIFY,
		GUI_PRINTER_SETUP,
		GUI_TERMINATE_APP,
		GUI_BROWSE
	} GUI_ACTION;

	typedef enum
	{
		GUI_ALIGN_LEFT,
		GUI_ALIGN_CENTER,
		GUI_ALIGN_RIGHT
	} GUI_ALIGN;

	// Bitmap display directives.
	typedef enum
	{
		GUI_BITMAP_FILL,
		GUI_BITMAP_CENTER,
		GUI_BITMAP_CLIP_LOWER_RIGHT,
		GUI_BITMAP_CLIP_UPPER_LEFT
	} GUI_BITMAP_DSP_MODE;

	// Control behavior when enclosing dialog box resizes.
	typedef unsigned int GUI_RESIZE_BEHAVIOR;

	const UINT GUI_RESIZE_GLUE_LEFT = 1;
	const UINT GUI_RESIZE_GLUE_RIGHT = 2;
	const UINT GUI_RESIZE_HORZ_STRETCH_FULL = 4;
	const UINT GUI_RESIZE_HORZ_STRETCH_PROPORTIONAL = 8;
	const UINT GUI_RESIZE_GLUE_TOP = 16;
	const UINT GUI_RESIZE_GLUE_BOTTOM = 32;
	const UINT GUI_RESIZE_VERT_STRETCH_FULL = 64;
	const UINT GUI_RESIZE_VERT_STRETCH_PROPORTIONAL = 128;

	class CGUI_Dlg;
	
	class CGUI_CallBack
	{
		public:
			CGUI_CallBack();
			virtual ~CGUI_CallBack();
		
		private:
	};
	

	// Controll types (internal use only in GUI lib.).
	typedef enum
	{
		DCT_NON_SPECIFIC,
		DCT_CHARARRAY,
		DCT_CHECKBOX,
		DCT_DATE,
		DCT_GROUP_BOX,
		DCT_PANE,
		DCT_PICKLIST,
		DCT_PLMULTISEL,
		DCT_PUSHBTN,
		DCT_PUSHBTNARRAY,
		DCT_RADIOGROUP,
		DCT_LED,
		DCT_SCROLL_BAR, 
		DCT_STATIC_TEXT, 
		DCT_STRING, 
		DCT_TABLE,
		DCT_TIME,
		DCT_TOGGLE_SWITCH,
		// Numeric Types
		DCT_CHAR,
		DCT_UCHAR,
		DCT_DOUBLE,
		DCT_FLOAT,
		DCT_INT,
		DCT_UINT,
		DCT_LONG,
		DCT_ULONG,
		DCT_SHORT,
		DCT_USHORT
	} DLG_CTRL_TYPE;
	                     
	// "Prototypes" for callback functions.
	typedef void (CGUI_CallBack::*GUI_CB_DlgNotifyFn)(CGUI_Dlg *pDlg,
		RESOURCE_ID CtrlID);
	typedef BOOL (CGUI_CallBack::*GUI_CB_DlgValidateFn)(CGUI_Dlg *pDlg,
		RESOURCE_ID CtrlID);
	typedef void (CGUI_CallBack::*GUI_CB_DlgResizeFn)(CGUI_Dlg *pDlg,
		int iDeltaWidth, int iDeltaHeight);
	typedef void (CGUI_CallBack::*GUI_CB_Fn)();
	typedef void (CGUI_CallBack::*GUI_CB_WithUserData_Fn)(void *pUserData);
	typedef BOOL (CGUI_CallBack::*GUI_CB_DlgExitFn)(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);

	class CGUI_Table;
	typedef void (CGUI_CallBack::*GUI_TableNotifyFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol);

	class CGStr;
	
	typedef BOOL (CGUI_CallBack::*GUI_CB_RequestServerFn)(CGStr ItemName, CGStr *pReturnData);
	typedef BOOL (CGUI_CallBack::*GUI_CB_ExecuteServerFn)(CGStr Cmd);

#endif
