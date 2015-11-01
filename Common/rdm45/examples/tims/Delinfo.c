#include <stdio.h>
#include "vista.h"
#include "tims.h"


/* Delete technical information records from TIMS database
*/
del_info()
{
   struct info irec;
   long count;
   char id[16], name[32];

   /* find info to be deleted */
   printf("id_code: ");
   gets(id);
   if (d_keyfind(ID_CODE, id, CURR_DB) == S_NOTFOUND)
   {
      printf("id_code %s not on file\n", id);
      return (0);
   }
   d_recread(&irec, CURR_DB);

   /* get author name */
   d_findco(HAS_PUBLISHED, CURR_DB);
   d_crread(NAME, name, CURR_DB);

   /* confirm delete request */
   printf("author: %s\n", name);
   printf("title : %s\n", irec.info_title);
   printf("delete (y/n)? ");
   gets(id);
   if (id[0] != 'Y' && id[0] != 'y')
      return (0);

   /* disconnect any listed articles */
   d_setom(ARTICLE_LIST, HAS_PUBLISHED, CURR_DB);
   for (d_findfm(ARTICLE_LIST, CURR_DB); db_status == S_OKAY;
        d_discon(ARTICLE_LIST, CURR_DB))
      ;

   /* disconnect and delete borrowers */
   d_setom(LOANED_BOOKS, HAS_PUBLISHED, CURR_DB);
   while (d_findfm(LOANED_BOOKS, CURR_DB) == S_OKAY)
   {
      d_discon(LOANED_BOOKS, CURR_DB);
      d_setmr(LOAN_HISTORY, CURR_DB);
      d_discon(LOAN_HISTORY, CURR_DB);
      d_delete(CURR_DB);
   }

   /* disconnect and delete abstract */
   d_setom(ABSTRACT, HAS_PUBLISHED, CURR_DB);
   while (d_findfm(ABSTRACT, CURR_DB) == S_OKAY)
   {
      d_discon(ABSTRACT, CURR_DB);
      d_delete(CURR_DB);
   }

   /* disconnect and delete intersect and (possibly) key word */
   d_setom(INFO_TO_KEY, HAS_PUBLISHED, CURR_DB);
   while (d_findfm(INFO_TO_KEY, CURR_DB) == S_OKAY)
   {
      d_discon(INFO_TO_KEY, CURR_DB);
      d_setmr(KEY_TO_INFO, CURR_DB);
      d_discon(KEY_TO_INFO, CURR_DB);
      d_delete(CURR_DB);
      d_members(KEY_TO_INFO, &count, CURR_DB);
      if (count == 0L)
      {
         /* delete key word */
         d_setro(KEY_TO_INFO, CURR_DB);
         d_delete(CURR_DB);
      }
   }

   /* disconnect info record from author and delete */
   d_discon(HAS_PUBLISHED, CURR_DB);
   d_delete(CURR_DB);

   /* delete author too, if he has no other pubs */
   d_members(HAS_PUBLISHED, &count, CURR_DB);
   if (count == 0L)
   {
      d_setmo(AUTHOR_LIST, HAS_PUBLISHED, CURR_DB);
      d_discon(AUTHOR_LIST, CURR_DB);
      d_delete(CURR_DB);
   }
   return (0);
}

/* $Revision:   1.4  $ */

/* vpp delinfo.c (Fri Sep 19 11:28:39 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

