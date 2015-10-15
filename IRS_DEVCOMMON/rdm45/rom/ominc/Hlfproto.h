#ifndef _HLFPROTO_H
#define _HLFPROTO_H

#ifdef __cplusplus
extern "C"
{
#endif

short REXTERNAL hlf_BlobDelete(
    DB_ADDR  owner_dba,
    SET_TYPE set,
    short    lminfo,
    RDM_DB   hDb);

short REXTERNAL hlf_BlobRead(
    SET_TYPE set,
    short    lminfo,
    USHORT  *buflen,
    void    *buffer,
    short   *more_flag,
    short    first_flag,
    RDM_DB   hDb);

short REXTERNAL hlf_BlobReadHeader(
    DB_ADDR  owner_dba,
    SET_TYPE set,
    short    lminfo,
    USHORT  *buflen,
    void    *buffer,
    short   *more_flag,
    RDM_DB   hDb);

short REXTERNAL hlf_BlobWrite(
    SET_TYPE set,
    short    lminfo,
    USHORT  *buflen,
    void    *buffer,
    short    more_flag,
    RDM_DB   hDb);

short REXTERNAL hlf_BlobWriteHeader(
    DB_ADDR  owner_dba,
    SET_TYPE set,
    short    lminfo,
    USHORT  *buflen,
    void    *buffer,
    short    more_flag,
    RDM_DB   hDb);

short REXTERNAL hlf_ConnectTo(
    DB_ADDR  owner_dba,
    DB_ADDR  dba,
    SET_TYPE set,
    short    lminfo,
    RDM_DB   hDb);

short REXTERNAL hlf_CsmSet(
    DB_ADDR     dba,
    SET_TYPE    set,
    const char *lmode,
    short       lminfo,
    RDM_DB      hDb);

short REXTERNAL hlf_Delete(
    DB_ADDR  dba,
    short    lminfo,
    RDM_DB   hDb);

short REXTERNAL hlf_DirectNavigation(
    DB_ADDR *dba,
    short    rectype,
    short    navctrl,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_DisConFrom(
    DB_ADDR  dba,
    SET_TYPE set,
    short    lminfo,
    RDM_DB   hDb);

short REXTERNAL hlf_Find(
    DB_ADDR *dba,
    FLD_TYPE keyfld,
    void    *keybuffer,
    short    rectype,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_FromMember(
    DB_ADDR  member_dba,
    DB_ADDR *dba,
    SET_TYPE set,
    short    *rectype,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_GotoMember(
    DB_ADDR *dba,
    SET_TYPE set,
    short   *rectype,
    short    navctrl,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_GotoMemberFromOwner(
    DB_ADDR  owner_dba,
    DB_ADDR *dba,
    SET_TYPE set,
    short   *rectype,
    short    navctrl,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_IClose(
    RDM_DB   hDb);

short REXTERNAL hlf_IOpen(
    const char *dbname,
    const char *openmode,
    RDM_SESS    hSess,
    RDM_DB     *hDb);

short REXTERNAL hlf_IsMember(
    DB_ADDR  dba,
    SET_TYPE set,
    RDM_DB   hDb);

short REXTERNAL hlf_IsMemberOfOwner(
    DB_ADDR  dba,
    SET_TYPE set,
    DB_ADDR  owner_dba,
    RDM_DB   hDb);

short REXTERNAL hlf_IsOwner(
    DB_ADDR  dba,
    SET_TYPE set,
    RDM_DB   hDb);

short REXTERNAL hlf_IsOwnerOfMember(
    DB_ADDR  dba,
    SET_TYPE set,
    DB_ADDR  member_dba,
    RDM_DB   hDb);

short REXTERNAL hlf_KeyNavigation(
    DB_ADDR *dba,
    short    rectype,
    long     keyfld,
    short    navctrl,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_MakeOwner(
    DB_ADDR     dba,
    SET_TYPE    set,
    const char *lmode,
    short       lminfo,
    RDM_DB      hDb);

short REXTERNAL hlf_MemCount(
    DB_ADDR  dba,
    SET_TYPE set,
    short    lminfo,
    long    *count,
    RDM_DB   hDb);

short REXTERNAL hlf_NewMember(
    DB_ADDR *dba,
    short    rectype,
    void    *buffer,
    DB_ADDR  owner_dba,
    SET_TYPE set,
    short    lminfo,
    RDM_DB   hDb);

short REXTERNAL hlf_NewObj(
    DB_ADDR *dba,
    short    rectype,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_Open(
    const char *dbname,
    const char *openmode,
    RDM_SESS    hSess,
    RDM_DB     *hDb);

short REXTERNAL hlf_Read(
    DB_ADDR *dba,
    short   *rectype,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_SetNavigation(
    DB_ADDR *dba,
    SET_TYPE set,
    short   *rectype,
    short    navctrl,
    short    lminfo,
    USHORT   buflen,
    void    *buffer,
    RDM_DB   hDb);

short REXTERNAL hlf_Write(
    DB_ADDR  dba,
    short    rectype,
    short    lminfo,
    void    *buffer,
    RDM_DB   hDb);

#define HLF_BLOBDELETE           0
#define HLF_BLOBREAD             1
#define HLF_BLOBREADHEADER       2
#define HLF_BLOBWRITE            3
#define HLF_BLOBWRITEHEADER      4
#define HLF_CONNECTTO            5
#define HLF_CSMSET               6
#define HLF_DELETE               7
#define HLF_DIRECTNAVIGATION     8
#define HLF_DISCONFROM           9
#define HLF_FIND                10
#define HLF_FROMMEMBER          11
#define HLF_GOTOMEMBER          12
#define HLF_GOTOMEMBERFROMOWNER 13
#define HLF_ICLOSE              14
#define HLF_IOPEN               15
#define HLF_ISMEMBER            16
#define HLF_ISMEMBEROFOWNER     17
#define HLF_ISOWNER             18
#define HLF_ISOWNEROFMEMBER     19
#define HLF_KEYNAVIGATION       20
#define HLF_MAKEOWNER           21
#define HLF_MEMCOUNT            22
#define HLF_NEWMEMBER           23
#define HLF_NEWOBJ              24
#define HLF_OPEN                25
#define HLF_READ                26
#define HLF_SETNAVIGATION       27
#define HLF_WRITE               28

short REXTERNAL hlfInit(RDM_SESS hSess);
short REXTERNAL hlfCleanup(RDM_SESS hSess);

#ifdef __cplusplus
}
#endif

#endif /* _HLFPROTO_H */

