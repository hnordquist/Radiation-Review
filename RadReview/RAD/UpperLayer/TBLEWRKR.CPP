// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 9/23/96

// TbleWrkr.CPP
// Member functions for Table Worker operations


#include "TbleWrkr.H"   



/////////////////////////////////////////////////////////////////////////////////////
//  Table Worker constructor:  Setup all intial parameters
/////////////////////////////////////////////////////////////////////////////////////
     
Table_Worker::Table_Worker(CGUI_Table *pTable_Worker_Table, UINT uiMax_Column_Name_Length, UINT uiMax_Table_Cell_Length) :
	pTable(pTable_Worker_Table),
	ppszColumns(NULL),
	uiMax_Col_Length(uiMax_Column_Name_Length),
	uiMax_Data_Length(uiMax_Table_Cell_Length),
	bTable_Worker_Created(TRUE),
	uiRows_Added(0),
	uiColumns_Added(0),
	bColors(FALSE),
	bProgress_Bar_Enabled(FALSE),
	pszProgress_Bar_Title(NULL),
	uiProgress_Bar_Icon(NULL),
	pProgress_Bar(NULL)
	{

}


Table_Worker::Table_Worker(void) :
	pTable(NULL),
	ppszColumns(NULL),
	uiMax_Col_Length(0),
	uiMax_Data_Length(0),
	bTable_Worker_Created(FALSE),
	uiRows_Added(0),
	uiColumns_Added(0),
	bColors(FALSE),
	bProgress_Bar_Enabled(FALSE),
	pszProgress_Bar_Title(NULL),
	uiProgress_Bar_Icon(NULL),
	pProgress_Bar(NULL)
	{

}


Table_Worker::~Table_Worker(void) {

	UINT ctIndex;
	

	if (ppszColumns) {
		for (ctIndex = 0; ctIndex < uiColumns_Added; ctIndex++)
			if (ppszColumns[ctIndex])
				delete [] ppszColumns[ctIndex];
		delete [] ppszColumns;
		ppszColumns = NULL;
		}
	if (pszProgress_Bar_Title) {
		delete [] pszProgress_Bar_Title;
		pszProgress_Bar_Title = NULL;
		}
	if (pProgress_Bar) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}

}


/////////////////////////////////////////////////////////////////////////////////////
//  Table Worker creation member functions
/////////////////////////////////////////////////////////////////////////////////////


BOOL Table_Worker::Table_Worker_Create(CGUI_Table *pTable_Worker_Table, UINT uiMax_Column_Name_Length, UINT uiMax_Table_Cell_Length) {

	pTable = pTable_Worker_Table;
	uiMax_Col_Length = uiMax_Column_Name_Length;
	uiMax_Data_Length = uiMax_Table_Cell_Length;
	UINT ctIndex;

	if (ppszColumns) {
		for (ctIndex = 0; ctIndex < uiColumns_Added; ctIndex++)
			delete [] ppszColumns[ctIndex];
		delete [] ppszColumns;
		ppszColumns = NULL;
		}
	uiRows_Added = 0;
	uiColumns_Added = 0;
	bTable_Worker_Created = TRUE;

	return (bTable_Worker_Created);
	
}


BOOL Table_Worker::Table_Worker_Created(void) {

	return (bTable_Worker_Created);

}


UINT Table_Worker::Get_Max_Table_Cell_Length(void) {

	return uiMax_Data_Length;

}

	
BOOL Table_Worker::Clear(void) {
 
	if ((!bTable_Worker_Created) ||
		(!Table_Worker_Create(pTable, uiMax_Col_Length, uiMax_Data_Length)))
		return FALSE;

	pTable->Clear();
	
	
	return TRUE;	             
	             
}


BOOL Table_Worker::Display_New(void) {
 
	if (!bTable_Worker_Created)
		return FALSE;

	pTable->SuppressRepaint(TRUE);
	FlushEvents();
	pTable->DisplayNew();
	FlushEvents();
	pTable->SuppressRepaint(FALSE);
	FlushEvents();

	
	return TRUE;	             
	             
}


BOOL Table_Worker::Close(void) {
 
	if ((!bTable_Worker_Created) ||
		(!Clear()))
		return FALSE;
	
	
	return TRUE;	             
	             
}


BOOL Table_Worker::Set_Read_Colors(BOOL bRead_Colors) {

	bColors = bRead_Colors;


	return TRUE;

}


BOOL Table_Worker::Set_Write_Enabled(UINT uiColumn_Num, BOOL bWrite_Enabled) {

	if (bTable_Worker_Created)
		return (pTable->SetColWriteEnable(uiColumn_Num - 1, bWrite_Enabled));
	else
		return FALSE;
		
}


BOOL Table_Worker::Set_Progress_Bar(BOOL bEnabled) {

	bProgress_Bar_Enabled = bEnabled;


	return TRUE;
	
}


BOOL Table_Worker::Set_Progress_Bar(const char *pszBar_Title, RESOURCE_ID Icon, unsigned long uiTotal_Calls, unsigned long uiUpdates_Per_Call, const char *pszMessage) {

	bProgress_Bar_Enabled = TRUE;

	if (pszProgress_Bar_Title) {
		delete [] pszProgress_Bar_Title;
		pszProgress_Bar_Title = NULL;
		}
	if (pProgress_Bar) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}

	uiProgress_Bar_Icon = Icon;
	if (pszBar_Title) {
		if ((pszProgress_Bar_Title = new char [strlen(pszBar_Title) + 1]) != NULL)
			strcpy(pszProgress_Bar_Title, pszBar_Title);
		else return FALSE;
		}


	if (uiTotal_Calls != 0) {
		if ((pProgress_Bar = new SEB_Progress_Bar((unsigned long)uiTotal_Calls, (unsigned long)uiUpdates_Per_Call, pszMessage, pszProgress_Bar_Title, Icon)) == NULL)
			return FALSE;
		}
	
	
	return TRUE;

}


BOOL Table_Worker::Close_Progress_Bar(void) {

	if (pProgress_Bar) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		return TRUE;
		}
	else return FALSE;

}


BOOL Table_Worker::Get_Read_Number_Rows(const char *pszRead_Database_Name, UINT *puiRows_Number) {

	Text_Read_Database *pRead_Database = NULL;
	BOOL bSuccess = FALSE;


	if ((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL)
		if (pRead_Database->Database_Opened()) {
			*puiRows_Number = pRead_Database->Rows_Get_Number();
			bSuccess = TRUE;
			}

	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}

	
	return bSuccess;
	
}


BOOL Table_Worker::Get_Read_Number_Columns(const char *pszRead_Database_Name, UINT *puiColumns_Number) {

	Text_Read_Database *pRead_Database = NULL;
	BOOL bSuccess = FALSE;


	if ((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL)
		if (pRead_Database->Database_Opened()) {
			*puiColumns_Number = pRead_Database->Columns_Get_Number();
			bSuccess = TRUE;
			}

	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}

	
	return bSuccess;
	
}


UINT Table_Worker::Get_Added_Number_Rows(void) {

	return uiRows_Added;
	
}


BOOL Table_Worker::Set_Added_Number_Rows(UINT uiTotal_Number_Rows) {

	uiRows_Added = uiTotal_Number_Rows;
	

	return TRUE;
	
}


UINT Table_Worker::Get_Added_Number_Columns(void) {

	return uiColumns_Added;
	
}


BOOL Table_Worker::Set_Added_Number_Columns(UINT uiTotal_Number_Cols) {

	uiColumns_Added = uiTotal_Number_Cols;


	return TRUE;

}


const char **Table_Worker::Get_Added_Column_Names(void) {
 
	return ((const char **)ppszColumns);

}


