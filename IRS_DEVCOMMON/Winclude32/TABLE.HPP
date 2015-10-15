
#ifndef _ZAPPTABLE_INCLUDED
#define _ZAPPTABLE_INCLUDED 1
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
// Next line borrowed from vis.hpp
enum ZTEXT_ALIGN { ZALIGN_LEFT=1, ZALIGN_RIGHT=2, ZALIGN_CENTER=4 };

// Next lines borrowed from port.hpp
#define SENDMSG(a,b,c,d) SendMessage(a,b,c,d)
#define POSTMSG(a,b,c,d) PostMessage(a,b,c,d)
#define MSGP1(a) a->wParam
#define MSGP2(a) a->lParam
#define MSG1(a) a.wParam
#define MSG2(a) a.lParam
#define FULLVKCODE(xx) xx->wParam
#define NEEDRETURN ES_WANTRETURN|ES_MULTILINE

//class definition borrowed from RESID.HPP
#ifndef WIN32
	class Z_DLLSPEC zResStrBlock {
		zUserResource ur;
		char *pos,*start;
	public:
		zResStrBlock(const zResId&);
		~zResStrBlock();
		char* operator()();
		void reset();
	};
#endif

#define ZRES_COLUMNCURSOR 30000
#define ZRES_ROWCURSOR 30001

#ifndef RC_INVOKED

enum zTLabelKind { ZTLABEL_ROWS=0,ZTLABEL_COLS=1,ZTLABEL_CORNER=2 };
enum zTLabelType { ZTLABEL_NUMERIC, ZTLABEL_BLANK, ZTLABEL_TEXTUAL, ZTLABEL_ALPHA};
enum zTLabelStyle { ZTLABEL_PLAIN, ZTLABEL_3D, ZTLABEL_3DHEAVY };
enum zTLabelDirection {
	ZTDIR_GOLEFT, ZTDIR_GORIGHT, ZTDIR_GOUP,
	ZTDIR_GODOWN, ZTDIR_DONTGO
};

#define ZIT_MAXATTRTYPE 10
#define ZIT_USERATTRTYPE 9
	
class Z_DLLSPEC zTable;
class Z_DLLSPEC zTableCell;
class Z_DLLSPEC zTableModel;
class Z_DLLSPEC zTableRegion;

#define ZTCELL_VALIDATED 1
#define ZTCELL_AUTOCHANGE 2
#define ZTCELL_FORMATTED 4
#define ZTCELL_NUMFMTPRIORITY 8

#define ZTCELLTYPE_NUMERIC 1
#define ZTCELLTYPE_ALLOWALL 2

typedef short* shortPtr;
declDynaArray(shortPtr)

typedef zTableCell* zTableCellPtr;
declDynaArray(zTableCellPtr)

typedef zTableCellPtrDynaArray* zTableCellPtrDynaArrayPtr;
declDynaArray(zTableCellPtrDynaArrayPtr)

typedef int (zEvH::*zKeyProc)(zKeyEvt*);
typedef int (zEvH::*zTableNotifyProc)(zTableModel*,const zTableRegion*);
class Z_DLLSPEC zTablePrintable;
class Z_DLLSPEC ziTableFontMap;

class Z_DLLSPEC zTableLoc {
protected:
	unsigned long _x;
	unsigned long _y;
public:
	zTableLoc(const zTableLoc&p) { _x=p._x; _y=p._y; }
	zTableLoc() { _x=0; _y=0; }
	zTableLoc(unsigned long A,unsigned long B) { _x=A; _y=B; }
	
