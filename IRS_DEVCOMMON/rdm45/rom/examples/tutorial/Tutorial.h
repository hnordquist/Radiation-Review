#ifndef TUTORIAL_H
#define TUTORIAL_H

/* Raima Database Manager 4.5 [Build 16] */

/* database name */

/* database tutorial record/key structure declarations */

struct Author {
   char lastname[15];
   char firstname[15];
};

struct Info {
   char idcode[16];
   char title[80];
   char publisher[31];
   char pubdate[12];
   short type;
   char loaned;
};

struct Author_fullname {
   char lastname[15];
   char firstname[15];
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define AUTHOR 10000
#define INFO 10001

/* Field Name Constants */
#define AUTHOR_LASTNAME 0L
#define AUTHOR_FIRSTNAME 1L
#define AUTHOR_FULLNAME 2L
#define INFO_IDCODE 1000L
#define INFO_TITLE 1001L
#define INFO_PUBLISHER 1002L
#define INFO_PUBDATE 1003L
#define INFO_TYPE 1004L
#define INFO_LOANED 1005L

/* Set Name Constants */
#define HASPUBLISHED 20000
#define AUTHORLIST 20001

/* Field Sizes */
#define SIZEOF_AUTHOR_LASTNAME 15
#define SIZEOF_AUTHOR_FIRSTNAME 15
#define SIZEOF_INFO_IDCODE 16
#define SIZEOF_INFO_TITLE 80
#define SIZEOF_INFO_PUBLISHER 31
#define SIZEOF_INFO_PUBDATE 12
#define SIZEOF_INFO_TYPE 2
#define SIZEOF_INFO_LOANED 1

#endif    /* TUTORIAL_H */
