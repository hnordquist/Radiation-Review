#include <stdio.h>
#include "vista.h"
#include "tims.h"

static struct info irec;
static struct author arec;

static int enter_key_words(P0);
static int enter_abstract(P0);
static int get_info(P0);


/* Enter technical information records into TIMS database
*/
ent_info()
{
   char s[32];                   /* generic string variable */

   /* enter tech info into TIMS database */
   while (get_info() != EOF)
   {
      /* see if author exists */
      for (d_findfm(AUTHOR_LIST, CURR_DB); db_status == S_OKAY;
            d_findnm(AUTHOR_LIST, CURR_DB))
      {
         d_crread(NAME, s, CURR_DB);
         if (strcmp(arec.name, s) == 0)
            break;                        /* author record on file */
      }
      if (db_status == S_EOS)
      {
         /* author not on file -- create record and connect to author list */
         d_fillnew(AUTHOR, &arec, CURR_DB);
         d_connect(AUTHOR_LIST, CURR_DB);
      }
      /* make author current owner of has_published set */
      d_setor(HAS_PUBLISHED, CURR_DB);

      /* create new tech. info record */
      if (d_fillnew(INFO, &irec, CURR_DB) == S_DUPLICATE)
         printf("duplicate id_code: %s\n", irec.id_code);
      else
      {
         /* connect to author record */
         d_connect(HAS_PUBLISHED, CURR_DB);

         /* set current owner for key words and abstract */
         d_setor(INFO_TO_KEY, CURR_DB);
         d_setor(ABSTRACT, CURR_DB);

         enter_key_words();

         enter_abstract();
      }
   }
   return (0);
}



/* Enter any key words
*/
static enter_key_words()
{
   char s[32];

   for (;;)
   {
      printf("key word: ");
      if (gets(s) == NULL || s[0] == '\0')
         break;
      /* see if key word record exists */
      if (d_keyfind(KWORD, s, CURR_DB) == S_NOTFOUND)
      {
         /* create new key word record */
         d_fillnew(KEY_WORD, s, CURR_DB);
      }
      d_setor(KEY_TO_INFO, CURR_DB);

      /* create intersection record */
      d_fillnew(INTERSECT, &irec.info_type, CURR_DB);
      d_connect(KEY_TO_INFO, CURR_DB);
      d_connect(INFO_TO_KEY, CURR_DB);
   }
   return (0);
}


/* Enter abstract description
*/
static enter_abstract()
{
   char text_line[80];

   for (;;)
   {
      printf("abstract: ");
      if (gets(text_line) == NULL || text_line[0] == '\0')
         return (0);
      d_fillnew(INFOTEXT, text_line, CURR_DB);
      d_connect(ABSTRACT, CURR_DB);
   }
   return (0);
}



/* Fill irec, arec with info data from user
*/
static int get_info()
{
   char txt[40];

   printf("author   : ");
   if (gets(arec.name) == NULL || arec.name[0] == '\0')
      return (EOF);
   else
   {
      for (;;)
      {
         printf("id_code  : ");
         gets(irec.id_code);
         printf("title    : ");
         gets(irec.info_title);
         printf("publisher: ");
         gets(irec.publisher);
         printf("pub. date: ");
         gets(irec.pub_date);
         for (;;)
         {
            printf("info type: ");
            gets(txt);
            sscanf(txt, "%d", &irec.info_type);
            if (irec.info_type >= 0 && irec.info_type <= 2)
               break;
            printf("invalid info type - correct types are:\n");
            printf("0 - book, 1 = magazine, 2 = article\n");
         }
         printf("enter data (y/n)? ");
         gets(txt);
         if (txt[0] == 'y' || txt[0] == 'Y')
            return (0);
      }
   }
}

/* $Revision:   1.5  $ */

/* vpp entinfo.c (Fri Sep 19 11:28:39 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

