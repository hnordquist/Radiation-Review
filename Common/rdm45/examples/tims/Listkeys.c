#include <stdio.h>
#include "vista.h"
#include "tims.h"


/* List key words
*/
list_keys()
{
   char key[32];

   printf("start key: ");
   gets(key);
   if (key[0] != '\0')
   {
      if (d_keyfind(KWORD, key, CURR_DB) == S_NOTFOUND)
         d_keynext(KWORD, CURR_DB);
   }
   else
      d_keyfrst(KWORD, CURR_DB);

   /* scan thru keys */
   while (db_status == S_OKAY)
   {
      d_keyread(key);
      printf("   %s\n", key);
      d_keynext(KWORD, CURR_DB);
   }
   printf("--- press <enter> to continue");
   gets(key);
   return (0);
}

/* $Revision:   1.4  $ */

/* vpp listkeys.c (Fri Sep 19 11:28:40 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

