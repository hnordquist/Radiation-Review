
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Wed Nov 12 09:27:07 2014
 */
/* Compiler settings for CoGrandImport.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __CoGrandImport_h__
#define __CoGrandImport_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IGrandImport_FWD_DEFINED__
#define __IGrandImport_FWD_DEFINED__
typedef interface IGrandImport IGrandImport;
#endif 	/* __IGrandImport_FWD_DEFINED__ */


#ifndef __GrandImport_FWD_DEFINED__
#define __GrandImport_FWD_DEFINED__

#ifdef __cplusplus
typedef class GrandImport GrandImport;
#else
typedef struct GrandImport GrandImport;
#endif /* __cplusplus */

#endif 	/* __GrandImport_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IGrandImport_INTERFACE_DEFINED__
#define __IGrandImport_INTERFACE_DEFINED__

/* interface IGrandImport */
/* [oleautomation][unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IGrandImport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BD000CB1-05DF-11D6-8176-00C04F60E89B")
    IGrandImport : public IDispatch
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
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteData( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE dtStartDate,
            /* [in] */ DATE dtEndDate,
            /* [retval][out] */ VARIANT_BOOL *pbStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteDataStore( 
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataByChannel( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataByStation( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStatistics( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
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
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStoreStatus( 
            /* [in] */ short sFacilityID,
            /* [retval][out] */ long *plStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataStoreTimeRange( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ VARIANT_BOOL bErrDisplay,
            /* [out] */ DATE *FirstTimestamp,
            /* [out] */ DATE *LastTimestamp,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDayStatistics( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDaySummaryData( 
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
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
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLastError( 
            /* [in] */ short sFacilityID,
            /* [out][in] */ BSTR *pbstrErrortext,
            /* [retval][out] */ long *plError) = 0;
        
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
            /* [in] */ VARIANT vParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGrandImportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGrandImport * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGrandImport * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGrandImport * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGrandImport * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGrandImport * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGrandImport * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGrandImport * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddData )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrFilename,
            /* [in] */ VARIANT_BOOL bOverwrite,
            /* [out] */ BSTR *bstrResult,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BackupDataStore )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrDestinationPath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseDataStore )( 
            IGrandImport * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewDataStore )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteData )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE dtStartDate,
            /* [in] */ DATE dtEndDate,
            /* [retval][out] */ VARIANT_BOOL *pbStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteDataStore )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataByChannel )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataByStation )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStatistics )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStorePath )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ BSTR *bstrStorePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreSize )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [out] */ double *pdSizeInBytes,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreStatus )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ long *plStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreTimeRange )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ VARIANT_BOOL bErrDisplay,
            /* [out] */ DATE *FirstTimestamp,
            /* [out] */ DATE *LastTimestamp,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDayStatistics )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDaySummaryData )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ VARIANT *pvDay,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilesList )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrRootDir,
            /* [in] */ VARIANT_BOOL bIncludeSubDirs,
            /* [in] */ VARIANT_BOOL bMostRecent,
            /* [out] */ VARIANT *psaFilesList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [out][in] */ BSTR *pbstrErrortext,
            /* [retval][out] */ long *plError);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetParameters )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [out] */ VARIANT *pvParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitializeDataStore )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenDataStore )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreDataStore )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrSourcePath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetParameters )( 
            IGrandImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT vParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        END_INTERFACE
    } IGrandImportVtbl;

    interface IGrandImport
    {
        CONST_VTBL struct IGrandImportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGrandImport_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGrandImport_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGrandImport_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGrandImport_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IGrandImport_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IGrandImport_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IGrandImport_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IGrandImport_AddData(This,sFacilityID,bstrFilename,bOverwrite,bstrResult,pbResult)	\
    (This)->lpVtbl -> AddData(This,sFacilityID,bstrFilename,bOverwrite,bstrResult,pbResult)

#define IGrandImport_BackupDataStore(This,sFacilityID,bstrDestinationPath,bQuestion,bDefault,pbResult)	\
    (This)->lpVtbl -> BackupDataStore(This,sFacilityID,bstrDestinationPath,bQuestion,bDefault,pbResult)

#define IGrandImport_CloseDataStore(This)	\
    (This)->lpVtbl -> CloseDataStore(This)

#define IGrandImport_CreateNewDataStore(This,sFacilityID,pbResult)	\
    (This)->lpVtbl -> CreateNewDataStore(This,sFacilityID,pbResult)

#define IGrandImport_DeleteData(This,sFacilityID,lStationID,dtStartDate,dtEndDate,pbStatus)	\
    (This)->lpVtbl -> DeleteData(This,sFacilityID,lStationID,dtStartDate,dtEndDate,pbStatus)

#define IGrandImport_DeleteDataStore(This,sFacilityID,pbStatus)	\
    (This)->lpVtbl -> DeleteDataStore(This,sFacilityID,pbStatus)

#define IGrandImport_GetDataByChannel(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pvData,pbResult)	\
    (This)->lpVtbl -> GetDataByChannel(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pvData,pbResult)

#define IGrandImport_GetDataByStation(This,sFacilityID,lStationID,StartTime,EndTime,pvData,pbResult)	\
    (This)->lpVtbl -> GetDataByStation(This,sFacilityID,lStationID,StartTime,EndTime,pvData,pbResult)

#define IGrandImport_GetDataStatistics(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)	\
    (This)->lpVtbl -> GetDataStatistics(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)

#define IGrandImport_GetDataStorePath(This,sFacilityID,bstrStorePath)	\
    (This)->lpVtbl -> GetDataStorePath(This,sFacilityID,bstrStorePath)

#define IGrandImport_GetDataStoreSize(This,sFacilityID,pdSizeInBytes,pbResult)	\
    (This)->lpVtbl -> GetDataStoreSize(This,sFacilityID,pdSizeInBytes,pbResult)

#define IGrandImport_GetDataStoreStatus(This,sFacilityID,plStatus)	\
    (This)->lpVtbl -> GetDataStoreStatus(This,sFacilityID,plStatus)

#define IGrandImport_GetDataStoreTimeRange(This,sFacilityID,lStationID,bErrDisplay,FirstTimestamp,LastTimestamp,pbResult)	\
    (This)->lpVtbl -> GetDataStoreTimeRange(This,sFacilityID,lStationID,bErrDisplay,FirstTimestamp,LastTimestamp,pbResult)

#define IGrandImport_GetDayStatistics(This,sFacilityID,lStationID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)	\
    (This)->lpVtbl -> GetDayStatistics(This,sFacilityID,lStationID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)

#define IGrandImport_GetDaySummaryData(This,sFacilityID,lStationID,StartTime,EndTime,pvDay,pbResult)	\
    (This)->lpVtbl -> GetDaySummaryData(This,sFacilityID,lStationID,StartTime,EndTime,pvDay,pbResult)

#define IGrandImport_GetFilesList(This,sFacilityID,bstrRootDir,bIncludeSubDirs,bMostRecent,psaFilesList)	\
    (This)->lpVtbl -> GetFilesList(This,sFacilityID,bstrRootDir,bIncludeSubDirs,bMostRecent,psaFilesList)

#define IGrandImport_GetLastError(This,sFacilityID,pbstrErrortext,plError)	\
    (This)->lpVtbl -> GetLastError(This,sFacilityID,pbstrErrortext,plError)

#define IGrandImport_GetParameters(This,sFacilityID,pvParms,pbResult)	\
    (This)->lpVtbl -> GetParameters(This,sFacilityID,pvParms,pbResult)

#define IGrandImport_InitializeDataStore(This,sFacilityID,bQuestion,pbSuccess)	\
    (This)->lpVtbl -> InitializeDataStore(This,sFacilityID,bQuestion,pbSuccess)

#define IGrandImport_OpenDataStore(This,sFacilityID,pbSuccess)	\
    (This)->lpVtbl -> OpenDataStore(This,sFacilityID,pbSuccess)

#define IGrandImport_RestoreDataStore(This,sFacilityID,bstrSourcePath,bQuestion,bDefault,pbResult)	\
    (This)->lpVtbl -> RestoreDataStore(This,sFacilityID,bstrSourcePath,bQuestion,bDefault,pbResult)

#define IGrandImport_SetParameters(This,sFacilityID,vParms,pbResult)	\
    (This)->lpVtbl -> SetParameters(This,sFacilityID,vParms,pbResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_AddData_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ BSTR bstrFilename,
    /* [in] */ VARIANT_BOOL bOverwrite,
    /* [out] */ BSTR *bstrResult,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_AddData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_BackupDataStore_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ BSTR bstrDestinationPath,
    /* [in] */ VARIANT_BOOL bQuestion,
    /* [in] */ VARIANT_BOOL bDefault,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_BackupDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_CloseDataStore_Proxy( 
    IGrandImport * This);


void __RPC_STUB IGrandImport_CloseDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_CreateNewDataStore_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_CreateNewDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_DeleteData_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ DATE dtStartDate,
    /* [in] */ DATE dtEndDate,
    /* [retval][out] */ VARIANT_BOOL *pbStatus);


void __RPC_STUB IGrandImport_DeleteData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_DeleteDataStore_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [retval][out] */ VARIANT_BOOL *pbStatus);


