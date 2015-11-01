#ifndef _ZAPPZSLIST_INCLUDED
#define _ZAPPZSLIST_INCLUDED 1
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

class Z_DLLSPEC zSlink;

class Z_DLLSPEC zSlink {
	friend class Z_DLLSPEC zSlist;
	friend class Z_DLLSPEC zSlistIter;
	
	zSlink* n;
	char* o;
	
	zSlink(void *obj) { o =(char*) obj; n=0; }
	~zSlink() {}
};

class Z_DLLSPEC zSlistIter;

class Z_DLLSPEC zSlist {
	friend class zSlistIter;
	
	zSlink* last;
protected:
	DWORD count;
	int atHead(void *);
	int atTail(void *);
	void *remHead();
	void *head();
	void *tail();
	zSlist(void *);
public:
	zSlist() { count=0; last = 0; }
	int join(zSlist *);
	virtual void io(zArchive&);
	~zSlist();
};

class Z_DLLSPEC zSlistIter {
	zSlink* end;
	zSlink* cur;
protected:
	friend class zSlist;
	void *getNext();
	zSlistIter(zSlist *p) { end = cur = p->last; }
public:
	void *getItemNum(int);
	void reset() { cur = end; }
};
	
#define OSLITER(ETYPE) ETYPE##SlistIter
#define OSLIST(ETYPE) ETYPE##Slist
#define OSTACK(ETYPE) ETYPE##Stack
#define OSTACKITER(ETYPE) ETYPE##StackItr
#define OQUEUE(ETYPE) ETYPE##Queue
#define OQUEUEITER(ETYPE) ETYPE##QueueItr


#ifdef __WATCOMC__

#define declSlist(ETYPE)									\
class Z_DLLSPEC OSLIST(ETYPE):public zSlist {							\
public:														\
	OSLIST(ETYPE)(){}										\
	OSLIST(ETYPE)(ETYPE *p):zSlist((void *)p){}				\
	int insert(ETYPE *p) { return atHead((void *)p); }			\
	int append(ETYPE *p) { return atTail((void *)p); }			\
	ETYPE *remove() { return (ETYPE *)zSlist::remHead();}	\
	ETYPE *front()	{ return (ETYPE *)head(); }				\
	ETYPE *end()	{ return (ETYPE *)tail(); }				\
	ETYPE *first()	{ return (ETYPE *)head(); }				\
	ETYPE *last()	{ return (ETYPE *)tail(); }				\
	DWORD length() { return count; }						\
};															\
															\
class Z_DLLSPEC OSLITER(ETYPE):public zSlistIter {					\
public:														\
	OSLITER(ETYPE)( OSLIST(ETYPE)* p):zSlistIter((zSlist *)p) {}\
	ETYPE * operator()(){									\
		return (ETYPE *)zSlistIter::getNext();			\
	}														\
};																\



#define declStack(ETYPE)									\
class Z_DLLSPEC OSTACK(ETYPE) : public zSlist {						\
public:														\
	OSTACK(ETYPE)()	{}										\
	OSTACK(ETYPE)(ETYPE *p):zSlist((void *)p)	{}					\
	int push(ETYPE *p)	{ return atHead((void *)p); }			\
	ETYPE *pop()	{ return (ETYPE *)remHead(); }			\
	ETYPE *top()	{ return (ETYPE *)head(); }				\
	DWORD height()	{ return count; }						\
};															\
class Z_DLLSPEC OSTACKITER(ETYPE) : public zSlistIter {			\
public:														\
	OSTACKITER(ETYPE)(OSTACK(ETYPE)* p):zSlistIter((zSlist *)p){}	\
	ETYPE * operator()(){									\
		return (ETYPE *)zSlistIter::getNext();			\
	}														\
};																\


		

#define declQueue(ETYPE)									\
class Z_DLLSPEC OQUEUE(ETYPE) : public zSlist {						\
public:														\
	OQUEUE(ETYPE)()	{}										\
	OQUEUE(ETYPE)(ETYPE *p):zSlist((void *)p)	{}					\
	int enqueue(ETYPE *p)	{ return atTail((void *)p); }		\
	ETYPE *dequeue()	{ return (ETYPE *)remHead(); }		\
	ETYPE *front()		{ return (ETYPE *)head(); }			\
	ETYPE *end()		{ return (ETYPE *)tail(); }			\
	DWORD length()		{ return count; }					\
};															\
class Z_DLLSPEC OQUEUEITER(ETYPE) : public zSlistIter {			\
public:														\
	OQUEUEITER(ETYPE)(OQUEUE(ETYPE)* p):zSlistIter((zSlist *)p){}		\
	ETYPE * operator()(){									\
		return (ETYPE *)zSlistIter::getNext();			\
	}														\
};																

