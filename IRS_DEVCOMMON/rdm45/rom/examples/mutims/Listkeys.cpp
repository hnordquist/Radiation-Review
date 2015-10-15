#include "header.hpp"

int ListKeys()
{
   char          input[81];
   KeyObj        key(KEYWORD_WORD, input);
   KeywordObj    keyword(key);

   printf("start keyword: ");
   if (gets(input), *input)
      input[SIZEOF_KEYWORD_WORD - 1] = 0;

   // lock the Keyword class
   if (keyword.ReadLock() == False)
      return (1);
   // if no start keyword, show all
   if (!*input)
      keyword[FIRST];
   else {
      if (keyword.Find(key).GetStatus() == S_NOTFOUND) {
         // if the keyword wasn't found, go to next one 
         // increment keyword to the next keyword after 
         // the location it would have been
         keyword++;
      }
   }

   if (!keyword.Okay() && keyword.GetStatus() != S_NOTFOUND) {
      keyword.FreeLock();
      return (1);
   }

   for (; keyword.Okay(); keyword++)
      printf("    %s\n", keyword.word);

   keyword.FreeLock();

   // if all went well, the status should be S_NOTFOUND
   if (keyword.GetStatus() != S_NOTFOUND)
      return (1);

   printf("--- press <enter> to continue");
   gets(input);
   return (0);
}

