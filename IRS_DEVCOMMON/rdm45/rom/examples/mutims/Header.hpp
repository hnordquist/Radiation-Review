#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#include <ctype.h>
#include "mutims.hpp"
#include <grlock.hpp>
#include <transact.hpp>


//
// Simple task class for the TIMS database.  Note the derivation and
// interaction with the ROMTask base class.
//

class TIMSTask : public StoreTask {
public:
   mutims timsdb;

   TIMSTask(char *mode, char *userid, char *path=(Pchar)NULL)
      : StoreTask(mode, ActivateTask("rmwds", "admin", "secret")) {

	  SetUserPaths(path, userid);
      lockcomm(LMC_INTERNAL);
      if (timsdb.Open() == False) {
         printf("Error: unable to open 'tims' database\n");
         SetStatus(timsdb.GetDbStatus());
      }
   }
};


extern TIMSTask *task;

int  ListPubsByAuthor();
int  ListPubsByKey();
int  DeleteInfo();
int  EnterInfo();
int  ListAuthors();
int  ListKeys();
int  ListLoaners();
int  LoanBook();
int  ReturnBook();

#endif

