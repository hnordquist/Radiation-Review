//-----------------------------------------------------
// This example illustrates the creation and use of
// the Polymorph class (Multi Membered Set)
//-----------------------------------------------------

#include "poly_db.hpp"
#include <storedb.hpp>
#include <storeobj.hpp>
#include <keyobj.hpp>
#include <poly.hpp>
#include <transact.hpp>
#include <grlock.hpp>

OMCLASS(Display)
OMCLASS(Rectangl)
OMCLASS(Triangle)
OMCLASS(Circle)
OMCLASS(Shape)
OMCLASS(AllShapes)


void OMEXTERNAL PolymorphErrorHandler(ROM_ERRINFO OM_FAR &errinfo)
{
#ifdef WINDOWS
   MessageBox(GetFocus(), (Pchar)errinfo.errtxt, (Pchar)"Polymorph Error Handler", 0);
#else
   Pchar detail = new char[om_strlen(errinfo.errtxt) + 60];
   sprintf(detail, "%s: %d\n%s\n", (Pchar)"Polymorph Error Handler", errinfo.errnum, errinfo.errtxt);
   puts((const Pchar)detail);
   gets(detail);
   delete detail;
#endif
}

/* database class declaration */
/*class OM_EXPORT poly_db : public StoreDb
{
public:
   poly_db() : StoreDb("poly_db", PDB_LOCATOR) {}
   DEFINE_DB_LOCATOR;
};
*/
class PolyTask : public StoreTask
{
private:
   poly_db db;
   
public: 
   PolyTask(Pchar mode, Pchar path) : StoreTask(mode, ActivateTask("RDS","admin","secret"))
      {
         SetUserPaths(path, "admin");
         lockcomm(LMC_INTERNAL);
         dbuserid("admin");
         SetErrorHandler(PolymorphErrorHandler);
         OpenDb();
         SetStatus(db.GetDbStatus());
      }
      
  ~PolyTask()    { CloseDb(); }

   int OpenDb()  { return db.Open();  }
   int CloseDb() { return db.Close(); }
};


class OM_EXPORT Display : public StoreObj, public _Display
{
public:
   Display(const PVistaDb pdb=poly_db::DbPtr) : StoreObj(pdb) {}
   Display(const RKeyObj key, const PVistaDb pdb=poly_db::DbPtr) : StoreObj(key, pdb) {}
   virtual ~Display() {}

   virtual REC_TYPE RecType() const { return _DISPLAY; }
   virtual Pvoid RecData() { return (Pvoid)(_Display OM_FAR *)this; }

   STOREDIN(poly_db);
   OWNEROF(Rectangl, ALLSHAPES);
   OWNEROF(Circle, ALLSHAPES);
   OWNEROF(Triangle, ALLSHAPES);
   OWNEROF(AllShapes, ALLSHAPES);

   void PreInit();
};


// This is our abstract base class
// define all polymorphic functions here
class OM_EXPORT Shape : public StoreObj
{
public:
   Shape(const PVistaDb pdb=poly_db::DbPtr) : StoreObj(pdb) {}
   Shape(const RSetObj set, const PVistaDb pdb=poly_db::DbPtr) : StoreObj(set, pdb) {}

   STOREDIN(poly_db);

   virtual void Show() = 0;
   virtual void UserNew() = 0;
};


class OM_EXPORT Rectangl : public Shape, public _Rectangle
{
public:
   Rectangl(const PVistaDb pdb=poly_db::DbPtr) : Shape(pdb) {}
   Rectangl(const RSetObj set, const PVistaDb pdb=poly_db::DbPtr) : Shape(set, pdb) {}
   Rectangl(const RDisplay disp, const PVistaDb pdb=poly_db::DbPtr) : Shape(SetObj(ALLSHAPES), pdb) {}

   virtual REC_TYPE RecType() const { return _RECTANGLE; }
   virtual Pvoid RecData() { return (Pvoid)(_Rectangle OM_FAR *)this; }

   MEMBEROF(Display, ALLSHAPES);

   virtual void Show();
   virtual void UserNew();
   
   void NewRect(short x, short y, short h, short w );
   void SetRect(short x, short y, short h, short w );
};


class OM_EXPORT Circle : public Shape, public _Circle
{
public:
   Circle(const PVistaDb pdb=poly_db::DbPtr) : Shape(pdb) {}
   Circle(const RSetObj set, const PVistaDb pdb=poly_db::DbPtr) : Shape(set, pdb) {}
   Circle(const RDisplay disp, const PVistaDb pdb=poly_db::DbPtr) : Shape(SetObj(ALLSHAPES), pdb) {}

   virtual REC_TYPE RecType() const { return _CIRCLE; }
   virtual Pvoid RecData() { return (Pvoid)(_Circle OM_FAR *)this; }

   MEMBEROF(Display, ALLSHAPES);

   virtual void Show();
   virtual void UserNew();
   
   void NewCircle(short x, short y, short d);
   void SetCircle(short x, short y, short d);
};


class OM_EXPORT Triangle : public Shape, public _Triangle
{
public:
   Triangle(const PVistaDb pdb=poly_db::DbPtr) : Shape(pdb) {}
   Triangle(const RSetObj set, const PVistaDb pdb=poly_db::DbPtr) : Shape(set, pdb) {}
   Triangle(const RDisplay disp, const PVistaDb pdb=poly_db::DbPtr) : Shape(SetObj(ALLSHAPES), pdb) {}

   virtual REC_TYPE RecType() const { return _TRIANGLE; }
   virtual Pvoid RecData() { return (Pvoid)(_Triangle OM_FAR *)this; }

   MEMBEROF(Display, ALLSHAPES);

   virtual void Show();
   virtual void UserNew();
   
   void NewTri(short xa, short ya, short xb, short yb, short xc, short yc); 
   void SetTri(short xa, short ya, short xb, short yb, short xc, short yc); 
};


//-----------------------------------------------------------
// To implement a Polymorphic set, each of the member objects
// is included in the Polymorph subclass (AllShapes).  Then 
// each object is Register()ed in the constuctor to this class.
//------------------------------------------------------------
class OM_EXPORT AllShapes : public Polymorph
{
private:
   Rectangl R;
   Triangle T;
   Circle C;
   
public:
   AllShapes() : Polymorph(ALLSHAPES, poly_db::DbPtr)
      { Register(R); Register(T); Register(C); }

   RAllShapes operator = (const RAllShapes ref)
      { ((RStoreObj)*this) = ((RStoreObj)ref); return *this; }

   STOREDIN(poly_db);
   MEMBEROF(Display, ALLSHAPES);

   PShape GetShape()   { return (PShape)GetObj(); }

   void   Show()       { GetShape()->Show(); }
   void   UserNew(const RDisplay);
};



