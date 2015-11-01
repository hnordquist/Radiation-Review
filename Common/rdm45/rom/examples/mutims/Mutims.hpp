#ifndef MUTIMS_HPP
#define MUTIMS_HPP

/* Raima Database Manager Version 4.5 */


/* Raima Object Manager specific header files */
#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>
#include <omblob.hpp>

/* include record/key structure definitions */
#include "mutims.h"

/* database class declaration */
class OM_EXPORT mutims : public StoreDb
{
public:
   mutims() : StoreDb("mutims", PDB_LOCATOR) {}
   DEFINE_DB_LOCATOR;
};


/* Persistent Object classes that are defined in this file */
OMCLASS(AuthorObj)
OMCLASS(InfoObj)
OMCLASS(BorrowerObj)
OMCLASS(KeywordObj)
OMCLASS(Keyword_IntersectObj)
OMCLASS(Borrower_IntersectObj)
OMCLASS(Text1Obj)
OMCLASS(Text2Obj)
OMCLASS(Text3Obj)
OMCLASS(AbstractObj)
OMCLASS(systemObj)
OMCLASS(Info_idcode_KeyObj)
OMCLASS(Borrower_name_KeyObj)
OMCLASS(Keyword_word_KeyObj)


class OM_EXPORT AuthorObj : public StoreObj, public Author
{
public:
   AuthorObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   AuthorObj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   virtual ~AuthorObj() {}

