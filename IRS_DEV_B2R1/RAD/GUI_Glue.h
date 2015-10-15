// GUI_Glue.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(GUI_GLUE_H)
#define GUI_GLUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int AllocStrArray(int iNumStrs, int iMaxLen, char **pAdrs[]);
void DeAllocStrArray(char **pAdrs);

#endif // !defined(GUI_GLUE_H)
