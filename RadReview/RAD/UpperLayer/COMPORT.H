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