   virtual REC_TYPE RecType() const { return AUTHOR; }
   virtual Pchar OMLOADDS RecName() const { return "AUTHOR"; }
   virtual Pvoid RecData() { return (Pvoid)(Author OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(AuthorObj);
   OWNEROF(InfoObj, HASPUBLISHED);
   MEMBEROF(systemObj, AUTHORLIST);
};


class OM_EXPORT InfoObj : public StoreObj, public Info
{
public:
   InfoObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   InfoObj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   InfoObj(const RKeyObj key, const PVistaDb pdb=mutims::DbPtr) : StoreObj(key, pdb) {}
   virtual ~InfoObj() {}

   virtual REC_TYPE RecType() const { return INFO; }
   virtual Pchar OMLOADDS RecName() const { return "INFO"; }
   virtual Pvoid RecData() { return (Pvoid)(Info OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(InfoObj);
   OWNEROF(InfoObj, ARTICLELIST);
   OWNEROF(Borrower_IntersectObj, INFOTOBORROWER);
   OWNEROF(Keyword_IntersectObj, INFOTOKEY);
   OWNEROF(Text1Obj, ABSTRACT);
   OWNEROF(Text2Obj, ABSTRACT);
   OWNEROF(Text3Obj, ABSTRACT);
   OWNEROFBLOB(AbstractObj, ABSTRACT);
   MEMBEROF(AuthorObj, HASPUBLISHED);
   MEMBEROF(InfoObj, ARTICLELIST);
};


class OM_EXPORT BorrowerObj : public StoreObj, public Borrower
{
public:
   BorrowerObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   BorrowerObj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   BorrowerObj(const RKeyObj key, const PVistaDb pdb=mutims::DbPtr) : StoreObj(key, pdb) {}
   virtual ~BorrowerObj() {}

   virtual REC_TYPE RecType() const { return BORROWER; }
   virtual Pchar OMLOADDS RecName() const { return "BORROWER"; }
   virtual Pvoid RecData() { return (Pvoid)(Borrower OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(BorrowerObj);
   OWNEROF(Borrower_IntersectObj, BORROWERTOINFO);
   MEMBEROF(systemObj, LOANHISTORY);
};


class OM_EXPORT KeywordObj : public StoreObj, public Keyword
{
public:
   KeywordObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   KeywordObj(const RKeyObj key, const PVistaDb pdb=mutims::DbPtr) : StoreObj(key, pdb) {}
   virtual ~KeywordObj() {}

   virtual REC_TYPE RecType() const { return KEYWORD; }
   virtual Pchar OMLOADDS RecName() const { return "KEYWORD"; }
   virtual Pvoid RecData() { return (Pvoid)(Keyword OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(KeywordObj);
   OWNEROF(Keyword_IntersectObj, KEYTOINFO);
};


class OM_EXPORT Keyword_IntersectObj : public StoreObj, public Keyword_Intersect
{
public:
   Keyword_IntersectObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   Keyword_IntersectObj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   virtual ~Keyword_IntersectObj() {}

   virtual REC_TYPE RecType() const { return KEYWORD_INTERSECT; }
   virtual Pchar OMLOADDS RecName() const { return "KEYWORD_INTERSECT"; }
   virtual Pvoid RecData() { return (Pvoid)(Keyword_Intersect OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(Keyword_IntersectObj);
   MEMBEROF(KeywordObj, KEYTOINFO);
   MEMBEROF(InfoObj, INFOTOKEY);
};


class OM_EXPORT Borrower_IntersectObj : public StoreObj, public Borrower_Intersect
{
public:
   Borrower_IntersectObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   Borrower_IntersectObj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   virtual ~Borrower_IntersectObj() {}

   virtual REC_TYPE RecType() const { return BORROWER_INTERSECT; }
   virtual Pchar OMLOADDS RecName() const { return "BORROWER_INTERSECT"; }
   virtual Pvoid RecData() { return (Pvoid)(Borrower_Intersect OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(Borrower_IntersectObj);
   MEMBEROF(InfoObj, INFOTOBORROWER);
   MEMBEROF(BorrowerObj, BORROWERTOINFO);
};


class OM_EXPORT Text1Obj : public StoreObj, public Text1
{
public:
   Text1Obj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   Text1Obj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   virtual ~Text1Obj() {}

   virtual REC_TYPE RecType() const { return TEXT1; }
   virtual Pchar OMLOADDS RecName() const { return "TEXT1"; }
   virtual Pvoid RecData() { return (Pvoid)(Text1 OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(Text1Obj);
   MEMBEROF(InfoObj, ABSTRACT);
};


class OM_EXPORT Text2Obj : public StoreObj, public Text2
{
public:
   Text2Obj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   Text2Obj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   virtual ~Text2Obj() {}

   virtual REC_TYPE RecType() const { return TEXT2; }
   virtual Pchar OMLOADDS RecName() const { return "TEXT2"; }
   virtual Pvoid RecData() { return (Pvoid)(Text2 OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(Text2Obj);
   MEMBEROF(InfoObj, ABSTRACT);
};


class OM_EXPORT Text3Obj : public StoreObj, public Text3
{
public:
   Text3Obj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   Text3Obj(const RSetObj set, const PVistaDb pdb=mutims::DbPtr) : StoreObj(set, pdb) {}
   virtual ~Text3Obj() {}

   virtual REC_TYPE RecType() const { return TEXT3; }
   virtual Pchar OMLOADDS RecName() const { return "TEXT3"; }
   virtual Pvoid RecData() { return (Pvoid)(Text3 OM_FAR *)this; }

   STOREDIN(mutims);
   CUSTOMOPS(Text3Obj);
   MEMBEROF(InfoObj, ABSTRACT);
};


class OM_EXPORT AbstractObj : public VariLenText
{
public:
   AbstractObj(const PVistaDb pdb=mutims::DbPtr) : VariLenText(ABSTRACT, pdb) {}
   AbstractObj(const PStoreObj owner, const PVistaDb pdb=mutims::DbPtr) : VariLenText(ABSTRACT, owner, pdb) {}
   virtual ~AbstractObj() {}

   STOREDIN(mutims);
   BLOBMEMBEROF(InfoObj, ABSTRACT);
};


class OM_EXPORT systemObj : public StoreObj
{
public:
   systemObj(const PVistaDb pdb=mutims::DbPtr) : StoreObj(pdb) {}
   virtual ~systemObj() {}

   virtual REC_TYPE RecType() const { return SYSTEM; }
   virtual Pchar OMLOADDS RecName() const { return "SYSTEM"; }

   STOREDIN(mutims);
   OWNEROF(AuthorObj, AUTHORLIST);
   OWNEROF(BorrowerObj, LOANHISTORY);
};


class OM_EXPORT Info_idcode_KeyObj : public KeyObj
{
public:
   char idcode[16];

   Info_idcode_KeyObj() : KeyObj(INFO_IDCODE, (Pvoid) &idcode[0]) {}
   virtual ~Info_idcode_KeyObj() {}
};


class OM_EXPORT Borrower_name_KeyObj : public KeyObj
{
public:
   char name[31];

   Borrower_name_KeyObj() : KeyObj(BORROWER_NAME, (Pvoid) &name[0]) {}
   virtual ~Borrower_name_KeyObj() {}
};


class OM_EXPORT Keyword_word_KeyObj : public KeyObj
{
public:
   char word[31];

   Keyword_word_KeyObj() : KeyObj(KEYWORD_WORD, (Pvoid) &word[0]) {}
   virtual ~Keyword_word_KeyObj() {}
};



#endif    /* MUTIMS_HPP */
