#ifndef _ZAPPDISPLAY_INCLUDED
#define _ZAPPDISPLAY_INCLUDED 1
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


class Z_DLLSPEC zColor {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE mode;
public:
	inline zColor();
	inline zColor(BYTE rd,BYTE gr,BYTE bl, int palRel=0);
	inline zColor(unsigned long c);
	inline zColor(const zColor&c);
	inline zColor& operator=(unsigned long c);
	inline zColor& operator=(const zColor& c);
	inline BYTE& red();
	inline BYTE& green();
	inline BYTE& blue();
	inline operator DWORD() const;
	inline DWORD id() const;
	BYTE red() const { return r; }
	BYTE green() const { return g; }
	BYTE blue() const { return b; }
	BOOL operator==(const zColor& c2) { return (r==c2.r) && (g==c2.g)
		&& (b==c2.b) && (mode==c2.mode); }
};


#ifdef Z_NP

class Z_DLLSPEC zDrawObj {

#else

class Z_DLLSPEC zDrawObj:public zStorable {
	declPersis(zDrawObj)

#endif
protected:
	HANDLE id;
	unsigned int flags;
	zDrawObj() { id = 0; flags = 0; }
	zDrawObj(zDrawObj&d):id(d.id),flags(d.flags) {}
public:
	operator HANDLE() { return id; }
			~zDrawObj();
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};


class Z_DLLSPEC zPen : public zDrawObj {
	declPersis(zPen)
	PenStyle pStyle;
	int pWidth;
	zColor pColor;
	StockPen pStock;
	zPen();
public:
	zPen(zColor, PenStyle=Solid, int width=0);
	zPen(StockPen);
	zPen(zPen&p);
	~zPen() {}
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
#ifdef STRICT
	operator HPEN() const { return (HPEN)id; }
#endif
};
declStack(zPen);

class Z_DLLSPEC zBrush : public zDrawObj {
	declPersis(zBrush)
	BrushHatch bHatch;
	StockBrush bStock;
	zColor bColor;
	zBrush();
public:
	zBrush(StockBrush);
	zBrush(const zColor&);		// Solid zColor brush.
	zBrush(const zColor&, BrushHatch);
	zBrush(const zBrush&b);
#ifndef __NOBITMAP__
	zBrush(zBitmap *);
#endif
	~zBrush() {}
#ifdef STRICT
	operator HBRUSH() const { return (HBRUSH)id; }
#endif

#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};
declStack(zBrush);

class Z_DLLSPEC zDisplay;
declStack(zDisplay);

#ifndef __NOBITMAP__
class Z_DLLSPEC zBitmap : public zDrawObj {
	friend class Z_DLLSPEC zBitmapDisplay;
	unsigned int numColors(void *);
	unsigned int palSize(void *);
public:
	zBitmap();
	void setBitmap(HBITMAP h) { id=h; }
	zBitmap(int width,int height,int depth,int planes);	// Full Specification.
	zBitmap(int width, int height, zDisplay*);			// Make it compatible.
	zBitmap(StockBitmap);
	zBitmap(const zResId&);
	zBitmap(zDisplay *,const char *);
	zDimension size();
#ifdef STRICT
	operator HBITMAP() const { return (HBITMAP)id; }
#endif
	
};
#endif

class Z_DLLSPEC zFont;



#ifdef Z_NP

class Z_DLLSPEC zFontInfo {

#else

class Z_DLLSPEC zFontInfo:public zStorable {
	declPersis(zFontInfo)
#endif

	
	// non-OS/2
	TEXTMETRIC tm;
protected:
	zFontInfo(TEXTMETRIC *tp) { tm = *tp; }
public:
	zFontInfo() {}
	zFontInfo(zDisplay *);
	int 	height() { return tm.tmHeight; }
	int 	ascent() { return tm.tmAscent; }
	int 	descent() { return tm.tmDescent; }
	int 	internLead() { return tm.tmInternalLeading; }
	int 	externLead() { return tm.tmExternalLeading; }
	int 	width() { return tm.tmAveCharWidth; }
	int 	maxWidth() { return tm.tmMaxCharWidth; }
 	int 	weight() { return tm.tmWeight; }
	BOOL	isItalic() { return tm.tmItalic; }
	BOOL	isUnderlined() { return tm.tmUnderlined; }
	BOOL	isStruckOut() { return tm.tmStruckOut; }
	BYTE	firstChar() { return tm.tmFirstChar; }
	BYTE	lastChar() { return tm.tmLastChar; }
	BYTE	defaultChar() { return tm.tmDefaultChar; }
	BYTE	breakChar() { return tm.tmBreakChar; }
	zFntPitch pitch() {
		return tm.tmPitchAndFamily&1?VariablePitch:FixedPitch;
	}
	zFntFam family() { return zFntFam(tm.tmPitchAndFamily&0xf0); }
	zChSet	charSet() { return (zChSet)tm.tmCharSet; }
	int 	overhang() { return tm.tmOverhang; }
	int 	aspectX() { return tm.tmDigitizedAspectX; }
	int 	aspectY() { return tm.tmDigitizedAspectY; }
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};

#ifdef Z_NP
class Z_DLLSPEC zFontSpec {
	
#else
	
class Z_DLLSPEC zFontSpec : public zStorable {
	declPersis(zFontSpec)
#endif
	friend class Z_DLLSPEC zFont;
	LOGFONT lf;
	short _pointSize;
public:
	zFontSpec(const char *faceName, zDimension d, int weight,
		zFntFam = ffDontCare, zFntPitch=VariablePitch, int style=0,
		int escapement=0, int orientation=0,zChSet=AnsiCharSet,
		zOutPrec=DefaultOutPrec,zClipPrec=DefaultClipPrec,
		zOutQual=DefaultQual);
	zFontSpec() { memset(&lf, 0, sizeof(LOGFONT)); _pointSize = 0; }
	zFontSpec(LOGFONT *fs) { lf = *fs; _pointSize = 0; }
	zFontSpec(const zFontSpec& fs) { lf=fs.lf; _pointSize = fs._pointSize; }
	operator LPLOGFONT() { return &lf; }
	char* faceName();
	short &pointSize() { return _pointSize; }
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};
		
		

extern "C" {
	int FAR PASCAL fontGrabber(LPLOGFONT, LPTEXTMETRIC, int, LPSTR);
}

class Z_DLLSPEC zFontDescript;

int operator==(const zFontDescript&,const zFontDescript&);


class Z_DLLSPEC zFontDescript : public zFontInfo {
	declPersis(zFontDescript)
	zFontSpec s;
	zFontDescript() : s(0) {}
	zFontDescript(TEXTMETRIC *t, LOGFONT *f):zFontInfo(t), s(f) {}
	friend int cppfontGrabber(LPLOGFONT, LPTEXTMETRIC, int, LPSTR);
public:
	operator zFontSpec() { return s; }
	char* faceName();	
	int operator==(const zFontDescript& fd) {
		return (s==((zFontDescript&)fd).s);
	}
	friend int operator==(const zFontDescript&,const zFontDescript&);
#ifndef RW_NO_STL	
	int operator<(const zFontDescript&) const {
		return 0;
	}
#endif
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};
declSlist(zFontDescript);

inline int operator==(const zFontDescript&,const zFontDescript&) {
	return 0;
}

class Z_DLLSPEC zFont : public zDrawObj {
	
