/*----------------------------------------------------------------------
   dberr.h -- text of error messages used by dberr function

   Copyright (c) 1984-1993, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

#ifndef DBERR_H
#define DBERR_H

/* ANY CHANGES MUST ALSO BE MADE TO THE VISTAW.RC FILES */

/* dberr error/status messages */
static char *user_error[] =
{
/*  -1 S_DBOPEN         */ "database not opened",
/*  -2 S_INVSET         */ "invalid set",
/*  -3 S_INVREC         */ "invalid record",
/*  -4 S_INVDB          */ "can not open dictionary",
/*  -5 S_INVFLD         */ "invalid field name",
/*  -6 S_INVADDR        */ "invalid database address",
/*  -7 S_NOCR           */ "no current record",
/*  -8 S_NOCO           */ "set has no current owner",
/*  -9 S_NOCM           */ "set has no current member",
/* -10 S_KEYREQD        */ "key value required",
/* -11 S_BADTYPE        */ "invalid lock value",
/* -12 S_HASMEM         */ "record is owner of non-empty set(s)",
/* -13 S_ISMEM          */ "record is member of set(s)",
/* -14 S_ISOWNED        */ "member already owned",
/* -15 S_ISCOMKEY       */ "field is a compound key",
/* -16 S_NOTCON         */ "record not connected to set",
/* -17 S_NOTKEY         */ "field is not a valid key",
/* -18 S_INVOWN         */ "record not legal owner of set",
/* -19 S_INVMEM         */ "record not legal member of set",
/* -20 S_SETPAGES       */ "must call d_setpages() before opening the database",
/* -21 S_INCOMPAT       */ "incompatible dictionary file",
/* -22 S_DELSYS         */ "illegal attempt to delete/add system record",
/* -23 S_NOTFREE        */ "illegal attempt to lock locked set/record",
/* -24 S_NOTLOCKED      */ "attempt to access unlocked set/record",
/* -25 S_TRANSID        */ "transaction ID not supplied",
/* -26 S_TRACTIVE       */ "transaction already active",
/* -27 S_TRNOTACT       */ "transaction not active",
/* -28 S_BADPATH        */ "directory string is invalid",
/* -29 S_TRFREE         */ "cannot free locks within a transaction",
/* -30 S_RECOVERY       */ "automatic recovery about to occur",
/* -31 S_NOTRANS        */ "cannot update database outside a transaction",
/* -32 S_EXCLUSIVE      */ "exclusive access required",
/* -33 S_STATIC         */ "locks not allowed on static files",
/* -34 S_USERID         */ "unspecified user id",
/* -35 S_NAMELEN        */ "database path or file name too long",
/* -36 S_RENAME         */ "invalid file number was passed to d_renfile",
/* -37 S_NOTOPTKEY      */ "field is not an optional key field",
/* -38 S_BADFIELD       */ "field is not defined in current record type",
/* -39 S_COMKEY         */ "record/field has/in a compound key",
/* -40 S_INVNUM         */ "invalid record or set number",
/* -41 S_TIMESTAMP      */ "record/set not timestamped",
/* -42 S_BADUSERID      */ "bad DBUSERID (too long or contains non-alphanumeric)",
/* -43 S_NOPROTOCOL     */ "", /* "unable to initialize communication protocol layer", - filled in by dberr() */
/* -44                  */ "",
/* -45                  */ "",
/* -46 S_NOTYPE         */ "no current record type",
/* -47 S_INVSORT        */ "invalid country table sort string",
/* -48 S_DBCLOSE        */ "database not closed",
/* -49 S_INVPTR         */ "invalid pointer",
/* -50 S_INVID          */ "invalid internal ID",
/* -51 S_INVLOCK        */ "invalid lockmgr communication type",
/* -52 S_INVTASK        */ "invalid task",
/* -53 S_NOLOCKCOMM     */ "Lock Manager Communication not initialized",
/* -54 S_NOTIMPLEMENTED */ "option is not implemented in this version"
};

