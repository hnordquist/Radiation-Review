#ifndef _ZAPPFORMDLG_INCLUDED
#define _ZAPPFORMDLG_INCLUDED 1
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


#include <locale.h>

#define zGAINFOCUS 0
#define zLOSEFOCUS 1

enum zFormatterStat { zFmtStatBad=0, zFmtStatSimpleIns=1, zFmtStatReplace=2 };

enum zNumPictStrType { zNumTypeRange=0, zNumTypeDecimalSep=1,
	zNumTypeThousandSep=2, zNumTypeExcept=3 };

enum zNumReadOnly { zNumReadWhole=0, zNumReadInt=1, zNumReadFrac=2,
	zNumReadNone=3 };

class Z_DLLSPEC zBitVector {
    unsigned char *bits;
    unsigned int setSize;
public:
    zBitVector(int numEls);
    zBitVector(const zBitVector& bv);
    ~zBitVector() { delete bits; }
	
	void set(unsigned int i) {
		bits[i/8] |= (unsigned char)(1 << (i & 7));
	}
    void reset(unsigned int i) {
		bits[i/8] &= (unsigned char) ~(1 << (i & 7));
    }
    int operator[](unsigned int i) const {
		return (bits[i/8] & (1 << (i & 7)))? 1: 0;
    }
    int isSet(unsigned int i) const {
		return (bits[i/8] & (1 << (i & 7)))? 1: 0;
    }
    void clear() {
		memset(bits,'\0',setSize/8);
    }
};

#define zPSE_LITERAL	0x0001
#define zPSE_WRITABLE	0x0002
#define zPSE_TOUCHED	0x0004
#define zPSE_DECIMALPT	0x0008
#define zPSE_COMMA		0x0010

class Z_DLLSPEC zNumPictStrEl {
public:
	zNumPictStrEl(char c, zNumPictStrType t);
	zNumPictStrEl(char clo, char chi, char c);
	zNumPictStrEl(char excpt, char c);

	BOOL isValid(char c) const;
	zNumPictStrType getNumType() const;
	char getGuide() const;
	
private:
	zNumPictStrType numType;
	char numChar, numChar2, guide;
};

inline zNumPictStrType zNumPictStrEl::getNumType() const { return numType; }
inline char zNumPictStrEl::getGuide() const { return guide; }


class Z_DLLSPEC zIntlContext {
public:
	zIntlContext();
	~zIntlContext();

	void setCurrSymbol(char *str);
	void setNegSymbol(char *str);
	void setPosSymbol(char *str);
	void setDecimalPt(char d);
	void setMonDecimalPt(char d);
	void setThousandSep(char t);
	void setMonThousandSep(char t);
	void setPosCSPrecedes(BOOL prec);
	void setNegCSPrecedes(BOOL prec);
	void setPosCSSepSpace(BOOL prec);
	void setNegCSSepSpace(BOOL prec);
	void setPosSignPos(char p);
	void setNegSignPos(char p);
	lconv localConv;
};

class Z_DLLSPEC zPictStrEl {
public:
	virtual ~zPictStrEl();
	BOOL isLiteral() const;
	BOOL isWritable() const;
	BOOL isTouched() const;
	BOOL isDecimalPt() const;
	BOOL isCommaPos() const;

	virtual int addChar(char c, BOOL checkOnly = 0, unsigned long options=0)=0;
	virtual char getChar(unsigned long options=0);
	virtual int delChar();

protected:
	unsigned short flags;
	char spec;

	zPictStrEl(char mapChar) {
		spec = mapChar;
		flags = 0;
	}
	zPictStrEl(const zPictStrEl &pe) {
		flags = pe.flags;
		spec = pe.spec;
	}
};

