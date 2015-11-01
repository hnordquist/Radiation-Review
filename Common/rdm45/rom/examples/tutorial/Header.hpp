#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#include <ctype.h>
#include "tutorial.hpp"
#include <grlock.hpp>
#include <transact.hpp>


//
// Simple task class for the tutorial database.  Note the derivation and
// interaction with the ROMTask base class.
//

class TutorialTask : public StoreTask {
public:
   tutorial tutdb;
   TutorialTask(char *mode, char *userid, char *path=(Pchar)NULL) : StoreTask(mode, ActivateTask("rds", "admin", "secret")) {
      SetUserPaths(path, userid);
      lockcomm(LMC_INTERNAL);
      if (tutdb.Open() == False) {
         printf("Error: unable to open 'tutorial' database\n");
         SetStatus(tutdb.GetDbStatus());
      }
   }
};


extern TutorialTask *task;

int  ListPubsByAuthor();
int  NavigateInfoObj();
int  RNavInfoObj();  //Relational Navigation
int  DNavInfoObj(); //Direct Navigation 
int  NNavInfoObj(); //Network Navigation
int  ListPubsByKey();
int  DeleteInfo(); 
int  DeleteAuthor();
int  EnterInfo();  
int  EnterAuthor(); 
int  EnterAuthor(AuthorObj& );
int  ListAuthors();
int  GetData(AuthorObj&);


#endif

