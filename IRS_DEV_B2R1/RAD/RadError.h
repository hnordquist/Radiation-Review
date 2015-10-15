
/* RADERROR.H	Error numbers */

#ifndef RADERROR_H
	#define RADERROR_H

//	#define uiRadReviewMsg						UINT
	#define uiRAD_REVIEW_INFO					UINT

// status
	#define uiDAY_NOT_IN_DB_STATUS				1000		// only status?
	#define uiFAC_NOT_IN_DB_ERR					1001
	#define uiFILE_DOES_NOT_EXIST_STATUS		1002
	#define	uiFILE_READ_ERR_STATUS				1003		// only status
	#define uiINVALID_TIME_INTERVAL_ERR 		1004
	#define uiMARK_NOT_IN_DB_WARN				1005                                      
	#define uiNO_DATA_FOR_STATION_WARN			1006		// old STA_EMPTY	
	#define	uiSKIP_FILE_STATUS					1007
	#define uiSTA_NOT_IN_DB_ERR					1008
	#define uiTIME_NOT_IN_DB_STATUS				1009
 	#define uiFILE_FAILED_SNF_CHECK_SIGNATURE	1010 
	#define uiEVENT_NOT_IN_DB_INFO				1116		// only status?
	#define uiNO_MATCHING_OP_REC_STATUS			1140		// only status
	//New and more enlightening signature checks
	#define uiUNKNOWN_CA						1170
	#define uiINVALID_SIGNATURE					1171
	#define uiLIBRARY_ERROR						1172
	#define uiNO_INPUT_FILE						1173
	#define uiEMPTY_INPUT_FILE					1174
	#define uiIO_ERROR							1175
	#define uiFILE_INVALID						1176//last number
// errors that have messages associated with them	                                        

	#define uiBLANK_STRING_ERR					1100
	#define uiBROWSE_INFO_WARN					1101

	#define uiCANNOT_OPEN_ANY_DATABASE_ERR		1102
	#define uiCHAN_NOT_DEFINED_ERR				1103                                   
	#define uiCOPY_DB_ERR						1104

	#define uiDB_DOES_NOT_EXIST_ERR 			1105 
	#define uiDATABASE_OPENED_INFO 				1106
	#define uiDB_BAD_ERR						1107 
	#define uiDB_EMPTY_STATUS					1108   
	#define uiDB_INIT_ERR     					1109 
	#define uiDEFAULT_CANNOT_BE_DELETED			1167	
	#define uiDEFAULT_CFG_PATH_TOO_LONG_ERR		1110
	#define uiDEFAULT_CFG_FILE_ERR				1111
	#define uiDFLT_DB_READ_ERR					1112
	#define uiDISPLAY_AT_LEAST_ONE_CHANNEL_ERR	1113
	#define uiDRIVE_DOES_NOT_EXIST_ERR			1114

	#define uiEVENT_NOT_FOUND_WARN				1115

	#define uiFACILITY_NOT_DEFINED_ERR			1117
	#define uiFAC_MGR_CHANGE_INFO				1168	
	#define	uiFILE_OPEN_ERR						1118        
	#define uiFILE_READ_ERR						1119
	#define uiFILE_WRITE_ERR					1120
	#define uiFILE_STA_NOT_IN_DB_WARN			1121

	#define uiINSPEC_INI_ERR					1165	
	#define uiINTERNAL_ERR						1122
	#define uiINVALID_DFLT_DB_FIELD_ERR			1123
	#define uiINVALID_FACILITY					1162
	#define uiINVALID_FILE_TYPE					1166	

	#define uiMAKE_DIR_ERR						1124
	#define uiMEMORY_ALLOCATION_ERR				1125

	#define uiNAME_TOO_LONG_ERR					1126
	#define uiNAMES_CFG_READ_ERR				1127
	#define uiNO_ASSOCIATED_GRAPH_TRACE_ERR		1128
	#define	uiNO_CHANNEL_STRING_MATCH_SUPPORTED_ERR	1163	
	#define uiNO_CHANS_SELECTED_WARN			1129 
	#define uiNO_DATA_IN_INTERVAL_WARN			1130
	#define uiNO_DEFAULT_CFG_FILE_ERR			1131    
	#define uiNO_DIR_SELECTED_WARN				1132
	#define uiNO_FACILITIES_ERR					1133
	#define uiNO_FILES_EXIST_WARN				1134
	#define uiNO_IRS_INI						1169	
	#define uiNO_LOCATION_NUM_MATCH_SUPPORTED_ERR	1135
	#define uiNO_MATCHING_CHAN_EVENTS_WARN		1136
	#define uiNO_MATCHING_DATA_WARN				1137
	#define uiNO_MATCHING_RAW_DATA_WARN			1138
	#define uiNO_MATCHING_DIR_EVENTS_WARN		1139
	#define uiNO_MEAS_SELECTED_WARN				1141
	#define uiNO_MEAS_TYPE_IN_INTERVAL_WARN		1142
	#define uiNO_MEAS_TYPES_SELECTED_WARN		1143
	#define uiNO_MEMORY_FOR_APP_ERR				1144
	#define uiNO_NCC_FILE_REQUESTED_ERR			1145
	#define uiNO_STAS_SELECTED_WARN				1146
	#define uiNO_STATIONS_FOR_FACILITY_ERR		1147
	#define uiNOT_ENOUGH_DATA_TO_ANALYZE_WARN	1148

	#define uiONLY_ONE_GRAPH_WARN				1149
	#define uiOPEN_COM_DB_WARN					1150

	#define uiPATH_DOES_NOT_EXIST_ERR			1151
	#define uiPATH_TOO_LONG_ERR					1152
	#define uiPATHS_TOO_LONG_ERR				1153
	#define uiPEAK_WIDTH_TOO_LARGE_WARN			1154

	#define uiRAD_INVALID_DATABASE_PATH_ERR		1155	
	#define uiRAD_START_FATAL_ERR				1156
	#define uiREQUESTED_FACILTY_NOT_CURRENT_FACILTY 1164	
	#define uiRT_IMPORT_ERR						1157

	#define uiSUMMARY_NOT_FOUND_STATUS			1158

	#define uiUNKNOWN_EVENT_TYPE_ERR			1159
	#define uiWILDCARDS_TOO_LONG_ERR			1160
	#define uiWILDCARD_DOES_NOT_EXIST			1161


	void RadReviewMsg(unsigned int uiError);
	void RadReviewMsg(unsigned int uiError, const char *szStr); 
	void RadReviewMsg(unsigned int uiError, const char *szStr, int iNum); 
	void RadReviewMsg(unsigned int uiError, const char *szStr1, const char *szStr2);
	
	void RadReviewMsg(unsigned int uiError, unsigned long ulNum1, unsigned long ulNum2);
	void RadReviewMsg(unsigned int uiError, DATE dDate1, DATE dDate2, const char *pStr);
	void RadReviewMsg(unsigned int uiError, DATE dDate1, DATE dDate2);

	void RadReviewModelessMsg(unsigned int uiError, const char *szStr);
	
#endif
