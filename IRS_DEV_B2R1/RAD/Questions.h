
/* Questions.H  Function Prototypes */

#ifndef Questions_H
	#define Questions_H
	
	int AskAndMakePath(char *TempPath);
	int AskOverwriteQuestion(char *TempPath);
	int AskNotEnoughRoomQuestion(unsigned long ulDbSize, unsigned long ulDiskSpace, char *szTempPath);

#endif
