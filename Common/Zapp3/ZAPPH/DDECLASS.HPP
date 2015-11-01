#ifndef _ZAPPDDECLASS_INCLUDED
#define _ZAPPDDECLASS_INCLUDED 1
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


extern "C" {
#include <ddeml.h>
}


#define zDdeAck DDE_FACK

#define zDdeNAck DDE_FNOTPROCESSED
#define zDdeBusy DDE_FBUSY

enum zDdeErrors {
	zNullConnection,
	zUnknownAdvise,
	zUnknownTransaction,
	zFailConversationCreate,
	zFailTransactionCreate,
	zUnknownError
};

typedef int (zEvH::*zDdeErrorProc)(zDdeErrors,int);

class Z_DLLSPEC zDdeDev {
protected:
	unsigned long _inst;
	BOOL status;
	static zEvH *errorObj;
	static zDdeErrorProc errorProc;
public:
	zDdeDev();
	inline unsigned long inst();
	BOOL isValid();
	static void error(zDdeErrors,int =0);
	void setErrorNotify(zEvH*,zDdeErrorProc);
	~zDdeDev();
};


class Z_DLLSPEC zDdeClient:public zDdeDev {
public:
	zDdeClient();
	~zDdeClient();
};


class Z_DLLSPEC zDdeServer:public zDdeDev {
	HSZ serverAppName;
	RWCString _serverAppName;
public:
	zDdeServer(const char *);
	~zDdeServer();
};

class Z_DLLSPEC zDdeEvent {
public:
	unsigned long _dSize;
	void *_pData;
	unsigned int type;
	unsigned int fmt;
	HCONV		conv;
	HSZ			sz1,sz2;
	HDDEDATA	hData;
	DWORD		data1,data2;
	inline HSZ hTopic() const;
	zExchangeFmt dataFormat();
};

class Z_DLLSPEC zAdviseDservEvt:public zDdeEvent {
public:
	inline HSZ hItem() const;
	RWCString item();
};

class Z_DLLSPEC zRenderDservEvt:public zDdeEvent {
public:
	inline HSZ hItem() const;
	unsigned long respondWith(void *,long,zExchangeFmt);
	unsigned long respondWith(const char *);
};

class Z_DLLSPEC zRequestDservEvt:public zDdeEvent {
public:
	inline HSZ hItem() const;
	unsigned long respondWith(void *,long len,zExchangeFmt);
	unsigned long respondWith(const char *);
	RWCString item();
};

class Z_DLLSPEC zExecuteDservEvt:public zDdeEvent {
public:
	inline char *cmd();
	inline unsigned long cmdLength();
};

class Z_DLLSPEC zPokeDservEvt:public zDdeEvent {
public:
	inline HSZ hItem() const;
	inline void* pData();
	inline unsigned long dataSize();
	RWCString item();
};

class Z_DLLSPEC zConnectDservEvt:public zDdeEvent {
	void setServerAppName(RWCString&);
public:
	inline HSZ hServer() const;
	RWCString topic();
	RWCString server();
	friend class ziServerPane;
};

class Z_DLLSPEC zTopicServer;

class Z_DLLSPEC zAdviseLink {
	zTopicServer *dad;
	RWCString _item;
	HSZ _hItem;
	zExchangeFmt _fmt;
public:
	zAdviseLink(zTopicServer*,zAdviseDservEvt*);
	void setDirty();
	inline RWCString& item();
	inline zExchangeFmt format();
	virtual unsigned long render(zRenderDservEvt*);
	virtual ~zAdviseLink();
	int operator<(const zAdviseLink&) const { return 0; }
	int operator==(const zAdviseLink&) const;
};