#else

#define declSlist(ETYPE)									\
class Z_DLLSPEC OSLIST(ETYPE):public zSlist {							\
public:														\
	OSLIST(ETYPE)(){}										\
	OSLIST(ETYPE)(ETYPE *p):zSlist((void *)p){}				\
	int insert(ETYPE *p) { return atHead((void *)p); }			\
	int append(ETYPE *p) { return atTail((void *)p); }			\
	ETYPE *remove() { return (ETYPE *)zSlist::remHead();}	\
	ETYPE *front()	{ return (ETYPE *)head(); }				\
	ETYPE *end()	{ return (ETYPE *)tail(); }				\
	ETYPE *first()	{ return (ETYPE *)head(); }				\
	ETYPE *last()	{ return (ETYPE *)tail(); }				\
	DWORD length() { return count; }						\
};															\
															\
class Z_DLLSPEC OSLITER(ETYPE):public zSlistIter {					\
public:														\
	OSLITER(ETYPE)( OSLIST(ETYPE)* p):zSlistIter((zSlist *)p) {}\
	ETYPE * operator()(){									\
		return (ETYPE *)zSlistIter::getNext();			\
	}														\
};																\
inline zArchive& operator<<(zArchive& arc,OSLIST(ETYPE)& lst) {	\
	lst.io(arc);												\
    return arc;													\
}																\
inline zArchive& operator>>(zArchive& arc,OSLIST(ETYPE)& lst) {	\
	lst.io(arc);												\
	return arc;													\
}																\


#define declStack(ETYPE)									\
class Z_DLLSPEC OSTACK(ETYPE) : public zSlist {						\
public:														\
	OSTACK(ETYPE)()	{}										\
	OSTACK(ETYPE)(ETYPE *p):zSlist((void *)p)	{}					\
	int push(ETYPE *p)	{ return atHead((void *)p); }			\
	ETYPE *pop()	{ return (ETYPE *)remHead(); }			\
	ETYPE *top()	{ return (ETYPE *)head(); }				\
	DWORD height()	{ return count; }						\
};															\
class Z_DLLSPEC OSTACKITER(ETYPE) : public zSlistIter {			\
public:														\
	OSTACKITER(ETYPE)(OSTACK(ETYPE)* p):zSlistIter((zSlist *)p){}	\
	ETYPE * operator()(){									\
		return (ETYPE *)zSlistIter::getNext();			\
	}														\
};																\
inline zArchive& operator<<(zArchive& arc,OSTACK(ETYPE)& lst) {	\
	lst.io(arc);												\
    return arc;													\
}																\
inline zArchive& operator>>(zArchive& arc,OSTACK(ETYPE)& lst) {	\
	lst.io(arc);												\
	return arc;													\
}																\


#define declQueue(ETYPE)									\
class Z_DLLSPEC OQUEUE(ETYPE) : public zSlist {						\
public:														\
	OQUEUE(ETYPE)()	{}										\
	OQUEUE(ETYPE)(ETYPE *p):zSlist((void *)p)	{}					\
	int enqueue(ETYPE *p)	{ return atTail((void *)p); }		\
	ETYPE *dequeue()	{ return (ETYPE *)remHead(); }		\
	ETYPE *front()		{ return (ETYPE *)head(); }			\
	ETYPE *end()		{ return (ETYPE *)tail(); }			\
	DWORD length()		{ return count; }					\
};															\
class Z_DLLSPEC OQUEUEITER(ETYPE) : public zSlistIter {			\
public:														\
	OQUEUEITER(ETYPE)(OQUEUE(ETYPE)* p):zSlistIter((zSlist *)p){}		\
	ETYPE * operator()(){									\
		return (ETYPE *)zSlistIter::getNext();			\
	}														\
};																\
inline zArchive& operator<<(zArchive& arc,OQUEUE(ETYPE)& lst) {	\
	lst.io(arc);												\
    return arc;													\
}																\
inline zArchive& operator>>(zArchive& arc,OQUEUE(ETYPE)& lst) {	\
	lst.io(arc);												\
	return arc;													\
}																\

#endif

#endif

