#ifndef VLTXT_DB_H
#define VLTXT_DB_H

/* Raima Database Manager 4.5 [Build 16] */


/* database vltxt_db record/key structure declarations */

struct Document {
   char Name[21];
};

struct VLText_4096 {
   char bits_4096[4076][1];
};

struct VLText_2048 {
   char bits_2048[2028][1];
};

struct VLText_512 {
   char bits_512[492][1];
};

struct VLText_128 {
   char bits_128[108][1];
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define DOCUMENT 10000
#define VLTEXT_4096 10001
#define VLTEXT_2048 10002
#define VLTEXT_512 10003
#define VLTEXT_128 10004

/* Field Name Constants */
#define DOCUMENT_NAME 0L
#define VLTEXT_4096_BITS_4096 1000L
#define VLTEXT_2048_BITS_2048 2000L
#define VLTEXT_512_BITS_512 3000L
#define VLTEXT_128_BITS_128 4000L

/* Set Name Constants */
#define VLTEXT_SET 20000

/* Field Sizes */
#define SIZEOF_DOCUMENT_NAME 21
#define SIZEOF_VLTEXT_4096_BITS_4096 4076
#define SIZEOF_VLTEXT_2048_BITS_2048 2028
#define SIZEOF_VLTEXT_512_BITS_512 492
#define SIZEOF_VLTEXT_128_BITS_128 108

#endif    /* VLTXT_DB_H */
