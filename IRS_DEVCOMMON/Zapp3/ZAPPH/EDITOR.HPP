#ifndef _ZAPPEDITOR_INCLUDED
#define _ZAPPEDITOR_INCLUDED 1
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

class Z_DLLSPEC zPrinterDisplay;
class Z_DLLSPEC zPrintJob;


class Z_DLLSPEC zFontSizeInfo {
public:
	zFontSizeInfo(zDisplay *dp);
	zCoOrd operator[](int i) const;
	zCoOrd charHeight() const;
	zCoOrd lineHeight() const;
	zCoOrd length(char *, int length);

private:
	zCoOrd _charHeight;
	zCoOrd _extLeading;
	zCoOrd charWidths[256];
};

inline zCoOrd zFontSizeInfo::operator[](int i) const { return charWidths[i]; }
inline zCoOrd zFontSizeInfo::charHeight() const { return _charHeight; }
inline zCoOrd zFontSizeInfo::lineHeight() const { return _charHeight + _extLeading; }


#if defined(Z_OS2) || defined(Z_XM)
class Z_DLLSPEC zEditor:public zEditBox {
#else
class Z_DLLSPEC zEditor:public zEditBase {
#endif
public:
	zEditor(zWindow *,zSizer *, long maxLen = 0x7fffffffL);
	BOOL doPrint(const char* title=0,zPrinterDisplay* =0,zPrJobDlg* =0);
	BOOL openFile(const char *);
	BOOL saveFile(const char *);

	void setHeaderText(const char*);
	void setPrintPageNums(int tf);

protected:
	char* findLineEnd(char *);
	char* findNextLine(char *);
	char* getLine(char *dest, char *src, zCoOrd lineWidth, int &len);
	char* calcLine(char *src, zCoOrd lineWidth);
	char* getNextPage(char *src, zRect &);
	virtual int printSetup(zPrinterDisplay *pr, zPrintJob *pj);
	virtual int print(zPrinterDisplay *pr, zRect *r);
	virtual BOOL morePages();
	virtual void printCleanup(zPrinterDisplay *pr, zPrintJob *pj);
	virtual void printHeader(zPrinterDisplay *pr, int calcOnly=0);
	virtual void printFooter(zPrinterDisplay *pr, int calcOnly=0);
private:
	BOOL inPrint;
	long totalLines, curLine, maxLength;
	int curPage,endPage;
	char *prBuf;
	char *curPrPos;
	char *curPageEnd;
	zFontSizeInfo *fi;
	RWCString headerTxt;
	int printPageNum;
};

#endif

