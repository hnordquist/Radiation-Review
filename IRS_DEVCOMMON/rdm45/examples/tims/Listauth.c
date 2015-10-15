#include <stdio.h>
#include "vista.h"
#include "tims.h"


/* List authors
*/
list_authors()
{
   char start[32], name[32];

   printf("start name: ");
   gets(start);
   if (start[0] != '\0')
   {
      /* scan for first name */
      for (d_findfm(AUTHOR_LIST, CURR_DB); db_status == S_OKAY;
            d_findnm(AUTHOR_LIST, CURR_DB))
      {
         d_crread(NAME, name, CURR_DB);
         if (strcmp(start, name) <= 0)
            break;
      }
   }
   else
      d_findfm(AUTHOR_LIST, CURR_DB);

   while (db_status == S_OKAY)
   {
      d_crread(NAME, name, CURR_DB);
      printf("   %s\n", name);
      d_findnm(AUTHOR_LIST, CURR_DB);
   }
   printf("--- press <enter> to continue");
   gets(name);
   return (0);
}

/* $Revision:   1.3  $ */

/* vpp listauth.c (Fri Sep 19 11:28:40 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

