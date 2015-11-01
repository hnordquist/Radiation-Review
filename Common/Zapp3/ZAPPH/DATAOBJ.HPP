#ifndef _DATAOBJ_H_
#define _DATAOBJ_H_
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

class Z_DLLSPEC zDataObjectImp;

class Z_DLLSPEC zDataObject {
public:
	zDataObject();
	~zDataObject();
	void clear();

	BOOL add(const char *);
	BOOL add(zBitmap *);
	BOOL add(zMetafileDisplay *);
	int getMetafile(zMetafileDisplay*);
	BOOL isMetafileAvail();
	virtual void onReceiveOleFmt(LPFORMATETC,LPSTGMEDIUM) {};
	BOOL add(void *pData, unsigned long length, const zExchangeFmt &fmt); 
	char* getText();
	zBitmap* getBitmap();
	void* getUserData(unsigned long &len, const zExchangeFmt &fmt);
	BOOL isTextAvail();
	BOOL isBitmapAvail();
	BOOL isUserDataAvail(const zExchangeFmt &fmt);
	BOOL registerFormat(const zExchangeFmt &fmt);
	BOOL unRegisterFormat(const zExchangeFmt &fmt);
	virtual void onReceiveFormat(const zExchangeFmt &) {};

protected:
	zDataObjectImp *pImp;
	void receiveFormat(const zExchangeFmt &fmt) { onReceiveFormat(fmt); };
	void receiveOleFmt(LPFORMATETC fe,LPSTGMEDIUM stgm) {onReceiveOleFmt(fe,stgm);};
private:
	friend class Z_DLLSPEC zDataObjectImp;
public:	
	BOOL registerOleFormat(LPFORMATETC,LPSTGMEDIUM);
	BOOL unRegisterOleFormat(LPFORMATETC,LPSTGMEDIUM);
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
};


#endif