inline BOOL zPictStrEl::isLiteral() const { return flags & zPSE_LITERAL; }
inline BOOL zPictStrEl::isWritable() const { return flags & zPSE_WRITABLE; }
inline BOOL zPictStrEl::isTouched() const { return flags & zPSE_TOUCHED; }
inline BOOL zPictStrEl::isDecimalPt() const { return flags & zPSE_DECIMALPT; }
inline BOOL zPictStrEl::isCommaPos() const { return flags & zPSE_COMMA; }

class Z_DLLSPEC zPictStrMaskEl : public zPictStrEl {
	char userChar;
	char guideChar;
public:
	zPictStrMaskEl(char c);
	zPictStrMaskEl(const zPictStrMaskEl &el) : zPictStrEl(el) {
		userChar = el.userChar;
		guideChar = el.guideChar;
	}
	virtual int addChar(char c, BOOL checkOnly=0,unsigned long options=0);
	virtual char getChar(unsigned long options=0);
	virtual int delChar();
};

class Z_DLLSPEC zPictStrSetEl : public zPictStrEl {
public:
	zPictStrSetEl(zBitVector *set, unsigned short exclude_);
	zPictStrSetEl(const zPictStrSetEl &el);
	~zPictStrSetEl();

	virtual int addChar(char c, BOOL checkOnly=0,unsigned long options=0);
	virtual char getChar(unsigned long options=0);
	virtual int delChar();
private:
	zBitVector *_set;
	unsigned short _exclude;
	char userChar, guideChar;
};

class Z_DLLSPEC zPictStrLiteral : public zPictStrEl {
public:
	zPictStrLiteral(char c) : zPictStrEl(c) {
		flags |= zPSE_LITERAL;
	}
	zPictStrLiteral(const zPictStrLiteral &el) : zPictStrEl(el) {}
	virtual int addChar(char c, BOOL checkOnly=0,unsigned long options=0);
	virtual char getChar(unsigned long options=0);
};

class Z_DLLSPEC zFormatter {
public:
	zFormatter(unsigned long mask);
	virtual ~zFormatter();
	virtual zFormatterStat
		addChars(RWCString &s, const char *c, int count,zRange &pos) = 0;
	virtual int checkChars(RWCString &s, zRange &pos) = 0;
	virtual int getChars(RWCString &str, unsigned long flags);
	virtual int firstWritablePosition(const char*);
	virtual int ziReset() { return 0; }

	BOOL hasStyle(unsigned long w) const;
protected:
	unsigned long flags;
};

inline BOOL zFormatter::hasStyle(unsigned long w) const { return (BOOL)(w & flags); }


class Z_DLLSPEC zMaskFormatter : public zFormatter {
public:
	zMaskFormatter(unsigned long mask) : zFormatter(mask) {}
	virtual zFormatterStat addChars(RWCString &s, const char *c, int count,zRange &pos);
	virtual int checkChars(RWCString &s, zRange &pos);
};

class Z_DLLSPEC zPictFormatter : public zFormatter {
public:
	zPictFormatter(unsigned long mask, const char *pict);
	~zPictFormatter();

	int mapStrToPict(const char *s, int count, int pos=0,BOOL doMap=1, int *lastChar=0);
	int mapPictToStr(RWCString &s, unsigned long flags=0);
	virtual zFormatterStat addChars(RWCString &s, const char *c, int count,zRange &pos);
	virtual int checkChars(RWCString &s, zRange &pos);
	virtual int getChars(RWCString &str, unsigned long flags);
	int firstWritablePosition(const char*);

protected:
	RWCString pict;
	int findNextWritable(int startPos=-1, BOOL matchBlankOnly=0);
	int findNextLiteral(int startPos=-1);
	int shiftRight(int dest, int src);
	int shiftLeft(int dest, int src);
	char *skipBlanks(char *s);
	getCharSetChar(char **s, int&c);
	buildSet(char **s);
	parseLiterals(char **s);
	repElement(char **s, int elType);
private:
	int numEls;
	zPictStrEl *elList[256];
	char tempElList[256];
	int *map, mapCount;
	int numlits;
	int literalPositions[20];
	int isLiteral(char);
	int findStringPosition(int,const char*,zRange&);
		
};

