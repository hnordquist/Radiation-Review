#include "header.hpp"

// Navigate the Info Object 
int NavigateInfoObj()
{   
   char input[10];
   int result;                 
   for (result = 0, *input = 0; result == 0 && toupper(*input) != 'M';)
   {
      printf("\n\nNavigation Type:\n");
      printf("   1 - Direct Navigation\n");
      printf("   2 - Relational Navigation\n");
      printf("   3 - Network Navigation \n"); 
      printf("   M - Main Menu\n");
      printf("\nEnter choice: "); 
      gets(input);
      switch(*input)
      { 
         case '1':
            result = DNavInfoObj();
            break;
         case '2':
            result = RNavInfoObj();
            break; 
         case '3':
            result = NNavInfoObj();
            break; 
         case 'M': 
         case 'm':
            break;
         default:
            printf("\n*** incorrect command -- re-enter\n");
            break;
      }
   }
  return result;    
}
int DNavInfoObj()
{
   GroupLock   lock;           
   InfoObj     info;
   char        input[81];

   
  
   lock.AddRead(info);
   if (lock.Lock() != S_OKAY)
      return (1);
   // Display all info Objects
   for (info[FIRST] ; info.Okay(); info++) {
         printf("    %s, %s, %s, %s, %d", info.idcode,info.title, 
                                       info.publisher,info.pubdate, info.type); 
         printf("\n");
      }
   lock.Unlock();
   if (info.GetStatus() != S_NOTFOUND)
   {
      return (1);   
   }

   printf("--- press <enter> to continue");
   gets(input);
   return (0);
}

int RNavInfoObj()
{
   GroupLock   lock;          
   InfoObj     info(KeyObj(INFO_IDCODE));
   char        input[81];                
   KeyObj      infoKey(INFO_IDCODE, input);
   int result = 0;
   printf("Enter Key (idcode) : ");
   if (gets(input), !*input)   
        return(EOF);

   if (!info.ReadLock())
        return (1);
         
   //Find Info
   info.Find(infoKey);
   
   //Check the status of Find() call
   if(!info.Okay()) 
       printf("\n\tKey not Found\n");
   
   else if(info.Okay())
   printf("    %s, %s, %s, %s, %d ", info.idcode,info.title, 
                                 info.publisher,info.pubdate, info.type); 
   
   else
         return 1;  
   
   for (result = 0, *input = 0; result == 0 && toupper(*input) != 'M';)
   {
         printf("\nRelational Navigation Commands\n");
         printf("   N - Display Next");
         printf("   P - Display Previous\n");
         printf("   M - Main Menu\n");
         printf("\n");
         printf("enter command: ");
         gets(input);
         switch (*input) 
         {
            case 'm':
            case 'M':
                break;
            case 'N': 
            case 'n':  
				  if(info++, info.Okay())
                  {
                      printf("  %s, %s, %s, %s ",info.idcode,info.title, info.publisher,info.pubdate);
                      result = 0;
                  }
                  else //(info.GetStatus() == S_NOTFOUND)
                  {
                     printf("No More Records\n"); 
                     result = 1;  
                  } 
                  break;
           case 'P': 
           case 'p': 
                 if(info--,info.Okay())
                 { 
                     info--;
                     printf("    %s, %s, %s, %s ", info.idcode,info.title, info.publisher,info.pubdate);
                     result = 0;
                 }
                 else //(info.GetStatus() == S_NOTFOUND)
                 {
                     printf("No More Records\n"); 
                     result = 1;
                 } 
               break;
               
               default:
                  printf("\n*** incorrect command -- re-enter\n");
               break;
         }
   }
                 
   info.FreeLock();
   return 0;
 }
 

int NNavInfoObj()
{
   GroupLock   lock;           
   AuthorObj   author;//(SetObj(HASPUBLISHED));
   InfoObj     info;//(SetObj(HASPUBLISHED));
   char        input[81];

   
  
   lock.AddRead(info);
   // Work with rdm
   //lock.AddRead(author)
   lock.AddRead(20000,(const StoreObj&)author); 
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

   if (!author.Okay()) {
       printf( "Author not in the database\n");
       return 1;
    }
    
   if (lock.Lock() != S_OKAY)
      return (1);
   // Display all info Objects
   for (author >> info ; info.Okay(); info++) {
   {
      printf("    %s, %s ",info.idcode, info.title); 
      printf("\n"); 
   }
}
  
   lock.Unlock();
   printf("--- press <enter> to continue");
   gets(input);
   return (0);
}