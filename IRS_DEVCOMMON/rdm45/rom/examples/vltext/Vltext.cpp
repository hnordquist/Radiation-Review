#include "vltext.hpp"
#include <transact.hpp>
#include <time.h>

DB_INIT(vltxt_db);

void main()
{
   char tmp[60];

   // Instantiate a DB Task and Open the database.
   VLTextTask Apptask(".\\");

   // make sure task setup was okay
   if (!Apptask.Okay())
      return;

   MyDocument  g_document(KeyObj(DOCUMENT_NAME));
   MyVLText    g_text;
            
   do
   {
      printf("a) Show all f) find     l) list     e) enter\n");
      printf("I) import   E) export   D) DELETE   q) quit : ");
      gets(tmp);
      if (*tmp == 'q')
         break;

      switch (*tmp)
      {
         case 'a':
         {
            MyDocument  document(KeyObj(DOCUMENT_NAME));
            MyVLText    text;
            
            for (document[FIRST]; document.Okay(); document++)
            {
               document.Show();

               time_t start, end;
               time(&start);
               document >> text;
               time(&end);
               printf("Time for Read of blob of %ld bytes: %d seconds\n", text.GetSize(), end-start);

//               text.Show();
               printf("press enter for more...");
               gets(tmp);
            }
            printf("\n");
            break;
         }

         case 'f':
         {
            printf("Enter name of document to find: ");
            gets(tmp);

            g_document.Find(KeyObj(DOCUMENT_NAME,tmp));
            if (!g_document.Okay())
               printf("   document not found\n");
            else
            {
               time_t start, end;
               time(&start);
               g_document >> g_text;
               time(&end);
               printf("Time for Read of blob of %ld bytes: %d seconds\n", g_text.GetSize(), end-start);
//               g_text.Show();
            }
            break;
         }

         case 'l':
         {
            MyDocument  document(KeyObj(DOCUMENT_NAME));
            
            for (document[FIRST]; document.Okay(); document++)
               document.Show();
            break;
         }

         case 'e':
         {
            MyDocument  document(KeyObj(DOCUMENT_NAME));
            MyVLText    text(&document);
            
            // start a transaction for updating the database
            TransAction T("add blob to database", &Apptask);

            document.UserNew();
            if (document.Okay())
               text.UserNew();

            // when transaction goes out of scope (destructor)
            // the transaction is ended
            break;
         }

         case 'I':
         {
            MyDocument  document(KeyObj(DOCUMENT_NAME));
            MyVLText    text(&document);
            
            // start a transaction for updating the database
            TransAction T("add blob to database", &Apptask);

            document.UserNew();

            time_t start, end;
            time(&start);
            if (!document.Okay() || text.Import(document.GetName()) == False)
            {
               printf("Could not open file to import\n");
               // abort the transaction
               T.Abort();
               break;
            }
            time(&end);
            printf("Time for Write of blob of %ld bytes: %d seconds\n", text.GetSize(), end-start);

            // end the transaction
            T.End();
//            text.Show();
            break;
         }

         case 'E':
         {
            MyDocument  document(KeyObj(DOCUMENT_NAME));
            MyVLText    text;
            
            printf("Enter name of document to export: ");
            gets(tmp);

            document.Find(KeyObj(DOCUMENT_NAME,tmp));
            if (!document.Okay())
               printf("   document not found\n");
            else
            {
               printf("Enter name of file to export to (create): ");
               gets(tmp);
               if ((document >> text).Export(tmp) == True)
                  printf("   File '%s' has been created.\n", tmp);
               else
                  printf("   File '%s' could not be created.\n", tmp);
            }
            break;
         }

         case 'D':
         {
            MyDocument  document(KeyObj(DOCUMENT_NAME));
            MyVLText    text;
            
            printf("Enter name of document to delete: ");
            gets(tmp);

            document.Find(KeyObj(DOCUMENT_NAME,tmp));
            if (!document.Okay())
               printf("   document not found\n");
            else
            {
               // start a transaction for updating the database
               TransAction T("add blob to database", &Apptask);
               
               document >> text;

               printf("Deleting variable length text blob...");
               time_t start, end;
               time(&start);
               text.Delete();
//               text.DeleteAll(document);
               printf("Done\n");

               printf("Deleting document object...");
               document.Delete();
               printf("Done\n");
               time(&end);
               printf("Time for Delete of blob: %d seconds\n", end-start);
            }
            break;
         }
      }
      printf("\n");
   } while (1 == 1);
}



//-----------------------------------------------------
// Object Source
//-----------------------------------------------------

T_F MyDocument::UserNew()
{
   printf("Enter document name: ");
   gets(Name);

   Find(KeyObj(DOCUMENT_NAME, Name));
   if (!Okay())
      NewObj();
      
   return Okay();
}


void MyVLText::Show()
{
   printf("%s\n", GetBuffer());
   printf("Size of document = [%ld]\n", GetVLSize());
}

// size of buffer allowed to be entered
#define BUFF_SIZE 10

T_F MyVLText::UserNew()
{
   // NOTE: there are so many ways to do the following code...
   //   in the future, there probably will be a higher level VariLenText
   //   class that will greatly simplify the interface.
   
   // Setup our blob buffer
   if (!ReadyBuffer(BUFF_SIZE))
      return (False);

   // Get our blob buffer
   Pchar buffer = (Pchar) GetBuffer();
   Pchar ptr = buffer;
   char  tmp[256];
   long  textlen;
   long  totallen = 0;

   printf("Enter text (return to end):\n");

   while (totallen < BUFF_SIZE)
   {
      gets(tmp);
      if (!*tmp)
         break;

      om_strcat(tmp, "\n");
      om_strcpy(ptr, tmp);
      textlen = om_strlen(tmp);
      ptr += textlen;
      totallen += textlen;
   }

   // set the size of the buffer that is used
   SetAmtUsed(totallen);

   // write the buffer to disk
   return Write();
}


