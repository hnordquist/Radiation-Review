///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : PerformTimer.cpp     
//Primary Author: Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "Radbuild.h"
//#include "stdafx.h"
#include "PerformTimer.h"
#include "PerformTimerForGuiLib.h"

CMyPerformanceTimer MyTimer;

void ClearPerformanceTimer()
{
	MyTimer.Clear();
}

void DisplayPerformanceTimer()
{
	MyTimer.Display();
}

void StartPerfomanceTimerInterval()
{
	MyTimer.StartInterval();
}

void MarkPerformanceTimerIntervalEnd(const char *szIntervalLabel)
{
	MyTimer.MarkIntervalEnd(szIntervalLabel);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPerformanceTimer

CMyPerformanceTimer::CMyPerformanceTimer()
{
	Clear();
	m_uiStartTicks = GetTickCount();
}


CMyPerformanceTimer::~CMyPerformanceTimer()
{
}


CMyPerformanceTimer::Clear()
{
	for (UINT ui = 0; ui < uiMAX_TIMER_MARKS; ui++) {
		m_MarkLabel[ui].Empty();
		m_uiMarkTicks[ui] = 0;
	}
	m_uiNumMarks = 0;
	m_uiStartTicks = GetTickCount();
}


bool CMyPerformanceTimer::StartInterval()
{
	m_uiStartTicks = GetTickCount();
	return(TRUE);
}


bool CMyPerformanceTimer::MarkIntervalEnd(const char *szIntervalLabel)
{
	bool bSuccess;

	if (m_uiNumMarks < uiMAX_TIMER_MARKS) {
		m_uiMarkTicks[m_uiNumMarks] = GetTickCount() - m_uiStartTicks;
		m_MarkLabel[m_uiNumMarks] = szIntervalLabel;
		m_uiNumMarks++;
		StartInterval();		// For next interval.
		bSuccess = true;
	}
	else {
		bSuccess = false;
	}
	return(bSuccess);
}


void CMyPerformanceTimer::Display(const char *szMsgBoxCaption)
{
	CString strFormattedData;
	CString strMsgLine;
	UINT uiThisMarkStartTicks = m_uiStartTicks;

	for (UINT ui = 0; ui < m_uiNumMarks; ui++) {
		strMsgLine.Format("%s:  %.3f secs\n", (const char *)m_MarkLabel[ui], ((float)m_uiMarkTicks[ui]) / 1000.0);
		strFormattedData+= strMsgLine;
	}
	const char *pCaption;

	if (szMsgBoxCaption) pCaption = szMsgBoxCaption;
	else pCaption = "Timer Results";
	MessageBeep(0);
	MessageBox(NULL, strFormattedData, pCaption, MB_OK);
}


void CMyPerformanceTimer::Rollback(UINT uiNumMarksToDrop)
{
	if (uiNumMarksToDrop > 0) {
		if (uiNumMarksToDrop > m_uiNumMarks) {
			Clear();
		}
		else {
			m_uiNumMarks -= uiNumMarksToDrop;
		}
	}
}