	declPersis(zFont)
	zFontSpec spec, actualSpec;
	StockFont fStock;
	zFont() : spec(0) {}
public:
	zFont(const char *faceName, zDimension d, int weight,
		zFntFam = ffDontCare, zFntPitch=VariablePitch, int style=0,
		int escapement=0, int orientation=0,
		zChSet=AnsiCharSet,zOutPrec=DefaultOutPrec,
		zClipPrec=DefaultClipPrec, zOutQual=DefaultQual);
	zFont(const char *faceName, zPrPoint d, int weight,
		zFntFam = ffDontCare, zFntPitch=VariablePitch, int style=0,
		int escapement=0, int orientation=0,
		zChSet=AnsiCharSet,zOutPrec=DefaultOutPrec,
		zClipPrec=DefaultClipPrec, zOutQual=DefaultQual);
	zFont(zFontSpec&, zDisplay * = 0);
	zFont(StockFont fontNum) :fStock(fontNum) {
		id = GetStockObject(fontNum);
		flags = STOCKOBJ;
	}
	zFontSpec *fontSpec() { return &spec; }
	// ZIP HFONT only constructor
	zFont(HFONT hf);
#ifdef STRICT
	operator HFONT() const { return (HFONT)id; }
#endif

#ifndef Z_NP
	virtual void io(zArchive&);
#endif

};
declStack(zFont);

class Z_DLLSPEC zIcon {

