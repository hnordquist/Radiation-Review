// ErrorProcessing.cpp

#include "stdafx.h"
#include "ErrorProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define B2R1_PRELIM //<<QA>>

//
// Constructor and destructor
//
CErrorProcessing::CErrorProcessing
(
	CLSID ComponentCLSID, 
	short sUniqueID,
	char* pmessageformatfile,
	short verbosity,
	char *componentname,
	COleDateTime& dtStartUp)
{
	m_csLastErrorText.Empty();
	m_lLastErrorValue = 0;

	m_bstrMessageFormatFile = NULL;  //1/9/2007
	m_bstrComponentGUID = NULL;  //1/9/2007

	m_csFacilityName = CString("FACILITY NAME NOT SET IN CErrorProcessing");

	m_csdatefmtstr = "%Y %m %j %H:%M:%S";

	LPOLESTR polestr;
	::StringFromCLSID(ComponentCLSID,&polestr);
	CString cs(polestr);
	::CoTaskMemFree(polestr);  //15 Mar 2007
	m_bstrComponentGUID = cs.AllocSysString();

	m_sUniqueID = sUniqueID;
	m_bDisableLogger = true;//do not create the IRSLogger yet.

	m_csMessageFormatFile = CString(pmessageformatfile);
	m_bstrMessageFormatFile = m_csMessageFormatFile.AllocSysString();

	SetVerbosity(verbosity);

	m_csComponentName = CString(componentname);

	m_dtStartUp = dtStartUp;
	m_bStartTimeAlreadySent = false;
	m_bPrependComponentAndMethodStrings = true;
	m_lSeverityFromIniFile = 5;

	m_sFacilityID = 0;
	m_lStationID = 0;
	m_sChannelID = 0;

	m_pILogger = NULL;

	m_csNULL = "";
	m_csColon = ":";
	m_csSuccessReadingFile = "Success reading file: ";
	m_csInvalidPointer = "Invalid pointer";
	m_csInternalInterfaceCallFailed = "Internal interface call failed: ";
	m_csUnimplementedMethod = "Unimplemented method";
	m_csIntervalStartTime = "Interval start time (";
	m_csExceedsEndTime = ") exceeds end time (";
	m_csCloseParen = ")";
	m_csParametersNotSet = "Parameters not set: ";
	m_csSAFEARRAYError = "SAFEARRAY error: ";
	m_csCouldNotOpenDatabase = "Could not open database: ";
	m_csCouldNotReadFile = "Could not read file: ";
	m_csCouldNotFindFile = "Could not find file: ";
	m_csCouldNotFindDatabase = "Could not find database: ";
	m_csCouldNotOpenFile = "Could not open file: ";
	m_csCaughtCOMErrorException = "Caught COM Error exception: ";
	m_csCaughtADODBException = "Caught ADODB Exception: ";
	m_csCaughtException = "Caught exception. ";
	m_csDatabaseError = "Database error: ";
	m_csCouldNotCopyDatabase = "Could not copy database: ";
	m_csCouldNotCreateDatabase = "Could not create database: ";
	m_csDebug = "Debug: ";

#ifdef B2R1_PRELIM
		AfxMessageBox("CoIRSLogger DISABLED in this test version.");
#endif

}
CErrorProcessing::~CErrorProcessing()
{
	if (m_bstrComponentGUID != NULL)  //1/9/2007
		::SysFreeString(m_bstrComponentGUID);

	if (m_bstrMessageFormatFile != NULL)  //1/9/2007
		::SysFreeString(m_bstrMessageFormatFile);

	if(m_pILogger)
		m_pILogger = NULL;
}

void CErrorProcessing::InitializeForMethod(
		CString &methodname,
		short sFacilityID,
		long lStationID,
		short sChannelID)
{
	InitializeForMethod((LPCTSTR)methodname,sFacilityID,lStationID,sChannelID);
}

void CErrorProcessing::InitializeForMethod(
	const char *methodname,
	short facilityid,	//0
	long stationid,		//0
	short schannelid)	//0
{
	m_sFacilityID = facilityid;
	m_csMethodName = CString(methodname);
	m_lStationID = stationid;
	m_sChannelID = schannelid;

	m_csLastErrorText.Empty();
	m_lLastErrorValue = 0;
}

long CErrorProcessing::GetSeverityFromINIFile(long lMessageID)
{
	sprintf(m_charbuf,"%d",lMessageID);
	int iSeverityFromINIFile = ::GetPrivateProfileInt(m_charbuf,"Sev",6,(LPCTSTR)m_csMessageFormatFile);
	if (iSeverityFromINIFile < 1 || iSeverityFromINIFile > 6)
		iSeverityFromINIFile = 2;
	m_lSeverityFromIniFile = (long)iSeverityFromINIFile;
	return m_lSeverityFromIniFile;
}

