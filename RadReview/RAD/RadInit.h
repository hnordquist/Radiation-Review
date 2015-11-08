/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
// RADInit.H
// Class definition for the Radiation Review specific initialization file data
       
       


#ifndef RADINIT_H

	#define RADINIT_H


//	#include <stdlib.h> 
	
	#include "upperlayer/RTInit.H"	// 8-Dec-04 SFK Added relative path
	#include "RADLbls.H"
                           
	#define szTOOL_INI "..\\RAD.INI"  
	#define szTOOL_NAME "Radiation Review"
		
    extern CGUI_App *pMy_Application;		// because of my strange application


	class RADInit : public RTInit {

		public:
		    
			RADInit(const char *pszInitialization_Name,  RESOURCE_ID Icon = NULL, BOOL bWrite_Access = TRUE); 
			~RADInit(void); 
           
			// Call this member function to load the parameters
			// Parameters will not be loaded until it is called
			BOOL Load_Parameters(void);

            // Switch to a new initialization file
			virtual BOOL Set_New_Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE);
			void Initialization_File_Close();	// SFK added 08-24-2004
		    
			// Be sure to check that the parameters were completely loaded without error			
			virtual BOOL Parameters_Loaded(void);

			// Data getting and setting member functions 

		//	BOOL RADInit::Get_RTInspectionValid(void);
		//	BOOL RADInit::Set_RTInspectionValid(BOOL bNewInspectionInfoValid);
		//	BOOL RADInit::Reset_RTInspectionValid(void);

			char *RADInit::Get_NCC_DataPath(void);
			BOOL RADInit::Set_NCC_DataPath(const char *pszNCC_DataPath);
			BOOL RADInit::Reset_NCC_DataPath(void);

			BOOL RADInit::Get_AutomateQuestion(void);
			BOOL RADInit::Set_AutomateQuestion(BOOL bNewAutomateQuestion);
			BOOL RADInit::Reset_AutomateQuestion(void);

			BOOL RADInit::Get_AutomateImportAll(void);
			BOOL RADInit::Set_AutomateImportAll(BOOL bNewAutomateImportAll);
			BOOL RADInit::Reset_AutomateImportAll(void);

			BOOL RADInit::Get_AutomateImportToday(void);
			BOOL RADInit::Set_AutomateImportToday(BOOL bNewAutomateImportToday);
			BOOL RADInit::Reset_AutomateImportToday(void);

			BOOL RADInit::Get_AutomateIntegrity(void);
			BOOL RADInit::Set_AutomateIntegrity(BOOL bNewAutomateIntegrity);
			BOOL RADInit::Reset_AutomateIntegrity(void);

			BOOL RADInit::Get_AutomateFindEvents(void);
			BOOL RADInit::Set_AutomateFindEvents(BOOL bNewAutomateFindEvents);
			BOOL RADInit::Reset_AutomateFindEvents(void);

			BOOL RADInit::Get_AutomateDirection(void);
			BOOL RADInit::Set_AutomateDirection(BOOL bNewAutomateDirection);
			BOOL RADInit::Reset_AutomateDirection(void);

			BOOL RADInit::Get_AutomateGraph(void);
			BOOL RADInit::Set_AutomateGraph(BOOL bNewAutomateGraph);
			BOOL RADInit::Reset_AutomateGraph(void);

			BOOL RADInit::Get_AutomateMeasurement(void);
			BOOL RADInit::Set_AutomateMeasurement(BOOL bNewAutomateMeasurement);
			BOOL RADInit::Reset_AutomateMeasurement(void);

			BOOL RADInit::Get_AutomateExport(void);
			BOOL RADInit::Set_AutomateExport(BOOL bNewAutomateExport);
			BOOL RADInit::Reset_AutomateExport(void);

			BOOL RADInit::Get_Menu_Opt_Import_Selected(void);
			BOOL RADInit::Set_Menu_Opt_Import_Selected(BOOL bNewImport_Selected_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Import_Selected(void);

			BOOL RADInit::Get_Menu_Opt_Import_Today(void);
			BOOL RADInit::Set_Menu_Opt_Import_Today(BOOL bNewImport_Today_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Import_Today(void);

			BOOL RADInit::Get_Menu_Opt_Determine_Measurements(void);
			BOOL RADInit::Set_Menu_Opt_Determine_Measurements(BOOL bNewDetermine_Measurements_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Determine_Measurements(void);

			BOOL RADInit::Get_Menu_Opt_Direction_Analysis(void);
			BOOL RADInit::Set_Menu_Opt_Direction_Analysis(BOOL bNewDirection_Analysis_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Direction_Analysis(void);

			BOOL RADInit::Get_Menu_Opt_Export_to_INCC(void);
			BOOL RADInit::Set_Menu_Opt_Export_to_INCC(BOOL bNewExport_to_INCC_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Export_to_INCC(void);

			BOOL RADInit::Get_Menu_Opt_Measurements_Summary(void);
			BOOL RADInit::Set_Menu_Opt_Measurements_Summary(BOOL bNewMeasurements_Summary_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Measurements_Summary(void);

			BOOL RADInit::Get_Menu_Opt_Direction_Summary(void);
			BOOL RADInit::Set_Menu_Opt_Direction_Summary(BOOL bNewDirection_Summary_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Direction_Summary(void);

			BOOL RADInit::Get_Menu_Opt_Data_Integrity(void);
			BOOL RADInit::Set_Menu_Opt_Data_Integrity(BOOL bNewData_Integrity_Menu_Opt);
            BOOL RADInit::Reset_Menu_Opt_Data_Integrity(void);

			BOOL RADInit::Get_WriteChanEventResults(void);
			BOOL RADInit::Set_WriteChanEventResults(BOOL bNewWriteChanEventResults);
            BOOL RADInit::Reset_WriteChanEventResults(void);

			BOOL RADInit::Get_WriteDirEventResults(void);
			BOOL RADInit::Set_WriteDirEventResults(BOOL bNewWriteDirEventResults);
            BOOL RADInit::Reset_WriteDirEventResults(void);

			UINT RADInit::Get_WriteMeasurementResults(void);	//1/3/2005 Change to UINT type
			BOOL RADInit::Set_WriteMeasurementResults(UINT uiNewWriteMeasurementResults);
            BOOL RADInit::Reset_WriteMeasurementResults(void);

//			const char *RADInit::Get_Facility_Program_Path(void);
//			BOOL RADInit::Set_Facility_Program_Path(const char *pszFAC_Path);
//			BOOL RADInit::Reset_Facility_Program_Path(void);

//			const char *RADInit::Get_Facility_Program_Name(void);
//			BOOL RADInit::Set_Facility_Program_Name(const char *pszFAC_Name);
//			BOOL RADInit::Reset_Facility_Program_Name(void);
		                                                     
//			const char *RADInit::Get_Facility_Names(void);
//			BOOL RADInit::Set_Facility_Names(const char *pszNamesCfg);
//			BOOL RADInit::Reset_Facility_Names(void);

//			const char *RADInit::Get_dbDatabase_Path(void);
//			BOOL RADInit::Set_dbDatabase_Path(const char *pszdbPath);
//			BOOL RADInit::Reset_dbDatabase_Path(void);

//			const char *RADInit::Get_RADExe_Path(void);
//			BOOL RADInit::Set_RADExe_Path(const char *pszdbPath);
//			BOOL RADInit::Reset_RADExe_Path(void);

			char *RADInit::Get_Import_All_Path(void);
			BOOL RADInit::Set_Import_All_Path(const char *pszImportAllPath);
			BOOL RADInit::Reset_Import_All_Path(void);

			char *RADInit::Get_Import_All_Wildcard(void);
			BOOL RADInit::Set_Import_All_Wildcard(const char *pszImportAllWildcard);
			BOOL RADInit::Reset_Import_All_Wildcard(void);

			BOOL RADInit::Get_Import_All_Overwrite(void);
			BOOL RADInit::Set_Import_All_Overwrite(BOOL bNewImportAllOverwrite);
			BOOL RADInit::Reset_Import_All_Overwrite(void);

			BOOL RADInit::Get_Import_All_Init(void);
			BOOL RADInit::Set_Import_All_Init(BOOL bNewImportAllInit);
			BOOL RADInit::Reset_Import_All_Init(void);

			BOOL RADInit::Get_Import_All_Subfolders(void);
			BOOL RADInit::Set_Import_All_Subfolders(BOOL bNewImportAllSubfolders);
			BOOL RADInit::Reset_Import_All_Subfolders(void);

			char *RADInit::Get_Import_Select_Path(void);
			BOOL RADInit::Set_Import_Select_Path(const char *pszImportSelectPath);
			BOOL RADInit::Reset_Import_Select_Path(void);

			char *RADInit::Get_Import_Select_Wildcard(void);
			BOOL RADInit::Set_Import_Select_Wildcard(const char *pszImportSelectWildcard);
			BOOL RADInit::Reset_Import_Select_Wildcard(void);

			BOOL RADInit::Get_Import_Select_Overwrite(void);
			BOOL RADInit::Set_Import_Select_Overwrite(BOOL bNewImportSelectOverwrite);
			BOOL RADInit::Reset_Import_Select_Overwrite(void);

			BOOL RADInit::Get_Import_Select_Init(void);
			BOOL RADInit::Set_Import_Select_Init(BOOL bNewImportSelectInit);
			BOOL RADInit::Reset_Import_Select_Init(void);

			BOOL RADInit::Get_Import_Select_Subfolders(void);
			BOOL RADInit::Set_Import_Select_Subfolders(BOOL bNewImportSelectSubfolders);
			BOOL RADInit::Reset_Import_Select_Subfolders(void);

			char *RADInit::Get_Import_Today_Path(void);
			BOOL RADInit::Set_Import_Today_Path(const char *pszImportTodayPath);
			BOOL RADInit::Reset_Import_Today_Path(void);

			char *RADInit::Get_Import_Today_Wildcard(void);
			BOOL RADInit::Set_Import_Today_Wildcard(const char *pszImportTodayWildcard);
			BOOL RADInit::Reset_Import_Today_Wildcard(void);

			BOOL RADInit::Get_Import_Today_Overwrite(void);
			BOOL RADInit::Set_Import_Today_Overwrite(BOOL bNewImportTodayOverwrite);
			BOOL RADInit::Reset_Import_Today_Overwrite(void);

			BOOL RADInit::Get_Import_Today_Init(void);
			BOOL RADInit::Set_Import_Today_Init(BOOL bNewImportTodayInit);
			BOOL RADInit::Reset_Import_Today_Init(void);

			BOOL RADInit::Get_Import_Today_Subfolders(void);
			BOOL RADInit::Set_Import_Today_Subfolders(BOOL bNewImportTodaySubfolders);
			BOOL RADInit::Reset_Import_Today_Subfolders(void);

			BOOL RADInit::Get_Import_Today_Since_Last(void);
			BOOL RADInit::Set_Import_Today_Since_Last(BOOL bNewImportTodaySinceLast);
			BOOL RADInit::Reset_Import_Today_Since_Last(void);

			char *RADInit::Get_dbBackup_Path(void);
			BOOL RADInit::Set_dbBackup_Path(const char *pszDbBackupPath);
			BOOL RADInit::Reset_dbBackup_Path(void);

			char *RADInit::Get_dbRestore_From_Path(void);
			BOOL RADInit::Set_dbRestore_From_Path(const char *pszDbRestoreFromPath);
			BOOL RADInit::Reset_dbRestore_From_Path(void);

			char *RADInit::Get_CfgBackup_Path(void);
			BOOL RADInit::Set_CfgBackup_Path(const char *pszCfgBackupPath);
			BOOL RADInit::Reset_CfgBackup_Path(void);

			char *RADInit::Get_CfgRestore_From_Path(void);
			BOOL RADInit::Set_CfgRestore_From_Path(const char *pszCfgRestoreFromPath);
			BOOL RADInit::Reset_CfgRestore_From_Path(void);

			char *RADInit::Get_LogCopy_To_Path(void);
			BOOL RADInit::Set_LogCopy_To_Path(const char *pszLogCopyToPath);
			BOOL RADInit::Reset_LogCopy_To_Path(void);

			BOOL RADInit::Get_RealTimeUpdate(void);
			BOOL RADInit::Set_RealTimeUpdate(BOOL bNewRealTimeUpdate);
            BOOL RADInit::Reset_RealTimeUpdate(void);

			BOOL RADInit::Get_ReanalyzeEnabled(void);
			BOOL RADInit::Set_ReanalyzeEnabled(BOOL bNewReanalyzeEnabled);
			BOOL RADInit::Reset_ReanalyzeEnabled(void);

			BOOL RADInit::Get_MessageIfNoLocationMatch(void);
			BOOL RADInit::Set_MessageIfNoLocationMatch(BOOL bNewMessageIfNoLocationMatch);
			BOOL RADInit::Reset_MessageIfNoLocationMatch(void);

//			BOOL RADInit::Get_SendLocation(void);
//			BOOL RADInit::Set_SendLocation(BOOL bNewSendLocation);
//			BOOL RADInit::Reset_SendLocation(void);

            UINT RADInit::Get_LocationMatchType(void);
            BOOL RADInit::Set_LocationMatchType(UINT uiLocationMatchType);
            BOOL RADInit::Reset_LocationMatchType(void);

            UINT RADInit::Get_LocationMatchItem(void);
            BOOL RADInit::Set_LocationMatchItem(UINT uiLocationMatchItem);
            BOOL RADInit::Reset_LocationMatchItem(void);

            float RADInit::Get_Meas_AT_Threshold(void);
            BOOL RADInit::Set_Meas_AT_Threshold(float fMeasAT_Threshold);
            BOOL RADInit::Reset_Meas_AT_Threshold(void);
	
            float RADInit::Get_Meas_AT_Bias(void);
            BOOL RADInit::Set_Meas_AT_Bias(float fMeasAT_Bias);
            BOOL RADInit::Reset_Meas_AT_Bias(void);

            float RADInit::Get_Meas_AT_Sigma(void);
            BOOL RADInit::Set_Meas_AT_Sigma(float fMeasAT_Sigma);
            BOOL RADInit::Reset_Meas_AT_Sigma(void);

            float RADInit::Get_Meas_GateWidth(void);
            BOOL RADInit::Set_Meas_GateWidth(float fMeasGateWidth);
            BOOL RADInit::Reset_Meas_GateWidth(void);

			BOOL RADInit::Get_MeasTerminateOnMoving(void);
			BOOL RADInit::Set_MeasTerminateOnMoving(BOOL bMeasTerminateOnMoving);
			BOOL RADInit::Reset_MeasTerminateOnMoving(void);

			BOOL RADInit::Get_MeasInclude_AT_Fail(void);
			BOOL RADInit::Set_MeasInclude_AT_Fail(BOOL bMeasInclude_AT_Fail);
			BOOL RADInit::Reset_MeasInclude_AT_Fail(void);

			BOOL RADInit::Get_ImportDiskPrompt(void);
			BOOL RADInit::Set_ImportDiskPrompt(BOOL bImportDiskPrompt);
			BOOL RADInit::Reset_ImportDiskPrompt(void);

			BOOL RADInit::Get_ImportMultipleDisks(void);
			BOOL RADInit::Set_ImportMultipleDisks(BOOL bImportMultipleDisks);
			BOOL RADInit::Reset_ImportMultipleDisks(void);
			
			// 11-Jul-2005 SFK Added
			float RADInit::Get_RTUpperLimit(void);
			BOOL RADInit::Set_RTUpperLimit(float fRTUpperLimit);
			BOOL RADInit::Reset_RTUpperLimit(void);

			// 11-Jul-2005 SFK Added
			BOOL RADInit::Get_EnableRatios(void);
			BOOL RADInit::Set_EnableRatios(BOOL bEnableRatios);
			BOOL RADInit::Reset_EnableRatios(void);

			// Parameter saving member functions           
            virtual BOOL Save_Parameters(void);         


			// Reset all to defaults
            virtual BOOL Reset_Parameters(void);         
            
            BOOL bParams_Loaded;
           


		protected:
		


		private:


	// RAD Review specific parameters
			typedef struct RAD_PARAMS_STRUCT_TAG {
				BOOL bImport_Selected_Menu_Opt;
				BOOL bImport_Today_Menu_Opt;
				BOOL bDetermine_Measurements_Menu_Opt;
				BOOL bDirection_Analysis_Menu_Opt;
				BOOL bExport_to_INCC_Menu_Opt;
				BOOL bMeasurements_Summary_Menu_Opt;
				BOOL bData_Integrity_Menu_Opt;
				BOOL bDirection_Summary_Menu_Opt;
				BOOL bChanEventResults;
				BOOL bDirEventResults;
				UINT uiMeasurementResults;		//1/3/2005 Changed to an int to keep more write information
				BOOL bReanalyzeEnabled;
				BOOL bMessageIfNoLocationMatch;
				//BOOL bSendLocation;
				BOOL bImportDiskPrompt;
				BOOL bImportMultipleDisks;
				
//				char *pszFAC_Path;
//				char *pszFAC_Name;  
		
			//	char *pszNamesCfg;
//				char *pszdbPath;  
			//	char *pszRADExePath;  
		
		//		BOOL bInspectionInfoValid;		
				BOOL bAutomateImportAll;
				BOOL bAutomateImportToday;
				BOOL bAutomateIntegrity;
				BOOL bAutomateFindEvents;
				BOOL bAutomateDirection;
				BOOL bAutomateGraph;
				BOOL bAutomateMeasurement;
				BOOL bAutomateExport;
				BOOL bAutomateQuestion;
				BOOL bRealTimeUpdate;
				
				char *pszNCC_DataPath;
			
				char *pszImportAllPath;
				char *pszImportAllWildcard;
				BOOL bImportAllOverwrite;
				BOOL bImportAllInit;
				BOOL bImportAllSubfolders;
				
				char *pszImportTodayPath;
				char *pszImportTodayWildcard;
				BOOL bImportTodayOverwrite;
				BOOL bImportTodayInit;
				BOOL bImportTodaySubfolders;
				BOOL bImportTodaySinceLast;
				
				char *pszImportSelectPath;
				char *pszImportSelectWildcard;
				BOOL bImportSelectOverwrite;
				BOOL bImportSelectInit;
				BOOL bImportSelectSubfolders;
				
				char *pszDbBackupPath;
				char *pszDbRestoreFromPath;
				char *pszCfgBackupPath;
				char *pszCfgRestoreFromPath;
				char *pszLogCopyToPath;

				UINT uiLocationMatchType;
				UINT uiLocationMatchItem;

				float fMeasAT_Threshold;
				float fMeasAT_Bias;
				float fMeasAT_Sigma;
				float fMeasGateWidth;
				BOOL bMeasTerminateOnMoving;
				BOOL bMeasInclude_AT_Fail;

				float fRTUpperLimit;		// 11-Jul-2005 SFK added
				BOOL bEnableRatios;			// 11-Jul-2005 SFK added
				
				} RAD_PARAMS;
		           
		    
			RAD_PARAMS *pParams_Current, *pParams_Default;
		    BOOL bSave_Ability;
		    
			BOOL RADInit_Load_Params(RAD_PARAMS *pParams);
		    void RADInit_Delete_Params(RAD_PARAMS *pParams);

		};
		
#endif



		
