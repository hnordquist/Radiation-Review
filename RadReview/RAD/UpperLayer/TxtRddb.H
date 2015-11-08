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
// 9/20/96

// TXT_RdDB.H
// Class definition for saving the TLR database


#ifndef TXT_RDDB_H

	#define TXT_RDDB_H

	#include <stdlib.h> 
	#include <fstream.h>
	#include <string.h>

	#include "windows.h"
	#include "gen.h"
	
                     
	class Text_Read_Database {

		public:
		
			// IR class functions
			Text_Read_Database(const char *pszDatabase_Name, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length); 
			virtual ~Text_Read_Database(void);
			
			BOOL Database_Opened(void);
			BOOL Database_Close(void);
			                        
			UINT Columns_Get_Number(void);
			UINT Rows_Get_Number(void);
			
			BOOL Column_Reset(UINT uiColumn_Number);
			
			// Expects column name to be already allocated
			BOOL Get_Column_Name(UINT uiColumn_Number, char *pszColumn_Name);
			const char *Get_Column_Name(UINT uiColumn_Number);
			const char **Get_Column_Names(void);
			BOOL Fix_Column_Name(char **ppszColumn_Name);

			// Option to remove leading and trailing spaces in data
			BOOL Row_Get_Next(UINT uiColumn_Number, char *pszData, BOOL bRemove_Lead_Trail_Spaces = FALSE);
			// Designed to be as fast as possible
			BOOL Row_Get_Next(char *pszData);
					
		private:

			typedef struct {
				char *pszColumn_Name;
			    DWORD dData_Position;
			    } COLUMN_DATA;

			typedef struct {
			    ifstream *pText_DB_File;
			    DWORD dRow_Position;
				struct COLUMN_DATA *pColumn_Info;
			    } READ_TEXT_DB_DATA;
			    
			char **ppszColumn_Names;
			    
			READ_TEXT_DB_DATA Columns;
			
		    BOOL bFile_Open;
		    
		    UINT uiNum_Cols, uiNum_Rows;
			UINT uiMax_Col_Length, uiMax_Data_Length;

			// Helper member functions
			BOOL Remove_Spaces_At_Begin(char *pszString);
			BOOL Remove_Spaces_At_End(char *pszString);
		    
		};
		
#endif