BOOL Table_Worker::Determine_Data_Range(const char *pszRead_Database_Name, UINT uiColumn_Number, BOOL *pbRange_Found, const char *pszSearch_Start, UINT *puiStart_Row, const char *pszSearch_End, UINT *puiEnd_Row, UINT uiStop_Row) {

	UINT uiNum_Rows, ctIndex;
	char **ppszData = NULL;
	BOOL bStart_Found = FALSE, bEnd_Found = FALSE, bSuccess = TRUE;
	UINT uiLast_End_Found;
	

	(*pbRange_Found) = FALSE;
    (*puiStart_Row) = (*puiEnd_Row) = 0;

    
	// Get the column of data
	if (uiStop_Row != NULL)
		uiNum_Rows = uiStop_Row;
	else if (!Get_Read_Number_Rows(pszRead_Database_Name, &uiNum_Rows))
		return FALSE;
	if (!Get_DB_Column(pszRead_Database_Name, uiColumn_Number, &ppszData, 1, uiNum_Rows))
		return FALSE;

	
	// Find the start row
    bStart_Found = FALSE; bEnd_Found = FALSE;
	while ((!bStart_Found) && ((*puiStart_Row) < uiNum_Rows)) {
		if ((strncmp(pszSearch_Start, ppszData[(*puiStart_Row)], strlen(pszSearch_Start)) <= 0) &&
			(ppszData[(*puiStart_Row)][0] != NULL))
			bStart_Found = TRUE;
		else {
			(*puiStart_Row)++;
			}		
		}
	// Actual row number is one more (index from 1)
	if (bStart_Found) (*puiStart_Row)++;

	// Find the end row
	while ((!bEnd_Found) && ((*puiEnd_Row) < uiNum_Rows)) {
		if (ppszData[(*puiEnd_Row)][0] != NULL) 
			uiLast_End_Found = (*puiEnd_Row);
		if ((strncmp(pszSearch_End, ppszData[(*puiEnd_Row)], strlen(pszSearch_End)) < 0) &&
			(ppszData[(*puiEnd_Row)][0] != NULL))
			bEnd_Found = TRUE;
		else 
			(*puiEnd_Row)++;
		}
	// If actually found, actual row number is one more (index from 1)
	if (bEnd_Found) { } // Do not increase by one because next one found proves end 
	// if reached the end of the data then was actually found
	else if ((*puiEnd_Row) == uiNum_Rows) {
		bEnd_Found = TRUE;
		if (uiLast_End_Found != (*puiEnd_Row) - 1)
			(*puiEnd_Row) = uiLast_End_Found + 1;
		}
	// Don't count the NULLs and make it found
	else if (uiLast_End_Found < (*puiEnd_Row)) {
		(*puiEnd_Row) = uiLast_End_Found + 1;
		bEnd_Found = TRUE;
		}
	SEB_DELETELISTSTRING(ppszData, uiNum_Rows, ctIndex);

	// Check that both ranges not before data	
	if ((!bStart_Found) ||
		(!bEnd_Found) ||
		((*puiEnd_Row) < (*puiStart_Row))) {
	    (*puiStart_Row) = (*puiEnd_Row) = 0;
		}
	else {
		(*pbRange_Found) = TRUE;
		}


	return TRUE;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Column getting member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Table_Worker::Get_DB_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, char ***pppszColumn_Data, char **ppszColumn_Name, UINT uiTotal_Rows_Number, BOOL bRemove_Colors) {

	UINT ctIndex = 0, uiRow_Num = 0, uiRows_Number;
	BOOL bBuild_Column_List = FALSE;
	char *pszTemp_Row = NULL, *pszTemp_Row_Start;
	Text_Read_Database *pRead_Database = NULL;
	
    
	(*pppszColumn_Data) = NULL;
	if (ppszColumn_Name)
		(*ppszColumn_Name) = NULL;
		
	// Create all of the structures, open the file, and verify that everything is ready
	if ((bTable_Worker_Created) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((uiRows_Number = pRead_Database->Rows_Get_Number()) != 0) &&
		((pszTemp_Row = new char [uiMax_Data_Length + 1]) != NULL) &&
		(pRead_Database->Column_Reset(uiColumn_Number)) &&
    	(((*pppszColumn_Data) = new (char (* ([uiRows_Number])))) != NULL)) {
	    	
		// Read in the data one column at a time
		bBuild_Column_List = TRUE; 
		while ((bBuild_Column_List) && (uiRow_Num < uiRows_Number)) {
		   	if (pProgress_Bar)
		   		bBuild_Column_List = pProgress_Bar->Progress_Bar_Update();
		   	if (!pRead_Database->Row_Get_Next(uiColumn_Number, pszTemp_Row))
		   		bBuild_Column_List = FALSE;
		   	else {
				pszTemp_Row_Start = pszTemp_Row;
				if (bRemove_Colors) {
					// Want to remove colors, find the character after the color
    				if (strncmp(pszTemp_Row, "RED-", strlen("RED-")) == 0)
    					pszTemp_Row_Start = &(pszTemp_Row[strlen("RED-")]);
    				else if (strncmp(pszTemp_Row, "YELLOW-", strlen("YELLOW-")) == 0)
    					pszTemp_Row_Start = &(pszTemp_Row[strlen("YELLOW-")]);
    				else if (strncmp(pszTemp_Row, "GREEN-", strlen("GREEN-")) == 0)
    					pszTemp_Row_Start = &(pszTemp_Row[strlen("GREEN-")]);
    				else if (strncmp(pszTemp_Row, "GRAY-", strlen("GRAY-")) == 0)
    					pszTemp_Row_Start = &(pszTemp_Row[strlen("GRAY-")]);
					}
				// Create the memory to hold the new data
				if (((*pppszColumn_Data)[uiRow_Num] = new char [strlen(pszTemp_Row_Start) + 1]) == NULL)
				   	bBuild_Column_List = FALSE;
				else {
				   	strcpy((*pppszColumn_Data)[uiRow_Num], pszTemp_Row_Start);
					uiRow_Num++;
					}
				}
			}

		// Get the column name next if required
		if (ppszColumn_Name) {
			if (((*ppszColumn_Name) = new char [strlen(pRead_Database->Get_Column_Name(uiColumn_Number)) + 1]) == NULL)
	   			bBuild_Column_List = FALSE;
	   		else 
	   			strcpy((*ppszColumn_Name), pRead_Database->Get_Column_Name(uiColumn_Number));
	   		}

		// Add NULLs at the end if uiTotal_Rows_Number exists and more data to get
		if (uiTotal_Rows_Number) {
			while ((bBuild_Column_List) && (uiRow_Num < uiTotal_Rows_Number)) {
		   		if (pProgress_Bar)
		   			bBuild_Column_List = pProgress_Bar->Progress_Bar_Update();
		   		if (((*pppszColumn_Data)[uiRow_Num] = new char [1]) == NULL)
		   			bBuild_Column_List = FALSE;
		   		else {
		   			((*pppszColumn_Data)[uiRow_Num][0]) = NULL;
					uiRow_Num++;
					}
				}
			}
		}         

	// If failed then delete everything
	if (!bBuild_Column_List) {
		SEB_DELETELISTSTRING((*pppszColumn_Data), uiRow_Num, ctIndex);
		if (ppszColumn_Name) {
			delete [] ppszColumn_Name;
			ppszColumn_Name = NULL;
			}
		}

	// Delete the temporary items
	if (pszTemp_Row) {
		delete [] pszTemp_Row;
		pszTemp_Row = NULL;
		}
	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}
	
	
	return bBuild_Column_List;

}


BOOL Table_Worker::Get_DB_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, char ***pppszColumn_Data, UINT uiStart_Row, UINT uiEnd_Row, char **ppszColumn_Name, UINT uiTotal_Rows_Number) {

	UINT ctIndex = 0, uiRow_Num = 0, uiRows_Number;
	BOOL bBuild_Column_List = FALSE;
	char *pszTemp_Row = NULL;
	Text_Read_Database *pRead_Database = NULL;
	
    
	(*pppszColumn_Data) = NULL;
	if (ppszColumn_Name)
		(*ppszColumn_Name) = NULL;
   	uiRows_Number = uiEnd_Row - uiStart_Row + 1;
		
	// Create all of the structures, open the file, and verify that everything is ready
	if ((bTable_Worker_Created) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((pszTemp_Row = new char [uiMax_Data_Length+1]) != NULL) &&
		(pRead_Database->Column_Reset(uiColumn_Number)) &&
    	(((*pppszColumn_Data) = new (char (* ([max(uiRows_Number, uiTotal_Rows_Number)])))) != NULL)) {
	    	
		// Read in the data one column at a time
		bBuild_Column_List = TRUE; 

		// Be sure that starting row is reached
		while ((bBuild_Column_List) && (ctIndex < uiStart_Row - 1)) {
			bBuild_Column_List = pRead_Database->Row_Get_Next(uiColumn_Number, pszTemp_Row);
		   	ctIndex++;
		   	}
		
	    uiRow_Num = 0;
		while ((bBuild_Column_List) && (uiRow_Num < uiRows_Number)) {
		   	if (pProgress_Bar)
		   		bBuild_Column_List = pProgress_Bar->Progress_Bar_Update();
		   	if ((!pRead_Database->Row_Get_Next(uiColumn_Number, pszTemp_Row)) ||
		   		(((*pppszColumn_Data)[uiRow_Num] = new char [strlen(pszTemp_Row) + 1]) == NULL))
		   		bBuild_Column_List = FALSE;
		   	else {
		   		strcpy((*pppszColumn_Data)[uiRow_Num], pszTemp_Row);
				uiRow_Num++;
				}
			}

		// Get the column name next if required
		if (ppszColumn_Name) {
			if (((*ppszColumn_Name) = new char [strlen(pRead_Database->Get_Column_Name(uiColumn_Number)) + 1]) == NULL)
	   			bBuild_Column_List = FALSE;
	   		else 
	   			strcpy((*ppszColumn_Name), pRead_Database->Get_Column_Name(uiColumn_Number));
	   		}

		// Add NULLs at the end if uiTotal_Rows_Number exists and more data to get
		if (uiTotal_Rows_Number) {
			while ((bBuild_Column_List) && (uiRow_Num < uiTotal_Rows_Number)) {
		   		if (pProgress_Bar)
		   			bBuild_Column_List = pProgress_Bar->Progress_Bar_Update();
		   		if (((*pppszColumn_Data)[uiRow_Num] = new char [1]) == NULL)
		   			bBuild_Column_List = FALSE;
		   		else {
		   			((*pppszColumn_Data)[uiRow_Num][0]) = NULL;
					uiRow_Num++;
					}
				}
			}
		}         

	// If failed then delete everything
	if (!bBuild_Column_List) {
		SEB_DELETELISTSTRING((*pppszColumn_Data), uiRow_Num, ctIndex);
		if (ppszColumn_Name) {
			delete [] ppszColumn_Name;
			ppszColumn_Name = NULL;
			}
		}

	// Delete the temporary items
	if (pszTemp_Row) {
		delete [] pszTemp_Row;
		pszTemp_Row = NULL;
		}
	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}
	
	
	return bBuild_Column_List;

}


