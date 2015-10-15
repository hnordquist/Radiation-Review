#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

/* Raima Database Manager Version 4.5 */


/* Raima Object Manager specific header files */
#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>

/* include record/key structure definitions */
#include "tutorial.h"

/* database class declaration */
class OM_EXPORT tutorial : public StoreDb
{
public:
   tutorial() : StoreDb("tutorial", PDB_LOCATOR) {}
   DEFINE_DB_LOCATOR;
};


/* Persistent Object classes that are defined in this file */
OMCLASS(AuthorObj)
OMCLASS(InfoObj)
OMCLASS(systemObj)
OMCLASS(Author_fullname_KeyObj)
OMCLASS(Info_idcode_KeyObj)


class OM_EXPORT AuthorObj : public StoreObj, public Author
{
public:
   AuthorObj(const PVistaDb pdb=tutorial::DbPtr) : StoreObj(pdb) {}
   AuthorObj(const RSetObj set, const PVistaDb pdb=tutorial::DbPtr) : StoreObj(set, pdb) {}
   AuthorObj(const RKeyObj key, const PVistaDb pdb=tutorial::DbPtr) : StoreObj(key, pdb) {}
   virtual ~AuthorObj() {}

   virtual REC_TYPE RecType() const { return AUTHOR; }
   virtual Pvoid RecData() { return (Pvoid)(Author OM_FAR *)this; }

   STOREDIN(tutorial);
   CUSTOMOPS(AuthorObj);
   OWNEROF(InfoObj, HASPUBLISHED);
   MEMBEROF(systemObj, AUTHORLIST);
};


class OM_EXPORT InfoObj : public StoreObj, public Info
{
public:
   InfoObj(const PVistaDb pdb=tutorial::DbPtr) : StoreObj(pdb) {}
   InfoObj(const RSetObj set, const PVistaDb pdb=tutorial::DbPtr) : StoreObj(set, pdb) {}
   InfoObj(const RKeyObj key, const PVistaDb pdb=tutorial::DbPtr) : StoreObj(key, pdb) {}
   virtual ~InfoObj() {}

   virtual REC_TYPE RecType() const { return INFO; }
   virtual Pvoid RecData() { return (Pvoid)(Info OM_FAR *)this; }

   STOREDIN(tutorial);
   CUSTOMOPS(InfoObj);
   MEMBEROF(AuthorObj, HASPUBLISHED);
};


class OM_EXPORT systemObj : public StoreObj
{
public:
   systemObj(const PVistaDb pdb=tutorial::DbPtr) : StoreObj(pdb) {}
   virtual ~systemObj() {}

   virtual REC_TYPE RecType() const { return SYSTEM; }

   STOREDIN(tutorial);
   OWNEROF(AuthorObj, AUTHORLIST);
};


class OM_EXPORT Author_fullname_KeyObj : public KeyObj, public Author_fullname
{
public:
   Author_fullname_KeyObj() : KeyObj(AUTHOR_FULLNAME, (Pvoid)(Author_fullname OM_FAR *)this) {}
   virtual ~Author_fullname_KeyObj() {}
};


class OM_EXPORT Info_idcode_KeyObj : public KeyObj
{
public:
   char idcode[16];

   Info_idcode_KeyObj() : KeyObj(INFO_IDCODE, (Pvoid) &idcode[0]) {}
   virtual ~Info_idcode_KeyObj() {}
};



#endif    /* TUTORIAL_HPP */
