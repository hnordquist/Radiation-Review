#ifndef _ZAPPSIZERS_INCLUDED
#define _ZAPPSIZERS_INCLUDED 1
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

enum zGrav {
	ZGRAV_LEFT,ZGRAV_RIGHT,ZGRAV_TOP,ZGRAV_BOTTOM,
	ZGRAV_CENTER,ZGRAV_MIDDLE,ZGRAV_NONE
};

class Z_DLLSPEC zReqGeom {
public:
	inline zPoint& at();
	inline void at(const zPoint&);
	inline zDimension& least();
	inline zDimension& want();
	inline zDimension& most();
	inline zGrav affinity() const;
	inline void affinity(const zGrav g);
	long& userData();
	void userPtr(void*);
	void* userPtr();
	zReqGeom operator =(zDimension);
	inline zReqGeom& operator=(zReqGeom& rg);
	void clear();
	void add(zReqGeom&);
private:
	zPoint _at;
	zDimension _least,_want,_most;
	zGrav _affinity;
	long _userData;
	void *_userPtr;
};
	

#define zSIZER             (1)
#define zSIZER_LEAF        (2)
#define zSIZER_FILLER      (3)
#define zSIZER_FIXED       (4)
#define zSIZER_GRAV        (5)
#define zSIZER_PERCENT     (6)
#define zSIZER_GROW        (7)
#define zSIZER_LAYOUT      (8)
#define zSIZER_WITH_PARENT (9)
#define zSIZER_EXPAND      (10)
#define ZSIZER_USERBASE	   (1000)
	
class Z_DLLSPEC zLayout;
class Z_DLLSPEC zSizer;

declDlist(zSizer)

class Z_DLLSPEC zSizer {
public:
	zSizer(zSizer* =0);
	zSizer(const zPoint&,const zDimension&,zSizer* =0);
	zSizer(zCoOrd,zCoOrd,zCoOrd,zCoOrd,zSizer* =0);
	zSizer(const zRect&,zSizer* =0);
	virtual ~zSizer();

	void pos(const zPoint &p);
	void dimension(const zDimension &d);

	inline operator zRect();
	inline operator zDimension() const;
	inline operator zPoint() const;
	inline zRect rect();
	inline void rect(const zRect&r);
	inline int width();
	inline int height();
	inline zPoint pos() const;
	inline zDimension dimension() const;

	virtual void resize(zWindow*,zSizeEvt *);
	virtual void update(zRect* =0);
	virtual void reqArea(zReqGeom&);
	virtual void setArea(zRect&,int =0);
	
	void setOwned(zWindow*);
	void setOwnerType(int);

	inline zSizer* parent();
	inline void parent(zSizer*);
	inline zGrav affinity() const;
	inline void affinity(const zGrav);

	void insert(zSizer*);
	void append(zSizer*);
	void replace(zSizer*s);
		// remove this zSizer, replace it with s. (tree otherwise is same).
	void remove(zSizer*s);
		// remove s from kidslist;
	void remove();
		// remove this zSizer from parent.

	
	inline zSizerDlist* kids();
	inline int numChildren() const;
	zWindow* ownedWindow();
	BOOL isNull();

	zLayout* layout();
	zLayout* layout(zLayout *newLayout);
	virtual void* asType(int type);

protected:
	zPoint _at;
	zDimension _size;
	zSizer* dad;
	zGrav _affinity;
#ifndef Z_IMPDLL	
	static zLayout *defLayout;
#endif
	zLayout *_layout;
	zSizerDlist *_kids;
	void *owned;
	int ownerType;
	int items;
//JC	int dontMove_;

};

class Z_DLLSPEC zLeafSizer:public zSizer {
public:
	zLeafSizer(zSizer*);
	virtual void update(zRect* =0);
	virtual void reqArea(zReqGeom&);
	virtual void setArea(zRect&,int =0);
	virtual void *asType(int type);
};

class Z_DLLSPEC zFillerSizer:public zLeafSizer {
public:
	zFillerSizer(zSizer*);
	inline zReqGeom& space();
	virtual void reqArea(zReqGeom&);
	virtual void *asType(int type);
private:
	zReqGeom mySpace;
};

class Z_DLLSPEC zFixedSizer:public zLeafSizer {
public:
	zFixedSizer(zSizer*,zGrav);
	inline void want(const zDimension);
	inline zDimension want() const;
	virtual void reqArea(zReqGeom&);
	virtual void *asType(int type);
private:
	zDimension _want;
};

class Z_DLLSPEC zGravSizer:public zSizer {
public:
	zGravSizer(zGrav,zDimension,zSizer* =0);
	zGravSizer(zGrav,int,zSizer* =0);
	inline zGrav gravity() const;
	inline void gravity(const zGrav);
	virtual void reqArea(zReqGeom&);
	inline zDimension useArea() const;
	inline void useArea(const zDimension);
	virtual void *asType(int type);
protected:
	zDimension _want;
};

class Z_DLLSPEC zPercentSizer:public zGravSizer {
public:
	zPercentSizer(zGrav,zSizer*);
	zPercentSizer(zGrav,float vertPer,float horPer,zSizer*);
	inline float vertical() const;
	inline void vertical(const float);
	inline float horizontal() const;
	inline void horizontal(const float);

