/*----------------------------------------------------------------
   Technical Information Management System (TIMS) Database
----------------------------------------------------------------*/

database mutims {
   data file "mutims.d00" contains system, Author;
   data file "mutims.d01" contains Info;
   data file "mutims.d02" contains Borrower;
   data file "mutims.d03" contains Keyword;
   data file "mutims.d04" contains Keyword_Intersect, Borrower_Intersect;
   data file "mutims.d05" contains Text1;
   data file "mutims.d06" contains Text2;
   data file "mutims.d07" contains Text3;
   key  file "mutims.k00" contains idcode;
   key  file "mutims.k01" contains Borrower.name;
   key  file "mutims.k02" contains word;
  


   record Author {
      char  name[31];                 /* author's name: "last, first"    */
   }                                  /* or editor's name                */

   record Info {
      unique key char  idcode[16];    /* dewey dec. or own coding tech.  */
      char             title[80];     /* title of book, article, mag.    */
      char             publisher[31]; /* name of publisher - prob. coded */
      char             pubdate[12];   /* date of publication
                                         (e.g. most recent copyright)    */
      short            type;          /* 0 = book, 1 = magazine,
                                         2 = article                     */
	  char             loaned;        /* flag indicating whether this    
	                                     publication is currently loaned */
   }
 
   record Borrower {
      key char  name[31];             /* name of borrower                */
   }
 
   record Keyword {
      unique key char  word[31];      /* subject key words or
                                         classification                  */
   }
 
   record Keyword_Intersect {
      short  infotype;                /* copy of info type to save I/O   */
   }                                  /* when looking only for, say,
                                         books                           */
 
   record Borrower_Intersect {
      long  borrowed;                 /* dates are stored initially as   */
      long  returned;                 /* numeric YYMMDD (e.g. 870226)    */
   }

   record Text1 {
      char  Text1_data[1002][1];
   }
 
   record Text2 {
      char  Text2_data[152][1];
   }
 
   record Text3 {
      char  Text3_data[16][1];
   }
 
 
   set AuthorList {
      order   ascending;
      owner   system;
      member  Author by name;
   }
 
   set LoanHistory {
      order   last;
      owner   system;
      member  Borrower;
   }
 
   set HasPublished {
      order   ascending;
      owner   Author;
      member  Info by title;
   }
 
   set ArticleList {
      order   last;
      owner   Info;
      member  Info;
   }
 
   set InfoToBorrower {
      order   last;
      owner   Info;
      member  Borrower_Intersect;
   }
 
   set BorrowerToInfo {
      order   last;
      owner   Borrower;
      member  Borrower_Intersect;
   }
 
   set KeyToInfo {
      order   last;
      owner   Keyword;
      member  Keyword_Intersect;
   }
 
   set InfoToKey {
      order   last;
      owner   Info;
      member  Keyword_Intersect;
   }
 
   varilen set Abstract {
      order   last;
      owner   Info;
      member  Text1;
      member  Text2;
      member  Text3;
   }
}
  
