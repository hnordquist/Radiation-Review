// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/12/97

// InitFile.H
// Class declaration for working with initialization files


#ifndef INITFILE_H

	#define INITFILE_H

	#include <stdlib.h> 
	#include <fstream.h>
	#include <iostream.h>
	#include <strstrea.h>
	#include <string.h>
	#include <stdio.h>
	#include <limits.h>
	#include <windows.h>
	
	#include "gen.h"

	#include "SEBMacro.H"



                     
	class Initialization_File : public fstream, private strstreambuf {

		public:
		
			Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE); 
			virtual ~Initialization_File(void);
			                        

			// Call to check if the file was open properly and is ready
			BOOL Initialization_File_Created(void); 
			const char *Get_Initialization_Name(void); // Returns NULL if file not open


			// Open the init file and get and save all future data using it
			BOOL Initialization_File_Open(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE);
			// Close the current Initialization file
			BOOL Initialization_File_Close(void);
			// Save the init file and the future changes made using the above member 
			// procedures under new name (and open)
			BOOL Initialization_Save_As(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE);


			// NOTE FOR ALL LABEL READS BELOW:
			//    ALL POINTER VALUES ARE PASS BY REFERENCE
			//    WILL AUTOMATICALLY ALLOCATE ALL VALUES EXCEPT SINGLE
			//        POINTERS WHICH ARE ASSUMED TO BE ALREADY ALLOCATED
			//    RESPONSIBILITY IS LEFT WITH THE USER TO DELETE THE VALUES
			// Creates and fills a character array which is a comma separated string of values
			BOOL Label_Read(const char *pszLabel, char **ppszValue);
			// Creates and fills a 2D array of characters which is a list of string values - Option to add a Null string at end of list
			// NOTE: uiNumber_Values does not count the NULL if added at the end
			// NOTE: Will check the last string for commas in order to determine if too many strings listed
			BOOL Label_Read(const char *pszLabel, char ***pppszValue, const UINT uiNumber_Values = UINT_MAX, BOOL bAdd_NULL_At_List_End = FALSE);
			// Sets an unsigned integer
			BOOL Label_Read(const char *pszLabel, UINT *puiValue);
			// Creates and fills an integer array
			BOOL Label_Read(const char *pszLabel, UINT **ppuiValue, const UINT uiNumber_Values = UINT_MAX);
			// Creates and fills an array of integer array
			BOOL Label_Read(const char *pszLabel, UINT ***pppuiValue, const UINT *puiNumber_Values, const UINT uiNumber_List);
			// Sets a boolean by default, int if specified
			BOOL Label_Read(const char *pszLabel, int *piValue, BOOL bBoolean_Value = TRUE);
			// Creates and fills an array of boolean by default, int if specified
			BOOL Label_Read(const char *pszLabel, int **ppiValue, const UINT uiNumber_Values = UINT_MAX, BOOL bBoolean_Value = TRUE);
			// Sets DWORD
			BOOL Label_Read(const char *pszLabel, DWORD *pdValue);
			// Sets float
			BOOL Label_Read(const char *pszLabel, float *pfValue);
			// Creates and sets GEN_DATE_STRUCT - in C++ structs pass by value
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			BOOL Label_Read(const char *pszLabel, GEN_DATE_STRUCT **pValue);
			// Creates and sets GEN_TIME_STRUCT - in C++ structs pass by value
			BOOL Label_Read(const char *pszLabel, GEN_TIME_STRUCT **pValue);
			

			// Associated label changes
			// NOTE: Change made immediately if Buffer_Changes is set to FALSE
			// NOTE: Changes are buffered in memory if Buffer_Changes is set to TRUE
			// NOTE: Changes being kept in memory are dumped to file when
			//       Buffer_Changes set to FALSE, any other action results
			//       in losing buffer
			BOOL Buffer_Changes(BOOL bStatus);
			BOOL Label_Change(const char *pszLabel, const char *pszValue);
			BOOL Label_Change(const char *pszLabel, const char **ppszValue, const UINT uiNumber_Values);
			BOOL Label_Change(const char *pszLabel, const UINT uiValue);
			BOOL Label_Change(const char *pszLabel, const UINT *puiValue, const UINT uiNumber_Values);
			BOOL Label_Change(const char *pszLabel, const UINT **ppuiValue, const UINT *puiNumber_Values, const UINT uiNumber_List);
			BOOL Label_Change(const char *pszLabel, const DWORD dValue);
			BOOL Label_Change(const char *pszLabel, const float fValue);
			BOOL Label_Change(const char *pszLabel, const int iValue, BOOL bBoolean_Value = TRUE);
			BOOL Label_Change(const char *pszLabel, const int *piValue, const UINT uiNumber_Values, BOOL bBoolean_Value = TRUE);
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			BOOL Label_Change(const char *pszLabel, const GEN_DATE_STRUCT *pValue);
			BOOL Label_Change(const char *pszLabel, const GEN_TIME_STRUCT *pValue);

			
			
			// COPYING AND DELETING HELPERS
			// Note: Expects the address of the destination pointer to be passed - Creates the copy for you
			BOOL Copy_2D_List(char ***pDestination, const UINT uiDestination_Length, const char ** pSource, const UINT uiSoure_Length);
			BOOL Copy_2D_List(UINT **pDestination, const UINT uiDestination_Length, const UINT *pSource, const UINT uiSource_Length);
			// Note: Expects the address of the source pointer to be passed
			//       Does automatic checking to see if not null
			void Delete_2D_List(char ***pSource, const UINT uiLength);
			void Delete_2D_List(UINT ***pSource, const UINT uiLength);


		
		protected:

		    fstream *pInit_File;

		    char *pszInit_Name;

			BOOL bInit_File_Created, bWritable;


			// Misc. member functions used to process the file
			BOOL Find_Label(const char *pszLabel, char **ppszLine);
			UINT Line_Find_Num_Strings(const char *pszLine);
			void Line_Replace_Commas(char *pszLine);
			BOOL Line_Find_Next(char **ppszSubstring, char **ppszLine);
			BOOL Set_Boolean(char *pszBoolean_String, BOOL *pbValue);
			BOOL Save_Label(const char *pszLabel, const char *pszValue);
			BOOL Remove_Spaces_At_Begin(char **ppszString);
			BOOL Remove_Spaces_At_End(char *pszString);
			BOOL No_Commas_In_String(const char *pszString);



		private:

		    BOOL bBuffer_Status;
			strstreambuf *pFile_Buffer, *pTemp_File_Buffer;

			void Initialization_File_Delete(void);

		    
		};
		
#endif

