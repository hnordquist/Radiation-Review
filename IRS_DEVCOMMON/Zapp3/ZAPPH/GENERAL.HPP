#ifndef _ZAPPGENERAL_INCLUDED
#ifndef _ZAPPZAPP_INCLUDED
#include "zapp.hpp"
#else
#define _ZAPPGENERAL_INCLUDED 1
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




class Z_DLLSPEC zPair {
protected:
	zCoOrd	_x;
	zCoOrd	_y;
public:
	zPair(const zPair&p) { _x=p._x; _y=p._y; }
	zPair() { _x=0; _y=0; }
	zPair(zCoOrd A,zCoOrd B ):_x(A),_y(B) {}
	zPair(unsigned long d):_x((zCoOrd)(d&0x0000ffff)), _y((zCoOrd)((d>>16)&0x0000ffff)){}
	zCoOrd &x() { return _x; }
	zCoOrd &y() { return _y; }
	zCoOrd x() const { return _x; }
	zCoOrd y() const { return _y; }
};

#ifdef __GNUC__
#ifndef Z_MIN
#define Z_MIN(a,b) ((a<b)?a:b)
#endif
#ifndef Z_MAX
#define Z_MAX(a,b) ((a>b)?a:b)
#endif
#else
#ifndef Z_MIN
#define Z_MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef Z_MAX
#define Z_MAX(a,b) (((a)>(b))?(a):(b))
#endif
#endif

class Z_DLLSPEC zRange:public zPair {
public:
	zRange() {}
	zRange(const zRange&r):zPair((const zPair&)r) {}
	zRange(zCoOrd a, zCoOrd b) :zPair(Z_MIN(a,b),Z_MAX(a,b)) {}
	zCoOrd& lo() { return _x; }
	zCoOrd& hi() { return _y; }
	zCoOrd lo() const { return _x; }
	zCoOrd hi() const { return _y; }
	zRange operator +(const zCoOrd a) {
	    return zRange(Z_MIN(_x, a), Z_MAX(_y,a));
	}
	zRange operator +(const zRange& a) const {
		return zRange(Z_MIN(_x, a._x), Z_MAX(_y, a._y));
	}
	zRange& operator +=(const zRange& a) {
		if (a._x < _x)
			_x = a._x;
		if (a._y > _y)
			_y = a._y;
		return *this;
	}
	int isNull() { return _x >= _y; }
	void makeNull() { _x = 0x7fff; _y = 0; }
	operator int() const { return _x>=_y? 0:_y-_x; }
	BOOL in(const zCoOrd i) const { return (i<=_y && i>=_x) ? TRUE:FALSE; }
};

class Z_DLLSPEC zPoint:public zPair {
public:
	zPoint() {}
	zPoint(zCoOrd u):zPair(u, u) {}
	zPoint(zCoOrd u,zCoOrd v):zPair(u,v) {}
	zPoint(const zPoint&p):zPair((zPair&)p) {}
	
	zCoOrd& x() { return _x; }
	zCoOrd& y() { return _y; }
	
	zCoOrd x() const { return _x; }
	zCoOrd y() const { return _y; }
	
	zPoint operator -() const { return zPoint(-_x,-_y); }
	zPoint operator +(const zPoint p) const { return zPoint(_x+p._x,_y+p._y);}
	inline zPoint operator +(const zDimension p) const;
	
	zPoint operator -(const zPoint p) const { return zPoint(_x-p._x,_y-p._y); }
	inline zPoint operator -(const zDimension p) const;
	
