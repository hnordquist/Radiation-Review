#include <process.h>
#include <stdio.h>
#include <errno.h>

#define LMC_INTERNAL
#define LOCKCOMM_DATA
#include "vista.h"

#ifdef TURBO
extern unsigned int _stklen = 8192;
#endif

int main(P0);

int loan_book(P0);
int return_book(P0);
int list_loaners(P0);
int del_info(P0);
int ent_info(P0);
int by_key(P0);
int by_author(P0);
int list_authors(P0);
int list_keys(P0);


/* Technical Information Management System
*/
main()
{
   char cmd[20];                    /* command entry string */

   if (d_open("tims", "o") != S_OKAY)
   {
      printf("errno: %d\n", errno);
      perror("Failed to open database");
      exit(0);
   }

   for (;;)
   {
      /* display command menu */
      printf("\nTIMS Commands:\n");
      printf("   1 - Display list of key words\n");
      printf("   2 - Display list of authors\n");
      printf("   3 - List publications by key word\n");
      printf("   4 - List publications by author\n");
      printf("   5 - Enter technical information\n");
      printf("   6 - Delete technical information\n");
      printf("   7 - Loan book\n");
      printf("   8 - Return loaned book\n");
      printf("   9 - List borrowed books\n");
      printf("   q - Quit\n");
      printf("enter command: ");
      gets(cmd);
      switch (cmd[0])
      {
         case 'q':
         case 'Q':   d_close();        exit(0);
         case '1':   list_keys();      break;
         case '2':   list_authors();   break;
         case '3':   by_key();         break;
         case '4':   by_author();      break;
         case '5':   ent_info();       break;
         case '6':   del_info();       break;
         case '7':   loan_book();      break;
         case '8':   return_book();    break;
         case '9':   list_loaners();   break;
         default:
            printf("*** incorrect command -- re-enter\n");
            break;
      }
   }
   return (0);
}

/* $Revision:   1.7  $ */

/* vpp tims.c (Fri Sep 19 11:28:39 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

