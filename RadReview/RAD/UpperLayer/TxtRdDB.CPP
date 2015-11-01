// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 9/20/96

// TXT_RdDB.CPP
// Member functions for reading text databases


#include "TXTRdDB.H"   



/////////////////////////////////////////////////////////////////////////////////////
//  Text_Read_Database constructor:  Setup all intial parameters, open file, and
//									write header
/////////////////////////////////////////////////////////////////////////////////////
     
Text_Read_Database::Text_Read_Database(const char *pszDatabase_Name, UINT uiMax_Column_Name_Length, UINT uiMax_Data_Length) :
	uiNum_Cols(0),
	uiNum_Rows(0),
	uiMax_Col_Length(uiMax_Column_Name_Length),
    uiMax_Data_Length(uiMax_Data_Length),
    ppszColumn_Names(NULL),
	bFile_Open(FALSE)
	{

	Columns.pText_DB_File = NULL;
	Columns.dRow_Position = 0;
	Columns.pColumn_Info = NULL;

	
	// Temporary variables
	UINT ctIndex;
	char *szTemp_Col_Names, *pDash;
	BOOL bContinue = TRUE;
	char szTitle_Buffer[50];

    
    // Open the file
	if ((Columns.pText_DB_File = new ifstream(pszDatabase_Name, ios::nocreate)) == NULL)
		return;

	// Read the header first	                      
	if (Columns.pText_DB_File->is_open()) {

		// Get the number of columns and get the number of rows
 		if ((*Columns.pText_DB_File >> uiNum_Cols) && 
			// Get header after columns but don't care what it is
			(Columns.pText_DB_File->getline(szTitle_Buffer, 50)) &&
			(*Columns.pText_DB_File >> uiNum_Rows) &&
			// Get header after rows but don't care what it is
			(Columns.pText_DB_File->getline(szTitle_Buffer, 50)) &&
			// Create the variables and open the file
 			(uiNum_Cols != 0) &&
			((Columns.pColumn_Info = new COLUMN_DATA [uiNum_Cols]) != NULL) &&
			((ppszColumn_Names = new (char (* ([uiNum_Cols])))) != NULL) &&
			((szTemp_Col_Names = new char [(uiMax_Column_Name_Length*uiNum_Cols)+1]) != NULL)) {
			
			// Add the column names			
			ctIndex = 0;
			while ((ctIndex < uiNum_Cols) && (bContinue)) {
				if (ctIndex < (uiNum_Cols - 1)) {
					Columns.pText_DB_File->getline(szTemp_Col_Names, (uiMax_Column_Name_Length * uiNum_Cols), '\t');
					if ((*szTemp_Col_Names != NULL) && 
						((ppszColumn_Names[ctIndex] = new char[strlen(szTemp_Col_Names)+1]) != NULL)) {
						Columns.pColumn_Info[ctIndex].pszColumn_Name = ppszColumn_Names[ctIndex];
						strcpy(Columns.pColumn_Info[ctIndex].pszColumn_Name, szTemp_Col_Names);
						// Replace -'s with newline to save space for long column names
						while ((pDash = strchr(Columns.pColumn_Info[ctIndex].pszColumn_Name, '-')) != NULL)
							*pDash = '\n';
						ctIndex++;
						}
					else
						bContinue = FALSE;
					}
				else {
					Columns.pText_DB_File->getline(szTemp_Col_Names, (uiMax_Column_Name_Length * uiNum_Cols), '\n');
					if ((*szTemp_Col_Names != NULL) &&
						((ppszColumn_Names[ctIndex] = new char[strlen(szTemp_Col_Names)+1]) != NULL)) {
						Columns.pColumn_Info[ctIndex].pszColumn_Name = ppszColumn_Names[ctIndex];
						strcpy(Columns.pColumn_Info[ctIndex].pszColumn_Name, szTemp_Col_Names);
						// Replace -'s with newline to save space for long column names
						while ((pDash = strchr(Columns.pColumn_Info[ctIndex].pszColumn_Name, '-')) != NULL)
							*pDash = '\n';
						ctIndex++;
						}
					else
						bContinue = FALSE;
					}					
				}
				
			if (ctIndex == uiNum_Cols) {
				// Set the file pointer and all of the row indicators to their proper positons
				Columns.dRow_Position = Columns.pText_DB_File->tellg();
				for (ctIndex = 0; ctIndex < uiNum_Cols; ctIndex++)
					Columns.pColumn_Info[ctIndex].dData_Position = Columns.dRow_Position; 
				bFile_Open = TRUE;
				} 

			delete [] szTemp_Col_Names;
			}
		}
		
}


