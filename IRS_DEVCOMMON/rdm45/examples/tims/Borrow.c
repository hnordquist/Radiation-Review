#include <stdio.h>
#include "vista.h"
#include "tims.h"


/* Loan book
*/
loan_book()
{
   char id[16];
   char date[20];
   struct borrower brec;

   printf("id_code of book to be loaned: ");
   if (gets(id) && id[0] != '\0')
   {
      if (d_keyfind(ID_CODE, id, CURR_DB) == S_NOTFOUND)
         printf("id_code %s not on file\n", id);
      else
      {
         d_setor(LOANED_BOOKS, CURR_DB);
         printf("name of borrower: ");
         gets(brec.friend);
         printf("date borrowed   : ");
         gets(date);
         sscanf(date, "%ld", &brec.date_borrowed);
         brec.date_returned = 0L;
         d_fillnew(BORROWER, &brec, CURR_DB);
         d_connect(LOANED_BOOKS, CURR_DB);
         d_connect(LOAN_HISTORY, CURR_DB);
      }
   }
   return (0);
}


/* Returned borrowed book
*/
return_book()
{
   char id[16];
   char friend[32];
   char date[20];
   long bdate;
   struct borrower brec;

   printf("id_code of returned book: ");
   if (gets(id) && id[0] != '\0')
   {
      if (d_keyfind(ID_CODE, id, CURR_DB) == S_NOTFOUND)
         printf("id_code %s not on file\n", id);
      else
      {
         d_setor(LOANED_BOOKS, CURR_DB);
         printf("name of borrower: ");
         gets(friend);
         while (d_findnm(LOANED_BOOKS, CURR_DB) == S_OKAY)
         {
            d_recread(&brec, CURR_DB);
            if (strcmp(brec.friend, friend) == 0)
            {
               if (brec.date_returned == 0L)
               {
                  printf("book borrowed on: %ld\n", brec.date_borrowed);
                  printf("date returned   : ");
                  gets(date);
                  sscanf(date, "%ld", &bdate);
                  d_crwrite(DATE_RETURNED, &bdate, CURR_DB);
                  return (0);
               }
            }
         }
         printf("borrower not on file\n");
      }
   }
   return (0);
}


/* List loaned out books
*/
list_loaners()
{
   struct borrower brec;
   char title[80];

   for (d_findfm(LOAN_HISTORY, CURR_DB); db_status == S_OKAY;
         d_findnm(LOAN_HISTORY, CURR_DB))
   {
      d_recread(&brec, CURR_DB);
      if (brec.date_returned == 0L)
      {
         d_findco(LOANED_BOOKS, CURR_DB);
         d_crread(INFO_TITLE, title, CURR_DB);
         printf("title: %s\n", title);
         printf("borrowed by %s on %ld\n\n", brec.friend, brec.date_borrowed);
      }
   }
   return (0);
}

/* $Revision:   1.3  $ */

/* vpp borrow.c (Fri Sep 19 11:28:39 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

