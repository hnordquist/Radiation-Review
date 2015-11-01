
/* DbDfault.H  Function Prototypes */

#ifndef DbDfault_H
	#define DbDfault_H

	int AskUpdateQuestion();
	int AskUpdateINI_Question();
	int AccessDefaultDatabase(short sAction, long lField, int *pbBool);
	int AccessDefaultDatabase(short sAction, long lField, short *pShort);
	int AccessDefaultDatabase(short sAction, long lField, int iIndex, int *pbBool);
	int AccessDefaultDatabase(short sAction, long lField, struct db_chan_event_dflt_rec *pStruct);
	int AccessDefaultDatabase(short sAction, long lField, struct db_dir_rec *pStruct);
	int AccessDefaultDatabase(short sAction, long lField, struct db_sta_dflt_rec *pStruct);
	int AccessDefaultDatabase(short sAction, long lField, char *szStr);
	void InitDbDfltGraphRec(struct db_graph_dflt_rec *pStruct);
	int AccessDefaultDatabase(short sAction, long lField, struct db_graph_dflt_rec *pStruct);
	int CreateGraphDefaultNamesPicklist(char **pAdrs[]);




#endif
