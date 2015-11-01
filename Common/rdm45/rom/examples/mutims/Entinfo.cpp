#include "header.hpp"


static int GetInfo(AuthorObj &author, InfoObj &info)
{
   char      input[81];
   KeyObj    key(INFO_IDCODE, input);

   for (;;) {
      printf("author   : ");
      if (gets(input), !*input)
         return (EOF);
      memset(author.name, 0, SIZEOF_AUTHOR_NAME);
      strncpy(author.name, input, SIZEOF_AUTHOR_NAME - 1);

      for (;;) {
         printf("id_code  : ");
         gets(input);
         memset(info.idcode, 0, SIZEOF_INFO_IDCODE);
         strncpy(info.idcode, input, SIZEOF_INFO_IDCODE - 1);
         if (!info.ReadLock())
            return (EOF);
         // see if the idcode is a duplicate
         info.Find(key);
         info.FreeLock();
         // check the status of the Find() call
         if (info.Okay()) 
            printf("duplicate id_code: %s\n", info.idcode);
         else
            break;
      }

      printf("title    : ");
      gets(input);
      memset(info.title, 0, SIZEOF_INFO_TITLE);
      strncpy(info.title, input, SIZEOF_INFO_TITLE - 1);

      printf("publisher: ");
      gets(input);
      memset(info.publisher, 0, SIZEOF_INFO_PUBLISHER);
      strncpy(info.publisher, input, SIZEOF_INFO_PUBLISHER);

      printf("pub. date: ");
      gets(input);
      memset(info.pubdate, 0, SIZEOF_INFO_PUBDATE);
      strncpy(info.pubdate, input, SIZEOF_INFO_PUBDATE - 1);

      for (;;) {
         printf("info type (0 - book, 1 = magazine, 2 = article): ");
         gets(input);
         sscanf(input, "%d", &info.type);
         if (info.type >= 0 && info.type <= 2)
            break;
         printf("invalid info type - correct types are:\n");
         printf("0 - book, 1 = magazine, 2 = article\n");
      }

      info.loaned = (char)False;

      printf("enter data (y/n)? ");
      gets(input);
      if (toupper(*input) == 'Y')
         break;
   }
   return (0);
}

int EnterInfo()
{
   char                    input[81];
   GroupLock               lock1, lock2, lock3;
   TransAction             trans;
   systemObj               system;
   AuthorObj               author(SetObj(AUTHORLIST)),
                           newauthor(SetObj(AUTHORLIST));
   InfoObj                 info(SetObj(HASPUBLISHED));
   KeyObj                  key(KEYWORD_WORD, input);
   KeywordObj              keyword;
   Keyword_IntersectObj    intersect;
   AbstractObj             abstract;

   lock1.AddWrite(SetObj(AUTHORLIST), system);
   lock1.AddWrite(SetObj(HASPUBLISHED), author);
   lock1.AddWrite(info);

   lock2.AddWrite(SetObj(KEYTOINFO), keyword);
   lock2.AddWrite(SetObj(INFOTOKEY), info);
   lock2.AddWrite(keyword);

   lock3.AddWrite(SetObj(ABSTRACT), info);

   while (GetInfo(newauthor, info) != EOF) {
      if (trans.Begin("EnterInfo") == False)
         return (1);
      if (lock1.Lock() != S_OKAY) {
         trans.Abort();
         return (1);
      }

      // once again, this should be implemented as an index on author name, 
      // but we're showing the system set capability
      for (system >> author; author.Okay(); ++author) {
         if (strcmp(author.name, newauthor.name) == 0)
            break;
      }

      if (!author.Okay()) {
         if (author.GetStatus() != S_EOS) {
            trans.Abort();
            return (1);
         }

         if (!newauthor.NewMemberOf(system).Okay()) {
            trans.Abort();
            return (1);
         }

         // arghh! this gets really clunky due to the system set search
         author = newauthor;
         if (!author.Okay()) {
            trans.Abort();
            return (1);
         }
      }

      if (!info.NewMemberOf(author).Okay()) {
         trans.Abort();
         return (1);
      }

      trans.End(lock1);

      for (;;) {
         printf("key word: ");
         if (gets(input), !*input)
            break;

         if (trans.Begin("EnterInfo - key") == False)
            return (1);
         if (lock2.Lock() != S_OKAY) {
            trans.Abort();
            return (1);
         }

         if (keyword.Find(key).GetStatus() == S_NOTFOUND) {
            memset(keyword.word, 0, SIZEOF_KEYWORD_WORD);
            strncpy(keyword.word, input, SIZEOF_KEYWORD_WORD - 1);
            keyword.NewObj();
         }
         if (!keyword.Okay()) {
            trans.Abort();
            return (1);
         }

         intersect.infotype = info.type;
         if (!intersect.NewObj().Okay() || !intersect.Connect(keyword) || !intersect.Connect(info)) {
            trans.Abort();
            return (1);
         }
         trans.End(lock2);
      }

      if (!abstract.ReadyBuffer(2048))
         return (1);

      // Get our blob buffer
      Pchar ptr = (Pchar)abstract.GetBuffer();
      char  tmp[256];

      printf("abstract: ");
      while ((ptr - (Pchar)abstract.GetBuffer()) < 2048)
      {
         gets(tmp);
         if (!*tmp)
            break;

         om_strcat(tmp, "\0");
         om_strcpy(ptr, tmp);
         ptr += om_strlen(tmp);
      }

      if (trans.Begin("EnterInfo - abstract text") == False)
         return (1);
      if (lock3.Lock() != S_OKAY) {
         trans.Abort();
         return (1);
      }

      // tell the blob how much of the 2048 was used
      abstract.SetAmtUsed(ptr - (Pchar)abstract.GetBuffer());

      // save the blob
      if (!abstract.Write(&info)) {
         trans.Abort();
         return (1);
      }

      trans.End(lock3);
   }
   return (0);
}

