// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 10/04/96

// InitWrkr.CPP
// Member functions for working with intilization files


#include "InitFile.H"

////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File constructor:  Setup all intial parameters, and open file
/////////////////////////////////////////////////////////////////////////////////////
     
Initialization_File::Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access) :
	strstreambuf(2000),
	pszInit_Name(NULL),
	bInit_File_Created(FALSE),
	bWritable(FALSE),
	bBuffer_Status(FALSE),
	pFile_Buffer(NULL),
	pTemp_File_Buffer(NULL)
	{


	if ((pszInit_Name = new char [strlen(pszInitialization_Name) + 1]) != NULL) {
		strcpy(pszInit_Name, pszInitialization_Name);

		// Open the file	                      
		if (bWritable = bWrite_Access)	
			fstream::open(pszInit_Name, ios::in | ios::out | ios::nocreate, filebuf::sh_read);
		else
			fstream::open(pszInit_Name, ios::in | ios::nocreate, filebuf::sh_read | filebuf::sh_write);
		if (fstream::is_open())
			bInit_File_Created = TRUE;

		}
		
		
}


Initialization_File::~Initialization_File(void) {

	// Allow the dynamic buffer to be deleted by me now
    strstreambuf::freeze(0);
	Initialization_File_Delete();
                             
}


void Initialization_File::Initialization_File_Delete(void) {

	// If one created, destroy it
	if (pszInit_Name) {
		delete [] pszInit_Name;
		pszInit_Name = NULL;
		}
	if (Initialization_File_Created())
		Initialization_File_Close();
	if (pFile_Buffer) {
		// Let the buffer delete itself
		pFile_Buffer->freeze(0);
		delete pFile_Buffer;
		pFile_Buffer = NULL;
		}
	if (pTemp_File_Buffer) {
		// Let the buffer delete itself
		pTemp_File_Buffer->freeze(0);
		delete pTemp_File_Buffer;
		pTemp_File_Buffer = NULL;
		}
		
}

/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File status member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Initialization_File::Initialization_File_Created(void) {
 
	return bInit_File_Created; 

}


const char *Initialization_File::Get_Initialization_Name(void) {

	if (Initialization_File_Created())
		return pszInit_Name;
	else return NULL;
}


/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File init open and closer
/////////////////////////////////////////////////////////////////////////////////////

BOOL Initialization_File::Initialization_File_Open(const char *pszInitialization_Name, BOOL bWrite_Access) {

	// If one created, destroy it
	Initialization_File_Delete();
		
	// Create the new one
	if ((pszInit_Name = new char [strlen(pszInitialization_Name) + 1]) == NULL)
		return FALSE;
		
	strcpy(pszInit_Name, pszInitialization_Name);
	
	if (bWritable = bWrite_Access)	
		fstream::open(pszInit_Name, ios::in | ios::out | ios::nocreate, filebuf::sh_read);
	else                                               
		fstream::open(pszInit_Name, ios::in | ios::nocreate, filebuf::sh_read | filebuf::sh_write);
		
	if (fstream::is_open())
		bInit_File_Created = TRUE; 


	return bInit_File_Created;

}

 
BOOL Initialization_File::Initialization_File_Close(void) {

	if (Initialization_File_Created()) {
        // Close down everything
		fstream::close();
		bInit_File_Created = FALSE;
		}
	
	return TRUE;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File init file reading member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Initialization_File::Label_Read(const char *pszLabel, char **pszValue) {

	char *pszLine;
	
	
	if (Initialization_File_Created()) {
		if ((Find_Label(pszLabel, &pszLine)) &&
			(Remove_Spaces_At_Begin(&pszLine)) &&
			(Remove_Spaces_At_End(pszLine))) {
			if (((*pszValue) = new char [strlen(pszLine) + 1]) != NULL) {
				strcpy((*pszValue), pszLine);
				return TRUE;
				}
			}
		}

    
    pszValue = NULL;
	return FALSE;
		
}		
		
		
BOOL Initialization_File::Label_Read(const char *pszLabel, char ***pppszValue, const UINT uiNumber_Values, BOOL bAdd_NULL_At_List_End) {

	char *pszLine, *pszSubstring;
	UINT uiNum_Strings = uiNumber_Values, ctString_Index = 0, ctIndex;
	BOOL bContinue = TRUE;
	
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {

			// Determine the number of strings
			if (uiNum_Strings == UINT_MAX)
				uiNum_Strings = Line_Find_Num_Strings(pszLine);

			// Check if no strings exist
			if (uiNum_Strings == 0) {
				(*pppszValue) = NULL;
				return TRUE;
				}

			// At this point, should be data there, if there was not data at this label then just return		
			if ((pszLine[0] == NULL) || (pszLine == NULL)) {
				(*pppszValue) = NULL;
				return FALSE;
				}

			// Create the list of strings
			if (((bAdd_NULL_At_List_End) && ((*pppszValue = new (char (*[uiNum_Strings+1]))) != NULL)) ||
				((!bAdd_NULL_At_List_End) && ((*pppszValue = new (char (*[uiNum_Strings]))) != NULL))) {
				// Set strings to NULLs
				for (ctIndex = 0; ctIndex < uiNum_Strings; ctIndex++)
					(*pppszValue)[ctIndex] = NULL;
				// Add the values to an array of values
				while ((bContinue) && (ctString_Index < uiNum_Strings)) {
					// Find the next
                   	if (bContinue = Line_Find_Next(&pszSubstring, &pszLine)) {
						// Check that the last string does not contain a comma - does not matter if bContinue set and string copied
						if (ctString_Index == uiNum_Strings - 1)
							bContinue = ((pszLine == NULL) || (pszLine[0] == NULL));
						Remove_Spaces_At_Begin(&pszSubstring);
						Remove_Spaces_At_End(pszSubstring);
						if (((*pppszValue)[ctString_Index] = new char [strlen(pszSubstring) + 1]) == NULL)
							bContinue = FALSE;
						else
							strcpy((*pppszValue)[ctString_Index], pszSubstring);
						ctString_Index++;
						}
					}
				// Add a null as a string at end if requested
				if (bAdd_NULL_At_List_End)
					(*pppszValue)[ctString_Index] = NULL;
	            
				if (!bContinue)
					Delete_2D_List(pppszValue, uiNum_Strings);
					
				return bContinue;
				}
			}
		}

    
    (*pppszValue) = NULL;
	return FALSE;

}


