
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
	
