#include "header.hpp"

int DeleteInfo()
{
   char                    input[81];
   GroupLock               lock;
   TransAction             trans;
   systemObj               system;
   KeyObj                  key(INFO_IDCODE, input);
   InfoObj                 info(key);
   AuthorObj               author;
   BorrowerObj             borrower(SetObj(BORROWERTOINFO));
   AbstractObj             abstract;
   Keyword_IntersectObj    keyintersect(SetObj(INFOTOKEY));
   Borrower_IntersectObj   borintersect(SetObj(INFOTOBORROWER));
   KeywordObj              keyword;

   printf("id_code: ");
   if (gets(input), !*input)
      return (0);

   if (info.ReadLock() == False)
      return (1);
   info.Find(key);
   if (info.FreeLock() == False)
      return (1);

   if (!info.Okay()) {
      if (info.GetStatus() == S_NOTFOUND)
         printf("id_code %s not on file\n", input);
      return (info.GetStatus() != S_NOTFOUND);
   }

   lock.AddRead(SetObj(HASPUBLISHED), author);
   if (lock.Lock() != S_OKAY)
      return (1);
   info << author;
   if (lock.Unlock() != S_OKAY)
      return (1);

   if (!author.Okay())
      return (1);

   printf("author: %s\n", author.name);
   printf("title : %s\n", info.title);
   printf("delete (y/n)? ");
   if (gets(input), toupper(*input) != 'Y')
      return (0);

   lock.Reset();
   lock.AddWrite(SetObj(AUTHORLIST), author);
   lock.AddWrite(SetObj(INFOTOBORROWER), info);
   lock.AddWrite(SetObj(BORROWERTOINFO), borrower);
   lock.AddWrite(SetObj(LOANHISTORY), system);
   lock.AddWrite(SetObj(INFOTOKEY), info);
   lock.AddWrite(SetObj(KEYTOINFO), keyword);
   lock.AddWrite(SetObj(HASPUBLISHED), author);
   lock.AddWrite(borrower);
   lock.AddWrite(keyword);
   lock.AddWrite(info);
   lock.AddWrite(abstract);

   if (trans.Begin("DeleteInfo") == False)
      return (1);
   if (lock.Lock() != S_OKAY) {
      trans.Abort();
      return (1);
   }

   while ((info >> borintersect).Okay()) {
      if (!(borintersect << borrower).Okay()) {
         trans.Abort();
         return (1);
      }
      if (!borintersect.DisConnect(info) || !borintersect.DisConnect(borrower) || !borintersect.Delete().Okay()) {
         trans.Abort();
         return (1);
      }

      if (!borrower.Members(Borrower_IntersectObj())) {
         if (!borrower.DisConnect(system) || !borrower.Delete().Okay()) {
            trans.Abort();
            return (1);
         }
      }
   }
   if (borintersect.GetStatus() != S_EOS) {
      trans.Abort();
      return (1);
   }

   if (!abstract.Delete(&info)) { 
      trans.Abort();
      return (1);
   }

   while ((info >> keyintersect).Okay()) {
      if (!(keyintersect << keyword).Okay()) {
         trans.Abort();
         return (1);
      }
      if (!keyintersect.DisConnect(info) || !keyintersect.DisConnect(keyword) || !keyintersect.Delete().Okay()) {
         trans.Abort();
         return (1);
      }

      if (!keyword.Members(Keyword_IntersectObj())) {
         if (!keyword.Delete().Okay()) {
            trans.Abort();
            return (1);
         }
      }
   }
   if (keyintersect.GetStatus() != S_EOS) {
      trans.Abort();
      return (1);
   }

   if (!info.DisConnect(author) || !info.Delete().Okay()) {
      trans.Abort();
      return (1);
   }

   if (!author.Members(InfoObj())) {
      if (!author.DisConnect(system) || !author.Delete().Okay()) {
         trans.Abort();
         return (1);
      }
   }

   trans.End();
   return (0);
}

