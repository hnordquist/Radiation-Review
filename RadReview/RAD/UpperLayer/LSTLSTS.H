// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 02/21/97

// LstLsts.H
// Class definition for a single linked lists of single
// linked lists
       

#ifndef LSTLSTS_H

	#define LSTLSTS_H

	#include <windows.h>
	#include <stdlib.h>
	#include <limits.h> 

	#include <rw/toolerr.h>
	#include <rw/coreerr.h>
	#include <rw/message.h>
	#include <rw/tpslist.h>

		
		
	class CLList_Of_LLists {

		public:

			CLList_Of_LLists(UINT uiDeminsionX, UINT uiDeminsionY); 
			virtual ~CLList_Of_LLists(void);
			
			BOOL bValid(void); 

			BOOL bAdd(UINT uiTool, UINT uiList, UINT uiItem);			
			BOOL bRemove(UINT uiTool, UINT uiList, UINT uiItem);			
			BOOL bEmpty(UINT uiTool, UINT uiList);
			BOOL bContains(UINT uiTool, UINT uiList, UINT uiItem);
			UINT uiGet_Least(UINT uiTool, UINT uiList);			
			void Remove_Range(UINT uiTool, UINT uiList, UINT uiStart_Value, UINT uiEnd_Value);			
			void Remove_Range_Range(UINT uiTool, UINT uiStart_Row, UINT uiEnd_Row, UINT uiStart_Value, UINT uiEnd_Value);			


		protected:

			BOOL bError;			
            UINT uiDimX, uiDimY;
            
            RWTPtrSlist<UINT> ***pppLList;
            
            
		private:      

		};
		
#endif		
    
    
    