	HICON hi;
public:
	int iconResBased;
	zResId _res;
	StockIcon _stock;	
	operator HICON() { return hi; }
	zIcon(const char *);
	zIcon(const zResId&);
	zIcon(StockIcon);
	zIcon(const zIcon&i);
	~zIcon();
};

 #ifdef STRICT
  #define ZREGION_HNDL HRGN
 #else
  #define ZREGION_HNDL HANDLE
 #endif

#ifdef Z_NP

class Z_DLLSPEC zRegion {
#else
class Z_DLLSPEC zRegion:public zStorable {
	declPersis(zRegion)
#endif
#ifndef Z_OS2_PLATFORM_SPECIFIC
	void operator=(zRegion &) {}
#endif
protected:
	HRGN hReg;
	zRegion(HANDLE h);
public:
	zRegion();
	~zRegion();
	
	operator HANDLE() { return hReg; }
#ifdef STRICT
	operator HRGN() const { return (HRGN)hReg; }
#endif
	void operator+=(zRegion &);
	void operator+=(zPoint p);
	void operator-=(zRegion &);
	void operator-=(zPoint p);
	void operator&=(zRegion &);
	void operator|=(zRegion &);
	void operator^=(zRegion &);
	BOOL operator==(zRegion &);
	BOOL operator!=(zRegion &);
	BOOL contains(zPoint p);
	BOOL contains(zRect *);
	void toRect(zRect &);
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};

class Z_DLLSPEC zRectRegion : public zRegion {
	declPersis(zRectRegion)
	zRect rct;
public:
	zRectRegion(int left, int top, int right, int bottom);
	zRectRegion(zRect *);
	zRectRegion() {}
	~zRectRegion() {}
	zRect &rect() { return rct; }
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};


class Z_DLLSPEC zEllipticRegion : public zRegion {
	declPersis(zEllipticRegion)
	zRect rct;
public:
	zEllipticRegion(int left, int top, int right, int bottom);
	zEllipticRegion(zRect *);
	zEllipticRegion() {}
	~zEllipticRegion() {}
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};

class Z_DLLSPEC zPolygonRegion : public zRegion {
	declPersis(zPolygonRegion)
	zPoint *pt;
	int npt;
	PolyFill pfMode;
public:
	zPolygonRegion(zPoint *p, int count, PolyFill mode);
	zPolygonRegion() {}
	~zPolygonRegion() {}
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};
	
class Z_DLLSPEC zRoundRectRegion : public zRegion {
	declPersis(zRoundRectRegion)
	zRect rct;
	zDimension corner;
public:
	zRoundRectRegion(int left, int top, int right, int bottom,
		int cHeight, int cWidth);
	zRoundRectRegion(zRect *, zDimension cornerSize);
	zRoundRectRegion() {}
	~zRoundRectRegion() {}
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};

typedef void (zEvH::*LINEPROC)(int , int );


class Z_DLLSPEC zDisplay {
	int	lockCount;
	zRect invalid;
	zColor background;
	zColor foreground;
	zBrush* curBrush;
	zBrushStack brushes;
	zPen* curPen;
	zPenStack pens;
	zFont* curFont;
	zFontStack fonts;
	BinDrawOp drMode;
	zPoint winOrg;
	zPoint viewOrg;
	zDimension winExt;
	zDimension viewExt;
	MapMode	mm;

protected:
	DispType dispType;
	HDC	hDC;
	unsigned _pixWidth;
	unsigned _pixHeight;
	unsigned _pixPerInchX;
	unsigned _pixPerInchY;
	unsigned _mmWidth;
	unsigned _mmHeight;
	zTextMode _textBackMode;
	zRect _pictureArea;
	zDisplay();

	virtual void get();
	virtual void release();
public:
	void flush();
	void lock();
	void unlock();

	inline BOOL isLocked() const;
	inline BOOL isValid() const;
	inline DispType type() const;
	virtual void getDirty(zRect& r);
	virtual void setDirty(zRect* r=0,BOOL eraseBackground =1);
	virtual void setDirty(zRegion& r,BOOL eraseBackground =1);

	operator HDC() const { return hDC; }
	virtual ~zDisplay();

	inline int pixWidth() const;
	inline int pixHeight() const;
	inline int pixPerInchX() const;
	inline int pixPerInchY() const;
	inline int mmWidth() const;
	inline int mmHeight() const;

	BOOL devToLog(zPoint &p);
	BOOL devToLog(zPoint *p, int count);
	BOOL devToLog(zRect &r);
	BOOL logToDev(zPoint &p);
	BOOL logToDev(zPoint *p, int count);
	BOOL logToDev(zRect &r);
	zCoOrd logToDevX(const zCoOrd x) const;
	zCoOrd logToDevY(const zCoOrd y) const;

