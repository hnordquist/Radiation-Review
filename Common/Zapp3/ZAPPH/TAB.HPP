#ifndef _ZAPPTAB_INCLUDED
#define _ZAPPTAB_INCLUDED 1
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

# if defined(WIN32) && defined(Z_NATIVE_NBD)
# include "commctrl.h"
# endif

// tab style defines
#define Z_TABTRAPEZOID 0x00000004L

#define Z_TABNOPREV  -1		// No previously selected Tab

class Z_DLLSPEC zTab;
class Z_DLLSPEC zNotebookDialog;
class Z_DLLSPEC zWizardDialog;
class Z_DLLSPEC zPageDef;
class Z_DLLSPEC zTabCtrl;
class Z_DLLSPEC ziTabNoteCtrl;


declDList(zTab)


class Z_DLLSPEC zTabCtrl:public zPaneControl {
public:
	zTabCtrl(zWindow*,zSizer*,unsigned long style_=0,const char* =0, int ctrlId=0);
	zTabCtrl(zWindow*,int ctrlId_);
	~zTabCtrl();

	void appendTab(zTab *tab);
	void createAndAppendTabs(char **,int *,unsigned int number);
	void createAndAppendTabs(zResStrBlock&);

	void removeTab(zTab *tab);
	void removeTab(int indx);
			
	void select(zTab*);
	void select(int newindx);

	void resetTabsPos();
	zTab* getTabInList(int posInList);

	void changeTabText(int indx_,const char*);
	void changeTabText(int indx_,const RWCString&);

	void setupMargins(int left,int top,int right,int bottom);

	virtual void onPageChange(int newindx, int oldindx);
	virtual int onAllowPageChange(int newindx, int oldindx);

	virtual int setToDefault();
	virtual int storeData();
	void setStoreAddr(int* tabindx);
	void setStoreAddr(zTab*);

	void setNotifySelChange(zEvH *obj,NotifyProc np);
	void removeNotifySelChange(zEvH *,NotifyProc);

	int isTrapezoidal() const;

	zTab* selectedTab() const;
	
protected:
	zSizer *botSpace,*topSpace,*leftSpace,*rightSpace;

	zGravSizer *tabAreaSizer, *tabsSizer,*scrollAreaSizer;
	zPercentSizer *scrollSubAreaSizer;

	zTab* lastVisTab();

	void drawSquareTop(zRect& dirtyRect, zRect &visRect);
	void drawTrapBottom(zRect& dirtyRect, zRect &visRect);

	int doTabsFit();
private:
	zTabDList *tabList;
	zTab *selTab;
	zRect tabsRect;
	int leftTabNumber;
	unsigned long _tabStyle;
	int trapOverlap;
	int tabsStartPos;
	void *_dest;
	int _destType;

	void ziSetup();
	int doKeyboard(zEvent*);
public:
	virtual int mouseButtonDown(zMouseClickEvt *);
	virtual void setMetrics();
	virtual int draw(zDrawEvt *de=0);
	int size(zSizeEvt*);
};

declDList(zPageDef)

class Z_DLLSPEC ziTabNoteCtrl:public zTabCtrl {
public:
	ziTabNoteCtrl(zNotebookDialog*,zSizer *,unsigned long style_ =0);
	ziTabNoteCtrl(zWizardDialog*,zSizer *,unsigned long style_ =0);
	~ziTabNoteCtrl();
	
	zPageDef* add(zPageDef*, zTab* =0);
	void remove(zPageDef*);
	
	void select(zPageDef*);
	void select(zTab*t) { zTabCtrl::select(t); }

	int selectNext();
	int selectPrev();
	void selectFirst();

	zPageDef* find(int id);
	zPageDefDList* kids() { return &pageList; }
	zPageDef* selected() const { return cur; }

	virtual int createPage(zPageDef*);
	virtual void onPageChange(int newIndex, int oldIndex);

	int isWizard() const;
private:
	zNotebookDialog *nbDlg_;
	zPageDef *first,*cur;
	zPageDefDList pageList;
	int _isWiz;
public:
	virtual int command(zCommandEvt*);
	virtual int storeData();
	int ch(zKeyEvt*);
	int focus(zFocusEvt*ev);
	long dispatch(zEvent*);
	int ziKeyDown(zEvent*);
	virtual int draw(zDrawEvt *de=0);
	virtual int mouseButtonDown(zMouseClickEvt*);
};


class Z_DLLSPEC zTab {
public:
	zTab(zTabCtrl *tc,int ind);
	zTab(zTabCtrl *tc,int ind, const RWCString& tabString);
	~zTab();
	
	zPoint& position();
	zDimension& dimension();
	zRect getRect() const;

	int isDirty() const;
	void setDirty();

	void setSelection(int sel=1);
	int isSelected() const;

