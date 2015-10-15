
/* TextFile.H  Function Prototypes */

#ifndef TextFile_H

	#define TextFile_H

////////////////////////////////////////////////////////////////////////////////////
//	Class that supports opening, writing to and closing text files.
////////////////////////////////////////////////////////////////////////////////////    								
class TextFile
{
	public:
		TextFile(void);
		~TextFile(void); 

		BOOL OpenTextFile(const char *szFilename, BOOL bAskFile = FALSE);
		BOOL ClearTextFile();
		BOOL WriteLineToTextFile(const char *szLineOfText);
		void FlushTextFile();
		void CloseTextFile();
		BOOL GetOpenStatus();
		BOOL GetCancelStatus();
		const char *GetFilename();


	protected:
		FILE *mpFileHandle;
		BOOL mbOpen;
		BOOL mbCancel;
		CGFmtStr mstrFilename;
	
};


#endif