	zPoint operator *(const zCoOrd i) const { return zPoint(_x*i,_y*i);}
	friend zPoint operator *(const zCoOrd i,const zPoint p) { return zPoint(i*p._x, i* p._y); }
	zPoint operator /(zCoOrd i) const { return zPoint(_x/i,_y/i);}
	zPoint operator %(zCoOrd i) const { return zPoint(_x%i,_y%i);}
	zPoint operator +=(const zPoint p) { return zPoint(_x+=p._x,_y+=p._y);}	
	zPoint operator -=(const zPoint p) { return zPoint(_x-=p._x,_y-=p._y);}
	zPoint operator *=(const zCoOrd i) { return zPoint(_x *= i, _y *= i); }
	int operator ==(const zPoint p) const { return _x==p._x && _y==p._y;}
	int operator !=(const zPoint p) const { return _x!=p._x || _y!=p._y;}
	int operator >=(const zPoint p) const { return _x>=p._x && _y>=p._y;}
	int operator <=(const zPoint p) const { return _x<=p._x && _y<=p._y;}
	int operator >(const zPoint p) const { return _x>p._x && _y>p._y;}
	int operator <(const zPoint p) const { return _x<p._x && _y<p._y;}
	zPoint operator =(const zCoOrd c) { _x = _y = c; return zPoint(c); }
	zPoint& operator =(const zPoint& pt) { _x = pt.x(); _y = pt.y(); return *this; }
	zPoint Min(const zPoint p) const { return zPoint(Z_MIN(_x, p._x), Z_MIN(_y, p._y));}
	zPoint Max(const zPoint p) const { return zPoint(Z_MAX(_x, p._x), Z_MAX(_y, p._y));}
	void setNull() { _x = _y = 0; }
	operator POINT() { return *((POINT *)this); }	
	void translate(zWindow* from, zWindow* to);
};

class Z_DLLSPEC zDimension:public zPair {
public:
	zDimension() {}
	zDimension(zCoOrd w,zCoOrd h):zPair(w,h) {}
	zDimension(const zDimension& d):zPair(d._x,d._y) {}
	zDimension(unsigned long d):zPair(d) {}
	zCoOrd& width() { return _x; }
	zCoOrd& height() { return _y; }
	zCoOrd width() const { return _x; }
	zCoOrd height() const { return _y; }

	int operator ==(const zDimension d) const { return _x==d._x && _y==d._y;}
	int operator !=(const zDimension d) const { return _x!=d._x || _y!=d._y;}

	zDimension operator -() const { return zDimension(-_x,-_y); }
	zDimension operator +(const zDimension p) const { return zDimension(_x+p._x,_y+p._y);}
	zDimension operator -(const zDimension p) const { return zDimension(_x-p._x,_y-p._y); }
	zDimension operator *(const zCoOrd i) const { return zDimension(_x*i,_y*i);}
	friend zDimension operator *(const zCoOrd i,const zDimension p) { return zDimension(i*p._x, i* p._y); }
	zDimension operator /(zCoOrd i) const { return zDimension(_x/i,_y/i);}
	zDimension operator %(zCoOrd i) const { return zDimension(_x%i,_y%i);}
	zDimension operator +=(const zDimension p) { return zDimension(_x+=p._x,_y+=p._y);}	
	zDimension operator -=(const zDimension p) { return zDimension(_x-=p._x,_y-=p._y);}
	zDimension operator *=(const zCoOrd i) { return zDimension(_x *= i, _y *= i); }
	int operator >=(const zDimension p) const { return _x>=p._x && _y>=p._y;}
	int operator <=(const zDimension p) const { return _x<=p._x && _y<=p._y;}
	int operator >(const zDimension p) const { return _x>p._x && _y>p._y;}
	int operator <(const zDimension p) const { return _x<p._x && _y<p._y;}
	zDimension operator =(const zCoOrd c) { _x = _y = c; return zDimension(c); }
	zDimension Min(const zDimension p) const { return zDimension(Z_MIN(_x, p._x), Z_MIN(_y, p._y));}
	zDimension Max(const zDimension p) const { return zDimension(Z_MAX(_x, p._x), Z_MAX(_y, p._y));}

};

class Z_DLLSPEC zDisplay;

#ifdef Z_NP

