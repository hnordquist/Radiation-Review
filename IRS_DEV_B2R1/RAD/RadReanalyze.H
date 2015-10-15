// Shirley Klosterbuer
// Los Alamos National Laboratory
// NIS-5
// 9/16/98

// RADReanalyze.H
// Radiation Review specific Reanalyze class definition
// NOTE: Defines the operations which are required through the
// generic review tool setup definition and defines the specific
// opertions for the radiation review tool setup      


#ifndef RADREANALYZE_H

	#define RADREANALYZE_H
                       
	#include "upperlayer/RTReanalyze.H"	// 8-Dec-04 SFK Added relative path
	#include "RADInit.H"


	
	class RADReanalyze : public RTReanalyze {

		public:
		
			// Operator class functions
			RADReanalyze(void); 
			~RADReanalyze(void); 


            // Required through RTReanalyze to provide the Begin procedure which is specific to operator review
			REANALYZE_OPERATION RT_Reanalyze_Begin();

            // Required through RTReanalyze to provide the Import Exit procedure which is specific to operator review
     		BOOL RT_Reanalyze_Exit(void); 

     		

		protected:
            
			// Overide the generic procedure in order to set to my specific type
			// Note: Could just always cast to my type but this makes code cleaner
			virtual BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
            RADInit *pCommon_Params;

            
        private:

		};
		
#endif


		
