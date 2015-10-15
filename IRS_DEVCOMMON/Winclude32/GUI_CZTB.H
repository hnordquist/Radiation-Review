
#ifndef GUI_CZTB_H
	#define GUI_CZTB_H
	
	class CGUI_Table;
	class CSplitPane;
	class CTableFrame;

	#include "gui_tbl.h"

	class CCtrlTable : public CGUI_DlgCtrl
	{
		public:
			CCtrlTable(CGUI_Dlg *pDlg, CGUI_Table *pParentTable, zSizer *pSizer, RESOURCE_ID CtrlID, RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlTable();
			BOOL AreCellLocsValid(const char *szMsgHdr);
			void Clear();
			BOOL Start(UINT uiNumRows);

			virtual BOOL GetPosition(int *piLeft, int *piTop, BOOL bClientRelative = TRUE);
			virtual BOOL SetPosition(int iLeft, int iTop);
			virtual BOOL GetSize(UINT *puiWidth, UINT *puiHeight);
			virtual BOOL SetSize(UINT uiWidth, UINT uiHeight);

			virtual BOOL GetHelpMsg(CGStr *pHelpMsg = NULL) const;
			void Invalidate();
			BOOL UpdateDisplay(UINT uiRow, UINT uiCol);
			void Update_zTableModel(UINT uiRow, UINT uiCol);
			BOOL SetColWriteEnable(UINT uiCol, BOOL bWriteEnabled);
			BOOL SetHeadingHeight(UINT uiNumLines);
			UINT GetCurRow();
			UINT GetCurCol();
			BOOL EditCell(UINT uiRow, UINT uiCol, const char *szText);
			BOOL SetCurCell(UINT uiRow, UINT uiCol);
			BOOL GetCurCell(UINT *puiRow, UINT *puiCol, CGStr *pText = NULL);
			
			BOOL SetColWidth(UINT uiCol, UINT uiColWidth);
			BOOL GetColWidth(UINT uiCol, UINT *puiColWidth);

			BOOL DeleteRow(UINT uiRow);
			BOOL InsertRow(UINT uiNewRowIndex);
			BOOL InsertColCells(UINT uiNewRowIndex, UINT uiColIndex, UINT uiNumNewCells = 1);
			BOOL MoveRow(UINT uiOrigRowIndex, UINT uiNewRowIndex);
			BOOL SwapRows(UINT uiRow1, UINT uiRow2);
			BOOL DeleteCol(UINT uiCol);

			BOOL InvalidateCell(UINT uiRow, UINT uiCol);

			BOOL CreateCell(UINT uiRow, UINT uiCol, GUI_TABLE_CELL_TYPE eCellType);
			BOOL CreateREAL8_Col(UINT uiDspWidth, const CGStr &ColHeading, UINT uiNumRows);
			BOOL CreateCheckBoxCol(UINT uiDspWidth, const CGStr &ColHeading, UINT uiNumRows);
			BOOL CreateTxtCol(UINT uiDspWidth, const CGStr &ColHeading, UINT uiNumrows);
			BOOL CreateDateCol(UINT uiDspWidth, const CGStr &ColHeading, UINT uiNumrows);
			BOOL CreateTimeCol(UINT uiDspWidth, const CGStr &ColHeading, UINT uiNumrows);
			BOOL CreatePickListCol(UINT uiDspWidth, const CGStr &ColHeading, UINT uiNumRows);
			BOOL GetFullTableHeight(UINT uiNumDataRows, ULONG *pulTableHeight);
			BOOL IsMouseButtonDown() {return(mbMouseDown);};

			// Member functions from base class CGUI_DlgCtrl
			virtual BOOL Validate(BOOL bDspLibValidFailMsg = FALSE);
			virtual void Update();
			virtual BOOL RetrieveUserData();
			virtual int NotifyButtonUp(zEvent *pEvent);
			virtual void PresentHelpMsg();
			BOOL Print(const char *szTitle, const char *szData);

			zWindow *getTableFrame () { return (zWindow *)mpTable; };
			zComboBoxFull *GetComboCtrl();
			BOOL IsSplitPaneShown();
			void SetPopupMenuID(RESOURCE_ID PopupMenuID) {mPopupMenuID = PopupMenuID;};
			void SetPopupMenuNotifyFn(void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID))
			{mpPopupMenuNotifyFn = pPopupMenuNotifyFn;};


		private:
//			void Initialize();
			CGUI_Table *mpParentTable;
			static DWORD dwSTYLE;
			HWND mTableWnd;
			RESOURCE_ID mID;
			CGUI_Dlg *mpDlg;
			UINT muiHeadingHeight;
			CTableFrame *mpTable;
			BOOL mbStarted;
			UINT muiInitRow;
			UINT muiInitCol;
	};
	

#endif