	int indx() const;

	void setText(const char*);
	void setText(const RWCString&);
	const RWCString& text() const;

	void setTabCtrlOwner(zTabCtrl *tc);
	virtual void setDimFromString();

	void draw();
protected:
	virtual void drawTab();
	virtual void drawTabContents();
	zDisplay* canvas();
private:
	zPoint _pos;
	zDimension _dim;
	zTabCtrl *ownerTC;
	RWCString tabStr;
	int _indx;
	int selected;
	int _isDirty;
};

class Z_DLLSPEC zDialogPage:public zPaneControl {
public:
	zDialogPage(zNotebookDialog*,zPageDef* =0,unsigned long style_ =0);
	zDialogPage(zWizardDialog*,zPageDef* =0,unsigned long style_ =0);
	~zDialogPage();

private:
	void ziHandleTabs(int);
public:
	virtual int invalidData(zWindow*);
	virtual int command(zCommandEvt*);
	int ch(zKeyEvt*);
	int size(zSizeEvt*);
	int focus(zFocusEvt*);
	int mouseButtonDown(zMouseClickEvt*);
	long dispatch(zEvent*);
	int ziKeyDown(zEvent*);
};

class Z_DLLSPEC zPageDef {
public:
	zPageDef(const zResId&,int id);
	zPageDef(zDialogPage*,int id);
	~zPageDef();

	zDialogPage* page() const;
	void setPage(zDialogPage*dp);

	void setAllowApply(int trueFalse);
	int allowApply() const;

	void setCaption(const char*);
	int getId() const { return _id; }
	unsigned long getStyle() const;
	int operator==(const zPageDef& pd )
		{ return ( pd.getId() == _id ) ? 1 : 0; }

protected:
	friend class Z_DLLSPEC zNotebookDialog;
	friend class Z_DLLSPEC zWizardDialog;
	friend class Z_DLLSPEC zDialogPage;
	friend class Z_DLLSPEC ziTabNoteCtrl;
	ziDialogReader *dr;
	int _id;
	zDialogPage* _page;
	int _hasHelp;
	int _allowApply;
	void load(zDialogPage*);
};


#define ZND_ADDHELP	1L
#define ZND_REMOVEAPPLY 2L


class Z_DLLSPEC zNotebookDialog:public zDialogFrame {
public:
	zNotebookDialog(zWindow*,zSizer*,unsigned long sty=0,const char *title_=0);
	zNotebookDialog(zWindow *w,zPageDef* pd,unsigned long d=0,const char*t =0);
	~zNotebookDialog();
	
	zPageDef* add(zPageDef*);
	void remove(zPageDef*);
	
	void select(zPageDef*);
	void setAllowApply(int trueFalse);

	zPageDef* findPage(int id);
	zPageDef* selected() const;

	virtual int createPage(zPageDef*);

	virtual int onCancel(zEvent*);
	virtual int onHelp(zEvent*);
	virtual int onApply(zEvent*);
	virtual int onOk(zEvent*);

	void changeButtonLabels(const char*ok, const char* cancel, const char*apply, const char* help);
	
	zTabCtrl* tabs();
	virtual void create();
private:
	friend class Z_DLLSPEC zDialogPage;  // Workaround for IBM AIX C++

	int ziOnCancel(zEvent*);
	int ziOnHelp(zEvent*);
	int ziOnApply(zEvent*);
	int ziOnOk(zEvent*);

	zFont *noteFont;
	zPushButton *cancelBut,*helpBut,*applyBut,*okBut;
	ziTabNoteCtrl *tab;
	void ziSetup();
	RWCString apply,ok,cancel,help;
	unsigned long _options;
public:
	virtual void onPageChange(zPageDef *curPage, zPageDef *prevPage);
	virtual void show(int =SW_SHOWNORMAL);
	int activate(zActivateEvt*);
	void ziWasSelected(zPageDef*);
};


class Z_DLLSPEC zWizardDialog:public zDialogFrame {
public:
	zWizardDialog(zWindow*,zSizer*,unsigned long sty_ =0,const char *title_=0);
	zWizardDialog(zWindow *w,zPageDef* pd,unsigned long d=0,const char*t =0);
	~zWizardDialog();
	
	zTabCtrl* tabs();
	
	zPageDef* add(zPageDef*);
	void remove(zPageDef*);
	
	void lockPage();
	void unlockPage();
		
	virtual int createPage(zPageDef*);

	zPageDef* findPage(int id);
	zPageDef* selected() const;

	virtual int onBack(zEvent*);
	virtual int onNext(zEvent*);
	virtual int onCancel(zEvent*);
	virtual int onHelp(zEvent*);
	virtual int onFinish(zEvent*);

	void changeButtonLabels(const char*finish, const char* cancel, const char*back, const char* next,const char* help);