Text_Read_Database::~Text_Read_Database(void) {

	UINT ctIndex;
	

	// Deleting the ppszColumn_Names will actually delete the names for each Columns.pColumn_Info[ctIndex].pszColumn_Name
	if (ppszColumn_Names) {
		for (ctIndex = 0; ctIndex < uiNum_Cols; ctIndex++)
			if (ppszColumn_Names[ctIndex] != NULL)
				delete [] ppszColumn_Names[ctIndex];
		delete [] ppszColumn_Names;
		}

	if (Columns.pColumn_Info)
		delete [] Columns.pColumn_Info;

	if (Columns.pText_DB_File) {
		Columns.pText_DB_File->close();
		delete Columns.pText_DB_File;
		}

}


/////////////////////////////////////////////////////////////////////////////////////
//  Text_Read_Database status member functions
/////////////////////////////////////////////////////////////////////////////////////



BOOL Text_Read_Database::Database_Opened(void) {
 
	return bFile_Open; 

}


BOOL Text_Read_Database::Database_Close(void) {

	if (bFile_Open)
		Columns.pText_DB_File->close();
		
	return TRUE;

}


UINT Text_Read_Database::Rows_Get_Number(void) {        
    
	if (bFile_Open)
		return uiNum_Rows;
	else
		return 0;

}


UINT Text_Read_Database::Columns_Get_Number(void) {        

	if (bFile_Open)
		return uiNum_Cols;
	else
		return 0;

}


BOOL Text_Read_Database::Column_Reset(UINT uiColumn_Number) {

	if ((uiColumn_Number > uiNum_Cols) || (uiColumn_Number <= 0))
		return FALSE;
		
	Columns.pColumn_Info[uiColumn_Number-1].dData_Position = Columns.dRow_Position;

	
	return TRUE;
	
}