	inline zTableLoc operator +(const zTableLoc&) const;
	inline zTableLoc operator *(const unsigned long) const;
	friend zTableLoc operator *(const unsigned long i,const zTableLoc& p) {
		return zTableLoc(i*p._x, i* p._y);
	}
	inline zTableLoc operator /(unsigned long) const;
	inline zTableLoc operator %(unsigned long) const;
	inline zTableLoc operator +=(const zTableLoc&);
	inline zTableLoc operator -=(const zTableLoc&);
	inline zTableLoc operator *=(const unsigned long);
	inline int operator ==(const zTableLoc&) const;
	inline int operator !=(const zTableLoc&) const;
	inline int operator >=(const zTableLoc&) const;
        inline int lessEqual(const zTableLoc&) const;
	inline int operator <=(const zTableLoc&) const;
	inline int operator >(const zTableLoc&) const;
	inline int operator <(const zTableLoc&) const;
	inline zTableLoc operator =(const unsigned long);
	inline zTableLoc operator =(const zTableLoc &);
	inline zTableLoc Min(const zTableLoc&) const;
	inline zTableLoc Max(const zTableLoc&) const;
	inline unsigned long& x();
	inline unsigned long& y();
	inline unsigned long x() const;
	inline unsigned long y() const;
	inline void setNull();
};

class Z_DLLSPEC zTableRegion {
	zTableLoc tl;
	zTableLoc br;
public:
	zTableRegion() {}
	zTableRegion(unsigned long lft,unsigned long tp,
		unsigned long rgt,unsigned long btm):tl(lft,tp),br(rgt,btm) {}
	zTableRegion(const zTableLoc& p,const zTableLoc& p2):tl(p),br(p2) {}
	zTableRegion(const zTableRegion&r):tl(r.tl),br(r.br) {}
		
	inline zTableLoc& topLeft();
	inline zTableLoc& bottomRight();
	inline zTableLoc topRight() const;
	inline zTableLoc bottomLeft() const;
	inline unsigned long& left();
	inline unsigned long& right();
	inline unsigned long& top();
	inline unsigned long& bottom();
	inline zTableLoc center() const;
	inline zTableLoc topLeft() const;
	inline zTableLoc bottomRight() const;
	inline unsigned long left() const;
	inline unsigned long right() const;
	inline unsigned long top() const;
	inline unsigned long bottom() const;
	inline void operator+=(const zTableLoc&);
	inline void operator-=(const zTableLoc&);
	inline zTableRegion& operator=(const zTableRegion&);
	inline int operator==(const zTableRegion&) const;
	inline int operator!=(const zTableRegion&) const;
	inline unsigned long width() const;
	inline unsigned long height() const;
	inline int isNull() const;
	inline zTableRegion& makeNull();
	inline int contains(const zTableLoc&) const;
	inline int contains(const zTableRegion&) const;
	inline int intersects(const zTableRegion&) const;
};

class Z_DLLSPEC zTableEditLine:public zFormattedEdit {
protected:
	zWindow *_win;
	zTable *_tbl;
	int _curRO;
public:
	zTableEditLine(zWindow*,zSizer*,DWORD=0,int=0);
	zTableEditLine(zWindow*,int);
	inline void setTable(zTable*);
	virtual void setCell(const zTableLoc&);
	void readOnly(int);
	inline int readOnly();
	int ch(zKeyEvt *);
	virtual ~zTableEditLine();
	virtual	long dispatch(zEvent*);
};

class Z_DLLSPEC zTableInplaceEditLine:public zTableEditLine {
	zTableLoc pos;
	zFont *fnt;
public:
	int showing;
	zTableInplaceEditLine(zTable*,DWORD=0, int=0);
	int ch(zKeyEvt *);
	int focus(zFocusEvt*);
	virtual void setCell(const zTableLoc&);
	virtual ~zTableInplaceEditLine();
};

class Z_DLLSPEC ziTableScroller:public zPaneScroller {
public:
	ziTableScroller(zPane*);
	virtual int respondToSize();	
	virtual int vertThumbPos();
	virtual int horzThumbPos();
	virtual int vertPageMove();
	virtual int horzPageMove();
};

class Z_DLLSPEC zTableAttr {
	int attrId;
protected:
	zTableRegion _region;
public:
	zTableAttr(int attrType,const zTableRegion& rgn);
	inline int type();
	virtual void setup(zDisplay*);
	virtual void reset(zDisplay*);
	inline zTableRegion& region();
	virtual ~zTableAttr();
};