	virtual void create();
private:
	friend class Z_DLLSPEC zDialogPage;  // Workaround for IBM AIX C++

	int ziOnBack(zEvent*);
	int ziOnNext(zEvent*);
	int ziOnCancel(zEvent*);
	int ziOnHelp(zEvent*);
	int ziOnFinish(zEvent*);

	zFont *noteFont;
	zPushButton *nextBut,*backBut,*cancelBut,*helpBut;
	ziTabNoteCtrl *tab;
	void ziSetup();
	RWCString fini,prev,next,cancel,help;
	int lockCount;
public:
	virtual void show(int =SW_SHOWNORMAL);
	int activate(zActivateEvt*);
};

# if defined(WIN32) && defined(Z_NATIVE_NBD)
class Z_DLLSPEC zDialogPageWin32;
class Z_DLLSPEC zPageDefWin32;

#ifdef STRICT
#define APIPROC	WNDPROC
#else
#define APIPROC	FARPROC
#endif

class Z_DLLSPEC zNotebookDialogWin32 : public zDialogFrame {

public:
    zNotebookDialogWin32(zWindow*p,zSizer*,ULONG sty=0 ,const char *title_=0);
    zNotebookDialogWin32(zWindow *w,zPageDefWin32* pd,unsigned long d=0,const char*t =0);
    ~zNotebookDialogWin32();
	
    virtual void	create();
    virtual void	show(int sw=SW_SHOWNORMAL);
    zPageDefWin32*	add(zPageDefWin32* pd);
    void		setHWND(HWND hw){ hWnd = hw; }
    virtual int		onCancel(zEvent*);
    virtual int		onOk(zEvent*);
    virtual int		onApply(zEvent*);
    virtual int		onHelp(zEvent*);
    virtual int		createPage(zPageDefWin32*);
    virtual long	dispatch(zEvent*);
    virtual int		command(zCommandEvt*);
    void		setAllowApply(int trueFalse);
    void		remove(int index);
    void		select(zPageDefWin32*);
    void		select(int index);
    void		remove(zPageDefWin32*);
    void		changeButtonLabels(const char *ok, const char *cancel,
					   const char *apply,const char *help)   { }
    zPageDefWin32*	findPage(int id);
    zPageDefWin32*	selected();
    void		tabs(){ }
    void		ziSetCurrentPage(UINT, zEvent*);
    APIPROC		getOrigProc() const { return origProc_; }
    int			amIModal() const { return isModal; }

private:
    void		ziSetup();
    void		ziInsertList();
    int 		ziFindPage(zPageDefWin32*);

    PROPSHEETHEADER		 psHead_;
    PROPSHEETPAGE		*psPages_;
    RWTPtrSlist<zPageDefWin32>	 pageList_;
    zWindow			*parentWin_;
    RWCString			 title_;
    UINT			 currentPage_;
    int				 pageCt_;
    ULONG			 flags_;
    zPageDefWin32		*pageDef_;
    APIPROC			 origProc_;
};


class Z_DLLSPEC zWizardDialogWin32:public zDialogFrame {

public:
    zWizardDialogWin32(zWindow*,zSizer*,unsigned long dStyle=0, const char *dTitle=0);
    zWizardDialogWin32(zWindow *w, zPageDefWin32 *pd, unsigned long dStyle=0, const char *dTitle=0);
    ~zWizardDialogWin32();
	
    zPageDefWin32* add(zPageDefWin32*);
    void           changeButtonLabels(const char *finish, const char *cancel,
			              const char *back, const char *next, const char *help);
    virtual int    command(zCommandEvt*);
    virtual void   create();
    virtual int    createPage(zPageDefWin32*);
    virtual long   dispatch(zEvent*);
    zPageDefWin32* findPage(int id) const;
    void           lockPage();
    virtual int    onBack(zEvent*);
    virtual int    onCancel(zEvent*);
    virtual int    onFinish(zEvent*);
    virtual int    onHelp(zEvent*);
    virtual int    onNext(zEvent*);
    void           remove(zPageDefWin32*);
    zPageDefWin32* selected() const;
    void           setHWND(HWND hw){ hWnd = hw; }
    virtual void   show(int s=SW_SHOWNORMAL);
    zTabCtrl*      tabs() {return 0; }
    void           unlockPage();
    void           ziSetCurrentPage(UINT, zEvent*);
    APIPROC        ziGetOrigProc() const { return origProc_; }
    int            ziAmIModal() const { return isModal; }
//    int            activate(zActivateEvt*);
	
private:
    int             ziFindPage(zPageDefWin32*);
    void            ziInsertList();
    void            ziRemove(int indx);
    void            ziSetup();

