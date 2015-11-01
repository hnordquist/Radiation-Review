#ifndef MUTIMS_H
#define MUTIMS_H

/* Raima Database Manager 4.5 [Build 16] */

/*----------------------------------------------------------------
   Technical Information Management System (TIMS) Database
----------------------------------------------------------------*/


/* database mutims record/key structure declarations */

struct Author {
   char name[31];
};

struct Info {
   char idcode[16];
   char title[80];
   char publisher[31];
   char pubdate[12];
   short type;
   char loaned;
};

struct Borrower {
   char name[31];
};

struct Keyword {
   char word[31];
};

struct Keyword_Intersect {
   short infotype;
};

struct Borrower_Intersect {
   long borrowed;
   long returned;
};

struct Text1 {
   char Text1_data[1002][1];
};

struct Text2 {
   char Text2_data[152][1];
};

struct Text3 {
   char Text3_data[16][1];
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define AUTHOR 10000
#define INFO 10001
#define BORROWER 10002
#define KEYWORD 10003
#define KEYWORD_INTERSECT 10004
#define BORROWER_INTERSECT 10005
#define TEXT1 10006
#define TEXT2 10007
#define TEXT3 10008

/* Field Name Constants */
#define AUTHOR_NAME 0L
#define INFO_IDCODE 1000L
#define INFO_TITLE 1001L
#define INFO_PUBLISHER 1002L
#define INFO_PUBDATE 1003L
#define INFO_TYPE 1004L
#define INFO_LOANED 1005L
#define BORROWER_NAME 2000L
#define KEYWORD_WORD 3000L
#define KEYWORD_INTERSECT_INFOTYPE 4000L
#define BORROWER_INTERSECT_BORROWED 5000L
#define BORROWER_INTERSECT_RETURNED 5001L
#define TEXT1_TEXT1_DATA 6000L
#define TEXT2_TEXT2_DATA 7000L
#define TEXT3_TEXT3_DATA 8000L

/* Set Name Constants */
#define AUTHORLIST 20000
#define LOANHISTORY 20001
#define HASPUBLISHED 20002
#define ARTICLELIST 20003
#define INFOTOBORROWER 20004
#define BORROWERTOINFO 20005
#define KEYTOINFO 20006
#define INFOTOKEY 20007
#define ABSTRACT 20008

/* Field Sizes */
#define SIZEOF_AUTHOR_NAME 31
#define SIZEOF_INFO_IDCODE 16
#define SIZEOF_INFO_TITLE 80
#define SIZEOF_INFO_PUBLISHER 31
#define SIZEOF_INFO_PUBDATE 12
#define SIZEOF_INFO_TYPE 2
#define SIZEOF_INFO_LOANED 1
#define SIZEOF_BORROWER_NAME 31
#define SIZEOF_KEYWORD_WORD 31
#define SIZEOF_KEYWORD_INTERSECT_INFOTYPE 2
#define SIZEOF_BORROWER_INTERSECT_BORROWED 4
#define SIZEOF_BORROWER_INTERSECT_RETURNED 4
#define SIZEOF_TEXT1_TEXT1_DATA 1002
#define SIZEOF_TEXT2_TEXT2_DATA 152
#define SIZEOF_TEXT3_TEXT3_DATA 16

#endif    /* MUTIMS_H */
