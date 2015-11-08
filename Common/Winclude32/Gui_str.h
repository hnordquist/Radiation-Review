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

// GUI_STR.h

#if !defined GUI_STR_H
	#define GUI_STR_H

	#ifndef RW_NO_STL
		#define RW_NO_STL
	#endif

	#ifndef ZAPP_INCLUDE		// added to get rid of any nested includes
		#define ZAPP_INCLUDE		// Pauline, ErgoTech, 4/7/98
		#include <zapp.hpp>
	#endif
	
	class CGStr : public zString
	{
		public:
			CGStr();
			CGStr(const CGStr& Str);
			CGStr(const char *szInitialStr);
			virtual ~CGStr();
			CGStr& AddChar(char cCharacter, UINT uiNumTimes);
			void CopyTo_sz(char *szDestStr, UINT uiDestDim);
			BOOL Create_sz(char **pszString);	// Caller must "delete [] *pszString".
			void Delete(unsigned int uiCharIndex);
			unsigned int GetDim() const;
			BOOL GetSubStr(unsigned int *puiStartIndex,
				const char *szBreakCharSet, CGStr *pLineStr, char *pcLineBreakChar = NULL) const;
			void GetWrappedStrings(UINT uiCharPerLine, const char *szIndentedLinePrefix,
				CGStr **paLineStr, UINT *puiNumLines);
			const char *Get_sz() const;
			CGStr& StripLeadAndTrail();
			CGStr& operator = (const CGStr& s);
			CGStr& operator = (const char* szText);
			CGStr& operator += (const CGStr& Str);
			CGStr& operator += (const char *szStr);
			CGStr& operator += (char c);
			operator const char *() const;
		private:
	};
	
	BOOL GetSubStr(const char *szInput, unsigned int *puiStartIndex,
		const char *szBreakCharSet, CGStr *pLineStr, char *pcLineBreakChar = NULL);
		        
#endif
	