BOOL Initialization_File::Label_Read(const char *pszLabel, UINT *puiValue) {

	char *pszLine;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {
			if (sscanf(pszLine, "%u\0", puiValue))
				return TRUE;
			}
		}

    
    (*puiValue) = 0;
	return FALSE;

}


BOOL Initialization_File::Label_Read(const char *pszLabel, UINT **puiValue, const UINT uiNumber_Values) {

	char *pszLine, *pszSubstring;
	UINT uiNum_Strings = uiNumber_Values, ctString_Index = 0;
	BOOL bContinue = TRUE;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {

			// Determine the number of strings
			if (uiNum_Strings == UINT_MAX)
				uiNum_Strings = Line_Find_Num_Strings(pszLine);
			else uiNum_Strings = uiNumber_Values;

			// Check if no strings exist
			if (uiNum_Strings == 0) {
				(*puiValue) = NULL;
				return TRUE;
				}

			// At this point, should be data there, if there was not data at this label then just return		
			if ((pszLine[0] == NULL) || (pszLine == NULL)) {
				(*puiValue) = NULL;
				return FALSE;
				}

			// Create the list of strings
			if (((*puiValue) = new UINT [uiNum_Strings]) != NULL) {
				// Add the values to an array of values
				while ((bContinue) && (ctString_Index < uiNum_Strings)) {
					// Find the next
                   	if (bContinue = Line_Find_Next(&pszSubstring, &pszLine)) {
						if (!sscanf(pszSubstring, "%u\0", &(*puiValue)[ctString_Index]))
							bContinue = FALSE;
						ctString_Index++;
						}
					}

				if (!bContinue) {
					delete [] (*puiValue);
				    (*puiValue) = NULL;
				    }
					
				return bContinue;
				}
			}
		}

    
    (*puiValue) = NULL;
	return FALSE;
	
}


BOOL Initialization_File::Label_Read(const char *pszLabel, UINT ***pppuiValue, const UINT *puiNumber_Values, const UINT uiNumber_Lists) {

	char *pszLine, *pszSubstring;
	UINT ctList_Index = 0, ctString_Index = 0, ctIndex;
	BOOL bContinue = TRUE;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {

			// Check if no strings exist
			if (uiNumber_Lists == 0) {
				(*pppuiValue) = NULL;
				return TRUE;
				}

			// At this point, should be data there, if there was not data at this label then just return		
			if ((pszLine[0] == NULL) || (pszLine == NULL)) {
				(*pppuiValue) = NULL;
				return FALSE;
				}

			// Create the list of strings
			if (((*pppuiValue) = new (UINT (* ([uiNumber_Lists])))) != NULL) {
				// Set strings to NULLs
				for (ctIndex = 0; ctIndex < uiNumber_Lists; ctIndex++)
					(*pppuiValue)[ctIndex] = NULL;
				// Add the values to an array of values
				while ((bContinue) && (ctList_Index < uiNumber_Lists)) {
					if (((*pppuiValue)[ctList_Index] = new UINT [puiNumber_Values[ctList_Index]]) == NULL)
						bContinue = FALSE;
					ctString_Index = 0;			
					while ((bContinue) && (ctString_Index < puiNumber_Values[ctList_Index])) {
						// Find the next
	                   	if (bContinue = Line_Find_Next(&pszSubstring, &pszLine)) {
							if (!sscanf(pszSubstring, "%u\0", &(*pppuiValue)[ctList_Index][ctString_Index]))
								bContinue = FALSE;
							ctString_Index++;
							}
						}
					ctList_Index++;
					}
					
				if (!bContinue) 
					Delete_2D_List(pppuiValue, uiNumber_Lists);
					
				return bContinue;
				}
			}
		}

    
    (*pppuiValue) = NULL;
	return FALSE;

}


BOOL Initialization_File::Label_Read(const char *pszLabel, int *piValue, BOOL bBoolean_Value) {

	char *pszLine;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {
			if (bBoolean_Value) 
				return Set_Boolean(pszLine, piValue); 
			else if (sscanf(pszLine, "%i", piValue))
				return TRUE;
			}
		}

    
    (*piValue) = 0;
	return FALSE;

}