#define NUMFLDMAXLEN 256
#define DEC_LEFT	0
#define DEC_RIGHT	1
#define ACT_ADD		0
#define ACT_SUB		1

class Z_DLLSPEC zNumFormatter : public zFormatter {
public:
	zNumFormatter(unsigned long mask, const char *p, zIntlContext *intlInfo=0);
	~zNumFormatter();
	
	zFormatterStat addChars(RWCString &s,const char *c,int count,zRange &pos);
	virtual int checkChars(RWCString &s, zRange &pos);	
	int getChars(RWCString &s, unsigned long options);
	int getFracDigits() const;
	int firstWritablePosition(const char*);
	virtual int ziReset();

	zIntlContext *numIntl;
protected:
	void addGuideChars(char *str, zRange &pos);
	void addCurSign(char *str, zRange &pos);
	int findDecPos(char *str);
	void stripGuideChars(char *str, zRange &pos, BOOL remTrail);
	void stripChars(char *str, zRange &pos);
	void formatStr(char *str);
	void getInitialStr(char *str);
	int getNumDigits(char *str, BOOL isLeft);
	void removeLeadZeroes(char *str, zRange &pos);
	void removeTrailZeroes(char *str, zRange &pos);
	void putInChar(char *str, int pos, char c);
	void insertChars(char *str, zRange &pos, char *c, int count);
	int getDeleteCnt(char *str, zRange pos);
	int deleteRange(char *str, int start, int cnt, BOOL noZero);
	void takeOutChars(char *str, int start, int cnt);
	int addZeroes(char *str);
	void addZero(char *str);
	void deleteChars(char *str, zRange &pos, int cnt);
	void adjustPos(char *str, zRange &pos, BOOL isLeft);
	int checkRange(char *str, zRange &pos);
	void stripSignCurrency(char *str, zRange &pos, BOOL remTrail);
	void stripThousandSep(char *str);
	void prepend(char *str, char *insStr, BOOL hasSpace);
	void append(char *str, char *insStr, BOOL hasSpace);
	void addCurSymbol(char *str, zRange &pos);
	void removeCurSymbol(char *str, zRange &pos);
	void adjustCurPos(zRange &pos, int delLeft, int delRight, int len);
	void removeMinusSymbol(char *str, zRange &pos);
	void removePlusSymbol(char *str, zRange &pos);
	void addMinusSymbol(char *str, zRange &pos);
	void addPlusSymbol(char *str, zRange &pos);
	void removePrefix(char *str, int cnt);
	void removeSuffix(char *str, int cnt);
private:
	zNumPictStrEl *numList[NUMFLDMAXLEN];
	zNumReadOnly rdOnly;   // what degree of read only desired
	char numLeftGuide, numRightGuide; // guide chars. used space for none
	char numDecimalSep, numThousandSep;
	
	int	numDecPos,			// pos. of dec. pt w/ respect to elList
		numListCnt,			// number of elements in num. pic. string list
		numLeft,			// # of digit positions in left side
		numRight,			// for the right side
		
		thouSubList[NUMFLDMAXLEN],	// commas deleted and their positions
	    thouSubCnt,					// current number of commas deleted
		thouAddList[NUMFLDMAXLEN],	// commas added and their positions
	    thouAddCnt,					// current number of commas added
		numCalled;					// for distinguishing initial value
	
	BOOL isRealNum,			// does picture string represent a real?
		 isRightThousand,
		
		 isLeftGuide,
		 isRightGuide,
		 isCurrency,		// does pic. string have currency formatter?
		 isFormatGuide,		// do we need to format guide characters?
		 isSigned,		    // is number signed ?
		 isMinus,			// sign of number
		 isShowMinus,		// should we show it?
		 isShowPlus,		// ditto
		 isReadOnly;
};

inline int zNumFormatter::getFracDigits() const { return numRight; }