	zColor backColor() const;
	zColor backColor(const zColor);

	zColor textColor() const;
	zColor textColor(const zColor);
	virtual zDimension getTextDim(const char *s, const int c=0);
	zBrush* brush() const;
	zBrush* brush(zBrush* );
	void pushBrush(zBrush *);
	zBrush* popBrush();
	
	inline zPen* pen() const;
	zPen* pen(zPen* );
	void pushPen(zPen *);
	zPen* popPen();
	
	inline zFont* font() const;
	zFont* font(zFont *f);
	void pushFont(zFont *f);		
	zFont* popFont();
	inline BinDrawOp drawMode() const;
	BinDrawOp drawMode(BinDrawOp);

	inline
	void setTextBackMode(zTextMode);
	inline zTextMode getTextBackMode();

	int	polyFillMode(int mode) {
		return SetPolyFillMode(hDC, mode);
	}
	int	polyFillMode() {
		return GetPolyFillMode(hDC);
	}
	MapMode mapMode(MapMode m);
	MapMode mapMode() { return mm? mm : MapPixels; }
	//
	// The Viewport is in device (physical) units.
	//
	unsigned long setViewportOrg(zCoOrd x, zCoOrd y);
	unsigned long setViewportExt(zCoOrd dx, zCoOrd dy);
	zPoint* getViewportOrg() { return &viewOrg; }

	zDimension* getViewportExt() { return &viewExt; }
	zDimension* getWindowExt() { return &winExt; }
	//
	// The Window is in logical units.
	//
	unsigned long setWindowOrg(zCoOrd x, zCoOrd y);
	unsigned long setWindowExt(zCoOrd dx, zCoOrd dy);
	inline
			int setClip(zRegion *r);
	inline int getClip(zRect &r);
	zPoint *getWindowOrg() { return &winOrg;}
	
	void setPictureArea(zRect *r) { _pictureArea = *r; }
	void getPictureArea(zRect &r) { r = _pictureArea; }
	
	virtual void getVisible(zRect &);
	zColor pixel(int x, int y, zColor c) {
		long ret = SetPixel(hDC, x, y, (DWORD)c);
		if (ret < 0) return background;
		else return zColor(ret);
	}
	zColor pixel(zPoint p, zColor c) {
		long ret = SetPixel(hDC, p.x(), p.y(), (DWORD)c);
		if (ret < 0) return background;
		else return zColor(ret);
	}
	zColor pixel(int x, int y) {
		long ret = GetPixel(hDC, x, y);
		if (ret < 0) return background;
		else return zColor(ret);
	}
	zColor pixel(zPoint p) {
		long ret = GetPixel(hDC, p.x(), p.y());
		if (ret < 0) return background;
		else return zColor(ret);
	}
	
	inline unsigned long moveTo(zCoOrd x, zCoOrd y);
	inline unsigned long moveTo(zPoint p);
		
	BOOL lineTo(zCoOrd x, zCoOrd y) { return LineTo(hDC, x, y); }
	BOOL lineTo(const zPoint p) { return LineTo(hDC, p.x(), p.y()); }
	
	BOOL polyline(zPoint *p, int count) {
		return Polyline(hDC, (LPPOINT)p, count);
	}
	BOOL polyline(const zRect& r);
	
	BOOL lineDDA(zPoint p1, zPoint p2, void *, LINEPROC);
	BOOL rectangle(zCoOrd x1,zCoOrd y1,zCoOrd x2,zCoOrd y2) {
		return Rectangle(hDC,x1,y1,x2,y2);
	}
	BOOL rectangle(zPoint tl, zPoint br) {
		return Rectangle(hDC,tl.x(),tl.y(),br.x(),br.y());
	}
	BOOL rectangle(zRect r) {
		return Rectangle(hDC,r.left(),r.top(),r.right(),r.bottom());
	}
	BOOL roundRect(zCoOrd x1,zCoOrd y1,zCoOrd x2,zCoOrd y2,
		zCoOrd elWidth, zCoOrd elHeight) {
		return RoundRect(hDC,x1,y1,x2,y2, elWidth, elHeight);
	}
	BOOL roundRect(zPoint tl, zPoint br, zDimension cornerDim){
		return RoundRect(hDC,tl.x(),tl.y(),br.x(),br.y(),
			cornerDim.width(), cornerDim.height()); }
	BOOL roundRect(zRect r, zDimension cornerDim) {
		return RoundRect(hDC,r.left(),r.top(),
			r.right(),r.bottom(),cornerDim.width(),
			cornerDim.height());
	}
	BOOL ellipse(zCoOrd x1,zCoOrd y1,zCoOrd x2,zCoOrd y2) {
		return Ellipse(hDC,x1,y1,x2,y2);
	}
	BOOL ellipse(zPoint pt1,zPoint pt2) {
		return Ellipse(hDC,pt1.x(),pt1.y(),pt2.x(),pt2.y());
	}
	BOOL ellipse(zRect r) {
		return Ellipse(hDC,r.left(),r.top(),
			r.right(),r.bottom());
	}
	