// Creates and fills an array of boolean by default, int if specified
BOOL Initialization_File::Label_Read(const char *pszLabel, int **ppiValue, const UINT uiNumber_Values, BOOL bBoolean_Value) {

	char *pszLine, *pszSubstring;
	UINT uiNum_Strings = uiNumber_Values, ctString_Index = 0;
	BOOL bContinue = TRUE;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {

			// Determine the number of strings
			if (uiNum_Strings == UINT_MAX)
				uiNum_Strings = Line_Find_Num_Strings(pszLine);
			else uiNum_Strings = uiNumber_Values;

			// Check if no strings exist
			if (uiNum_Strings == 0) {
				(*ppiValue) = NULL;
				return TRUE;
				}

			// At this point, should be data there, if there was not data at this label then just return		
			if ((pszLine[0] == NULL) || (pszLine == NULL)) {
				(*ppiValue) = NULL;
				return FALSE;
				}

			// Create the list of strings
			if (((*ppiValue) = new int [uiNum_Strings]) != NULL) {
				// Add the values to an array of values
				while ((bContinue) && (ctString_Index < uiNum_Strings)) {
					// Find the next
                   	if (bContinue = Line_Find_Next(&pszSubstring, &pszLine)) {
						if (bBoolean_Value) 
							bContinue = Set_Boolean(pszSubstring, &((*ppiValue)[ctString_Index])); 
						else if (!sscanf(pszSubstring, "%i", &((*ppiValue)[ctString_Index])))
							bContinue = FALSE;
						ctString_Index++;
						}
					}
					
	            if (!bContinue) {
	            	delete [] (*ppiValue);
	            	(*ppiValue) = NULL;
	            	}
	            
				return bContinue;
				}
			}
		}

    
    (*ppiValue) = NULL;
	return FALSE;
 
}


BOOL Initialization_File::Label_Read(const char *pszLabel, DWORD *pdValue) {

	char *pszLine;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {
			if (sscanf(pszLine, "%lu", pdValue))
				return TRUE;
			}
		}

    
    (*pdValue) = 0;
	return FALSE;

}


BOOL Initialization_File::Label_Read(const char *pszLabel, float *pfValue) {

	char *pszLine;
	
	
	if (Initialization_File_Created()) {
		if (Find_Label(pszLabel, &pszLine)) {
			if (sscanf(pszLine, "%f", pfValue))
				return TRUE;
			}
		}

    
    (*pfValue) = 0;
	return FALSE;

}


BOOL Initialization_File::Label_Read(const char *pszLabel, GEN_DATE_STRUCT **pValue) {

	UINT *puiValue;
	BOOL bSuccess = FALSE;


	// Y2K - No problem since GEN_DATE_STRUCT takes four digits

	if (Label_Read(pszLabel, &puiValue, 3)) {
		if (((*pValue) = new GEN_DATE_STRUCT) != NULL) {
			(*pValue)->yr = puiValue[0];
			(*pValue)->mon = puiValue[1];
			(*pValue)->day = puiValue[2];
			bSuccess = TRUE;
			}
		delete [] puiValue;
		} 
	else 
	    (*pValue) = NULL;


	return bSuccess;

}


BOOL Initialization_File::Label_Read(const char *pszLabel, GEN_TIME_STRUCT **pValue) {

	UINT *puiValue;
	BOOL bSuccess = FALSE;


	if (Label_Read(pszLabel, &puiValue, 4)) {
		if (((*pValue) = new GEN_TIME_STRUCT) != NULL) {
			(*pValue)->hr = puiValue[0];
			(*pValue)->min = puiValue[1];
			(*pValue)->sec = puiValue[2];
			(*pValue)->msec = puiValue[3];
			bSuccess = TRUE;
			}
		delete [] puiValue;
		}
	else 
	    (*pValue) = NULL;


	return bSuccess;

}


/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File init file changing member functions
/////////////////////////////////////////////////////////////////////////////////////