    int    lockCount_;
    DWORD  buttonFlags_;
//    zFont  *noteFont;
//    zPushButton *nextBut,*backBut,*cancelBut,*helpBut;
//    ziTabNoteCtrl *tab;
//    RWCString fini,prev,next,cancel,help;
    PROPSHEETHEADER		 psHead_;
    PROPSHEETPAGE		*psPages_;
    RWTPtrSlist<zPageDefWin32>	 pageList_;
    zWindow			*parentWin_;
    RWCString			 title_;
    UINT			 currentPage_;
    int				 pageCt_;
    ULONG			 flags_;
    zPageDefWin32		*pageDef_;
    APIPROC			 origProc_;
};


class Z_DLLSPEC zPageDefWin32 {

public:
    zPageDefWin32(const zResId&,int id);
    zPageDefWin32(zDialogPageWin32*,int id);
    ~zPageDefWin32();

    int operator==(const zPageDefWin32& pd ) { return ( pd.getId() == ctrlId_ ) ? 1 : 0; }
//    const int		id() const { return ctrlId_; }
    zDialogPageWin32*	page() const { return page_; }
    void		setPage(zDialogPageWin32* p) { page_ = p; }
    int			getId() const { return ctrlId_; }
    const zResId&	getResId() const { return resId_; }
    void		setAllowApply( int truefalse ){ allowApply_ = truefalse; }
    int			allowApply() const { return allowApply_; }
    void		setCaption(const char* c){ }	//implement.
    ULONG		getStyle() const { return pdStyle_; }
    void		setStyle(ULONG style){ pdStyle_ = style; }


protected:
    int			hasHelp_;
    int			allowApply_;
    zDialogPageWin32*	page_;

private:
    void		ziCreate();

    RWCString		title_;
    int			ctrlId_;
    zResId		resId_;
    ULONG		pdStyle_;
};


class Z_DLLSPEC zDialogPageWin32 : public zDialogFrame {

public:
    zDialogPageWin32(zNotebookDialogWin32*,zPageDefWin32* =0,unsigned long style_ =0);
    zDialogPageWin32(zWizardDialogWin32*,zPageDefWin32* =0,unsigned long style_ =0);
    ~zDialogPageWin32();

    virtual int		focus(zFocusEvt*);
    //virtual int	invalidData(zWindow*);
    const zResId&	resId() const { return resId_; }
	
    int operator== (const zDialogPageWin32& pd)
	{ return ( pd.resId().id() == resId_.id() ) ? 1 : 0; }

private:
    const HPROPSHEETPAGE	ziHandle_Page() const { return hPage_; }
    void			ziCreate();
    
    zPageDefWin32*	pageDef_;
    PROPSHEETPAGE	psPage_;
    zResId		resId_;
    HPROPSHEETPAGE	hPage_;	
    zWindow*		parentNbk_;
    zWindow*		parentWiz_;
    ULONG		dpStyle_;
};
# endif

inline int ziTabNoteCtrl::isWizard() const {
	return _isWiz;
}

inline zTab* zTabCtrl::getTabInList(int posInList) {
    return &(*tabList)[posInList];
}

inline zDialogPage* zPageDef::page() const {
	return _page;
}
	
inline void zPageDef::setAllowApply(int trueFalse) {
	_allowApply=trueFalse;
}

inline int zPageDef::allowApply() const {
	return _allowApply;
}

inline int zTabCtrl::isTrapezoidal() const {
	return (int)(_tabStyle&Z_TABTRAPEZOID);
}

inline zDisplay* zTab::canvas() {
	return ownerTC->canvas();
}

inline zPoint& zTab::position() {
	return _pos;
}

inline zDimension& zTab::dimension() {
	return _dim;
}

inline zRect zTab::getRect() const {
	return zRect(_pos,_dim);
}

inline int zTab::isDirty() const {
	return _isDirty;
}

inline void zTab::setDirty() {
	_isDirty=1;
}

inline void zTab::setSelection(int sel) {
	selected=sel;
}

inline int zTab::isSelected() const {
	return selected;
}

inline int zTab::indx() const {
	return _indx;
}

inline const RWCString& zTab::text() const {
	return tabStr;
}

inline void zTab::setTabCtrlOwner(zTabCtrl *tc) {
	ownerTC=tc;
}

inline zTabCtrl* zNotebookDialog::tabs() {
	return tab;
}

inline zTabCtrl* zWizardDialog::tabs() {
	return tab;
}

inline zPageDef* zNotebookDialog::findPage(int id) {
	return tab->find(id);
}

inline zPageDef* zNotebookDialog::selected() const {
	return tab->selected();
}

inline zPageDef* zWizardDialog::findPage(int id) {
	return tab->find(id);
}

inline zPageDef* zWizardDialog::selected() const {
	return tab->selected();
}

inline zTab* zTabCtrl::selectedTab() const {
	return selTab;
}



#endif
