
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

		bool OpenTextFile(CString Filename, bool bAskFile = false, bool bWrite = false);
		bool ClearTextFile();
		bool TextFile::WriteLineToTextFile(CString strString);
		void FlushTextFile();
		void CloseTextFile();
		bool GetOpenStatus();
		bool GetCancelStatus();
		CString GetFilename();


	protected:
		//FILE *mpFileHandle;
		CStdioFile File;
		bool mbOpen;
		bool mbCancel;
		CString mstrFilename;
			
};


#endif
