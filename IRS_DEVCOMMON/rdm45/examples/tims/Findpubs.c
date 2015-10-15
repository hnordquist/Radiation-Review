#include <stdio.h>
#include "vista.h"
#include "tims.h"

static char cmd[10];

static int pr_keywords(P0);
static int pr_abstract(P0);


/* Find publications by key word
*/
by_key()
{
   struct info irec;                /* info record variable */
   char name[32];                   /* author's name */
   char key[32];                    /* key word */

   /* find key word record */
   printf("key word: ");
   gets(key);
   if (d_keyfind(KWORD, key, CURR_DB) == S_NOTFOUND)
      printf("no records found\n");
   else
   {
      /* scan thru key_to_info set */
      d_setor(KEY_TO_INFO, CURR_DB);
      for (d_findfm(KEY_TO_INFO, CURR_DB); db_status == S_OKAY;
            d_findnm(KEY_TO_INFO, CURR_DB))
      {
         /* find current owner (info) of current record (intersect) */
         d_findco(INFO_TO_KEY, CURR_DB);

         /* read contents of info record */
         d_recread(&irec, CURR_DB);

         /* find author of info record */
         d_findco(HAS_PUBLISHED, CURR_DB);
         d_crread(NAME, name, CURR_DB);

         /* print results */
         printf("id_code: %s\n", irec.id_code);
         printf("author : %s\n", name);
         printf("title  : %s\n", irec.info_title);
         printf("publ.  : %s, %s\n", irec.publisher, irec.pub_date);
         pr_keywords();
         pr_abstract();
         printf("--- press <enter> to continue");
         gets(cmd);
      }
   }
   return (0);
}


/* Find publication by author
*/
by_author()
{
   struct info irec;                /* info record variable */
   char search[32];                 /* author to search for */
   char name[32];

   /* find author record */
   printf("author: ");
   gets(search);
   for (d_findfm(AUTHOR_LIST, CURR_DB); db_status == S_OKAY;
         d_findnm(AUTHOR_LIST, CURR_DB))
   {
      d_crread(NAME, name, CURR_DB);
      if (strcmp(search, name) == 0)
         break;
      else if (strcmp(search, name) < 0)
      {
         printf("author record not found\n");
         return (0);
      }
   }
   if (db_status == S_OKAY)
   {
      d_setor(HAS_PUBLISHED, CURR_DB);
      for (d_findfm(HAS_PUBLISHED, CURR_DB); db_status == S_OKAY;
           d_findnm(HAS_PUBLISHED, CURR_DB))
      {
         d_recread(&irec, CURR_DB);

         /* read and print info record */
         printf("id_code: %s\n", irec.id_code);
         printf("author : %s\n", name);
         printf("title  : %s\n", irec.info_title);
         printf("publ.  : %s, %s\n", irec.publisher, irec.pub_date);
         pr_keywords();
         pr_abstract();
         printf("--- press <enter> to continue");
         gets(cmd);
      }
   }
   return (0);
}


/* Print key words
*/
static pr_keywords()
{
   long count;                      /* number of info_to_key members */
   char key[32];                    /* key word or phrase */
   DB_ADDR dba;                     /* db addr of key_to_info member */

   /* the current member of the has_published set is the info record whose
    * key words are to be listed */
   d_setom(INFO_TO_KEY, HAS_PUBLISHED, CURR_DB);

   /* fetch number of members of info_to_key */
   d_members(INFO_TO_KEY, &count, CURR_DB);

   /* list the key words, if any */
   if (count > 0L)
   {
      /* save current member of key_to_info because it's going to change and
       * we may be currently scanning through that set */
      d_csmget(KEY_TO_INFO, &dba, CURR_DB);

      printf("key words:\n----------\n");
      /* find each intersect member record */
      while (d_findnm(INFO_TO_KEY, CURR_DB) == S_OKAY)
      {
         /* find, read and print corresponding key_word */
         d_findco(KEY_TO_INFO, CURR_DB);
         d_crread(KWORD, key, CURR_DB);
         printf("   %s\n", key);
      }
      printf("\n");

      /* reset key_to_info current member and owner */
      if (dba)
         d_csmset(KEY_TO_INFO, &dba, CURR_DB);
   }
   return (0);
}



/* Print abstract
*/
static pr_abstract()
{
   long count;                      /* number of abstract members */
   char txt[80];                    /* line of abstract text */

   /* the current member of has_published is the info record whose abstract
    * is to be printed */
   d_setom(ABSTRACT, HAS_PUBLISHED, CURR_DB);

   /* fetch number of lines in abstract */
   d_members(ABSTRACT, &count, CURR_DB);

   /* print abstract, if one exists */
   if (count > 0)
   {
      printf("abstract:\n---------\n");

      /* find, read and print each abstract text line */
      while (d_findnm(ABSTRACT, CURR_DB) != S_EOS)
      {
         d_csmread(ABSTRACT, LINE, txt, CURR_DB);
         printf("  %s\n", txt);
      }
   }
   printf("\n");
   return (0);
}

/* $Revision:   1.4  $ */

/* vpp findpubs.c (Fri Sep 19 11:28:40 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

