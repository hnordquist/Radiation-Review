
// GUI_FSTR.h

#if !defined GUI_FSTR_H
	#define GUI_FSTR_H
	#include "gui_str.h"
	#include "gui_lib.h"	
	
	class CGFmtStr : public CGStr
	{
//		friend void CGUI_Dbg::Printf(const char *szFormat, ...);
		public: 
			CGFmtStr();
			CGFmtStr(const char *szInitialStr);
			CGFmtStr& Printf(const char *szFormat, ... );
			CGFmtStr& PrintfAppend(const char *szFormat, ... );
			CGFmtStr& DoPrintf(const char *szFormat, va_list pArgs);
			CGFmtStr& operator = (const CGStr& s);
			CGFmtStr& operator = (const char* szText);

		private:
	};
	
#endif
