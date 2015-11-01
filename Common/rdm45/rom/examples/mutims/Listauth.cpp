#include "header.hpp"

int ListAuthors()
{
   GroupLock   lock;
   systemObj   system;
   AuthorObj   author(SetObj(AUTHORLIST));
   char        input[81];

   printf("start name: ");
   if (gets(input), *input)
      input[SIZEOF_AUTHOR_NAME - 1] = 0;
   
   // must lock the set to do the navigation from system to author
   lock.AddRead(SetObj(AUTHORLIST), system);
   if (lock.Lock() != S_OKAY)
      return (1);

   // loop through all authors searching for the start name
   for (system >> author; *input && author.Okay(); ++author) {
      if (strcmp(input, author.name) <= 0)
         break;
   }

   // loop through all remaining authors (if any)
   for (; author.Okay(); ++author)
      printf("    %s\n", author.name);

   lock.Unlock();
   if (author.GetStatus() != S_EOS)
      return (1);

   printf("--- press <enter> to continue");
   gets(input);
   return (0);
}

