
/* Plot.H  Function Prototypes */

#ifndef Plot_H
	#define Plot_H
	
	void GraphData(void);
	void CloseDataGraph(void);
	void GraphSetDisplayDate(DATE dAbsDisplayStart, DATE dReqViewPeriodDATE);
	BOOL GraphSetCursorTime(DATE dAbsCursorTime, int iPL_ChanIndex = 1);
	void GraphGetViewPeriodParameters(DATE *pdDisplayStartTime, DATE *pdViewPeriodInDATE);
	BOOL GraphDisplayedNow();
	void UpdateGraphData(BOOL bDisplayFirstIfNecessary);

#endif
