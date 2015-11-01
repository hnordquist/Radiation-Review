
#if !defined(GUI_TBL_H)

	#define GUI_TBL_H
	#include <gui_lib.h>
	#include <gui_nfld.h>

	const UINT CGUI_TABLE_COL_WIDTH_FULL = 1;
	const UINT CGUI_TABLE_COL_WIDTH_SCALED = 2;
	const UINT CGUI_TABLE_COL_WIDTH_SAVED = 3;
	
	const int iALL_TABLE_ROWS = -1;
	const int iALL_TABLE_COLS = -1;

	// Table cell types.
	typedef enum
	{
		GUI_TABLE_BOOL_CELL,
		GUI_TABLE_LIST_CELL,
		GUI_TABLE_TEXT_CELL,
		GUI_TABLE_DOUBLE_CELL,
		GUI_TABLE_DATE_CELL,
		GUI_TABLE_TIME_CELL,
	} GUI_TABLE_CELL_TYPE;


	class CCell;
	class CTblCol;

	extern const char *aszDefaultBoolLbls[];

	class CCtrlTable;
	class CTblPickList;

const int iNUM_PREV_MSGS = 6;	//zzzzz

	class CGUI_Table {
		public:
			CGUI_Table(CGUI_Dlg *pParentDlg, RESOURCE_ID TblCtrlID, CCtrlTable **ppTableCtrl, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CGUI_Table();
			
			BOOL AreAnyCellsNULL();
			AreCellLocsValid(const char *szMsgHdr = "");
			void Clear();
			void DisplayNew();

			BOOL Print(const char *szTitle = "", const char *szData = "");

			void GetNotifyMsgCodes(UINT **nauiMsgCodes);	// Only for DataTable based tables.
			void PresentHelpMsg(UINT uiCol);
			BOOL GetStrRep(UINT uiRow, UINT uiCol, CGStr *pStr);
			
			// Get/Set table-oriented attributes.
			void SetHelpFn(void (*pHelpFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol));
			void SetNotifyPosChangeFn (void (*pNotifyPosChangeFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol, BOOL bTableHasFocus));
			void NotifyPosChange();
			void NotifyTableHasFocusChange(BOOL bTableHasFocus);
			BOOL TableHasFocus() {return(mbTableHasFocus);};
			BOOL SetColWidths(UINT uiColWidthType);
			BOOL SetColWidth(UINT uiCol, UINT uiColWidthPix);
			BOOL GetColWidth(UINT uiCol, UINT *puiColWidthPix);
			BOOL SetTableWriteEnable(BOOL bWriteEnabled);
			BOOL SuppressRepaint(BOOL bSuppressRepaint);
			BOOL IsRepaintSuppressed();

			// Set column-oriented attributes.
			void SetColLabel(UINT uiCol, const char *szColLabel);
			BOOL SetColWriteEnable(UINT uiCol, BOOL bWriteEnabled);
			BOOL SetHelpMsg(UINT uiCol, const char *szMsgText);
			BOOL SetValidateFn(UINT uiCol,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol));
	
			// Get/Set cell-oriented attributes.
			BOOL SetCellFmtStr(UINT uiRow, UINT uiCol, const char *szFormat);
			BOOL IsCellNULL(UINT uiRow, UINT uiCol);
	
			// Define date cells.
			int AddColDate(const char *szColLabel, UINT uiNumValues, struct GEN_DATE_STRUCT aDate[],
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellDate(UINT uiRow, UINT uiCol, struct GEN_DATE_STRUCT *pDate);
			BOOL CellDateSetValue(UINT uiRow, UINT uiCol, const struct GEN_DATE_STRUCT &Date);
			BOOL CellDateSetValue(UINT uiRow, UINT uiCol, const char *szFmtdDateStr);
			BOOL CellDateGetValue(UINT uiRow, UINT uiCol, struct GEN_DATE_STRUCT *pDate);

			// Define time cells.
			int AddColTime(const char *szColLabel, UINT uiNumValues, struct GEN_TIME_STRUCT aTime[],
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellTime(UINT uiRow, UINT uiCol, struct GEN_TIME_STRUCT *pTime);
			BOOL CellTimeSetValue(UINT uiRow, UINT uiCol, const struct GEN_TIME_STRUCT &Time);
			BOOL CellTimeGetValue(UINT uiRow, UINT uiCol, struct GEN_TIME_STRUCT *pTime);

			// Define text cells.
			// CGStr objects.
			int AddColTxt(const char *szColLabel, UINT uiNumValues, CGStr *apCGStr[],
				UINT uiStrDim = GUI_LIB_NO_MAX_NUM_CHAR, const char *szFormat = "%s",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellTxt(UINT uiRow, UINT uiCol, CGStr *pStr,
				UINT uiMaxDim, const char *szFormat = "%s");
			// Null-terminated character arrays - "C strings".
			int AddColTxt(const char *szColLabel, UINT uiNumValues, char *apszValues[],
				UINT uiStrDim, const char *szFormat = "%s",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellTxt(UINT uiRow, UINT uiCol, char *szText,
				UINT uiMaxStrDim, const char *szFormat = "%s");
			// Handle setup of display width for string field.
			int CGUI_Table::InitColTxt(const char *szColLabel, UINT uiNumValues, UINT uiMaxStrDim,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol));
			// Get/Set text cell values.
			BOOL TxtSetValue(UINT uiRow, UINT uiCol, const CGStr &Str);
			BOOL TxtSetValue(UINT uiRow, UINT uiCol, const char *szStr);
			BOOL TxtGetValue(UINT uiRow, UINT uiCol, CGStr *pValue);
			BOOL TxtGetValue(UINT uiRow, UINT uiCol, char *szValue, UINT uiDimension);

			// Add column of numeric cells. Define Numeric cell. Get/Set cell value.
			// Character.
			int AddColNum(const char *szColLabel, UINT uiNumValues, char acValue[],
				char cMinValue = CHAR_MIN, char cMaxValue = CHAR_MAX, const char *szFormat = "%c",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellNum(UINT uiRow, UINT uiCol, char *pcValue,
				char cMinVal, char cMaxVal, const char *szFormat = "%c");
			BOOL NumGetValue(UINT uiRow, UINT uiCol, char *pcValue);
			BOOL NumSetValue(UINT uiRow, UINT uiCol, char cValue);

			// Double.
			int AddColNum(const char *szColLabel, UINT uiNumValues, double adValues[],
				double dMinVal = -MY_DBL_MAX, double dMaxVal = MY_DBL_MAX,
				const char *szFormat = "%lf.0",
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellNum(UINT uiRow, UINT uiCol, double *pdValue,
				double dMinVal = -MY_DBL_MAX, double dMaxVal = MY_DBL_MAX,
				const char *szFormat = "%lf.0");
			BOOL NumGetValue(UINT uiRow, UINT uiCol, double *pdValue);
			BOOL NumSetValue(UINT uiRow, UINT uiCol, double dValue);
	
			// Define Check Box (Boolean) cells.
			int AddColCheckBox(const char *szColLabel, UINT uiNumValues, BOOL *abSelected,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellCheckBox(UINT uiRow, UINT uiCol, BOOL *pbValue);
			BOOL CheckBoxGetValue(UINT uiRow, UINT uiCol, BOOL *pbChecked);
			BOOL CheckBoxSetValue(UINT uiRow, UINT uiCol, BOOL bChecked);
	
			// Define Pick List cells.
			int AddColPickList(const char *szColLabel, UINT uiNumValues,
				UINT auiPickIndex[], char *naszList[] = NULL,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL DefineCellPickList(UINT uiRow, UINT uiCol, UINT *puiPickIndex);
			CTblPickList *GetPickList(UINT uiCol);
			BOOL PickListSetValue(UINT uiRow, UINT uiCol, UINT uiPickIndex);
			BOOL PickListGetValue(UINT uiRow, UINT uiCol, UINT *puiPickIndex,
				CGStr *pPickedItemText = NULL);
			BOOL PickListGetValue(UINT uiRow, UINT uiCol, UCHAR *pucPickIndex,
				CGStr *pPickedItemText = NULL);

			// Define Combo List cells.
			int AddColComboList(const char *szColLabel, UINT uiNumValues,
				UINT auiPickIndex[], char *naszList[] = NULL,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol) = NULL);
			BOOL ComboListAdd(UINT uiCol, const char *szNewItem);
			BOOL ComboListClear(UINT uiCol);
			BOOL ComboListGetValue(UINT uiRow, UINT uiCol, int *piPickIndex,
				CGStr *pPickedItemText = NULL);
			BOOL ComboListSetValue(UINT uiRow, UINT uiCol, int iPickIndex);
			
			BOOL IsConversionErrPending(BOOL bPresentErrMsg = FALSE);
			void RegisterConversionErr(BOOL bErrExists, UINT uiRow = 0, UINT uiCol = 0,
				const char *szErroneousEntry = NULL);
			BOOL GoToConversionErr();
			BOOL MoveFocusToTable();
			void NotifyTableCtrlDeleted(CCtrlTable *pTblCtrl);
			
			// Get, Set current row, col.
			BOOL GetCurrentPos(UINT *puiRow, UINT *puiCol);
			BOOL SetCurrentPos(UINT uiRow, UINT uiCol);
			// SetCurrentCell is obsolete -- use SetCurrentPos
			BOOL SetCurrentCell(UINT uiRow, UINT uiCol) {return(SetCurrentPos(uiRow, uiCol));};
			
			BOOL SetNewValue(UINT uiRow, UINT uiCol, double dData);
			BOOL ConvertFld(UINT uiRow, UINT uiCol, const char *szText,
				BOOL bPresentErrMsg = TRUE);
			static BOOL CurFldStrToVal(const char *szText);
			static BOOL CurFldValToStr(char *szDspBuffer, UINT uiBufferDim);

			BOOL UpdateAllCells();
			BOOL UpdateCell(UINT uiRow, UINT uiCol);
			BOOL UpdateCol(UINT uiCol);
			BOOL UpdateRow(UINT uiRow);
			
			// Retrieve cell values.
			BOOL RetrieveAllCellValues();

			static CGUI_Table *GetCurrentTable();
			BOOL GetColDataLen(UINT uiCol, UINT *puiDataLen);

			void NotifyDoubleClick(UINT uiRow, UINT uiCol);
//#ifdef WIN32
			void SetDoubleClickNotifyFn(CGUI_CallBack *pCB_Obj,	GUI_TableNotifyFn pNotifyFn);
//#endif
			BOOL GetCellColor(UINT uiRow, UINT uiCol, COLORREF *pFgColor, COLORREF *pBkgColor);
			BOOL SetCellColor(int iRow, int iCol, COLORREF FgColor = CGUI_Table::mDEFAULT_FG_COLOR,
				COLORREF BkgColor = CGUI_Table::mDEFAULT_BG_COLOR);

			BOOL DeleteRow(UINT uiRow);
			BOOL InsertRow(UINT uiNewRowIndex);
			BOOL MoveRow(UINT uiOrigRowIndex, UINT uiNewRowIndex);
			BOOL SwapRows(UINT uiRow1, UINT uiRow2);
			BOOL DeleteCol(UINT uiCol);
			
			static const COLORREF mDEFAULT_FG_COLOR, mDEFAULT_BG_COLOR;
			CCell *GetCell(UINT uiRow, UINT uiCol);
			CTblCol *GetTblCol(UINT uiColNum);
			CCtrlTable *mpTableCtrl;
			CGUI_Dlg *mpTblDlg;
			BOOL IsErrorConvert() {return mbConversionErrPending;};
			BOOL IsSuppressRepaint() {return mbSuppressRepaint;};
			zComboBoxFull *GetComboCtrl();
			BOOL SetPopupMenu(CGUI_Menu *pPopupMenu, void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			CGUI_Menu *GetPopupMenu() {return(mpPopupMenu);};

		protected:
			friend void CGUI_Dlg::CreateTableCtrls();
            friend int CGUI_Dlg::TableMsgHandler(zEvent *pEvent);
			friend int CGUI_Dlg::command(zCommandEvt *pEvent);

			// Next member functions for use by CGUI_Dlg only.
			BOOL Create();
			BOOL HandleVM_Msg(WORD wMsg, long lParam);
//			BOOL HandleNotificationMsg(int iMsgID);
			BOOL HandleNotificationMsg(zCommandEvt *pEvent);
			
		private:
			BOOL mbTableCtrlInitialized;
			BOOL mbTableHasFocus;
			void (*mpNotifyPosChangeFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol, BOOL bTableHasFocus);
			void (*mpHelpFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol);
			BOOL (*GetUserValidationFn(UINT uiCol))(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol);
			BOOL GetPaddedCellText(UINT uiRow, UINT uiCol, CGStr *pCellTextStr);
			void SetCurrentCellColor(COLORREF Color);
			COLORREF mCurrentCellColor;
			COLORREF mCellBkgColor;
			COLORREF mCellTextColor;
//CGStr mPrevMsg[iNUM_PREV_MSGS];
			void SetStatusMsg(const char *szHelpMsg);
			zRect StatusMsgRect;
			void ComputeTblDspDim(UINT *puiPixHeight, UINT *puiPixWidth, UINT *puiRowLblWidth);
			void PresentMsg(const CGStr &Msg);

			// Handle conversion error.
			BOOL mbConversionErrPending;
			CGStr mErroneousText;
			BOOL mbProcessingConversionErr;
			UINT muiErrCol;
			UINT muiErrRow;
			
			// Have CGUI_Table be a wrapper for the actual table class
			// rather than derived from to minimize exposure of GUI Lib
			// API consumer to implementation details of the actual table
			// class -- specifically include files.
			CTblCol **mapTblCol;
			HWND m_hWnd;
			int miID;
			RESOURCE_ID mTblCtrlID;
			UINT muiNumCols;	// Data columns only -- row labels not included.
			UINT muiMaxNumRows;	// Longest column; data rows only -- column labels do not count.
			void *PtrCellData(UINT uiRow, UINT uiCol);
			BOOL IsCellContentsValid(UINT uiNotifyRow, UINT uiNotifyCol);
			BOOL GetNotifyPos(UINT *puiRow, UINT *puiCol);
			//zzzzzzz DWORD GetRowData(DWORD dwRowIndex, void *pRowData);
			int AddCol(const char *szColLabel, UINT uiNumRows,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol),
				UINT uiDataLen, UINT uiMaxCellDspWidthPix = 0);
			void Update_zTableModel(UINT uiRow, UINT uicol);
			UINT muiColWidths;
			UINT *mauiColFullWidthsPix;
			UINT *mauiColFullWidthsChar;
			UINT muiTableWidth;
			UINT muiTableHeight;
			UINT muiMaxHeadingLines;

			BOOL mbSuppressRepaint;
			BOOL RegisterCell(UINT uiRow, UINT uiCol, CCell *pNewCell, UINT uiDataLen = 0);
			void ComputeColWidths(BOOL bPrinter = FALSE);
			void ComputeLayout(const char *aszBtnLabels[]);
			void DeleteTblCols();
			void GetMetrics(const char *aszBtnLabels[]);
			BOOL UpdateUserVar(UINT uiRow, UINT uiCol);
			BOOL PrepareColData(CGStr **apCGStrValues, UINT uiDim,
				UINT *puiMaxDataWidth);
			UINT muiTblWidth;
			UINT muiTblHeight;
			BOOL IsValidCol(UINT uiColNum);
			BOOL IsValidRow(UINT uiRowNum);
			BOOL mbRowLbls;
			BOOL mbColLbls;
			UINT muiRowHeight;
			BOOL mbAllowRowDelete;
			static UINT muiDefaultColWidth;
			static CGUI_Table *mpCurrentFocusTable;
			CGUI_CallBack *mpCBO_DoubleClickObj;
//#ifdef WIN32
			GUI_TableNotifyFn mpCBO_DoubleClickFn;
//#endif
			CGUI_Menu *mpPopupMenu;
	};

	
	class CTblPickList {
		public:
			CTblPickList(char *naszPickList[] = NULL);
			~CTblPickList();
			BOOL Add(const char *szChoiceText);
			//BOOL ModifyItemText(UINT uiPickIndex, const char *szChoiceText);
			BOOL GetItemText(UINT uiWhichItem, CGStr *pItemText);
			BOOL Clear();
			UINT GetNumItems();
			UINT GetLongestItemDim();
			
		private:
        	CGStr **mapPickListItems;
        	UINT muiNumItems;

	};
	
#endif
