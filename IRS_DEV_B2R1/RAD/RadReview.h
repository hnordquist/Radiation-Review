// Shirley Klosterbuer
// Los Alamos National Laboratory
// NIS-5
// 4/23/97

// RADRview.H
// Radiation Review specific Review class definition
// NOTE: Defines the operations which are required through the
// generic review tool review definition and defines the specific
// opertions for the operator review tool review


#ifndef RADRVIEW_H

	#define RADRVIEW_H

	#include <gui_lib.h>
	#include <stdlib.h> 
	
	//#define COMPILE_WITH_TIMER
	#include "upperlayer/SEBMacro.H"  // Timer helper procedures - must be after above #define  // 8-Dec-04 SFK Added relative path
	#include "upperlayer/RTReview.H"	// 8-Dec-04 SFK Added relative path
	#include "RADInit.H"

	#include "resource.h"
	#include "RADPROTO.H"
	
	// <<QA>> eventually get the version info from resources
	#define szTOOL_VERSION	"3.2.1.1"		
	#define szCONTACT_NAME		"H. Nordquist"
	#define szCONTACT_EMAIL		"heather@lanl.gov"

                        
	#define uiRAD_REVIEW_ERROR	UINT
    
	
	
	class RADReview : public RTReview {

		public:
		
			// Operator class functions
			RADReview(void); 
			~RADReview(void); 
			

            // Required through RTReview to provide the Review Begin procedure which is specific to operator review
			RTReview::REVIEW_OPERATION RT_Review_Begin(void);

			// Required through RTReview to provide the member functions which display associated data specific to operator review
			//BOOL RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp);
			BOOL RADReview::RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation, BOOL *pbLocation_Failed);
			
			// Required through RTReview to provide the member functions to save all data
			BOOL RT_Review_My_Save(void);
				
            // Required through RTReview to provide the Review Exit procedure which is specific to operator review
     		BOOL RT_Review_My_Exit(void); 
     		                                                                               
			// Enable/disable main menu options
			void RADReview::RADReviewSetMainMenuOptions(void);
			
			// Link with the graphic module 
			void RAD_MatchDataForChannel(const char *pTimeDateStr, struct IDStruct ChanID);
			void RAD_Menu_File_Exit(void);
			void RAD_Common_Exit(void);            
            BOOL bMayClose;
			BOOL bRealTimeUpdate;
			//	Called when graph is closing down.
			void RAD_Review_CloseMatchDataChoices(void);

			// function will give me access to RTReview Print_Header
			BOOL RAD_Print_Header(char **ppszHeader, BOOL bAdd_Tool_Name_Title = TRUE, BOOL bReload_Common_Inspec_Info = TRUE, const char *pszDatabase_Name = NULL, const char *pszAppend_String = NULL);

		protected:

			// Access to the common data items across all review tool operations
			// Note: No internal checking is provided when accessing the common data, it is left
			//			up to the user to use the boolean values to check to see if they are set yet.
			BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
            RADInit *pRadParms;

			// Basic Operator Review operations
			void RAD_Review_RAD_Delete(void);



        private:
        
			SEB_About_Box *mpRAD_About_Rad_Review;
			SEB_Product_Support_Box *mpRAD_Product_Support;
            CGUI_Dlg *mpRAD_Review_Choices;
			CGUI_Menu *mpMainMenu;

            int *mpToolNums;
			int miTools;
			
			void RADReview::RADReviewMakeMainMenu(void);
			void RADReview::Protected_ImportAllMenuOption(void);
			void RADReview::Protected_ImportSomeMenuOption();
			void RADReview::Protected_ImportTodayMenuOption();
			void RADReview::Protected_SelectFacilityMenuOption();
			void RADReview::Protected_DataIntegrityMenuOption();
			void RADReview::Protected_FindEventsMenuOption();
			void RADReview::Protected_DirectionAnalysisMenuOption(void);
			void RADReview::Protected_DetermineMeasurementsMenuOption();
			void RADReview::Protected_ExportToINCCMenuOption();
			void RADReview::Protected_FacilityConfigMenuOption(); //km
			void RADReview::Protected_SensorManagerMenuOption(); //km
			void RADReview::Protected_UnitsManagerMenuOption(); //km
			void RADReview::Protected_RadMarksSummariesMenuOption();
			void RADReview::Protected_EventsSummariesMenuOption();
			void RADReview::Protected_MeasurementSummariesMenuOption();
			void RADReview::Protected_DirectionSummariesMenuOption();
			void RADReview::Protected_DbDaysMenuOption();
			void RADReview::Protected_RawDataMenuOption();
			//void RADReview::Protected_SelectDbMenuOption();
			void RADReview::Protected_BackupDbMenuOption();
			void RADReview::Protected_RestoreDbMenuOption();
			void RADReview::Protected_BackupCfgMenuOption();
			void RADReview::Protected_RestoreCfgMenuOption();
			void RADReview::Protected_CopyLogsMenuOption();
			//void RADReview::Protected_ClearDbMenuOption();
			void RADReview::Protected_ReviewPeriodMenuOption();
			void RADReview::Protected_AdjustStationTimeMenuOption();

			void RADReview::SelectTool(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL RADReview::ChoicesExitNotifyFn(CGUI_Dlg *pDlg, GUI_ACTION Action);

			void RAD_Menu_Help_ProductSupport(void);
			void RAD_Menu_Help_About(void);
			
		};
		
#endif