	BOOL circle(zPoint pt,int radius) {
		return Ellipse(hDC,pt.x()-radius,
			pt.y()-radius,pt.x()+radius,pt.y()+radius);
	}
	BOOL icon(zPoint pt,zIcon *ico) {
		return DrawIcon(hDC,pt.x(),pt.y(),*ico);
	}
	BOOL text(int x, int y,const char *s, int len) {
		return TextOut(hDC, x,y,(char*)s, len);
	}
	
	BOOL text(int x, int y,const char *s) {
		return TextOut(hDC, x,y, (char*)s, strlen(s));
	}
	BOOL text(const zPoint& p,const char *s, int len) {
		return TextOut(hDC, p.x(),p.y(), (char*)s, len);
	}
	BOOL text(const zPoint& p,const char *s) {
		return TextOut(hDC, p.x(), p.y(), (char*)s, strlen(s));
	}
	BOOL text(const zRect& r,const char *s,int len=0);

	BOOL patBlt(int x, int y, int width, int height, DWORD rop) {
		return PatBlt(hDC, x,y,width, height, rop);
	}
	BOOL patBlt(zPoint p, zDimension d, DWORD rop) {
		return PatBlt(hDC,p.x() ,p.y(),d.width(),
			d.height(), rop);
	}
	BOOL fill(zRect r, zBrush *br=0) {	
		return FillRect(hDC,(RECT *)&r,
			(br)? (HBRUSH)*br: (HBRUSH)*curBrush);
	}
	BOOL fill(zRegion *rgn, zBrush *br=0) {	
		return FillRgn(hDC,*rgn,
			br? (HBRUSH)*br:(HBRUSH)*curBrush);
	}
	BOOL frame(zRegion *rgn, zBrush *br=0) {	
		return FrameRgn(hDC,*rgn,
			br? (HBRUSH)*br:(HBRUSH)*curBrush, 1, 1);
	}
	BOOL frame(zRect r, zBrush *br=0) {	
		return FrameRect(hDC,(RECT *)&r,
			br? (HBRUSH)*br:(HBRUSH)*curBrush);
	}
#ifndef __NOBITMAP__
	virtual BOOL bitmap(zBitmap *bmp,zPoint p,DWORD rop=SRCCOPY);
	BOOL bitmap(zBitmap *bmp,zPoint p,zDimension size,
		DWORD rop=SRCCOPY,int mode=COLORONCOLOR);
#endif
	BOOL arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
		return Arc(hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	BOOL arc(zPoint p1, zPoint p2, zPoint p3, zPoint p4)
	{return Arc(hDC,p1.x(),p1.y(),p2.x(),p2.y(),p3.x(),p3.y(),p4.x(),p4.y());}
	BOOL arc(zRect r, zPoint p1, zPoint p2){
		return Arc(hDC,r.left(),r.top(),r.right(),r.bottom(),p1.x(),p1.y(),p2.x(),p2.y());
	}
	BOOL chord(zRect r, zPoint p1, zPoint p2) {
		return Chord(hDC, r.left(),r.top(),r.right(),r.bottom(),
			p1.x(),p1.y(),p2.x(),p2.y());
	}
	BOOL polygon(zPoint *p, int count) {
		return Polygon(hDC, (LPPOINT)p, count);
	}
	BOOL pie(zRect r, zPoint p1, zPoint p2) {
		return Pie(hDC,r.left(),r.top(),r.right(),r.bottom(),
			p1.x(),p1.y(),p2.x(),p2.y());
	}
};

class Z_DLLSPEC zReadableDisplay:public zDisplay {
protected:
	zReadableDisplay() {}
public:
	virtual void copyTo(zDisplay *,int xDest,int yDest,int w,int h,int xSrc,int ySrc);
	~zReadableDisplay() {}
};

extern int doPaneDraw(zPane *, zEvent*);
class Z_DLLSPEC zWinDisplay;
extern void Z_DLLSPEC zAppDoSetDraw(zWinDisplay*,int);

class Z_DLLSPEC zWinDisplay: public zReadableDisplay {
	friend int Z_DLLSPEC doPaneDraw(zPane*, zEvent *);
	friend void Z_DLLSPEC zAppDoSetDraw(zWinDisplay*,int);
	zWindow* dad;
	BOOL inDrawEvt;
	PAINTSTRUCT ps;
protected:
	friend class Z_DLLSPEC zPane;
	void ziSetWindow(zWindow*);
	virtual void get();
	virtual void release();
public:
	zWinDisplay(zWindow *);
	virtual void getDirty(zRect&);
	virtual void setDirty(zRect* =0,BOOL eraseBackground =1);
	virtual void setDirty(zRegion&,BOOL eraseBackground =1);
	virtual void getVisible(zRect &);
	virtual ~zWinDisplay();
};

class Z_DLLSPEC ziInfoDisplay:public zReadableDisplay {
protected:
	virtual void get();
	virtual void release();
public:
	ziInfoDisplay();
	~ziInfoDisplay();
};

#ifndef __NOBITMAP__
class Z_DLLSPEC zBitmapDisplay : public zReadableDisplay {
	zBitmap* curBitmap;
	HANDLE hNullBitmap;
protected:
	void ziSetup(zDisplay *, zBitmap *);

public:
	zBitmapDisplay(zBitmap *);
	zBitmapDisplay(zDisplay *, int, int);
	zBitmapDisplay(zDisplay *, zBitmap *);
	virtual ~zBitmapDisplay();
	
