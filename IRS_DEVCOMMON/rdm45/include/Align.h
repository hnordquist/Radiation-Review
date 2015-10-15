/*-----------------------------------------------------------------------

   align.h  -- RDM data field alignment computation module.

   This module computes the alignment of data fields within records
   to match against the schema.

   This header file is included by <lockcomm.h>

   Copyright (c) 1988-1995, Raima Corporation.  All Rights Reserved.

-----------------------------------------------------------------------*/

#include <stddef.h>

/* internal definitions not yet defined */
#define MAXDIMS     3
#define STRUCTFLD   0x0002   /* field is sub-field of struct */


/* ======================================================================
   Calculate aligned field offset and field length
*/
int EXTERNAL_CALLBACK ALIGNMENT_PROC_NAME(
   DB_SHORT DB_FAR *offset,    /* updates according to field type alignment */
                               /* if NULL, updates length to account for structure padding */
   DB_SHORT DB_FAR *length,    /* compute length of data type */
   unsigned char fd_type,      /* dict field type */
   DB_SHORT DB_FAR *fd_dim,    /* field dimensions [x][y][z] */
   DB_SHORT fd_flags)          /* field flags contains STRUCTFLD to compute struct alignment */
{
   /* define alignment and padding boundaries */
   static struct {
      unsigned char type;              /* dict field type */
      unsigned char field_length;
      unsigned char field_align;       /* alignment of field */
      unsigned char struct_align;      /* alignment of struct containing field */
      unsigned char struct_pad;        /* padding of struct containing field */
      unsigned char struct_array_pad;  /* padding of struct array containing field */
   } align[] = {
        { 'c', 0, 0, 0, 0, 0 }   /* CHARACTER */
      , { 's', 0, 0, 0, 0, 0 }   /* SHORTINT  */
      , { 'i', 0, 0, 0, 0, 0 }   /* REGINT    */
      , { 'l', 0, 0, 0, 0, 0 }   /* LONGINT   */
      , { 'd', 0, 0, 0, 0, 0 }   /* DBADDR    */
      , { 'f', 0, 0, 0, 0, 0 }   /* FLOAT     */
      , { 'F', 0, 0, 0, 0, 0 }   /* DOUBLE    */
   };
#define MAX_ALIGN   (sizeof(align) / sizeof(align[0]))

   DB_SHORT i, j, elements;
   static int init = 0;

   if (init == 0)
   {
      /*lint -e612 */
#if _MSC_VER >= 700
#pragma warning(disable:4121)  /* alignment of member was sensitive to packing */
#endif
      /*
         Some compilers may warn that these structures are sensitive to packing.
         !!! DO NOT CHANGE THAT !!!
         These structures determine the packing method used by the compiler.
      */
      static struct cs { char c1; char c2; };
      static struct stcs {
         char st1;
         struct cstruct { char st2; char st3; } sts1;
         struct cstruct_array { char st4; char st5; } sts2[2];
      };

      static struct ss { char s1; short s2; };
      static struct stss {
         char st1;
         struct sstruct { short st2; char st3; } sts1;
         struct sstruct_array { short st4; char st5; } sts2[2];
      };

      static struct is { char i1; int i2; };
      static struct stis {
         char st1;
         struct istruct { int st2; char st3; } sts1;
         struct istruct_array { int st4; char st5; } sts2[2];
      };

      static struct ls { char l1; long l2; };
      static struct stls {
         char st1;
         struct lstruct { long st2; char st3; } sts1;
         struct lstruct_array { long st4; char st5; } sts2[2];
      };

      static struct as { char a1; DB_ADDR a2; };
      static struct stas {
         char st1;
         struct astruct { DB_ADDR st2; char st3; } sts1;
         struct astruct_array { DB_ADDR st4; char st5; } sts2[2];
      };

      static struct fs { char f1; float f2; };
      static struct stfs {
         char st1;
         struct fstruct { float st2; char st3; } sts1;
         struct fstruct_array { float st4; char st5; } sts2[2];
      };

      static struct ds { char d1; double d2; };
      static struct stds {
         char st1;
         struct dstruct { double st2; char st3; } sts1;
         struct dstruct_array { double st4; char st5; } sts2[2];
      };
      /*lint +e612 */
#if _MSC_VER >= 700
#pragma warning(default:4121)
#endif

      align[0].field_length = (unsigned char)sizeof(char);
      align[0].field_align  = (unsigned char)offsetof(struct cs, c2);
      align[0].struct_align = (unsigned char)offsetof(struct stcs, sts1);
      align[0].struct_pad   = (unsigned char)(sizeof(struct cstruct) - sizeof(char));
      align[0].struct_array_pad = (unsigned char)(sizeof(struct cstruct_array) - sizeof(char));

      align[1].field_length = (unsigned char)sizeof(short);
      align[1].field_align  = (unsigned char)offsetof(struct ss, s2);
      align[1].struct_align = (unsigned char)offsetof(struct stss, sts1);
      align[1].struct_pad   = (unsigned char)(sizeof(struct sstruct) - sizeof(short));
      align[1].struct_array_pad = (unsigned char)(sizeof(struct sstruct_array) - sizeof(short));

      align[2].field_length = (unsigned char)sizeof(int);
      align[2].field_align  = (unsigned char)offsetof(struct is, i2);
      align[2].struct_align = (unsigned char)offsetof(struct stis, sts1);
      align[2].struct_pad   = (unsigned char)(sizeof(struct istruct) - sizeof(int));
      align[2].struct_array_pad = (unsigned char)(sizeof(struct istruct_array) - sizeof(int));

      align[3].field_length = (unsigned char)sizeof(long);
      align[3].field_align  = (unsigned char)offsetof(struct ls, l2);
      align[3].struct_align = (unsigned char)offsetof(struct stls, sts1);
      align[3].struct_pad   = (unsigned char)(sizeof(struct lstruct) - sizeof(long));
      align[3].struct_array_pad = (unsigned char)(sizeof(struct lstruct_array) - sizeof(long));

      align[4].field_length = (unsigned char)sizeof(DB_ADDR);
      align[4].field_align  = (unsigned char)offsetof(struct as, a2);
      align[4].struct_align = (unsigned char)offsetof(struct stas, sts1);
      align[4].struct_pad   = (unsigned char)(sizeof(struct astruct) - sizeof(DB_ADDR));
      align[4].struct_array_pad = (unsigned char)(sizeof(struct astruct_array) - sizeof(DB_ADDR));

      align[5].field_length = (unsigned char)sizeof(float);
      align[5].field_align  = (unsigned char)offsetof(struct fs, f2);
      align[5].struct_align = (unsigned char)offsetof(struct stfs, sts1);
      align[5].struct_pad   = (unsigned char)(sizeof(struct fstruct) - sizeof(float));
      align[5].struct_array_pad = (unsigned char)(sizeof(struct fstruct_array) - sizeof(float));

      align[6].field_length = (unsigned char)sizeof(double);
      align[6].field_align  = (unsigned char)offsetof(struct ds, d2);
      align[6].struct_align = (unsigned char)offsetof(struct stds, sts1);
      align[6].struct_pad   = (unsigned char)(sizeof(struct dstruct) - sizeof(double));
      align[6].struct_array_pad = (unsigned char)(sizeof(struct dstruct_array) - sizeof(double));

      init = 1;
   }

   /* calculate number of array elements */
   for (i = 0, elements = 1; fd_dim && i < MAXDIMS; ++i)
   {
      if (fd_dim[i])
         elements *= fd_dim[i];
   }

   /* lookup data type in alignment table */
   for (j = 0; j < MAX_ALIGN && align[j].type != fd_type; ++j)
      ;
   if (j == MAX_ALIGN)
      return S_INVFLD;

   if (!offset)
   {
      /* ending a struct definition may require padding */
      DB_SHORT pad = (elements > 1) ? align[j].struct_array_pad : align[j].struct_pad;
      *length += ((pad - (*length % pad)) % pad);
      *length *= elements;
   }
   else
   {
      /* force offset of field or struct to appropriate boundary */
      i = (fd_flags & STRUCTFLD) ? align[j].struct_align : align[j].field_align;
      *offset += ((i - (*offset % i)) % i);

      if (length)
         *length = align[j].field_length * elements;
   }

   return S_OKAY;
}

#undef MAX_ALIGN
#undef MAXDIMS
#undef STRUCTFLD

/* $Revision:   1.4  $ */

/* vpp ALIGN.H (Fri Sep 19 11:56:08 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