BOOL Initialization_File::Buffer_Changes(BOOL bStatus) {

	BOOL bBuffer_Saved = FALSE;


	if (bBuffer_Status && bStatus)
		return TRUE;
	if (!bBuffer_Status && !bStatus)
		return TRUE;


	// Start buffering all changes
	if (bStatus) {
		// Create the buffer on the heap so no stack overflow
		if (((pFile_Buffer = new strstreambuf(10000)) == NULL) ||
			((pTemp_File_Buffer = new strstreambuf(10000)) == NULL)) {
			// Be sure that delete occurs if one created
			if (pFile_Buffer) {
				// Let the buffer delete itself
				pFile_Buffer->freeze(0);
				delete pFile_Buffer;
				pFile_Buffer = NULL;
				}
			if (pTemp_File_Buffer) {
				// Let the buffer delete itself
				pTemp_File_Buffer->freeze(0);
				delete pTemp_File_Buffer;
				pTemp_File_Buffer = NULL;
				}
			return FALSE;
			}

		// Make sure to rewind to the begining of the file
		fstream::clear();
		fstream::seekg(0L, ios::beg);

		// Read the entire file into a dynamic buffer
		// Just read into one now, the other is used as a temporary buffer
		fstream::get(*pFile_Buffer, EOF);
		pFile_Buffer->sputc(NULL);

		if (!fstream::good())
			return FALSE;
		else {
			// Only set once know succesful
			bBuffer_Status = bStatus;
			return TRUE;
			}
		}

	// Save all of the changes to disk
	else {
		// Clear the old file
		fstream::close();
		bInit_File_Created = FALSE;
		fstream::open(pszInit_Name, ios::out | ios::nocreate | ios::trunc, filebuf::sh_none);
		if (fstream::is_open()) {
			bInit_File_Created = TRUE;

			// Output the vaild information up to the new data
			fstream::write(pFile_Buffer->str(), strlen(pFile_Buffer->str()));
			fstream::flush();
			// Check to see if success
			if (fstream::good())
				bBuffer_Saved = TRUE;
	   	

			// Reopen normally
			fstream::close();
			if (bWritable)	
				fstream::open(pszInit_Name, ios::in | ios::out | ios::nocreate, filebuf::sh_read);
			else
				fstream::open(pszInit_Name, ios::in | ios::nocreate, filebuf::sh_read | filebuf::sh_write);
		
			if (fstream::is_open())
				bInit_File_Created = TRUE;
				
			}

		// Let the buffer delete itself
		pFile_Buffer->freeze(0);
		delete pFile_Buffer;
		pFile_Buffer = NULL;
		// Let the buffer delete itself
		pTemp_File_Buffer->freeze(0);
		delete pTemp_File_Buffer;
		pTemp_File_Buffer = NULL;


		// Always set back to no buffer even if unsuccessful
		bBuffer_Status = bStatus;


		return bBuffer_Saved;
		}

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const char *pszValue) {

	if (!Initialization_File_Created() || !bWritable)
		return FALSE;
	else
		return Save_Label(pszLabel, pszValue);
        
        
  	return FALSE;
  	
}


BOOL Initialization_File::Label_Change(const char *pszLabel, const char **ppszValue, const UINT uiNumber_Values) {

	UINT ctIndex, uiList_Length = 0;
	BOOL bSuccess = FALSE;
	char *pszValue;
	

	if (!Initialization_File_Created() || !bWritable)
		return FALSE;

	if (uiNumber_Values == 0)
		return TRUE;

	// Figure out list length   		
	for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++)
		uiList_Length += (strlen(ppszValue[ctIndex]) + 2);
			
	if ((pszValue = new char [uiList_Length + 1]) != NULL) {

		// Build the string of values
		for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++) {
   			if (ctIndex == 0)
				strcpy(pszValue, ppszValue[ctIndex]);
			else {
				strcat(pszValue, ", ");
				strcat(pszValue, ppszValue[ctIndex]);
				}
			}

		// Save the string			
		bSuccess = Save_Label(pszLabel, pszValue);
		delete [] pszValue;
		}

	
	return bSuccess;
	
}


