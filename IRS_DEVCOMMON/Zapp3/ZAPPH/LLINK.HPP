#ifndef _ZAPPLLINK_INCLUDED
#define _ZAPPLLINK_INCLUDED 1
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


class Z_DLLSPEC zLink;

class Z_DLLSPEC zLink {
protected:
	zLink* next;
	zLink* prev;
	unsigned short refCount;
	
	zLink(zLink*l);
public:
	zLink* Next() { return next; }
	zLink() {
		next=this;
		prev=this;
		refCount=0;
	}
	BOOL remove();
	~zLink();

	void zinsert2() { ZNEWH zLink(this); }
	unsigned short &usage() { return refCount; }
};

class Z_DLLSPEC zDlist:private zLink {
public:
	zDlist();
	zLink* first() { return next; }
	zLink* last() { return prev; }
};

#define OLINK(ETYPE) ETYPE##Link
#define OITER(ETYPE) ETYPE##DlistIter
#define ODlist(ETYPE) ETYPE##Dlist

class Z_DLLSPEC zPlink:public zLink {					
protected:											
	char* item;
	BOOL del;
	zPlink(zLink* l,char* i=0):zLink(l) { item=i; del = 0; }
public:												
	zPlink();
	~zPlink();
	char** Item() { return &item; }
	void zinsert(char* i)				
		{ ZNEWH zPlink(this,i); }		
	void zappend(char* i) {			
		((zPlink*)prev)->zinsert(i);	
	}
	BOOL &markDel() { return del; }
};			

class Z_DLLSPEC zPlist:public zPlink {			
public:												
	zPlist();
	zPlink* first()							
		{ return (zPlink*)next; }				
	zPlink* last()							
		{ return (zPlink*)prev; }			
	void zipush(char* i)				
		{ zinsert(i);}						
	char* peek()								
		{ return *(first()->Item()); }					
	char* pop();								
	char* dequeue();
	void zienqueue(char* i)	{ zinsert(i);}
	virtual void io(zArchive&);
};													
													
class Z_DLLSPEC zPListIter {								
	zPlink *cur;								
	zPlist *list;								
public:												
	zPListIter(zPlist *s);
	~zPListIter();
	void reset();
	void remove();
	char* Iter();
};


#define OLINK(ETYPE) ETYPE##Link
#define OITER(ETYPE) ETYPE##DlistIter
#define ODlist(ETYPE) ETYPE##Dlist

#define declDlist(ETYPE)													\
class Z_DLLSPEC OLINK(ETYPE):public zPlink {											\
public:																		\
	OLINK(ETYPE)(zLink* l,ETYPE* i=0):zPlink(l,(char*)i){}					\
	OLINK(ETYPE)() {}														\
	~OLINK(ETYPE)  () { /*if(item!=0) delete(ETYPE*)item;*/ }				\
	ETYPE*  *Item() { return ((ETYPE* *)&item); }						\
	void insert(ETYPE* i) { ZNEWH OLINK(ETYPE)(this,i); }						\
	void append(ETYPE* i) { ((OLINK(ETYPE) *)prev)->insert(i); }			\
};																			\
																			\
class Z_DLLSPEC ODlist(ETYPE):public zPlist {											\
public:																		\
	ODlist(ETYPE)() {}														\
	OLINK(ETYPE) *first() { return (OLINK(ETYPE)*)next; }					\
	OLINK(ETYPE) *last() { return (OLINK(ETYPE)*)prev; }					\
	ETYPE *peek() { return *(first()->Item()); }							\
	ETYPE *pop()  { return (ETYPE *)zPlist::pop();}							\
	ETYPE *removeFirst()  { return (ETYPE *)zPlist::pop();}					\
	void insert(ETYPE* i) { ZNEWH OLINK(ETYPE)(this,i); }						\
	void append(ETYPE* i) { ((OLINK(ETYPE) *)prev)->insert(i); }			\
	void enqueue(ETYPE* i) { insert(i);}									\
	ETYPE *dequeue() { return( ETYPE *)zPlist::dequeue(); }					\
	void push(ETYPE* i) { insert(i);}										\
	ETYPE *removeLast() { return( ETYPE *)zPlist::dequeue(); }				\
	~ODlist(ETYPE)() {}														\
};																			\
																			\
class Z_DLLSPEC OITER(ETYPE):public zPListIter {										\
public:																		\
	OITER(ETYPE)(ODlist(ETYPE) *s):zPListIter((zPlist*)s) {}				\
	ETYPE *operator()(void) { return (ETYPE*)Iter();}						\
};																			\
inline zArchive& operator<<(zArchive& arc,ODlist(ETYPE)& lst) {					\
	lst.io(arc);															\
    return arc;																\
}																			\
inline zArchive& operator>>(zArchive& arc,ODlist(ETYPE)& lst) {					\
	lst.io(arc);															\
	return arc;																\
}																			\

#endif

