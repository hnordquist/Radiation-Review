#include "header.hpp"


DB_INIT(mutims);

TIMSTask       *task = (TIMSTask*)NULL;

main()
{
   char   input[20];
   int    result;

   task = new TIMSTask("s", "USER1", ".\\");
   if (task == NULL || !task->Okay())
      return (1);

   for (result = 0, *input = 0; result == 0 && toupper(*input) != 'Q';) {
      printf("\n\n\nTIMS Commands:\n");
      printf("   1 - Display list of authors\n");
      printf("   2 - Display list of keywords\n");
      printf("   3 - List publications by author\n");
      printf("   4 - List publications by keyword\n");
      printf("   5 - Enter new publication\n");
      printf("   6 - Delete publication\n");
      printf("   7 - Loan a publication\n");
      printf("   8 - Return loaned publication\n");
      printf("   9 - List loaned publications\n");
      printf("\n");
      printf("   q - Quit\n");
      printf("\nenter command: ");
      gets(input);
      switch (*input) {
         case 'q':
         case 'Q':
            break;
         case '1':
            result = ListAuthors();
            break;
         case '2':
            result = ListKeys();
            break;
         case '3':
            result = ListPubsByAuthor();
            break;
         case '4':
            result = ListPubsByKey();
            break;
         case '5':
            result = EnterInfo();
            break;
         case '6':
            result = DeleteInfo();
            break;
         case '7':
            result = LoanBook();
            break;
         case '8':
            result = ReturnBook();
            break;
         case '9':
            result = ListLoaners();
            break;
         default:
            printf("\n*** incorrect command -- re-enter\n");
            break;
      }
   }

   delete task;

   if (result != 0)
      printf("\nERROR!\n");
   return (result != 0);
}

