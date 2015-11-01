/*-----------------------------------------------------------------------
   xio.h - access database files without runtime

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
-----------------------------------------------------------------------*/
#ifndef _XIO_H_
#define _XIO_H_

typedef struct
{
   F_ADDR pz_dchain;                /* delete chain pointer (init -1) */
   F_ADDR pz_next;                  /* next page or record slot */
   long   timestamp;                /* file's timestamp (initially 1) */
   time_t cdate;                    /* creation date,time */
   time_t bdate;                    /* date/time of last backup */
} PAGE_ZERO;

#ifdef XIO_C
#define EXTERN /**/
#else
#define EXTERN extern
#endif

EXTERN PAGE_ZERO *pz_table;

int xio_init(void);
int xio_cleanup(void);
int xio_open(FILE_NO fno);
int xio_close(FILE_NO fno);
int xio_pzread(FILE_NO fno);
F_ADDR xio_filesize(FILE_NO fno);
int xio_read(FILE_NO fno, F_ADDR rno, char **rec);
int xio_in(FILE_NO fno, F_ADDR pgno, char **pg);

#undef EXTERN

#endif   /* _XIO_H_ */

/* $Revision:   1.3  $ */

/* vpp XIO.H (Fri Sep 19 11:56:12 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

