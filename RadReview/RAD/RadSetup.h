// Shirley Klosterbuer
// Los Alamos National Laboratory
// NIS-5
// 4/23/97

// RADSetup.H
// Radiation Review specific Setup class definition
// NOTE: Defines the operations which are required through the
// generic review tool setup definition and defines the specific
// opertions for the radiation review tool setup      


#ifndef RADSETUP_H

	#define RADSETUP_H

	#include "upperlayer/RTSetup.H"	// 8-Dec-04 SFK Added relative path
	#include "RADPROTO.H"
	#include "RADInit.H"
	
	
	class RADSetup : public RTSetup {

		public:
			// Operator class functions
			RADSetup(void); 
			~RADSetup(void); 


            // Required through RTSetup to provide the Setup Begin procedure which is specific to operator review
			RTSetup::SETUP_OPERATION RT_Setup_Begin(void);

            // Required through RTSetup to provide the Setup Exit procedure which is specific to operator review
     		BOOL RT_Setup_Exit(void); 

		
		protected:
			// Overide the generic procedure in order to set to my specific type
			// Note: Could just always cast to my type but this makes code cleaner
			virtual BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
            RADInit *pCommon_Params;

            
        private:
            RTSetup::SETUP_OPERATION bRAD_Setup_Done;                            
		};
		
#endif


		
