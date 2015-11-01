#include "header.hpp"

int ListAuthors()
{
   GroupLock   lock;
   AuthorObj   author;
   char        input[81];
   lock.AddRead(author);
   if (lock.Lock() != S_OKAY)
      return (1);

   // loop through all remaining authors (if any)
  // system >> author;
   for (author[FIRST]; author.Okay(); author++)
      printf("    %s, %s\n", author.lastname, author.firstname);

   lock.Unlock();
   if (author.GetStatus() != S_NOTFOUND)
      return (1);

   printf("--- press <enter> to continue");
   gets(input);
   return (0);
}

