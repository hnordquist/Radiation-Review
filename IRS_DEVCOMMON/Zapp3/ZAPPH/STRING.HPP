#ifndef _ZAPPSTRING_INCLUDED
#define _ZAPPSTRING_INCLUDED 1
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


#define Z_STRSIZE 8

class Z_DLLSPEC zString Z_STORABLE {
	declPersis(zString)

	RWCString p;
public:
	zString(const zString&, unsigned int chunk=Z_STRSIZE);
	zString(const char*, unsigned int chunk=Z_STRSIZE);
	zString(char c, unsigned int ln=1, unsigned int chunk=Z_STRSIZE);
	zString(unsigned int chunk);
	zString(const RWCString&);
	zString();
	zString(const zResId&,unsigned int maxSize =200);
	~zString();
	operator char*() { return (char*)(const char*)p; }
	operator const char*() const { return p; }
	operator unsigned char*() { return (unsigned char *)(char*)(const char*)p;}
	operator const unsigned char*() const { return (const unsigned char*)(const char*)p; }
	operator const RWCString&() const { return p; }
	operator RWCString&() { return p; }
	operator RWCString*() { return &p; }
	int isNull() { return p.isNull(); }
	unsigned int length() const { return p.length(); }

	char& operator[](size_t offset) { return p[offset]; }

	zString& operator=(const zString&);
	zString& operator=(const char*);
	zString& operator=(const char);
	zString& operator=(const RWCString&);
	
	int operator<(const zString& s)	const { return strcmp(p,s.p)<0; }
	int operator>(const zString& s)	const { return strcmp(p,s.p)>0; }
	int operator<=(const zString& s) const { return strcmp(p,s.p)<=0; }
	int operator>=(const zString& s) const { return strcmp(p,s.p)>=0; }
	int operator==(const zString& s) const;
	int operator!=(const zString& s)  const { return !(*this==s); }

	int operator<(const char* str) const { return strcmp(p,str)<0; }
	int operator>(const char* str) const { return strcmp(p,str)>0; }
	int operator<=(const char* str) const { return strcmp(p,str)<=0; }
	int operator>=(const char* str) const { return strcmp(p,str)>=0; }
	int operator==(const char* str) const { return strcmp(p,str)==0; }
	int operator!=(const char* str) const { return strcmp(p,str)!=0; }

	friend int operator<(const char* str,const zString& s) {
	    return strcmp(str,s.p)<0;
	}
	friend int operator>(const char* str,const zString& s) {
	    return strcmp(str,s.p)>0;
	}
	friend int operator<=(const char* str,const zString& s) {
	    return strcmp(str,s.p)<=0;
	}
	friend int operator>=(const char* str,const zString& s) {
	    return strcmp(str,s.p)>=0;
	}
	friend int operator==(const char* str,const zString& s) {
	    return strcmp(str,s.p)==0;
	}
	friend int operator!=(const char* str,const zString& s) {
	    return strcmp(str,s.p)!=0;
	}
	
	zString operator&(zString& s);
	zString operator&(const char* str);
	friend Z_DLLSPEC zString operator&(const char* str,zString& s);

	zString& operator&=(zString&);
	zString& operator&=(const char* str);

	zString& operator&=(char c);

	char& at(size_t i) { return p[i]; }

	unsigned reSize(unsigned newCap);

	void toLower();
	void toUpper();
	unsigned capacity() { return p.capacity()-1; }
	unsigned size() { return p.capacity(); }
#ifndef Z_NP
	virtual void io(zArchive&);
#endif
};

declSlist(zString)
declSlist(RWCString)
declDlist(RWCString)

#endif
