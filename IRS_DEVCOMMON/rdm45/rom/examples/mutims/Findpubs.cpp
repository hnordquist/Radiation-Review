#include "header.hpp"


int ListPubsByKey()
{
   char                  input[81];
   GroupLock             lock;
   KeyObj                key(KEYWORD_WORD, input);
   KeywordObj            keyword(key);
   Keyword_IntersectObj  intersect(SetObj(KEYTOINFO));
   InfoObj               info;
   AuthorObj             author;
   AbstractObj           abstract;

   printf("key word: ");
   if (gets(input), !*input)
      return (0);

   // Lock Keyword class
   if (keyword.ReadLock() == False)
      return (1);
   // do lookup on Keyword class
   keyword.Find(key);
   // Free Keyword class
   if (keyword.FreeLock() == False)
      return (1);

   // check status from keyword.Find()
   if (!keyword.Okay()) {
      if (keyword.GetStatus() == S_NOTFOUND)
         printf("no records found\n");
      return (keyword.GetStatus() != S_NOTFOUND);
   }

   lock.AddRead(SetObj(KEYTOINFO), keyword);
   lock.AddRead(SetObj(INFOTOKEY), info);
   lock.AddRead(SetObj(HASPUBLISHED), author);
   lock.AddRead(SetObj(ABSTRACT), info);
   if (lock.Lock() != S_OKAY)
      return (1);

   // navigate the many-to-many set from keyword to info
   for (keyword >> intersect; intersect.Okay(); intersect++) {
      (intersect << info) << author;
      if (!info.Okay() || !author.Okay()) {
         lock.Unlock();
         return (1);
      }

      printf("id_code: %s\n", info.idcode);
      printf("author : %s\n", author.name);
      printf("title  : %s\n", info.title);
      printf("publ.  : %s, %s\n", info.publisher, info.pubdate);

      // display all keywords for this publication
      if (info.Members(Keyword_IntersectObj())) {
         Keyword_IntersectObj   intersect(SetObj(INFOTOKEY));
         KeywordObj             keyword;

         printf("key words:\n----------\n");
         for (info >> intersect; intersect.Okay(); ++intersect) {
            if (!(intersect << keyword).Okay()) {
               lock.Unlock();
               return (1);
            }
            printf(" %s\n", keyword.word);
         }

         // if all went well, status should be S_EOS
         if (intersect.GetStatus() != S_EOS) {
            lock.Unlock();
            return (1);
         }
         printf("\n");
      }

      // load and display the variable length text object
      if ((info >> abstract).Okay()) {
         printf("abstract:\n---------\n");
         printf("   %s\n\n", abstract.GetBuffer());
      }
      else if (abstract.GetStatus() != S_EOS) {
         lock.Unlock();
         return (1);
      }

      if (lock.Unlock() != S_OKAY)
         return (1);

      printf("--- press <enter> to continue");
      gets(input);

      if (lock.Lock() != S_OKAY)
         return (1);
   }

   if (lock.Unlock() != S_OKAY)
      return (1);

   return (intersect.GetStatus() != S_EOS);
}


int ListPubsByAuthor()
{
   GroupLock    lock;
   systemObj    system;
   AuthorObj    author(SetObj(AUTHORLIST));
   InfoObj      info(SetObj(HASPUBLISHED));
   AbstractObj  abstract;
   char         input[81];
   int          compare;

   printf("author: ");
   if (gets(input), !*input)
      return (0);
   input[SIZEOF_AUTHOR_NAME - 1] = 0;

   lock.AddRead(SetObj(AUTHORLIST), system);
   if (lock.Lock() != S_OKAY)
      return (1);

   // this demonstrates the use of the system set
   // a better implementation would have been to index the
   // author name and simply do a Find on it
   for (system >> author; author.Okay(); author++) {
      if ((compare = strcmp(input, author.name)) <= 0) {
         if (compare < 0)
            author.SetStatus(S_EOS);
         break;
      }
   }

   if (lock.Unlock() != S_OKAY)
      return (1);

   // this is also clunky because we're demonstrating the system set
   if (!author.Okay()) {
      if (author.GetStatus() == S_EOS)
         printf("author record not found\n");
      return (author.GetStatus() != S_EOS);
   }

   lock.AddRead(SetObj(HASPUBLISHED), author);
   lock.AddRead(SetObj(KEYTOINFO), info);
   lock.AddRead(SetObj(INFOTOKEY), info);
   lock.AddRead(SetObj(ABSTRACT), info);
   if (lock.Lock() != S_OKAY)
      return (1);

   for (author >> info; info.Okay(); info++) {
      printf("id_code: %s\n", info.idcode);
      printf("author : %s\n", author.name);
      printf("title  : %s\n", info.title);
      printf("publ.  : %s, %s\n", info.publisher, info.pubdate);

      if (info.Members(Keyword_IntersectObj())) {
         Keyword_IntersectObj   intersect(SetObj(INFOTOKEY));
         KeywordObj             keyword;

         printf("key words:\n----------\n");
         for (info >> intersect; intersect.Okay(); intersect++) {
            if (!(intersect << keyword).Okay()) {
               lock.Unlock();
               return (1);
            }
            printf("    %s\n", keyword.word);
         }

         if (intersect.GetStatus() != S_EOS) {
            lock.Unlock();
            return (1);
         }
         printf("\n");
      }

      // load and display the variable length text object
      if ((info >> abstract).Okay()) {
         printf("abstract:\n---------\n");
         printf("   %s\n\n", abstract.GetBuffer());
      }
      else if (abstract.GetStatus() != S_EOS) {
         lock.Unlock();
         return (1);
      }

      if (lock.Unlock() != S_OKAY)
         return (1);

      printf("--- press <enter> to continue");
      gets(input);

      if (lock.Lock() != S_OKAY)
         return (1);
   }

   if (lock.Unlock() != S_OKAY)
      return (1);

   return (info.GetStatus() != S_EOS);
}

