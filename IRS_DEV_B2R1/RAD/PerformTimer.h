#if !defined(PERFORMTIMER_H)
#define PERFORMTIMER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyPerformanceTimer

static const UINT uiMAX_TIMER_MARKS = 50;

class CMyPerformanceTimer {
	public:
		CMyPerformanceTimer();
		~CMyPerformanceTimer();
		Clear();
		bool StartInterval();
		bool MarkIntervalEnd(const char *szIntervalLabel);
		void Display(const char *szMsgBoxCaption = NULL);
		void Rollback(UINT uiNumMarksToDrop = 1);
	protected:
		CString m_MarkLabel[uiMAX_TIMER_MARKS];
		UINT m_uiMarkTicks[uiMAX_TIMER_MARKS];
		UINT m_uiNumMarks;
		UINT m_uiStartTicks;
};



#endif // !defined(PERFORMTIMER_H)
