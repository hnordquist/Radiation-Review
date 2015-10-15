#ifndef _ZAPPPERSIST_INCLUDED
#define _ZAPPPERSIST_INCLUDED 1
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


#ifdef _MSC_VER
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
//#define lseek _lseek
//#define open _open
#else
extern "C" {
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
}
#endif

#ifndef Z_NP
struct Z_DLLSPEC TypeInfo {
	int id;
	char *nm;
	unsigned int size;
	void* (*buildIt)(void* build);
	
	
//  static TypeInfo **getDLLList() { return dll list}; this is for both
// this function is not inline and is exported.
	
//  then there is a seconond funcion
// static TypeInfo ** getAppList() {return user list};
//  user list is in asm file.
	
// static TypeInfo *first is gone;
#if defined(Z_IMPDLL) || defined(Z_USEDLL)

	static TypeInfo ** Z_DLLSPEC getDLLList();
	static TypeInfo ** Z_DLLSPEC getAppList();
#endif

	static TypeInfo *first;
	TypeInfo *prev;
};

class Z_DLLSPEC TypeInit {
public:
	static int ids;
	TypeInit(TypeInfo*, int caller=0);
};

#define declPersis(ETYPE)								\
public:													\
	static TypeInfo zInf##ETYPE;						\
	static TypeInit zInfInit##ETYPE;					\
	virtual TypeInfo* typeInfo();						\
	static void* buildNew(void *);						\
	friend zArchive& operator <<(zArchive&,ETYPE &);	\
	friend zArchive& operator >>(zArchive&,ETYPE &);	\
	friend zArchive& operator <<(zArchive&,ETYPE*);		\
	friend zArchive& operator >>(zArchive&,ETYPE**);	\
private:


