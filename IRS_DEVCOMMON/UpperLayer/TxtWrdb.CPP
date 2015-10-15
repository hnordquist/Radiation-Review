// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 9/20/96

// TXT_WrDB.CPP
// Member functions for writing text databases


#include "TXTWrDB.H"   


/////////////////////////////////////////////////////////////////////////////////////
//  Text_Write_Database constructor:  Setup all intial parameters, open file, and
//										write header
/////////////////////////////////////////////////////////////////////////////////////

// Column Name array of strings version     
Text_Write_Database::Text_Write_Database(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char **ppszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length) :
	bFile_Open(FALSE),
	pText_File(NULL) {
	                      
	Database_Create(pszDatabase_Name, uiNumber_Columns, uiNumber_Rows, ppszColumn_Names, uiMax_Column_Name_Length, uiMax_Data_Length)		;

}


// Column Name comma seperated string version
Text_Write_Database::Text_Write_Database(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char *pszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length) :
	bFile_Open(FALSE),
	pText_File(NULL) {
	                      
	Database_Create(pszDatabase_Name, uiNumber_Columns, uiNumber_Rows, pszColumn_Names, uiMax_Column_Name_Length, uiMax_Data_Length);		
	
}


Text_Write_Database::Text_Write_Database(void) :
	uiNum_Cols(0),
	uiNum_Rows(0),
	uiMax_Col_Length(0),
    uiMax_Data_Lngth(0),
	bFile_Open(FALSE),
	pText_File(NULL) {
		
}


Text_Write_Database::~Text_Write_Database(void) {

	if (pText_File) {
		delete pText_File;
		pText_File = NULL;
		}

}


/////////////////////////////////////////////////////////////////////////////////////
//  Text_Write_Database status member functions
/////////////////////////////////////////////////////////////////////////////////////

// sk 05/15/2003  Modified to save the position of where the number of rows is written so we can update this later
// when actually know now many rows were written
BOOL Text_Write_Database::Database_Create(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char **ppszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length) {

	// Temporary variables
	UINT ctIndex;
	char *pszTemp_Column_Name = NULL, *pDash;


	uiNum_Cols = uiNumber_Columns;
	uiNum_Rows = uiNumber_Rows;
    uiMax_Col_Length = uiMax_Column_Name_Length;
    uiMax_Data_Lngth = uiMax_Data_Length;
    
	// Build the header first
	if (pText_File) {
		delete pText_File;
		pText_File = NULL;
	}
	pText_File = new ofstream(pszDatabase_Name, ios::out);
	if ((pText_File->is_open()) &&
		((pszTemp_Column_Name = new char [uiMax_Column_Name_Length + 1]) != NULL)) {
		// Add the number of columns and add the number of rows
		// sk 5/15/2003 if ((*pText_File << uiNum_Cols << " Columns" << endl) && (*pText_File << uiNum_Rows << " Rows" << endl)) {
		if (*pText_File << uiNum_Cols << " Columns" << endl)  {	//sfk 5/15/2003
			muiNumRowsPosition = pText_File->tellp();		//sfk 5/15/2003 remember where the # of rows is written
			if (*pText_File << uiNum_Rows << " Rows" << endl) {	//sfk 5/15/2003
				ctIndex = 0;
				// Add the column names
				while (ctIndex < uiNum_Cols) {
					strcpy(pszTemp_Column_Name, ppszColumn_Names[ctIndex]);
					while ((pDash = strchr(pszTemp_Column_Name, '\n')) != NULL)
						*pDash = '-';
					*pText_File << pszTemp_Column_Name;
					ctIndex++;
					if (ctIndex == uiNum_Cols)
						*pText_File << endl;
					else
						*pText_File << "\t";
					}
				if (ctIndex == uiNum_Cols)
					bFile_Open = TRUE;
				}
		}
		}

	if (pszTemp_Column_Name) {
		delete [] pszTemp_Column_Name;
		pszTemp_Column_Name = NULL;
		}

			
	return bFile_Open;

}