BOOL Table_Worker::Get_Table_Column(UINT uiColumn_Number, UINT uiRows_Number, char ***pppszColumn_Data) {

	UINT uiRow_Num = 0, ctIndex = 0;
	BOOL bSuccess = TRUE;           
	CGStr Row;
	
    
    if ((uiColumn_Number < 1) || (uiColumn_Number > Get_Added_Number_Columns()))
    	return FALSE;
    
	if (((*pppszColumn_Data) = new (char (* ([uiRows_Number])))) == NULL)
		return FALSE;

	for (uiRow_Num = 0; uiRow_Num < uiRows_Number; uiRow_Num++) {
		pTable->TxtGetValue(uiRow_Num, uiColumn_Number - 1, &Row);
		if (((*pppszColumn_Data)[uiRow_Num] = new char [strlen(Row.Get_sz()) + 1]) != NULL)
			strcpy((*pppszColumn_Data)[uiRow_Num], Row.Get_sz());
		else
			bSuccess = FALSE;
		}
		
	if (!bSuccess) {
		SEB_DELETELISTSTRING((*pppszColumn_Data), uiRow_Num, ctIndex);
		}


	return bSuccess;
	
}


BOOL Table_Worker::Get_DB_NULL_Column(char ***pppszColumn_Data, UINT uiNum_Rows) {

	UINT uiRow_Num = 0, ctIndex = 0;
	BOOL bSuccess = TRUE;           
	
    
	if (((*pppszColumn_Data) = new (char (* ([uiNum_Rows])))) == NULL)
		return FALSE;

	uiRow_Num = 0;
	while ((bSuccess) && (uiRow_Num < uiNum_Rows)) {
		if (pProgress_Bar)
		   	bSuccess = pProgress_Bar->Progress_Bar_Update();
		if (((*pppszColumn_Data)[uiRow_Num] = new char [1]) != NULL)
			(*pppszColumn_Data)[uiRow_Num][0] = NULL;
		else
			bSuccess = FALSE;
		uiRow_Num++;
		}
		
	if (!bSuccess) {
		SEB_DELETELISTSTRING((*pppszColumn_Data), uiRow_Num, ctIndex);
		}


	return bSuccess;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Table Worker filling member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Table_Worker::Fill_Table_With_Sequence(const char *pszColumn_Name, UINT uiStart_Number, UINT uiEnd_Number, BOOL bWrite_Enabled) {
                 
	BOOL bFill_Table_Success = TRUE;
	UINT ctIndex;
	char **ppszTable_Header;


	// Will call once if progress bar set
	if (pProgress_Bar)
		pProgress_Bar->Progress_Bar_Update();
			
	if ((bTable_Worker_Created) && (uiStart_Number <= uiEnd_Number)) {	
		if ((ppszTable_Header = (char **) new (char (*[uiEnd_Number - uiStart_Number + 1]))) != NULL) {
			// Add the sequence column
		   	for (ctIndex = 0; ctIndex < (uiEnd_Number - uiStart_Number + 1); ctIndex++) {
		   		if ((ppszTable_Header[ctIndex] =  new char [7]) == NULL)
		   			bFill_Table_Success = FALSE;
		   		else 
		   			sprintf(ppszTable_Header[ctIndex], "%04d", uiStart_Number + ctIndex);
				}
			if (bFill_Table_Success) {				
				if ((pTable->AddColTxt(pszColumn_Name, (uiEnd_Number - uiStart_Number + 1), ppszTable_Header, uiMax_Data_Length)) >= 0) {
					// Determine total number rows and add column name to our list
					uiRows_Added = max(uiRows_Added, (uiEnd_Number - uiStart_Number + 1)); 
					Add_Column_Name(pszColumn_Name);
					if (!bWrite_Enabled)
						pTable->SetColWriteEnable(uiColumns_Added - 1, bWrite_Enabled);
					}
				else 
		   			bFill_Table_Success = FALSE;
				}
			SEB_DELETELISTSTRING(ppszTable_Header, (uiEnd_Number - uiStart_Number + 1), ctIndex);
			}
		}

		
	return bFill_Table_Success; 
 
}


BOOL Table_Worker::Fill_Table_With_Column(const char *pszColumn_Name, const char **ppszValues, UINT uiNum_Rows, BOOL bWrite_Enabled) {

    BOOL bSuccess = FALSE;
	

	// Will call once if progress bar set
	if (pProgress_Bar)
		pProgress_Bar->Progress_Bar_Update();
			
	if ((bTable_Worker_Created) && (uiNum_Rows >= 0) && (pszColumn_Name) && (ppszValues)) {
		// Add the column
		if ((pTable->AddColTxt(pszColumn_Name, uiNum_Rows, (char ** const) ppszValues, uiMax_Data_Length)) >= 0) {
			Add_Column_Name((char * const)pszColumn_Name);
			// Define all the empty cells
			Create_New_Table_Cells(uiNum_Rows);
			// Determine total number rows and add column name to our list
			uiRows_Added = max(uiRows_Added, uiNum_Rows); 
			if (!bWrite_Enabled)
				pTable->SetColWriteEnable(uiColumns_Added - 1, bWrite_Enabled);
			bSuccess = TRUE;
			}
		}


	return bSuccess;

}


BOOL Table_Worker::Fill_Table_With_NULL_Column(const char *pszColumn_Name, UINT uiNum_Rows, BOOL bWrite_Enabled) {

	UINT ctRow_Number;
	BOOL bBuild_Column_List = TRUE;
	BOOL bSuccess = FALSE;
    
    char **ppszTemp_Table = NULL;
    BOOL bCancel = FALSE;
	
    
	if ((bTable_Worker_Created)  && (pszColumn_Name != NULL) && (uiNum_Rows > 0)) {
	    	
	   	if ((ppszTemp_Table = new (char (*[uiNum_Rows]))) != NULL) {
	   		for (ctRow_Number = 0; ctRow_Number < uiNum_Rows; ctRow_Number++) {
	   			if ((ppszTemp_Table[ctRow_Number] = new char [1]) == NULL)
	   				bBuild_Column_List = FALSE;
	   			else ppszTemp_Table[ctRow_Number][0] = NULL;
		   		if (pProgress_Bar)
		   			bCancel = !pProgress_Bar->Progress_Bar_Update();
	   			}
	   		}
	   	else
			bBuild_Column_List = FALSE;
	   		
	   	
	    // Read in the data one column at a time 
	    if (bBuild_Column_List) {
			if ((pTable->AddColTxt(pszColumn_Name, uiNum_Rows, ppszTemp_Table, uiMax_Col_Length)) >= 0) {
				Add_Column_Name((char * const)pszColumn_Name);
				// Define all the empty cells
				Create_New_Table_Cells(uiNum_Rows);
				// Determine total number rows and add column name to our list
				uiRows_Added = max(uiRows_Added, uiNum_Rows); 
				if (!bWrite_Enabled)
					pTable->SetColWriteEnable(uiColumns_Added - 1, bWrite_Enabled);
				bSuccess = TRUE;
				}

			// Delete the temporary list
			SEB_DELETELISTSTRING(ppszTemp_Table, uiNum_Rows, ctRow_Number);
			}
		else {
			bSuccess = FALSE;
			}
		
		}         


	return (bSuccess && !bCancel);

}


BOOL Table_Worker::Fill_Table_With_Database(const char *pszRead_Database_Name, BOOL bWrite_Enabled) {

	UINT uiRows_Number, uiColumns_Number;
	Text_Read_Database *pRead_Database = NULL;
	BOOL bSuccess = FALSE;
	
    
	if ((bTable_Worker_Created) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((uiRows_Number = pRead_Database->Rows_Get_Number()) != 0) &&
		((uiColumns_Number = pRead_Database->Columns_Get_Number()) != 0)) {
		// Close the database
		delete pRead_Database;
		pRead_Database = NULL;
	    bSuccess = Fill_Table_With_Columns(pszRead_Database_Name, 1, uiColumns_Number, bWrite_Enabled);
		}

	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}


	return FALSE;

}


