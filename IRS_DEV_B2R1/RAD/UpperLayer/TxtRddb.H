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

