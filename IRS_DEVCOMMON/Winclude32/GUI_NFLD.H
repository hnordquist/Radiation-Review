
// GUI_NFLD.h	Numeric field classes.

#if !defined GUI_NFLD_H
	#define GUI_NFLD_H
	#include "gui_lib.h"
//	#include <gui_fstr.h>
	#include <ctype.h>		// for isdigit() etc.
	#include <stdarg.h>
	#include <stdlib.h>
	
	typedef enum
	{
		GNFT_CHAR,
		GNFT_UCHAR,
		GNFT_DOUBLE,
		GNFT_FLOAT,
		GNFT_INT,
		GNFT_UINT,
		GNFT_LONG,
		GNFT_ULONG,
		GNFT_SHORT,
		GNFT_USHORT
	} GUI_NUM_FLD_TYPE;
	                     
// Public:
// Constructor takes a GUI_NUM_FLD_TYPE and format argument.
//
// DoLibValidation checks for ability to convert from string and then checks range
// of converted value. Returns overall success and optionally the text of an error msg.
//
// GetVal & GetValAsStr return numeric and string respectively values of field.
//
// Protected:
//
// ConvertFromStr converts string representation of value to numeric value. Returns
// success and optionally the text of an error msg.
//
// ConvertToStr converts field value to string representation. The first argument, the
// conversion format to use, is primarily used as a "marker" for va_start. It is almost
// always mFmtStr.
//
// CheckRange check field value against limits. Returns success and optionally the
// text of an error msg.
//
// CFldNum *mpDerivedObj: Pointer to object of derived class. Used to call
// type-specific member functions during handling of calls to the general
// base class CFldNum e.g. SetValStr().
//
// CGStr mFmtStr: String used for sprintf formatting of field value. A modified
// form of this is used for sscanf conversion of string to field value.
//
// CGFmtStr mValAsStr: String which contains string representation of field value.
//
// GUI_NUM_FLD_TYPE mFldType: Type of numeric field -- used for various sanity checks.
//
// StripWidthAndPrecision strips off width adn precision from "printf" format so that
// it can be used for scanf conversion of string representation to numeric value.
//
//	CGStr mFieldTypeStr: Used to constuct sensible English error message when
// ConvertFromStr fails.


	class CFldNum
	{
		public:
			CFldNum(GUI_NUM_FLD_TYPE NumType, const char *szFormat);
			virtual ~CFldNum();
			
			void SetFmtStr(const char *szFormat);
			virtual BOOL ForceNewConversion() {return(FALSE);};

			// GetVal(void *) is potentially dangerous (if destination is not long
			// enough for data).
			virtual BOOL GetVal(void *pVal);

			virtual BOOL GetVal(char *pcVal) {return(FALSE);};
			virtual BOOL SetVal(char cVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(UCHAR *pucVal) {return(FALSE);};
			virtual BOOL SetVal(UCHAR ucVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(short *psVal) {return(FALSE);};
			virtual BOOL SetVal(short sVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(USHORT *pusVal) {return(FALSE);};
			virtual BOOL SetVal(USHORT usVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(int *piVal) {return(FALSE);};
			virtual BOOL SetVal(int iVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(UINT *puiVal) {return(FALSE);};
			virtual BOOL SetVal(UINT uiVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(long *plVal) {return(FALSE);};
			virtual BOOL SetVal(long lVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(ULONG *pulVal) {return(FALSE);};
			virtual BOOL SetVal(ULONG ulVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(float *pfVal) {return(FALSE);};
			virtual BOOL SetVal(float fVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL GetVal(double *pdVal) {return(FALSE);};
			virtual BOOL SetVal(double dVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			virtual BOOL SetVal(const char *szStrVal, CGStr *pErrMsg = NULL) {return(FALSE);};

			const CGStr *GetStrRep();
		
		protected:
			void ConvertToStr(const CGStr *pFmtStr, ...);
			virtual void CreateConversionErrMsg(const char *szNewStrValue, CGStr *pErrMsg);
			CGStr mFmtStr;
			CGStr mValAsStr;

		private:
			GUI_NUM_FLD_TYPE mFldType;                                 
	};

	// =========================== Double =======================================

	class CFldDouble : public CFldNum
	{
		public:
			CFldDouble(double dVal, const char *szFormat,
				double dMinVal, double dMaxVal);
			~CFldDouble();
			virtual BOOL GetVal(double *pdVal);
			virtual BOOL GetVal(void *pVal) {return(GetVal((double *)pVal));};
			virtual BOOL SetVal(double dNewVal, CGStr *pErrMSg = NULL);
			virtual BOOL SetVal(const char *szStrVal, CGStr *pErrMsg = NULL);
		
		protected:
			BOOL CheckRange(double dValue, CGStr *pErrMsg = NULL);
			BOOL ConvertFromStr(const char *szNewStrVal, double *pdValue, CGStr *pErrMsg = NULL);
			virtual void CreateConversionErrMsg(const char *szNewStrValue, CGStr *pErrMsg);
			virtual BOOL ForceNewConversion();
			
		private:	
			double mdValue;
			double mdMinValue;
			double mdMaxValue;
	};


#endif