BOOL Initialization_File::Label_Change(const char *pszLabel, const UINT uiValue) {

	char szTemp[25];


	if (!Initialization_File_Created() || !bWritable)
		return FALSE;


   	sprintf(szTemp, "%u", uiValue);
	if (Initialization_File_Created() && bWritable)
		return Save_Label(pszLabel, szTemp);

  	return FALSE;

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const UINT *puiValue, const UINT uiNumber_Values) {

	UINT ctIndex, uiList_Length = 0;
	BOOL bSuccess = FALSE;
	char szTemp[25];
	char *pszValue;
	
	
	if (!Initialization_File_Created() || !bWritable)
		return FALSE;

	if (uiNumber_Values == 0)
		return TRUE;

	// Figure out list length   		
	for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++) {
	   	sprintf(szTemp, "%u", puiValue[ctIndex]);
		uiList_Length += (strlen(szTemp) + 2);
		}
	

 	if ((pszValue = new char [uiList_Length + 1]) != NULL) {

		// Build the string of values
	 	for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++) {
		   	sprintf(szTemp, "%u", puiValue[ctIndex]);
   			if (ctIndex == 0)
				strcpy(pszValue, szTemp);
			else {
				strcat(pszValue, ", ");
				strcat(pszValue, szTemp);
				}
			}
			
		// Save the string			
		bSuccess = Save_Label(pszLabel, pszValue);
		delete [] pszValue;
		}

	
	return bSuccess;

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const UINT **ppuiValue, const UINT *puiNumber_Values, const UINT uiNumber_Lists) {

	UINT ctList, ctIndex, uiList_Length = 0;
	char szTemp[25];
	char *pszValue;
	BOOL bSuccess = FALSE;
	
	
	if (!Initialization_File_Created() || !bWritable)
		return FALSE;

	if (uiNumber_Lists == 0)
		return TRUE;


	// Figure out list length   		
	for (ctList = 0; ctList < uiNumber_Lists; ctList++) {
		for (ctIndex = 0; ctIndex < puiNumber_Values[ctList]; ctIndex++) {
		   	sprintf(szTemp, "%u", ppuiValue[ctList][ctIndex]);
			uiList_Length += (strlen(szTemp) + 2);
			}
		}
		
 	if ((pszValue = new char [uiList_Length + 1]) != NULL) {

		// Build the string of values
		for (ctList = 0; ctList < uiNumber_Lists; ctList++) {
			for (ctIndex = 0; ctIndex < puiNumber_Values[ctList]; ctIndex++) {
			   	sprintf(szTemp, "%u", ppuiValue[ctList][ctIndex]);
	   			if ((ctList == 0) && (ctIndex == 0)) 
					strcpy(pszValue, szTemp);
				else {
					strcat(pszValue, ", ");
					strcat(pszValue, szTemp);
					}
				}
			}
			
		// Save the string			
		bSuccess = Save_Label(pszLabel, pszValue);
		delete [] pszValue;
		}

	
	return bSuccess;

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const DWORD dValue) {

	char szTemp[25];


	if (!Initialization_File_Created() || !bWritable)
		return FALSE;


   	sprintf(szTemp, "%lu", dValue);
	if (Initialization_File_Created() && bWritable)
		return Save_Label(pszLabel, szTemp);

  	return FALSE;

}

                                
BOOL Initialization_File::Label_Change(const char *pszLabel, const float fValue) {

	char szTemp[32];


	if (!Initialization_File_Created() || !bWritable)
		return FALSE;


   	sprintf(szTemp, "%f", fValue);
	if (Initialization_File_Created() && bWritable)
		return Save_Label(pszLabel, szTemp);

  	return FALSE;

}

                                
BOOL Initialization_File::Label_Change(const char *pszLabel, const int iValue, BOOL bBoolean_Value) {

	char szTemp[25];


	if (!Initialization_File_Created() || !bWritable)
		return FALSE;


   	if (bBoolean_Value) {
   		if (iValue == TRUE)
   			strcpy(szTemp, "TRUE");
   		else // Could have been set to some other false value
   			strcpy(szTemp, "FALSE");
   		}
   	else
   		sprintf(szTemp, "%i", iValue);
	   	
	return Save_Label(pszLabel, szTemp);

	
	return TRUE;

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const int *piValue, const UINT uiNumber_Values, BOOL bBoolean_Value) {

	UINT ctIndex, uiList_Length = 0;
	BOOL bSuccess = FALSE;
	char *pszValue;
	char szTemp[25];
	

	if (!Initialization_File_Created() || !bWritable)
		return FALSE;

	if (uiNumber_Values == 0)
		return TRUE;


	// Figure out list length
	if (bBoolean_Value) 
		uiList_Length = uiNumber_Values * strlen("FALSE, ");
	else {
		for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++) {
		   	sprintf(szTemp, "%i", piValue[ctIndex]);
			uiList_Length += (strlen(szTemp) + 2);
			}
		}
			
	if ((pszValue = new char [uiList_Length + 1]) != NULL) {

		// Build the string of values
		strcpy(pszValue, "\0");
		if (bBoolean_Value) {
			for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++) {
	   			if (ctIndex != 0)
					strcat(pszValue, ", ");
   				if (piValue[ctIndex]) 
   					strcat(pszValue, "TRUE");
				else strcat(pszValue, "FALSE");
				}
			}
		else {
			for (ctIndex = 0; ctIndex < uiNumber_Values; ctIndex++) {
			   	sprintf(szTemp, "%i", piValue[ctIndex]);
	   			if (ctIndex != 0)
					strcat(pszValue, ", ");
				strcat(pszValue, szTemp);
				}
			}

		// Save the string			
		bSuccess = Save_Label(pszLabel, pszValue);
		delete [] pszValue;
		}

	
	return bSuccess;

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const GEN_DATE_STRUCT *pValue) {

	UINT puiValue[3];

	
	// Y2K - No problem since GEN_DATE_STRUCT takes four digits

	if (!Initialization_File_Created() || !bWritable)
		return FALSE;


	puiValue[0] = pValue->yr;       
	puiValue[1] = pValue->mon;
	puiValue[2] = pValue->day;
	
	return Label_Change(pszLabel, puiValue, 3);

}


BOOL Initialization_File::Label_Change(const char *pszLabel, const GEN_TIME_STRUCT *pValue) {

	UINT puiValue[4];
	

	if (!Initialization_File_Created() || !bWritable)
		return FALSE;


	puiValue[0] = pValue->hr;       
	puiValue[1] = pValue->min;
	puiValue[2] = pValue->sec;
	puiValue[3] = pValue->msec;
	
	return Label_Change(pszLabel, puiValue, 4);

}


