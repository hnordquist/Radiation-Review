#ifndef POLY_DB_H
#define POLY_DB_H

/* Raima Database Manager 4.5 [Build 16] */


/* database poly_db record/key structure declarations */

struct _Display {
   char DisplayName[21];
};

struct _Rectangle {
   short UpLfX;
   short UpLfY;
   short Height;
   short Width;
};

struct _Circle {
   short CentX;
   short CentY;
   short Diameter;
};

struct _Triangle {
   short Xa;
   short Ya;
   short Xb;
   short Yb;
   short Xc;
   short Yc;
};

/* record, field and set table entry definitions */

/* File Id Constants */

/* Record Name Constants */
#define _DISPLAY 10000
#define _RECTANGLE 10001
#define _CIRCLE 10002
#define _TRIANGLE 10003

/* Field Name Constants */
#define _DISPLAY_DISPLAYNAME 0L
#define _RECTANGLE_UPLFX 1000L
#define _RECTANGLE_UPLFY 1001L
#define _RECTANGLE_HEIGHT 1002L
#define _RECTANGLE_WIDTH 1003L
#define _CIRCLE_CENTX 2000L
#define _CIRCLE_CENTY 2001L
#define _CIRCLE_DIAMETER 2002L
#define _TRIANGLE_XA 3000L
#define _TRIANGLE_YA 3001L
#define _TRIANGLE_XB 3002L
#define _TRIANGLE_YB 3003L
#define _TRIANGLE_XC 3004L
#define _TRIANGLE_YC 3005L

/* Set Name Constants */
#define ALLSHAPES 20000

/* Field Sizes */
#define SIZEOF__DISPLAY_DISPLAYNAME 21
#define SIZEOF__RECTANGLE_UPLFX 2
#define SIZEOF__RECTANGLE_UPLFY 2
#define SIZEOF__RECTANGLE_HEIGHT 2
#define SIZEOF__RECTANGLE_WIDTH 2
#define SIZEOF__CIRCLE_CENTX 2
#define SIZEOF__CIRCLE_CENTY 2
#define SIZEOF__CIRCLE_DIAMETER 2
#define SIZEOF__TRIANGLE_XA 2
#define SIZEOF__TRIANGLE_YA 2
#define SIZEOF__TRIANGLE_XB 2
#define SIZEOF__TRIANGLE_YB 2
#define SIZEOF__TRIANGLE_XC 2
#define SIZEOF__TRIANGLE_YC 2

#endif    /* POLY_DB_H */
