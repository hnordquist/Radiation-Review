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

class Z_DLLSPEC ziWindowCell {
	HWND cellKey;
	zWindow* windowData;
public:
	ziWindowCell(HWND cKey,zWindow *wData) {cellKey=cKey;windowData=wData;}
	HWND key() {return cellKey;}
	zWindow *data() {return windowData;}
};

declDlist(ziWindowCell)

typedef ziWindowCellDlist *ziWindowCellDlistPtr;
typedef unsigned int  (*HashProc)(void *);

unsigned int Z_DLLSPEC WindowHashProc(void *);


class Z_DLLSPEC ziWindowMap {
	unsigned int _numBuckets;
	ziWindowCellDlistPtr *map;
	HashProc _hashProc;
	ziWindowMap operator =(const ziWindowMap&) { return ziWindowMap((HashProc) 0, 0); }
	ziWindowMap(ziWindowMap&) {;}
	int operator ==(const ziWindowMap&) {return 0;}
	
public:
	ziWindowMap(HashProc hasher,unsigned int nBuckets=97);
	~ziWindowMap();
	unsigned int numBuckets() {return _numBuckets;}
	zWindow *insert(HWND hWnd, zWindow *window);
	zWindow *remove(HWND hWnd);
	zWindow *get(HWND hWnd);
#ifdef Z_DB
	ziWindowCellDlist *bucket(unsigned int b) {return map[b];}
	unsigned int numEntries();
#endif
};