	virtual void reqArea(zReqGeom&);
	virtual void *asType(int type);
private:
	float vPer,hPer;
};

enum zFit { ZFIT_BOTH,ZFIT_WIDTH,ZFIT_HEIGHT };

class Z_DLLSPEC zGrowToFitSizer:public zGravSizer {
public:
	zGrowToFitSizer(zGrav,zSizer* =0);
	zGrowToFitSizer(zGrav,zFit select,int other,zSizer* =0);
	virtual void reqArea(zReqGeom&);
	virtual void setArea(zRect&,int =0);
	virtual void *asType(int type);
private:
	zFit sides;
};

class Z_DLLSPEC zLayoutSizer:public zGravSizer {
public:
	zLayoutSizer(zSizer*);
	inline zLayout* layout();
	inline void layout(zLayout *);
	~zLayoutSizer();
	virtual void *asType(int type);
};

class Z_DLLSPEC zSizeWithParent:public zSizer {
public:
	zSizeWithParent();
	virtual void reqArea(zReqGeom&);
	virtual void resize(zWindow*,zSizeEvt *);
	virtual void *asType(int type);
};



#define ZEXPAND_LEFT	0x01
#define ZEXPAND_RIGHT   0x02
#define ZEXPAND_TOP     0x04
#define ZEXPAND_BOTTOM  0x08
#define ZEXPAND_VERT    0x10
#define ZEXPAND_HORIZ   0x20
#define ZEXPAND_DEFAULT ZEXPAND_RIGHT | ZEXPAND_BOTTOM

//
// ExpandSizer:  Contains information about an object whose geometry is
//  being managed via a set of expansion flags.
//
class Z_DLLSPEC zExpandSizer : public zSizer {
public:
  zExpandSizer(unsigned int flags = ZEXPAND_DEFAULT,zSizer *parent_ =0);
  
  zExpandSizer(const zPoint& at,
    const zDimension& size,
    unsigned int flags = ZEXPAND_DEFAULT,
    zSizer *parent_ = 0);

  zExpandSizer(
    zCoOrd x,
    zCoOrd y,
    zCoOrd width,
    zCoOrd height,
    unsigned int flags = ZEXPAND_DEFAULT,
    zSizer *parent_ = 0);

  zExpandSizer(
    const zRect& r,
    unsigned int flags = ZEXPAND_DEFAULT,
    zSizer *parent_ = 0);

  unsigned int flags() const;
  void flags(unsigned int newFlags);

  virtual void reqArea(zReqGeom& rg);
  virtual void *asType(int type);

private:
  unsigned int flags_;         // where should expansion occur, when 
                               // parent changes size.

  float        leftPercent_;   // percent of expandable area taken up
  float        horzPercent_;   // by left margin, width, top margin, and
  float        topPercent_;    // height
  float        vertPercent_;

  int          horzFixed_;     // amount of space that shouldn't shrink
  int          vertFixed_;     // or grow

  zPoint       oldAt_;         // indicates that percentages and fixed
  zDimension   oldSize_;       // amounts need to be recalculated
  BOOL         dirty_;

                               // calculates percentages and fixed amounts,
                               // if necessary
  void         calcAmounts(const zDimension& area);

  friend class Z_DLLSPEC zExpandLayout;
};


class Z_DLLSPEC zLayout {
public:
	virtual void allocate(zRect &d,zSizerDlist *req,int)=0;
	virtual void request(zReqGeom &d,zSizerDlist *req,int)=0;
protected:
	zLayout();
};

class Z_DLLSPEC zNormalLayout:public zLayout {
public:
        zNormalLayout() {}
	virtual void allocate(zRect &d,zSizerDlist *req,int);
	virtual void request(zReqGeom &d,zSizerDlist *req,int);
#ifndef __GNUC__
	~zNormalLayout() {}
#endif
};

class Z_DLLSPEC zTiler:public zLayout {
public:
	zTiler(int t);
	virtual void allocate(zRect &d,zSizerDlist *req,int);
	virtual void request(zReqGeom &d,zSizerDlist *req,int);
#ifndef __GNUC__
	~zTiler() {}
#endif
private:
	int todo;
};

class Z_DLLSPEC zLRtiler:public zTiler {
public:
	zLRtiler();
#ifndef __GNUC__
	~zLRtiler() {}
#endif
};

class Z_DLLSPEC zTBtiler:public zTiler {
public:
	zTBtiler();
#ifndef __GNUC__
    ~zTBtiler() {}
#endif
};

//
// ExpandLayout:  A geometry manager that lays out children based upon a
//  set of flags that defines where each child wants expansion to occur:
//  on its left border, across its horizontal dimension, and/or on its
//  right border, etc.
//

class Z_DLLSPEC zExpandLayout : public zLayout {
public:
	zExpandLayout(zWindow*);
	zExpandLayout(const zDimension&);

    virtual void allocate(zRect &area, zSizerDlist *lst, int items);
    virtual void request(zReqGeom &reqGeom, zSizerDlist *lst, int items);

private:
	int     prevWidth_;
    int     prevHeight_;
};


#include <zapph/sizer.inl>
#endif

