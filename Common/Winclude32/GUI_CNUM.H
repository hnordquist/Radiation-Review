
// GUI_CNUM.h	Numeric control classes.

#if !defined GUI_CNUM_H
	#define GUI_CNUM_H
	#include "gui_lib.h"

	class CCtrlNumeric : public CGUI_DlgCtrlValidated
	{
		public:
			CCtrlNumeric(DLG_CTRL_TYPE CtrlType, CGUI_Dlg *pDlg,
				RESOURCE_ID CtrlID, const char *szFormat,
				const char *szErrMsgFieldTypeStr,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlNumeric();
			void CreateBaseCtrl();
			virtual BOOL RetrieveUserData() = 0;		// Puts data into address passed in at creation.
			virtual BOOL SetDefaultFmt(const char *szFormat);
			virtual BOOL SetNumFmt(const char *szFmt);
			virtual void Update() = 0;
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE) = 0;
			virtual BOOL SelectText(BOOL bSelectAll);
			virtual BOOL SelectText(int iFirstChar, int iLastChar);
		
		protected:
			virtual BOOL ConvertFld(void *pScanValue);
			void PresentRangeErrorMsg(const CGStr &MinValueStr,
				const CGStr &MaxValueStr, const CGStr &CurrentValueStr);
			void PresentConversionErrorMsg();
			void ConvertValueToASCII(const CGStr *pFmtStr, ...);
			virtual BOOL RangeCheckFld(void *pScanValue) = 0;
			virtual void DisplayValue() = 0;
			CGStr *mpFmtStr;
			CGFmtStr *mpFldStr;

		private:
			virtual int NotifyButtonUp(zEvent *pEvent);
			BOOL StripWidthAndPrecision(CGStr *pFmtStr);
			CGStr *mpErrMsgFieldTypeStr;
	};

	// =========================== Char =======================================
	class CCtrlChar : public CCtrlNumeric
	{
		public:
			CCtrlChar(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, char *pcValue,
				const char *szFormat, char cMinValue, char cMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlChar();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(char *pcValue);
			virtual void PutValue(char cNewValue);
			virtual void SetLimits(char cMinValue, char cMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			char mcValue;
			char mcMinValue;
			char mcMaxValue;
			char *mpcUserValue;
	};

	// =========================== Double =======================================
	class CCtrlDouble : public CCtrlNumeric
	{
		public:
			CCtrlDouble(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, double *pdValue,
				const char *szFormat, double dMinValue, double dMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlDouble();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(double *pdValue);
			virtual void PutValue(double dNewValue);
			virtual void SetLimits(double dMinValue, double dMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
					
		protected:
			virtual BOOL ConvertFld(void *pScanValue);
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			double mdValue;
			double mdMinValue;
			double mdMaxValue; 
			double *mpdUserValue;
	};
	
	// =========================== Float =======================================
	class CCtrlFloat : public CCtrlNumeric
	{
		public:
			CCtrlFloat(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, float *pdValue,
				const char *szFormat,float dMinValue, float dMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			virtual ~CCtrlFloat();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(float *pfValue);
			virtual void PutValue(float fNewValue);
			virtual void SetLimits(float fMinValue, float fMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			float mfValue;
			float mfMinValue;
			float mfMaxValue;
			float *mpfUserValue;
	};
	
	// =========================== Int =======================================
	class CCtrlInt : public CCtrlNumeric
	{
		public:
			CCtrlInt(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, int *piValue,
				const char *szFormat, int iMinValue, int iMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlInt();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(int *plValue);
			virtual void PutValue(int iNewValue);
			virtual void SetLimits(int iMinValue, int iMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			int miValue;
			int miMinValue;
			int miMaxValue;
			int *mpiUserValue;
	};

	// =========================== Long =======================================
	class CCtrlLong : public CCtrlNumeric
	{
		public:
			CCtrlLong(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, long *plValue,
				const char *szFormat, long lMinValue, long lMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlLong();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(long *plValue);
			virtual void PutValue(long lNewValue);
			virtual void SetLimits(long lMinValue, long lMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			long mlValue;
			long mlMinValue;
			long mlMaxValue;
			long *mplUserValue;
	};

	// =========================== Short =======================================
	class CCtrlShort : public CCtrlNumeric
	{
		public:
			CCtrlShort(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, short *psValue,
				const char *szFormat, short sMinValue, short sMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlShort();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(short *psValue);
			virtual void PutValue(short sNewValue);
			virtual void SetLimits(short sMinValue, short sMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			short msValue;
			short msMinValue;
			short msMaxValue;
			short *mpsUserValue;
	};


	// =========================== UChar =======================================
	class CCtrlUChar : public CCtrlNumeric
	{
		public:
			CCtrlUChar(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, UCHAR *pucValue,
				const char *szFormat, UCHAR ucMinValue, UCHAR ucMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlUChar();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(UCHAR *pucValue);
			virtual void PutValue(UCHAR ucNewValue);
			virtual void SetLimits(UCHAR ucMinValue, UCHAR ucMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			UCHAR mucValue;
			UCHAR mucMinValue;
			UCHAR mucMaxValue;
			UCHAR *mpucUserValue;
	};

	// =========================== UInt =======================================
	class CCtrlUInt : public CCtrlNumeric
	{
		public:
			CCtrlUInt(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, unsigned int *puiValue,
				const char *szFormat, unsigned int uiMinValue, unsigned int uiMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlUInt();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(unsigned int *puiValue);
			virtual void PutValue(unsigned int uiNewValue);
			virtual void SetLimits(unsigned int uiMinValue, unsigned int uiMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			unsigned int muiValue;
			unsigned int muiMinValue;
			unsigned int muiMaxValue;
			unsigned int *mpuiUserValue;
	};

	// =========================== ULong =======================================
	class CCtrlULong : public CCtrlNumeric
	{
		public:
			CCtrlULong(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, ULONG *pulValue,
				const char *szFormat, ULONG ulMinValue, ULONG ulMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlULong();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(ULONG *pulValue);
			virtual void PutValue(ULONG ulNewValue);
			virtual void SetLimits(ULONG ulMinValue, ULONG ulMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			ULONG mulValue;
			ULONG mulMinValue;
			ULONG mulMaxValue;
			ULONG *mpulUserValue;
	};

	// =========================== UShort =======================================
	class CCtrlUShort : public CCtrlNumeric
	{
		public:
			CCtrlUShort(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID, USHORT *pusValue,
				const char *szFormat, USHORT usMinValue, USHORT usMaxValue,
				BOOL (*pValidateFn)(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID),
				RESOURCE_ID PopupMenuID = -1,
				void (*pPopupMenuNotifyFn)(CGUI_Dlg *pDlg, RESOURCE_ID ControlID, RESOURCE_ID MenuItemID) = NULL);
			~CCtrlUShort();
			virtual BOOL RetrieveUserData();		// Puts data into address passed in at creation.
			virtual BOOL GetValue(USHORT *pusValue);
			virtual void PutValue(USHORT usNewValue);
			virtual void SetLimits(USHORT usMinValue, USHORT usMaxValue);
			virtual void Update();
			virtual BOOL DoLibValidation(BOOL bDspLibValidFailMsg = FALSE);
		
		protected:
			virtual BOOL RangeCheckFld(void *pScanValue);
			virtual void DisplayValue();
			
		private:	
			USHORT musValue;
			USHORT musMinValue;
			USHORT musMaxValue;
			USHORT *mpusUserValue;
	};




#endif