void __RPC_STUB IGrandImport_DeleteDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataByChannel_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ short sChannelID,
    /* [in] */ DATE StartTime,
    /* [in] */ DATE EndTime,
    /* [out][in] */ VARIANT *pvData,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDataByChannel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataByStation_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ DATE StartTime,
    /* [in] */ DATE EndTime,
    /* [out][in] */ VARIANT *pvData,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDataByStation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataStatistics_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ short sChannelID,
    /* [in] */ DATE StartTime,
    /* [in] */ DATE EndTime,
    /* [out] */ DATE *pActualStartTime,
    /* [out] */ DATE *pActualEndTime,
    /* [out] */ long *plNumMembers,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDataStatistics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataStorePath_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [retval][out] */ BSTR *bstrStorePath);


void __RPC_STUB IGrandImport_GetDataStorePath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataStoreSize_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [out] */ double *pdSizeInBytes,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDataStoreSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataStoreStatus_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [retval][out] */ long *plStatus);


void __RPC_STUB IGrandImport_GetDataStoreStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDataStoreTimeRange_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ VARIANT_BOOL bErrDisplay,
    /* [out] */ DATE *FirstTimestamp,
    /* [out] */ DATE *LastTimestamp,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDataStoreTimeRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDayStatistics_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ DATE StartTime,
    /* [in] */ DATE EndTime,
    /* [out] */ DATE *pActualStartTime,
    /* [out] */ DATE *pActualEndTime,
    /* [out] */ long *plNumMembers,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDayStatistics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetDaySummaryData_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ long lStationID,
    /* [in] */ DATE StartTime,
    /* [in] */ DATE EndTime,
    /* [out] */ VARIANT *pvDay,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetDaySummaryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetFilesList_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ BSTR bstrRootDir,
    /* [in] */ VARIANT_BOOL bIncludeSubDirs,
    /* [in] */ VARIANT_BOOL bMostRecent,
    /* [out] */ VARIANT *psaFilesList);


