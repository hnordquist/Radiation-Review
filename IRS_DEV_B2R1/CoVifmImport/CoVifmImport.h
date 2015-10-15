

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Jan 29 10:25:57 2014
 */
/* Compiler settings for .\CoVifmImport.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CoVifmImport_h__
#define __CoVifmImport_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVifmImport_FWD_DEFINED__
#define __IVifmImport_FWD_DEFINED__
typedef interface IVifmImport IVifmImport;
#endif 	/* __IVifmImport_FWD_DEFINED__ */


#ifndef __VifmImport_FWD_DEFINED__
#define __VifmImport_FWD_DEFINED__

#ifdef __cplusplus
typedef class VifmImport VifmImport;
#else
typedef struct VifmImport VifmImport;
#endif /* __cplusplus */

#endif 	/* __VifmImport_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IVifmImport_INTERFACE_DEFINED__
#define __IVifmImport_INTERFACE_DEFINED__

/* interface IVifmImport */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVifmImport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0F0A1D73-667B-11D6-819C-00C04F60E89B")
    IVifmImport : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddData( 
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrFilename,
            /* [in] */ VARIANT_BOOL bOverwrite,
            /* [out] */ BSTR *bstrResult,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BackupDataStore( 
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrDestinationPath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseDataStore( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNewDataStore( 
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataByChannel( 
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataByStation( 
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStatistics( 
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStorePath( 
            /* [in] */ short sFacilityID,
            /* [retval][out] */ BSTR *bstrStorePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStoreSize( 
            /* [in] */ short sFacilityID,
            /* [out] */ double *pdSizeInBytes,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStoreTimeRange( 
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ VARIANT_BOOL bErrDisplay,
            /* [out] */ DATE *FirstTimestamp,
            /* [out] */ DATE *LastTimestamp,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDayStatistics( 
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationId,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDaySummaryData( 
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ VARIANT *pvDay,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFilesList( 
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrRootDir,
            /* [in] */ VARIANT_BOOL bIncludeSubDirs,
            /* [in] */ VARIANT_BOOL bMostRecent,
            /* [out] */ VARIANT *psaFilesList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetParameters( 
            /* [in] */ short sFacilityID,
            /* [out] */ VARIANT *pvParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitializeDataStore( 
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenDataStore( 
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreDataStore( 
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrSourcePath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetParameters( 
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT *pvParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVifmImportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVifmImport * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVifmImport * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVifmImport * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVifmImport * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVifmImport * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVifmImport * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVifmImport * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddData )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrFilename,
            /* [in] */ VARIANT_BOOL bOverwrite,
            /* [out] */ BSTR *bstrResult,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BackupDataStore )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrDestinationPath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseDataStore )( 
            IVifmImport * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewDataStore )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataByChannel )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataByStation )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStatistics )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStorePath )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ BSTR *bstrStorePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreSize )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [out] */ double *pdSizeInBytes,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreTimeRange )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ VARIANT_BOOL bErrDisplay,
            /* [out] */ DATE *FirstTimestamp,
            /* [out] */ DATE *LastTimestamp,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDayStatistics )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationId,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDaySummaryData )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ short sStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ VARIANT *pvDay,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilesList )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrRootDir,
            /* [in] */ VARIANT_BOOL bIncludeSubDirs,
            /* [in] */ VARIANT_BOOL bMostRecent,
            /* [out] */ VARIANT *psaFilesList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetParameters )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [out] */ VARIANT *pvParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitializeDataStore )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenDataStore )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreDataStore )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrSourcePath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetParameters )( 
            IVifmImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT *pvParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        END_INTERFACE
    } IVifmImportVtbl;

    interface IVifmImport
    {
        CONST_VTBL struct IVifmImportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVifmImport_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVifmImport_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVifmImport_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVifmImport_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVifmImport_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVifmImport_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVifmImport_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVifmImport_AddData(This,sFacilityID,bstrFilename,bOverwrite,bstrResult,pbResult)	\
    ( (This)->lpVtbl -> AddData(This,sFacilityID,bstrFilename,bOverwrite,bstrResult,pbResult) ) 

#define IVifmImport_BackupDataStore(This,sFacilityID,bstrDestinationPath,bQuestion,bDefault,pbResult)	\
    ( (This)->lpVtbl -> BackupDataStore(This,sFacilityID,bstrDestinationPath,bQuestion,bDefault,pbResult) ) 

#define IVifmImport_CloseDataStore(This)	\
    ( (This)->lpVtbl -> CloseDataStore(This) ) 

#define IVifmImport_CreateNewDataStore(This,sFacilityID,pbResult)	\
    ( (This)->lpVtbl -> CreateNewDataStore(This,sFacilityID,pbResult) ) 

#define IVifmImport_GetDataByChannel(This,sFacilityID,sStationID,sChannelID,StartTime,EndTime,pvData,pbResult)	\
    ( (This)->lpVtbl -> GetDataByChannel(This,sFacilityID,sStationID,sChannelID,StartTime,EndTime,pvData,pbResult) ) 

#define IVifmImport_GetDataByStation(This,sFacilityID,sStationID,StartTime,EndTime,pvData,pbResult)	\
    ( (This)->lpVtbl -> GetDataByStation(This,sFacilityID,sStationID,StartTime,EndTime,pvData,pbResult) ) 

#define IVifmImport_GetDataStatistics(This,sFacilityID,sStationID,sChannelID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)	\
    ( (This)->lpVtbl -> GetDataStatistics(This,sFacilityID,sStationID,sChannelID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult) ) 

#define IVifmImport_GetDataStorePath(This,sFacilityID,bstrStorePath)	\
    ( (This)->lpVtbl -> GetDataStorePath(This,sFacilityID,bstrStorePath) ) 

#define IVifmImport_GetDataStoreSize(This,sFacilityID,pdSizeInBytes,pbResult)	\
    ( (This)->lpVtbl -> GetDataStoreSize(This,sFacilityID,pdSizeInBytes,pbResult) ) 

#define IVifmImport_GetDataStoreTimeRange(This,sFacilityID,sStationID,bErrDisplay,FirstTimestamp,LastTimestamp,pbResult)	\
    ( (This)->lpVtbl -> GetDataStoreTimeRange(This,sFacilityID,sStationID,bErrDisplay,FirstTimestamp,LastTimestamp,pbResult) ) 

#define IVifmImport_GetDayStatistics(This,sFacilityID,sStationId,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)	\
    ( (This)->lpVtbl -> GetDayStatistics(This,sFacilityID,sStationId,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult) ) 

#define IVifmImport_GetDaySummaryData(This,sFacilityID,sStationID,StartTime,EndTime,pvDay,pbResult)	\
    ( (This)->lpVtbl -> GetDaySummaryData(This,sFacilityID,sStationID,StartTime,EndTime,pvDay,pbResult) ) 

#define IVifmImport_GetFilesList(This,sFacilityID,bstrRootDir,bIncludeSubDirs,bMostRecent,psaFilesList)	\
    ( (This)->lpVtbl -> GetFilesList(This,sFacilityID,bstrRootDir,bIncludeSubDirs,bMostRecent,psaFilesList) ) 

#define IVifmImport_GetParameters(This,sFacilityID,pvParms,pbResult)	\
    ( (This)->lpVtbl -> GetParameters(This,sFacilityID,pvParms,pbResult) ) 

#define IVifmImport_InitializeDataStore(This,sFacilityID,bQuestion,pbSuccess)	\
    ( (This)->lpVtbl -> InitializeDataStore(This,sFacilityID,bQuestion,pbSuccess) ) 

#define IVifmImport_OpenDataStore(This,sFacilityID,pbSuccess)	\
    ( (This)->lpVtbl -> OpenDataStore(This,sFacilityID,pbSuccess) ) 

#define IVifmImport_RestoreDataStore(This,sFacilityID,bstrSourcePath,bQuestion,bDefault,pbResult)	\
    ( (This)->lpVtbl -> RestoreDataStore(This,sFacilityID,bstrSourcePath,bQuestion,bDefault,pbResult) ) 

#define IVifmImport_SetParameters(This,sFacilityID,pvParms,pbResult)	\
    ( (This)->lpVtbl -> SetParameters(This,sFacilityID,pvParms,pbResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVifmImport_INTERFACE_DEFINED__ */



#ifndef __COVIFMIMPORTLib_LIBRARY_DEFINED__
#define __COVIFMIMPORTLib_LIBRARY_DEFINED__

/* library COVIFMIMPORTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COVIFMIMPORTLib;

EXTERN_C const CLSID CLSID_VifmImport;

#ifdef __cplusplus

class DECLSPEC_UUID("0F0A1D74-667B-11D6-819C-00C04F60E89B")
VifmImport;
#endif
#endif /* __COVIFMIMPORTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