	zBitmap *getBitmap() { return curBitmap; }
	zBitmap *setBitmap(zBitmap *);
	virtual void getVisible(zRect & r);
	virtual void getDirty(zRect& r);
	void copyTo(zDisplay *,int xDest,int yDest,int w, int h,int xSrc, int ySrc);
};
#endif

class Z_DLLSPEC zPrintJob;


class Z_DLLSPEC zPrinterDisplay : public zDisplay {
	friend class Z_DLLSPEC zPrintJob;
	RWCString _driverName;
	RWCString _deviceName;
	RWCString _portName;
	setAbortProc(FARPROC f);
protected:
	int _startPage;
	int _endPage;
	int _currentPage;
	int _numCopies;
	zRect dispArea;
	int collate;
public:
	zPrinterDisplay(zPrinterSpec *ps=0);
	virtual ~zPrinterDisplay();
	BOOL printerSetup();
	virtual void getDirty(zRect&);
	virtual void getVisible(zRect & r);
	void adjustVisible(const zRect &r, int adjustViewOrg=0);
	void resetVisible();
	int getCurrentPage() { return _currentPage; }	// zApp 3.0
	void setCurrentPage(int i) { _currentPage = i; }
	int getStartPage() { return _startPage; }	
	void setStartPage(int i) { _startPage = i; }
	void setNumCopies(int i) { _numCopies = i; }
	int getNumCopies() { return _numCopies; }
	int getEndPage() { return _endPage; }
	void setEndPage(int i) { _endPage = i; }
	void adjustPrintRange(int first, int last) {
		if (first > _startPage)
			_startPage = first;
		if (last < _endPage)
			_endPage = last;
	}
	int ziStartJob(const char *n);
	int ziEndPage();
	int ziEndJob();
	int abortJob() { return Escape(hDC, ABORTDOC, 0,0,0); }
	int ziNextBand(zRect &r) { return Escape(hDC, NEXTBAND, 0,0,(LPSTR)&r); }
	int ziStartPage();
	RWCString *driverName() { return &_driverName; }
	RWCString *deviceName() { return &_deviceName; }
	RWCString *portName() { return &_portName; }
#ifndef __NOBITMAP__
	virtual BOOL bitmap(zBitmap *bmp,zPoint p,DWORD rop=SRCCOPY);
#endif
	void configure();
};


class Z_DLLSPEC zMetafileDisplay:public zReadableDisplay {
	zDisplay *compatDisp;
	HMETAFILE metafile;
protected:
	virtual void get();
	virtual void release();
public:
	zMetafileDisplay(zDisplay*);
	virtual void copyTo(zDisplay *,int xDest,int yDest,int w,int h,int xSrc,int ySrc);
	virtual void getVisible(zRect &);
	virtual void getDirty(zRect &);
	virtual zDimension getTextDim(const char *s, const int c=0);
	~zMetafileDisplay();
	operator HMETAFILE() const { return metafile; }
	void setMetafile(HMETAFILE m) { metafile = m; }
	void getMetafile(METAFILEPICT &pict);
};



class Z_DLLSPEC zDisplayInfo {
	int lockCount;
	zDisplay* dp;
	HDC hDC;
	unsigned _drvVer;
	unsigned _pixWidth;
	unsigned _pixHeight;
	unsigned _pixPerInchX;
	unsigned _pixPerInchY;
	unsigned _mmWidth;
	unsigned _mmHeight;
	unsigned driveVer;
	unsigned rasterCaps;
	unsigned curveCaps;
	unsigned lineCaps;
	unsigned polygonCaps;
	unsigned textCaps;
	BOOL haveIC;
public:
	zDisplayInfo(zDisplay *);
	~zDisplayInfo();

