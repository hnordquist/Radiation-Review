#ifndef _ZAPPDLIST_INCLUDED
#define _ZAPPDLIST_INCLUDED 1
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



class Z_DLLSPEC zDListNode {
	zDListNode *_next;
	zDListNode *_prev;
protected:
	void *_obj;
	
	friend class Z_DLLSPEC zDblList;
	friend class Z_DLLSPEC zDListIter;
	inline zDListNode();
	inline zDListNode(const void *);
#ifndef __IBMCPP__
	virtual
#endif
			~zDListNode();
	
	void insert(zDListNode *);
	inline void append(zDListNode *newNode);
	inline void insert(const void *);
	inline void append(const void *);
public:
	inline zDListNode *next() const;
	inline zDListNode *prev() const;
	inline zDListNode *next(const zDListNode *n);
	inline zDListNode *prev(const zDListNode *p);
	inline void *obj() const;
	inline void detach();
	inline void removeNext();
	inline void removePrev();
};


class Z_DLLSPEC zDListIter;	

class Z_DLLSPEC zDblList : public zDListNode {
	friend class zDListIter;
	inline zDblList(const zDblList &d);
	inline zDblList& operator=(const zDblList& in);
protected:
    long _length;
    
    inline void insert(const void *obj_);
    inline void append(const void *obj_);
    inline void insert(zDListNode *newNode);
    inline void append(zDListNode *newNode);
    void *nthNode(const int index_);
    inline void *first();
    inline void *last();
public:	
	inline zDblList();
#ifndef __IBMCPP__
	virtual
#endif
			~zDblList();
	inline int isEmpty();
	inline void removeFirst();
	inline void removeLast();
	inline long length();
	void removeAll(int doDeleteObj);
	int findAndRemove(const void *p);
};



class Z_DLLSPEC zDListIter {
protected:
	zDblList *_list;
	zDListNode *_curNode;
	int _direction;

	friend class zDblList;
	inline void insert(zDListNode *p);
	inline void append(zDListNode *p);
	inline void insert(const void *p);
	inline void append(const void *p);
	
	inline void* operator++(int);
	inline void* operator--(int);
	inline void* operator++();
	inline void* operator--();
	
	inline void* value();
public:
	inline zDListIter();
	inline zDListIter(const zDblList *list);
#ifndef __IBMCPP__
	virtual
#endif
			~zDListIter();
	void remove();
	inline void reset();
	inline int isFirst();
	inline int isLast();
	inline int isNowhere();
	inline void setDirForward();
	inline void setDirReverse();
	inline int isForward();
	inline zDListNode *curNode();
	inline void * operator()();
	int jumpTo(int index_);
	zDListIter& find(const void *);
};

//////////////////////////////////////////////////////////////////////////

#ifdef __IBMCPP__
#define TEMPLATE
#endif

#ifdef TEMPLATE

#define DNODE(ETYPE) ETYPE##DListNode
#define DITER(ETYPE) ETYPE##DListIter
#define DLIST(ETYPE) ETYPE##DList


#define declDList(ETYPE) \
typedef zTDListNode<ETYPE> DNODE(ETYPE);\
typedef zTDListIter<ETYPE> DITER(ETYPE);\
typedef zTDList<ETYPE> DLIST(ETYPE);


template<class T>
class Z_DLLSPEC zTDListNode:private zDListNode {
public:
	zTDListNode(T* obj=0):zDListNode((void *)obj) {}
	void insert(const T* obj)	{ zDListNode::insert((const void *)obj); }
	void append(const T* obj)	{ zDListNode::append((const void *)obj); }
	
	void detach()				{ zDListNode::detach(); }
	void removeNext()			{ zDListNode::removeNext(); }
	void removePrev()			{ zDListNode::removePrev(); }

	zTDListNode *next() const   { return (zTDListNode *) zDListNode::next(); }
	zTDListNode *prev() const   { return (zTDListNode *) zDListNode::prev(); }
	zTDListNode<T> *next(const zTDListNode<T> *n) {
		return (zTDListNode<T> *) zDListNode::next((const zDListNode *) n); }
	zTDListNode<T> *prev(const zTDListNode *p) {
		return (zTDListNode<T> *) zDListNode::prev((const zDListNode *) p); }
	T *obj() const				{ return (T *) zDListNode::obj(); }
};


template<class T>
class Z_DLLSPEC zTDList:private zDblList {
public:
	zTDList() : zDblList()		{ }
	void insert(const T* obj) { zDblList::insert((const void *) obj); }
	void append(const T* obj)	{ zDblList::append((const void *) obj) ;}
	T& operator[](const int index) { return *((T *) zDblList::nthNode(index)); }
	
	int isEmpty()					{ return zDblList::isEmpty(); }
	long length()					{ return zDblList::length(); }
	
	void removeFirst()				{ zDblList::removeFirst(); }
	void removeLast()				{ zDblList::removeLast(); }
	T* first()						{ return (T *) zDblList::first(); }
	T* last()						{ return (T *) zDblList::last(); }
	void removeAll()		        { zDblList::removeAll(0); }
	void removeAndDeleteAll()       { zDblList::removeAll(1); }
	int findAndRemove(const void *p) { return zDblList::findAndRemove(p); }
};

template<class T>
class Z_DLLSPEC zTDListIter:private zDListIter {
public:
	zTDListIter(const zTDList<T> *s):zDListIter( (const zDblList *) s) { }
	T* operator()(void)		{ return (T*)zDListIter::operator()();  }
	T* operator++(int i)	{ return(T *)zDListIter::operator++(i); }
	T* operator--(int i)	{ return(T *)zDListIter::operator--(i); }
	T* operator++()			{ return (T *)zDListIter::operator++(); }
	T* operator--()			{ return (T *)zDListIter::operator--(); }
	T* value()				{ return (T *)zDListIter::value(); }
	void insert(const T *p)	{ zDListIter::insert((const void *)p); }
	void append(const T *p)	{ zDListIter::append((const void *)p); }
	