declDlist(zTableAttr)

class Z_DLLSPEC zTableTextColorAttr:public zTableAttr {
	zColor clr,prev;
	int needReset;
public:
	zTableTextColorAttr(zColor c,const zTableRegion& rgn);
	virtual void setup(zDisplay*);
	virtual void reset(zDisplay*);
	inline zColor& color();
	inline void setColor(zColor color_);
};

class Z_DLLSPEC zTableBackColorAttr:public zTableAttr {
	zColor clr;
	zBrush *myBrush,*prev;
	int needReset;
public:
	zTableBackColorAttr(zColor c,const zTableRegion& rgn);
	virtual void setup(zDisplay*);
	virtual void reset(zDisplay*);
	~zTableBackColorAttr();
};

class Z_DLLSPEC zTableFontAttr:public zTableAttr {
	friend class Z_DLLSPEC ziTableFontMap;
	zFont *myFont,*prev,*myPrtFont;
	int needReset;
public:
	zTableFontAttr(zFont *f,const zTableRegion& rgn);
	virtual void setup(zDisplay*);
	virtual void reset(zDisplay*);
	~zTableFontAttr();
};

class Z_DLLSPEC zTableReadOnlyAttr:public zTableAttr {
public:
	zTableReadOnlyAttr(const zTableRegion& rgn);
	~zTableReadOnlyAttr();
};

class Z_DLLSPEC zTableFormatterAttr:public zTableAttr {
protected:
	zFormatter* _fmt;
public:
	zTableFormatterAttr(const zTableRegion& rgn);
	zTableFormatterAttr(const zTableRegion& rgn,zFormatter *);
	inline zFormatter* getFormatter();
	zFormatter* setFormatter(zFormatter*);
	~zTableFormatterAttr();
};

class Z_DLLSPEC zTableNumFormatterAttr:public zTableAttr {
protected:
	zNumFormatter* _fmt;
public:
	zTableNumFormatterAttr(const zTableRegion& rgn);
	zTableNumFormatterAttr(const zTableRegion& rgn,zNumFormatter*);
	inline zNumFormatter* getFormatter();
	zNumFormatter* setFormatter(zNumFormatter*);
	~zTableNumFormatterAttr();
};

class Z_DLLSPEC zTableTextAlignmentAttr:public zTableAttr {
	ZTEXT_ALIGN align;
public:
	zTableTextAlignmentAttr(ZTEXT_ALIGN a,const zTableRegion& rgn);
	inline ZTEXT_ALIGN getAlignment();
	inline void setAlignment(ZTEXT_ALIGN);
	~zTableTextAlignmentAttr();
};

class Z_DLLSPEC ziTableEnv {
	zTableAttrDlist* attrs;
public:
	int hilight;
	int active;
	zTableAttr *list[ZIT_MAXATTRTYPE];
	ziTableEnv(zTableModel*);
	ziTableEnv(zTable*);
	void setup(const zTableLoc&);
	~ziTableEnv();
};

class Z_DLLSPEC zTableCell:public zEvH {
protected:
	zTableModel *_dad;
	unsigned long _flags;
public:	
	zTableCell(zTableModel *,const zTableLoc&);
	zTableLoc _pos;
	virtual void draw(const zRect& where,zDisplay*,ziTableEnv*);

	virtual const char* getText();
	virtual void setText(const char *);

	inline zTableModel* parent();
	inline unsigned long getFlags();
	inline void setFlag(unsigned long);
	inline void clearFlag(unsigned long);
	inline zTableLoc& location();
	virtual ~zTableCell();
};

class Z_DLLSPEC zTableTextCell:public zTableCell {
protected:
	RWCString _text;
public:
	zTableTextCell(zTableModel*,const char *t,const zTableLoc&);
	virtual const char* getText();
	virtual void setText(const char *);
	virtual void draw(const zRect& where,zDisplay*,ziTableEnv*);
};

