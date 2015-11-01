#ifndef _ZAPPCLIP_INCLUDED
#define _ZAPPCLIP_INCLUDED 1
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

enum zExchangeType {
	ZFMT_TEXT=CF_TEXT, ZFMT_BITMAP=CF_BITMAP,
	ZFMT_METAFILE=CF_METAFILEPICT, ZFMT_SYLK=CF_SYLK,
	ZFMT_DIF=CF_DIF, ZFMT_TIFF=CF_TIFF,
	ZFMT_OEMTEXT=CF_OEMTEXT, ZFMT_DIB=CF_DIB,
	ZFMT_PALETTE=CF_PALETTE,
	ZFMT_CUSTOM,
	ZFMT_NONE
};

class Z_DLLSPEC zExchangeFmt {
public:
	zExchangeFmt(zExchangeType);
	zExchangeFmt(const char *);
	zExchangeFmt(const zExchangeFmt&);
	zExchangeFmt(const RWCString&);
	zExchangeFmt(unsigned int);
	unsigned int id() const;
	int operator==(const zExchangeType& d);
	int operator==(const zExchangeFmt& d);
	int operator==(const char*);
	int operator==(const RWCString&);
	operator zExchangeType() const { return fmt; }
private:
	zExchangeType fmt;
	unsigned int fmtId;
};

class Z_DLLSPEC zDataObject;

class Z_DLLSPEC zClipboard:public zEvH {
public:
	zClipboard(zWindow*);
	void add(char *);
	void add(zBitmap*);
	BOOL isMetafileAvail();
	void add(zMetafileDisplay*);
	void add(void *,unsigned long len,zExchangeFmt);
    //add (zDataobject);
	BOOL isUserDataAvail(zExchangeFmt);
        void* getUserData(unsigned long &len,zExchangeFmt);
	BOOL isBitmapAvail();
	BOOL isTextAvail();
	zBitmap *getBitmap(zDisplay*);
	char *getText();
    int getMetafile(zMetafileDisplay* m);
	void clear();
	~zClipboard();
	//enhancements to zClipboard

private:
	zDataObject *copyObject;  //provided to OleSetClipboard
	LPDATAOBJECT lpPasteObject; //acquired from OleGetClipboard
    LPDATAOBJECT lpDataObj;

	int res;
	zWindow *w;

};

#endif

