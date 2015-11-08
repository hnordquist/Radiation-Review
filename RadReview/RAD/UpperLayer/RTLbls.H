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
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/11/97

// RT_Lbls.H
// The common labels within a Review Tool initialization file


#ifndef RT_LBLS_H

	#define RT_LBLS_H

	
	// REVIEW TOOL COMMON SETUP PARAMETERS
	#define szRT_NAME								"RT_NAME"
	#define szRT_START_MODE							"RT_START_MODE"
	#define szRT_SPLASH_SCREEN_TIMEOUT_SECONDS		"RT_SPLASH_SCREEN_TIMEOUT_SECONDS"

    
	#define szRT_USE_OTHER_TOOL_AS_COMMANDER		"RT_USE_OTHER_TOOL_AS_COMMANDER"
	#define szRT_OTHER_TOOL_COMMANDER_NUM			"RT_OTHER_TOOL_COMMANDER_NUM"

	#define szRT_OTHER_TOOLS_NUM					"RT_OTHER_TOOLS_NUM"
	#define szRT_OTHER_TOOLS_EXECS					"RT_OTHER_TOOLS_EXECS"
	#define szRT_OTHER_TOOLS_NAMES					"RT_OTHER_TOOLS_NAMES"
	#define szRT_OTHER_TOOLS_INSTALLED				"RT_OTHER_TOOLS_INSTALLED"


	// REVIEW TOOL COMMON COMMUNICATION PARAMETERS
	#define szRT_PIPE_MY_NAME						"RT_PIPE_MY_NAME"
	#define szRT_PIPE_OTHER_NAMES					"RT_PIPE_OTHER_NAMES"
	#define szRT_PIPE_OTHER_TIMEOUTS				"RT_PIPE_OTHER_TIMEOUTS"


	// REVIEW TOOL COMMON INSPECTION INFORMATION
	#define szRT_INSPEC_INFO_COMMON_FILENAME		"RT_INSPEC_INFO_COMMON_FILENAME"
	#define szRT_MIC_INSPEC_INFO_FILE_USE			"RT_MIC_INSPEC_INFO_FILE_USE"
	#define szRT_MIC_INSPEC_INFO_FILENAME			"RT_MIC_INSPEC_INFO_FILENAME"
	#define szRT_FACILITY_NAME						"RT_FACILITY_NAME"
	#define szRT_FACILITY_LOCATION					"RT_FACILITY_LOCATION"
	#define szRT_INSPECTION_NUMBER					"RT_INSPECTION_NUMBER"
	#define szRT_INSPECTION_DATE					"RT_INSPECTION_DATE"
	#define szRT_INSPECTION_TIME					"RT_INSPECTION_TIME"
	#define szRT_INSPECTION_DATA_DATE_START			"RT_INSPECTION_DATA_DATE_START"
	#define szRT_INSPECTION_DATA_TIME_START			"RT_INSPECTION_DATA_TIME_START"
	#define szRT_INSPECTION_DATA_DATE_END			"RT_INSPECTION_DATA_DATE_END"
	#define szRT_INSPECTION_DATA_TIME_END			"RT_INSPECTION_DATA_TIME_END"
	#define szRT_INSPECTORS_NUMBER					"RT_INSPECTORS_NUMBER"
	#define szRT_INSPECTORS_IDS						"RT_INSPECTORS_IDS"
	#define szRT_INSPECTORS_IDS_CHOICES_NUM			"RT_INSPECTORS_IDS_CHOICES_NUM"
	#define szRT_INSPECTORS_IDS_CHOICES				"RT_INSPECTORS_IDS_CHOICES"
	#define szRT_INSPECTION_COMMENTS				"RT_INSPECTION_COMMENTS"


	// REVIEW TOOL COMMON DATABASE PARAMETERS
	#define szRT_COMMON_DATABASE_PATH				"RT_COMMON_DATABASE_PATH"


	// REVIEW TOOL COMMON SUMMARY DATA PARAMETERS
	#define szRT_SUMMARY_DATA_CREATE_FILE			"RT_SUMMARY_DATA_CREATE_FILE"
	#define szRT_SUMMARY_DATA_FILENAME				"RT_SUMMARY_DATA_FILENAME"


	// REIVEW TOOL COMMON REVIEW PARAMETERS
	#define szRT_DATA_MATCH_ERROR_SECONDS			"RT_DATA_MATCH_ERROR_SECONDS"
	#define szRT_DATA_MATCH_MINIMIZE				"RT_DATA_MATCH_MINIMIZE"
	#define szRT_DATA_MATCH_RESTORE					"RT_DATA_MATCH_RESTORE"
	#define szRT_DATA_MATCH_OTHER_TOOLS				"RT_DATA_MATCH_OTHER_TOOLS"
	#define szRT_DATA_MATCH_DISPLAY_CONNECT_MESSAGE	"RT_DATA_MATCH_DISPLAY_CONNECT_MESSAGE"
	#define szRT_DATA_MATCH_NEVER_SEND_LOCATION		"RT_DATA_MATCH_NEVER_SEND_LOCATION"
	#define szRT_DATA_MATCH_ALWAYS_SEND_LOCATION	"RT_DATA_MATCH_ALWAYS_SEND_LOCATION"
	
		
#endif