	void lock();
	void unlock();
	inline BOOL isValid() const;

	//	Font List.
	//
    RWTPtrSlist<zFontDescript>* getFontList(char *faceName=0);
    RWTPtrSlist<zFontDescript>* getAllFontsList();
    void releaseFontList(RWTPtrSlist<zFontDescript> *p);

	//
	// Information Requests.
	//
	int driverVersion() { return _drvVer; }
	DisplayTech technology();
	unsigned pixWidth() { return _pixWidth; }
	unsigned pixHeight() { return _pixHeight; }
	unsigned pixPerInchX() { return _pixPerInchX; }
	unsigned pixPerInchY() { return _pixPerInchY; }
	unsigned mmWidth()	{ return _mmWidth; }
	unsigned mmHeight() { return _mmHeight; }
	int pixDepth();
	int colorPlanes();
	int numBrushes();
	int numPens();
	int numFonts();
	int numColors();
	int aspectX();
	int aspectY();
	int aspectXY();
	int sizePal();
	int sizePalReserved();
	int colorRes();
	BOOL doClip();
	//
	//	Raster Caps.
	//
	BOOL doBanding() { return rasterCaps&RC_BANDING; }
	BOOL doBitBlt() { return rasterCaps&RC_BITBLT; }
	BOOL doBitmap64() { return rasterCaps&RC_BITMAP64; }
	BOOL doDIB() { return rasterCaps&RC_DI_BITMAP; }
	BOOL doDIBtoDev() { return rasterCaps&RC_DIBTODEV; }
	BOOL doFloodFill() { return rasterCaps&RC_FLOODFILL; }
	BOOL doGDI2() { return rasterCaps&RC_GDI20_OUTPUT; }
	BOOL hasPal() { return rasterCaps&RC_PALETTE; }
	BOOL doScale() { return rasterCaps&RC_SCALING; }
	BOOL doStretch() { return rasterCaps&RC_STRETCHBLT; }
	BOOL doStretchDIB() { return rasterCaps&RC_STRETCHDIB; }
	BOOL doBigFonts() { return rasterCaps&RC_BIGFONT; }
	//
	//	Curve Caps
	//
	BOOL doCircle() { return curveCaps & CC_CIRCLES; }
	BOOL doPies() { return curveCaps & CC_PIE; }
	BOOL doChordArcs() {return curveCaps & CC_CHORD; }
	BOOL doEllipses() {return curveCaps & CC_ELLIPSES; }
	BOOL doWideCurves() { return curveCaps&CC_WIDE; }
	BOOL doStyledCurves() { return curveCaps & CC_STYLED; }
	BOOL doWideStyledCurves() {return curveCaps&CC_WIDESTYLED; }
	BOOL doCurveInterior() {return curveCaps&CC_INTERIORS; }
	//
	//	Line Caps
	//
	BOOL doPolyLines() { return lineCaps& LC_POLYLINE; }
	BOOL doMarkers() { return lineCaps & LC_MARKER; }
	BOOL doPolymarkers() { return lineCaps & LC_POLYMARKER; }
	BOOL doWideLines() {return lineCaps & LC_WIDE; }
	BOOL doStyledLines() {return lineCaps& LC_STYLED; }
	BOOL doWideStyledLines() {return lineCaps&LC_WIDESTYLED; }
	BOOL doLineInteriors() {return lineCaps&LC_INTERIORS; }
	//
	//	Polygonal Caps
	//
	BOOL doAltFillPoly() {return polygonCaps&PC_POLYGON; }
	BOOL doRectangle() {return polygonCaps & PC_RECTANGLE;  }
	BOOL doWindFillPoly() {return polygonCaps & PC_WINDPOLYGON; }
	BOOL doScanLinePoly() {return polygonCaps & PC_SCANLINE; }
	BOOL doWidePoly() {return polygonCaps & PC_WIDE; }
	BOOL doStyledPoly() {return polygonCaps& PC_STYLED; }
	BOOL doWideStyledPoly() { return polygonCaps& PC_WIDESTYLED; }
	BOOL doPolyInterior() { return polygonCaps& PC_INTERIORS; }
	//
	//	Text Caps
	//
	int doCharPrec() {return textCaps& TC_OP_CHARACTER; }
	int doStrokePrec() {return textCaps& TC_OP_STROKE; }
	int doStrokeClipPrec() {return textCaps&TC_CP_STROKE; }
	int doRotate90() {return textCaps &TC_CR_90 ; }
	int doRotateAny() {return textCaps& TC_CR_ANY; }
	int doScaleXY() {return textCaps& TC_SF_X_YINDEP; }
	int doScaleDouble() {return textCaps & TC_SA_DOUBLE; }
	int doScaleIntMult() {return textCaps&TC_SA_INTEGER; }
	int doScaleAny() { return textCaps & TC_SA_CONTIN; }
	int doDoubleWeight() { return textCaps & TC_EA_DOUBLE; }
	int doItalics() { return textCaps & TC_IA_ABLE; }
	int doUnderline() { return textCaps & TC_UA_ABLE; }
	int doStrikeout() { return textCaps & TC_SO_ABLE; }
	int doRasterFonts() { return textCaps & TC_RA_ABLE; }
	int doVectorFonts() { return textCaps & TC_VA_ABLE; }

