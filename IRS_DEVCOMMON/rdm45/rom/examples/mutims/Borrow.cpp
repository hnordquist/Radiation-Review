#include "header.hpp"

int LoanBook()
{
   char                    input[81];
   GroupLock               lock;
   TransAction             trans;
   systemObj               system;
   KeyObj                  idcode(INFO_IDCODE, input);
   InfoObj                 info(idcode);
   BorrowerObj             borrower;
   Borrower_IntersectObj   intersect;

   // display all available publications sorted by idcode
   printf("available publications:\n");
   if (info.ReadLock() == False)
      return (1);
   for (info[FIRST]; info.Okay(); info++) {
      if (!info.loaned)
         printf("%-8s %-70s\n", info.idcode, info.title);
   }
   if (info.FreeLock() == False)
      return (1);

   printf("\nenter id_code of publication to be loaned: ");
   if (gets(input), !*input)
      return (0);

   // locate particular info object by idcode
   if (info.ReadLock() == False)
      return (1);
   info.Find(idcode);
   if (info.FreeLock() == False)
      return (1);
   if (!info.Okay()) {
      if (info.GetStatus() == S_NOTFOUND)
         printf("id_code %s not on file\n", input);
      return (info.GetStatus() != S_NOTFOUND);
   }
   // make sure it wasn't loaned out while waiting for user input
   if (info.loaned) {
      printf("publication %s is not available\n", input);
      return (0);
   }

   memset(borrower.name, 0, SIZEOF_BORROWER_NAME);
   printf("name of borrower: ");
   gets(input);
   strncpy(borrower.name, input, SIZEOF_BORROWER_NAME - 1);
   intersect.borrowed = intersect.returned = 0L;
   printf("   date borrowed: ");
   gets(input);
   sscanf(input, "%ld", &intersect.borrowed);

   lock.AddWrite(SetObj(BORROWERTOINFO), info);
   lock.AddWrite(SetObj(INFOTOBORROWER), info);
   lock.AddWrite(SetObj(LOANHISTORY), info);
   lock.AddWrite(borrower);
   lock.AddWrite(info);

   if (trans.Begin("LoanBook") == False)
      return (1);
   if (lock.Lock() != S_OKAY) {
      trans.Abort();
      return (1);
   }

   info.loaned = (char)True;

   if (!info.Write().Okay() ||         // update the loaned flag in info object
       !borrower.NewObj().Okay() ||    // create new borrower object
       !borrower.Connect(system) ||    // connect the borrower to the system set
       !intersect.NewObj().Okay() ||   // create new intersect between borrower and info
       !intersect.Connect(info) ||     // connect intersect to info                                                   
       !intersect.Connect(borrower)) { // connect intersect to borrower
	  trans.Abort();
	  return (1);
   }

   trans.End();
   return (0);
}

int ReturnBook()
{
   char                    input[81];
   GroupLock               lock;
   TransAction             trans;
   KeyObj                  idcode(INFO_IDCODE, input);
   InfoObj                 info(idcode);
   BorrowerObj             borrower;
   Borrower_IntersectObj   intersect(SetObj(INFOTOBORROWER));

   // display all loaned publications sorted by idcode
   printf("\ncurrently loaned publications:\n");
   if (info.ReadLock() == False)
      return (1);
   for (info[FIRST]; info.Okay(); info++) {
      if (info.loaned)
         printf("%-8s %-70s\n", info.idcode, info.title);
   }
   if (info.FreeLock() == False)
      return (1);

   printf("\nenter id_code of publication to be returned: ");
   if (gets(input), !*input)
      return (0);

   // locate particular info object by idcode
   if (info.ReadLock() == False)
      return (1);
   info.Find(idcode);
   if (info.FreeLock() == False)
      return (1);
   if (!info.Okay()) {
      if (info.GetStatus() == S_NOTFOUND)
         printf("id_code %s not on file\n", input);
      return (info.GetStatus() != S_NOTFOUND);
   }
   // make sure it wasn't returned in the mean time
   if (!info.loaned) {
      printf("publication %s is not currently loaned out\n", input);
      return (0);
   }

   lock.AddRead(SetObj(INFOTOBORROWER), info);
   if (lock.Lock() != S_OKAY)
      return (1);
   // locate the last loan occurence for this publication
   if (!info.MakeOwnerOf(intersect) || !intersect[LAST].Okay())
      return (1);
   if (lock.Unlock() != S_OKAY)
      return (1);

   printf("book borrowed on: %ld\n", intersect.borrowed);
   printf("date returned   : ");
   gets(input);
   sscanf(input, "%ld", &intersect.returned);
   info.loaned = (char)False;

   lock.Reset();
   lock.AddWrite(SetObj(INFOTOBORROWER), info);
   lock.AddWrite(info);

   if (trans.Begin("ReturnBook") == False)
      return (1);
   if (lock.Lock() != S_OKAY) {
      trans.Abort();
      return (1);
   }

   if (!intersect.Write().Okay() ||    // update the intersect object
       !info.Write().Okay()) {         // update the loaned flag in the info object
      trans.Abort();
      return (1);
   }

   trans.End();
   return (0);
}

int ListLoaners()
{
   GroupLock               lock;
   systemObj               system;
   InfoObj                 info(KeyObj(INFO_IDCODE));
   BorrowerObj             borrower;
   Borrower_IntersectObj   intersect;
   short                   ct;

   lock.AddRead(SetObj(INFOTOBORROWER), info);
   lock.AddRead(SetObj(BORROWERTOINFO), info);
   lock.AddRead(info);
   if (lock.Lock() != S_OKAY)
      return (1);

   // display all loaned publications sorted by idcode
   printf("\n");
   for (ct = 0, info[FIRST]; info.Okay(); info++) {
      if (info.loaned) {
         if (!info.MakeOwnerOf(intersect) ||      // make the info object the owner of the set
             !intersect[LAST].Okay() ||         // load the last occurence of the intersect
             !(intersect << borrower).Okay())   // navigate from intersect to the borrower
            return (1);
         printf(" id_code: %s\n", info.idcode);
         printf("   title: %s\n", info.title);
         printf("borrower: %s\n", borrower.name);
         printf("    date: %ld\n\n", intersect.borrowed);
         ct++;
      }
   }

   if (lock.Unlock() != S_OKAY)
      return (1);
   if (!ct)
      printf("no books are currently loaned out.\n");
   return (info.GetStatus() != S_NOTFOUND);
}