/*
void CErrorProcessing::ExtractINIFileEntryInfo(
	long lMessageID, 
	INI_ENTRIES *pie,
	UINT unumstringsinput)
{
	DWORD dw;
	pie->lMessageID = lMessageID;

	sprintf(pie->szMessageID,"%d",lMessageID);
	pie->sSeverityFromINIFile = ::GetPrivateProfileInt(pie->szMessageID,"Sev",6,m_csMessageFormatFile);
	dw = ::GetPrivateProfileString(pie->szMessageID,"SOH","",pie->szSOH, sizeof(pie->szSOH), m_csMessageFormatFile);

	dw = ::GetPrivateProfileString(
			pie->szMessageID,
			"Msg",
			"ERROR",
			pie->szMsg,
			sizeof(pie->szMsg),
			m_csMessageFormatFile);

	if (0 == strcmp(pie->szMsg,"ERROR"))  //INI file read okay
	{
		strncpy(pie->szMsg,"No message was available: ",sizeof(pie->szMsg));

		for (UINT u = 0; u < unumstringsinput; u++)
		{
			if (u < MAX_NUMBER_OF_SUBSTITUTION_STRINGS)
				strcat(pie->szMsg, " %s");
			else
				break;
		}
	}

	int iPos;
	pie->sNumSubstitutions = 0;
	CString str(pie->szMsg);
	while ((iPos = str.Find("%s"))>=0)
	{
		str.SetAt(iPos,'^');
		pie->sNumSubstitutions++;
	}
}
*/
void CErrorProcessing::SetDisableLogger(bool b)
{
	if (m_bDisableLogger == b)  //nothing to do
		return;

#ifdef B2R1_PRELIM  //<<QA>>
	m_bDisableLogger = true;
#else
	m_bDisableLogger = b;
#endif

	try
	{
		if (m_bDisableLogger)
		{
			m_pILogger = NULL;  //smart pointer
		}
		else
		{
			m_pILogger.CreateInstance(CLSID_IRSLogger);

			if (m_pILogger)
			{
				VARIANT_BOOL vb = 
					m_pILogger->Startup(m_bstrComponentGUID,m_sUniqueID,m_sVerbosity);

				ASSERT(vb == VARIANT_TRUE);

				if (vb == VARIANT_FALSE)
					m_pILogger = NULL;
				else if (!m_bStartTimeAlreadySent)
					SendStartTimeToLogger();
			}
		}
	}
	catch(...)
	{
		m_pILogger = NULL;
	}
}


