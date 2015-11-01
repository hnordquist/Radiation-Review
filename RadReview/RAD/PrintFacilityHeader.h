
/* PrintFacilityHeader.H  Function Prototypes */

#ifndef PrintFacilityHeader_H
	#define PrintFacilityHeader_H
	
	BOOL PrintFacilityHeader(CGUI_TextWin *pWin, short sFacNum, DATE dStart, DATE dEnd);
	//JCD added to make sure report headers to test win and file would be the same
	BOOL PrintFacilityHeader(CGFmtStr *pStr, short sFacNum, DATE dStart, DATE dEnd);

#endif

