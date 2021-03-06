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
#pragma once


#define MASK_QUIET_MODE			0x00010000

#define DISABLE_LOGGER		0x00020000
#define QUIET_MODE			0x00010000
#define QUIET_MODE_OFF		0
#define VERBOSITY1_AUTH		0x00000001
#define VERBOSITY2_FATAL	0x00000002
#define VERBOSITY3_NONFATAL	0x00000003
#define VERBOSITY4_WARNING	0x00000004
#define VERBOSITY5_INFO		0x00000005
#define VERBOSITY6_DIAG		0x00000006
#define VERBOSITY6_ALL		0x00000006

#define MAX_RECOMMENDED_DB_SIZE 838860800 //800MB
#define WITHIN_FIVE_PERCENT 1020054732.8//950 MB


#define AUTHENTICATION 1
#define FATAL 2
#define NON_FATAL 3
#define WARNING 4
#define INFORMATION 5
#define DIAGNOSTIC 6

// Max length of error text
#define MAX_ERROR 5012

// Errors
#define FACILITY_UNDEFINED 20
#define STATION_UNDEFINED 28
#define EXCEPTION_CAUGHT 12
#define DEBUG_MSG 13
#define STARTED 12000
#define FAIL_CREATE_VAR 12001
#define COM_FOUND_ERR 12002
#define FAIL_CREATE_SA 12003
#define COM_INTER_FAIL 12004
#define EXITED 12005
#define INVALID_PARAMETER 21
#define CHAN_ALREADY_DEFINED 12006
#define UNDEFINED_CHANNEL 22
#define NOT_IMPLEMENTED 12007
#define INVALID_REQUEST 12008
#define NO_ANALYSIS_DATA 12009
#define INVALID_POINTER 14
#define PARMS_NOT_SET 15
#define BAD_INSTRUMENT_TYPE 12010
#define COM_EXCEPTION 27
#define FILE_READ_ERR 3
#define FILE_COPY_ERR 6
#define DIR_DEL_ERR 29
#define DELETE_DB_ERR 28
#define CHAN_NOT_DEFINED 31
#define NO_DB 8

// HN - 8/8/2006 - modified the errors for exceptions to use common analysis ones
// for interface methods

#define EXCEPTION_CAUGHT_EXEC_ANAL 2005
#define EXCEPTION_CAUGHT_GET_ANAL_RESULTS 2007
#define EXCEPTION_CAUGHT_GET_LAST_ERR 2015
#define EXCEPTION_CAUGHT_GET_PARMS 2016
#define EXCEPTION_CAUGHT_SET_PARMS 2021

#define EXCEPTION_BACKUP_DATASTORE 1001
#define EXCEPTION_CLOSE_DATASTORE 1002
#define EXCEPTION_CREATE_DATASTORE 1003
#define EXCEPTION_GET_DATA_STATION 1004
#define EXCEPTION_GET_DATA_CHANNEL 1005
#define EXCEPTION_DELETE_DATA 1006
#define EXCEPTION_GET_FILES_LIST 1007
#define EXCEPTION_GET_DATA_STATISTICS 1008
#define EXCEPTION_GET_DATASTORE_PATH 1009
#define EXCEPTION_GET_DATASTORE_SIZE 1010
#define EXCEPTION_GET_DATASTORE_STATUS 1011
#define EXCEPTION_GET_DATASTORE_TIMERANGE 1012
#define EXCEPTION_GET_DAY_STATISTICS 1013
#define EXCEPTION_GET_DAY_SUMMARY_DATA 1014
#define EXCEPTION_GET_LAST_ERROR 1015
#define EXCEPTION_GET_PARAMETERS 1016
#define EXCEPTION_INIT_DATASTORE 1017
#define EXCEPTION_OPEN_DATASTORE 1018
#define EXCEPTION_RESTORE_DATASTORE 1019
#define EXCEPTION_ADD_DATA 1020
#define EXCEPTION_SET_PARAMETERS 1021

// GetLasError severities
#define FATAL_ERROR (0x80000000)
#define NON_FATAL_ERROR (0x40000000)
#define WARNING_ERROR (0x20000000)
#define NON_ERROR (0x0000000)

// INI files
#define MY_INI "CoBackgroundAnalysis.ini"
#define ERROR_INI "CommonErrorMessages.ini"

#define MY_GUID "B29F34D6-F347-4ca8-8014-0E23BE9744AE"
#define DEFAULT_VERB 3
#define MAX_INI 5012

#define SET_PARMS_NOT_CALLED 15
#define STARTUP 23000

#define EOSS_DB_NAME "EOSSImport.mdb"
