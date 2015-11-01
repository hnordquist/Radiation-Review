#include "header.hpp"

 int GetData(AuthorObj &author)
{
   char      input[81];
   Author_fullname_KeyObj authKey;

   for (;;) {
      printf("author:: first name   : ");
      if (gets(input), !*input)
         return (EOF);
      memset(author.firstname, 0, SIZEOF_AUTHOR_FIRSTNAME);
      strncpy(author.firstname, input, SIZEOF_AUTHOR_FIRSTNAME - 1);
      //Copy key feild first name
      strncpy(authKey.firstname, input, SIZEOF_AUTHOR_FIRSTNAME - 1);   
      printf("author:: last name  : ");
      if (gets(input), !*input)
         return (EOF);
      memset(author.lastname, 0, SIZEOF_AUTHOR_LASTNAME);
      strncpy(author.lastname, input, SIZEOF_AUTHOR_LASTNAME - 1);
      //Copy key field lastname
      strncpy(authKey.lastname, input, SIZEOF_AUTHOR_LASTNAME - 1);
      if (!author.ReadLock())
      return (EOF);
      // see if the fullname is duplicate
      author.Find(authKey);
      author.FreeLock();
      // check the status of the Find() call
      if (author.Okay()) 
          //printf("duplicate author name: %s, %s\n", author.firstname,
          //                                          author.lastname);  
          return 1;
      else
          break;
   }                                                                        
   return (0);
}

int EnterAuthor()
{
   char                    input[81];
   GroupLock               lock1, lock2, lock3;
   TransAction             trans;
   AuthorObj               author,
                           newauthor;
   InfoObj				   info;  
   short				   AStatus;
   lock1.AddWrite(SetObj(HASPUBLISHED), author);
   lock1.AddWrite(info);


   while ((AStatus = GetData(newauthor)) != EOF) 
   {
      if (trans.Begin("EnterAuthor") == False)
         return (1);
      if (lock1.Lock() != S_OKAY) 
      {
         trans.Abort();
         return (1);
      }
  // You can Create a this way also !!
        /* if (!newauthor.NewMemberOf(system).Okay()) {
            trans.Abort();
            return (1);   */
      if (AStatus)
      {
          printf("Duplicate Author\n");
          return 1;
      }
           
      if (!newauthor.NewObj().Okay()) 
      {
         printf("Could not create new Author\n"); 
         trans.Abort();
         return (1);
      }  
      trans.End(lock1);
     }     
      return 0;
}


