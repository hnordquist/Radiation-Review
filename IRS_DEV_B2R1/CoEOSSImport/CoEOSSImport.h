

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Aug 06 11:14:13 2010
 */
/* Compiler settings for CoEOSSImport.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __CoEOSSImport_h__
#define __CoEOSSImport_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IEOSSImport_FWD_DEFINED__
#define __IEOSSImport_FWD_DEFINED__
typedef interface IEOSSImport IEOSSImport;
#endif 	/* __IEOSSImport_FWD_DEFINED__ */


#ifndef __EOSSImport_FWD_DEFINED__
#define __EOSSImport_FWD_DEFINED__

#ifdef __cplusplus
typedef class EOSSImport EOSSImport;
#else
typedef struct EOSSImport EOSSImport;
#endif /* __cplusplus */

#endif 	/* __EOSSImport_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IEOSSImport_INTERFACE_DEFINED__
#define __IEOSSImport_INTERFACE_DEFINED__

/* interface IEOSSImport */
/* [oleautomation][unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEOSSImport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F0B152C3-D724-4578-9306-325DE0E006F3")
    IEOSSImport : public IDispatch
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

    typedef struct IEOSSImportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEOSSImport * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEOSSImport * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEOSSImport * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEOSSImport * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEOSSImport * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEOSSImport * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEOSSImport * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddData )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrFilename,
            /* [in] */ VARIANT_BOOL bOverwrite,
            /* [out] */ BSTR *bstrResult,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BackupDataStore )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrDestinationPath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseDataStore )( 
            IEOSSImport * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewDataStore )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteData )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE dtStartDate,
            /* [in] */ DATE dtEndDate,
            /* [retval][out] */ VARIANT_BOOL *pbStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteDataStore )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataByChannel )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ short sChannelID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataByStation )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out][in] */ VARIANT *pvData,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStatistics )( 
            IEOSSImport * This,
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
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ BSTR *bstrStorePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreSize )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [out] */ double *pdSizeInBytes,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreStatus )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ long *plStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataStoreTimeRange )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ VARIANT_BOOL bErrDisplay,
            /* [out] */ DATE *FirstTimestamp,
            /* [out] */ DATE *LastTimestamp,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDayStatistics )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ DATE *pActualStartTime,
            /* [out] */ DATE *pActualEndTime,
            /* [out] */ long *plNumMembers,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDaySummaryData )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ long lStationID,
            /* [in] */ DATE StartTime,
            /* [in] */ DATE EndTime,
            /* [out] */ VARIANT *pvDay,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilesList )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrRootDir,
            /* [in] */ VARIANT_BOOL bIncludeSubDirs,
            /* [in] */ VARIANT_BOOL bMostRecent,
            /* [out] */ VARIANT *psaFilesList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [out][in] */ BSTR *pbstrErrortext,
            /* [retval][out] */ long *plError);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetParameters )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [out] */ VARIANT *pvParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitializeDataStore )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenDataStore )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [retval][out] */ VARIANT_BOOL *pbSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestoreDataStore )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ BSTR bstrSourcePath,
            /* [in] */ VARIANT_BOOL bQuestion,
            /* [in] */ VARIANT_BOOL bDefault,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetParameters )( 
            IEOSSImport * This,
            /* [in] */ short sFacilityID,
            /* [in] */ VARIANT vParms,
            /* [retval][out] */ VARIANT_BOOL *pbResult);
        
        END_INTERFACE
    } IEOSSImportVtbl;

    interface IEOSSImport
    {
        CONST_VTBL struct IEOSSImportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEOSSImport_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEOSSImport_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEOSSImport_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEOSSImport_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEOSSImport_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEOSSImport_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEOSSImport_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEOSSImport_AddData(This,sFacilityID,bstrFilename,bOverwrite,bstrResult,pbResult)	\
    ( (This)->lpVtbl -> AddData(This,sFacilityID,bstrFilename,bOverwrite,bstrResult,pbResult) ) 

#define IEOSSImport_BackupDataStore(This,sFacilityID,bstrDestinationPath,bQuestion,bDefault,pbResult)	\
    ( (This)->lpVtbl -> BackupDataStore(This,sFacilityID,bstrDestinationPath,bQuestion,bDefault,pbResult) ) 

#define IEOSSImport_CloseDataStore(This)	\
    ( (This)->lpVtbl -> CloseDataStore(This) ) 

#define IEOSSImport_CreateNewDataStore(This,sFacilityID,pbResult)	\
    ( (This)->lpVtbl -> CreateNewDataStore(This,sFacilityID,pbResult) ) 

#define IEOSSImport_DeleteData(This,sFacilityID,lStationID,dtStartDate,dtEndDate,pbStatus)	\
    ( (This)->lpVtbl -> DeleteData(This,sFacilityID,lStationID,dtStartDate,dtEndDate,pbStatus) ) 

#define IEOSSImport_DeleteDataStore(This,sFacilityID,pbStatus)	\
    ( (This)->lpVtbl -> DeleteDataStore(This,sFacilityID,pbStatus) ) 

#define IEOSSImport_GetDataByChannel(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pvData,pbResult)	\
    ( (This)->lpVtbl -> GetDataByChannel(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pvData,pbResult) ) 

#define IEOSSImport_GetDataByStation(This,sFacilityID,lStationID,StartTime,EndTime,pvData,pbResult)	\
    ( (This)->lpVtbl -> GetDataByStation(This,sFacilityID,lStationID,StartTime,EndTime,pvData,pbResult) ) 

#define IEOSSImport_GetDataStatistics(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)	\
    ( (This)->lpVtbl -> GetDataStatistics(This,sFacilityID,lStationID,sChannelID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult) ) 

#define IEOSSImport_GetDataStorePath(This,sFacilityID,bstrStorePath)	\
    ( (This)->lpVtbl -> GetDataStorePath(This,sFacilityID,bstrStorePath) ) 

#define IEOSSImport_GetDataStoreSize(This,sFacilityID,pdSizeInBytes,pbResult)	\
    ( (This)->lpVtbl -> GetDataStoreSize(This,sFacilityID,pdSizeInBytes,pbResult) ) 

#define IEOSSImport_GetDataStoreStatus(This,sFacilityID,plStatus)	\
    ( (This)->lpVtbl -> GetDataStoreStatus(This,sFacilityID,plStatus) ) 

#define IEOSSImport_GetDataStoreTimeRange(This,sFacilityID,lStationID,bErrDisplay,FirstTimestamp,LastTimestamp,pbResult)	\
    ( (This)->lpVtbl -> GetDataStoreTimeRange(This,sFacilityID,lStationID,bErrDisplay,FirstTimestamp,LastTimestamp,pbResult) ) 

#define IEOSSImport_GetDayStatistics(This,sFacilityID,lStationID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult)	\
    ( (This)->lpVtbl -> GetDayStatistics(This,sFacilityID,lStationID,StartTime,EndTime,pActualStartTime,pActualEndTime,plNumMembers,pbResult) ) 

#define IEOSSImport_GetDaySummaryData(This,sFacilityID,lStationID,StartTime,EndTime,pvDay,pbResult)	\
    ( (This)->lpVtbl -> GetDaySummaryData(This,sFacilityID,lStationID,StartTime,EndTime,pvDay,pbResult) ) 

#define IEOSSImport_GetFilesList(This,sFacilityID,bstrRootDir,bIncludeSubDirs,bMostRecent,psaFilesList)	\
    ( (This)->lpVtbl -> GetFilesList(This,sFacilityID,bstrRootDir,bIncludeSubDirs,bMostRecent,psaFilesList) ) 

#define IEOSSImport_GetLastError(This,sFacilityID,pbstrErrortext,plError)	\
    ( (This)->lpVtbl -> GetLastError(This,sFacilityID,pbstrErrortext,plError) ) 

#define IEOSSImport_GetParameters(This,sFacilityID,pvParms,pbResult)	\
    ( (This)->lpVtbl -> GetParameters(This,sFacilityID,pvParms,pbResult) ) 

#define IEOSSImport_InitializeDataStore(This,sFacilityID,bQuestion,pbSuccess)	\
    ( (This)->lpVtbl -> InitializeDataStore(This,sFacilityID,bQuestion,pbSuccess) ) 

#define IEOSSImport_OpenDataStore(This,sFacilityID,pbSuccess)	\
    ( (This)->lpVtbl -> OpenDataStore(This,sFacilityID,pbSuccess) ) 

#define IEOSSImport_RestoreDataStore(This,sFacilityID,bstrSourcePath,bQuestion,bDefault,pbResult)	\
    ( (This)->lpVtbl -> RestoreDataStore(This,sFacilityID,bstrSourcePath,bQuestion,bDefault,pbResult) ) 

#define IEOSSImport_SetParameters(This,sFacilityID,vParms,pbResult)	\
    ( (This)->lpVtbl -> SetParameters(This,sFacilityID,vParms,pbResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEOSSImport_INTERFACE_DEFINED__ */



#ifndef __COEOSSIMPORTLib_LIBRARY_DEFINED__
#define __COEOSSIMPORTLib_LIBRARY_DEFINED__

/* library COEOSSIMPORTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COEOSSIMPORTLib;

EXTERN_C const CLSID CLSID_EOSSImport;

#ifdef __cplusplus

class DECLSPEC_UUID("B29F34D6-F347-4ca8-8014-0E23BE9744AE")
EOSSImport;
#endif
#endif /* __COEOSSIMPORTLib_LIBRARY_DEFINED__ */

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


