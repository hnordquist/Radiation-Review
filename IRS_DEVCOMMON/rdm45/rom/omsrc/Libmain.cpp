//-----------------------------------------------------
//  LIBMAIN.CPP for Object Manager dll
//-----------------------------------------------------
//  Raima Object Manager -- an object storage class library / ODBMS.
//  For C++ 2.0 or above.
//  Copyright (c) 1990-1995, Raima Corporation.  All rights reserved.
//-----------------------------------------------------
#include <vpp.hpp>

extern "C" {

#ifdef WIN32

BOOL bWin32s;   //flags Win32s environment

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID Reserved)
{
   bWin32s = (BOOL)(GetVersion() & 0x80000000);  /* TRUE if in Win32s */

	switch(fdwReason) 
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
            break;
	}
	return(TRUE);
}

#else // WIN32

#ifdef WINDOWS

int PASCAL LibMain(HANDLE, WORD, WORD cbHeapSize, LPSTR)
{
   if (cbHeapSize > 0);
   return 1;
}

#ifndef __BCPLUSPLUS__
// This is defined in BORLAND Windows 3.10 WINDOWS.H file but not in Microsoft
VOID FAR PASCAL WEP(int)
{
   return ;
}
#endif // __BCPLUSPLUS__

#endif // WINDOWS

#endif // WIN32

}


