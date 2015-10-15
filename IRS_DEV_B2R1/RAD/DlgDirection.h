
/* DlgDirection.H  Function Prototypes */

#ifndef DlgDirection_H
	#define DlgDirection_H

	class CGUI_TextWin;
	class TextFile;

	void DirectionAnalysisMenuOption();
	BOOL DirectionAnalysisDialogBox();
	BOOL DirectionAnalysisDoIt();
	void CleanUpDirectionAnalysis();

	void FormatDirEvent(struct db_dir_event_rec *pdbDirEvent, unsigned long ulNumPrinted, CGFmtStr *pFormattedStr);
	void PrintDirEventToTextWin(struct db_dir_event_rec *pdbDirEvent, CGUI_TextWin *pWin, unsigned long ulNumPrinted);
	void PrintDirEventToTextFile(struct db_dir_event_rec *pdbDirEvent, TextFile *pFile, unsigned long ulNumPrinted);

#endif
