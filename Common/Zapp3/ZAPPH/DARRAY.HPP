#ifndef _ZAPPDARRAY_INCLUDED
#define _ZAPPDARRAY_INCLUDED 1
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


#if defined(__ZTC__) && !defined(__SC__)
#define ARRAYDEL
#else
#define ARRAYDEL []
#endif

class Z_DLLSPEC zArchive;

#define ODA(ETYPE) ETYPE##DynaArray

#define declDynaArray(ETYPE)											\
class Z_DLLSPEC ODA(ETYPE) {											\
	void grow(unsigned int til);										\
	unsigned int limit;													\
	unsigned int growby;												\
	ETYPE def;															\
	ETYPE* me;															\
public:																	\
	ODA(ETYPE)() { me=0; limit=0; growby=1; }							\
	ODA(ETYPE)(int rate):growby(rate) { me=0; limit=0; }				\
	ODA(ETYPE)(int rate,ETYPE &errnode):growby(rate),def(errnode)		\
	{ limit=0; }														\
	ODA(ETYPE)(const ODA(ETYPE)&);										\
	~ODA(ETYPE)() { if (me) delete ARRAYDEL me; }						\
	ODA(ETYPE)& operator=(const ODA(ETYPE)&);							\
	ETYPE& operator[](unsigned int pos) {								\
		if (pos+1>limit) grow(pos);										\
		if (pos+1<=limit) return *(me+pos);								\
		else return def;														\
	}																	\
	unsigned int size() const { return limit; }							\
	unsigned int growBy() const { return growby; }						\
	void setDef(const ETYPE& e) { def=e; }								\
	void growBy(unsigned int i) { growby=i; }							\
};																		\
																		\



#define implDynaArray(ETYPE)											\
void ODA(ETYPE)::grow(unsigned int til) {								\
	unsigned int alloc;													\
	if (limit+growby < til+1)											\
		alloc=til+1;													\
	else																\
		alloc=limit+growby;												\
	if (me==0) {														\
		me=new ETYPE[alloc];											\
		if (me) {														\
			limit=alloc;												\
			for (unsigned int i=0;i<limit;i++) me[i]=def;				\
		}																\
	} else {															\
		ETYPE* n;														\
		n=new ETYPE[alloc];												\
		if (n) {														\
			unsigned int i;												\
			for (i=0;i<limit;i++) n[i]=me[i];							\
			delete ARRAYDEL me;											\
			for (i=limit;i<alloc;i++) n[i]=def;							\
			me=n;														\
			limit=alloc;												\
		}																\
	}																	\
}																		\
																		\
ODA(ETYPE)::ODA(ETYPE)(const ODA(ETYPE)& d) {							\
	limit=d.limit;														\
	growby=d.growby;													\
	def=d.def;															\
	me=new ETYPE[limit];												\
	for (unsigned int i=0;i<limit;i++) me[i]=d.me[i];					\
}																		\
																		\
ODA(ETYPE)& ODA(ETYPE)::operator=(const ODA(ETYPE)& d) {				\
	if (me!=0) delete ARRAYDEL me;										\
	limit=d.limit;														\
	growby=d.growby;													\
	def=d.def;															\
	me=new ETYPE[limit];												\
	for (unsigned int i=0;i<limit;i++) me[i]=d.me[i];					\
	return *this;														\
}																		\
																		\


#define OPDA(ETYPE) ETYPE##DynaPersisArray

#define declDynaPersisArray(ETYPE)										\
class Z_DLLSPEC OPDA(ETYPE) {											\
	void grow(unsigned int til);										\
	unsigned int limit;													\
	unsigned int growby;												\
	ETYPE def;															\
	ETYPE* me;															\
public:																	\
	OPDA(ETYPE)() { me=0; limit=0; growby=1; }							\
	OPDA(ETYPE)(int rate):growby(rate) { me=0; limit=0; }				\
	OPDA(ETYPE)(int rate,ETYPE &errnode):growby(rate),def(errnode)		\
	{ limit=0; }														\
	OPDA(ETYPE)(const OPDA(ETYPE)&);									\
	~OPDA(ETYPE)() { if (me) delete ARRAYDEL me; }						\
	OPDA(ETYPE)& operator=(const OPDA(ETYPE)&);							\
	ETYPE& operator[](unsigned int pos) {								\
		if (pos+1>=limit) grow(pos);									\
		if (pos+1<=limit) return *(me+pos);								\
		return def;														\
	}																	\
	unsigned int size() const { return limit; }							\
	unsigned int growBy() const { return growby; }						\
	void growBy(unsigned int i) { growby=i; }							\
};																		\
																		\
zArchive& operator<<(zArchive&,OPDA(ETYPE)&);							\
zArchive& operator>>(zArchive&,OPDA(ETYPE)&);							\



#define implDynaPersisArray(ETYPE)										\
void OPDA(ETYPE)::grow(unsigned int til) {								\
	unsigned int alloc;													\
	if (limit+growby < til+1)											\
		alloc=til+1;													\
	else																\
		alloc=limit+growby;												\
	if (me==0) {														\
		me=new ETYPE[alloc];											\
		if (me) {														\
			 limit=alloc;												\
			 for (unsigned int i = 0;i<limit;i++) me[i]=def;			\
		}																\
	} else {															\
		ETYPE* n;														\
		n=new ETYPE[alloc];												\
		if (n) {														\
			unsigned int i;												\
			for (i=0;i<limit;i++) n[i]=me[i];							\
			delete ARRAYDEL me;											\
			for (i=limit;i<alloc;i++) n[i]=def;							\
			me=n;														\
			limit=alloc;												\
		}																\
	}																	\
}																		\
																		\
OPDA(ETYPE)::OPDA(ETYPE)(const OPDA(ETYPE)& d) {						\
	limit=d.limit;														\
	growby=d.growby;													\
	def=d.def;															\
	me=new ETYPE[limit];												\
	for (unsigned int i=0;i<limit;i++) me[i]=d.me[i];					\
}																		\
																		\
OPDA(ETYPE)& OPDA(ETYPE)::operator=(const OPDA(ETYPE)& d) {				\
	if (me!=0) delete ARRAYDEL me;										\
	limit=d.limit;														\
	growby=d.growby;													\
	def=d.def;															\
	me=new ETYPE[limit];												\
	for (unsigned int i=0;i<limit;i++) me[i]=d.me[i];					\
	return *this;														\
}																		\
																		\
zArchive& operator<<(zArchive &arc,OPDA(ETYPE) &lst) {					\
		arc << lst.size();												\
		arc << lst.growBy();											\
		for (unsigned int i=0;i<lst.size();i++) arc << lst[i];			\
}																		\
																		\
zArchive& operator>>(zArchive &arc,OPDA(ETYPE) &lst) {					\
		unsigned int cnt,gb;											\
		arc >> cnt;														\
		arc >> gb;														\
		lst.growBy(cnt);												\
		for (unsigned int i=0;i<cnt;i++) arc >> lst[i];					\
		lst.growBy(gb);													\
}																		\
			
	
#endif