class Z_DLLSPEC zTableFmtCellBase:public zTableTextCell {
protected:
	RWCString _unformattedText;
public:
	zTableFmtCellBase(zTableModel*,const char*,const zTableLoc&);
	virtual const char* getText();
	virtual void setText(const char*);
	inline const char* getUnformattedText();
	inline const char* getFormattedText();
	virtual void draw(const zRect& where,zDisplay*,ziTableEnv*);
};

class Z_DLLSPEC zTableFmtTextCell:public zTableFmtCellBase {
public:
	zTableFmtTextCell(zTableModel*,const char*,const zTableLoc&);
	~zTableFmtTextCell();
};

class Z_DLLSPEC zTableFmtNumCell:public zTableFmtCellBase {
public:
	zTableFmtNumCell(zTableModel*,const char*,const zTableLoc&);
	~zTableFmtNumCell();
};

class Z_DLLSPEC zTableValidatedFmtTextCell:public zTableFmtCellBase {
public:
	zTableValidatedFmtTextCell(zTableModel*,const char*,const zTableLoc&);
	~zTableValidatedFmtTextCell();
};

class Z_DLLSPEC zTableValidatedFmtNumCell:public zTableFmtCellBase {
public:
	zTableValidatedFmtNumCell(zTableModel*,const char*,const zTableLoc&);
	~zTableValidatedFmtNumCell();
};

class Z_DLLSPEC zTableBmpCell:public zTableCell {
protected:
	zBitmap *_bmp;
public:
	zTableBmpCell(zTableModel*,zBitmap*,const zTableLoc&);
	zBitmap* getBitmap();
	zBitmap* setBitmap(zBitmap*);
	virtual void draw(const zRect& where,zDisplay*,ziTableEnv*);
	virtual ~zTableBmpCell();
};

class Z_DLLSPEC zTableDrawCell:public zTableCell {
public:
	zTableDrawCell(zTableModel*,const zTableLoc&);
	virtual void draw(const zRect& where,zDisplay*,ziTableEnv*);
	virtual int userDraw(const zRect& where,zDisplay*,ziTableEnv*)=0;
};

class Z_DLLSPEC zTableCellMaker {
protected:
	zTableModel *_model;
public:
	zTableCellMaker(zTableModel*);
	virtual int findType(const zTableLoc&,const char*);
	virtual zTableCell* makeCellFromType(int,const char *txt,const zTableLoc&);
};

class Z_DLLSPEC ziTableViewNode {
public:
	ziTableViewNode(zEvH* callme,zTableNotifyProc p);
	void callback(zTableModel* m) { (procObj->*proc)(m,&dirtyRect); }
	void callbackReset(zTableModel* m) { (procObj->*proc)(m,0); }
	
	zTableRegion dirtyRect;
	int isReset;
	int isDirty;
	zEvH *procObj;
	zTableNotifyProc proc;
};

declDlist(ziTableViewNode)

class Z_DLLSPEC zTableModel:public zEvH {
	friend class Z_DLLSPEC zTableCell;
protected:
	unsigned long _numCols, _numRows;

	zTableAttrDlist* _attrs;
	ziTableViewNodeDlist *_views;
	zTableCellMaker *_ttc;
public:
	zTableModel(unsigned long cols,unsigned long rows);
	virtual void setCell(const zTableLoc& pt,zTableCell*cel)=0;
	virtual zTableCell* getCell(const zTableLoc&)=0;
	
	inline zTableAttrDlist* attrList();
	zTableAttr* addAttr(zTableAttr*);
	zTableAttr* removeAttr(zTableAttr*);

	zTableCellMaker* getCellMaker();
	zTableCellMaker* setCellMaker(zTableCellMaker*);

	zTableCell* textToCell(const zTableLoc&,const char*);
	virtual void setDimension(unsigned long cols,unsigned long rows)=0;
	virtual zTableRegion getDimension();
	unsigned long getNumRows();
	unsigned long getNumCols();
	
