/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
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
