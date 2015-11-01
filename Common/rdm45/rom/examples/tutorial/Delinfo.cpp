#include "header.hpp"

int DeleteInfo()
{
   char                    input[81];
   GroupLock               lock;
   TransAction             trans;
   KeyObj                  key(INFO_IDCODE, input);
   InfoObj                 info(key);
   AuthorObj               author;

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

   printf("author: %s, %s\n",author.firstname, author.lastname);
   printf("title : %s\n", info.title);
   printf("delete (y/n)? ");
   if (gets(input), toupper(*input) != 'Y')
      return (0);

   lock.Reset();
   lock.AddWrite(author);
   lock.AddWrite(SetObj(HASPUBLISHED), author);
   lock.AddWrite(info);

   if (trans.Begin("DeleteInfo") == False)
      return (1);
   if (lock.Lock() != S_OKAY) {
      trans.Abort();
      return (1);
   }

   if ((info << author).Okay()) 
   {
      if (!info.DisConnect(author) || !info.Delete().Okay()) 
      {
         trans.Abort();
         return (1);
      }

   }

   trans.End();
   return (0);
}