BOOL Text_Write_Database::Database_Create(const char *pszDatabase_Name, UINT uiNumber_Columns, UINT uiNumber_Rows, const char *pszColumn_Names, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length) {

	// Temporary variables
	char *pszTemp_Column_Name, *pDash;


	uiNum_Cols = uiNumber_Columns;
	uiNum_Rows = uiNumber_Rows;
    uiMax_Col_Length = uiMax_Column_Name_Length;
    uiMax_Data_Lngth = uiMax_Data_Length;
    
	// Build the header first
	pText_File = new ofstream(pszDatabase_Name, ios::out);
	if ((pText_File->is_open()) &&
		((pszTemp_Column_Name = new char [strlen(pszColumn_Names) + 1]) != NULL)) {
		// Add the number of columns and add the number of rows
		//if ((*pText_File << uiNum_Cols << " Columns" << endl) && (*pText_File << uiNum_Rows << " Rows" << endl)) {
		// sk 5/15/2003 if ((*pText_File << uiNum_Cols << " Columns" << endl) && (*pText_File << uiNum_Rows << " Rows" << endl)) {
		if (*pText_File << uiNum_Cols << " Columns" << endl)  {	//sfk 5/15/2003
			muiNumRowsPosition = pText_File->tellp();		//sfk 5/15/2003 remember where the # of rows is written
			if (*pText_File << uiNum_Rows << " Rows" << endl) {	//sfk 5/15/2003

				strcpy(pszTemp_Column_Name, pszColumn_Names);
				while ((pDash = strchr(pszTemp_Column_Name, '\n')) != NULL)
					*pDash = '-';
				// Add the column names
				bFile_Open = TRUE;
				if (!(Row_Add(pszTemp_Column_Name, TRUE)))
					bFile_Open = FALSE;
				}
				delete [] pszTemp_Column_Name;
			}
		}
			
	return bFile_Open;

}


BOOL Text_Write_Database::Database_Created(void) {
 
	return bFile_Open; 

}


BOOL Text_Write_Database::Database_Close(void) {

	if (bFile_Open) {
		pText_File->close();
		bFile_Open = FALSE;
		}
			
	return TRUE;

}


UINT Text_Write_Database::Rows_Get_Number(void) {        
    
	if (bFile_Open)
		return uiNum_Rows;
	else
		return 0;

}


UINT Text_Write_Database::Columns_Get_Number(void) {        

	if (bFile_Open)
		return uiNum_Cols;
	else
		return 0;

}
 
 
/////////////////////////////////////////////////////////////////////////////////////
//  Text_Write_Database database reading member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Text_Write_Database::Row_Add(const char *pszRow, BOOL bComma_Separated) {

	const char *pszCurrent_Location = pszRow;
	char *pszData, *pszNext_Comma;
	BOOL bSuccess = TRUE;
	UINT uiData_Length, uiColumn_Num = 1;
	

	if (bFile_Open)
		if (!bComma_Separated) {
			if (*pText_File << pszRow << endl)
		    	return TRUE;
		    }
		else {
			if ((pszData = new char [uiMax_Data_Lngth]) == NULL)
				return FALSE;
		    while ((uiColumn_Num < uiNum_Cols) && (bSuccess)) {
		    	// Find the next comma
		    	if (pszNext_Comma = strchr(pszCurrent_Location, ',')) {
				    uiData_Length = strlen(pszCurrent_Location) - strlen(pszNext_Comma);
				    strncpy(pszData, pszCurrent_Location, uiData_Length);
				    pszData[uiData_Length] = '\0';
				    if (pszCurrent_Location[uiData_Length+1] == ' ')
					    pszCurrent_Location += (uiData_Length + 2);
					else 
				    	pszCurrent_Location += (uiData_Length + 1);
					if (!(*pText_File << pszData << '\t'))
						bSuccess = FALSE;
			    	uiColumn_Num++;
			    	}
			    else
		    		bSuccess = FALSE;
				}
			
			if (bSuccess) {	
				strcpy(pszData, pszCurrent_Location);				
				if (!(*pText_File << pszData << endl))
					bSuccess = FALSE;
				}

			delete [] pszData;
			return bSuccess;
	    	}

	return FALSE;

}


BOOL Text_Write_Database::Row_Add(char **ppszRow) {
               
	UINT ctColumn_Number = 0;               
               
	if (bFile_Open) {
		//if (uiNum_Cols != sizeof(ppszRow))
		//	return FALSE;
		while ((ctColumn_Number < uiNum_Cols) &&
				(*pText_File << ppszRow[ctColumn_Number])) {
			if (ctColumn_Number < (uiNum_Cols - 1))
				*pText_File << '\t';
			ctColumn_Number++;
			}
		if (ctColumn_Number == uiNum_Cols) {
			*pText_File << endl;
	    	return TRUE;
	    	}
	    }

	return FALSE;

}


// 05-15-2003 sfk Added this function so can update the number of rows part of the table after the rows
// are written to the table.
UINT Text_Write_Database::Rows_Set_Number(const char * NumRowsStr) 
{        
	long lTempFilePos;

	if (bFile_Open)
		lTempFilePos = pText_File->tellp();		// save current file pointer
		pText_File->seekp(muiNumRowsPosition);	// move to the spot where you know the # rows digits are
		if (*pText_File << NumRowsStr)		
		return 1;
	else
		return 0;

}
 