/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
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


		
