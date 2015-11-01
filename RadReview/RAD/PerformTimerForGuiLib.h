#if !defined(PERFORMTIMERFORGUI_H)
#define PERFORMTIMERFORGUI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

void ClearPerformanceTimer();

void DisplayPerformanceTimer();

void StartPerfomanceTimerInterval();

void MarkPerformanceTimerIntervalEnd(const char *szIntervalLabel);



#endif // !defined(PERFORMTIMERFORGUI_H)