	void attachView(zEvH*,zTableNotifyProc);
	void detachView(zEvH*,zTableNotifyProc);
	virtual void updateAllViews();
	void updateOtherViews(zEvH*);
	void setChanged(const zTableLoc&);
	void setChanged(zTableRegion* =0);
	int getViewDirty(zEvH*,zTableRegion&);
	
	virtual ~zTableModel();
	virtual void deleteRow(unsigned long);
	virtual void deleteCol(unsigned long);

	virtual void insertRow(unsigned long);
	virtual void insertCol(unsigned long);
};

class Z_DLLSPEC zTableSmallModel:public zTableModel {
	zTableCellPtrDynaArrayPtrDynaArray data;
protected:
	friend class Z_DLLSPEC zTableCell;
public:
	zTableSmallModel(unsigned long cols,unsigned long rows);
	virtual void setCell(const zTableLoc& pt,zTableCell*cel);
	virtual void setDimension(unsigned long cols,unsigned long rows);
	virtual zTableCell* getCell(const zTableLoc&);
	~zTableSmallModel();
	virtual void deleteRow(unsigned long);
	virtual void deleteCol(unsigned long);

	virtual void insertRow(unsigned long);
	virtual void insertCol(unsigned long);
};

class Z_DLLSPEC zTableLargeModel:public zTableModel {
	zTableCellPtrDynaArrayPtrDynaArray data;
protected:
	friend class Z_DLLSPEC zCell;
public:
	zTableLargeModel(unsigned long cols,unsigned long rows);
	virtual void setCell(const zTableLoc& pt,zTableCell*cel);
	virtual void setDimension(unsigned long cols,unsigned long rows);
	virtual zTableCell* getCell(const zTableLoc&);
	~zTableLargeModel();
	virtual void deleteRow(unsigned long);
	virtual void deleteCol(unsigned long);

	virtual void insertRow(unsigned long);
	virtual void insertCol(unsigned long);
};

class Z_DLLSPEC ziTableVirtualRecord {
public:
	ziTableVirtualRecord();
	zTableCellPtrDynaArray* data;
	unsigned long pos;
};

declDList(ziTableVirtualRecord)

class Z_DLLSPEC zTableVirtualRowModel:public zTableModel {
	ziTableVirtualRecordDList data;
	int cacheQty;
	int dirty;
	int numCached;
	int inDiscardRow;
	zTableVirtualRowModel(const zTableVirtualRowModel &) : zTableModel(0,0) {}
	const zTableVirtualRowModel & operator=(const zTableVirtualRowModel &) {
			return *this; }
protected:
	friend class Z_DLLSPEC zTableCell;
	void deleteCacheElement(unsigned long row);
	ziTableVirtualRecord* getFreeCacheElement();
public:
	zTableVirtualRowModel(unsigned long cols,unsigned long rows);
	virtual void setCell(const zTableLoc& pt,zTableCell*cel);
	virtual void setDimension(unsigned long cols,unsigned long rows);
	virtual zTableCell* getCell(const zTableLoc&);
	~zTableVirtualRowModel();
	inline int getNumRowsCached();
	void setNumRowsCached(int);
	void clearCache();
	virtual int discardRow(unsigned long row)=0;
	virtual int demandLoadRow(unsigned long row)=0;

	virtual void deleteRow(unsigned long);
	virtual void deleteCol(unsigned long);

	virtual void insertRow(unsigned long);
	virtual void insertCol(unsigned long);
};

