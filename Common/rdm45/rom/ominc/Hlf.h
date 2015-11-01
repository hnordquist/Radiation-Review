#ifndef _HLF_H
#define _HLF_H

#ifdef DEBUG
void REXTERNAL _ROMSE_Display(int line, char *str);
#define DEBUG_OUT(str) _ROMSE_Display(__LINE__, str)
#else
#define DEBUG_OUT(str) /* */
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* actual information for each blob in a database */
typedef struct BLOB_MEMBER_ENTRY_S
{
   REC_TYPE   rectype;
   void      *recdata;
   DB_USHORT  recdatasize;
} BLOB_MEMBER_ENTRY;

/* set entry for each blob in the database */
typedef struct BLOB_SET_ENTRY_S
{
   SET_TYPE                  set;
   SET_ENTRY                 se;
   BLOB_MEMBER_ENTRY        *info;
   struct BLOB_SET_ENTRY_S  *next;
} BLOB_SET_ENTRY;

/* structure for storing Blob information per database */
typedef struct BLOB_DB_ENTRY_S
{
   RDM_DB                    hDb;
   BLOB_SET_ENTRY           *set;
   unsigned short            refcount;
   struct BLOB_DB_ENTRY_S   *next;
} BLOB_DB_ENTRY;

/* low-level functions that are only used in hlf.c */
void               llf_Blob_CreateDbEntry(RDM_DB hDb);
void               llf_Blob_DeleteDbEntry(RDM_DB hDb);
void               llf_Blob_DeleteSetEntry(BLOB_SET_ENTRY *pSet);
BLOB_SET_ENTRY*    llf_Blob_FindSetEntry(SET_TYPE set, RDM_DB hDb);
BLOB_SET_ENTRY*    llf_Blob_GetSetEntry(SET_TYPE set, RDM_DB hDb);
short              llf_Blob_GetWriteMemberIndex(USHORT buflen, BLOB_SET_ENTRY *pSet, SET_TYPE set, RDM_DB hDb);
short              llf_CheckForMoreMembers(SET_TYPE set, RDM_DB hDb);

#ifdef __cplusplus
}
#endif

#endif /* _HLF_H */