/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File init file changing member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Initialization_File::Save_Label(const char *pszLabel, const char *pszValue) {

	char *pszLabel_Location, *pszActual_Label = NULL;
	BOOL bFound = FALSE, bLabel_Changed = FALSE;
	strstreambuf *pBuffer_Switch;
	BOOL bTab_Delimited = FALSE, bSpace_Delimited = FALSE, bEqual_Delimited = FALSE;


	// Create the actual label to search for, one that starts on a newline
	if ((pszActual_Label = new char [1 + strlen(pszLabel) + 1]) == NULL)
		return FALSE;
	strcpy(pszActual_Label, "\n");
	strcat(pszActual_Label, pszLabel);


	// If do not want to buffer then create the proper output file buffer
	if (!bBuffer_Status) {
		// Create the buffer on the heap so no stack overflow
		// The 10000 is only an initial recommended value, it will grow if needed
		if ((pFile_Buffer = new strstreambuf(10000)) == NULL) {
			if (pszActual_Label) delete [] pszActual_Label;
			return FALSE;
			}

		// Make sure to rewind to the begining of the file
		fstream::clear();
		fstream::seekg(0L, ios::beg);

		// Read the entire file into a dynamic buffer
		fstream::get(*pFile_Buffer, EOF);
		pFile_Buffer->sputc(NULL);
    
		// Determine if label exists
		if (fstream::good()) {
    		pszLabel_Location = pFile_Buffer->str();
			while ((!bFound) && (pszLabel_Location != NULL)) {
				// Will bottom out when pszLabel_Location set to NULL below
				if ((pszLabel_Location = strstr(pszLabel_Location, pszActual_Label)) != NULL) {
					// Verify that something is left
		    		if (strlen(pszLabel_Location) > strlen(pszActual_Label)) {
						// Next character MUST be either a space, tab, or equal otherwise could only be part of a label
						// Example: MY_LABEL and MY_LABEL_AGAIN
						// Next character should be a tab, however will overwrite any of the following cases:
						// 1. No tab exists
						// 2. Several tabs exist
						// 3. An equal exists
						// 4. Several equals exist
						if (pszLabel_Location[strlen(pszActual_Label)] == ' ') { // Not strlen(pszActual_Label) + 1 since start at 0
							bSpace_Delimited = TRUE;
							bFound = TRUE;
							}
						else if (pszLabel_Location[strlen(pszActual_Label)] == '\t') { // Not strlen(pszLabel) + 1 since start at 0
							bTab_Delimited = TRUE;
							bFound = TRUE;
							}
						else if (pszLabel_Location[strlen(pszActual_Label)] == '=') { // Not strlen(pszActual_Label) + 1 since start at 0
							bEqual_Delimited = TRUE;
							bFound = TRUE;
							}
						// Set it to find the next possible label
		    			else pszLabel_Location = &(pszLabel_Location[1]);
						}
		    		}
				}
			}
    			
		if (bFound) {
			// Clear the old file
			fstream::close();
			bInit_File_Created = FALSE;
			fstream::open(pszInit_Name, ios::out | ios::nocreate | ios::trunc, filebuf::sh_none);
			if (fstream::is_open()) {
				bInit_File_Created = TRUE;

				// Mark the label location
				*pszLabel_Location = NULL;
				// Output the vaild information up to the new data
				fstream::write(pFile_Buffer->str(), strlen(pFile_Buffer->str()));
				// Output the new data
				fstream::write(pszActual_Label, strlen(pszActual_Label));
				if (bSpace_Delimited)
					fstream::put(' ');
				else if (bTab_Delimited)
					fstream::put('\t');
				else if (bEqual_Delimited)
					fstream::put('=');
				fstream::write(pszValue, strlen(pszValue));
				// Output the vaild information after the new data
				if ((pszLabel_Location = strchr(&pszLabel_Location[1], '\n')) != NULL)
					fstream::write(pszLabel_Location, strlen(pszLabel_Location));
				fstream::flush();
				// Check to see if success
				if (fstream::good())
					bLabel_Changed = TRUE;
	   		

				// Reopen normally
				fstream::close();
				if (bWritable)	
					fstream::open(pszInit_Name, ios::in | ios::out | ios::nocreate, filebuf::sh_read);
				else
					fstream::open(pszInit_Name, ios::in | ios::nocreate, filebuf::sh_read | filebuf::sh_write);
			
				if (fstream::is_open())
					bInit_File_Created = TRUE;
					
				}
			}

		// Let the buffer delete itself
		pFile_Buffer->freeze(0);
		delete pFile_Buffer;
		pFile_Buffer = NULL;

		if (pszActual_Label) delete [] pszActual_Label;
	    		
		return (bLabel_Changed && bInit_File_Created);
		}

	// Write to buffer
	else {
		// Determine if label exists
    	pszLabel_Location = pFile_Buffer->str();
		// While used for repeating if two labels with similar names found
		while ((!bFound) && (pszLabel_Location != NULL)) {
			// Will bottom out when pszLabel_Location set to NULL below
			if ((pszLabel_Location = strstr(pszLabel_Location, pszActual_Label)) != NULL) {
				// Verify that something is left
		    	if (strlen(pszLabel_Location) > strlen(pszActual_Label)) {
					// Next character MUST be either a space, tab, or equal otherwise could only be part of a label
					// Example: MY_LABEL and MY_LABEL_AGAIN
					// Next character should be a tab, however will overwrite any of the following cases:
					// 1. No tab exists
					// 2. Several tabs exist
					// 3. An equal exists
					// 4. Several equals exist
					if (pszLabel_Location[strlen(pszActual_Label)] == ' ') { // Not strlen(pszActual_Label) + 1 since start at 0
						bSpace_Delimited = TRUE;
						bFound = TRUE;
						}
					else if (pszLabel_Location[strlen(pszActual_Label)] == '\t') { // Not strlen(pszActual_Label) + 1 since start at 0
						bTab_Delimited = TRUE;
						bFound = TRUE;
						}
					else if (pszLabel_Location[strlen(pszActual_Label)] == '=') { // Not strlen(pszActual_Label) + 1 since start at 0
						bEqual_Delimited = TRUE;
						bFound = TRUE;
						}
					// Set it to find the next possible label
		    		else pszLabel_Location = &(pszLabel_Location[1]);
					}
		    	}
			}
    			
		if (bFound) {
			// Mark the label location
			*pszLabel_Location = NULL;

			// Assume will be successful
			bLabel_Changed = TRUE;

			// Rewind the temporary buffer back to the begining
			bLabel_Changed = bLabel_Changed && (0L == pTemp_File_Buffer->seekpos(0L));
			// Copy the current buffer to the temporary up to the label
			bLabel_Changed = bLabel_Changed && (strlen(pFile_Buffer->str()) == (size_t)pTemp_File_Buffer->sputn(pFile_Buffer->str(), strlen(pFile_Buffer->str())));

			// Output the new data
			bLabel_Changed = bLabel_Changed && (strlen(pszActual_Label) == (size_t)pTemp_File_Buffer->sputn(pszActual_Label, strlen(pszActual_Label)));
			if (bSpace_Delimited)
				bLabel_Changed = bLabel_Changed && (EOF != pTemp_File_Buffer->sputc(' '));
			else if (bTab_Delimited)
				bLabel_Changed = bLabel_Changed && (EOF != pTemp_File_Buffer->sputc('\t'));
			else if (bEqual_Delimited)
				bLabel_Changed = bLabel_Changed && (EOF != pTemp_File_Buffer->sputc('='));
			bLabel_Changed = bLabel_Changed && (strlen(pszValue) == (size_t)pTemp_File_Buffer->sputn(pszValue, strlen(pszValue)));
			
			// Output the vaild information after the new data
			if ((pszLabel_Location = strchr(&pszLabel_Location[1], '\n')) != NULL)
				bLabel_Changed = bLabel_Changed && (strlen(pszLabel_Location) == (size_t)pTemp_File_Buffer->sputn(pszLabel_Location, strlen(pszLabel_Location)));

			// Add the NULL at the end
			bLabel_Changed = bLabel_Changed && (EOF != pTemp_File_Buffer->sputc('\0'));

			// Switch the buffers
			pBuffer_Switch = pFile_Buffer;
			pFile_Buffer = pTemp_File_Buffer;
			pTemp_File_Buffer = pBuffer_Switch;
			}


		if (pszActual_Label) delete [] pszActual_Label;

		return (bLabel_Changed);
		}

}