BOOL Table_Worker::Fill_Table_With_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, const char *pszColumn_Name, BOOL bWrite_Enabled) {

	UINT uiNum_Rows;
	Text_Read_Database *pRead_Database = NULL;
	BOOL bSuccess = FALSE;
	
    
	if ((bTable_Worker_Created) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((uiNum_Rows = pRead_Database->Rows_Get_Number()) != 0) &&
		(uiColumn_Number <= pRead_Database->Columns_Get_Number()) &&
		(uiColumn_Number >= 0)) {
		// Close the database
		delete pRead_Database;
		pRead_Database = NULL;
	    bSuccess = Fill_Table_With_Column(pszRead_Database_Name, uiColumn_Number, 1, uiNum_Rows, pszColumn_Name, bWrite_Enabled);
		}

	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}


	return bSuccess;

}
    

BOOL Table_Worker::Fill_Table_With_Column(const char *pszRead_Database_Name, UINT uiColumn_Number, UINT uiStart_Row, UINT uiEnd_Row, const char *pszColumn_Name, BOOL bWrite_Enabled) {

	UINT ctIndex, ctRow_Number, uiNum_Rows;
	BOOL bBuild_Column_List = TRUE;
	BOOL bSuccess = FALSE;
    BOOL bCancel = FALSE;
	BOOL bProgress_Bar_Created = FALSE;
    
	char *pszTemp_Row = NULL, *pszTemp_Column_Name = NULL;
    char **ppszTemp_Table= NULL;
    COLORREF *pdTemp_Colors = NULL;
	Text_Read_Database *pRead_Database = NULL;
	
	
	if ((bTable_Worker_Created) &&
		((pszTemp_Column_Name = (char *) new char [uiMax_Col_Length+1]) != NULL) &&
		((pszTemp_Row = (char *) new char [uiMax_Data_Length+1]) != NULL) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		(uiColumn_Number <= pRead_Database->Columns_Get_Number()) &&
		(uiColumn_Number >= 0)) {
	
	    if (uiEnd_Row < uiStart_Row)
	    	bBuild_Column_List = FALSE;
		else 
    		uiNum_Rows = uiEnd_Row - uiStart_Row + 1;

	    // Create the temporary table	
	   	if (((ppszTemp_Table = new (char (*[uiNum_Rows]))) != NULL) &&
	   		((pdTemp_Colors = new COLORREF [uiNum_Rows]) != NULL)) {
	   		for (ctRow_Number = 0; ctRow_Number < uiNum_Rows; ctRow_Number++) {
	   			if ((ppszTemp_Table[ctRow_Number] = new char [uiMax_Data_Length+1]) == NULL)
	   				bBuild_Column_List = FALSE;
	   			}
	   		}
	   	else
			bBuild_Column_List = FALSE;

		// Create the progress bar if required
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			if ((pProgress_Bar = new SEB_Progress_Bar(uiNum_Rows, 1, "Loading Column...", pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bBuild_Column_List = FALSE;
			else bProgress_Bar_Created = TRUE;
			}

	   	
	    // Read in the data one column at a time 
	    if (bBuild_Column_List) {
		   	ctIndex = 0;
	   		// Be sure that starting row is reached
			while ((!bCancel) && (ctIndex < uiStart_Row - 1)) {
				bCancel = !pRead_Database->Row_Get_Next(uiColumn_Number, pszTemp_Row);
		   		ctIndex++;
		   		}
		   	ctIndex = 0;
			while ((!bCancel) && (ctIndex < uiNum_Rows) && (pRead_Database->Row_Get_Next(uiColumn_Number, pszTemp_Row))) {
		   		// Update the progress bar if it is there
		   		if (pProgress_Bar)
		   			bCancel = !pProgress_Bar->Progress_Bar_Update();
				if (bColors) {
    				if (strncmp(pszTemp_Row, "RED-", strlen("RED-")) == 0) {
    					pdTemp_Colors[ctIndex] = GUI_RED;
						strcpy(ppszTemp_Table[ctIndex], &(pszTemp_Row[strlen("RED-")]));
						}
    				else if (strncmp(pszTemp_Row, "YELLOW-", strlen("YELLOW-")) == 0) {
    					pdTemp_Colors[ctIndex] = GUI_YELLOW;
						strcpy(ppszTemp_Table[ctIndex], &(pszTemp_Row[strlen("YELLOW-")]));
						}
    				else if (strncmp(pszTemp_Row, "GREEN-", strlen("GREEN-")) == 0) {
    					pdTemp_Colors[ctIndex] = GUI_GREEN;
						strcpy(ppszTemp_Table[ctIndex], &(pszTemp_Row[strlen("GREEN-")]));
						}
    				else if (strncmp(pszTemp_Row, "GRAY-", strlen("GRAY-")) == 0) {
    					pdTemp_Colors[ctIndex] = GUI_GRAY_LIGHT;
						strcpy(ppszTemp_Table[ctIndex], &(pszTemp_Row[strlen("GRAY-")]));
						}
    				else {
    					pdTemp_Colors[ctIndex] = GUI_WHITE;
						strcpy(ppszTemp_Table[ctIndex], pszTemp_Row);
						}
    				}
    			else 
					strcpy(ppszTemp_Table[ctIndex], pszTemp_Row);
				ctIndex++;
				}
			if ((!bCancel) && (ctIndex == uiNum_Rows)) {
				// If a column name is not passed then use one in DB
				if (!pszColumn_Name) {
					if (!pRead_Database->Get_Column_Name(uiColumn_Number, pszTemp_Column_Name)) { //fix, wants char*[]
						strcpy(pszTemp_Column_Name, "???");
						}
					}
				// Fix the dashes in the column name
				else {
					strcpy(pszTemp_Column_Name, pszColumn_Name);
					pRead_Database->Fix_Column_Name(&pszTemp_Column_Name);
					}
				if ((pTable->AddColTxt(pszTemp_Column_Name, uiNum_Rows, ppszTemp_Table, uiMax_Col_Length)) >= 0) {
					Add_Column_Name(pszTemp_Column_Name);
					// Define all the empty cells
					Create_New_Table_Cells(uiNum_Rows);
					// Determine total number rows and add column name to our list
					uiRows_Added = max(uiRows_Added, uiNum_Rows); 
					if (!bWrite_Enabled)
						pTable->SetColWriteEnable(uiColumns_Added - 1, bWrite_Enabled);
					if (bColors) {
						for (ctIndex = 0; ctIndex < uiNum_Rows; ctIndex++)
							pTable->SetCellColor(ctIndex, uiColumns_Added - 1, GUI_BLACK, pdTemp_Colors[ctIndex]);
						}
					bSuccess = TRUE;
					}
				}
			else 
				bSuccess = FALSE;
						
			}
		}         
						

	// Delete the temporary data
	SEB_DELETELISTSTRING(ppszTemp_Table, uiNum_Rows, ctRow_Number);
	if (pdTemp_Colors) {
		delete [] pdTemp_Colors;
		pdTemp_Colors = NULL;
		}
	if (pszTemp_Row) {
		delete [] pszTemp_Row;
		pszTemp_Row = NULL;
		}
	if (pszTemp_Column_Name) {
		delete [] pszTemp_Column_Name;
		pszTemp_Column_Name = NULL;
		}
	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}


	return bSuccess;

}


BOOL Table_Worker::Fill_Table_With_Columns(const char *pszRead_Database_Name, UINT uiStart_Column_Number, UINT uiEnd_Column_Number, BOOL bWrite_Enabled) {

	UINT uiNum_Rows;
	UINT ctIndex;
    BOOL bSuccess = FALSE;
	BOOL bProgress_Bar_Created = FALSE;
	Text_Read_Database *pRead_Database = NULL;
    CGStr Progress_Message;
	

	if ((bTable_Worker_Created) && (uiStart_Column_Number >= 0) && 
		(uiEnd_Column_Number >= 0) && (uiStart_Column_Number <= uiEnd_Column_Number) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((uiNum_Rows = pRead_Database->Rows_Get_Number()) != 0)) {
		// Close the database
		delete pRead_Database;
		pRead_Database = NULL;

		bSuccess = TRUE;

		// Create the progress bar if necessary
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			Progress_Message = "Loading \"";
			Progress_Message += pszRead_Database_Name;
			Progress_Message += "\"";
			if ((pProgress_Bar = new SEB_Progress_Bar(uiNum_Rows * (uiStart_Column_Number - uiEnd_Column_Number + 1), (uiStart_Column_Number - uiEnd_Column_Number + 1), Progress_Message.Get_sz(), pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bSuccess = FALSE;
			else bProgress_Bar_Created = TRUE;
			}
		
		// Add the columns
		ctIndex = uiStart_Column_Number;
		while ((bSuccess) && (ctIndex < uiEnd_Column_Number)) {
			bSuccess = Fill_Table_With_Column(pszRead_Database_Name, ctIndex, NULL, bWrite_Enabled);
			ctIndex++;
			}
		}


	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}


	return bSuccess;

}


BOOL Table_Worker::Fill_Table_With_Columns(const char *pszRead_Database_Name, const UINT *puiColumn_Numbers, UINT uiNumber_Columns, const char **ppszColumn_Names, BOOL bWrite_Enabled) {

	UINT uiRows_Number;
    BOOL bSuccess = FALSE;
	Text_Read_Database *pRead_Database = NULL;
    
	
	if ((bTable_Worker_Created) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((uiRows_Number = pRead_Database->Rows_Get_Number()) != 0) &&
		(uiNumber_Columns >= 0)) {

		// Close the database
		delete pRead_Database;
		pRead_Database = NULL;

		// Add the columns
		bSuccess = Fill_Table_With_Columns(pszRead_Database_Name, puiColumn_Numbers, uiNumber_Columns, 1, uiRows_Number, ppszColumn_Names, bWrite_Enabled);
		}


	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}


	return bSuccess;

}


BOOL Table_Worker::Fill_Table_With_Columns(const char *pszRead_Database_Name, const UINT *puiColumn_Numbers, UINT uiNumber_Columns, UINT uiStart_Row, UINT uiEnd_Row, const char **ppszColumn_Names, BOOL bWrite_Enabled) {

	UINT ctIndex = 0;
	UINT uiRows_Number;
    BOOL bSuccess = FALSE;
	BOOL bProgress_Bar_Created = FALSE;
	Text_Read_Database *pRead_Database = NULL;
    CGStr Progress_Message;
    
	
	if ((bTable_Worker_Created) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		((uiRows_Number = pRead_Database->Rows_Get_Number()) != 0) &&
		(uiNumber_Columns >= 0) &&
		(uiStart_Row <= uiEnd_Row)) {

		// Create the progress bar if necessary
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			Progress_Message = "Loading \"";
			Progress_Message += pszRead_Database_Name;
			Progress_Message += "\"";
			if ((pProgress_Bar = new SEB_Progress_Bar(((unsigned long)(uiEnd_Row - uiStart_Row) * (unsigned long)uiNumber_Columns), (unsigned long)uiNumber_Columns, Progress_Message.Get_sz(), pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bSuccess = FALSE;
			else bProgress_Bar_Created = TRUE;
			}

		// Add the columns
		bSuccess = TRUE;
		while ((bSuccess) && (ctIndex < uiNumber_Columns)) {
			if (ppszColumn_Names)
				bSuccess = Fill_Table_With_Column(pszRead_Database_Name, puiColumn_Numbers[ctIndex], uiStart_Row, uiEnd_Row, ppszColumn_Names[puiColumn_Numbers[ctIndex] - 1], bWrite_Enabled);
			else
				bSuccess = Fill_Table_With_Column(pszRead_Database_Name, puiColumn_Numbers[ctIndex], uiStart_Row, uiEnd_Row, NULL, bWrite_Enabled);
			ctIndex++;
			}
		}

	if (pRead_Database) {
		delete pRead_Database;
		pRead_Database = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}


	return bSuccess;

}


BOOL Table_Worker::Fill_Table_With_Columns_FAST(const char *pszRead_Database_Name, const UINT *puiColumn_Numbers, UINT uiNumber_Columns, UINT uiStart_Row, UINT uiEnd_Row, BOOL bWrite_Enabled) {
	
	return FALSE;

}


void Table_Worker::Create_New_Table_Cells(UINT uiNum_Rows) {

	UINT ctCol_Index, ctRow_Index;


	if (uiNum_Rows < uiRows_Added) {
		for (ctRow_Index = uiNum_Rows; ctRow_Index < uiRows_Added; ctRow_Index++) {
			pTable->DefineCellTxt(ctRow_Index, uiColumns_Added - 1, "", uiMax_Col_Length);
			}
		}
	else if (uiNum_Rows > uiRows_Added) {
		for (ctCol_Index = uiNum_Rows; ctCol_Index < uiColumns_Added - 1; ctCol_Index++) {
			for (ctRow_Index = uiNum_Rows; ctRow_Index < uiRows_Added; ctRow_Index++) {
				pTable->DefineCellTxt(ctRow_Index, ctCol_Index, "", uiMax_Col_Length);
				}
			}
		}
    
}


BOOL Table_Worker::Add_Column_Name(const char *pszColumn_Name) {

    char **ppszTemp_Columns;
	BOOL bSuccess = FALSE;
	UINT ctIndex;
	

   	if ((ppszTemp_Columns = (char **) new (char (*[uiColumns_Added]))) != NULL) {
   		for (ctIndex = 0; ctIndex < uiColumns_Added; ctIndex++) {
   			if ((ppszTemp_Columns[ctIndex] = new char [strlen(ppszColumns[ctIndex]) + 1]) != NULL)
   				strcpy(ppszTemp_Columns[ctIndex], ppszColumns[ctIndex]);
   			}
   			
		if (uiColumns_Added > 0) {
			for (ctIndex = 0; ctIndex < uiColumns_Added; ctIndex++)
				delete [] ppszColumns[ctIndex];
			delete [] ppszColumns;
			}

	   	if ((ppszColumns = (char **) new (char (*[uiColumns_Added+1]))) != NULL) {
	   		for (ctIndex = 0; ctIndex < uiColumns_Added; ctIndex++) {
	   			if ((ppszColumns[ctIndex] = new char [strlen(ppszTemp_Columns[ctIndex]) + 1]) != NULL)
	   				strcpy(ppszColumns[ctIndex], ppszTemp_Columns[ctIndex]);
	   			}
	   		}

		if (ppszTemp_Columns) {
			for (ctIndex = 0; ctIndex < uiColumns_Added; ctIndex++)
				delete [] ppszTemp_Columns[ctIndex];
			delete [] ppszTemp_Columns;
			}
	   			
	   	uiColumns_Added++;
		if ((ppszColumns[uiColumns_Added-1] = new char [strlen(pszColumn_Name) + 1]) != NULL) {
			strcpy(ppszColumns[uiColumns_Added-1], pszColumn_Name);
			bSuccess = TRUE;
			}
			
		}
    
    return bSuccess;
    
}

    
/////////////////////////////////////////////////////////////////////////////////////
//  Table Worker saving member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Table_Worker::Save_Table_To_File(const char *pszWrite_Database_Name) {

	UINT uiCol_Number, uiRow_Number;
    char *pszTemp_Row = NULL, *pszTemp_Data = NULL;
    BOOL bSaved_Table = FALSE, bProgress_Bar_Created = FALSE;
    COLORREF Forground_Color, Background_Color;
	Text_Write_Database *pWrite_Database = NULL;
    CGStr Progress_Message;
	

	// Create all of the structures
	if ((bTable_Worker_Created) &&
		((pszTemp_Row = new char [uiColumns_Added * uiMax_Data_Length + uiColumns_Added + 1]) != NULL) &&
		((pszTemp_Data = new char [uiMax_Data_Length + 1]) != NULL) &&
		((pWrite_Database = new Text_Write_Database(pszWrite_Database_Name, uiColumns_Added, uiRows_Added, (const char **)ppszColumns, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pWrite_Database->Database_Created())) {
           

		// Create the progress bar if necessary
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			Progress_Message = "Saving \"";
			Progress_Message += pszWrite_Database_Name;
			Progress_Message += "\"";
			if ((pProgress_Bar = new SEB_Progress_Bar(((unsigned long)(uiRows_Added)) * ((unsigned long)(uiColumns_Added)), (unsigned long)uiColumns_Added, Progress_Message.Get_sz(), pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bSaved_Table = FALSE;
			else bProgress_Bar_Created = TRUE;
			}


		// Save the table
		bSaved_Table = TRUE;
		uiRow_Number = 0;
		while ((bSaved_Table) && (uiRow_Number < uiRows_Added)) {
			pszTemp_Row[0] = NULL;
			// Update the progress bar
			if (pProgress_Bar)
				bSaved_Table = pProgress_Bar->Progress_Bar_Update();
			for (uiCol_Number = 0; uiCol_Number < uiColumns_Added; uiCol_Number++) {
				// Save the colors if required
				if (bColors) {
					pTable->GetCellColor(uiRow_Number, uiCol_Number, &Forground_Color, &Background_Color);
    				if (Background_Color == GUI_WHITE) {}
    				else if (Background_Color == GUI_RED)
   						strcat(pszTemp_Row, "RED-");
    				else if (Background_Color == GUI_YELLOW)
   						strcat(pszTemp_Row, "YELLOW-");
    				else if (Background_Color == GUI_GREEN)
   						strcat(pszTemp_Row, "GREEN-");
					else if (Background_Color == GUI_GRAY_LIGHT)
						strcat(pszTemp_Row, "GRAY-");
    				}
    			// Add the column
				pTable->TxtGetValue(uiRow_Number, uiCol_Number, pszTemp_Data, uiMax_Data_Length);
				strcat(pszTemp_Row, pszTemp_Data);
				if (uiCol_Number != uiColumns_Added - 1)
					strcat(pszTemp_Row, "\t");
				}
			if (!pWrite_Database->Row_Add(pszTemp_Row))
				bSaved_Table = FALSE;
			uiRow_Number++;
			}

		}
		
	if (pszTemp_Row) {
		delete [] pszTemp_Row;
		pszTemp_Row = NULL;
		}
	if (pszTemp_Data) {
		delete [] pszTemp_Data;
		pszTemp_Data = NULL;
		}
	if (pWrite_Database) {
		delete pWrite_Database;
		pWrite_Database = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}

				
	return bSaved_Table;
	
}		
        	

BOOL Table_Worker::Save_Table_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, const UINT *puiColumn_Positions, UINT uiStart_Insert_Row) {
    
    Text_Read_Database *pRead_Database = NULL;
    Text_Write_Database *pWrite_Database = NULL;
    char *pszTemp_Write_Name = NULL;
    UINT uiRead_Columns, uiRead_Rows;
	UINT uiCol_Number, uiRow_Number, ctInsert_Col_Number;
    char *pszTemp_Read_Row = NULL, *pszTemp_Write_Row = NULL, *pszTemp_Data = NULL;
	BOOL bRead_Data_Left;
    COLORREF Forground_Color, Background_Color;
    CGStr Progress_Message;
    BOOL bUsed_Table, bSaved_Table = FALSE;
    BOOL bProgress_Bar_Created = FALSE;
	UINT uiTotal_Write_Rows;
	BOOL bTemp_Name_Passed = FALSE;
	char *pszCurrent_Read_Data = NULL, *pszTemp_Current_Read_Data = NULL;
    

	// Check the parameters
    if ((!pszRead_Database_Name) || (!pszWrite_Database_Name) || (!puiColumn_Positions))
    	return FALSE;


	// Create a write temporary file for the write - in case read = write or fail occurs
	if ((pszTemp_Write_Name = new char [strlen(pszWrite_Database_Name) + 6 + 1]) != NULL) {
		strcpy(pszTemp_Write_Name, pszWrite_Database_Name);
		if (strlen(pszTemp_Write_Name) >= 6)
			strcpy(&(pszTemp_Write_Name[strlen(pszTemp_Write_Name) - 6]), "XXXXXX");
		else
			strcpy(pszTemp_Write_Name, "XXXXXX");
		if (!mktemp(pszTemp_Write_Name)) {
			delete [] pszTemp_Write_Name;
			pszTemp_Write_Name = NULL;
			}
		// If the names are the same then a temporary file was passed.  Be sure not to destroy it later
		if (strcmp(pszTemp_Write_Name, pszWrite_Database_Name) == 0)
			bTemp_Name_Passed = TRUE;
		}


	// Open and create all of the files and local data structures
	if ((bTable_Worker_Created) &&
		(pszTemp_Write_Name) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		(uiRead_Rows = pRead_Database->Rows_Get_Number()) &&
		(uiRead_Columns = pRead_Database->Columns_Get_Number()) &&
		((pszTemp_Write_Row = new char [(uiRead_Columns * uiMax_Data_Length) + uiRead_Columns + 1]) != NULL) &&
		((pszTemp_Read_Row = new char [(uiRead_Columns * uiMax_Data_Length) + uiRead_Columns + 1]) != NULL) &&
		((pszTemp_Data = new char [uiMax_Data_Length + 1]) != NULL)) {


		// Determine the total number of rows for the new DB
		if (uiStart_Insert_Row > uiRead_Rows)
			uiTotal_Write_Rows = uiRead_Rows + uiRows_Added;
		else
			uiTotal_Write_Rows = uiStart_Insert_Row + uiRows_Added - 1;


		// Create the write database now that we know the total number of rows
		if (((pWrite_Database = new Text_Write_Database(pszTemp_Write_Name, uiRead_Columns, uiTotal_Write_Rows, pRead_Database->Get_Column_Names(), uiMax_Col_Length, uiMax_Data_Length)) == NULL) ||
			(!pWrite_Database->Database_Created()))
			bSaved_Table = FALSE;


		// Create the progress bar if necessary
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			Progress_Message = "Saving \"";
			Progress_Message += pszWrite_Database_Name;
			Progress_Message += "\"";
			if ((pProgress_Bar = new SEB_Progress_Bar(((unsigned long)(uiRows_Added)), 1, Progress_Message.Get_sz(), pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bSaved_Table = FALSE;
			else bProgress_Bar_Created = TRUE;
			}


		// Fill the write temporary with the read and table
		bSaved_Table = TRUE;
		uiRow_Number = 1;

		// Get the rows up to the starting row
		if (uiStart_Insert_Row != NULL) {
			while (bSaved_Table && (uiRow_Number < uiStart_Insert_Row)) {
				if ((!pRead_Database->Row_Get_Next(pszTemp_Write_Row)) ||
					(!pWrite_Database->Row_Add(pszTemp_Write_Row)))
					bSaved_Table = FALSE;
				uiRow_Number++;
				}
			}
		
		uiRow_Number = 0;
		while (bSaved_Table && (uiRow_Number < uiRows_Added)) {
			pszTemp_Write_Row[0] = NULL;
			// Update the progress bar
			if (pProgress_Bar)
				bSaved_Table = pProgress_Bar->Progress_Bar_Update();
			// Get the next read row in case needed
			bRead_Data_Left = pRead_Database->Row_Get_Next(pszTemp_Read_Row);
			pszCurrent_Read_Data = pszTemp_Read_Row;
			for (uiCol_Number = 0; uiCol_Number < uiRead_Columns; uiCol_Number++) {
				// Check to see if this column is in the table
				bUsed_Table = FALSE;
				for (ctInsert_Col_Number = 0; ctInsert_Col_Number < uiColumns_Added; ctInsert_Col_Number++) {
					if (puiColumn_Positions[ctInsert_Col_Number] == uiCol_Number) {
						// Save the colors if required
						if (bColors) {
							pTable->GetCellColor(uiRow_Number, ctInsert_Col_Number, &Forground_Color, &Background_Color);
		    				if (Background_Color == GUI_WHITE) {}
		    				else if (Background_Color == GUI_RED)
		   						strcat(pszTemp_Write_Row, "RED-");
		    				else if (Background_Color == GUI_YELLOW)
		   						strcat(pszTemp_Write_Row, "YELLOW-");
		    				else if (Background_Color == GUI_GREEN)
		   						strcat(pszTemp_Write_Row, "GREEN-");
		    				else if (Background_Color == GUI_GRAY_LIGHT)
		   						strcat(pszTemp_Write_Row, "GRAY-");
		    				}
						// Get the data in the table
						pszTemp_Data[0] = NULL;
						pTable->TxtGetValue(uiRow_Number, ctInsert_Col_Number, pszTemp_Data, uiMax_Data_Length);
						strcat(pszTemp_Write_Row, pszTemp_Data);
						bUsed_Table = TRUE;
						// Point read data to next data - be sure to only point if not last data in string
						pszTemp_Current_Read_Data = strchr(pszCurrent_Read_Data, '\t');
						if (pszTemp_Current_Read_Data != NULL)
							pszCurrent_Read_Data = &(pszTemp_Current_Read_Data[1]);
						}
					}
    			// Save NULL as data
    			if (!bUsed_Table) {
					// If NO data is left in the read file then just add empty string
					if (!bRead_Data_Left)
						strcat(pszTemp_Write_Row, ""); // Just add empty string
					else {
						pszTemp_Current_Read_Data = strchr(pszCurrent_Read_Data, '\t');
						// If not on the last data, then mark it as a string and go to the next
						if (pszTemp_Current_Read_Data != NULL) {
							*pszTemp_Current_Read_Data = '\0';
							strcat(pszTemp_Write_Row, pszCurrent_Read_Data);
							// Point read data to next data
							pszCurrent_Read_Data = &(pszTemp_Current_Read_Data[1]);
							}
						// If is the last data then just add it - will just keep adding last if extra column bug
						else {
							strcat(pszTemp_Write_Row, pszCurrent_Read_Data);
							}
						}
					}
				// Add the tab at the end if necessary
				if (uiCol_Number != (uiRead_Columns - 1))
					strcat(pszTemp_Write_Row, "\t");
				}

			// Add the row to the file
			if (!pWrite_Database->Row_Add(pszTemp_Write_Row))
				bSaved_Table = FALSE;
	
		    uiRow_Number++;
			}
	    
		}
	

	// Close all of the files - must be closed before can rename
	if (pRead_Database) {
		pRead_Database->Database_Close();
		delete pRead_Database;
		pRead_Database = NULL;
		}
	if (pWrite_Database) {
		pWrite_Database->Database_Close();
		delete pWrite_Database;
		pWrite_Database = NULL;
		}
	
	// Rename the write temporary as the write
	if ((bSaved_Table) && (!bTemp_Name_Passed)) {
		// Check to see if it already exists
		if (access(pszWrite_Database_Name, 0) != -1)
			remove(pszWrite_Database_Name);
		if (rename(pszTemp_Write_Name, pszWrite_Database_Name) != 0)
			bSaved_Table = FALSE;
		}

	if (!bSaved_Table) // Could turn false above
		remove(pszTemp_Write_Name);


	// Call one last time to cover time for remaning above
	pProgress_Bar->Progress_Bar_Update();

	
	// Delete local data
	if (pszTemp_Write_Name) {
		delete [] pszTemp_Write_Name;
		pszTemp_Write_Name = NULL;
		}
	if (pszTemp_Data) {
		delete [] pszTemp_Data;
		pszTemp_Data = NULL;
		}
	if (pszTemp_Write_Row) {
		delete [] pszTemp_Write_Row;
		pszTemp_Write_Row = NULL;
		}
	if (pszTemp_Read_Row) {
		delete [] pszTemp_Read_Row;
		pszTemp_Read_Row = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}

				
	return bSaved_Table;

}


BOOL Table_Worker::Save_Data_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiDB_Position, const char **ppszColumn, UINT uiNum_Data_Rows) {

	return Save_Data_Merge_With_File(pszRead_Database_Name, pszWrite_Database_Name, uiDB_Position, ppszColumn, uiNum_Data_Rows, 1, uiRows_Added, 1);

}


BOOL Table_Worker::Save_Column_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiTable_Position, UINT uiDB_Position) {

	return Save_Column_Merge_With_File(pszRead_Database_Name, pszWrite_Database_Name, uiTable_Position, uiDB_Position, 1, uiRows_Added, 1);

}


BOOL Table_Worker::Save_Data_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiDB_Position, const char **ppszColumn, UINT uiNum_Data_Rows, UINT uiDelete_Start_Row, UINT uiDelete_End_Row, UINT uiData_Start_Row) {

    Text_Read_Database *pRead_Database = NULL;
    Text_Write_Database *pWrite_Database = NULL;
    char *pszTemp_Write_Name = NULL;
	UINT ctIndex;
    UINT uiRead_Columns, uiRead_Rows;
	UINT uiCol_Number, uiRow_Number, uiTotal_Rows_Number;
    char *pszTemp_Row = NULL, *pszTemp_Data = NULL;
    CGStr Table_Data, Progress_Message;
    BOOL bSaved_Column = FALSE;
	BOOL bProgress_Bar_Created = FALSE;
	BOOL bFirst_Time_Finished = TRUE;
    

	// Check the parameters
    if ((!pszRead_Database_Name) || (!pszWrite_Database_Name))
    	return FALSE;

	
	// Create a write temporary file for the write - in case read = write or fail occurs
	if ((pszTemp_Write_Name = new char [strlen(pszWrite_Database_Name) + 6 + 1]) != NULL) {
		strcpy(pszTemp_Write_Name, pszWrite_Database_Name);
		if (strlen(pszTemp_Write_Name) >= 6)
			strcpy(&(pszTemp_Write_Name[strlen(pszTemp_Write_Name) - 6]), "XXXXXX");
		else
			strcpy(pszTemp_Write_Name, "XXXXXX");
		if (!mktemp(pszTemp_Write_Name)) {
			delete [] pszTemp_Write_Name;
			pszTemp_Write_Name = NULL;
			}
		}


	// Open and create all of the files and local data structures
	if ((bTable_Worker_Created) &&
		(pszTemp_Write_Name) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		(uiRead_Columns = pRead_Database->Columns_Get_Number()) &&
		(uiRead_Rows = pRead_Database->Rows_Get_Number()) &&
		((pszTemp_Data = new char [uiMax_Data_Length + 1]) != NULL) &&
		((pszTemp_Row = new char [(uiRead_Columns * uiMax_Data_Length) + uiRead_Columns + 1]) != NULL)) {

		bSaved_Column = TRUE;

		// Determine the total number of rows to be inserted
		int iTotal_Top_Keep_Rows = uiDelete_Start_Row - 1;
		int iTotal_Top_NULL_Rows = uiData_Start_Row - uiDelete_Start_Row;
		int iTotal_Bottom_NULL_Rows = uiDelete_End_Row - (uiData_Start_Row + uiNum_Data_Rows - 1);
		if (iTotal_Bottom_NULL_Rows < 0)
				iTotal_Bottom_NULL_Rows = 0;
		int iTotal_Bottom_Keep_Rows = uiRead_Rows - uiDelete_End_Row;
		uiTotal_Rows_Number = (UINT)(iTotal_Top_Keep_Rows + iTotal_Top_NULL_Rows + (int)uiNum_Data_Rows  + iTotal_Bottom_NULL_Rows + iTotal_Bottom_Keep_Rows);

		// Open the write database
		if (((pWrite_Database = new Text_Write_Database(pszTemp_Write_Name, uiRead_Columns, uiTotal_Rows_Number, pRead_Database->Get_Column_Names(), uiMax_Col_Length, uiMax_Data_Length)) == NULL) ||
			(!pWrite_Database->Database_Created()))
			bSaved_Column = FALSE;

		// Create the progress bar if necessary
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			Progress_Message = "Saving \"";
			Progress_Message += pszWrite_Database_Name;
			Progress_Message += "\"";
			if ((pProgress_Bar = new SEB_Progress_Bar(((unsigned long)(uiTotal_Rows_Number)), 1, Progress_Message.Get_sz(), pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bSaved_Column = FALSE;
			else bProgress_Bar_Created = TRUE;
			}

		
		uiRow_Number = 1;
		// Get all of the initial data first
		// If current row is not start row then just get the read data
		while (bSaved_Column && (uiRow_Number < uiDelete_Start_Row)) {
			if ((!pRead_Database->Row_Get_Next(pszTemp_Row)) ||
				(!pWrite_Database->Row_Add(pszTemp_Row)))
				bSaved_Column = FALSE;
			uiRow_Number++;
			}
		
		// Fill the write temporary with the read and table
		while (bSaved_Column && (uiRow_Number <= uiTotal_Rows_Number)) {

			pszTemp_Row[0] = NULL;

			// Update the progress bar
			if (pProgress_Bar)
				bSaved_Column = pProgress_Bar->Progress_Bar_Update();

			// Check to see if current column is the one to be inserted
			for (uiCol_Number = 0; uiCol_Number < uiRead_Columns; uiCol_Number++) {

				if (uiDB_Position - 1 == uiCol_Number) {

					// If reached insert row but not data start then just add nulls
					if (uiRow_Number < uiData_Start_Row) {
						strcat(pszTemp_Row, "");
						}

					else if (uiRow_Number < uiData_Start_Row + uiNum_Data_Rows) {
						// Add the data from the passed column
						strcat(pszTemp_Row, ppszColumn[uiRow_Number - uiData_Start_Row]);
						}

					// If still have insert rows left then add nulls
					else if (uiRow_Number <= uiDelete_End_Row) {
						strcat(pszTemp_Row, "");
						}

					// If still data left in the file for this column then add it
					else {
						// FIRST TIME, Get all of the deleted rows
						if (bFirst_Time_Finished) {
							for (ctIndex = uiDelete_Start_Row; ctIndex <= uiDelete_End_Row; ctIndex++) {
								if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
									bSaved_Column = FALSE;
								}
							bFirst_Time_Finished = FALSE;
							}
						if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
							bSaved_Column = FALSE;
						else 
							strcat(pszTemp_Row, pszTemp_Data);
						}
					}
				else {
					// Check to be sure that data is left in the file
					if (uiRow_Number <= uiRead_Rows) {
						if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
							bSaved_Column = FALSE;
						else
							strcat(pszTemp_Row, pszTemp_Data);
						}
					else strcat(pszTemp_Row, ""); // Add NULLs
					}

				
				// Add the tab at the end if necessary
				if (uiCol_Number != (uiRead_Columns - 1))
					strcat(pszTemp_Row, "\t");
				}

			// Add the row to the file
			if (!pWrite_Database->Row_Add(pszTemp_Row))
				bSaved_Column = FALSE;
	
		    uiRow_Number++;
			}
	    
		}
	

	// Close all of the files - must be closed before can rename
	if (pRead_Database) {
		pRead_Database->Database_Close();
		delete pRead_Database;
		pRead_Database = NULL;
		}
	if (pWrite_Database) {
		pWrite_Database->Database_Close();
		delete pWrite_Database;
		pWrite_Database = NULL;
		}
	

	// Rename the write temporary as the write
	if (bSaved_Column) {
		// Check to see if it already exists
		if (access(pszWrite_Database_Name, 0) != -1)
			remove(pszWrite_Database_Name);
		if (rename(pszTemp_Write_Name, pszWrite_Database_Name) != 0)
			bSaved_Column = FALSE;
		}

	if (!bSaved_Column) // Could turn false above
		remove(pszTemp_Write_Name);
	
	// Delete local data
	if (pszTemp_Write_Name) {
		delete [] pszTemp_Write_Name;
		pszTemp_Write_Name = NULL;
		}
	if (pszTemp_Data) {
		delete [] pszTemp_Data;
		pszTemp_Data = NULL;
		}
	if (pszTemp_Row) {
		delete [] pszTemp_Row;
		pszTemp_Row = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}

				
	return bSaved_Column;

}


BOOL Table_Worker::Save_Column_Merge_With_File(const char *pszRead_Database_Name, const char *pszWrite_Database_Name, UINT uiTable_Position, UINT uiDB_Position, UINT uiDelete_Start_Row, UINT uiDelete_End_Row, UINT uiData_Start_Row) {

    Text_Read_Database *pRead_Database = NULL;
    Text_Write_Database *pWrite_Database = NULL;
    char *pszTemp_Write_Name = NULL;
	UINT ctIndex;
    UINT uiRead_Columns, uiRead_Rows;
	UINT uiCol_Number, uiRow_Number, uiTotal_Rows_Number;
    char *pszTemp_Row = NULL, *pszTemp_Data = NULL;
    CGStr Table_Data, Progress_Message;
    BOOL bSaved_Column = FALSE;
	BOOL bProgress_Bar_Created = FALSE;
    COLORREF Forground_Color, Background_Color;
	BOOL bFirst_Time_Finished = TRUE;
    

	// Check the parameters
    if ((!pszRead_Database_Name) || (!pszWrite_Database_Name))
    	return FALSE;

	
	// Create a write temporary file for the write - in case read = write or fail occurs
	if ((pszTemp_Write_Name = new char [strlen(pszWrite_Database_Name) + 6 + 1]) != NULL) {
		strcpy(pszTemp_Write_Name, pszWrite_Database_Name);
		if (strlen(pszTemp_Write_Name) >= 6)
			strcpy(&(pszTemp_Write_Name[strlen(pszTemp_Write_Name) - 6]), "XXXXXX");
		else
			strcpy(pszTemp_Write_Name, "XXXXXX");
		if (!mktemp(pszTemp_Write_Name)) {
			delete [] pszTemp_Write_Name;
			pszTemp_Write_Name = NULL;
			}
		}


	// Open and create all of the files and local data structures
	if ((bTable_Worker_Created) &&
		(pszTemp_Write_Name) &&
		((pRead_Database = new Text_Read_Database(pszRead_Database_Name, uiMax_Col_Length, uiMax_Data_Length)) != NULL) &&
		(pRead_Database->Database_Opened()) &&
		(uiRead_Columns = pRead_Database->Columns_Get_Number()) &&
		(uiRead_Rows = pRead_Database->Rows_Get_Number()) &&
		((pszTemp_Data = new char [uiMax_Data_Length + 1]) != NULL) &&
		((pszTemp_Row = new char [(uiRead_Columns * uiMax_Data_Length) + uiRead_Columns+1]) != NULL)) {

		bSaved_Column = TRUE;

		// Determine the total number of rows to be inserted
		int iTotal_Top_Keep_Rows = uiDelete_Start_Row - 1;
		int iTotal_Top_NULL_Rows = uiData_Start_Row - uiDelete_Start_Row;
		int iTotal_Bottom_NULL_Rows = uiDelete_End_Row - (uiData_Start_Row + uiRows_Added - 1);
		if (iTotal_Bottom_NULL_Rows < 0)
				iTotal_Bottom_NULL_Rows = 0;
		int iTotal_Bottom_Keep_Rows = uiRead_Rows - uiDelete_End_Row;
		uiTotal_Rows_Number = (UINT)(iTotal_Top_Keep_Rows + iTotal_Top_NULL_Rows + (int)uiRows_Added + iTotal_Bottom_NULL_Rows + iTotal_Bottom_Keep_Rows);


		// Open the write database
		if (((pWrite_Database = new Text_Write_Database(pszTemp_Write_Name, uiRead_Columns, uiTotal_Rows_Number, pRead_Database->Get_Column_Names(), uiMax_Col_Length, uiMax_Data_Length)) == NULL) ||
			(!pRead_Database->Database_Opened()))
			bSaved_Column = FALSE;

		// Create the progress bar if necessary
		if ((bProgress_Bar_Enabled) && (!pProgress_Bar)) {
			Progress_Message = "Saving \"";
			Progress_Message += pszWrite_Database_Name;
			Progress_Message += "\"";
			if ((pProgress_Bar = new SEB_Progress_Bar(((unsigned long)(uiTotal_Rows_Number)), 1, Progress_Message.Get_sz(), pszProgress_Bar_Title, uiProgress_Bar_Icon)) == NULL)
				bSaved_Column = FALSE;
			else bProgress_Bar_Created = TRUE;
			}

		
		// Fill the write temporary with the read and table
		uiRow_Number = 1;
		while (bSaved_Column && (uiRow_Number <= uiTotal_Rows_Number)) {

			pszTemp_Row[0] = NULL;

			// Update the progress bar
			if (pProgress_Bar)
				bSaved_Column = pProgress_Bar->Progress_Bar_Update();

			// Check to see if current column is the one to be inserted
			for (uiCol_Number = 0; uiCol_Number < uiRead_Columns; uiCol_Number++) {

				if (uiDB_Position - 1 == uiCol_Number) {
					// If current row is not insert start row then just get the read data
					if (uiRow_Number < uiDelete_Start_Row) {
						if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
							bSaved_Column = FALSE;
						else 
							strcat(pszTemp_Row, pszTemp_Data);
						}

					// If reached insert row but not data start then just add nulls
					else if (uiRow_Number < uiData_Start_Row) {
						strcat(pszTemp_Row, "");
						}

					// If reached insert row and data then add data from table
					else if (uiRow_Number < uiData_Start_Row + uiRows_Added) {
						// Set the color if required
						if (bColors) {
							pTable->GetCellColor(uiRow_Number - uiData_Start_Row, uiTable_Position - 1, &Forground_Color, &Background_Color);
		    				if (Background_Color == GUI_WHITE) {}
		    				else if (Background_Color == GUI_RED)
		   						strcat(pszTemp_Row, "RED-");
		    				else if (Background_Color == GUI_YELLOW)
		   						strcat(pszTemp_Row, "YELLOW-");
		    				else if (Background_Color == GUI_GREEN)
		   						strcat(pszTemp_Row, "GREEN-");
		    				else if (Background_Color == GUI_GRAY_LIGHT)
		   						strcat(pszTemp_Row, "GRAY-");
		    				}
						// Get the data in the table
						pTable->TxtGetValue(uiRow_Number - uiData_Start_Row, uiTable_Position - 1, &Table_Data);
						strcat(pszTemp_Row, Table_Data.Get_sz());
						}

					// If still have insert rows left then add nulls
					else if (uiRow_Number <= uiDelete_End_Row) {
						strcat(pszTemp_Row, "");
						}

					// If still data left in the file for this column then add it
					else {
						// FIRST TIME, Get all of the deleted rows
						if (bFirst_Time_Finished) {
							for (ctIndex = uiDelete_Start_Row; ctIndex <= uiDelete_End_Row; ctIndex++) {
								if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
									bSaved_Column = FALSE;
								}
							bFirst_Time_Finished = FALSE;
							}
						if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
							bSaved_Column = FALSE;
						else 
							strcat(pszTemp_Row, pszTemp_Data);
						}
					}
				else {
					// Check to be sure that data is left in the file
					if (uiRow_Number <= uiRead_Rows) {
						if (!pRead_Database->Row_Get_Next(uiCol_Number + 1, pszTemp_Data))
							bSaved_Column = FALSE;
						else
							strcat(pszTemp_Row, pszTemp_Data);
						}
					else strcat(pszTemp_Row, ""); // Add NULLs
					}

				
				// Add the tab at the end if necessary
				if (uiCol_Number != (uiRead_Columns - 1))
					strcat(pszTemp_Row, "\t");
				}

			// Add the row to the file
			if (!pWrite_Database->Row_Add(pszTemp_Row))
				bSaved_Column = FALSE;
	
		    uiRow_Number++;
			}
	    
		}
	

	// Close all of the files - must be closed before can rename
	if (pRead_Database) {
		pRead_Database->Database_Close();
		delete pRead_Database;
		pRead_Database = NULL;
		}
	if (pWrite_Database) {
		pWrite_Database->Database_Close();
		delete pWrite_Database;
		pWrite_Database = NULL;
		}
	

	// Rename the write temporary as the write
	if (bSaved_Column) {
		// Check to see if it already exists
		if (access(pszWrite_Database_Name, 0) != -1)
			remove(pszWrite_Database_Name);
		if (rename(pszTemp_Write_Name, pszWrite_Database_Name) != 0)
			bSaved_Column = FALSE;
		}

	if (!bSaved_Column) // Could turn false above
		remove(pszTemp_Write_Name);
	
	// Delete local data
	if (pszTemp_Write_Name) {
		delete [] pszTemp_Write_Name;
		pszTemp_Write_Name = NULL;
		}
	if (pszTemp_Data) {
		delete [] pszTemp_Data;
		pszTemp_Data = NULL;
		}
	if (pszTemp_Row) {
		delete [] pszTemp_Row;
		pszTemp_Row = NULL;
		}
	if ((bProgress_Bar_Created) && (pProgress_Bar)) {
		delete pProgress_Bar;
		pProgress_Bar = NULL;
		}

				
	return bSaved_Column;

}


