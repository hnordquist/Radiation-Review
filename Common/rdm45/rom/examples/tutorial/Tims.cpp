#include "header.hpp"

/*#include <strstrea.h>
#include "sqlvel.h"
#include <string.h>
#include "SQLColAttribute.hpp"	 */


// initialize the database (in one module only)
DB_INIT(tutorial);

// create a task
TutorialTask *task = (TutorialTask*)NULL;

void main()
{
   char   input[20];
   int    result;

   task = new TutorialTask("s", "USER1", ".\\");
   if (task == NULL || !task->Okay())
      return;

   for (result = 0, *input = 0; result == 0 && toupper(*input) != 'Q';) {
      printf("\n\n\nTutorial Commands:\n");
      printf("   1 - Display list of authors sorted by last name\n");
      printf("   2 - Display info \n");
      printf("   3 - Enter new Author \n");  
      printf("   4 - Enter new Info \n");
      printf("   5 - Delete author\n");
      printf("   6 - Delete info\n");
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
            result = NavigateInfoObj();
            break;
         case '3':
            result = EnterAuthor();
            break;
         case '4':
            result = EnterInfo();
            break;
         case '5':
            result = DeleteAuthor();
            break;
         case '6':
            result = DeleteInfo();
            break;
         default:
            printf("\n*** incorrect command -- re-enter\n");
            break;
      }
   }

   delete task;

   if (result != 0)
      printf("\nERROR!\n");
   return;
}
 