	void remove()			{ zDListIter::remove(); }
	void reset()			{ zDListIter::reset(); }
	void setDirForward()	{ zDListIter::setDirForward(); }
	void setDirReverse()	{ zDListIter::setDirReverse(); }
	
	int isFirst()			{ return zDListIter::isFirst(); }
	int isLast()			{ return zDListIter::isLast(); }
	int isNowhere()			{ return zDListIter::isNowhere(); }
	int isForward()			{ return zDListIter::isForward(); }
	int jumpTo(int index)	{ return zDListIter::jumpTo(index); }
	
	zTDListNode<T> *curNode() {
		return (zTDListNode<T> *) zDListIter::curNode();
	}
	zTDListIter<T>& find(const void *p) { return (zTDListIter<T> &) zDListIter::find(p);}

};


#else



	
#define DNODE(ETYPE) ETYPE##DListNode
#define DITER(ETYPE) ETYPE##DListIter
#define DLIST(ETYPE) ETYPE##DList


#define declDList(ETYPE)                                             \
class Z_DLLSPEC DNODE(ETYPE):public zDListNode {                               \
public:                                                               \
	DNODE(ETYPE)(const ETYPE* obj_=0):zDListNode((const void *)obj_)	{}        \
	void insert(const ETYPE* obj_)	{ zDListNode::insert((const void *)obj_); }  \
	void append(const ETYPE* obj_)	{ zDListNode::append((const void *)obj_); }  \
	void detach()				{ zDListNode::detach(); }\
	void removeNext()			{ zDListNode::removeNext(); }\
	void removePrev()			{ zDListNode::removePrev(); }\
	\
	DNODE(ETYPE) *next() const   { return (DNODE(ETYPE) *) zDListNode::next(); }\
	DNODE(ETYPE) *prev() const   { return (DNODE(ETYPE) *) zDListNode::prev(); }\
	DNODE(ETYPE) *next(const DNODE(ETYPE) *n) {\
		return (DNODE(ETYPE) *) zDListNode::next((const zDListNode *) n); }\
	DNODE(ETYPE) *prev(const DNODE(ETYPE) *p) {\
		return (DNODE(ETYPE) *) zDListNode::prev((const zDListNode *) p); }\
	ETYPE *obj() const				{ return (ETYPE *) zDListNode::obj(); }\
};                                                                    \
                                                                      \
                                                                      \
class Z_DLLSPEC DLIST(ETYPE):public zDblList {                                  \
public:                                                               \
	DLIST(ETYPE)()			{ }                                   \
	void insert(const ETYPE* obj_)	{ zDblList::insert((const void *) obj_); }     \
	void append(const ETYPE* obj_)	{ zDblList::append((const void *) obj_) ;}     \
	ETYPE& operator[](const int index_) { return *((ETYPE *) zDblList::nthNode(index_)); }\
	\
	int isEmpty()					{ return zDblList::isEmpty(); }\
	long length()					{ return zDblList::length(); }\
	\
	void removeFirst()				{ zDblList::removeFirst(); }\
	void removeLast()				{ zDblList::removeLast(); }\
	ETYPE * first()					{ return (ETYPE *) zDblList::first(); }\
	ETYPE * last()					{ return (ETYPE *) zDblList::last(); }\
	void removeAll()		        { zDblList::removeAll(0); }\
	void removeAndDeleteAll()       { zDblList::removeAll(1); }\
	int findAndRemove(const ETYPE *p) { return zDblList::findAndRemove(p); }\
};                                                                    \
                                                                      \
class Z_DLLSPEC DITER(ETYPE):public zDListIter {                               \
public:                                                               \
	DITER(ETYPE)(const DLIST(ETYPE) *s):zDListIter(s) { }                \
	ETYPE* operator()(void)	{ return (ETYPE*)zDListIter::operator()(); } \
	ETYPE* operator++(int i)	{ return (ETYPE *)zDListIter::operator++(i);}\
	ETYPE* operator--(int i)	{ return (ETYPE *)zDListIter::operator--(i);}\
	ETYPE* operator++()		{ return (ETYPE *)zDListIter::operator++(); }\
	ETYPE* operator--()		{ return (ETYPE *)zDListIter::operator--(); }\
	ETYPE* value()				{ return (ETYPE *)zDListIter::value(); }     \
	void insert(const ETYPE *p)		{ zDListIter::insert((const void *)p); }            \
	void append(const ETYPE *p)		{ zDListIter::append((const void *)p); }            \
	void remove()			{ zDListIter::remove(); }\
	void reset()			{ zDListIter::reset(); }\
	void setDirForward()	{ zDListIter::setDirForward(); }\
	void setDirReverse()	{ zDListIter::setDirReverse(); }\
	\
	int isFirst()			{ return zDListIter::isFirst(); }\
	int isLast()			{ return zDListIter::isLast(); }\
	int isForward()			{ return zDListIter::isForward(); }\
	int jumpTo(int index_)	{ return zDListIter::jumpTo(index_); }\
	\
	DNODE(ETYPE) *curNode() {\
		return (DNODE(ETYPE) *) zDListIter::curNode();\
	}\
	DITER(ETYPE)& find(const void *p) { return (DITER(ETYPE) &) zDListIter::find(p);}\
};

#endif

#include <zapph/dlist.inl>


#endif