//
// "real" functions
//
void CErrorProcessing::Generic(long lMessageID, CString& csin)
{
	m_lOriginalMessageID = lMessageID;
	long severity = GetSeverityFromINIFile(lMessageID);
	LogMessage(100 + severity, csin);
}
void CErrorProcessing::SuccessReadFile(CString& dbOutputString){
	m_cs = m_csSuccessReadingFile + dbOutputString;
	Generic(24, m_cs);
}
void CErrorProcessing::ErrorInvalidPointer(){;
	Generic(14, m_csInvalidPointer);
}
void CErrorProcessing::ErrorEmbeddedInterfaceCallFailed(CString& internalmethodname){
	m_cs = m_csInternalInterfaceCallFailed + internalmethodname;
	Generic(18,m_cs);
}
void CErrorProcessing::ErrorUnimplementedMethod(){
	Generic(17, m_csUnimplementedMethod );
}
void CErrorProcessing::ErrorStartTimeAfterEndTime(DATE starttime, DATE endtime){
	COleDateTime st = COleDateTime(starttime);
	COleDateTime et = COleDateTime(endtime);
	CString csstart = st.Format((LPCTSTR)m_csdatefmtstr); 
	CString csend = et.Format((LPCTSTR)m_csdatefmtstr);
	m_cs = m_csIntervalStartTime  + csstart + m_csExceedsEndTime + csend + m_csCloseParen;
	Generic(16,m_cs);
}
void CErrorProcessing::ErrorParametersNotSet(CString& additionalinfo){
	m_cs = m_csParametersNotSet + additionalinfo;
	Generic(15,m_cs);
}
void CErrorProcessing::ErrorSAFEARRAY(CString& additionalinfo){
	m_cs = m_csSAFEARRAYError  + additionalinfo;
	Generic(19,m_cs);
}
void CErrorProcessing::ErrorOpenDatabase(const CString& additionalinfo){
	Generic(1,m_csCouldNotOpenDatabase + additionalinfo);
}
void CErrorProcessing::ErrorReadFile(CString& filename, CString& additionalinfo){
	m_cs = m_csCouldNotReadFile + filename + m_csColon + additionalinfo;
	Generic(3,m_cs);
}
void CErrorProcessing::ErrorFindFile(CString& filename, CString& additionalinfo){
	//if (!filename.IsEmpty())
		m_cs = m_csCouldNotFindFile + filename + m_csColon + additionalinfo;
	//else
	//	m_cs = m_csCouldNotFindFile + filename;
	Generic(9,m_cs);
}
void CErrorProcessing::ErrorFindDatabase(CString& databasename, CString& additionalinfo){
	if (!additionalinfo.IsEmpty())
		m_cs = m_csCouldNotFindDatabase + databasename + m_csColon + additionalinfo;
	else
		m_cs = m_csCouldNotFindDatabase + databasename;
	Generic(8,m_cs);
}
void CErrorProcessing::ErrorOpenFile(const CString& filename, const CString& additionalinfo){
	if (filename.IsEmpty())
		m_cs = m_csCouldNotOpenFile + additionalinfo;
	else
		m_cs = m_csCouldNotOpenFile + filename + ":" + additionalinfo;
	Generic(2,m_cs);
}
void CErrorProcessing::COMErrorCatch(_com_error& e){
	m_cs = m_csCaughtCOMErrorException + CString(e.ErrorMessage());
	Generic(27,m_cs);
}
void CErrorProcessing::ADOErrorCatch(CAdodbException *cae)
{
	char buf[1024];
	cae->GetErrorMessage(buf,sizeof(buf));
	cae->Delete();
	m_cs = m_csCaughtADODBException + CString(buf);
	Generic(27,m_cs);
}
void CErrorProcessing::ErrorCatch(int inum)
{
	m_cs.Format("%s Internal exception value = %d.", m_csCaughtException, inum);
	Generic(12,m_cs);//m_csCaughtException + m_cs);//cs2);
}
void CErrorProcessing::ErrorCatch()
{

	DWORD dw = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	m_cs.Format("System GetLastError() returns code %d, \"%s",dw,(char *)lpMsgBuf);
	int ilen = m_cs.GetLength();
	m_cs = m_cs.Left(ilen-2);
	m_cs += "\"";
	m_cs = m_csCaughtException + m_cs;
	LocalFree(lpMsgBuf);
	Generic(12,m_cs);//m_csCaughtException + m_cs);//cs2);
}
void CErrorProcessing::ErrorDatabase(CString& dbtype, CString& additionalinfo){
	m_cs = m_csDatabaseError + dbtype + m_csColon + additionalinfo;
	Generic(10,m_cs);
}
void CErrorProcessing::ErrorCopyDatabase(CString& additionalinfo){
	m_cs = m_csCouldNotCopyDatabase + additionalinfo;
	Generic(7,m_cs);
}
void CErrorProcessing::ErrorCreateDatabase(CString& additionalinfo){
	m_cs = m_csCouldNotCreateDatabase + additionalinfo;
	Generic(11,m_cs);
}
void CErrorProcessing::ErrorDebug(CString& additionalinfo){
	Generic(13, m_csDebug + additionalinfo);
}
void CErrorProcessing::SendStartTimeToLogger(){
	m_cs = "COM " + m_csComponentName + " started on facility " + m_csFacilityName;
	m_bPrependComponentAndMethodStrings = false;
	Generic(25,m_cs);
	m_bStartTimeAlreadySent = true;
	m_bPrependComponentAndMethodStrings = true;
}
void CErrorProcessing::StopTime(){
	m_cs = "COM " + m_csComponentName + " shut down on facility " + m_csFacilityName;
	m_bPrependComponentAndMethodStrings = false;
	Generic(26,m_cs);
	m_bPrependComponentAndMethodStrings = true;
}