BOOL Initialization_File::Initialization_Save_As(const char *pszInitialization_Name, BOOL bWrite_Access) {

	strstreambuf *pFile_Buffer;
	BOOL bCopy_Success = FALSE;
	

	if (!Initialization_File_Created() || !bWritable)
		return FALSE;

	// Create the buffer on the heap so no stack overflow
	if ((pFile_Buffer = new strstreambuf) == NULL)
		return FALSE;


	// Read the entire file into a dynamic buffer
	fstream::clear();
	fstream::seekg(0L, ios::beg);
    fstream::get(*pFile_Buffer, EOF);
	pFile_Buffer->sputc(NULL);
    if (fstream::good()) {

	    // Open and clear the new file
		fstream::close();
		bInit_File_Created = FALSE;
		fstream::open(pszInitialization_Name, ios::out | ios::trunc, filebuf::sh_none);
		if (fstream::is_open())  {
			bInit_File_Created = TRUE;
	
			// Write the entire old file over to new one
		    fstream::write(pFile_Buffer->str(), strlen(pFile_Buffer->str()));
		    fstream::flush();
		    // Check to see if success
		    if (fstream::good())
			    bCopy_Success = TRUE;
			}
		}
	        
	// Let the buffer delete itself
	pFile_Buffer->freeze(0);
	delete pFile_Buffer;
	   	
	// Reopen normally
	Initialization_File_Open(pszInitialization_Name, bWritable);
	                         
	    	
	return (bCopy_Success && bInit_File_Created);

}


/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File copying and deleting helpers
/////////////////////////////////////////////////////////////////////////////////////

BOOL Initialization_File::Copy_2D_List(char ***pDestination, const UINT uiDestination_Length, const char **ppSource, const UINT uiSource_Length) {

	UINT ctIndex = 0;
	BOOL bSuccess = TRUE;


	SEB_COPYLISTSTRING((*pDestination), uiDestination_Length, ppSource, uiSource_Length, ctIndex);
	
	if ((*pDestination) == NULL)
		bSuccess = FALSE;

	
	return bSuccess;

}


BOOL Initialization_File::Copy_2D_List(UINT **pDestination, const UINT uiDestination_Length, const UINT *pSource, const UINT uiSource_Length) {

	UINT ctIndex;
	BOOL bSuccess = TRUE;
	

	SEB_COPYLISTUINT((*pDestination), pSource, uiSource_Length, ctIndex);

	if ((*pDestination) == NULL)
		bSuccess = FALSE;

	
	return bSuccess;

}


void Initialization_File::Delete_2D_List(char ***pSource, const UINT uiLength) {

	UINT ctIndex = 0;
 
 
	SEB_DELETELISTSTRING((*pSource), uiLength, ctIndex);

}


void Initialization_File::Delete_2D_List(UINT ***pSource, const UINT uiLength) {

	UINT ctIndex = 0;
 
 
	SEB_DELETELISTLISTSUINT((*pSource), uiLength, ctIndex);

}


/////////////////////////////////////////////////////////////////////////////////////
//  Initialization_File misc. procedures for prcessing the file
/////////////////////////////////////////////////////////////////////////////////////

