// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/11/97

// RTInit.H
// Class definition for a Review Tool initialization file
       
       


#ifndef RTINIT_H

	#define RTINIT_H

    #include <gui_lib.h>
    
	#include <stdlib.h> 
	
	#include "SEBBoxes.H"
	#include "InitFile.H"
	#include "RTLbls.H"
	#include "RTError.H"


	#define RTINIT_VERSION		__TIMESTAMP__


	// The commented ones are already defined
	//#define uiRT_ERROR_NO_MEMORY						1
	//#define uiRT_ERROR_NOT_INSTALLED					2		// Requires tool name to be passed
	//#define uiRT_ERROR_INIT_LOAD						3		// Requires param errored for first and param warning for second, NULL accepted
	//#define uiRT_ERROR_CANT_START_TOOL				4		// Requires tool name to be passed
	//#define uiRT_ERROR_PIPE_OPEN_FAIL					5		// Requires tool name and your name to be passed
	//#define uiRT_ERROR_PIPE							6		// Requires tool name to be passed
	//#define uiRT_ERROR_PIPE_DEAD						15		// Requires tool name to be passed
	//#define uiRT_ERROR_OPTIONS_SAVE_FAIL				20
	//#define uiRT_ERROR_OPTIONS_RESET_FAIL				21
	//#define uiRT_ERROR_DB_CANT_SAVE					35
	//#define uiRT_ERROR_INSPEC_CHANGE					40
	//#define uiRT_ERROR_OVERWRITE_RAWDATA				50
	//#define uiRT_ERROR_COMMON_INSPEC_LOAD				60		// Requires label warning name to be passed as first parameter
	//#define uiRT_ERROR_COMMON_INSPEC_SAVE				61
	#define uiRT_INIT_ERROR								UINT
	#define uiRT_INIT_ERROR_NO_MEMORY					100
	#define uiRT_INIT_ERROR_OPTIONS_SAVE_FAIL			120
	#define uiRT_INIT_ERROR_OVERWRITE_RAWDATA			140
	#define uiRT_INIT_ERROR_COMMON_INSPEC_LOAD			150
	#define uiRT_INIT_ERROR_COMMON_INSPEC_SAVE			151




	class RTInit : public Initialization_File {

		public:
		
			RTInit(const char *pszInitialization_Name, RESOURCE_ID Icon = NULL, BOOL bWrite_Access = TRUE); 
			~RTInit(void); 


			// Call this member function to load the parameters
			// Parameters will not be loaded until it is called
			virtual BOOL Load_Parameters(void);
			// Just loads the start mode - Will create the string for you
			BOOL Load_Start_Mode(char **ppszStart_Mode);
			// Just loads the splash screen timeout - You must have the UINT created
			BOOL Load_Splash_Screen_Timeout_Seconds(UINT *puiSplash_Screen_Timeout_Seconds);


            // Switch to a new initialization file
			BOOL Set_New_Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE);
			void RTInit::Initialization_File_Close_Now();		// added SFK 08-27-2004
			            
            
			// Be sure to check that the parameters were completely loaded without error			
			virtual BOOL Parameters_Loaded(void);
			// If inspection info not properly loaded, this class will display error message and clear the inspection info
			// However, the app can continue
			virtual BOOL Inspec_Info_Loaded(void);
			// Returns the errored parameter if Parameters_Loaded returns FALSE, otherwise returns NULL
			virtual const char *Get_Errored_Init_Parameter(void);
			// Returns the warning parameter if Parameters_Loaded returns FALSE, otherwise returns NULL
			virtual const char *Get_Warning_Init_Parameter(void);


 			// Parameter saving member functions           
            virtual BOOL Save_Parameters(void);
			// Automatically saves common inspection info
            BOOL Save_Inspection_Info(void);


			// Inspection information helper functions 
			// Returns TRUE if options changed
			BOOL RT_Init_Change_Inspection_Info(RESOURCE_ID Icon = NULL, BOOL bSet_To_New = FALSE, BOOL bSave_Info = TRUE, BOOL bReload_Inspec_Info = TRUE);
			// Call to reload the inspection information
			// Will display an error message if failed
			BOOL Reload_Common_Inspec_Info(void);
  
  
			///////////////////////////////////////////////////////////////////
			// Data getting and setting member functions
			///////////////////////////////////////////////////////////////////

			// RT_NAME
            const char *Get_My_Name(void);
            BOOL Set_My_Name(const char *pszMy_Name);
            BOOL Reset_My_Name(void);

			// RT_START_MODE
            const char *Get_My_Start_Mode(void);
            BOOL Set_My_Start_Mode(const char *pszMy_Start_Mode);
            BOOL Reset_My_Start_Mode(void);

			//RT_SPLASH_SCREEN_TIMEOUT_SECONDS
            UINT Get_Splash_Screen_Timeout_Seconds(void);
            BOOL Set_Splash_Screen_Timeout_Seconds(UINT uiSplash_Screen_Timeout_Seconds);
            BOOL Reset_Splash_Screen_Timeout_Seconds(void);

			// RT_USE_OTHER_TOOL_AS_COMMANDER
            BOOL Get_IR_Used(void);
            BOOL Set_IR_Used(BOOL bIR_Used);
            BOOL Reset_IR_Used(void);

			// RT_OTHER_TOOL_COMMANDER_NUM
            UINT Get_IR_Number(void);
            BOOL Set_IR_Number(UINT uiIR_Number);
            BOOL Reset_IR_Number(void);

			// RT_OTHER_TOOLS_NUM
            UINT Get_Tools_Num(void);
            BOOL Set_Tools_Num(UINT uiTools_Num);
            BOOL Reset_Tools_Num(void);
            
			// RT_OTHER_TOOLS_EXECS
            const char *Get_Tool_Exec_Name(UINT uiTool_Exec_Index);
            const char **Get_Tool_Exec_Names(void);
            BOOL Set_Tool_Exec_Name(UINT uiTool_Exec_Index, const char *pszTool_Exec_Name);
            BOOL Reset_Tool_Exec_Name(UINT uiTool_Exec_Index);
            BOOL Reset_Tool_Exec_Names(void);
            
			// RT_OTHER_TOOLS_NAMES
            const char *Get_Tool_Name(UINT uiTool_Name_Index);
            const char **Get_Tool_Names(void);
            BOOL Set_Tool_Name(UINT uiTool_Name_Index, const char *pszTool_Name);
            BOOL Reset_Tool_Name(UINT uiTool_Name_Index);
            BOOL Reset_Tool_Names(void);
            
			// RT_OTHER_TOOLS_INSTALLED
            BOOL Get_Tool_Installed(UINT uiTool_Installed_Index);
            BOOL Set_Tool_Installed(UINT uiTool_Installed_Index, BOOL bTool_Installed);
            BOOL Reset_Tool_Installed(UINT uiTool_Installed_Index);
            BOOL Reset_Tool_Installed(void);
            
			// RT_PIPE_MY_NAME
            const char *Get_Pipe_My_Name(void);
            BOOL Set_Pipe_My_Name(const char *pszPipe_My_Name);
            BOOL Reset_Pipe_My_Name(void);
            
			// RT_PIPE_OTHER_NAMES
            const char *Get_Pipe_Other_Name(UINT uiPipe_Other_Name_Index);
            const char **Get_Pipe_Other_Names(void);
            BOOL Set_Pipe_Other_Name(UINT uiPipe_Other_Name_Index, const char *pszPipe_Other_Name);
            BOOL Reset_Pipe_Other_Name(UINT uiPipe_Other_Name_Index);
            BOOL Reset_Pipe_Other_Names(void);
            
			// RT_PIPE_OTHER_TIMEOUTS
            UINT Get_Pipe_Timeout(UINT uiPipe_Timeout_Index);
            BOOL Set_Pipe_Timeout(UINT uiPipe_Timeout_Index, UINT uiPipe_Timeout);
            BOOL Reset_Pipe_Timeout(UINT uiPipe_Timeout_Index);
            BOOL Reset_Pipe_Timeouts(void);
            
			///////////////////////////////////////////////////////////////////

			// RT_INSPEC_INFO_COMMON_FILENAME
            const char *Get_Inspec_Info_Common_Filename(void);
            BOOL Set_Inspec_Info_Common_Filename(const char *pszInspec_Info_Common_Filename);
            BOOL Reset_Inspec_Info_Common_Filename(void);

			// RT_MIC_INSPEC_INFO_FILE_USE
            BOOL Get_MIC_Inspec_Info_File_Use(void);
            BOOL Set_MIC_Inspec_Info_File_Use(BOOL bMIC_Inspec_Info_File_Use);
            BOOL Reset_MIC_Inspec_Info_File_Use(void);

			// RT_MIC_INSPEC_INFO_FILENAME
            const char *Get_MIC_Inspec_Info_Filename(void);
            BOOL Set_MIC_Inspec_Info_Filename(const char *pszMIC_Inspec_Info_Filename);
            BOOL Reset_MIC_Inspec_Info_Filename(void);

			// RT_FACILITY_NAME
            const char *Get_Facility_Name(void);
            BOOL Set_Facility_Name(const char *pszFacility_Name);
            BOOL Reset_Facility_Name(void);
            
			// RT_FACILITY_LOCATION
            const char *Get_Facility_Location(void);
            BOOL Set_Facility_Location(const char *pszFacility_Location);
            BOOL Reset_Facility_Location(void);
            
			// RT_INSPECTION_NUMBER
            const char *Get_Inspection_Number(void);
            BOOL Set_Inspection_Number(DWORD dInspection_Number);
            BOOL Set_Inspection_Number(const char *dInspection_Number);
            BOOL Reset_Inspection_Number(void);
            
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			// RT_INSPECTION_DATE
            const GEN_DATE_STRUCT *Get_Inspection_Date(void);
            BOOL Set_Inspection_Date(const GEN_DATE_STRUCT *pInspection_Date);
            BOOL Reset_Inspection_Date(void);
            
			// RT_INSPECTION_TIME
            const GEN_TIME_STRUCT *Get_Inspection_Time(void);
            BOOL Set_Inspection_Time(const GEN_TIME_STRUCT *pInspection_Time);
            BOOL Reset_Inspection_Time(void);
            
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			// RT_INSPECTION_DATA_DATE_START
            const GEN_DATE_STRUCT *Get_Start_Date(void);
            BOOL Set_Start_Date(const GEN_DATE_STRUCT *pStart_Date);
            BOOL Reset_Start_Date(void);
            
			// RT_INSPECTION_DATA_TIME_START
            const GEN_TIME_STRUCT *Get_Start_Time(void);
            BOOL Set_Start_Time(const GEN_TIME_STRUCT *pStart_Time);
            BOOL Reset_Start_Time(void);
            
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			// RT_INSPECTION_DATA_DATE_END
            const GEN_DATE_STRUCT *Get_End_Date(void);
            BOOL Set_End_Date(const GEN_DATE_STRUCT *pEnd_Date);
            BOOL Reset_End_Date(void);
            
			// RT_INSPECTION_DATA_TIME_END
            const GEN_TIME_STRUCT *Get_End_Time(void);
            BOOL Set_End_Time(const GEN_TIME_STRUCT *pEnd_Time);
            BOOL Reset_End_Time(void);
            
			// RT_INSPECTORS_NUMBER
            UINT Get_Inspector_ID_Num(void);
            BOOL Set_Inspector_ID_Num(UINT uiInspector_ID_Num);
            BOOL Reset_Inspector_ID_Num(void);
                                                     
			// RT_INSPECTORS_IDS
            const char *Get_Inspector_ID(UINT uiInspector_ID_Index);
            const char **Get_Inspector_IDs(void);
            BOOL Set_Inspector_ID(UINT uiInspector_ID_Index, const char *pszInspector_ID);
            BOOL Set_Inspector_IDs(const char **pszInspector_ID, UINT uiNum_Inspector_ID);
            BOOL Reset_Inspector_ID(UINT uiInspector_ID_Index);
            BOOL Reset_Inspector_IDs(void);
                                      
			// RT_INSPECTORS_IDS_CHOICES_NUM
            UINT Get_Inspector_ID_Choices_Num(void);
            BOOL Set_Inspector_ID_Choices_Num(UINT uiInspector_ID_Choices_Num);
            BOOL Reset_Inspector_ID_Choices_Num(void);
                                                     
			// RT_INSPECTORS_IDS_CHOICES
            UINT Get_Inspector_ID_Choice(UINT uiInspector_ID_Choice_Num);
            const UINT *Get_Inspector_ID_Choices(void);
            BOOL Set_Inspector_ID_Choice(UINT uiInspector_ID_Choice_Num, UINT uiInspector_ID_Choice);
            BOOL Set_Inspector_ID_Choices(UINT *puiInspector_ID_Choices, UINT uiInspector_ID_Choices_Num);
            BOOL Reset_Inspector_ID_Choice(UINT uiInspector_ID_Choice_Num);
            BOOL Reset_Inspector_ID_Choices(void);

			// RT_INSPECTION_COMMENTS
            const char *Get_Inspection_Comments(void);
            BOOL Set_Inspection_Comments(const char *pszInspection_Comments);
            BOOL Reset_Inspection_Comments(void);

			///////////////////////////////////////////////////////////////////

			// RT_COMMON_DATABASE_PATH
			// NOTE: Always returns a path with a '\' at the end, regardless of whether user adds one or not
            const char *Get_Common_Database_Path(void);
            BOOL Set_Common_Database_Path(const char *pszCommon_Database_Path);
            BOOL Reset_Common_Database_Path(void);

			///////////////////////////////////////////////////////////////////

			// RT_SUMMARY_DATA_CREATE_FILE
            BOOL Get_Summary_Data_Create_File(void);

			// RT_SUMMARY_DATA_FILENAME
            const char *Get_Summary_Data_Filename(void);

			///////////////////////////////////////////////////////////////////

			// RT_DATA_MATCH_ERROR_SECONDS
            float Get_Data_Match_Error_Seconds(void);
            BOOL Set_Data_Match_Error_Seconds(float fData_Match_Error_Seconds);
            BOOL Reset_Data_Match_Error_Seconds(void);

			// RT_DATA_MATCH_MINIMIZE
            BOOL Get_Data_Match_Minimize(void);
            BOOL Set_Data_Match_Minimize(BOOL bData_Match_Minimize);
            BOOL Reset_Data_Match_Minimize(void);
 
			// RT_DATA_MATCH_RESTORE
            BOOL Get_Data_Match_Restore(void);
            BOOL Set_Data_Match_Restore(BOOL bData_Match_Restore);
            BOOL Reset_Data_Match_Restore(void);

			// RT_DATA_MATCH_OTHER_TOOLS
            BOOL Get_Data_Match_Tool(UINT uiData_Match_Tool);
            BOOL Set_Data_Match_Tool(UINT uiData_Match_Tool, BOOL bData_Match_Tool);
            BOOL Reset_Data_Match_Tool(void);

			// RT_DATA_MATCH_DISPLAY_CONNECT_MESSAGE
            BOOL Get_Data_Match_Display_Connect_Message(UINT uiData_Match_Tool);
            BOOL Set_Data_Match_Display_Connect_Message(UINT uiData_Match_Tool, BOOL bDisplay_Connect_Message);
            BOOL Reset_Data_Match_Display_Connect_Message(void);

			// RT_DATA_MATCH_NEVER_SEND_LOCATION
            BOOL Get_Data_Match_Never_Send_Location(UINT uiData_Match_Tool);
            BOOL Set_Data_Match_Never_Send_Location(UINT uiData_Match_Tool, BOOL bNever_Send_Location);
            BOOL Reset_Data_Match_Never_Send_Location(void);

			// RT_DATA_MATCH_ALWAYS_SEND_LOCATION
            BOOL Get_Data_Match_Always_Send_Location(UINT uiData_Match_Tool);
            BOOL Set_Data_Match_Always_Send_Location(UINT uiData_Match_Tool, BOOL bAlways_Send_Location);
            BOOL Reset_Data_Match_Always_Send_Location(void);



			// Member functions to reset to defaults
			BOOL Reset_Inspection_Info(void);
            virtual BOOL Reset_Parameters(void);
            

			// Getting associated parameter member functions
			BOOL Get_Pipe_Other_Name(const char *pszTool_Name, const char **ppszPipe_Other_Name);
			UINT Get_Tool_Index_Number(const char *pszTool_Name);
			UINT Get_Pipe_Other_Index_Number(const char *pszPipe_Other_Name);
			UINT Get_Pipe_Timeout_Max(void);

            
      		// Use the default error messages below except when an error occurs specific to your tool
      		Review_Tool_Error RTError;
            void RT_Init_Error(uiRT_INIT_ERROR uiError, const char *pszError_Parameter_1 = NULL, const char *pszError_Parameter_2 = NULL);



		protected:
		

			// Set this parameter to the label that errored but is not essential for running 
			//     and it will automatically get reported when application starts
			// NOTE: You must report error if you reload the parameters later (by possible calling proper
			//      Review_Tool_Error
			//       These variables retain their values from their last setting even if parameter loading
			//        later does not fail
			CGStr Label_Warning;

			// Set this parameter to the label that errored and it will automatically get reported
			//      when application starts
			// NOTE: You must report error if you reload the parameters later (by possible calling proper
			//      Review_Tool_Error
			//       These variables retain their values from their last setting even if parameter loading
			//        later does not fail
			CGStr Label_Error;


		private:           
		    
		    // Review Tool Parameters
            typedef struct PARAMS_SETUP_TAG {
				char *pszInspec_Info_Common_Filename;
            	char *pszFacility_Name;
				SEB_IAEA_Inspection_Box::IAEA_INSPECTION_INFO Inspection_Info;
				} PARAMS_SETUP;     
				
            typedef struct PARAMS_REANALYZE_IMPORT_TAG {
				char *pszCommon_Database_Path;
 				BOOL bMIC_Inspec_Info_File_Use;
 				char *pszMIC_Inspec_Info_Filename;
				BOOL bSummary_Data_Create_File;
				char *pszSummary_Data_Filename;
				} PARAMS_REANALYZE_IMPORT;
				
            typedef struct PARAMS_REVIEW_TAG {
            	UINT uiTools_Num;
				BOOL bIR_Used;
				UINT uiIR_Number;
            	char **ppszTool_Name;
            	char **ppszTool_Exec_Name;
				BOOL *pbTool_Installed;
				BOOL *pbData_Match_Tools;
				BOOL *pbData_Match_Display_Connect_Message;
				BOOL *pbData_Match_Never_Send_Location;
				BOOL *pbData_Match_Always_Send_Location;
				float fData_Match_Error_Seconds;
				BOOL bData_Match_Minimize;
				BOOL bData_Match_Restore;
				} PARAMS_REVIEW;

            typedef struct PARAMS_PIPE_TAG {
				UINT *puiPipe_Timeout;
				char *pszPipe_My_Name;
				char **ppszPipe_Other_Name;
				} PARAMS_PIPE;
				      
            typedef struct RT_PARAMS_TAG {
				PARAMS_SETUP Setup;
				PARAMS_REANALYZE_IMPORT ReanalyzeImport;
				PARAMS_REVIEW Review;
				PARAMS_PIPE Pipes;
				char *pszMy_Name;
				char *pszMy_Start_Mode;
				UINT uiSplash_Screen_Timeout_Seconds;
				} RT_PARAMS;
				
				
			RT_PARAMS *pParams_Current, *pParams_Default;
		    BOOL bSave_Ability, bParams_Loaded, bInspec_Info_Loaded;
			RESOURCE_ID uiIcon;
		    

			BOOL RTInit_Load_Params(RT_PARAMS *pParams);
			// Call this one to specify whether to force a save to the app ini
            BOOL Save_Inspection_Info(BOOL bForce_Save_To_App_INI);
			BOOL Load_Inspection_Info(RT_PARAMS *pParams);
			BOOL Load_Common_Inspection_Info(RT_PARAMS *pParams);
		    void RTInit_Delete_Params(RT_PARAMS *pParams);
			void Clear_Common_Inspection_Info(RT_PARAMS *pParams);
		    void Delete_Common_Inspection_Info(RT_PARAMS *pParams);

		};
		
#endif


		
