/*
   querysap.h

   Implement General Services Query functions.
*/

typedef struct
{
   DB_WORD serverType;           /* hi - lo   */
   DB_BYTE serverName[48];
   IPXAddress serverAddress;
   DB_WORD interveningNetworks;  /* hi - lo   */
} ServerInfo;

int EXTERNAL_FIXED QueryGeneralServices(W2(DB_DWORD) DB_WORD, DB_WORD, ServerInfo DB_FAR *, DB_WORD);
int EXTERNAL_FIXED QueryNamedService(W2(DB_DWORD) DB_WORD, DB_WORD, DB_BYTE DB_FAR *, IPXAddress DB_FAR *);

/* $Revision:   1.7  $ */

/* vpp QUERYSAP.H (Fri Sep 19 11:56:11 1997) 
 
   -nRDM_SU -dMSDOS -nUNIX -nAIX -nRISC -nSUN -nSCO -dDB_SHARE -dWIN
   -nWIN16 -dWIN32 -nVANILLA_BSD -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL
   -dFAR_ALLOC -nARCHIVING -nBSD -nMEMLOCK -nIS_UNIX_REALLY -nREOPEN_FILES
   -nONE_DB -nNO_DBN -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER
   -nNO_TRANS -nVer330 -d_WINDOWS -nQNX -nQNX2 -nQNX4 -nNEEDED -nUSED
   -nMACROS_ALLOWED -nOLD_WAY -dTRUENAME -nLAT -nSEGMENTS -nPHARLAP -dANSI
   -nCOMMENTS -dLM_BACKGROUND -nTSR -nICC -nTCPIP -nMEMCHECK
*/

