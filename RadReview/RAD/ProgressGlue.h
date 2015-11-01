
/* ProgressGlue.H  Function Prototypes */

#ifndef ProgressGlue_H

	#define ProgressGlue_H

	// define some non-GUI links that can be accesses from the new part of RAd
	void ProgressBarCreate(unsigned long ulTotal_Progress_Calls, unsigned long ulCalls_Per_Update, const char *pszMessage = NULL, const char *pszBox_Title = NULL);
	bool ProgressBarUpdate(const char *pszMessage);
	bool ProgressBarClose(void);

	void TwoLineProgressBarCreate(unsigned long ulTotalProgress_Calls, unsigned long ulCallsPerUpdate, const char *pszLine1, const char *pszLine2, const char *pszBoxTitle, bool bCancelQuestion);
	bool TwoLineProgressBarUpdate(const char *pszLine1, const char *pszLine2);
	bool TwoLineProgressBarClose(void);

#endif
