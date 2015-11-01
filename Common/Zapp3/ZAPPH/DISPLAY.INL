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

inline void zDisplay::flush() {
	GdiFlush();
}



inline zColor::zColor() {
	r=0;
	g=0;
	b=0;
	mode=0;
}

inline zColor::zColor(BYTE rd,BYTE gr,BYTE bl, int palRel) {
	r=rd;
	g=gr;
	b=bl;	
	mode=(BYTE)palRel;
}

inline zColor::zColor(unsigned long c) {
	r=GetRValue(c);
	g=GetGValue(c);
	b=GetBValue(c);
	mode=((BYTE)(c>>24));
}

inline zColor::zColor(const zColor&c):r(c.r),g(c.g),b(c.b),mode(c.mode) {}


inline zColor& zColor::operator=(unsigned long c) {
	r=GetRValue(c);
	g=GetGValue(c);
	b=GetBValue(c);
	mode=(BYTE)PALETTEINDEX(c);
	return *this;
}

inline zColor& zColor::operator=(const zColor& c) {
	r=c.r;
	g=c.g;
	b=c.b;
	mode=c.mode;
	return *this;
}

inline BYTE& zColor::red() {
	return r;
}

inline BYTE& zColor::green() {
	return g;
}

inline BYTE& zColor::blue() {
	return b;
}

inline zColor::operator DWORD() const {
    DWORD pal = ((DWORD)((BYTE)(r) | ((WORD)(g)<<8) | (((DWORD)(BYTE)(b)) << 16) | (((DWORD)(BYTE)(mode)) <<24)));
	if (mode==0x01) return PALETTEINDEX(pal);
	return (DWORD)((mode) ? PALETTERGB(r,g,b):RGB(r,g,b));
}

inline DWORD zColor::id() const {
	return (DWORD)((mode) ? PALETTERGB(r,g,b):RGB(r,g,b));
}

inline BOOL zDisplay::isLocked() const {
	return lockCount;
}

inline BOOL zDisplay::isValid() const {
	return (hDC!=0);
}

inline DispType zDisplay::type() const {
	return dispType;
}

inline int zDisplay::pixWidth() const {
	return _pixWidth;
}

inline int zDisplay::pixHeight() const {
	return _pixHeight;
}

inline int zDisplay::pixPerInchX() const {
	return _pixPerInchX;
}

inline int zDisplay::pixPerInchY() const {
	return _pixPerInchY;
}

inline int zDisplay::mmWidth() const {
	return _mmWidth;
}

inline int zDisplay::mmHeight() const {
	return _mmHeight;
}

inline zColor zDisplay::backColor() const {
	return background;
}

inline zColor zDisplay::textColor() const {
	return foreground;
}

inline zBrush* zDisplay::brush() const {
	return curBrush;
}

inline zPen* zDisplay::pen() const {
	return curPen;
}

inline zFont* zDisplay::font() const {
	return curFont;
}

inline BinDrawOp zDisplay::drawMode() const {
	return drMode;
}


inline unsigned long zDisplay::moveTo(zCoOrd x, zCoOrd y) {
	return MoveToEx(hDC, x, y,0);
}

inline unsigned long zDisplay::moveTo(zPoint p) {
	return MoveToEx(hDC, p.x(), p.y(),0);
}

inline BOOL zDisplay::devToLog(zPoint &p) {
	return DPtoLP(hDC, (LPPOINT)&p, 1);
}
inline BOOL zDisplay::devToLog(zPoint *p, int count) {
	return DPtoLP(hDC, (LPPOINT)p, count);
}
inline BOOL zDisplay::devToLog(zRect &r) {
	return DPtoLP(hDC, (LPPOINT)&r, 2);
}
inline BOOL zDisplay::logToDev(zPoint &p) {
	return LPtoDP(hDC, (LPPOINT)&p, 1);
}
inline BOOL zDisplay::logToDev(zPoint *p, int count) {
	return LPtoDP(hDC, (LPPOINT)p, count);
}
inline BOOL zDisplay::logToDev(zRect &r) {
	return LPtoDP(hDC, (LPPOINT)&r, 2);
}

inline zCoOrd zDisplay::logToDevX(const zCoOrd x) const {
	zPoint p(x,0);
	LPtoDP(hDC, (LPPOINT)&p, 1);
	return p.x();
}

inline zCoOrd zDisplay::logToDevY(const zCoOrd y) const {
	zPoint p(0,y);
	LPtoDP(hDC, (LPPOINT)&p, 1);
	return p.y();
}


inline void zDisplay::setTextBackMode(zTextMode tm) {
	if (isValid()) SetBkMode(hDC,(unsigned int)tm);
	_textBackMode=tm;
}

inline zTextMode zDisplay::getTextBackMode() {
	return _textBackMode;
}

inline int zDisplay::setClip(zRegion *r)  {
	return SelectClipRgn(hDC,r ?
	   (ZREGION_HNDL) *r : 
		0);
}

inline int zDisplay::getClip(zRect &r) {
	return GetClipBox(hDC,(LPRECT)&r);
}



inline BOOL zDisplayInfo::isValid() const {
	return (hDC!=0);
}

inline BOOL& zPrintJob::banding() {
	return doBand;
}

inline void zPrintJob::setPrint(zEvH *obj, zPrintProc proc) {
	objPrint = obj;
	print = proc;
}

inline void zPrintJob::setSetup(zEvH *obj, zPrSetupProc proc) {
	objSetup = obj;
	setup = proc;
}

inline void zPrintJob::setMorePages(zEvH *obj, zPrMorePagesProc proc) {
	objMorePages = obj;
	mp = proc;
}

inline void zPrintJob::setCleanup(zEvH *obj, zPrCleanupProc proc) {
	objCleanup = obj;
	cleanupProc = proc;
}

inline int zImageList::numImages() const {
	return _numImages;
}

inline const zDimension& zImageList::imageSize() const {
	return bmpSize;
}
	


