// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 8/20/96

// MACROS.H
// This file contains macros which I have found useful


#include <time.h>


#ifndef MACROS_H

	#define MACROS_H                      

	
	///////////////////////////////////////////////////////////////////////////
	// Simple max macro
	///////////////////////////////////////////////////////////////////////////
	#define SEB_MAX(a, b) (a > b ? a : b)

                         
	///////////////////////////////////////////////////////////////////////////
	// Simple tight loop waiting macro
	///////////////////////////////////////////////////////////////////////////
	#define WAIT(START, FINISH, WAIT_TIME)						\
					time(&START);								\
					time(&FINISH);								\
   					while (difftime(FINISH, START) < WAIT_TIME)	\
   						{time(&FINISH);};						
 
 
	///////////////////////////////////////////////////////////////////////////
	// These MACROs are a clean compilation fix for the timer problem
	// Be sure to define/undefine COMPILE_WITH_TIMER appropriately and
	//    BEFORE INCLUDING THIS HEADER FILE!!!!!!!!!!!!!!!!!!!!
	///////////////////////////////////////////////////////////////////////////
	#ifdef COMPILE_WITH_TIMER
		#define REGISTER_TIMER(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6)					\
				    CGUI_App::RegisterTimerFn(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6);
		
		#define REMOVE_TIMER(PARAM1, PARAM2)					\
					CGUI_App::RemoveTimerFn(PARAM1, PARAM2);
		
		#define ACTIVATE_TIMER(PARAM1, PARAM2, PARAM3, PARAM4)					\
			    	CGUI_App::ActivateTimerFn(PARAM1, PARAM2, PARAM3, PARAM4);
	#endif		 
	
	#ifndef COMPILE_WITH_TIMER
		#define REGISTER_TIMER(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6) {};
		#define REMOVE_TIMER(PARAM1, PARAM2) {};
		#define ACTIVATE_TIMER(PARAM1, PARAM2, PARAM3, PARAM4) {};
	#endif
	
	
	///////////////////////////////////////////////////////////////////////////
	// These macros can be used to copy and delete various lists
	// NOTE: These are LARGE macros, use them carefully!
	//		 All macros must have an index passed
	/////////////////////////////////////////////////////////////////////////// 
	
	// Deletes a list of strings
	#define SEB_DELETELISTSTRING(ppszSEB_DESTINATION, uiSEB_DESTINATIONLENGTH, ctSEB_INDEX)				\
		if (ppszSEB_DESTINATION) {														\
			for (ctSEB_INDEX = 0; ctSEB_INDEX < uiSEB_DESTINATIONLENGTH; ctSEB_INDEX++) {	\
				if (ppszSEB_DESTINATION[ctSEB_INDEX])									\
					delete [] ppszSEB_DESTINATION[ctSEB_INDEX];							\
				}																			\
			delete [] ppszSEB_DESTINATION;												\
			ppszSEB_DESTINATION = NULL;													\
			}


	// Deletes a list of strings - Expects a NULL as list string to know when to stop deleting
	#define SEB_DELETELISTSTRINGNULL(ppszSEB_DESTINATION, ctSEB_INDEX)			\
		{																\
		ctSEB_INDEX	= 0;												\
		if (ppszSEB_DESTINATION) {									\
			while (ppszSEB_DESTINATION[ctSEB_INDEX]) {				\
				delete [] ppszSEB_DESTINATION[ctSEB_INDEX];			\
				ctSEB_INDEX++;										\
				}													\
			delete [] ppszSEB_DESTINATION;							\
			ppszSEB_DESTINATION = NULL;								\
			}														\
		}


	// Deletes a list of lists of UINT's
	// NOTE: Not the same as UINT copy above
	#define SEB_DELETELISTLISTSUINT(ppuiSEB_DESTINATION, uiSEB_DESTINATIONLENGTH, ctSEB_INDEX)			\
		if (ppuiSEB_DESTINATION) {														\
			for (ctSEB_INDEX = 0; ctSEB_INDEX < uiSEB_DESTINATIONLENGTH; ctSEB_INDEX++) {	\
				if (ppuiSEB_DESTINATION[ctSEB_INDEX])									\
					delete [] ppuiSEB_DESTINATION[ctSEB_INDEX];							\
				}																			\
			delete [] ppuiSEB_DESTINATION;												\
			ppuiSEB_DESTINATION = NULL;													\
			}


	// Copies (and creates if necessary) a list of strings to another
	// If fails then deletes and returns destination as NULL
	#define SEB_COPYLISTSTRING(ppszSEB_DESTINATION, uiSEB_DESTINATIONLENGTH, ppszSEB_SOURCE, uiSEB_SOURCELENGTH, ctSEB_INDEX)					\
			SEB_DELETELISTSTRING(ppszSEB_DESTINATION, uiSEB_DESTINATIONLENGTH, ctSEB_INDEX);                                                        \
			ctSEB_INDEX	= 0;																										\
			ppszSEB_DESTINATION = new char * [uiSEB_SOURCELENGTH];                                        \
			if (ppszSEB_DESTINATION != NULL) {                                        \
				while ((ctSEB_INDEX < uiSEB_SOURCELENGTH) &&                                                                        \
						((ppszSEB_DESTINATION[ctSEB_INDEX] = new char [strlen(ppszSEB_SOURCE[ctSEB_INDEX]) + 1]) != NULL)) {    \
					strcpy(ppszSEB_DESTINATION[ctSEB_INDEX], ppszSEB_SOURCE[ctSEB_INDEX]);                                      \
					ctSEB_INDEX++;                                                                                                  \
					}                                                                                                               \
				}                                                                                                                   \
			if (ctSEB_INDEX != uiSEB_SOURCELENGTH) SEB_DELETELISTSTRING(ppszSEB_DESTINATION, uiSEB_DESTINATIONLENGTH, ctSEB_INDEX);                     
			


	// Copies (and creates if necessary) one list of UINTs to another
	#define SEB_COPYLISTUINT(puiSEB_DESTINATION, puiSEB_SOURCE, uiSEB_SOURCELENGTH, ctSEB_INDEX)	\
		if (uiSEB_SOURCELENGTH > 0) {																\
			if (puiSEB_DESTINATION)                                                                 \
				delete [] puiSEB_DESTINATION;														\
			if ((puiSEB_DESTINATION = new UINT [uiSEB_SOURCELENGTH]) != NULL)                       \
				for (ctSEB_INDEX = 0; ctSEB_INDEX < uiSEB_SOURCELENGTH; ctSEB_INDEX++)				\
					puiSEB_DESTINATION[ctSEB_INDEX] = puiSEB_SOURCE[ctSEB_INDEX];         			\
			}																						\
		if ((ctSEB_INDEX != uiSEB_SOURCELENGTH) && (puiSEB_DESTINATION != NULL)) {					\
			delete [] puiSEB_DESTINATION;															\
			puiSEB_DESTINATION = NULL;																\
			}



	// Copies (and creates if necessary) one list of lists of UINTs to another
	#define SEB_COPYLISTLISTUINT(ppuiSEB_DESTINATION, uiSEB_DESTLENGTH, ppuiSEB_SOURCE, uiSEB_SOURCELENGTH, puiSEB_SOURCELISTSLENGTH, ctSEB_INDEX1, ctSEB_INDEX2)		\
		SEB_DELETELISTLISTSUINT(ppuiSEB_DESTINATION, uiSEB_DESTLENGTH, ctSEB_INDEX2);																	\
		if ((puiSEB_SOURCELISTSLENGTH) && (uiSEB_SOURCELENGTH > 0)) {																					\
			if ((ppuiSEB_DESTINATION = new (UINT (* ([uiSEB_SOURCELENGTH])))) != NULL) { 																\
				ctSEB_INDEX1 = 0;																														\
				do {																																	\
					ppuiSEB_DESTINATION[ctSEB_INDEX1] = NULL;																							\
					SEB_COPYLISTUINT(ppuiSEB_DESTINATION[ctSEB_INDEX1], ppuiSEB_SOURCE[ctSEB_INDEX1], puiSEB_SOURCELISTSLENGTH[ctSEB_INDEX1], ctSEB_INDEX2)	\
					ctSEB_INDEX1++;																														\
					} while ((ctSEB_INDEX1 < uiSEB_SOURCELENGTH) && (ppuiSEB_DESTINATION[ctSEB_INDEX1-1]));												\
				if ((ctSEB_INDEX1 != uiSEB_SOURCELENGTH) || (ppuiSEB_DESTINATION[ctSEB_INDEX1-1] == NULL))												\
					SEB_DELETELISTLISTSUINT(ppuiSEB_DESTINATION, uiSEB_SOURCELENGTH, ctSEB_INDEX1);														\
                }																																		\
            }																																			\


#endif