class Z_DLLSPEC zFormattedEdit : public zEditLine {
public:
	zFormattedEdit(zWindow*,int id,zFormatter *,unsigned long fieldFlags_=0);
	zFormattedEdit(zWindow *, zSizer *, unsigned long, const char *,
			int, zFormatter *, unsigned long fieldFlags_=0);
	~zFormattedEdit();

	int pasteStr(RWCString &str);
	RWCString getRawText(unsigned long options);
	zFormatter* getFormatter() const;
	void setFormatter(zFormatter *fmter);

	virtual int checkData();

private:
	int isClipboardAction;
protected:
	unsigned long _fieldFlags;
	zFormatter *format;
public:
	virtual int ch(zKeyEvt *);
	int keyDown(zEvent *);
	int cut(zEvent *);
	int paste(zEvent *);
};

inline zFormatter* zFormattedEdit::getFormatter() const { return format; }


class Z_DLLSPEC zFormDialog : public zDialogFrame {
public:
	zFormDialog(zWindow*, const zResId &, unsigned long = 0);
	zFormDialog(zWindow*,zSizer*,unsigned long =0,const char* =0);
	~zFormDialog();
	BOOL& completed();

	int storeData();
	int setControlsToDefault();

private:
	unsigned long  _flags;
	BOOL           _completed;
	zPushButton   *okButton_;
	zPushButton   *cancelButton_;

public:
	virtual void show(int=SW_SHOWNORMAL);
	int endOk(zEvent *);
	int endCancel(zEvent *);
};

inline BOOL& zFormDialog::completed() { return _completed; }


class Z_DLLSPEC zStringEdit : public zFormattedEdit {
public:
	zStringEdit(zWindow*,int id_,RWCString *dest_,const char *pictStr_,
		unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,int id_,RWCString *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,int id_,RWCString *dest_=0,unsigned long fFlags_=0);
    
	zStringEdit(zWindow*,zSizer *,unsigned long,int id_, RWCString *dest_,
		const char *pictStr_,unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,zSizer *,unsigned long,int id_, RWCString *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,zSizer *,unsigned long,int id_, RWCString *dest_,
		unsigned long fieldFlags_=0);


	zStringEdit(zWindow*,int id_,zString *dest_,const char *pictStr_,
		unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,int id_,zString *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,int id_,zString *dest_=0,unsigned long fFlags_=0);
    
	zStringEdit(zWindow*,zSizer *,unsigned long,int id_, zString *dest_,
		const char *pictStr_,unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,zSizer *,unsigned long,int id_, zString *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zStringEdit(zWindow*,zSizer *,unsigned long,int id_, zString *dest_,
		unsigned long fieldFlags_=0);
	
	virtual int setToDefault();
	virtual int storeData();
	RWCString* setStoreAddr(RWCString *);
	RWCString* setStoreAddr(zString *);
protected:
	RWCString* _dest;
};

class Z_DLLSPEC zIntEdit : public zFormattedEdit {
public:
	zIntEdit(zWindow*,int id_,int *dest_,const char *pictStr_,
		unsigned long fieldFlags_ = 0);
	zIntEdit(zWindow*,int id_,int *dest_,zFormatter *formatter_,
		unsigned long fieldFlags_= 0);
	zIntEdit(zWindow*, int id_, int *dest_=0, unsigned long fieldFlags_= 0);

	zIntEdit(zWindow*,zSizer *,unsigned long,int id_, int *dest_,
		const char *pictStr_,unsigned long fieldFlags_=0);
	zIntEdit(zWindow*,zSizer *,unsigned long,int id_, int *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zIntEdit(zWindow*,zSizer *,unsigned long,int id_, int *dest_,
		unsigned long fieldFlags_=0);
	
	virtual int setToDefault();
	virtual int storeData();
	int* setStoreAddr(int *);
protected:
	int* _dest;
};

class Z_DLLSPEC zUnsignedIntEdit : public zFormattedEdit {
public:
	zUnsignedIntEdit(zWindow*,int id_,unsigned int *dest_,
		const char *pictStr_,unsigned long fieldFlags_ = 0);
	zUnsignedIntEdit(zWindow*,int id_,unsigned int *dest_,
		zFormatter *formatter_,	unsigned long fieldFlags_= 0);
	zUnsignedIntEdit(zWindow*,int id_,unsigned int *dest_=0,
		unsigned long fieldFlags_= 0);

