
// GUI_TxWn.h

#ifndef GUI_TXWN_H
	#define GUI_TXWN_H

//	#include "gen_fstr.h"
	#include <limits.h>
	
	// UpperLeft, Top, UpperRight, Right Side, LowerRight, Bottom, LowerLeft, Left Side
	const UINT uiNUM_BOX_DRAW_CHAR = 8;
	// Next constant is intended for use with SetMaxCols() & SetMaxRows().
	const UINT GUI_TXWN_NO_MAX = 0;         
	// Next two constants are intended for use with SetTopDspLine().
	const UINT uiTOP_OF_LISTING = 0;
	const UINT uiBOTTOM_OF_LISTING = UINT_MAX;

	const UINT uiEND_OF_LINE = UINT_MAX;
	
	const UINT uiDEFAULT_PRINTER_FONT_SIZE = 7;		// 7.5 inches * 72 pts/inch / 80 cols => 6.75 pts.
	const UINT uiDEFAULT_PRINTER_HT_TO_WIDTH_RATIO = 18;	// 10 inches * 72 pts/inch / 60 lines => 12 pts.
															// 12 pts. / 6.75 pts. => ~1.8 so to use integer
															// arithmetic multiply by 18 and then divide by 10.
	
	class CGStr;
	class CGUI_TextWin;
	class CGUI_SDITextWin;
	class CScrollTextPane;
	class CGUI_PrintJob;
	class CGFmtStr;
	class CMy_zPaneScrollerAuto;
	class CTW_StatusBarPane;
	declDlist(CGUI_TextWin)
	declDlist(CGUI_SDITextWin)
	
	const UINT GUI_HiLiteBox = 1;
	const UINT GUI_HiLiteBold = 2;

	typedef enum
	{
		TW_FONT_NORMAL,
		TW_FONT_BOLD
	} TW_FONT;



	// ============================================================================
	// ========================== CGUI_TextWin ====================================
	// ============================================================================

	class CGUI_TextWin : public zMDIChildFrame {
		public:
			// ================================================================
			// ============= Public Fns intended for API Consumer =============
			// ================================================================
			CGUI_TextWin(const char *szTitle, UINT uiInitDspRows = 0,
				UINT uiInitDspCols = 0, BOOL bInitVisible = TRUE,
				BOOL bStatusBar = FALSE);
			virtual ~CGUI_TextWin();
			static void AllowPrinterFontScaling(UINT uiMinPts = uiDEFAULT_PRINTER_FONT_SIZE,
				UINT uiMaxPts = uiDEFAULT_PRINTER_FONT_SIZE, CGUI_TextWin *pTextWin = NULL);
			UINT GetMaxColsPerPage();
			void Clear();
			void EnableUserClose(BOOL (*pUserCloseNotifyFn)(CGUI_TextWin *pWin) = NULL);
			void GetNumCols(UINT *puiNumCols, UINT *puiMaxNuCols = NULL);
			void GetNumLines(UINT *puiNumLines, UINT *puiMaxNumLines = NULL);
			BOOL GetText(CGStr **ppLineText, UINT uiLineNum, BOOL *pbBold = NULL);
			const char *GetTitle();
			BOOL GetVisibleDim(UINT *puiVisibleRows,
				UINT *puiVisibleCols);
			void Iconize(BOOL bIconized);
			BOOL IsLastLineShowing();
			void LineDelete(UINT uiLineNumToDelete);
			void LineInsert(UINT uiLineNumInsertBefore);
			void LineReplacePrintf(UINT uiLineNumToReplace, const char *szFormat, ...);
			BOOL Print(BOOL bDeleteAfterPrinting = FALSE, BOOL bHeader = TRUE,
				BOOL bClearAfterPrinting = FALSE, BOOL bPrintSelOnly = FALSE, UINT uiMaxColsPerPage = 0, BOOL bInhibitPageNo =  FALSE);
			BOOL Printf(const char *szFormat, ...);
			BOOL Printf(TW_FONT eFontType, const char *szFormat, ...);
			void RegisterCloseNotifyFn(void (*pCloseNotifyFn)(CGUI_TextWin *pWin));
			BOOL SaveAs(const char *szFileName = NULL, BOOL bSkipDialog = FALSE,
				BOOL bAppendIfExists = FALSE, BOOL bSaveSelOnly = FALSE);
			void SetDisplaySize(UINT uiRows = 0, UINT uiCols = 0);
			// If default 0 is given, use DEFAULT uiMAX_DEFAULT_INIT_ROWS(COLS)
			// if actual rows(cols) == 0; otherwise use current actual num rows(cols).
			BOOL SetIcon(RESOURCE_ID IconID);
			UINT SetMaxCols(UINT uiMaxCols);
			UINT SetMaxRows(UINT uiMaxRows);
			void SetFocus();
			void SetNonScrollingHeader(const char *szHeaderText = NULL);
			void SetTitle(const char *szTitle);
			UINT SetTopDspLine(UINT uiReqTopLine);
			void Show(BOOL bVisible);
			BOOL Write(const char *szText);
		
			void UpdateStatusBar(UINT uiCurrentTopLine);
			void UpdateStatusBarTotalLines(UINT uiTotalLines);
			void GetSelection(BOOL *pbSelectionActive, UINT *puiFirstSelectedLine,
				UINT *puiLastSelectedLine);
			void SetSelection(BOOL bSelectionActive, UINT uiFirstSelectedLine,
				UINT uiLastSelectedLine);

			void SetTitleLines(UINT uiNumTopTitleLines);
			//void SplitScreen(double dTopViewFraction);
			//void RegisterMouseHitFn(void (*UserNotifyFn)(CGUI_TextWin *pWin,
			//	UINT *puiLineDn, UINT *puiColDn, UINT *puiLineUp, UINT *puiColUp));
			void GetSelection(UINT *puiFirstLine, UINT *puiLastLine);
			BOOL GetStatusBarVisible();
			void ShowStatusBar(BOOL bVisible);

			static void SetBoxDrawChar(const char acDrawChar[uiNUM_BOX_DRAW_CHAR] = NULL);
			void DrawBox(UINT uiTopRow, UINT uiBottomRow, UINT uiFirstCol, UINT uiLastCol);
			void StartBox();
			void EndBox(BOOL bBoldIfPossible = FALSE, UINT uiFirstBoxCol = 0, UINT uiLastBoxCol = 0);
			void SetLineBolding(BOOL bBold, UINT uiFirstBoldLine = UINT_MAX, UINT uiLastBoldLine = UINT_MAX);
			void StartLineBolding();
			void EndLineBolding();
			BOOL AnyLinesBolded();

			// ================================================================
			// =========== Public Fns NOT intended for API Consumer ===========
			// ================================================================
			BOOL AddSysMenuItem(const char *szMenuItemText,
				void (*UserNotifyFn)(CGUI_TextWin *pWin, const char *szMenuItemText), BOOL bActive);
			BOOL CopyToClipBoard(BOOL bCopySelOnly = FALSE);
			void FinishedPrintingNotifyFn(BOOL *pbDeleteAfterPrinting);
			static CGUI_TextWin *GetCurrentWindow();
			static void MDI_CloseAll();
			static int HandleMenuCmd(zEvent *pEvent);
			BOOL Printing();
			void PrintCleanupFn(zPrinterDisplay*,zPrintJob*);
			void SetDisplayFont(const char *szFontName, int iPointSize);
			void SetPrinterFont(const char *szFontName, int iPointSize);
			static UINT GetNumTextWindows();

#ifdef GUI_LIB_PRIVATE		
			BOOL GetLineBolding(UINT uiLineIndex) {return(mabBoldLine[uiLineIndex]);};
			BOOL IsLastLineEmpty() {return(mbLastLineEmpty);};
			void GetLastLineText(CGStr **ppLineText) {*ppLineText = mapLineStr[muiNumRows];};
		protected:
			void Invalidate();
			int MenuHitNotifyFn(zEvent *pEvent);
			void AddTextWin(CGUI_TextWin *pTextWin);
			void RemoveTextWin(CGUI_TextWin *pTextWinArg);
			BOOL FindPrintPendingTextWin(CGUI_TextWin **ppTextWin);
			BOOL LastLineShowing();
			void SetSysCloseEnable(BOOL bNewEnable);
			int NotifyKill(zEvent *pEvnt);
			BOOL ReallocateLineStrArray(UINT uiNumLines);
			BOOL IncrementLineStrArray();
			int MDIActivate(zActivateEvt *pEvnt);
			int focus(zFocusEvt *pEvnt);
			zSystemMenu *mpSysMenu;
			BOOL mbSelectionActive;
			UINT muiFirstSelectedLine;
			UINT muiLastSelectedLine;
			BOOL mbStatusBar;
			UINT muiStatusBarHeight;
			zFont *mpzFont;
			CScrollTextPane *mpTextPane;
			CTW_StatusBarPane *mpStatusBar;
			UINT muiLineStrDim;
			UINT muiCurrentRow;
			UINT muiCurrentCol;
			UINT muiNumRows;
			UINT muiNumCols;
			UINT muiMaxRows;
			UINT muiMaxCols;
			BOOL mbPrintPending;
			BOOL mbClearAfterPrinting;
			BOOL mbDeleteAfterPrinting;
			BOOL mbHeader;
			BOOL mbPrintSelOnly;
			CGUI_PrintJob *mpPrintJob;
			zIcon *mpzIcon;

			static UINT muiMinPrinterFontSize;
			static UINT muiMaxPrinterFontSize;
			static BOOL mbHasSetInitialOffset;
			static int miLeftOffset;
			static int miTopOffset;
			static CGUI_TextWin *mpTextWinLatestFocus;
			static CGUI_TextWinDlist *mpTextWinDlist;
			static BOOL mbPrintingActive;
			BOOL mbVisible;
			CGStr huge **mapLineStr;	                             
			BOOL huge *mabBoldLine;
			static char macUserBoxDrawChar[uiNUM_BOX_DRAW_CHAR];
			static BOOL mbUserBoxDrawCharDefined;
			UINT muiFirstBoxLine;
			BOOL mbBoxInProgress;
			BOOL mbLineBoldingOn;
			BOOL mbLineDrawCharAvail;
			BOOL mbUserCanClose;
			BOOL (*mpUserCloseNotifyFn)(CGUI_TextWin *pWin);
			void (*mpCloseNotifyFn)(CGUI_TextWin *pWin);
			BOOL mbUserCloseFnCalled;
			BOOL mbLastLineEmpty;
#endif	// GUI_LIB_PRIVATE
	};
	
	
	// ============================================================================
	// ========================== CGUI_SDITextWin ====================================
	// ============================================================================

	class CGUI_SDITextWin : public zDialogFrame {
		public:
			// ================================================================
			// ============= Public Fns intended for API Consumer =============
			// ================================================================
			CGUI_SDITextWin(const char *szTitle, UINT uiInitDspRows = 0,
				UINT uiInitDspCols = 0, BOOL bInitVisible = TRUE,
				BOOL bStatusBar = FALSE);
			virtual ~CGUI_SDITextWin();
			static void AllowPrinterFontScaling(UINT uiMinPts = uiDEFAULT_PRINTER_FONT_SIZE,
				UINT uiMaxPts = uiDEFAULT_PRINTER_FONT_SIZE, CGUI_SDITextWin *pTextWin = NULL);
			UINT GetMaxColsPerPage();
			void Clear();
			void EnableUserClose(BOOL (*pUserCloseNotifyFn)(CGUI_SDITextWin *pWin) = NULL);
			void GetNumCols(UINT *puiNumCols, UINT *puiMaxNuCols = NULL);
			void GetNumLines(UINT *puiNumLines, UINT *puiMaxNumLines = NULL);
			BOOL GetText(CGStr **ppLineText, UINT uiLineNum, BOOL *pbBold = NULL);
			const char *GetTitle();
			BOOL GetVisibleDim(UINT *puiVisibleRows,
				UINT *puiVisibleCols);
			void Iconize(BOOL bIconized);
			BOOL IsLastLineShowing();
			void LineDelete(UINT uiLineNumToDelete);
			void LineInsert(UINT uiLineNumInsertBefore);
			void LineReplacePrintf(UINT uiLineNumToReplace, const char *szFormat, ...);
			BOOL Print(BOOL bDeleteAfterPrinting = FALSE, BOOL bHeader = TRUE,
				BOOL bClearAfterPrinting = FALSE, BOOL bPrintSelOnly = FALSE, UINT uiMaxColsPerPage = 0);
			BOOL Printf(const char *szFormat, ...);
			BOOL Printf(TW_FONT eFontType, const char *szFormat, ...);
			void RegisterCloseNotifyFn(void (*pCloseNotifyFn)(CGUI_SDITextWin *pWin));
			BOOL SaveAs(const char *szFileName = NULL, BOOL bSkipDialog = FALSE,
				BOOL bAppendIfExists = FALSE, BOOL bSaveSelOnly = FALSE);
			void SetDisplaySize(UINT uiRows = 0, UINT uiCols = 0);
			// If default 0 is given, use DEFAULT uiMAX_DEFAULT_INIT_ROWS(COLS)
			// if actual rows(cols) == 0; otherwise use current actual num rows(cols).
			BOOL SetIcon(RESOURCE_ID IconID);
			UINT SetMaxCols(UINT uiMaxCols);
			UINT SetMaxRows(UINT uiMaxRows);
			void SetFocus();
			void SetNonScrollingHeader(const char *szHeaderText = NULL);
			void SetTitle(const char *szTitle);
			UINT SetTopDspLine(UINT uiReqTopLine);
			void Show(BOOL bVisible);
			BOOL Write(const char *szText);
		
			void UpdateStatusBar(UINT uiCurrentTopLine);
			void UpdateStatusBarTotalLines(UINT uiTotalLines);
			void GetSelection(BOOL *pbSelectionActive, UINT *puiFirstSelectedLine,
				UINT *puiLastSelectedLine);
			void SetSelection(BOOL bSelectionActive, UINT uiFirstSelectedLine,
				UINT uiLastSelectedLine);

			void SetTitleLines(UINT uiNumTopTitleLines);
			//void SplitScreen(double dTopViewFraction);
			//void RegisterMouseHitFn(void (*UserNotifyFn)(CGUI_SDITextWin *pWin,
			//	UINT *puiLineDn, UINT *puiColDn, UINT *puiLineUp, UINT *puiColUp));
			void GetSelection(UINT *puiFirstLine, UINT *puiLastLine);
			BOOL GetStatusBarVisible();
			void ShowStatusBar(BOOL bVisible);

			static void SetBoxDrawChar(const char acDrawChar[uiNUM_BOX_DRAW_CHAR] = NULL);
			void DrawBox(UINT uiTopRow, UINT uiBottomRow, UINT uiFirstCol, UINT uiLastCol);
			void StartBox();
			void EndBox(BOOL bBoldIfPossible = FALSE, UINT uiFirstBoxCol = 0, UINT uiLastBoxCol = 0);
			void SetLineBolding(BOOL bBold, UINT uiFirstBoldLine = UINT_MAX, UINT uiLastBoldLine = UINT_MAX);
			void StartLineBolding();
			void EndLineBolding();
			BOOL AnyLinesBolded();
			
			// ================================================================
			// =========== Public Fns NOT intended for API Consumer ===========
			// ================================================================
			BOOL AddSysMenuItem(const char *szMenuItemText,
				void (*UserNotifyFn)(CGUI_SDITextWin *pWin, const char *szMenuItemText), BOOL bActive);
			BOOL CopyToClipBoard(BOOL bCopySelOnly = FALSE);
			void FinishedPrintingNotifyFn(BOOL *pbDeleteAfterPrinting);
			static CGUI_SDITextWin *GetCurrentWindow();
			static void SDI_CloseAll();
			BOOL Printing();
			void PrintCleanupFn(zPrinterDisplay*,zPrintJob*);

#ifdef GUI_LIB_PRIVATE		
			BOOL GetLineBolding(UINT uiLineIndex) {return(mabBoldLine[uiLineIndex]);};
			BOOL IsLastLineEmpty() {return(mbLastLineEmpty);};
			void GetLastLineText(CGStr **ppLineText) {*ppLineText = mapLineStr[muiNumRows];};
		private:
			void Invalidate();
			int MenuHitNotifyFn(zEvent *pEvent);
			void AddTextWin(CGUI_SDITextWin *pTextWin);
			void RemoveTextWin(CGUI_SDITextWin *pTextWinArg);
			BOOL FindPrintPendingTextWin(CGUI_SDITextWin **ppTextWin);
			BOOL LastLineShowing();
			void SetSysCloseEnable(BOOL bNewEnable);
			int NotifyKill(zEvent *pEvnt);
			BOOL ReallocateLineStrArray(UINT uiNumLines);
			BOOL IncrementLineStrArray();
			int focus(zFocusEvt *pEvnt);
			zSystemMenu *mpSysMenu;
			BOOL mbSelectionActive;
			UINT muiFirstSelectedLine;
			UINT muiLastSelectedLine;
			BOOL mbStatusBar;
			UINT muiStatusBarHeight;
			zFont *mpzFont;
			CScrollTextPane *mpTextPane;
			CTW_StatusBarPane *mpStatusBar;
			UINT muiLineStrDim;
			UINT muiCurrentRow;	                             
			UINT muiCurrentCol;
			UINT muiNumRows;
			UINT muiNumCols;
			UINT muiMaxRows;
			UINT muiMaxCols;
			BOOL mbPrintPending;
			BOOL mbClearAfterPrinting;
			BOOL mbDeleteAfterPrinting;
			BOOL mbHeader;
			BOOL mbPrintSelOnly;
			CGUI_PrintJob *mpPrintJob;
			zIcon *mpzIcon;

			static UINT muiMinPrinterFontSize;
			static UINT muiMaxPrinterFontSize;
			static BOOL mbHasSetInitialOffset;
			static int miLeftOffset;
			static int miTopOffset;
			static CGUI_SDITextWin *mpTextWinLatestFocus;
			static CGUI_SDITextWinDlist *mpTextWinDlist;
			static BOOL mbPrintingActive;
			BOOL mbVisible;
			CGStr huge **mapLineStr;	                             
			BOOL huge *mabBoldLine;
			static char macUserBoxDrawChar[uiNUM_BOX_DRAW_CHAR];
			static BOOL mbUserBoxDrawCharDefined;
			UINT muiFirstBoxLine;
			BOOL mbBoxInProgress;
			BOOL mbLineBoldingOn;
			BOOL mbLineDrawCharAvail;
			BOOL mbUserCanClose;
			BOOL (*mpUserCloseNotifyFn)(CGUI_SDITextWin *pWin);
			void (*mpCloseNotifyFn)(CGUI_SDITextWin *pWin);
			BOOL mbUserCloseFnCalled;
			BOOL mbLastLineEmpty;
#endif	// GUI_LIB_PRIVATE
	};

		
	// ============================================================================
	// ========================== CGUI_PrintJob ===================================
	// ============================================================================

	class CGUI_PrintJob : public zPrintJob
	{
		public:
			//CGUI_PrintJob(zPane *, zPrinterDisplay *, zPrJobDlg *, CGUI_TextWin *);
			//CGUI_PrintJob(zPrinterDisplay *, zPrJobDlg *, CGUI_TextWin *);
			CGUI_PrintJob(zPane *, zPrinterDisplay *, zPrJobDlg *);
			CGUI_PrintJob(zPrinterDisplay *, zPrJobDlg *);
			virtual ~CGUI_PrintJob();
#ifdef GUI_LIB_PRIVATE		
		private:
			CGUI_TextWin *mpTextWin;
#endif	// GUI_LIB_PRIVATE
	};


	// ============================================================================
	// ========================== CScrollTextPane =================================
	// ============================================================================

	class zMyPrinterDisplay;
	class CGUI_PrintJob;
	class CGUI_PrJobDlg;
	class CGUI_SDITextWin;

	class CScrollTextPane : public zPane {
		friend void CGUI_TextWin::FinishedPrintingNotifyFn(BOOL *pbDeleteAfterPrinting);
		friend void CGUI_SDITextWin::FinishedPrintingNotifyFn(BOOL *pbDeleteAfterPrinting);
		
		public:
			CScrollTextPane(CGUI_TextWin *, zSizer *, DWORD style,
				UINT uiRows = 25, UINT uiCols = 80);
			CScrollTextPane(CGUI_SDITextWin *, zSizer *, DWORD style,
				UINT uiRows = 25, UINT uiCols = 80);
			~CScrollTextPane();
			BOOL GetCharSize(UINT *puiCharWidthPixels,
				UINT *puiCharHeightPixels);
			BOOL IsLastLineShowing();
			void GetCurrentDisplayedLines(UINT *puiTopDipslayedLine,
				UINT *pBottomDisplayedLine);
			BOOL Print(CGUI_PrintJob **ppPrintJob, BOOL bHeader = TRUE,
				BOOL bPrintSelOnly = FALSE, UINT uiMaxColsPerPage = 0, BOOL bInhibitPageNo = FALSE);
			void AllowPrinterFontScaling(UINT uiMinPts, UINT uiMaxPts);
			UINT GetMaxColsPerPage();
			void SetFullSizeRowsCols(UINT uiRows, UINT uiCols);
			void SetNonScrollingHeader(const char *szHeaderText);
			UINT SetTopDspLine(UINT uiTopDipslayedLine);
			void Invalidate();
			UINT SetMaxRows(UINT uiReqMaxRows);
			static void PrinterDisplayDeletedNotifyFn(CScrollTextPane *);
			static void NotifyJobCanceled(CScrollTextPane *);

#ifdef GUI_LIB_PRIVATE		
			void SetDisplayFont(zFont *pFont) {mpDisplayFont=pFont;};
			void SetPrinterFont(zFont *pFont) {mpUsrPrinterFont=pFont;};
			void SetBoldPrinterFont(zFont *pFont) {mpBoldPrinterFont=pFont;};

		protected:
			void DeletePrinterLineStrArrays();
			
		private:
			UINT muiUserSpecifiedMaxColsPerPage;
			void HandlePrinterDisplayDelete();
			CGUI_PrintJob *mpPJ;
			CGUI_PrJobDlg *mpPJDlg;
			zMyPrinterDisplay *mpPD;
			UINT muiMinPrinterFontSize;
			UINT muiMaxPrinterFontSize;
			BOOL LineToVisRect(UINT uiLineIndex, RECT *pRect);
			void InvalidateLine(UINT uiLineIndex);
			UINT MouseVertPixToLine(zEvent *pEvent, int *pRelVert = NULL);
			UINT MouseVertPixToLine(int iVertPix, int *pRelVert);
			void SetSelectionState(zEvent *pEvent);
			int KeyNotifyFn(zEvent *pEvent);
			int NotifyButtonDown(zEvent *pEvent);
			int NotifyMouseMove(zEvent *pEvent);
			int NotifyButtonUp(zEvent *pEvent);
			UINT muiNumNonScrollingHeaderLines;
			CGStr mNonScrollingHeaderText;
			UINT muiCharHeight;
			UINT muiCharWidth;
			UINT muiVisHeightPix;
			BOOL mbMouseDown;
			BOOL mbMoveTrackingAlreadyActive;
			BOOL mbSelectionActive;
			UINT muiMouseDownLine;
			UINT muiMouseUpLine;
			int miMouseDownPix;
			UINT muiFirstSelectedLine;
			UINT muiLastSelectedLine;
			static const muiNumPrntHdrLines;
			BOOL mbLineScrollMode;
			zFont *mpBoldPrinterFont;
			zFont *mpPrinterFont;
			zFont *mpUsrPrinterFont;
			CGFmtStr mPrintDateStr;
			CGFmtStr mPrintTimeStr;
			CGStr *maPrinterLineStr;
			UINT muiPrinterLineStrCurrentIndex;
			UINT muiPrinterLineStrNumLines;
			CGStr *maPrevPagePrinterLineStr;
			UINT muiPrevPagePrinterLineStrCurrentIndex;
			UINT muiPrevPagePrinterLineStrNumLines;
			BOOL mbHeader;
			BOOL mbPrintSelOnly;
			UINT muiLinesPerPage;
			UINT muiPageNum;
			BOOL mbFirstDrawThisPage;
			UINT muiNumPages;
			UINT muiNumExtraWrappedLines;
			UINT muiNumExtraWrappedLinesPrevPages;
			zFont *mpDisplayFont;
			static zFont *mpBoldDisplayFont;
			UINT GetMaxWindowsRows();
			void SetScrollerIncrement();
			void CreatePrinterReportHdr(CGFmtStr *pHdrStr, UINT uiCharPerLine);
			void CreatePrinterPageHdr(CGFmtStr *pHdrStr, UINT uiCurrentPageNum);
			void PrintPageHeader(UINT uiCharPerLine,
				UINT uiLeftPixel, UINT uiCharHeight, UINT *puiTopPixel);
			virtual int scroll(zEvent *pEvent);
			virtual int size(zSizeEvt *pEvent);
			virtual int print(zPrinterDisplay *pPD, zRect *);
			virtual BOOL morePages();
			int CurrentPrinterPageTopLine();
			BOOL CreatePrinterFont(UINT *puiMaxCharPerLine, UINT uiDesiredCharPerLine = 0,
				zFont **ppPrinterFont = NULL, zFont **ppBoldPrinterFont = NULL);
			void DeterminePrinterFont(UINT *puiCharPerLine, UINT *puiLeftPixel);
			void InitializePrinterDrawing(UINT *puiCharPerLine, UINT *puiLeftPixel);
			int GetText(CGStr **ppLineText, int *piCurrentLineIndex, BOOL bPrinting,
				UINT uiMaxCharPerLine, CGStr *pLineTextBuffer, BOOL *pBoldLine);
			virtual int draw(zDrawEvt *);
			UINT muiFullNumRows;
			UINT muiFullNumCols;
			CGUI_TextWin *mpParent;
			CGUI_SDITextWin *mpSDIParent;
			BOOL getPreferredSize(zDimension& d);
			CMy_zPaneScrollerAuto *pScroller;
			BOOL mbPrintJobCanceled;
			BOOL mbPrintPending;	// TRUE from time of print request until finished printing.
			BOOL mbInhibitPageNo;
			int miFirstDirtyLine;
			int miFormFeedStatus;
			CGStr *mpBuffer;
#endif	// GUI_LIB_PRIVATE
	};


	/*
		FLAGS:
			UserCloseable;
			UserSizeable;
			Maximized;
			
	*/		

#endif // GUI_TXWN_H