class Z_DLLSPEC zTopicServer {
	zDdeServer *_serverDev;
	RWCString name;
	HSZ hName;
	static RWTPtrDlist<zTopicServer> *cslist;
	RWTPtrDlist<zAdviseLink> *_links;
protected:
	inline RWTPtrDlist<zTopicServer>* topicList();
	inline zDdeServer* serverDev();
public:
	zTopicServer(zDdeServer*,const char *);
	virtual void connection(zConnectDservEvt*);
	virtual unsigned long request(zRequestDservEvt*);
	virtual unsigned long adviseStart(zAdviseDservEvt*);
	virtual unsigned long adviseStop(zAdviseDservEvt*);
	virtual unsigned long execute(zExecuteDservEvt*);
	virtual unsigned long poke(zPokeDservEvt*);
	static zTopicServer* find(char*);
	zAdviseLink* findLink(const char *item);
	inline RWTPtrDlist<zAdviseLink>* links();
	inline void links(RWTPtrDlist<zAdviseLink>*);
	inline RWCString topic();
	inline HSZ hTopic();
	int operator<(const zTopicServer&) const { return 0; }
	~zTopicServer();
	int operator==(const zTopicServer&) const;
};

class Z_DLLSPEC zAdviseDcliEvt:public zDdeEvent {
public:
	inline HSZ hItem() const;
	inline void* pData();
	inline unsigned long dataSize();
};

class Z_DLLSPEC zCompleteDcliEvt:public zDdeEvent {
public:
	inline HSZ hItem() const;
	inline void* pData();
	inline unsigned long dataSize();
	inline HDDEDATA data();
	inline unsigned long tid();
	inline BOOL isValid();
};


class Z_DLLSPEC zConversation {
	zDdeDev *_clientDev;
	BOOL status;
	HCONV cid;
	RWCString _topic, _server;
	HSZ csz,csv;

public:
	zConversation(zDdeClient *,const char *server,const char *top);
	BOOL isValid();
	inline zDdeDev* clientDev();
	inline const char *topic();
	inline const char *server();
	inline operator HCONV();
	inline HSZ convString();

	~zConversation();

	virtual unsigned long disconnect( zDdeEvent *Ev );	
	int operator==(const zConversation&) const;
};


class Z_DLLSPEC zTransaction {
protected:
	zConversation *_conv;
	unsigned long tid;
	unsigned int fmt;
	HSZ dit;
	RWCString _item;
	int autodel;
	zTransaction(zConversation*,zExchangeFmt =ZFMT_TEXT);
public:
	zTransaction(zConversation*,char *item,zExchangeFmt =ZFMT_TEXT);
	inline zConversation* conv();
	inline const char *item();
	int autoDelete();
	virtual unsigned long completed(zCompleteDcliEvt*);
	virtual ~zTransaction();
	int operator==(const zTransaction&) const;
};

class Z_DLLSPEC zExecuteTrans:public zTransaction {
public:
	zExecuteTrans(zConversation*,char *cmds,int wait=0);
};

class Z_DLLSPEC zRequestTrans:public zTransaction {
	unsigned long data;
public:
	zRequestTrans(zConversation*,char *item,zExchangeFmt=ZFMT_TEXT,int wait=0);
	void* getSyncData(unsigned long& dataSize);
	void freeSyncData();
};

class Z_DLLSPEC zAdviseTrans:public zTransaction {
	static RWTPtrDlist<zAdviseTrans> *tlist;
public:
	zAdviseTrans(zConversation*,char *item,zExchangeFmt =ZFMT_TEXT);
	static zAdviseTrans* find(zAdviseDcliEvt*);
	virtual unsigned long data(zAdviseDcliEvt*);
	virtual ~zAdviseTrans();
	static RWTPtrDlist<zAdviseTrans>* adviseList();
	int operator<(const zAdviseTrans&) const { return 0; }
	int operator==(const zAdviseTrans&) const;
};

class Z_DLLSPEC zPokeTrans:public zTransaction {
public:
	zPokeTrans(zConversation*,char *item,char* data,int wait=0);
	zPokeTrans(zConversation*,char *item,void* data,
		unsigned int dataLen,zExchangeFmt =ZFMT_TEXT,int wait=0);
};


#include <zapph\ddeclass.inl>

#endif

