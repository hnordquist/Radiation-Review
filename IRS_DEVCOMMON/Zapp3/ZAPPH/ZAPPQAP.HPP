#ifndef _ZAP_QAP_
#define _ZAP_QAP_ 1
  #include "qapwinek.h"
void ZAPQAP_Terminate();
extern int QAPActive;

  #define FIND_WIDGET zAppGetAppVar(app)->winMap()->get(pArgs->hWnd)
  #define QAPDECL(name) void QAPFUNC name(PARGS pArgs)
  #define QAPINITARGS
  #define QAPINIT QAP_Initialize()

void ZAPQAP_Init(QAPINITARGS);
void ZIPQAP_Init(QAPINITARGS);

#define GET_OBJECT(name, type)\
    type name = (type)FIND_WIDGET;\
    if (name==0) {\
      QAP_RaiseError(1,"Widget not found.");\
      return;\
    }
#define GetListArgItem(list, index, type) GetListItem(GetArg(list,List),index,type)

//Method declarations:
//Each method declaration is 'QAPDECL(<class>_<method>);'

extern QAPDECL(zWindow_BackgroundColor);
extern QAPDECL(zWindow_SetFocus);
extern QAPDECL(Scrollbar_GetPosition);
extern QAPDECL(zTabBox_GetTabRect);
extern QAPDECL(zTabBox_GetSelected);

//zTextPane
extern QAPDECL(zTextPane_ClearRect);
extern QAPDECL(zTextPane_GetPos);
extern QAPDECL(zTextPane_MoveStart);
extern QAPDECL(zTextPane_MoveHome);
extern QAPDECL(zTextPane_MoveDown);
extern QAPDECL(zTextPane_MoveRight);
extern QAPDECL(zTextPane_MoveLeft);
extern QAPDECL(zTextPane_MoveUp);
extern QAPDECL(zTextPane_MoveTo);
extern QAPDECL(zTextPane_ClearRect);
extern QAPDECL(zTextPane_DrawLine);
extern QAPDECL(zTextPane_LogToText);
extern QAPDECL(zTextPane_LogToText);
extern QAPDECL(zTextPane_TextToLog);
extern QAPDECL(zTextPane_TextToLog);
extern QAPDECL(zTextPane_Put);
extern QAPDECL(zTextPane_GetStartOfLine);
extern QAPDECL(zTextPane_GetPreferredSize);
extern QAPDECL(zTextPane_PrintAt);
extern QAPDECL(zTextPane_SetMetrics);
extern QAPDECL(zTextPane_GetText);

extern QAPDECL(zPane_PutTextAt);
extern QAPDECL(zPane_TextColor);
extern QAPDECL(zPane_FontSize);
extern QAPDECL(zPane_FontName);

extern QAPDECL(zEditor__GetSelection);

extern QAPDECL(zRadioButton_IsChecked);

extern QAPDECL(zMDIAppFrame__CloseWindow);
#endif
