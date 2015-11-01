#ifndef _DATAOBJI_H_
#define _DATAOBJI_H_
/*
 * (c) Copyright 1991 - 1996 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 **************************************************************************/

class Z_DLLSPEC zDataObject;



class Z_DLLSPEC zDataObjectImp : public IDataObject {
public:
	zDataObjectImp(zDataObject *);
	~zDataObjectImp();

	// zDataObject Implementation Interface
	void     clear();
	BOOL     add(const char *);
	BOOL     add(zBitmap *);
	BOOL     add(void *pData, unsigned long length, const zExchangeFmt &fmt); 
	char*    getText();
	zBitmap* getBitmap();
	void*    getUserData(unsigned long &len, const zExchangeFmt &fmt);
	BOOL     isTextAvail();
	BOOL     isBitmapAvail();
	BOOL     isUserDataAvail(const zExchangeFmt &fmt);

	//Registration functions tell the zDataObjectImp what types
	//of data to accept.
	BOOL     registerFormat(const zExchangeFmt &fmt);
	BOOL     unRegisterFormat(const zExchangeFmt &fmt);
		
	// Windows sepecific interface definitions
	BOOL     add(zMetafileDisplay *);
	int getMetafile(zMetafileDisplay*);
	BOOL     isMetafileAvail();
	BOOL     registerOleFormat(LPFORMATETC,LPSTGMEDIUM);
	BOOL     unRegisterOleFormat(LPFORMATETC,LPSTGMEDIUM);

	//OLE specific functions
	BOOL acceptDataObject(LPDATAOBJECT);
	BOOL fillFromDataObject(LPDATAOBJECT);

	//Non-delegating object IUnknown
	STDMETHODIMP         QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	//IDataObject members
	STDMETHODIMP GetData(LPFORMATETC, LPSTGMEDIUM);
	STDMETHODIMP GetDataHere(LPFORMATETC, LPSTGMEDIUM);
	STDMETHODIMP QueryGetData(LPFORMATETC);
	STDMETHODIMP GetCanonicalFormatEtc(LPFORMATETC,LPFORMATETC);
	STDMETHODIMP SetData(LPFORMATETC, LPSTGMEDIUM, BOOL);
	STDMETHODIMP EnumFormatEtc(DWORD, LPENUMFORMATETC *);
	STDMETHODIMP DAdvise(LPFORMATETC, DWORD,  LPADVISESINK, DWORD *);
	STDMETHODIMP DUnadvise(DWORD);
	STDMETHODIMP EnumDAdvise(LPENUMSTATDATA *);

protected:
	zDataObject*            pInterface_;

	unsigned                numFormats_;
	unsigned		numRegFormats_;
	ULONG                   cRef_;
	LPUNKNOWN               pOuterUnknown_;
	ZPFNDESTROYED           pfnDestroy_;
	
	RWTValSlist<FORMATETC>  fmtEtcVector_; //list of formats we can currently give
	RWTValSlist<STGMEDIUM>  stgMediumVector_;//list of stgmediums we can currently give
	RWTValSlist<FORMATETC>  regFmtEtcVector_;  //list of formats we are interested in receiving
	RWTValSlist<STGMEDIUM>  regStgMediumVector_;//list of stgmediums we are interested in receiving
};


//zEnumFORMATETC object that is created from
// IDataObject::EnumFormatEtc.  This object lives on its own.
class Z_DLLSPEC zEnumFormatEtc : public IEnumFORMATETC {
public:
	zEnumFormatEtc(RWTValSlist<FORMATETC>&);
	zEnumFormatEtc(ULONG, LPFORMATETC);
	~zEnumFormatEtc();

	//IUnknown members
	STDMETHODIMP         QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	
	//IEnumFORMATETC members
	STDMETHODIMP Next(ULONG, LPFORMATETC, ULONG *);
	STDMETHODIMP Skip(ULONG);
	STDMETHODIMP Reset();
	STDMETHODIMP Clone(IEnumFORMATETC **);
private:
	ULONG           cRef_;         //Object reference count
	ULONG           iCur_;         //Current element.
	ULONG           cfe_;          //Number of FORMATETCs in us
	LPFORMATETC     prgfe_;        //Source of FORMATETCs

};

typedef zEnumFormatEtc *pZEnumFormatEtc;

#endif
