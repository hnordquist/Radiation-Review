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
// Shirley Klosterbuer
// Los Alamos National Laboratory
// NIS-5
// 04/22/97
// 05-28-98 Added when to write results files parameters

// RADLbls.H
// The labels specific to Radiation Review in the rad.ini file


#ifndef RADLBLS_H

	#define RADLBLS_H
 
 
	// Installed tools
//	#define szRAD_FAC_PATH				"RAD_FAC_PATH"
//	#define szRAD_FAC_NAME				"RAD_FAC_NAME"

	// File preferences
	#define szRAD_NAMES_CFG  			"RAD_NAMES_CFG"
	#define szRAD_NCC_DATA_PATH			"RAD_NCC_DATA_PATH" 
//	#define szRAD_db_PATH           	"RAD_db_PATH"
	#define szRAD_EXE_PATH				"RAD_EXE_PATH"
	
	// Whether should use the inspection info                      
	#define szRAD_INSPECTION_INFO_VALID	 "RAD_INSPECTION_INFO_VALID" 	                      

	// Which options to automate during /IMPORT	     
	#define szRAD_IMPORT_ALL			"RAD_IMPORT_ALL"				
	#define szRAD_IMPORT_TODAY			"RAD_IMPORT_TODAY"
	#define szRAD_INTEGRITY				"RAD_INTEGRITY"
	#define szRAD_FIND_EVENTS			"RAD_FIND_EVENTS"
	#define szRAD_DIRECTION				"RAD_DIRECTION"
	#define szRAD_GRAPH					"RAD_GRAPH"
	#define szRAD_MEASUREMENT			"RAD_MEASUREMENT"
	#define szRAD_EXPORT				"RAD_EXPORT"	

	// Which menu options to activate
	#define szRAD_MENU_IMPORT_TODAY				"RAD_MENU_IMPORT_TODAY"
	#define szRAD_MENU_IMPORT_SELECTED			"RAD_MENU_IMPORT_SELECTED"
	#define szRAD_MENU_DETERMINE_MEASUREMENTS	"RAD_MENU_DETERMINE_MEASUREMENTS"
	#define szRAD_MENU_DIRECTION_ANALYSIS		"RAD_MENU_DIRECTION_ANALYSIS"
	#define szRAD_MENU_EXPORT_TO_INCC			"RAD_MENU_EXPORT_TO_INCC"
	#define szRAD_MENU_MEASUREMENTS_SUMMARY		"RAD_MENU_MEASUREMENTS_SUMMARY"
	#define szRAD_MENU_DIRECTION_SUMMARY		"RAD_MENU_DIRECTION_SUMMARY"
	#define szRAD_MENU_INTEGRITY_CHECK			"RAD_MENU_INTEGRITY_CHECK"

	// When to write results files
	#define szRAD_WRITE_CHAN_EVENT_RESULTS_FILE		"RAD_WRITE_CHAN_EVENT_RESULTS_FILE"
	#define szRAD_WRITE_DIR_EVENT_RESULTS_FILE		"RAD_WRITE_DIR_EVENT_RESULTS_FILE"
	#define szRAD_WRITE_MEASUREMENT_RESULTS_FILE	"RAD_WRITE_MEASUREMENT_RESULTS_FILE"

	// Import default parameters
	#define szRAD_IMPORT_AUTOMATE_QUESTION	"RAD_IMPORT_AUTOMATE_QUESTION"
	#define szRAD_IMPORT_DISK_PROMPT		"RAD_IMPORT_DISK_PROMPT"
	#define szRAD_IMPORT_MULTIPLE_DISKS		"RAD_IMPORT_MULTIPLE_DISKS"

	#define szRAD_IMPORT_ALL_PATH		"RAD_IMPORT_ALL_PATH" 
	#define szRAD_IMPORT_ALL_WILDCARD	"RAD_IMPORT_ALL_WILDCARD"
	#define szRAD_IMPORT_ALL_OVERWRITE	"RAD_IMPORT_ALL_OVERWRITE"
	#define szRAD_IMPORT_ALL_INIT		"RAD_IMPORT_ALL_INIT"
	#define szRAD_IMPORT_ALL_SUBFOLDERS	"RAD_IMPORT_ALL_SUBFOLDERS"
	
	#define szRAD_IMPORT_TODAY_PATH		"RAD_IMPORT_TODAY_PATH"
	#define szRAD_IMPORT_TODAY_WILDCARD	"RAD_IMPORT_TODAY_WILDCARD"
	#define szRAD_IMPORT_TODAY_OVERWRITE "RAD_IMPORT_TODAY_OVERWRITE"
	#define szRAD_IMPORT_TODAY_INIT		"RAD_IMPORT_TODAY_INIT"
	#define szRAD_IMPORT_TODAY_SUBFOLDERS	"RAD_IMPORT_TODAY_SUBFOLDERS"
	#define szRAD_IMPORT_TODAY_SINCE_LAST	"RAD_IMPORT_TODAY_SINCE_LAST"
	
	#define szRAD_IMPORT_SELECT_PATH	"RAD_IMPORT_SELECT_PATH"
	#define szRAD_IMPORT_SELECT_WILDCARD "RAD_IMPORT_SELECT_WILDCARD"
	#define szRAD_IMPORT_SELECT_OVERWRITE "RAD_IMPORT_SELECT_OVERWRITE"
	#define szRAD_IMPORT_SELECT_INIT	"RAD_IMPORT_SELECT_INIT"
	#define szRAD_IMPORT_SELECT_SUBFOLDERS	"RAD_IMPORT_SELECT_SUBFOLDERS"
	
	// Database default parameters
	#define szRAD_DB_BACKUP_PATH		"RAD_DB_BACKUP_PATH" 
	#define szRAD_DB_RESTORE_FROM_PATH	"RAD_DB_RESTORE_FROM_PATH"
	#define szRAD_CFG_BACKUP_PATH		"RAD_CFG_BACKUP_PATH" 
	#define szRAD_CFG_RESTORE_FROM_PATH	"RAD_CFG_RESTORE_FROM_PATH"
	#define szRAD_LOG_COPY_TO_PATH		"RAD_LOG_COPY_TO_PATH"

	// Whether real time update is active
	#define szRAD_REAL_TIME_UPDATE		"RAD_REAL_TIME_UPDATE"

	// Whether reanalyze is enabled or not
	#define szRAD_REANALYZE_ENABLED		"RAD_REANALYZE_ENABLED"

	// Whether to match locations based on facility, station or channel
	// Whether to use names or numbers
	// Whether to display a message if no match
	#define szRAD_LOCATION_MATCH_TYPE	"RAD_LOCATION_MATCH_TYPE"
	#define szRAD_LOCATION_MATCH_ITEM	"RAD_LOCATION_MATCH_ITEM"
	#define szRAD_LOCATION_NO_MATCH_MSG	"RAD_LOCATION_NO_MATCH_MSG"
	#define szRAD_SEND_LOCATION			"RAD_SEND_LOCATION"

	// Analysis parameters specific to the SR data
	#define szRAD_MEAS_AT_THRES			"RAD_MEAS_AT_THRES"
	#define szRAD_MEAS_AT_BIAS			"RAD_MEAS_AT_BIAS"	
	#define szRAD_MEAS_AT_SIGMA			"RAD_MEAS_AT_SIGMA"
	#define szRAD_MEAS_GATE_WIDTH		"RAD_MEAS_GATE_WIDTH"
	#define szRAD_MEAS_TERM_ON_MOVING	"RAD_MEAS_TERM_ON_MOVING"
	#define szRAD_MEAS_INCLUDE_AT_FAIL	"RAD_MEAS_INCLUDE_AT_FAIL"

	// 11-Jul-2005 Added these new parameters to INI
	#define szRAD_R_T_UPPER_LIMIT		"RAD_R_T_UPPER_LIMIT"
	#define szRAD_ENABLE_RATIOS			"RAD_ENABLE_RATIOS"




#endif  



