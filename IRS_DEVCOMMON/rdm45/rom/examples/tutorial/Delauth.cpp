#include "header.hpp"

int DeleteAuthor()
{
   char                    input[81];
   GroupLock               lock;
   TransAction             trans;
   InfoObj                 info(SetObj(HASPUBLISHED));
   AuthorObj               author;
   Author_fullname_KeyObj authKey;

   printf("author:: first name   : ");
     if (gets(input), !*input)
         return (EOF);
   
   memset(author.firstname, 0, SIZEOF_AUTHOR_FIRSTNAME);
   strncpy(author.firstname, input, SIZEOF_AUTHOR_FIRSTNAME - 1);
   //Copy key feild firstname
   strncpy(authKey.firstname, input, SIZEOF_AUTHOR_FIRSTNAME - 1);   
   
   printf("author:: last name  : ");
      if (gets(input), !*input)
         return (EOF);
      
   memset(author.lastname, 0, SIZEOF_AUTHOR_LASTNAME);
   strncpy(author.lastname, input, SIZEOF_AUTHOR_LASTNAME - 1);
   //Copy key field lastname
   strncpy(authKey.lastname, input, SIZEOF_AUTHOR_LASTNAME - 1);

   if (author.ReadLock() == False)
      return (1);
   author.Find(authKey);
  
   if (author.FreeLock() == False)
      return (1);

   if (!author.Okay()) 
   {
      printf("author: %s %s not on file\n", author.firstname, author.lastname);
      return (author.GetStatus() != S_NOTFOUND);
   }

   lock.AddRead(SetObj(HASPUBLISHED), author);
   if (lock.Lock() != S_OKAY)
      return (1);
   
   author >> info;
   if (lock.Unlock() != S_OKAY)
      return (1);
   
   
   //if (!info.Okay())
   //   return (1);
   
   lock.Reset();
   lock.AddWrite(author);
   lock.AddWrite(SetObj(HASPUBLISHED), author);
   lock.AddWrite(info);
  
   if (trans.Begin("DeleteAllInfo") == False)
      return (1);
   if (lock.Lock() != S_OKAY) {
      trans.Abort();
      return (1);
   }
   for(author >> info; info.Okay(); info++)
   {  
      int delInfo = 0;
      printf("name: %s\n", author.lastname);
      printf("title : %s\n", info.title);
      printf("delete (y/n)? ");
      if (gets(input), toupper(*input) == 'Y')
          delInfo = 1;
      
      if(delInfo)
      {
         if (!info.DisConnect(author) || !info.Delete().Okay()) 
         {
            trans.Abort();
            printf("abort transaction delete info\n");
            return (1);
         } 
         delInfo = 0;
      }
   } 
   
   if(!author.Members(InfoObj()))
   {
        if (!author.Delete().Okay()) 
        {
           trans.Abort();  
           printf("abort transaction delete author\n");
           return (1);  
        } 
   }
   
   trans.End();
   return (0);
}

