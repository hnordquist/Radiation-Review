// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 9/23/96

// TBLEWRKR.H
// Class definition for Table Worker: Helps build and save GUI Tables


#ifndef TBLEWRKR_H

	#define TBLEWRKR_H

	#include <gui_tbl.h>
	#include <gui_lib.h> 
	#include <stdlib.h> 
	#include <stdio.h>
	#include <string.h>
	#include <io.h>
	
	#include <TXTRdDB.h>
	#include <TXTWrDB.h>
	#include "SEBBoxes.H"

	#include "gen.h"


	// Anyone who uses tables and Table_Worker should use these
	#define uiMAX_COLUMN_NAME_LENGTH	100
	#define uiMAX_TABLE_CELL_LENGTH		100

	

	class Table_Worker {

		public:

			// ALL ROW AND COLUMN INDEXS START AT 1!!!!!!!!!!!!!!!
			// Colors and Progress Bar default to OFF

			// Table worker creation member functions
		    // Note: Pointer to table must remain vaild while using the table worker
			Table_Worker(CGUI_Table *pTable_Worker_Table, UINT uiMax_Column_Name_Length = uiMAX_COLUMN_NAME_LENGTH, UINT uiMax_Table_Cell_Length = uiMAX_TABLE_CELL_LENGTH); 
			Table_Worker(void); 
			virtual ~Table_Worker(void); 

			virtual BOOL Table_Worker_Create(CGUI_Table *pTable_Worker_Table, UINT uiMax_Column_Name_Length = uiMAX_COLUMN_NAME_LENGTH, UINT uiMax_Table_Cell_Length = uiMAX_TABLE_CELL_LENGTH);
			virtual BOOL Table_Worker_Created(void); 


			// Table worker configuration setting and getting member functions
			virtual BOOL Clear(void); 
			virtual BOOL Display_New(void); 
			virtual BOOL Close(void); 
			// NOTE : If bRead_Colors is TRUE then any data prepended with a color name and a '-' gets it's cell turned to that color, similar principle for saving
			virtual BOOL Set_Read_Colors(BOOL bRead_Colors);
			virtual BOOL Set_Write_Enabled(UINT uiColumn_Num, BOOL bWrite_Enabled);
			// Will create a progress when Filling and Saving
			virtual BOOL Set_Progress_Bar(BOOL bEnabled);
			// NOTE: Can set the first three parameters to be used for automatically created progress bars
			// NOTE: Can set this progress bar to cover multiple Fill and Save calls
			virtual BOOL Set_Progress_Bar(const char *pszBar_Title, RESOURCE_ID Icon = NULL, unsigned long uiTotal_Calls = NULL, unsigned long uiUpdates_Per_Call = NULL, const char *pszMessage = NULL);
			virtual BOOL Close_Progress_Bar(void);

			UINT Get_Max_Table_Cell_Length(void);
			BOOL Get_Read_Number_Rows(const char *pszRead_Database_Name, UINT *puiRows_Number);
			BOOL Get_Read_Number_Columns(const char *pszRead_Database_Name, UINT *puiColumns_Number);
			UINT Get_Added_Number_Rows(void);
			BOOL Set_Added_Number_Rows(UINT uiTotal_Number_Rows); // Be sure to set if manually added
			UINT Get_Added_Number_Columns(void);
			BOOL Set_Added_Number_Columns(UINT uiTotal_Number_Cols); // Be sure to set if manually added
			const char **Get_Added_Column_Names(void);
			// Determines a range through incremental string comparisons
			BOOL Determine_Data_Range(const char *pszRead_Database_Name, UINT uiColumn_Number, BOOL *pbRange_Found, const char *pszSearch_Start, UINT *puiStart_Row, const char *pszSearch_End, UINT *puiEnd_Row, UINT uiStop_Row = NULL);
			

			// Table worker column getting member functions
			// Get a column from the read database, creates the list of strings for you
			// Specifying Total_Rows_Number spreads (adds NULLS) to extend to that length
			BOOL Get_DB_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, char ***pppszColumn_Data, char **ppszColumn_Name = NULL, UINT uiTotal_Rows_Number = NULL, BOOL bRemove_Colors = TRUE);
			BOOL Get_DB_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, char ***pppszColumn_Data, UINT uiGet_Start_Row, UINT uiGet_End_Row, char **ppszColumn_Name = NULL, UINT uiTotal_Rows_Number = NULL);
			// Creates a list of NULL column data
			BOOL Get_DB_NULL_Column(char ***pppszColumn_Data, UINT uiNum_Rows);
			// Get a column from the table, creates the list of strings for you
			BOOL Get_Table_Column(UINT uiColumn_Number, UINT uiRows_Number, char ***pppszColumn_Data);


			// Table worker table filling member functions
			// The following will update a progress bar once when called
			BOOL Fill_Table_With_Sequence(const char *pszColumn_Name, UINT uiStart_Number, UINT uiEnd_Number, BOOL bWrite_Enabled = FALSE);
			BOOL Fill_Table_With_Column(const char *pszColumn_Name, const char **ppszValues, UINT uiNum_Rows, BOOL bWrite_Enabled = FALSE);
			// The following have the ability to use a progress bar
			BOOL Fill_Table_With_NULL_Column(const char *pszColumn_Name, UINT uiNum_Rows, BOOL bWrite_Enabled = FALSE);
			BOOL Fill_Table_With_Database(const char *pszRead_Database_Name, BOOL bWrite_Enabled = FALSE);
			BOOL Fill_Table_With_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, const char *pszColumn_Name = NULL, BOOL bWrite_Enabled = FALSE);
			BOOL Fill_Table_With_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, UINT uiStart_Row, UINT uiEnd_Row, const char *pszColumn_Name = NULL, BOOL bWrite_Enabled = FALSE);
			BOOL Fill_Table_With_Columns(const char *pszRead_Database_Name, UINT uiStart_Column_Number, UINT uiEnd_Column_Number, BOOL bWrite_Enabled = FALSE);
			// Each Fill_Table_With_Columns uses the puiColumn_Numbers to index into (from 0) the ppszColumn_Names to select the correct column name
			BOOL Fill_Table_With_Columns(const char *pszRead_Database_Name, const UINT *puiColumn_Numbers, UINT uiNumber_Columns, const char **ppszColumn_Names = NULL, BOOL bWrite_Enabled = FALSE);
			BOOL Fill_Table_With_Columns(const char *pszRead_Database_Name, const UINT *puiColumn_Numbers, UINT uiNumber_Columns, UINT uiStart_Row, UINT uiEnd_Row, const char **ppszColumn_Names = NULL, BOOL bWrite_Enabled = FALSE);
			// Unconditional FAST table filling member functions
			BOOL Fill_Table_With_Columns_FAST(const char *pszRead_Database_Name, const UINT *puiColumn_Numbers, UINT uiNumber_Columns, UINT uiStart_Row, UINT uiEnd_Row, BOOL bWrite_Enabled = FALSE);


			// Usually just use to save entire table						
			BOOL Save_Table_To_File(const char *pszWrite_Database_Name);
            // Allows table to be merged into a saved table file 
            // Useful for when only a limited number columns are displayed from a table file and their changes need to be saved back in
            // NOTE: Table and file MUST be same dimensions
            // NOTE: Column positions are the positions of each column in the table to be inserted over the columns in the file
            //       The columns to be inserted over MUST exist in the file
            // NOTE: Can set Read and Write names to same file to perform normal save and can set them to different names to perform save as
			// NOTE: In Save_Table can pass the row where data will be inserted and delete to end of table
			// NOTE: Save_Table_Merge_With_File ACCEPTS a list with column indexes starting at 0!!!!
			BOOL Save_Table_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, const UINT *puiColumn_Positions, UINT uiStart_Insert_Row = 1);
			BOOL Save_Data_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiColumn_Position, const char **ppszColumn, UINT uiNum_Data_Rows);
			BOOL Save_Column_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiTable_Position, UINT uiDB_Position);
			// Saves column or data over existing portion in database (From Start_Row to End_Row)
			BOOL Save_Data_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiDB_Position, const char **ppszColumn, UINT uiNum_Data_Rows, UINT uiDelete_Start_Row, UINT uiDelete_End_Row, UINT uiData_Start_Row);
			BOOL Save_Column_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiTable_Position, UINT uiDB_Position, UINT uiDelete_Start_Row, UINT uiDelete_End_Row, UINT uiData_Start_Row);


			
		private:

			CGUI_Table *pTable;
			char **ppszColumns;
			UINT uiMax_Col_Length, uiMax_Data_Length;
			UINT uiRows_Added, uiColumns_Added;
			BOOL bTable_Worker_Created;
			BOOL bColors; 
			
			void Create_New_Table_Cells(UINT uiNum_Rows);
			BOOL Add_Column_Name(const char *pszColumn_Name);

			// Progress bar data and member functions
			BOOL bProgress_Bar_Enabled;
			char *pszProgress_Bar_Title;
			UINT uiProgress_Bar_Icon, uiProgress_Bar_Bitmap;
			SEB_Progress_Bar *pProgress_Bar;

		};
		
#endif






