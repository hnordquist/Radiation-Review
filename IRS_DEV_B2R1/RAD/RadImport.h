// Shirley Klosterbuer
// Los Alamos National Laboratory
// NIS-5
// 4/23/97

// RADImprt.H
// Radiation Review specific Import class definition
// NOTE: Defines the operations which are required through the
// generic review tool setup definition and defines the specific
// opertions for the radiation review tool setup      


#ifndef RADIMPRT_H

	#define RADIMPRT_H
                       


	#include "upperlayer/RTImport.H" // 8-Dec-04 SFK Added relative path
	#include "RADPROTO.H"
	#include "RADInit.H"

	
	class RADImport : public RTImport {

		public:
		
			// Operator class functions
			RADImport(void); 
			~RADImport(void); 


            // Required through RTImport to provide the Import Begin procedure which is specific to operator review
			IMPORT_OPERATION RT_Import_Begin(BOOL bBypass_Import_Prompt);

            // Required through RTImport to provide the Import Exit procedure which is specific to operator review
     		BOOL RT_Import_Exit(void); 

			// If TRUE, then have been commanded to run in IMPORT mode
			BOOL bInImportMode;

     		

		protected:
            

			// Overide the generic procedure in order to set to my specific type
			// Note: Could just always cast to my type but this makes code cleaner
			virtual BOOL Set_Common_Params(RTInit *const pRT_Common_Params);
            RADInit *pCommon_Params;

			// Displays the setup box 
            virtual void RAD_Import_Delete(void);
            

            
        private:

			BOOL RADImport::RAD_ImportPromptForDisk(void);
			BOOL RADImport::RAD_ImportMultipleDisks(void);
            BOOL RADImport::RAD_ImportAutomateQuestion(void);

            RTImport::IMPORT_OPERATION bRAD_Import_Done;                            
            BOOL RAD_Import_FTP(void);
		};
		
#endif


		