	zUnsignedIntEdit(zWindow*,zSizer *,unsigned long,int id_,
		unsigned int *dest_, const char *pictStr_,unsigned long fieldFlags_=0);
	zUnsignedIntEdit(zWindow*,zSizer *,unsigned long,int id_,
		unsigned int *dest_,zFormatter *formatter_,unsigned long fieldFlags_=0);
	zUnsignedIntEdit(zWindow*,zSizer *,unsigned long,int id_,
		unsigned int *dest_, unsigned long fieldFlags_=0);

	virtual int setToDefault();
	virtual int storeData();
	unsigned int* setStoreAddr(unsigned int *);
protected:
	unsigned int* _dest;
};

class Z_DLLSPEC zLongEdit : public zFormattedEdit {
public:
    zLongEdit(zWindow*,int id_,long *dest_,const char *pictStr_,
	      unsigned long fieldFlags_ = 0);
    zLongEdit(zWindow*,int id_,long *dest_,zFormatter *formatter_,
	      unsigned long fieldFlags_= 0);
    zLongEdit(zWindow*, int id_, long *dest_=0, unsigned long fieldFlags_= 0);
    
    zLongEdit(zWindow*,zSizer *,unsigned long,int id_, long *dest_,
	      const char *pictStr_,unsigned long fieldFlags_=0);
    zLongEdit(zWindow*,zSizer *,unsigned long,int id_, long *dest_,
	      zFormatter *formatter_,unsigned long fieldFlags_=0);
    zLongEdit(zWindow*,zSizer *,unsigned long,int id_, long *dest_,
	      unsigned long fieldFlags_=0);
    virtual int setToDefault();
    virtual int storeData();
    long* setStoreAddr(long *);
protected:
	long* _dest;
};

class Z_DLLSPEC zUnsignedLongEdit : public zFormattedEdit {
public:
	zUnsignedLongEdit(zWindow*, int id_,unsigned long *dest_,
		const char *pictStr_,unsigned long fieldFlags_ = 0);
	zUnsignedLongEdit(zWindow*, int id_, unsigned long *dest_,
		zFormatter *formatter_,	unsigned long fieldFlags_= 0);
	zUnsignedLongEdit(zWindow*,int id_,unsigned long *dest_=0,
		unsigned long fieldFlags_= 0);

	zUnsignedLongEdit(zWindow*,zSizer *,unsigned long,int id_,
		unsigned long *dest_,const char *pictStr_,unsigned long fieldFlags_=0);
	zUnsignedLongEdit(zWindow*,zSizer *,unsigned long,int id_,
		unsigned long *dest_,zFormatter *formatter_,unsigned long fieldFlags_=0);
	zUnsignedLongEdit(zWindow*,zSizer *,unsigned long,int id_,
		unsigned long *dest_,unsigned long fieldFlags_=0);

	virtual int setToDefault();
	virtual int storeData();
	unsigned long* setStoreAddr(unsigned long *);
protected:
	unsigned long* _dest;
};

class Z_DLLSPEC zFloatEdit : public zFormattedEdit {
public:
	zFloatEdit(zWindow*,int id_,float *dest_,const char *pictStr_,
		unsigned long fieldFlags_ = 0);
	zFloatEdit(zWindow*, int id_, float *dest_,zFormatter *formatter_,
		unsigned long fieldFlags_= 0);
	zFloatEdit(zWindow*, int id_, float *dest_=0,unsigned long fieldFlags_= 0);