void __RPC_STUB IGrandImport_GetFilesList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetLastError_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [out][in] */ BSTR *pbstrErrortext,
    /* [retval][out] */ long *plError);


void __RPC_STUB IGrandImport_GetLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_GetParameters_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [out] */ VARIANT *pvParms,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_GetParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_InitializeDataStore_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ VARIANT_BOOL bQuestion,
    /* [retval][out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IGrandImport_InitializeDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_OpenDataStore_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [retval][out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB IGrandImport_OpenDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_RestoreDataStore_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ BSTR bstrSourcePath,
    /* [in] */ VARIANT_BOOL bQuestion,
    /* [in] */ VARIANT_BOOL bDefault,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_RestoreDataStore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGrandImport_SetParameters_Proxy( 
    IGrandImport * This,
    /* [in] */ short sFacilityID,
    /* [in] */ VARIANT vParms,
    /* [retval][out] */ VARIANT_BOOL *pbResult);


void __RPC_STUB IGrandImport_SetParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGrandImport_INTERFACE_DEFINED__ */



#ifndef __COGRANDIMPORTLib_LIBRARY_DEFINED__
#define __COGRANDIMPORTLib_LIBRARY_DEFINED__

/* library COGRANDIMPORTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COGRANDIMPORTLib;

EXTERN_C const CLSID CLSID_GrandImport;

#ifdef __cplusplus

class DECLSPEC_UUID("BD000CB2-05DF-11D6-8176-00C04F60E89B")
GrandImport;
#endif
#endif /* __COGRANDIMPORTLib_LIBRARY_DEFINED__ */

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


