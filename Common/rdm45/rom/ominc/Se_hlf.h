#ifndef _SE_HLF_H_
#define _SE_HLF_H_

short REXTERNAL Ehlf_BlobDelete(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_BlobRead(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_BlobReadHeader(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_BlobWrite(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_BlobWriteHeader(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_ConnectTo(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_CsmSet(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_Delete(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_DirectNavigation(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_DisConFrom(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_Find(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_FromMember(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_GotoMember(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_GotoMemberFromOwner(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_IClose(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_IOpen(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_IsMember(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_IsMemberOfOwner(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_IsOwner(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_IsOwnerOfMember(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_KeyNavigation(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_MakeOwner(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_MemCount(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_NewMember(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_NewObj(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_Open(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_Read(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_SetNavigation(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);
short REXTERNAL Ehlf_Write(PDPLDATADESC pInParms, PDPLDATADESC pOutParms);

static short llfPutDictParm(DPLDATADESC *, short, long, void *data, RDM_DB);
short REXTERNAL d_getDD(short, long, short *, short *, DPLDATAITEM **, RDM_DB);

#endif