/* dberr system error messages */
static char *system_error[] =
{
/* -900 S_NOSPACE        */ "out of disk space",
/* -901 S_SYSERR         */ "system error",
/* -902 S_FAULT          */ "page fault",
/* -903 S_NOWORK         */ "cannot access dbQuery dictionary",
/* -904 S_NOMEMORY       */ "out of memory",
/* -905 S_NOFILE         */ "error opening data or key file",
/* -906 S_DBLACCESS      */ "unable to open TAF or DBL or LOG file",
/* -907 S_DBLERR         */ "error reading/writing TAF or DBL file",
/* -908 S_BADLOCKS       */ "inconsistent database locks",
/* -909 S_RECLIMIT       */ "file record limit exceeded",
/* -910 S_KEYERR         */ "key file inconsistency",
/* -911 S_TTSUNAVAILABLE */ "Novell's Transaction Tracking is not available",
/* -912 S_FSEEK          */ "file seek error",
/* -913 S_LOGIO          */ "error reading/writing LOG file",
/* -914 S_READ           */ "error reading from a data or key file",
/* -915 S_NETSYNC        */ "lock manager synchronization error",
/* -916 S_DEBUG          */ "debug check interrupt",
/* -917 S_NETERR         */ "network communications error",
/* -918                  */ "",
/* -919 S_WRITE          */ "error writing to a data or key file",
/* -920 S_NOLOCKMGR      */ "no lock manager is installed",
/* -921 S_DUPUSERID      */ "DBUSERID is already being used",
/* -922 S_LMBUSY         */ "the lock manager table(s) are full",
/* -923 S_DISCARDED      */ "attempt to lock discarded memory",
/* -924 S_SEM            */ "RDM Semaphore / Mutex error",
/* -925 S_LMCERROR       */ "", /* network layer error */
/* -926 S_TTSERROR       */ "", /* Novell TTS error */
/* -927                  */ "",
/* -928                  */ "",
/* -929                  */ "",
/* -930                  */ "",
/* -931                  */ "",
/* -932                  */ "",
/* -933                  */ "",
/* -934                  */ "",
/* -935                  */ "",
/* -936                  */ "",
/* -937 S_CLOSE     	    */ "error closing file",
/* -938 S_NODBQUERY      */ "db_QUERY not linked with this Raima DLL",
/* -939 S_READONLY       */ "unable to update file due to READONLY option",
/* -940 S_EACCESS        */ "file in use",
/* -941 S_NOSHARE        */ "SHARE not loaded",
/* -942 S_PROTECTED      */ "must run in protected mode",
/* -943 S_RECFAIL        */ "recovery failed",
/* -944 S_TAFSYNC        */ "TAF-lockmgr synchronization error",
/* -945 S_BLOCKIOPG      */ "Block-I/O page sizes must be multiples of 512",
/* -946 S_ALIGNMENT      */ "dictionary alignment does not match program",
/* -947 S_REENTER        */ "RDM entered re-entrantly"
};

/* dberr internal error messages */
static char *internal_error[] =
{
/* -9001 SYS_BADLEVEL    */ "bad nesting level",
/* -9002 SYS_INVOWNER    */ "illegal owner record",
/* -9003 SYS_BADTREE     */ "b-tree malformed",
/* -9004 SYS_KEYEXIST    */ "key value already exists",
/* -9005 SYS_INVHANDLE   */ "free of invalid handle",
/* -9006 SYS_LOCKARRAY   */ "lock packet array exceeded",
/* -9007 SYS_INTERNALMEM */ "accessing shared table",
/* -9008 SYS_BADFREE     */ "attempt to free empty table",
/* -9009 SYS_BADOPTKEY   */ "calculating optkey index",
/* -9010 SYS_TIMER       */ "unable to set system time",
/* -9011 SYS_IXNOTCLEAR  */ "ix-cache not reset after trans",
/* -9012 SYS_INVLOGPAGE  */ "invalid page in log file",
/* -9013 SYS_INVFLDTYPE  */ "illegal field type",
/* -9014 SYS_INVSORT     */ "illegal sort ordering",
/* -9015 SYS_BADREOPEN   */ "bad reopen status",
/* -9016 SYS_INVPGTAG    */ "invalid page tag",
/* -9017 SYS_INVHOLD     */ "bad hold count",
/* -9018 SYS_HASHCYCLE   */ "cycle detected in hash chain",
/* -9019 SYS_INVLRU      */ "invalid lru page",
/* -9020 SYS_INVPAGE     */ "invalid cache page",
/* -9021 SYS_INVPGCOUNT  */ "bad page tag page count",
/* -9022 SYS_INVPGSIZE   */ "invalid cache page size",
/* -9023 SYS_PZACCESS    */ "invalid access to page zero",
/* -9024 SYS_BADPAGE     */ "wrong page",
/* -9025 SYS_INVEXTEND   */ "illegal attempt to extend file",
/* -9026 SYS_BADREAD     */ "bad read",
/* -9027 SYS_PZNEXT      */ "bad pznext",
/* -9028 SYS_DCHAIN      */ "bad dchain",
/* -9029 SYS_EOF         */ "attempt to write past EOF",
/* -9030 SYS_FILEMODIFIED*/ "locked file was modified by another user"
};

#endif

/* $Revision:   1.33  $ */

/* vpp DBERR.H (Fri Sep 19 11:56:08 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

