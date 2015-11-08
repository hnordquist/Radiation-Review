// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 9/20/96

// TXT_WrDB.H
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
// Class definition for saving the TLR database


#ifndef TXT_WRDB_H

	#define TXT_WRDB_H


	#include <stdlib.h>
	#include <stdio.h> 
	#include <fstream.h>
	#include <string.h>

	#include "windows.h"
	#include "gen.h"
	
                     
	class Text_Write_Database {

		public:
		    
		    // Using the following two constructors acually creates the file immediatily
			// Creates with an array of column names - file created
			Text_Write_Database(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char **ppszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length); 
			// Creates with a comma separated list of column names - file created
			Text_Write_Database(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char *pszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length); 
			Text_Write_Database(void);
			virtual ~Text_Write_Database(void);
			                        
			// Creates with an array of column names - file created
			BOOL Database_Create(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char **ppszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length);			                        
			// Creates with a comma separated list of column names - file created
			BOOL Database_Create(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char *pszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length);			                        
			BOOL Database_Created(void); 
			BOOL Database_Close(void);
			
			// Add a row as a string, defaults to tabs
			BOOL Row_Add(const char *pszRow, BOOL bComma_Separated = FALSE);
			// Add a row where each column data is in an array
			BOOL Row_Add(char **ppszRow);

			UINT Columns_Get_Number(void);
			UINT Rows_Get_Number(void);

			// Allow user to set the number of rows are the column had been set
			// Caution
			UINT Rows_Set_Number(const char * NumRowsStr);	 
					
		private:

		    ofstream *pText_File;
		    BOOL bFile_Open;
		    
		    UINT uiNum_Cols, uiNum_Rows, muiNumRowsPosition;	// sfk 05/15/2003 added nuiNumRowsPosition
			UINT uiMax_Col_Length, uiMax_Data_Lngth;
		    
		};

#endif

