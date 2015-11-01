
#if !defined(GUI_CELL_H)

	#define GUI_CELL_H

	#include <gui_lib.h>
	#include "gui_tbl.h"
//	#include "gui_ctbl.h"
	#include "gui_nfld.h"

	class CCell;
	class CTblPickList;

	class CTblCol {
		public:
			CTblCol(const char *szLabel, UINT uiNumRows,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol),
				UINT uiDataLen, UINT uiMaxCellDspWidthPix);
			~CTblCol();
BOOL AreAnyCellsNULL();
			UINT GetDataLen();
			BOOL DeleteRow(UINT uiRow);
			BOOL InsertRow(UINT uiNewRowIndex);
			BOOL MoveRow(UINT uiOrigRowIndex, UINT uiNewRowIndex);
			BOOL SwapRows(UINT uiRow1, UINT uiRow2);
			BOOL ChangeNumRows(UINT uiNewNumRows);
			UINT ComputeDspWidth(BOOL bPrinter = FALSE);
			CCell *GetCell(UINT uiRow);
			BOOL SetCell(UINT uiRow, CCell *pCell, UINT uiDataLen = 0);
			void SetHelpMsg(const char *szMsgText);
			BOOL GetHelpMsg(CGStr *pMsgText);
			const CGStr *GetLabel();
			void SetLabel(const char *szLabel);
			void SetColWidth(UINT uiColWidthPix) {muiMaxCellDspWidthPix = uiColWidthPix;};

			void GetLabelMetrics(UINT *puiNumLines, UINT *puiDspWidth);
			BOOL GetLabelText(CGStr *pLabelLineText, UINT uiLabelLineNum);
			UINT GetNumRows() {return(muiNumRows);};
			BOOL CopyData(UINT uiRowIndex, char *pcRowDataBuffer, UINT *puiDataLen);
			void DefinePickList(char *naszPickList[] = NULL);
			CTblPickList *GetPickList();
			BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, UINT uiDspWidth);
			BOOL SetWriteEnable(BOOL bWriteEnabled);
			BOOL GetWriteEnable(BOOL *pbWriteEnabled);
			BOOL SetUserValidateFn(BOOL (*pValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol));
			BOOL (*GetUserValidationFn())(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol);
	
		private:      
			BOOL (*mpValidateFn)(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol);
			BOOL mbWriteEnabled;
			BOOL GetRepresentativeCell(CCell **ppCell);
			UINT muiMaxCellDspWidthPix;
			UINT muiDataLen;
			UINT muiNumRows;
			CCell **mapCells;
			CGStr mLabel;
			CTblPickList *mpPickList;
		    UINT muiHeadingLines;
		    UINT muiLabelWidthPix;
		    UINT muiLabelWidthChar;
		    CGStr mHelpMsg;
	};
	
	class CCell {
		public:
			friend BOOL CGUI_Table::UpdateUserVar(UINT uiRow, UINT uiCol);
			friend BOOL CGUI_Table::UpdateCell(UINT uiRow, UINT uiCol);

			// CCell may be constructed only from friend function(s) in CGUI_Table. This
			// low-level implementation is not meant for general consumption.
			CCell(CGUI_Table *pTbl, UINT uiCol, void *pUserValue);
			// Make abstract class so can only instantiate its descendents.
			virtual ~CCell() = 0;
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual UINT GetDspWidth() {return(0);};
			virtual const CGStr *GetStrRep() = 0;
			virtual BOOL IsCustomDrawCell() {return(FALSE);};
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL GetGenDspWidth(UINT *puiCellDspWidth);
			
			void GetColor(COLORREF *pFgColor, COLORREF *pBgColor);
			void SetColor(COLORREF FgColor, COLORREF BgColor);

			BOOL GetModified() {return(mbModified);};
			void SetModified(BOOL bModified) {mbModified = bModified;};
				
			virtual BOOL SetFmtStr(const char *szFormat) {return(FALSE);};
			virtual BOOL IsBoolCell() {return(FALSE);};
			virtual BOOL IsStrCell() {return(FALSE);};
			virtual UINT GetStrDim() {return(0);};

			virtual GUI_ALIGN GetAlignment() {return(GUI_ALIGN_LEFT);};
			virtual BOOL GetVal(char *pcVal) {return(FALSE);};
			virtual BOOL SetVal(char cVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(UCHAR *pucVal) {return(FALSE);};
			virtual BOOL SetVal(UCHAR ucVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(short *psVal) {return(FALSE);};
			virtual BOOL SetVal(short sVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(USHORT *pusVal) {return(FALSE);};
			virtual BOOL SetVal(USHORT usVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(int *piVal) {return(FALSE);};
			virtual BOOL SetVal(int iVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(UINT *puiVal) {return(FALSE);};
			virtual BOOL SetVal(UINT uiVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(long *plVal) {return(FALSE);};
			virtual BOOL SetVal(long lVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(ULONG *pulVal) {return(FALSE);};
			virtual BOOL SetVal(ULONG ulVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(float *pfVal) {return(FALSE);};
			virtual BOOL SetVal(float fVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(double *pdVal) {return(FALSE);};
			virtual BOOL SetVal(double dVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(struct GEN_DATE_STRUCT *pDate) {return(FALSE);};
			virtual BOOL SetVal(struct GEN_DATE_STRUCT Date, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(struct GEN_TIME_STRUCT *pTime) {return(FALSE);};
			virtual BOOL SetVal(struct GEN_TIME_STRUCT Time, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			virtual BOOL GetVal(CGStr *pStrVal) {return(FALSE);};
			// The following line can be for any cell type - especially for numeric.
			virtual BOOL SetVal(const char *szStrVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};

			//virtual BOOL GetValBool(BOOL *pbVal) {return(FALSE);};
			//virtual BOOL SetValBool(BOOL bVal, CGStr *pErrMsg = NULL) {return(NoProperOverloadForSetVal(pErrMsg));};
			UINT muiCol;
			CGUI_Table *mpTbl;
			
		protected:                                     
			// Each derived class will implement GetVal(void *) probably in terms of
			// its "natural" GetVal fn else base class will return success = FALSE.
			// Likewise a generic SetVal(void *).
			virtual BOOL GetVal(void *) {return(FALSE);};
			virtual BOOL SetVal(void *) {return(FALSE);};
			// Only CGUI_Table can access values via "void *" argument in an attempt
			// to protect the innocent user from hurting her/himself.
			void *mpUserVar;
			BOOL NoProperOverloadForSetVal(CGStr *pErrMsg) {if (pErrMsg != NULL) *pErrMsg = ""; return(FALSE);};
					
		private: 
	    	COLORREF mFG_Color;
			COLORREF mBG_Color;
			BOOL mbModified;
	};
	
	
	class CCellNum : public CCell  {
		public:
			CCellNum(CGUI_Table *pTbl, UINT uiCol, void *pValue);
			virtual ~CCellNum();
			virtual const CGStr *GetStrRep() {return(mpFldNum->GetStrRep());};
			virtual GUI_ALIGN GetAlignment() {return(GUI_ALIGN_RIGHT);};
			virtual BOOL SetFmtStr(const char *szFormat);
		
		protected:
			CFldNum *mpFldNum;
	
		private: 
	};
	
	
	class CCellDouble : public CCellNum {
		public:
			CCellDouble(CGUI_Table *pTbl, UINT uiCol, double *pdValue,  const char *szFormat = "%lf",
				double dMinValue = -MY_DBL_MAX, double dMaxValue = MY_DBL_MAX);
			virtual ~CCellDouble();
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual BOOL GetVal(double *pdValue);
			virtual BOOL SetVal(double dNewValue, CGStr *pErrMsg = NULL);
			virtual BOOL SetVal(const char *szNewValue, CGStr *pErrMsg = NULL);
		
		protected:
			virtual BOOL GetVal(void *pValue) {return(GetVal((double *)pValue));};
			virtual BOOL SetVal(void *pValue) {return(SetVal(*(double *)pValue));};
			
		private:
			double mdValue;
	};
	
	
	class CCellStr : public CCell {
		public:
			CCellStr(CGUI_Table *pTbl, UINT uiCol, CGStr *pCGStrValue, UINT uiMaxDim);
			CCellStr(CGUI_Table *pTbl, UINT uiCol, char *szText, UINT uiMaxDim);
			virtual ~CCellStr();
			virtual const CGStr *GetStrRep() {return(mpStrValue);};
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual BOOL IsStrCell() {return(TRUE);};
			virtual UINT GetStrDim() {return(muiMaxDim);};
			virtual BOOL GetGenDspWidth(UINT *puiCellDspWidth);
			virtual BOOL GetVal(CGStr *pStrVal);
			BOOL GetVal(char *szDestStr, UINT uiDestDim);
			virtual BOOL SetVal(const char *szStrVal, CGStr *pErrMsg = NULL);
			
		protected:
			virtual BOOL GetVal(void *pValue);
			virtual BOOL SetVal(void *pValue);
			CGStr *mpStrValue;
			
		private:
			UINT muiMaxDim;
			BOOL mbStringZero;	// FALSE => CGStr object address passed in to contructor.
	};
	
	
	class CCellDate : public CCell {
		public:
			CCellDate(CGUI_Table *pTbl, UINT uiCol, struct GEN_DATE_STRUCT *pDate, UINT uiStrDim);
			virtual ~CCellDate();
			virtual const CGStr *GetStrRep() {return(&mStrValue);};
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual BOOL GetGenDspWidth(UINT *puiCellDspWidth);
			virtual BOOL GetVal(struct GEN_DATE_STRUCT *pDate);
			virtual BOOL SetVal(struct GEN_DATE_STRUCT Date, CGStr *pErrMsg = NULL);
			virtual BOOL SetVal(const char *szFmtdDateStr, CGStr *pErrMsg = NULL);
			
		protected:
			virtual BOOL GetVal(void *pValue) {return(GetVal((struct GEN_DATE_STRUCT *)pValue));};
			virtual BOOL SetVal(void *pValue) {return(SetVal(*(struct GEN_DATE_STRUCT *)pValue));};
			
		private:
			static const UINT uiMAX_DATE_STR_DIM;
			struct GEN_DATE_STRUCT mStructDate;
			CFmtdDate mFmtdDate;
			CGStr mStrValue;
			UINT muiMaxDim;
	};
	
	
	class CCellTime : public CCell {
		public:
			CCellTime(CGUI_Table *pTbl, UINT uiCol, struct GEN_TIME_STRUCT *pTime, UINT uiStrDim);
			virtual ~CCellTime();
			virtual const CGStr *GetStrRep() {return(&mStrValue);};
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual BOOL GetGenDspWidth(UINT *puiCellDspWidth);
			virtual BOOL GetVal(struct GEN_TIME_STRUCT *pTime);
			virtual BOOL SetVal(struct GEN_TIME_STRUCT Time, CGStr *pErrMsg = NULL);
			virtual BOOL SetVal(const char *szFmtdTimeStr, CGStr *pErrMsg = NULL);
			
		protected:
			virtual BOOL GetVal(void *pValue) {return(GetVal((struct GEN_TIME_STRUCT *)pValue));};
			virtual BOOL SetVal(void *pValue) {return(SetVal(*(struct GEN_TIME_STRUCT *)pValue));};
			
		private:
			struct GEN_TIME_STRUCT mStructTime;
			CFmtdTime mFmtdTime;
			CGStr mStrValue;
			UINT muiMaxDim;
	};

	
	class CCellBool : public CCell {
		public:
			CCellBool(CGUI_Table *pTbl, UINT uiCol, BOOL *pbValue);
			virtual ~CCellBool();
			virtual const CGStr *GetStrRep();
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL IsBoolCell() {return(TRUE);};
			virtual BOOL GetGenDspWidth(UINT *puiCellDspWidth);
			virtual BOOL GetVal(BOOL *pbValue);
			virtual BOOL SetVal(BOOL bValue, CGStr *pErrMsg = NULL);
			virtual GUI_ALIGN GetAlignment() {return(GUI_ALIGN_CENTER);};
			
		protected:
			virtual BOOL GetVal(void *pValue) {return(GetVal((BOOL *)pValue));};
			virtual BOOL SetVal(void *pValue) {return(SetVal(*(BOOL *)pValue));};
			
		private:
			BOOL mbValue;
			static CGStr TRUE_BoolStrRep;
			static CGStr FALSE_BoolStrRep;
	};


	class CCellPickList : public CCell {
		public:
			CCellPickList(CGUI_Table *pTbl, UINT uiCol, UINT *puiListIndex);
			virtual ~CCellPickList();
//			virtual const CGStr *GetStrRep() {return(&EmptyPickListStrRep);};
			virtual const CGStr *GetStrRep();
			virtual BOOL CopyVal(char *pcBuffer, UINT uiNumBytes);
			virtual BOOL CreateDataTableCol(CCtrlTable *pTableCtrl, const CGStr &Label,
				UINT uiDspWidth, UINT uiNumRows);
			virtual BOOL GetGenDspWidth(UINT *puiCellDspWidth);
			virtual BOOL GetVal(CGStr *pStrVal);
			virtual BOOL SetVal(const char *szStrVal, CGStr *pErrMsg = NULL);
			virtual BOOL GetVal(UINT *puiIndex);
			virtual BOOL SetVal(UINT uiIndex);
			
		protected:
			virtual BOOL GetVal(void *pValue) {return(GetVal((UINT *)pValue));};
			virtual BOOL SetVal(void *pValue) {return(SetVal(*(UINT *)pValue));};
			
		private:
			UINT muiValue;
			static CGStr EmptyPickListStrRep;
			CGStr mSelectionText;
	};

#endif
