#include "header.hpp"


static int getNewInfo(InfoObj& info)
{
   
   char      input[81];
   Info_idcode_KeyObj    infoKey;

for(;;)
{   
   
    for (;;)
    {
        printf("id_code  : ");
        if (gets(input), !*input)
          return (EOF);
        memset(info.idcode, 0, SIZEOF_INFO_IDCODE);
        strncpy(info.idcode, input, SIZEOF_INFO_IDCODE - 1);  
        strncpy(infoKey.idcode, input, SIZEOF_INFO_IDCODE - 1);
        if (!info.ReadLock())
            return (EOF);
         // see if the idcode is a duplicate
        info.Find(infoKey);
        info.FreeLock();
         // check the status of the Find() call
        if (info.Okay()) 
            printf("duplicate id_code: %s\n", info.idcode);
        else
           break;
    }  

//Title (Mandatory)
   printf("info:: Title  : ");
   if (gets(input), !*input)
         return (EOF);
      
   memset(info.title, 0, SIZEOF_INFO_TITLE);
   strncpy(info.title, input, SIZEOF_INFO_TITLE - 1); 
   
//Publisher  (optional)   
   printf("info:: Publisher  : ");
   gets(input);   
   memset(info.publisher, 0, SIZEOF_INFO_PUBLISHER);
   strncpy(info.publisher, input, SIZEOF_INFO_PUBLISHER - 1);

   printf("info:: Pubdate  : ");
   gets(input);
   memset(info.pubdate, 0, SIZEOF_INFO_PUBDATE);
   strncpy(info.pubdate, input, SIZEOF_INFO_PUBDATE - 1);

   for (;;) 
   {
      printf("info type (0 - book, 1 = magazine, 2 = article): ");
      gets(input);
      sscanf(input, "%d", &info.type);
      if (info.type >= 0 && info.type <= 2)
           break;
      printf("invalid info type - correct types are:\n");
      printf("0 - book, 1 = magazine, 2 = article\n"); 
            
   }   
   break;
  } 
  return (0); 
}

int EnterInfo()
{
   char                    input[81];
   GroupLock               lock1;
   TransAction             trans;
   AuthorObj               author;
   InfoObj				   info(SetObj(HASPUBLISHED));
   short AStatus;

   while (AStatus = getNewInfo(info) != EOF) 
   {   
    
      if (EnterAuthor(author))
      {  
         printf("Could not create new Author\n");
         return (1);
      }
     
      if (trans.Begin("EnterInfo") == False)
         return (1); 
      lock1.AddWrite(SetObj(HASPUBLISHED), author);
      lock1.AddWrite(info);                                              
      
      if (lock1.Lock() != S_OKAY) 
      {  
         trans.Abort();
         return (1);
      }                                                   
      if(!info.NewObj().Okay()||!info.Connect(author)) 
      {
          printf("Could not Connect set HASPUBLISHED\n"); 
          trans.Abort();
          return (1);
      }
     
      trans.End(lock1);   
   }     
      return 0;
}

 int EnterAuthor(AuthorObj& author)
{
   char                    input[81];
   GroupLock               lock1;
   TransAction             trans;
   short                   AStatus = 1;
   lock1.AddWrite(SetObj(HASPUBLISHED), author);
   
   if(!GetData(author)) 
   {
      if (trans.Begin("EnterInfo") == False)
         return (1);
      
      if (lock1.Lock() != S_OKAY) 
      {
         trans.Abort();
         return (1);
      }
            
      if (!author.NewObj().Okay()) 
      {
         printf("Could not create new Author\n"); 
         trans.Abort();
         return (1);
      }  
      trans.End(lock1);
     }     
      return 0;
}