class Z_DLLSPEC zTableVirtualColModel:public zTableModel {
	ziTableVirtualRecordDList data;
	int cacheQty;
	int dirty;
	int numCached;
	int inDiscardCol;
	zTableVirtualColModel(const zTableVirtualColModel &) : zTableModel(0,0) {}
	const zTableVirtualColModel & operator=(const zTableVirtualColModel &) {
		return *this; }
protected:
	friend class Z_DLLSPEC zTableCell;
	void deleteCacheElement(unsigned long col);
	ziTableVirtualRecord* getFreeCacheElement();
public:
	zTableVirtualColModel(unsigned long cols,unsigned long rows);
	virtual void setCell(const zTableLoc& pt,zTableCell*cel);
	virtual void setDimension(unsigned long cols,unsigned long rows);
	virtual zTableCell* getCell(const zTableLoc&);
	~zTableVirtualColModel();
	inline int getNumColsCached();
	void setNumColsCached(int);
	void clearCache();
	virtual int discardCol(unsigned long col)=0;
	virtual int demandLoadCol(unsigned long col)=0;

	virtual void deleteRow(unsigned long);
	virtual void deleteCol(unsigned long);

	virtual void insertRow(unsigned long);
	virtual void insertCol(unsigned long);
};

typedef RWCString * RWCStringPtr;

declDynaArray(RWCStringPtr)

class Z_DLLSPEC zTable:public zPane {
	friend class Z_DLLSPEC zTableCell;
	friend class Z_DLLSPEC ziTableScroller;
	friend class Z_DLLSPEC zTablePrintable;
	friend class Z_DLLSPEC AutoResizeColTable;
	zTableLoc bound;
	int deleteModel;
	
	int _labels;
	int _allowRowResize;
	int _allowColResize;
	zTLabelDirection _editMoveDir;
	zDimension _defaultRowColSize;

	zPen *tPen;
	zTableModel *cells;
	int mouseState,mdown;

	BinDrawOp prevDrMode;
	zPoint curMousePos;
	zColor _gridLineColor;
	
	zCursor colCursor, rowCursor;
	int curCursor;
	unsigned long sizingCol,sizingRow; //,offsetX,offsetY;
	zPoint oldSizingPos;

	zTLabelStyle labelStyle;
	int _allowMultiSelect;
	int _allowLabelSelect;
	int _allowMultiLabelSelect;
	int _allowLabelRowSizing;
	int _allowLabelColSizing;
	unsigned long _selectedLabelNumLo,_selectedLabelNumHi;
	zTLabelKind _selectedLabelKind;
	unsigned long _labelDragStart,_labelDragEnd;
	RWCStringPtrDynaArray rowText;
	RWCStringPtrDynaArray colText;	
	int _wasSetMetricsCalled;
	int _hasFocus;
	zDimension _labelSize;

	// zRect area; moved to protected
	zDimension _defCellSize;

	unsigned long maxSetRow,maxSetCol;
	int _colShift,_rowShift;
	int spacePixels;
	shortPtrDynaArray rowHeights;
	shortPtrDynaArray colWidths;

	unsigned int _rowPos[100];
	unsigned int _colPos[100];

	unsigned int _tilRows,_tilCols;
	zEvH *_notifyCellObj;
	CallBackProc _notifyCellProc;
	zEvH *_notifySelObj;
	CallBackProc _notifySelProc;
//	virtual void calcVisible();
//	void calcMove(int& x,int& y);
//	int keyDownInput(zEvent *);
	void calcSub(int&,int&);
//	void updateScrollers(int,int);
protected:
	int isScrollBarCalcDone_;
	zTableEditLine *_tEdit;
	zRectRegion* _tableClipRegion;

	friend class Z_DLLSPEC zTableEditLine;
	friend class CGUI_InPlaceEditLine;
	virtual void editEscape();
	virtual void editClose();
	virtual void editReturn();
	virtual void editMove(int dir);
	virtual int keyDownInput(zEvent *);
	
	void numToAlphaLabel(char*,unsigned long num);
	void clearSelectedLabels();
	void changeCurSelLoc(int,int,int =1);
	virtual void changeActive(const zTableLoc&,int forceUpdate=0);
	int isCellPoint(const zPoint& posIn,zTableLoc& logicalOut);
	int isLabelPoint(const zPoint&,unsigned long& num,zTLabelKind& kind);
	void doLabelClick(const zPoint&,unsigned long num,zTLabelKind kind);
	void doLabelMouseMove(const zPoint&,unsigned long num,zTLabelKind kind);
	virtual void drawEmptyCell(const zRect& where,ziTableEnv*);
	int updateProc(zTableModel*,const zTableRegion*);
	zTableRegion dirtyToLogical(const zRect& dirty);