	zFloatEdit(zWindow*,zSizer *,unsigned long,int id_, float *dest_,
		const char *pictStr_,unsigned long fieldFlags_=0);
	zFloatEdit(zWindow*,zSizer *,unsigned long,int id_, float *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zFloatEdit(zWindow*,zSizer *,unsigned long,int id_, float *dest_,
		unsigned long fieldFlags_=0);

	virtual int setToDefault();
	virtual int storeData();
	float* setStoreAddr(float *);
protected:
	float* _dest;
};

class Z_DLLSPEC zDoubleEdit : public zFormattedEdit {
public:
	zDoubleEdit(zWindow*, int id_, double *dest_, const char *pictStr_,
			unsigned long fieldFlags_ = 0);
	zDoubleEdit(zWindow*, int id_, double *dest_, zFormatter *formatter_,
		unsigned long fieldFlags_= 0);
	zDoubleEdit(zWindow*,int id_,double *dest_ =0,unsigned long fieldFlags_= 0);
	zDoubleEdit(zWindow*,zSizer *,unsigned long,int id_, double *dest_,
		const char *pictStr_,unsigned long fieldFlags_=0);
	zDoubleEdit(zWindow*,zSizer *,unsigned long,int id_, double *dest_,
		zFormatter *formatter_,unsigned long fieldFlags_=0);
	zDoubleEdit(zWindow*,zSizer *,unsigned long,int id_, double *dest_,
		unsigned long fieldFlags_=0);

	virtual int setToDefault();
	virtual int storeData();
	double* setStoreAddr(double *);
protected:
	double* _dest;
};

class Z_DLLSPEC zRadioGroup : public zRadioButton {
	zEvH *clickObj;
	NotifyProc clickProc;
public:
	zRadioGroup(zWindow*, int idLo_, int idHi_);
	zRadioGroup(zWindow*, int idLo_, int idHi_,int *dest_,
		unsigned long fieldFlags_=0);
	virtual ~zRadioGroup();

	void setNotifyClicked(zEvH *obj,NotifyProc np) {
		clickObj = obj; clickProc = np;
	}
	void setNotifyClicked(zEvH *obj,ClickProc np) {
		clickObj = obj; clickProc = (NotifyProc)np;
	}
	void setNotifyClicked(zEvH *obj,CommandProc np) {
	    clickObj = obj; clickProc = (NotifyProc)np;
	}
	virtual int setToDefault();
	virtual int checkData();
	virtual int storeData();
	void setStoreAddr(int *);
protected:
	int *_dest;
	unsigned long _fieldFlags;
	int _idLo;
	int _idHi;
	zRadioButton *cur;
	void setupClickProcessing();
	void removeClickProcessing();
	int processClicks(zEvent *ce);
	virtual void ziBackgroundColor(const zColor& d);
};

class Z_DLLSPEC zRadioController : public zDevice {
	zEvH *clickObj;
	NotifyProc clickProc;
public:
	zRadioController(zWindow*, int idLo_, int idHi_);
	zRadioController(zWindow*, int idLo_, int idHi_,int *dest_,
		unsigned long fieldFlags_=0);
	virtual ~zRadioController();

	void setNotifyClicked(zEvH *obj,NotifyProc np) {
		clickObj = obj; clickProc = np;
	}
	void setNotifyClicked(zEvH *obj,ClickProc np) {
		clickObj = obj; clickProc = (NotifyProc)np;
	}
	void setNotifyClicked(zEvH *obj,CommandProc np) {
	    clickObj = obj; clickProc = (NotifyProc)np;
	}
	virtual int setToDefault();
	virtual int checkData();
	virtual int storeData();
	void setStoreAddr(int *);

	void backgroundColor(const zColor& d);
	void enableAll(int enable);
protected:
	int *_dest;
	unsigned long _fieldFlags;
	int _idLo;
	int _idHi;
	zRadioButton *cur;
	void setupClickProcessing();
	void removeClickProcessing();
	int processClicks(zEvent *ce);
	zWindow *WndParent;
};

#endif

