#ifndef _ZREGIMP_INCLUDED
#define _ZREGIMP_INCLUDED 1
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

class Z_DLLSPEC ziRegImp {
public:
    ziRegImp();
    ziRegImp(const RWCString& filename);
    ~ziRegImp();
    int createKey(const RWCString& key);
    int createKeyValue(const RWCString& key, const RWCString& valueName, const RWCString& value);
    int createKeyValue(const RWCString& key, const RWCString& valueName,const void *value,unsigned long length);

    //cannot delete a key with sub-keys
    int deleteKey(const RWCString& key);
    int deleteValue(const RWCString& key, const RWCString& valueName);

    int getChildKeys(const RWCString& parentKey,RWTPtrSlist<RWCString>* childKeyList) const;
    int getValueNames(const RWCString& key,RWTPtrSlist<RWCString>* valueNameList) const;
    zRegistryValueType getValueType(const RWCString& key,const RWCString& valueName) const;

    //returns true if key exists
    int keyExists(const RWCString& key) const;
    //returns true if value exists
    int valueNameExists(const RWCString& key, const RWCString& valueName) const;

    //note: if valueName parameter is empty string, it will get the default value
    int getValue(const RWCString& key,const RWCString& valueName,RWCString *outValue) const;
    //note: this function will return FALSE if the buffer you provide is not big
    //      enough to hold the data. The necessary buffer size is returned in
    //      the length parameter.
    int getValue(const RWCString& key,const RWCString& valueName,void *buf,unsigned long length) const;

private:
    //this file manager manages the whole registry file
    RWFileManager* pFm_;
    //holds the key-child heirarchy part of registry
    RWBTreeOnDisk* pBChildTree_;
    //holds the key-Value heirarchy part of registry
    RWBTreeOnDisk* pBValueTree_;
    //holds the offsets to the two BTreeOnDisks
    RWoffset treeArray_[2];

    //pointer to the BTree that holds the key-child heirarchy info
    //this tree gets created and deleted for each operation. It
    //maintains no state.
    
    //adds a key to the key-child heirachy part of registry
    void iAddKeyChild(const RWCString& key);

    void iAddStringToCollection(const RWCString& valueName, const RWCString value,RWSlistCollectables *collectList);
    void iAddBinaryToCollection(const RWCString& valueName,unsigned char *data,unsigned long length,RWSlistCollectables *collectList);
    //stores the currently active value collection for the key.
    //the key must not exist, ie, you should have removed it
    //before calling this function
    void iStoreValueCollection(const RWCString& key,RWSlistCollectables *collectList);
    //clears out active collection and fills it with the new
    //collection for key if it exists.
    int iGetValueCollection(const RWCString& key,RWSlistCollectables *collectList) const;
    int iKeyHasChildren(const RWCString& key);
    int iDeleteBTreeValueCollection(const RWCString& key);
    int iDeleteBTreeChildCollection(const RWCString& key);
    int keyLevels(const RWCString& key);
    static void iPutChildrenInCollection(RWCollectable* string, void *childKeyList);
    static void iGetRootNodes(const char *string, RWstoredValue val, void *childKeyList);
};//end of class zRegImp

class Z_DLLSPEC ziRegValue : public RWCollectable {
    RWDECLARE_COLLECTABLE(ziRegValue)

    public:
        ziRegValue();
        ~ziRegValue();

        //from clas RWCollectable
        RWspace     binaryStoreSize() const;
        int         compareTo(const RWCollectable*) const;
        RWBoolean   isEqual(const RWCollectable*) const;
        void        restoreGuts(RWFile&);
        void        restoreGuts(RWvistream&);
        void        saveGuts(RWFile&) const;
        void        saveGuts(RWvostream&) const;

    public:

        RWCString valueName;
        zRegistryValueType type;
        RWCString stringValue;
        unsigned long binLength;
        unsigned char *binData;
};


#endif