	int doEscape(int escNum) {
		return (int)Escape(hDC, QUERYESCSUPPORT, sizeof(int), (LPSTR)&escNum,0);
	}
};


class Z_DLLSPEC zPrJobDlg;
typedef int (zEvH::*zPrSetupProc)(zPrinterDisplay *, zPrintJob *);
typedef int (zEvH::*zPrintProc)(zPrinterDisplay *, zRect *);
typedef BOOL (zEvH::*zPrMorePagesProc)();
typedef void (zEvH::*zPrCleanupProc)(zPrinterDisplay*,zPrintJob*);

class Z_DLLSPEC zPrintJob : public zEvH {
protected:
	zPrinterDisplay* printer;
	zPane *pane;
	zEvH* objPrint;
	zEvH* objSetup;
	zEvH* objMorePages;
	zEvH* objCleanup;
	RWCString name;
	zPrJobDlg* dlg;
	BOOL doBand;
	int bandNum;
	int pageNum;
	int numToPrint;
	zPrSetupProc setup;
	zPrintProc print;
	zPrMorePagesProc mp;
	zPrCleanupProc cleanupProc;
	void cleanup(int killDlg);
public:
	zPrintJob(zPane *, zPrinterDisplay *, zPrJobDlg *);
	zPrintJob(zPrinterDisplay *, zPrJobDlg *);
	virtual int poll();
	void go(int modalDlg =0);
	void setJobName(const char *n);
	void abort(BOOL killDialog=0);
	inline BOOL &banding();
	inline void setPrint(zEvH *obj, zPrintProc proc);
	inline void setSetup(zEvH *obj, zPrSetupProc proc);
	inline void setMorePages(zEvH *obj, zPrMorePagesProc proc);
	inline void setCleanup(zEvH *obj, zPrCleanupProc proc);
};



class Z_DLLSPEC zImageList {
	zBitmapDisplay *bds;
	zDimension bmpSize;
	int _numImages;
	int _maxImages;
public:
	zImageList(zDisplay*compatDisp,const zDimension& imageSize,int maxImages);
	~zImageList();

	zBitmap* addBitmap(zBitmap*,int& id_);
	int addBitmap(zReadableDisplay *rd,int row,int col);
	int addBitmap(const zResId&);

	zBitmap* replaceBitmap(zBitmap*,int id_);
	void replaceBitmap(const zResId&,int id_);
	
	int numImages() const;
	const zDimension& imageSize() const;

	void copyTo(zDisplay*,const zPoint&,int id_);
	void copyTo(zDisplay*,int x,int y,int id_);
};

#include <zapph\display.inl>

#endif

