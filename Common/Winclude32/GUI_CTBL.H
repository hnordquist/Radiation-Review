
#ifndef GUI_CTBL_H

	#define GUI_CTBL_H
	
	class CGUI_Table;
	
	#include "mydtsrc\datatbl.h"
	
	extern "C" long FAR PASCAL DataTblWndFn (HWND hMyWnd, WORD wMsg, WORD wParam, long lParam);

	// additional types
	typedef long	DT_STATUS;
	typedef int		EDIT_ACTION;
	typedef COLNUM far *	LPCOLNUM;
	typedef ROWNUM far *	LPROWNUM;
		
	#define DTE_NOWINDOW	DTE_MESSAGENUM
	// This error is only returned by a member function when there is no
	// interface element (HWindow) associated with this object.  If all
	// operations on the interface element are done through the member
	// functions of the object, DTE_MESSAGENUM will never be returned.
	// Therefore, DTE_NOWINDOW can safely share the same number.
	

	class CCtrlTable : public CGUI_DlgCtrl
	{
		public:
			CCtrlTable(CGUI_Dlg *pDlg, CGUI_Table *pParentTable, zSizer *pSizer, RESOURCE_ID CtrlID);
			~CCtrlTable();
			void Initialize();
			BOOL Start(UINT uiNumRows);

			BOOL UpdateDisplay(UINT uiRow, UINT uiCol);
			BOOL SetColWriteEnable(UINT uiCol, BOOL bWriteEnabled);
			BOOL SetHeadingHeight(UINT uiNumLines);

			BOOL CreateINT1_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateINT2_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateINT4_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateUINT1_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateUINT2_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateUINT4_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateREAL4_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateREAL8_Col(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateCheckBoxCol(UINT uiDspWidth, const CGStr *pColHeading, BOOL bWriteEnabled);
			BOOL CreateTxtCol(UINT uiDspWidth, const CGStr *pColHeading, UINT uiStrDim, BOOL bWriteEnabled);
			BOOL CreatePickListCol(UINT uiDspWidth, const CGStr *pColHeading,
				UINT uiStrDim, CGStr *apPickListItems[], BOOL bWriteEnabled);
			BOOL GetFullTableHeight(UINT uiNumDataRows, ULONG *pulTableHeight);
			BOOL IsMouseButtonDown() {return(mbMouseDown);};

			// Member functions from base class CGUI_DlgCtrl
			virtual BOOL Validate(BOOL bDspLibValidFailMsg = FALSE);
			virtual void Update();
			virtual BOOL RetrieveUserData();
			virtual int NotifyButtonUp(zEvent *pEvent);

			//----- EditCurrCell -----
			DT_STATUS EditCurrCell (EDIT_ACTION anEditAction)
				{ return (DT_STATUS) SendDataTblMessage (DTM_EDITCURRCELL, anEditAction); }
		    //----------------  GetEditHwnd ----------------
		    HWND GetEditHwnd () const
		        { return (HWND) SendDataTblMessage (DTM_GETEDITHWND); }
			//----- GetAccessCol -----
			COLNUM GetAccessCol () const
				{ return (COLNUM) SendDataTblMessage (DTM_GETACCESSCOL); }
			//----- GetAccessRow -----
			ROWNUM GetAccessRow () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETACCESSROW); }
		    //----- GetNotifyCol -----
			COLNUM GetNotifyCol () const
				{ return (COLNUM) SendDataTblMessage (DTM_GETNOTIFYCOL); }
			//----- GetNotifyRow -----
			ROWNUM GetNotifyRow () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETNOTIFYROW); }
			//----- SetCurCell -----
			DT_STATUS SetCurCell (COLNUM aColumnNumber, ROWNUM aRowNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCURCELL, aColumnNumber, aRowNumber); }
			//----- GetCurCol -----
			COLNUM GetCurCol () const
				{ return (COLNUM) SendDataTblMessage (DTM_GETCURCOL); }
			//----- GetCurRow -----
			ROWNUM GetCurRow () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETCURROW); }
		    //----- SetCurCellColor ----------------
		    DT_STATUS SetCurCellColor (LPDTCOLORSTRUCT color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCURCELLCOLOR, NULL, (long) color); }
		    //----- InvalidateCell ----------------
		    DT_STATUS InvalidateCell (ROWNUM Row, COLNUM Col)
		        { return (DT_STATUS) SendDataTblMessage (DTM_INVALIDATECELL, Col, Row); }
			//----- SetAccessPos -----
			DT_STATUS SetAccessPos(COLNUM aColumnNumber, ROWNUM aRowNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETACCESSPOS, aColumnNumber, aRowNumber); }
		    //----- GetDTColor ----------------
		    DT_STATUS GetDTColor (LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETDTCOLOR, NULL, (long) Color); }
		    //----- SetDTColor----------------
		    DT_STATUS SetDTColor (LPDTCOLORSTRUCT Color)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETDTCOLOR, NULL, (LONG) Color); }
		    //----- SetColColor ----------------
		    DT_STATUS SetColColor (ACCMODE anAccessMode, LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCOLCOLOR, anAccessMode, (LONG) Color); }
			//----- SetColDatalen -----
			DT_STATUS SetColDatalen (COLNUM aColumnNumber, COLLEN columnLength)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLDATALEN, aColumnNumber, columnLength); }
			//----- GetColDatalen -----
			COLLEN GetColDatalen (COLNUM aColumnNumber) const
				{ return (COLLEN) SendDataTblMessage (DTM_GETCOLDATALEN, aColumnNumber); }

		private:
			CGUI_Table *mpParentTable;
			static DWORD dwSTYLE;
			HWND mTableWnd;
			RESOURCE_ID mID;
			CGUI_Dlg *mpDlg;
			UINT muiHeadingHeight;

			//----- DeleteCol -----
			DT_STATUS DeleteCol (ACCMODE anAccessMode)
				{ return (DT_STATUS) SendDataTblMessage (DTM_DELETECOL, anAccessMode); }
			//----- DeleteRow -----
			DT_STATUS DeleteRow (ACCMODE anAccessMode)
				{ return (DT_STATUS) SendDataTblMessage (DTM_DELETEROW, anAccessMode); }
			//----- GetCharSel -----
			DWORD GetCharSel () const
				{ return SendDataTblMessage (DTM_GETCHARSEL); }
			//----- GetColCfg -----
			DT_STATUS GetColCfg (COLNUM aColumnNumber, LPVOID theColumnConfig) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_GETCOLCFG, aColumnNumber, (LONG) theColumnConfig); }
			//----- GetColCfgSize -----
			int GetColCfgSize (COLNUM aColumnNumber) const
				{ return (int) SendDataTblMessage (DTM_GETCOLCFGSIZE, aColumnNumber); }
			//----- GetColCount -----
			int GetColCount () const
				{ return (int) SendDataTblMessage (DTM_GETCOLCOUNT); }
			//----- GetColExtra1 -----
			DWORD GetColExtra1 (COLNUM aColumnNumber) const
				{ return (DWORD) SendDataTblMessage (DTM_GETCOLEXTRA1, aColumnNumber); }
			//----- GetColExtra2 -----
			DWORD GetColExtra2 (COLNUM aColumnNumber) const
				{ return (DWORD) SendDataTblMessage (DTM_GETCOLEXTRA2, aColumnNumber); }
			//----- GetColFormat -----
			DT_STATUS GetColFormat (COLNUM aColumnNumber, LPVOID theColumnFormat) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_GETCOLFORMAT, aColumnNumber, (LONG) theColumnFormat); }
			//----- GetColHead -----
			DT_STATUS GetColHead (COLNUM aColumnNumber, LPSTR theColumnHead) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_GETCOLHEAD, aColumnNumber, (LONG) theColumnHead); }
			//----- GetColName -----
			DT_STATUS GetColName (COLNUM aColumnNumber, LPSTR theColumnName) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_GETCOLNAME, aColumnNumber, (LONG) theColumnName); }
			//----- GetColNumber -----
			int GetColNumber (LPSTR theColumnName) const
				{ return (int) SendDataTblMessage (DTM_GETCOLNAME, NULL, (LONG) theColumnName); }
			//----- GetColSelCount -----
			int GetColSelCount () const
				{ return (int) SendDataTblMessage (DTM_GETCOLSELCOUNT); }
			//----- GetColSelFirst -----
			int GetColSelFirst () const
				{ return (int) SendDataTblMessage (DTM_GETCOLSELFIRST); }
			//----- GetColSelList -----
			DT_STATUS GetColSelList (LPCOLNUM aColumnSelList) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_GETCOLSELLIST, NULL, (LONG) aColumnSelList); }
			//----- GetColSelNext -----
			int GetColSelNext (COLNUM thePreviousSelCol) const
				{ return (int) SendDataTblMessage (DTM_GETCOLSELNEXT, thePreviousSelCol); }
			//----- GetColStatus -----
			BOOL GetColStatus (COLNUM aColumnNumber) const
				{ return (BOOL) SendDataTblMessage (DTM_GETCOLSTATUS, aColumnNumber); }
			//----- GetColStyle -----
			COLSTYLE GetColStyle (COLNUM aColumnNumber) const
				{ return (COLSTYLE) SendDataTblMessage (DTM_GETCOLSTYLE, aColumnNumber); }
			//----- GetColType -----
			COLTYPE GetColType (COLNUM aColumnNumber) const
				{ return (COLTYPE) SendDataTblMessage (DTM_GETCOLTYPE, aColumnNumber); }
			//----- GetColWidth -----
			int GetColWidth (COLNUM aColumnNumber) const
				{ return (int) SendDataTblMessage (DTM_GETCOLWIDTH, aColumnNumber); }
			//----- GetConfig -----
			DWORD GetConfig (LPVOID theDataTblConfig) const
				{ return SendDataTblMessage (DTM_GETCONFIG, NULL, (LONG) theDataTblConfig); }
			//----- GetConfigSize -----
			int GetConfigSize () const
				{ return (int) SendDataTblMessage (DTM_GETCONFIGSIZE); }
			//----- GetCurColSel -----
			COLNUM GetCurColSel () const
				{ return (COLNUM) SendDataTblMessage (DTM_GETCURCOLSEL); }
			//----- GetCurRowSel -----
			ROWNUM GetCurRowSel () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETCURROWSEL); }
			//----- GetDTMetrics -----
			LONG	GetDTMetrics (int aMetricIndex) const
				{ return (LONG) SendDataTblMessage (DTM_GETDTMETRICS, aMetricIndex); }
			//----- GetEditKey -----
			WORD GetEditKey (EDIT_ACTION anEditAction) const
				{ return (WORD) SendDataTblMessage (DTM_GETEDITKEY, anEditAction); }
			//----- GetLeftIndex -----
			COLNUM GetLeftIndex () const
				{ return (COLNUM) SendDataTblMessage (DTM_GETLEFTINDEX); }
		    //---------------- GetClickPos ----------------
		    int GetClickPos () const
		        { return (int) SendDataTblMessage (DTM_GETCLICKPOS); }
		    //----------------  GetRowHeight ----------------
		    int GetRowHeight(ACCMODE anAccessMode) const
		        { return (int) SendDataTblMessage (DTM_GETROWHEIGHT, anAccessMode); }
			//----- GetRowCount -----
			ROWNUM GetRowCount () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETROWCOUNT); }
			//----- GetRowSelCount -----
			ROWNUM GetRowSelCount () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETROWSELCOUNT); }
			//----- GetRowSelFirst -----
			ROWNUM GetRowSelFirst () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETROWSELFIRST); }
			//----- GetRowSelList -----
			DT_STATUS GetRowSelList (LPROWNUM aRowSelList) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_GETROWSELLIST, NULL, (LONG) aRowSelList); }
			//----- GetRowSelNext -----
			LONG GetRowSelNext (ROWNUM thePreviousSelRow) const
				{ return (LONG) SendDataTblMessage (DTM_GETROWSELNEXT, NULL, thePreviousSelRow); }
			//----- GetRowStatus -----
			BOOL GetRowStatus (ROWNUM aRowNumber) const
		        { return (BOOL) SendDataTblMessage (DTM_GETROWSTATUS, NULL, aRowNumber); }
			//----- GetSelMode -----
			BOOL GetSelMode () const
				{ return (BOOL) SendDataTblMessage (DTM_GETSELMODE); }
			//----- GetTopIndex -----
			ROWNUM GetTopIndex () const
				{ return (ROWNUM) SendDataTblMessage (DTM_GETTOPINDEX); }
			//----- GetVisCols -----
			int GetVisCols () const
				{ return (int) SendDataTblMessage (DTM_GETVISCOLS); }
			//----- GetVisRows -----
			LONG GetVisRows () const
				{ return (LONG) SendDataTblMessage (DTM_GETVISROWS); }
			//----- GetWndStyle -----
			DWORD GetWndStyle () const
				{ return SendDataTblMessage (DTM_GETWNDSTYLE); }
			//----- InsertCol -----
			DT_STATUS InsertCol (ACCMODE anAccessMode, LPCOLUMNCFGSTRUCT theColConfig)
				{ return (DT_STATUS) SendDataTblMessage (DTM_INSERTCOL, anAccessMode, (LONG) theColConfig); }
			//----- InsertRow -----
			DT_STATUS InsertRow (ACCMODE anAccessMode, LPVOID theRowData)
				{ return (DT_STATUS) SendDataTblMessage (DTM_INSERTROW, anAccessMode, (LONG) theRowData); }
			//----- MoveColTo -----
			DT_STATUS MoveColTo (ACCMODE anAccessMode, COLNUM aNewColumnNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_MOVECOLTO, anAccessMode, aNewColumnNumber); }
			//----- MoveRowTo -----
			DT_STATUS MoveRowTo (ACCMODE anAccessMode, ROWNUM aNewRowNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_MOVEROWTO, anAccessMode, aNewRowNumber); }
			//----- ReadCell -----
		    DT_STATUS ReadCell (ACCMODE anAccessMode, LPVOID theCellData) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_READCELL, anAccessMode, (LONG) theCellData); }
			//----- ReadCellRect -----
			DT_STATUS ReadCellRect (ACCMODE anAccessMode, LPRECT theCellRect) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_READCELLRECT, anAccessMode, (LONG) theCellRect); }
			//----- ReadCol -----
			DT_STATUS ReadCol (ACCMODE anAccessMode, LPVOID theColData) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_READCOL, anAccessMode, (LONG) theColData); }
			//----- ReadRow -----
			DT_STATUS ReadRow (ACCMODE anAccessMode, LPVOID theRowData) const
				{ return (DT_STATUS) SendDataTblMessage (DTM_READROW, anAccessMode, (LONG) theRowData); }
			//----- ReplicateColTo -----
			DT_STATUS ReplicateColTo (ACCMODE anAccessMode, COLNUM aNewColumnNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_REPLICATECOLTO, anAccessMode, aNewColumnNumber); }
			//----- ReplicateRowTo -----
			DT_STATUS ReplicateRowTo (ACCMODE anAccessMode, ROWNUM aNewRowNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_REPLICATEROWTO, anAccessMode, aNewRowNumber); }
			//----- ResetAll -----
			DT_STATUS ResetAll ()
				{ return (DT_STATUS) SendDataTblMessage (DTM_RESETALL); }
			//----- ResetData -----
			DT_STATUS ResetData ()
				{ return (DT_STATUS) SendDataTblMessage (DTM_RESETDATA); }
			//----- SelAllCols -----
		    DT_STATUS SelAllCols (BOOL bSelect)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SELALLCOLS, NULL, bSelect); }
			//----- SelAllRows -----
		    DT_STATUS SelAllRows (BOOL bSelect)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SELALLROWS, bSelect); }
			//----- SelCol -----
		    DT_STATUS SelCol (COLNUM aColumnNumber, BOOL bSelect)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SELCOL, aColumnNumber, bSelect); }
			//----- SelColRange -----
		    DT_STATUS SelColRange (COLNUM theLastColumnInRange, BOOL bSelect)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SELCOLRANGE, theLastColumnInRange, bSelect); }
			//----- SelRow -----
		    DT_STATUS SelRow (BOOL bSelect, ROWNUM aRowNumber)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SELROW, bSelect, aRowNumber); }
			//----- SelRowRange -----
		    DT_STATUS SelRowRange (BOOL bSelect, ROWNUM theLastRowInRange)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SELROWRANGE, bSelect, theLastRowInRange); }
			//----- SetCharSel -----
			DT_STATUS SetCharSel (int theFirstSelectedChar, int theLastSelectedChar)
			 	{ return (DT_STATUS) SendDataTblMessage (DTM_SETCHARSEL, NULL,
					MAKELONG (theFirstSelectedChar, theLastSelectedChar)); }
			//----- SetColCfg -----
			DT_STATUS SetColCfg (COLNUM aColumnNumber, LPVOID aColumnConfig)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLCFG, aColumnNumber, (LONG) aColumnConfig); }
			//----- SetColCount -----
			DT_STATUS SetColCount (int columnCount)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLCOUNT, columnCount); }
			//----- SetColExtra1 -----
			DT_STATUS SetColExtra1 (COLNUM aColumnNumber, DWORD columnExtra)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLEXTRA1, aColumnNumber, columnExtra); }
			//----- SetColExtra2 -----
			DT_STATUS SetColExtra2 (COLNUM aColumnNumber, DWORD columnExtra)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLEXTRA2, aColumnNumber, columnExtra); }
			//----- SetColFormat -----
			DT_STATUS SetColFormat (COLNUM aColumnNumber, LPSTR aColumnFormat)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLFORMAT, aColumnNumber, (LONG) aColumnFormat); }
			//----- SetColHead -----
			DT_STATUS SetColHead (COLNUM aColumnNumber, LPSTR aColumnHead)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLHEAD, aColumnNumber, (LONG) aColumnHead); }
			//----- SetColName -----
			DT_STATUS SetColName (COLNUM aColumnNumber, LPSTR aColumnName)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLNAME, aColumnNumber, (LONG) aColumnName); }
			//----- SetColStyle -----
			DT_STATUS SetColStyle (COLNUM aColumnNumber, COLSTYLE columnStyle)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLSTYLE, aColumnNumber, columnStyle); }
			//----- SetColType -----
			DT_STATUS SetColType (COLNUM aColumnNumber, COLTYPE aColumnType)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLTYPE, aColumnNumber, aColumnType); }
			//----- SetColWidth -----
			DT_STATUS SetColWidth (COLNUM aColumnNumber, int columnWidth)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCOLWIDTH, aColumnNumber, columnWidth); }
			//----- SetConfig -----
			DT_STATUS SetConfig (LPVOID theDataTblConfig)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETCONFIG, NULL, (LONG) theDataTblConfig); }
			//----- SetEditKey -----
			DT_STATUS SetEditKey (EDIT_ACTION anEditAction, WORD aVirtualKey)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETEDITKEY, anEditAction, aVirtualKey); }
			//----- SetLeftIndex -----
			DT_STATUS SetLeftIndex (COLNUM aColumnNumber)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETLEFTINDEX, aColumnNumber,NULL); }
		    //----- SetTopIndex -----
			DT_STATUS SetTopIndex (ROWNUM aRowNumber)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETTOPINDEX, NULL, aRowNumber); }
			//----- SetWndStyle -----
			DT_STATUS SetWndStyle (DWORD windowStyle)
				{ return (DT_STATUS) SendDataTblMessage (DTM_SETWNDSTYLE, NULL, windowStyle); }
			//----- UpdateCell -----
		    DT_STATUS UpdateCell (ACCMODE anAccessMode, LPVOID theCellData)
				{ return (DT_STATUS) SendDataTblMessage (DTM_UPDATECELL, anAccessMode, (LONG) theCellData); }
			//----- UpdateCol -----
		    DT_STATUS UpdateCol (ACCMODE anAccessMode, LPVOID theColData)
				{ return (DT_STATUS) SendDataTblMessage (DTM_UPDATECOL, anAccessMode, (LONG) theColData); }
			//----- UpdateRow -----
		    DT_STATUS UpdateRow (ACCMODE anAccessMode, LPVOID theRowData)
				{ return (DT_STATUS) SendDataTblMessage (DTM_UPDATEROW, anAccessMode, (LONG) theRowData); }
		    //---------------- RowColor ----------------
		    DT_STATUS SetRowColor (ACCMODE anAccessMode, LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETROWCOLOR, anAccessMode, (LONG) Color); }
		    //---------------- ResetColor ---------------
			DT_STATUS ResetDTColor ()
				{ return (DT_STATUS) SendDataTblMessage (DTM_RESETDTCOLOR, NULL, NULL); }
		    //---------------- ResetAllColor ------------
		    DT_STATUS ResetAllColor ()
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETALLCOLOR, NULL, NULL); }
		    //---------------- ResetRowColor ------------
		    DT_STATUS ResetRowColor (ACCMODE anAccessMode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETROWCOLOR, anAccessMode, NULL); }
		    //---------------- ResetColColor ------------
		    DT_STATUS ResetColColor (ACCMODE anAccessMode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETCOLCOLOR, anAccessMode, NULL); }
		    //---------------- ResetCellColor------------
		    DT_STATUS ResetCellColor (ACCMODE anAccessMode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETCELLCOLOR, anAccessMode, NULL); }
		    //---------------- SetDtFont ----------------
		    DT_STATUS SetDTFont (LOGFONT FAR * Font)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETDTFONT, NULL, (LONG) Font); }
		    //---------------- RowFont ----------------
		    DT_STATUS SetRowFont (ACCMODE anAccessMode, LOGFONT FAR * Font)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETROWFONT, anAccessMode, (LONG) Font); }
		    //---------------- ColFont ----------------
		    DT_STATUS SetColFont (ACCMODE anAccessMode, LOGFONT FAR * Font)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCOLFONT, anAccessMode, (LONG) Font); }
		    //---------------- CellColor ----------------
		    DT_STATUS SetCellColor (ACCMODE anAccessMode, LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCELLCOLOR, anAccessMode, (LONG) Color); }
		    //---------------- CellFont ----------------
		    DT_STATUS SetCellFont (ACCMODE anAccessMode, LOGFONT FAR * Font)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCELLFONT, anAccessMode, (LONG) Font); }
		    //---------------- ResetFont ---------------
		    DT_STATUS ResetDTFont ()
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETDTFONT, NULL, NULL); }
		    //---------------- ResetAllFont ------------
		    DT_STATUS ResetAllFont ()
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETALLFONT, NULL, NULL); }
		    //---------------- ResetRowFont ------------
		    DT_STATUS ResetRowFont (ACCMODE anAccessMode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETROWFONT, anAccessMode, NULL); }
		    //---------------- ResetColFont ------------
		    DT_STATUS ResetColFont (ACCMODE anAccessMode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETCOLFONT, anAccessMode, NULL); }
		    //---------------- ResetCellFont------------
		    DT_STATUS ResetCellFont (ACCMODE anAccessMode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_RESETCELLFONT, anAccessMode, NULL); }
		    //---------------- VMInitialize------------
		    DT_STATUS VmInitalize (BOOL UserDefKeys,ROWNUM NumOfRows)
		        { return (DT_STATUS) SendDataTblMessage (DTM_VM_INITIALIZE, (WORD) UserDefKeys,NumOfRows); }
		    //---------------- SetSortCol------------
		    DT_STATUS SetSortCol(COLNUM col,BOOL Ascending)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETSORTCOL, col,(LONG) Ascending); }
		    //---------------- GetSortCol------------
		    COLNUM GetSortCol() const
		        { return (COLNUM) SendDataTblMessage (DTM_GETSORTCOL, NULL,NULL); }
		    //---------------- GetSortDir-(for backward compatability)
		    BOOL GetSortDir () const
		        { return (BOOL) SendDataTblMessage (DTM_GETSORTASC, NULL,NULL); }
		    //---------------- GetSortAsc------------
		    BOOL GetSortAsc () const
		        { return (BOOL) SendDataTblMessage (DTM_GETSORTASC, NULL,NULL); }
		    //---------------- SetCFEndEdit-----------
		    DT_STATUS SetCFEndEdit (BOOL EndEdit)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCFENDEDIT, (WORD) EndEdit,NULL); }
		    //---------------- GetCFEndEdit----------
		    BOOL GetCFEndEdit () const
		        { return (BOOL) SendDataTblMessage (DTM_GETCFENDEDIT, NULL,NULL); }
		    //---------------- SetFixedHT-----------
		    DT_STATUS SetFixedHT (WORD Height)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETFIXEDHT, (WORD) Height,NULL); }
		    //---------------- GetFixedHT----------
		    WORD GetFixedHT () const
		        { return (WORD) SendDataTblMessage (DTM_GETFIXEDHT, NULL,NULL); }
		    //---------------- SetVertAlign---------
		    DT_STATUS SetVertAlign (WORD VertAlign)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETVERTALIGN, (WORD) VertAlign,NULL); }
		    //---------------- GetVertAlign--------
		    WORD GetVertAlign () const
		        { return (WORD) SendDataTblMessage (DTM_GETVERTALIGN, NULL,NULL); }
		    //---------------- SetKey---------
		    DT_STATUS SetKey (WORD KeyCode)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETKEY, (WORD) KeyCode,NULL); }
		    //---------------- GetKey----------
		    WORD GetKey () const
		        { return (WORD) SendDataTblMessage (DTM_GETKEY, NULL,NULL); }
		    //----------------  GetFlagStr ----------------
		    DT_STATUS GetFlagStr (LPSTR theFlagStr)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETFLAGSTR, NULL, (long)theFlagStr); }
		    //----------------  SetFlagStr ----------------
		    DT_STATUS SetFlagStr (LPSTR theFlagStr)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETFLAGSTR, NULL, (long)theFlagStr); }
		    //----------------  GetCellStyle ----------------
		    WORD GetCellStyle (ACCMODE anAccessMode) const
		        { return (WORD) SendDataTblMessage (DTM_GETCELLSTYLE, anAccessMode); }
		    //----------------  SetCellStyle ----------------
		    DT_STATUS SetCellStyle (ACCMODE anAccessMode, WORD CellStyle)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCELLSTYLE, anAccessMode, (long)CellStyle); }
		    //----------------  GetCurCellColor ----------------
		    DT_STATUS GetCurCellColor (LPDTCOLORSTRUCT color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETCURCELLCOLOR, NULL, (long) color); }
		    //----------------  SetCurCellFont ----------------
		    DT_STATUS SetCurCellFont (LPLOGFONT Font)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCURCELLFONT, NULL, (long) Font); }
		    //----------------  SetRowLabel ----------------
		    DT_STATUS SetRowLabel (ACCMODE anAccessMode, LPSTR Label)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETROWLABEL, anAccessMode, (long) Label); }
		    //----------------  GetRowLabel ----------------
		    DT_STATUS GetRowLabel (ACCMODE anAccessMode, LPSTR Label)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETROWLABEL, anAccessMode, (long) Label); }
		    //----------------  SetRowHeight ----------------
		    DT_STATUS SetRowHeight (ACCMODE anAccessMode, int Height)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETROWHEIGHT, anAccessMode, (long) Height); }
		    //----------------  InvalidateRow ----------------
		    DT_STATUS InvalidateRow (ROWNUM Row)
		        { return (DT_STATUS) SendDataTblMessage (DTM_INVALIDATEROW, NULL, Row); }
		    //----------------  InvalidateCol ----------------
		    DT_STATUS InvalidateCol (COLNUM Col)
		        { return (DT_STATUS) SendDataTblMessage (DTM_INVALIDATECOL, Col); }
		    //----------------  MoveToNextCell ----------------
		    DT_STATUS MoveToNextCell ()
		        { return (DT_STATUS) SendDataTblMessage (DTM_MOVETONEXTCELL); }
		    //----------------  MoveToPrevCell ----------------
		    DT_STATUS MoveToPrevCell ()
		        { return (DT_STATUS) SendDataTblMessage (DTM_MOVETOPREVCELL); }
		    //----------------  MakeCellVis ----------------
		    DT_STATUS MakeCellVis (ROWNUM Row, COLNUM Col)
		        { return (DT_STATUS) SendDataTblMessage (DTM_MAKECELLVIS, Col, Row); }
		    //----------------  GetColEditStyle ----------------
		    DWORD GetColEditStyle (COLNUM Col) const
		        { return (DWORD) SendDataTblMessage (DTM_GETCOLEDITSTYLE, Col); }
		    //----------------  SetColEditStyle ----------------
		    DT_STATUS SetColEditStyle (COLNUM Col, DWORD dwStyle)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCOLEDITSTYLE, Col, dwStyle); }
		    //----------------  SetGridColor ----------------
		    DT_STATUS SetGridColor (COLORREF Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETGRIDCOLOR, NULL, (long) Color); }
		    //----------------  GetGridColor ----------------
		    COLORREF GetGridColor () const
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETGRIDCOLOR); }
		    //----------------  GetExtWndStyle ----------------
		    DWORD GetExtWndStyle() const
		        { return (DWORD) SendDataTblMessage (DTM_GETEXTWNDSTYLE); }
		    //----------------  SetExtWndStyle ----------------
		    DT_STATUS SetExtWndStyle(DWORD dwStyle)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETEXTWNDSTYLE, NULL, dwStyle); }
		    //----------------  GetColList ----------------
		    DT_STATUS GetColList(COLNUM Col, LPSTR List)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETCOLLIST, Col, (long) List); }
		    //----------------  SetColList ----------------
		    DT_STATUS SetColList(COLNUM Col, LPSTR List)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCOLLIST, Col, (long) List); }
		    //----------------  GetColCfgExt ----------------
		    DT_STATUS GetColCfgExt (COLNUM Col, LPCOLUMNCFGEXTSTRUCT ColCfgExt)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETCOLCFGEXT, Col, (long) ColCfgExt); }
		    //----------------  SetColCfgExt ----------------
		    DT_STATUS SetColCfgExt (COLNUM Col, LPCOLUMNCFGEXTSTRUCT ColCfgExt)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCOLCFGEXT, Col, (long) ColCfgExt); }
		    //----------------  GetConfigExt ----------------
		    DT_STATUS GetConfigExt (LPTABLECFGEXTSTRUCT TableCfgStr)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETCONFIGEXT, NULL, (long) TableCfgStr); }
		    //----------------  SetConfigExt ----------------
		    DT_STATUS SetConfigExt (LPTABLECFGEXTSTRUCT TableCfgStr)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETCONFIGEXT, NULL, (long) TableCfgStr); }
		    //----------------  SetSortKey ----------------
		    DT_STATUS SetSortKey(COLNUM Col, int SortKey)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETSORTKEY, Col, (long) SortKey); }
		    //----------------  GetSortKey ----------------
		    DT_STATUS GetSortKey(int SortKey)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETSORTKEY, NULL, (long)SortKey); }
		    //----------------  SortTable ----------------
		    DT_STATUS SortTable()
		        { return (DT_STATUS) SendDataTblMessage (DTM_SORTTABLE); }
		    //----------------  VMSetCellStyle ----------------
		    DT_STATUS VMSetCellStyle(COLNUM Col, WORD wStyle)
		        { return (DT_STATUS) SendDataTblMessage (DTM_VMSETCELLSTYLE, Col, (long) wStyle); }
		    //----------------  GetRowColor ----------------
		    DT_STATUS GetRowColor (ACCMODE anAccessMode, LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETROWCOLOR, anAccessMode, (long)Color); }
		    //----------------  GetColColor ----------------
		    DT_STATUS GetColColor (ACCMODE anAccessMode, LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETCOLCOLOR, anAccessMode, (long) Color); }
		    //----------------  GetCellColor ----------------
		    DT_STATUS GetCellColor (ACCMODE anAccessMode, LPDTCOLORSTRUCT Color)
		        { return (DT_STATUS) SendDataTblMessage (DTM_GETCELLCOLOR, anAccessMode, (long) Color); }
		    //----------------  GetRowStyle ----------------
		    WORD GetRowStyle (ACCMODE anAccessMode) const
		        { return (WORD) SendDataTblMessage (DTM_GETROWSTYLE, anAccessMode); }
		    //----------------  SetRowStyle ----------------
		    DT_STATUS SetRowStyle (ACCMODE anAccessMode, WORD wStyle)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETROWSTYLE, anAccessMode, (long) wStyle); }
		    //----------------  NotifyResults ----------------
		    DT_STATUS NotifyResults(DWORD dwResults)
		        { return (DT_STATUS) SendDataTblMessage (DTM_NOTIFYRESULTS, NULL, dwResults); }
		    //----------------  InsertColExt ----------------
		    DT_STATUS InsertColExt (ACCMODE anAccessMode, LPCOLUMNCFGEXTSTRUCT lpColCfg)
		        { return (DT_STATUS) SendDataTblMessage (DTM_INSERTCOLEXT, anAccessMode, (long) lpColCfg); }
		    //----------------  SetSortAsc ----------------
		    DT_STATUS SetSortAsc (BOOL bAscending, int SortKey)
		        { return (DT_STATUS) SendDataTblMessage (DTM_SETSORTASC, (WORD) bAscending, (long) SortKey); }
		    //----------------  GetSortCaseI --------------
		    DT_STATUS GetSortCaseI (int SortKey) const
		        { return (BOOL) SendDataTblMessage (DTM_GETSORTCASEI, NULL, (long) SortKey); }
		    //----------------  SetSortCaseI --------------
		    DT_STATUS SetSortCaseI (BOOL bSortCaseI, int SortKey)
		        { return (BOOL) SendDataTblMessage (DTM_SETSORTCASEI, (WORD) bSortCaseI, (long) SortKey); }

			DWORD SendDataTblMessage (WORD wMsg, WORD wParam = NULL, LONG lParam = NULL) const
			{
				if (mTableWnd)
					#ifdef GUI_DATA_TABLE
						return DataTblWndFn(mTableWnd, wMsg, wParam, lParam);
					#else
						return 0;
					#endif
					// return ::SendMessage (mTableWnd, wMsg, wParam, lParam);
				else
					return DTE_NOWINDOW;
			}
	};
	
#endif
