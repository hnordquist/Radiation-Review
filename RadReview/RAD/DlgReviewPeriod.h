
/* DlgReviewPeriod.H  Function Prototypes */

#ifndef DlgReviewPeriod_H
	#define DlgReviewPeriodSummary_H
	
	void ReviewPeriodMenuOption(BOOL bAutomated);
	BOOL ReviewPeriodMenuOptionExecuted();
//	void GetReviewPeriodFromMenuOption(unsigned long *pulStart, unsigned long *pulEnd, unsigned long *pulIntervalInDays);
	void GetReviewPeriodFromMenuOption(DATE *pdStart, DATE *pdEnd, unsigned long *pulIntervalInDays);

#endif