BOOL Text_Read_Database::Get_Column_Name(UINT uiColumn_Number, char *pszColumn_Name) {
                                                                                 
	if ((bFile_Open) && (uiColumn_Number <= uiNum_Cols)) {
		strcpy(pszColumn_Name, ppszColumn_Names[uiColumn_Number-1]);
		return TRUE;
        }


	return FALSE;

}

 
const char *Text_Read_Database::Get_Column_Name(UINT uiColumn_Number) {
                                                                                 
	if ((bFile_Open) && (uiColumn_Number <= uiNum_Cols))
		return ((const char *)ppszColumn_Names[uiColumn_Number-1]);


	return FALSE;

}

 
const char **Text_Read_Database::Get_Column_Names(void) {
                                                                                 
	if (bFile_Open)
		return ((const char **)ppszColumn_Names);


	return FALSE;

}

 
BOOL Text_Read_Database::Fix_Column_Name(char **ppszColumn_Name) {

	char *pDash;


	// Replace -'s with newline to save space for long column names
	while ((pDash = strchr((*ppszColumn_Name), '-')) != NULL)
		*pDash = '\n';


	return TRUE;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Text_Read_Database database reading member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Text_Read_Database::Row_Get_Next(UINT uiColumn_Number, char *pszData, BOOL bRemove_Lead_Trail_Spaces) {

	UINT ctIndex;
	char *pszTemp_Row_Data;
    BOOL bSuccess = FALSE;
    

	if ((pszTemp_Row_Data = new char [(uiMax_Data_Length*uiNum_Cols)+1]) == NULL)
		return FALSE;

	if ((bFile_Open) && (pszTemp_Row_Data != NULL)) {
		ctIndex = 0;	
		Columns.pText_DB_File->seekg(Columns.pColumn_Info[uiColumn_Number-1].dData_Position);
		// Get the columns up to the requested column, use pszData as a temp holder
		while (ctIndex < uiColumn_Number-1) {
			Columns.pText_DB_File->getline(pszData, uiMax_Data_Length, '\t');
			ctIndex++;
			}
		// Continue if data was found and at the proper column
		if ((pszData != NULL) && (ctIndex == uiColumn_Number-1)) {
			// If at last column, get data up until newline
			if (uiColumn_Number == uiNum_Cols) {
				Columns.pText_DB_File->getline(pszData, uiMax_Data_Length, '\n');
				if (Columns.pText_DB_File->good()) {
					bSuccess = TRUE;
					Columns.pColumn_Info[uiColumn_Number-1].dData_Position = Columns.pText_DB_File->tellg();
					}
				}
			// If not at last column, get data up until next tab
			else {
				Columns.pText_DB_File->getline(pszData, uiMax_Data_Length, '\t');
				if (Columns.pText_DB_File->good()) {
					bSuccess = TRUE;
					Columns.pText_DB_File->sync();
					Columns.pText_DB_File->getline(pszTemp_Row_Data, (uiMax_Data_Length*uiNum_Cols), '\n');
					// BUG FIX: This is a very strange bug with the C Lib tellg call:  Sometimes it seems to return a value which is one position forward
					//          too many.  If reading the first column then the first character gets skipped.  To correct this problem I have added a
					//          check for the first column which computes it based on the number of characters read in the first column.
					if (uiColumn_Number == 1)
						Columns.pColumn_Info[uiColumn_Number-1].dData_Position = Columns.pColumn_Info[uiColumn_Number-1].dData_Position + strlen(pszData) + 1 + strlen(pszTemp_Row_Data) + 2;
					else
					Columns.pColumn_Info[uiColumn_Number-1].dData_Position = Columns.pText_DB_File->tellg();
					}
				}
			}

		// Remove leading and trailing spaces if requested
		if ((bSuccess) && (bRemove_Lead_Trail_Spaces)) {
			Remove_Spaces_At_Begin(pszData);
			Remove_Spaces_At_End(pszData);
			}
		}

	delete [] pszTemp_Row_Data;


	return bSuccess;

}


BOOL Text_Read_Database::Row_Get_Next(char *pszData) {

	UINT ctIndex;
	DWORD dData_Position;


	if (bFile_Open) {
		Columns.pText_DB_File->seekg(Columns.dRow_Position);
		Columns.pText_DB_File->getline(pszData, (uiMax_Col_Length * (uiMax_Data_Length + 1)));
		if (*pszData != NULL) {
			Columns.dRow_Position = Columns.pText_DB_File->tellg();
			// Set all of the individual column-row data
			dData_Position = Columns.pText_DB_File->tellg();
			for (ctIndex = 0; ctIndex < uiNum_Cols; ctIndex++)
				Columns.pColumn_Info[ctIndex].dData_Position = dData_Position;
			return TRUE;
			}
		}
	
	return FALSE;

}

			
/////////////////////////////////////////////////////////////////////////////////////
//  Text_Read_Database database helper member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Text_Read_Database::Remove_Spaces_At_Begin(char *pszString) {

	char *pszString_Holder, *pszString_Start;


	pszString_Holder = pszString;
	pszString_Start = pszString;

	// Find the start of the string beyond the spaces
	while ((*pszString_Start != NULL) && (*pszString_Start == ' '))
		pszString_Start++;
	
	// Copy over each character up to the NULL
	if (pszString_Holder != pszString_Start) {
		while (*pszString_Start != NULL) {
			*pszString_Holder = *pszString_Start;
			pszString_Holder++;
			pszString_Start++;
			}
		*pszString_Holder = NULL;
		}
			

	return TRUE;

}


BOOL Text_Read_Database::Remove_Spaces_At_End(char *pszString) {

	UINT ctString_End;


	ctString_End = strlen(pszString);
	while ((ctString_End > 0) &&
			(pszString[ctString_End - 1] == ' '))
		ctString_End--;
	
	// Only set the next character to NULL to mark the end of the string if a character was found,
	// otherwise could go over bounds of string
	if (ctString_End > 0)
		pszString[ctString_End] = NULL;


	return TRUE;

}


