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

// Comport.CPP
// Member functions for comport communications

 
#include "Comport.H"

 
ComPort_Processor::ComPort_Processor(const float fCom_Send_Timeout, const float fCom_Recv_Timeout,
                                     const UINT uiCom_Port, const DWORD dBaud_Rate, const char cParity, const UINT uiData_Length, const UINT uiStop_Bits) :
	cTerminator(NULL),
	bCommand_Set(FALSE),
	bComPort_Status(FALSE),
	uiCom_Port_Num(uiCom_Port),
	fSend_Timeout(fCom_Send_Timeout),
	fRecv_Timeout(fCom_Recv_Timeout)
	{
	bComPort_Status = com_open(uiCom_Port_Num, dBaud_Rate, cParity, uiData_Length, uiStop_Bits);
	if ((bComPort_Status == COM_SUCCESS) || (bComPort_Status == COM_IN_USE))
		bComPort_Status = TRUE;
	}
	

ComPort_Processor::ComPort_Processor(const char *pszSend, const char *pszRecv, const char *pszEnd_Identifier,
									 const float fCom_Send_Timeout, const float fCom_Recv_Timeout,
									 const UINT uiCom_Port, const DWORD dBaud_Rate, const char cParity, const UINT uiData_Length, const UINT uiStop_Bits) :
	cTerminator(NULL),
	bCommand_Set(TRUE),
	bComPort_Status(FALSE),
	uiCom_Port_Num(uiCom_Port),
	fSend_Timeout(fCom_Send_Timeout),
	fRecv_Timeout(fCom_Recv_Timeout)
	{
	
	pszSend_String = new char[strlen(pszSend)+1];
	pszRecv_String = new char[strlen(pszRecv)+1];
	pszRecv_Buffer = new char[strlen(pszRecv)+1];
	pszEnd_String = new char[strlen(pszEnd_Identifier)+1];
	strcpy(pszSend_String, pszSend);
	strcpy(pszRecv_String, pszRecv);
	strcpy(pszEnd_String, pszEnd_Identifier);
	
	bComPort_Status = com_open(uiCom_Port, dBaud_Rate, cParity, uiData_Length, uiStop_Bits);
	if ((bComPort_Status == COM_SUCCESS) || (bComPort_Status == COM_IN_USE))
		bComPort_Status = TRUE;
	}


ComPort_Processor::~ComPort_Processor(void) {

	if (bCommand_Set) {
		delete [] pszSend_String;
		delete [] pszRecv_String;
		delete [] pszRecv_Buffer;
		delete [] pszEnd_String;
	}
	
}


BOOL ComPort_Processor::ComPort_Opened(void) {

		return bComPort_Status;
}
     
     
BOOL ComPort_Processor::Set_Timeout(float fCom_Send_Timeout, float fCom_Recv_Timeout) {

	fSend_Timeout = fCom_Send_Timeout;
	fRecv_Timeout = fCom_Recv_Timeout;

	return TRUE;
}
     
     
BOOL ComPort_Processor::Get_Timeout(float *pfCom_Send_Timeout, float *pfCom_Recv_Timeout) {

	(*pfCom_Send_Timeout) = fSend_Timeout;
	(*pfCom_Recv_Timeout) = fRecv_Timeout;

	return TRUE;
}
     
     
BOOL ComPort_Processor::Send(void) {
    
    if (bCommand_Set)
		if (com_xmt_string(uiCom_Port_Num, fSend_Timeout, &cTerminator, pszSend_String, &num_sent) == COM_SUCCESS)
			if (com_rcv_string(uiCom_Port_Num, fRecv_Timeout, strlen(pszRecv_String), pszEnd_String, pszRecv_Buffer) == COM_SUCCESS)
				if (strncmp(pszRecv_Buffer, pszRecv_String, strlen(pszRecv_String)-1) == 0)
					return TRUE;
					
					
	return FALSE;

}


BOOL ComPort_Processor::Send(const char *pszSend, char *pszRecv, const UINT uiRecv_Length, const char *pszEnd_Identifier) {

	if (com_xmt_string(uiCom_Port_Num, fSend_Timeout, (char *)pszEnd_Identifier, (char *)pszSend, &num_sent) == COM_SUCCESS)
		if (com_rcv_string(uiCom_Port_Num, fRecv_Timeout, uiRecv_Length, (char *)pszEnd_Identifier, pszRecv) == COM_SUCCESS)
			return TRUE;
    
    
	return FALSE;
	
}


BOOL ComPort_Processor::Send(const char *pszSend, const char *pszRecv, const char *pszEnd_Identifier) {

	char *pszTemp_Recv_Buffer;
	pszTemp_Recv_Buffer = new char[strlen(pszRecv)+1];
	BOOL bSuccess = FALSE;

                                                                   
	if (com_xmt_string(uiCom_Port_Num, fSend_Timeout, (char *)pszEnd_Identifier, (char *)pszSend, &num_sent) == COM_SUCCESS)
		if (com_rcv_string(uiCom_Port_Num, fRecv_Timeout, strlen(pszRecv), (char *)pszEnd_Identifier, pszTemp_Recv_Buffer) == COM_SUCCESS)
			if (strncmp(pszTemp_Recv_Buffer, pszRecv, strlen(pszRecv)-1) == 0)
				bSuccess = TRUE;

	
	delete [] pszTemp_Recv_Buffer;
	return bSuccess;
	
}