class Z_DLLSPEC zLogMeasure:public zPair {

#else
	
class Z_DLLSPEC zLogMeasure:public zPair, public zStorable {

#endif
	declPersis(zLogMeasure)
protected:
	zPair log;
public:
	zLogMeasure();
	zLogMeasure(zCoOrd lx, zCoOrd ly);
	zLogMeasure(const zLogMeasure&r):zPair((const zPair&)r) { log=r.log; }
	~zLogMeasure();
	zPair logical() const { return log; }
	zPair physical() const { return zPair(_x,_y); }
	operator zPoint() const { return zPoint(_x,_y); }
	operator zDimension() const { return zDimension(_x,_y); }
	virtual zCoOrd setLogX(zCoOrd c, zDisplay *d=0);
	virtual zCoOrd setLogY(zCoOrd c, zDisplay *d=0);
	void newDisplay(zDisplay *);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zLoMetric:public zLogMeasure {
	declPersis(zLoMetric)
public:
    zLoMetric();
    zLoMetric(const zLoMetric&r):zLogMeasure((const zLogMeasure&)r) {}
    zLoMetric(zCoOrd i, zCoOrd j, zDisplay* d=0);
    ~zLoMetric();
    virtual zCoOrd setLogX(zCoOrd x, zDisplay* d=0);
    virtual zCoOrd setLogY(zCoOrd y, zDisplay* d=0);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zHiMetric:public zLogMeasure {
	declPersis(zHiMetric)
public:
	zHiMetric();
	zHiMetric(zCoOrd i, zCoOrd j, zDisplay* d=0);
	zHiMetric(const zHiMetric&r):zLogMeasure((const zLogMeasure&)r) {}
	~zHiMetric();
	virtual zCoOrd setLogX(zCoOrd x, zDisplay* d=0);
	virtual zCoOrd setLogY(zCoOrd y, zDisplay* d=0);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zLoEnglish:public zLogMeasure {
	declPersis(zLoEnglish)
public:
	zLoEnglish();
	zLoEnglish(zCoOrd i, zCoOrd j, zDisplay* d=0);
	zLoEnglish(const zLoEnglish&r):zLogMeasure((const zLogMeasure&)r) {}
	~zLoEnglish();
	virtual zCoOrd setLogX(zCoOrd x, zDisplay* d=0);
	virtual zCoOrd setLogY(zCoOrd y, zDisplay* d=0);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zHiEnglish:public zLogMeasure {
	declPersis(zHiEnglish)
public:
	zHiEnglish();
	zHiEnglish(zCoOrd i, zCoOrd j, zDisplay* d=0);
	zHiEnglish(const zHiEnglish&r):zLogMeasure((const zLogMeasure&)r) {}
	~zHiEnglish();
	virtual zCoOrd setLogX(zCoOrd x, zDisplay* d=0);
	virtual zCoOrd setLogY(zCoOrd y, zDisplay* d=0);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zPrPoint:public zLogMeasure {
	declPersis(zPrPoint)
public:
	zPrPoint();
	zPrPoint(zCoOrd i, zCoOrd j, zDisplay* d=0);
	zPrPoint(const zPrPoint&r):zLogMeasure((const zLogMeasure&)r) {}
	~zPrPoint();
	virtual zCoOrd setLogX(zCoOrd x, zDisplay* d=0);
	virtual zCoOrd setLogY(zCoOrd y, zDisplay* d=0);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zPica:public zLogMeasure {
	declPersis(zPica)
public:
	zPica();
	zPica(zCoOrd i, zCoOrd j, zDisplay* d=0);
	zPica(const zPica&r):zLogMeasure((const zLogMeasure&)r) {}
	~zPica();
	virtual zCoOrd setLogX(zCoOrd x, zDisplay* d=0);
	virtual zCoOrd setLogY(zCoOrd y, zDisplay* d=0);
#ifndef Z_NP
	virtual void io(zArchive &arc);
#endif
};

class Z_DLLSPEC zRect {
	zPoint tl;
	zPoint br;
public:
	zRect() {}
	zRect(zCoOrd lft,zCoOrd tp,zCoOrd rgt,zCoOrd btm):tl(lft, tp),br(rgt, btm) {}
	zRect(const zPoint& p,const zDimension& d):tl(p),br(p.x()+d.width(), p.y()+d.height()) {}
	zRect(const zPoint& p,const zPoint& p2) : tl(p), br(p2) {}
	zRect(const zRect&r):tl(r.tl),br(r.br) {}
		
	zPoint& topLeft()  { return tl; }
	zPoint& bottomRight() { return br; }
	zPoint topRight() { return zPoint(br.x(), tl.y()); }
	zPoint bottomLeft() { return zPoint(tl.x(), br.y()); }
	zPoint topRight() const { return zPoint(br.x(), tl.y()); }
	zPoint bottomLeft() const { return zPoint(tl.x(), br.y()); }
	
	zCoOrd& left()  { return tl.x();}
	zCoOrd& right() { return br.x();}
	zCoOrd& top()   { return tl.y();}
	zCoOrd& bottom(){ return br.y();}
	zPoint center() const { return (tl+br)/2;}

	zPoint topLeft() const { return tl; }
	zPoint bottomRight() const { return br; }
	zCoOrd left() const { return tl.x(); }
	zCoOrd right() const { return br.x(); }
	zCoOrd top() const { return tl.y(); }
	zCoOrd bottom() const { return br.y(); }
	
	zRect& operator+=(const zPoint &p) { tl +=p; br +=p; return *this; }
	zRect& operator-=(const zPoint &p) { tl -= p; br -= p; return *this; }
	zRect operator+(const zPoint &p) { return zRect(tl+p, br+p); }
	zRect operator-(const zPoint &p) { return zRect(tl-p, br-p); }
	int operator==(const zRect &r) const { return (tl==r.tl && br==r.br); }
	int operator!=(const zRect &r) const { return (tl!=r.tl || br!=r.br); }
	zDimension size() const { return zDimension(br.x()-tl.x(),br.y()-tl.y()); }
	int width() const { return br.x() - tl.x(); }
	int height() const { return br.y() - tl.y(); }
	BOOL isNull() const { return tl.x()==tl.y()==br.x()==br.y()?1:0; }
	zRect& makeNull() { tl = 0; br=0; return *this; }
	zRect rectIntersect(const zRect&) const;
	zRect rectUnion(const zRect&) const;
	zRect& operator=(const RECT& rr) {
		tl.x() =rr.left; br.x() =rr.right; tl.y() =rr.top; br.y()=rr.bottom;
		return *this;
	}

	BOOL contains(const zPoint p) const { return (p >= tl) && (p <= br); }
	BOOL contains(const zRect& r) const { return (contains(r.tl) && contains(r.br)); }

#if defined(Z_OS2) || defined(__ZTC__)
	BOOL intersects(const zRect& r) const;
#else
	BOOL intersects(const zRect& r) const { return (tl.Max(r.tl)<br.Min(r.br)) ? 1:0; }
#endif
};

class Z_DLLSPEC zPrinterSpec {
	char buf[256];
	char* drive;
	char* dev;
	char* out;
public:
	zPrinterSpec(char *s=0);
	zPrinterSpec(const zPrinterSpec &ps);
	char* driverName() { return drive; }
	char* deviceName() { return dev; }
	char* output() { return out; }
#ifndef HP
	int operator==(const zPrinterSpec& ps) {
		return (drive==ps.drive && dev==ps.dev && out==ps.out);
	}
#endif
	int operator<(const zPrinterSpec&) const {
		return 0;
	}
};

inline int Z_DLLSPEC operator==(const zPrinterSpec&,const zPrinterSpec&) {
	return 0;
}

declSlist(zPrinterSpec)


class Z_DLLSPEC zSystemInfo {
	int horPix;
	int verPix;
	int horPixInch;
	int verPixInch;
	int logHeight;
	int logWidth;
public:
	zSystemInfo();
	int pixHeight() const { return verPix; }
	int pixWidth() const { return horPix; }
	int pixPerInchX() const { return horPixInch; }
	int pixPerInchY() const { return verPixInch; }
	int mmHeight() const { return logHeight; }
	int mmWidth() const { return logWidth; }
    RWTPtrSlist<zPrinterSpec>* getPrinterList();
    void releasePrinterList(RWTPtrSlist<zPrinterSpec> *p);
};

inline zPoint zPoint::operator +(const zDimension p) const {
	return zPoint(_x+p.width(),_y+p.height());
}

inline zPoint zPoint::operator -(const zDimension p) const {
	return zPoint(_x-p.width(),_y-p.height());
}


class Z_DLLSPEC zUserResource {
#ifdef STRICT
	HGLOBAL hRes;
#else
	HANDLE hRes;
#endif
	HANDLE hModule;
	void *pRes;
public:
	zUserResource(const zResId&);
	zUserResource(const zResId&,const RWCString&);
	operator void*() const { return pRes; }
	~zUserResource();
};

#endif

#endif