BOOL Initialization_File::Find_Label(const char *pszLabel, char **ppszLine) {
	
	// NOTE: This member function accepts a char pointer which it points to
	//	it's internal strstreambuf.  This strstreambuf or pszLine does not
	//	ever need to get deleted until the object is deleted.

		
	BOOL bLabel_Found = FALSE;


	// Make sure to rewind to the begining of the file
	fstream::clear();
	fstream::seekg(0L, ios::beg);
	
	// Search for the label			
	while (!fstream::eof() && !bLabel_Found) {
		// NOTE: I am a strstreambuf therefore I don't need to delete pszLine anywhere
		strstreambuf::freeze(0);
    	strstreambuf::pbump(-1 * strstreambuf::out_waiting());
		fstream::get(*this);
		strstreambuf::sputc(NULL);
		// If line commented out, then compare won't work, therefore comments work
		if (strncmp(strstreambuf::str(), pszLabel, strlen(pszLabel)) == 0) {
			// Next character MUST be either a space, tab, or equal otherwise could only be part of a label
			// Example: MY_LABEL and MY_LABEL_AGAIN
			if ((((strstreambuf::str())[strlen(pszLabel)]) == ' ') || // Not strlen(pszLabel) + 1 since start at 0
				(((strstreambuf::str())[strlen(pszLabel)]) == '\t') || // Not strlen(pszLabel) + 1 since start at 0
				(((strstreambuf::str())[strlen(pszLabel)]) == '=')) { // Not strlen(pszLabel) + 1 since start at 0
				bLabel_Found = TRUE;
				// Next character should be a tab, however handle the cases where:
				// 1. No tab exists
				//    OLD -> if (((strstreambuf::str())[strlen(pszLabel)] == '\t')) {
				// 2. Several tabs exist
				// 3. An equal exists
				// 3. Several equals exist
				// Be sure that next character is a tab, but data does not have to be their
				(*ppszLine) = &((strstreambuf::str())[strlen(pszLabel)]);
				while (((**ppszLine) == '\t') ||
						((**ppszLine) == '='))
					(*ppszLine) = &((*ppszLine)[1]);
				}
			}
		// Get the newline
		fstream::get();
	    } 


	return bLabel_Found;

}


UINT Initialization_File::Line_Find_Num_Strings(const char *pszLine) {

	UINT uiNum_Strings = 1;
	char *pszCurrent_String = (char *)pszLine;


	while (pszCurrent_String = strchr(pszCurrent_String, ',')) {
		uiNum_Strings++;
		pszCurrent_String++;
		}
		                 
		                 
	return uiNum_Strings;

}


void Initialization_File::Line_Replace_Commas(char *pszLine) {

	char *pszCurrent_String = pszLine; 


	// Mark the end of each substring
	while (pszCurrent_String = strchr(pszCurrent_String, ',')) {
		*pszCurrent_String = NULL;
		pszCurrent_String++;
		}

}


BOOL Initialization_File::Line_Find_Next(char **ppszSubstring, char **ppszLine) {

	char *pszCurrent_Substring;


	// If at the NULL then return FALSE
	if (((*ppszLine) == NULL) || (*ppszLine[0] == NULL))
		return FALSE;


    (*ppszSubstring) = (*ppszLine);

	// Determine if this is the last string or not
	if (pszCurrent_Substring = strchr((*ppszLine), ',')) {

		// Add a NULL where the comma is and move the line pointer
		*pszCurrent_Substring = NULL;
		(*ppszLine) = ++pszCurrent_Substring;
		}

	// Last sting, move the line pointer to the NULL
	else {
		(*ppszLine) = &((*ppszLine)[strlen(*ppszLine)]);
		}


	// Remove any leading spaces
	while (**ppszSubstring == ' ')
		(*ppszSubstring)++;


	return TRUE;

}


BOOL Initialization_File::Set_Boolean(char *pszBoolean_String, BOOL *pbValue) {

	Remove_Spaces_At_Begin(&pszBoolean_String);
	// Use strncmp to get rid of any spaces at the end
	if (strnicmp(pszBoolean_String, "TRUE", 4) == 0) {
		(*pbValue) = TRUE;
		return TRUE;
		}
	else if (strnicmp(pszBoolean_String, "FALSE", 5) == 0) {
		(*pbValue) = FALSE;
		return TRUE;
		}
		
	(*pbValue) = FALSE;
	return FALSE;
		
}


BOOL Initialization_File::Remove_Spaces_At_Begin(char **ppszString) {

	while (**ppszString == ' ')
		(*ppszString)++;


	return TRUE;

}


BOOL Initialization_File::Remove_Spaces_At_End(char *pszString) {

	UINT ctString_Position;


	ctString_Position = strlen(pszString);
	while ((ctString_Position > 0) &&
			(pszString[ctString_Position - 1] == ' '))
		ctString_Position--;
	
	// Only set the next character to NULL to mark the end of the string if a character was found,
	// otherwise could go over bounds of string
	if (ctString_Position > 0)
		pszString[ctString_Position] = NULL;


	return TRUE;

}


BOOL Initialization_File::No_Commas_In_String(const char *pszString) {

	if (strchr(pszString, ','))
		return FALSE;


	return TRUE;

}

