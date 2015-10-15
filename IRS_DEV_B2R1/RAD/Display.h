// Display.H  Function Prototypes


#ifndef DISPLAY_H
#define DISPLAY_H

#include "ComDataStructures.h"

int DisplayRawData(struct IDStruct StaID, const DATE dStart, const DATE dEnd, const struct DATA_DISPLAY_FLAGS *pFlags, const int iErrAllow);
int DisplayRawData(struct IDStruct StaID, const DATE dStart, const DATE dEnd, const BOOL bAllData, const BOOL *pbBytes, const BOOL bLogFile, const BOOL bChattyBits);

void FormatRawDataPoint(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_LONG_DATA_PT *plPts, CGFmtStr *pFormattedString, const unsigned long ulLineNum);
void FormatRawDataPoint(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_DOUBLE_DATA_PT1 *pdPts, CGFmtStr *pFormattedString, const unsigned long ulLineNum);
void FormatRawDataPoint(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_FLOAT_DATA_PT1 *pfPts, CGFmtStr *pFormattedString, const unsigned long ulLineNum);

void PrintPointToTextWin(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_FLOAT_DATA_PT1 *pfPts, CGUI_TextWin *pWin, const unsigned long ulLineNum, const BOOL bChattyBits = TRUE);
void PrintPointToTextWin(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_DOUBLE_DATA_PT1 *pdPts, CGUI_TextWin *pWin, const unsigned long ulLineNum, const BOOL bChattyBits = TRUE);
void PrintPointToTextWin(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_LONG_DATA_PT *plPts, CGUI_TextWin *pWin, const unsigned long ulLineNum, const BOOL bChattyBits = TRUE);

void PrintPointToTextFile(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_FLOAT_DATA_PT1 *pfPts,TextFile *pFile, const unsigned long ulLineNum);
void PrintPointToTextFile(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_DOUBLE_DATA_PT1 *pdPts, TextFile *pFile, const unsigned long ulLineNum);
void PrintPointToTextFile(const struct db_sta_rec *pdbSta, const DATE dJulianTime, const char *pReasonStr, const WHOLE_LONG_DATA_PT *plPts, TextFile *pFile, const unsigned long ulLineNum);

void PrintStatusDefs(const struct db_sta_rec *pdbSta, unsigned long ulStatus, CGUI_TextWin *pWin);

#endif //DISPLAY_H