#define declPersis2(ETYPE,E2TYPE)						\
public:													\
	static TypeInfo zInf##ETYPE##E2TYPE;				\
	static TypeInit zInfInit##ETYPE##E2TYPE;			\
	virtual TypeInfo* typeInfo();						\
	static void* buildNew(void *);						\
	friend zArchive& operator <<(zArchive&,ETYPE##E2TYPE &);	\
	friend zArchive& operator >>(zArchive&,ETYPE##E2TYPE &);	\
	friend zArchive& operator <<(zArchive&,ETYPE##E2TYPE*);		\
	friend zArchive& operator >>(zArchive&,ETYPE##E2TYPE**);	\
private:



#define declTopPersis(ETYPE)							\
public:													\
	static TypeInfo zInf##ETYPE;						\
	static TypeInit zInfInit##ETYPE;					\
	virtual TypeInfo* typeInfo();						\
	static void* buildNew(void *);						\
private:


#if defined(Z_USEDLL)
#define ZPERSISTCALLER 1
#else
#define ZPERSISTCALLER 0
#endif
	
	
#ifdef __BCPLUSPLUS__		
	
#define implPersis(ETYPE)								\
void* ETYPE::buildNew(void*) { return ZNEWH ETYPE;}		\
TypeInfo ETYPE::zInf##ETYPE=							\
	{ 0,#ETYPE,sizeof(ETYPE),ETYPE::buildNew,0 };		\
TypeInit ETYPE::zInfInit##ETYPE(&ETYPE::zInf##ETYPE,ZPERSISTCALLER);	\
TypeInfo* ETYPE::typeInfo(){ return &ETYPE::zInf##ETYPE;}\
zArchive& Z_DLLSPEC operator<<(zArchive& arc,ETYPE &ob)			\
	{arc <<(zStorable &)ob; return arc;}				\
zArchive& Z_DLLSPEC operator>>(zArchive& arc,ETYPE &ob)			\
	{arc >>(zStorable &)ob; return arc;}				\
zArchive& Z_DLLSPEC operator<<(zArchive& arc,ETYPE* ob)			\
	{arc <<(zStorable* )ob; return arc;}				\
zArchive& Z_DLLSPEC operator>>(zArchive& arc,ETYPE**ob)			\
	{arc >>(zStorable**)ob; return arc;}				\

		
#else

#define implPersis(ETYPE)								\
void* ETYPE::buildNew(void*) { return ZNEWH ETYPE;}		\
TypeInfo ETYPE::zInf##ETYPE=							\
	{ 0,#ETYPE,sizeof(ETYPE),&ETYPE::buildNew,0 };		\
TypeInit ETYPE::zInfInit##ETYPE(&ETYPE::zInf##ETYPE, ZPERSISTCALLER);	\
TypeInfo* ETYPE::typeInfo(){ return &ETYPE::zInf##ETYPE;}\
zArchive& Z_DLLSPEC operator<<(zArchive& arc,ETYPE &ob)			\
	{arc <<(zStorable &)ob; return arc;}				\
zArchive& Z_DLLSPEC operator>>(zArchive& arc,ETYPE &ob)			\
	{arc >>(zStorable &)ob; return arc;}				\
zArchive& Z_DLLSPEC operator<<(zArchive& arc,ETYPE* ob)			\
	{arc <<(zStorable* )ob; return arc;}				\
zArchive& Z_DLLSPEC operator>>(zArchive& arc,ETYPE**ob)			\
	{arc >>(zStorable**)ob; return arc;}				\

		
#endif


#define implPersis2(ETYPE,E2TYPE)														\
void* ETYPE##E2TYPE::buildNew(void*) { return ZNEWH ETYPE##E2TYPE;}						\
TypeInfo ETYPE##E2TYPE::zInf##ETYPE##E2TYPE=											\
	{ 0,#ETYPE#E2TYPE,sizeof(ETYPE##E2TYPE),&ETYPE##E2TYPE::buildNew,0 };				\
TypeInit ETYPE##E2TYPE::zInfInit##ETYPE##E2TYPE(&ETYPE##E2TYPE::zInf##ETYPE##E2TYPE,ZPERSISTCALLER);	\
TypeInfo* ETYPE##E2TYPE::typeInfo(){ return &ETYPE##E2TYPE::zInf##ETYPE##E2TYPE;}		\
zArchive& Z_DLLSPEC operator<<(zArchive& arc,ETYPE##E2TYPE &ob)			\
	{arc <<(zStorable &)ob; return arc;}						\
zArchive& Z_DLLSPEC operator>>(zArchive& arc,ETYPE##E2TYPE &ob)			\
	{arc >>(zStorable &)ob; return arc;}						\
zArchive& Z_DLLSPEC operator<<(zArchive& arc,ETYPE##E2TYPE* ob)			\
	{arc <<(zStorable* )ob; return arc;}						\
zArchive& Z_DLLSPEC operator>>(zArchive& arc,ETYPE##E2TYPE**ob)			\
		{arc >>(zStorable**)ob; return arc;}					\
		



#define implTopPersis(ETYPE)							\
void* ETYPE::buildNew(void*) { return ZNEWH ETYPE;}		\
TypeInfo ETYPE::zInf##ETYPE=							\
	{ 0,#ETYPE,sizeof(ETYPE),&ETYPE::buildNew,0 };		\
TypeInit ETYPE::zInfInit##ETYPE(&ETYPE::zInf##ETYPE,ZPERSISTCALLER);	\
TypeInfo* ETYPE::typeInfo(){return &ETYPE::zInf##ETYPE;}\


#define Z_STORABLE :public zStorable

		
class Z_DLLSPEC zStorable {
	declTopPersis(zStorable)
public:
	virtual void io(zArchive& arc);
	zStorable() {}
	virtual ~zStorable();
};

enum ZIO_MODE { ZIO_READING,ZIO_WRITING };

enum ZIO_STATUS {
	ZIO_GOODBIT=0x00,ZIO_EOFBIT=0x01,
	ZIO_FAILBIT=0x02,ZIO_BADBIT=0x04
};

declDynaArray(long)

class Z_DLLSPEC zArchive {
	friend class Z_DLLSPEC zString;
	FILE *fp;
	void putNext(const void*,unsigned long);
	void getNext(void*,unsigned long =0);
	void putTypeName(char*);
	char* getTypeName();
	int which,hi;
	longDynaArray ptrs;
	ZIO_STATUS status;
public:
	zArchive(const char *fname,ZIO_MODE);
	zArchive& operator<<(zStorable &ob);
	zArchive& operator>>(zStorable &ob);
	zArchive& operator<<(zStorable* ob);
	zArchive& operator>>(zStorable**ob);
	zArchive& operator<<(const short&);
	zArchive& operator>>(short&);
	zArchive& operator<<(const unsigned int&);
	zArchive& operator>>(unsigned int&);
	zArchive& operator<<(const int&);
	zArchive& operator>>(int&);
	zArchive& operator<<(const char&);
	zArchive& operator>>(char&);
	zArchive& operator<<(const float&);
	zArchive& operator>>(float&);
	zArchive& operator<<(const double&);
	zArchive& operator>>(double&);
	zArchive& operator<<(const long&);
	zArchive& operator>>(long&);
	zArchive& operator<<(const unsigned long&);
	zArchive& operator>>(unsigned long&);
#if defined(_MSC_VER) || !defined(Z_DOS)
#ifndef __IBMCPP__
	zArchive& operator<<(const BYTE&);
	zArchive& operator>>(BYTE&);
#endif
#endif
	zArchive& operator<<(TypeInfo&);
	zArchive& operator>>(TypeInfo&);

	zArchive& operator>>(zPair&);
	zArchive& operator<<(const zPair&);

	zArchive& operator>>(zRange&);
	zArchive& operator<<(const zRange&);

	zArchive& operator>>(zPoint&);
	zArchive& operator<<(const zPoint&);
	
	zArchive& operator>>(zDimension&);
	zArchive& operator<<(const zDimension&);

	zArchive& operator>>(zRect&);
	zArchive& operator<<(const zRect&);

	zArchive& operator>>(zColor&);
	zArchive& operator<<(const zColor&);

	void close();
	BOOL isOutput() const;
	BOOL isInput() const;
	inline BOOL good() const;
	inline BOOL bad() const;
	inline BOOL fail() const;
	inline BOOL eof() const;
	inline void clear(int =0);
	~zArchive();
};

inline BOOL zArchive::good() const { return (status==0); }
inline BOOL zArchive::bad() const { return status&ZIO_BADBIT; }
inline BOOL zArchive::fail() const { return status&(ZIO_BADBIT|ZIO_FAILBIT); }
inline BOOL zArchive::eof() const { return status&ZIO_EOFBIT; }
inline void zArchive::clear(int s){ status=(ZIO_STATUS)s; }
#else
#define declPersis(xxx)
#define declPersis2(xxx,yyy)
#define implPersis(xxx);
#define implPersis2(xxx,yyy);
class Z_DLLSPEC zArchive {
};
#define Z_STORABLE
#endif

#endif