	// Moved the following items from private for access by CGUI_zTable
	zTableRegion cellArea;
	void updateScrollers(int,int);
	virtual void calcVisible();
	void calcMove(int& x,int& y);
	unsigned long offsetX,offsetY;
	zRect area;
	unsigned long _rows,_cols;
	zTLabelType labelTypes[3];
	zBrush *liteGrayBrush,*whiteBrush,*darkGrayBrush,*blackBrush;
	zPen *blackPen,*whitePen,*grayPen;
	zTableLoc act;
	// End of moved items

	unsigned long selectedLabelNumLo() { return _selectedLabelNumLo; }
	unsigned long selectedLabelNumHi() { return _selectedLabelNumHi; }
	zTLabelKind selectedLabelKind() { return _selectedLabelKind; }

	unsigned int rowPos(unsigned long row);
	unsigned int colPos(unsigned long col);
	inline void doNotifyActiveChange();
	inline void doNotifySelChange();
	void setup();
	void ziCalcScrollers();

public:
	zTable(zWindow*,zSizer*,DWORD,zTableModel*);
	zTable(zWindow*,zSizer*,zCursor&,zCursor&,DWORD=0,zTableModel* =0);
	zTable(zWindow*,zSizer*,DWORD style_=0,const char* =0, int ctrlId_=0);
	zTable(zWindow*,int ctrlid_,zTableModel* =0);
	virtual ~zTable();	// Tom added "virtual" qualifier 29Oct96.

	void setMetrics();
	virtual void scrollPane(int x, int y,const zRect& scR);
	virtual long dispatch(zEvent*);
	virtual int draw(zDrawEvt *);
	virtual int size(zSizeEvt *);
	virtual int mouseButtonDown(zMouseClickEvt *);
	virtual int mouseMove(zMouseMoveEvt *);
	virtual int mouseButtonUp(zMouseClickEvt *);
	virtual int focus(zFocusEvt*);
	virtual int ch(zKeyEvt *);
	
	virtual void drawLabel(const zRect&,unsigned long num,zTLabelKind);
	void setEditLine(zTableEditLine*);

	void showLabels(int);
	int areLabelsShowing();
	void setLabelType(zTLabelKind,zTLabelType);
	zTLabelType getLabelType(zTLabelKind);
	zTLabelStyle getLabelStyle();
	void setLabelStyle(zTLabelStyle st);
	const char* getLabelText(zTLabelKind,unsigned long num);
	void setLabelText(zTLabelKind,unsigned long num,const char *txt);
	void setLabelText(zTLabelKind,const zResStrBlock &);
	zDimension getLabelSize();
	void setLabelSize(const zDimension& di);

	void enableMultiSelect(int b);
	int isMultiSelectEnabled();

	void enableLabelSelect(int b);
	int isLabelSelectEnabled();

	void enableMultiLabelSelect(int b);
	int isMultiLabelSelectEnabled();

	void enableColSizing(int b);
	void enableRowSizing(int b);

	int isColSizingEnabled();
	int isRowSizingEnabled();

	zTLabelDirection getActiveMoveDirection();
	void setActiveMoveDirection(zTLabelDirection);
	
	void setSelection(const zTableRegion&);
	void getSelection(zTableRegion&);

	void setSelection(zTLabelKind,unsigned long num);
	void setSelection(zTLabelKind,unsigned long loNum,unsigned long hiNum);
	
	void setResizeSpace(int pixels);

	zColor getGridLineColor();
	void setGridLineColor(zColor);
	inline zTableModel* model();

	void setDirty(const zTableLoc&);
	void setDirty(const zTableRegion&);

