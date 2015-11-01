// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 9/20/96

// TXT_WrDB.H
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

