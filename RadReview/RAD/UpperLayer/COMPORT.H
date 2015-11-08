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
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 7/17/96

// COMPORT.H
// Class definition for a comport processor and it's operations
       
       


#ifndef COMPORT_H
    
	#define COMPORT_H
	
    #include "windows.h"
    #include <string.h>
     
	#ifdef _WIN32
		#ifndef COMPORT_COM32_H
    		#define COMPORT_COM32_H
   			#include "com32.h"
		#endif
	#else
		#ifndef COMPORT_COM_H
    		#define COMPORT_COM_H
   			#include "com.h"
		#endif
	#endif

    #define fSEND_TIMEOUT 0.5  // Number of seconds until a timeout occurs when transfering a command
    #define fRECV_TIMEOUT 1    // Number of seconds until a timeout occurs when receiving a command

    #define uiCOM_PORT			2		// 1 - 10
	#define dBAUD_RATE			9600
    #define uiDATA_LENGTH		7
    #define cPARITY				'O'		// O = ODD or E = EVEN
    #define uiSTOP_BITS			1
    

    
	class ComPort_Processor {

		public:                                        
		
			ComPort_Processor(const float fCom_Send_Timeout = fSEND_TIMEOUT, const float fCom_Recv_Timeout = fRECV_TIMEOUT, 
							  const UINT uiCom_Port = uiCOM_PORT, const DWORD dBaud_Rate = dBAUD_RATE, const char cParity = cPARITY, const UINT uiData_Length = uiDATA_LENGTH, const UINT uiStop_Bits = uiSTOP_BITS);
			ComPort_Processor(const char *pszSend, const char *pszRecv, const char *pszEnd_Identifier,
							  const float fCom_Send_Timeout = fSEND_TIMEOUT, const float fCom_Recv_Timeout = fRECV_TIMEOUT,
							  const UINT uiCom_Port = uiCOM_PORT, const DWORD dBaud_Rate = dBAUD_RATE, const char cParity = cPARITY, const UINT uiData_Length = uiDATA_LENGTH, const UINT uiStop_Bits = uiSTOP_BITS);
			virtual ~ComPort_Processor(void);
			BOOL ComPort_Opened(void);
			BOOL Set_Timeout(float fCom_Send_Timeout, float fCom_Recv_Timeout);
			BOOL Get_Timeout(float *pfCom_Send_Timeout, float *pfCom_Recv_Timeout);
			BOOL Send(void);                                                                              
			BOOL Send(const char *pszSend, const char *pszRecv, const char *pszEnd_Identifier);
			BOOL Send(const char *pszSend, char *pszRecv, const UINT uiRecv_Length, const char *pszEnd_Identifier);


		protected:

			char *pszSend_String, *pszRecv_String, *pszRecv_Buffer, *pszEnd_String;
			UINT uiCom_Port_Num;
			int num_sent;
			char cTerminator;
			BOOL bCommand_Set, bComPort_Status;
			float fSend_Timeout, fRecv_Timeout;


		};

#endif