	void setLabelsDirty(zTLabelKind typ,unsigned long lo,unsigned long hi);
	void getVisibleCells(zTableRegion&);
	void getCellRect(zRect&,const zTableLoc&);

	unsigned int rowHeight(unsigned long row);
	unsigned int colWidth(unsigned long col);

	void setRowHeight(unsigned long row,unsigned int value);
	void setRowHeight(unsigned long lo,unsigned long hi,unsigned int value);
	virtual void setColWidth(unsigned long col,unsigned int value);
	void setColWidth(unsigned long lo,unsigned long hi,unsigned int value);

	void deleteRow(unsigned long);
	void insertRow(unsigned long);
	void deleteCol(unsigned long);
	void insertCol(unsigned long);

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void setActiveCell(const zTableLoc&);
	zTableLoc getActiveCell();

	void setNotifySelChange(zEvH*,CallBackProc);
	void setNotifyActiveCellChange(zEvH*,CallBackProc);

	inline int hasFocus();
	inline void setDirty(zRect* r=0,BOOL eraseBackground=1);
};

class Z_DLLSPEC ziTableFontMapDlist;

class Z_DLLSPEC ziTableFontMap {
	zFont *src;
	zFont *dest;
public:
	ziTableFontMap();
	ziTableFontMap(zFont*,zFont*);
	~ziTableFontMap();
	static zFont*map(ziTableFontMapDlist**,zFont*,double scaleX,double scaleY);
	static void setup(zTable*,ziTableFontMapDlist**,double sX,double sY);
	static void cleanup(ziTableFontMapDlist**);
};

declDlist(ziTableFontMap)

class Z_DLLSPEC zTablePrintable:public zTable {
	int inPrint;
	int pageNo,totPages;
	zTableRegion prtRange;
	void calcPrtVisible();
	zTableRegion printToLogical(const zRect& dirty);
	int downAcross,rowLabels,colLabels,repeatCols,repeatRows;
	int boldLabelFrames;
	int pixWinX,pixWinY;
	double rowScale,colScale;
	ziTableFontMapDlist *_fntList;
	zFont *winFont;
	int tdraw(zDrawEvt*);
        BOOL pushed;

public:
	zTablePrintable(zWindow*,zSizer*,DWORD,zTableModel*);
	zTablePrintable(zWindow*,zSizer*,zCursor&,zCursor&,DWORD=0,zTableModel* =0);
	zTablePrintable(zWindow*,zSizer*,DWORD style_=0,const char* =0, int ctrlId_=0);
	zTablePrintable(zWindow*,int ctrlid_,zTableModel* =0);

	virtual int print(zPrinterDisplay *pr, zRect *r);
	virtual BOOL morePages();
	virtual int printSetup(zPrinterDisplay *pr, zPrintJob *pj);
	virtual void printCleanup(zPrinterDisplay *pr, zPrintJob *);
	void setMetrics();
	void drawLabel(const zRect& r,unsigned long num, zTLabelKind kind);
	int calcPage(zTableRegion& prev,int first=0);
	void calcPageNum(zTableRegion& prev,int n);

	
	virtual void printFooter(zPrinterDisplay *pr,int calcOnly =0);
	virtual void printHeader(zPrinterDisplay *pr,int calcOnly =0);
	virtual void printMargins(zPrinterDisplay *pr,int calcOnly =0);

	void setPrintRange(const zTableRegion&);
	void setPrintDownAcross();
	void setPrintAcrossDown();
	void setPrintRowLabels(int tf);
	void setPrintColumnLabels(int tf);
	void setPrintRepeatRowLabels(int tf);
	void setPrintRepeatColumnLabels(int tf);
	void setPrintBoldLabelFrames(int tf);

	int getPrintDownAcross();
	int getPrintAcrossDown();
	int getPrintRowLabels();
	int getPrintColumnLabels();
	int getPrintRepeatRowLabels();
	int getPrintRepeatColumnLabels();
	int getPrintBoldLabelFrames();
};

#include "intpack/ziph/table.inl"
#endif
#endif
