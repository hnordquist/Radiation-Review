//-----------------------------------------------------
// This example illustrates the use of a VariLenText class
//-----------------------------------------------------

#include "vltxt_db.hpp"    // database structures from DDLP


//-----------------------------------------------------
// Task class defintion
//-----------------------------------------------------
class VLTextTask : public StoreTask
{
private:
   vltxt_db db;

public:
   VLTextTask(Pchar path) : StoreTask(ActivateTask("solaris","admin","secret"))
      {
         SetPaths(path, path, path, path, "admin");
         OpenDb();
         SetStatus(db.GetDbStatus());
      }
   ~VLTextTask() { CloseDb(); }

   int OpenDb()  { return db.Open();  }
   int CloseDb() { return db.Close(); }
};


//-----------------------------------------------------
// Document class
// The Document class is the "owner" of the VLText (Blob)
// and contains the name of the document
//-----------------------------------------------------
class MyDocument : public DocumentObj
{
public:
   MyDocument() {}
   MyDocument(RKeyObj key) : DocumentObj(key) {}
   ~MyDocument() {}

   Pchar GetName() { return Name; }
   void  Show()    { printf("Document: %s\n", Name); }
   T_F   UserNew();

   CUSTOMOPS(MyDocument);
};


//--------------------------------------------
// Text class
// The actual Blob
//--------------------------------------------
class MyVLText : public VLText_SetObj
{
public:
   MyVLText() {}
   MyVLText(const PStoreObj owner) : VLText_SetObj(owner) {}
   ~MyVLText() {}

   void Show();
   T_F  UserNew();
   long GetSize() const { return VLSize; }
};
