#ifndef _ZAPPTEXT_INCLUDED
#define _ZAPPTEXT_INCLUDED 1
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


class Z_DLLSPEC zTextBuf {
	int off;
	int r, c;
	char** lines;		// the text buffer.
	zRange* upTab;		// the per line update areas.
	char*	lineLens;	// length of each line.
	int loUp, hiUp;		// lowest & highest line numbers needing update.
	int getRow(int i) { return (i+off)%r; }
	char* operator[](int i) {	return lines[(i+off)%r]; }
public:
	zTextBuf(int rows=25, int cols=80);
	~zTextBuf();
	
	char* getChars(zPoint p);
	int attStrLen(int x,int,int count) { return count-x; }
	zRange& dirty(int i) { return upTab[getRow(i)]; }
	void addDirtyRow(int, const zRange&);
	zRange getDirtyRows();
	void clearDirtyRows() { loUp = r; hiUp = 0; }
	int copyStr(int x, int y, char* s, int count, zPoint &p);
	void scrollVert(int i);
	int lineLen(int i) { return lineLens[getRow(i)]; }
	void clearRect(zRect* r = 0);
#ifdef Z_ASIAN
	int zInMiddleOfCharacter(const zPoint& p);
#endif
	void exchange(zTextBuf&);
};

#ifndef nl
#define nl <<"\n"
#endif

class Z_DLLSPEC zTextStream {
public:
	virtual void put(char *);
	virtual zTextStream& putchr(char c);
	void printf(const char *s,...);
	
	zTextStream& operator<<(char c) { return putchr(c); }
	zTextStream& operator<<(unsigned char c) { return putchr(c); }
	zTextStream& operator<<(char*s) { put(s); return *this; }
	zTextStream& operator<<(int i)  { printf("%d",i); return *this; }
	zTextStream& operator<<(unsigned int i)
		{ printf("%u",i); return *this; }
	zTextStream& operator<<(long i)
		{ printf("%ld",i); return *this; }
	zTextStream& operator<<(unsigned long i)
		{ printf("%lu",i); return *this; }

	zTextStream& operator<<(void* op)
		{ printf("%x:%x",HIWORD(op),LOWORD(op)); return *this; }
};

char *form(char *,...);

zTextStream& Z_DLLSPEC operator<<(zTextStream&,zPoint);
zTextStream& Z_DLLSPEC operator<<(zTextStream&,zDimension);

class Z_DLLSPEC zTextPane:public zPane,public zTextStream {
protected:
	zTextBuf tb;
	zPoint* UpdateTab;
	zPoint loc;
	int rows, cols, rowOff, colOff;
	int cHeight, cWidth, topMargin, bottomMargin, leftMargin, rightMargin;
	BOOL deferOutput, needUpdate;
	int sol;
	void tpsetup(int r=25,int c=80);
public:
	zTextPane(zWindow*,zSizer*,DWORD =0,int r=25,int c=80);
	zTextPane(zWindow*,zSizer*,DWORD style_,const char*, int ctrlId_=0);
	zTextPane(zWindow*,int ctrlid_);
	zPoint pos() { return loc; }
	void moveStart() { loc.x() = sol;	}
	void moveHome() { loc.x() = sol; loc.y() = 0; }
	void moveDown(int count=1);
	void moveRight(int count=1);
	void moveLeft(int count=1);
	void moveUp(int count=1);
	void moveTo(int x, int y) { loc.x() = Z_MAX(sol, x); loc.y() = y; }
	void clearRect(zRect* r = 0);
	void scrollVert(int count=1);
	virtual void drawLine(zPoint p, int count);
	
	virtual void logToText(zPoint&);
	virtual void logToText(zRect&);
	virtual void textToLog(zPoint&);
	virtual void textToLog(zRect &);
	virtual void put(char *s);
#ifdef __IBMCPP__
	virtual zTextStream& put(char c) { return zTextStream::putchr(c); }
#endif
	virtual int draw(zDrawEvt* Ev);
	int startOfLine() { return sol; }
	int startOfLine(int i) { sol ^= i; i ^= sol; sol ^= i; return i; }
	virtual void show(int=SW_SHOWNORMAL);
	virtual ~zTextPane();
	virtual BOOL getPreferredSize(zDimension &);
	void printAt(int x, int y, const char *s, ...);
    virtual void setMetrics();
#ifdef Z_ASIAN
	int zInMiddleOfCharacter(const zPoint& p);
#endif
	zTextBuf* ziTextBuf() { return &tb; }
};
#endif