//void CErrorProcessing::LogMessage(long lMessageID){
//	LogMessage(lMessageID,m_csNULL);
//}
void CErrorProcessing::LogMessage(long lMessageID, CString& csinput)
{
	SetGetLastErrorSeverity();

	if (m_bPrependComponentAndMethodStrings)
	{ 
		m_csLastErrorText = m_csComponentName + m_csColon + m_csMethodName + m_csColon + csinput;//inputstring[0];
	}
	else
	{
		m_csLastErrorText = csinput;
		m_bPrependComponentAndMethodStrings = true;
	}

#ifdef B2R1_PRELIM  //<<QA>>
	if (true)
#else
	if (m_bDisableLogger)  //From caller's SetParameters results
#endif
		return;

	VARIANT varSubstitutions;		//The SAFEARRAY container for the BSTR's
	::VariantInit(&varSubstitutions);	//Sets VT_EMPTY

	SAFEARRAYBOUND bounds;
	bounds.cElements = 1;//substitutions
	bounds.lLbound = 0;

	varSubstitutions.parray = SafeArrayCreate(VT_VARIANT,1,&bounds);
	varSubstitutions.vt = VT_VARIANT|VT_ARRAY;

	long lDimension = 0;

	VARIANT var;
	::VariantInit(&var);
	var.vt = VT_BSTR;
	var.bstrVal = m_csLastErrorText.AllocSysString();

	SafeArrayPutElement(varSubstitutions.parray,&lDimension,&var);
	::VariantClear(&var);

	try
	{
		if (m_pILogger)
		{
			DATE date;
			if (m_lOriginalMessageID == 25) //startup
			{
				date = m_dtStartUp.m_dt;
			}
			else
			{
				COleDateTime datetime = COleDateTime::GetCurrentTime();
				date = datetime.m_dt;
			}

			m_pILogger->LogMessage(
				date,
				m_sFacilityID,
				lMessageID,
				m_sVerbosity,
				m_bstrMessageFormatFile,
				varSubstitutions);
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
	}
	::VariantClear(&varSubstitutions);
}

void CErrorProcessing::SetGetLastErrorSeverity()
	//INI_ENTRIES ie, 
	//CString inputstring[],
	//UINT substitutions_to_be_made)
{
/*
	Per NDAR I3S Software Design Document (Table 9)
		1 = AUTH
		2 = FATAL
		3 = ERROR
		4 = WARN
		5 = INFO
		6 = DIAG

	But, for GetLastError, we have these in our LANL specs:
		#define FATAL		(0x80000000)
		#define NON_FATAL	(0x40000000)
		#define WARNING		(0x20000000)
*/
	long lSeverityForGetLastError;

	switch (m_lSeverityFromIniFile)
	{
		case 1:
		case 2:
			lSeverityForGetLastError = FATAL; break;
		case 3:
			lSeverityForGetLastError = NON_FATAL; break;
		case 4:
			lSeverityForGetLastError = WARNING; break;
		default:
			lSeverityForGetLastError = 0;
	}

	m_lLastErrorValue = lSeverityForGetLastError | m_lOriginalMessageID;
}

///////////////////////////////////////////////////
//
//			CLASS CAdodbException
//
///////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CAdodbException,CException);

CAdodbException::CAdodbException():CException(TRUE) //makes explicit delete work
{
	strncpy(infoADO,"No info string available.",sizeof(infoADO));
}
CAdodbException::CAdodbException(CString &cs):CException(TRUE) //makes explicit delete work
{
	strncpy(infoADO,LPCTSTR(cs),sizeof(infoADO));
}
CAdodbException::CAdodbException(char *pch):CException(TRUE) //makes explicit delete work
{
	strncpy(infoADO,pch,sizeof(infoADO));
}

void CAdodbException::SetErrorString(LPCTSTR errorstr)	
	{strncpy(infoADO,errorstr,sizeof(infoADO));}

void CAdodbException::SetErrorString(CString &cs)		
	{strncpy(infoADO,(LPCTSTR)cs,sizeof(infoADO));}

BOOL CAdodbException::GetErrorMessage(LPTSTR pszErr,UINT nMaxErr,UINT* pnHelpID)
{
	if ((pszErr == NULL) || (nMaxErr == 0))
		return FALSE;

	strncpy(pszErr,infoADO,nMaxErr);
	return TRUE;
}

/*
class CAdodbException : public CException
{
	DECLARE_DYNAMIC(CAdodbException);

	CAdodbException();
	CAdodbException(CString &cs);
	CAdodbException(char *pch);

	char infoADO[256];

	void SetErrorString(LPCTSTR errorstr);
	void SetErrorString(CString &cs);

	BOOL GetErrorMessage(LPTSTR pszErr,UINT nMaxErr,UINT* pnHelpID = NULL);
};
*/
#ifdef _DEBUG
//Diagnostic function
void CErrorProcessing::DoAll()
{
	CString cs0("cs0");
	CString cs1("cs1");
	CString cs2("cs2");
	CString divider("*******************************************");
	Generic(6,divider);

	//Generic(6,cs0);
	SuccessReadFile(cs0);	Sleep(1000);

	ErrorInvalidPointer();
	ErrorEmbeddedInterfaceCallFailed(cs0);
	ErrorUnimplementedMethod();	Sleep(1000);
	ErrorStartTimeAfterEndTime(234567.,334567.);
	ErrorParametersNotSet(cs0);
	ErrorSAFEARRAY(cs0);	
	ErrorOpenDatabase(cs0);	
	ErrorReadFile(cs0,cs1);	
	ErrorFindFile(cs0,cs1);	
	ErrorFindDatabase(cs0,cs1);	
	ErrorOpenFile(cs0,cs1);	
	//COMErrorCatch(e);
	ErrorCatch();	
	ErrorDatabase(cs0,cs1);	
	ErrorCopyDatabase(cs0);	
	//SendStartTimeToLogger();
	//StopTime();
	ErrorCreateDatabase(cs0);
	//ErrorDebug(cs0);
}
#endif
