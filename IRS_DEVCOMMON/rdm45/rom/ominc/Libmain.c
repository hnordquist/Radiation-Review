/*-----------------------------------------------------------------------
   libmain.c -- Included by the main module of each RDS dll
                for Windows and NT

   Copyright (c) 1991-1994, Raima Corporation.  All Rights Reserved.
-----------------------------------------------------------------------*/
HANDLE hClientInst;

#ifdef WIN32_NT
INT APIENTRY LibMain(
    HANDLE hInstance,
    ULONG  ul_reason_being_called,
    LPVOID lpReserved)
{
    hClientInst = hInstance;

    switch (ul_reason_being_called)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
            if (!_CRT_INIT(hInstance, ul_reason_being_called, lpReserved))
                return FALSE;

            break;

        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            if (!_CRT_INIT(hInstance, ul_reason_being_called, lpReserved))
                return FALSE;

        break;
    }
    return 1;
}

#else

int PASCAL LibMain(
    HANDLE hInstance,
    WORD   cbHeapSize,
    WORD   wDataSeg,
    LPSTR  lpszCmdLine)
{
    hClientInst = hInstance;

    UNREF_PARM(cbHeapSize)
    UNREF_PARM(wDataSeg)
    UNREF_PARM(lpszCmdLine)

    return (1);
}

int PASCAL WEP (
    int nParameter )
{
    if ( nParameter == WEP_SYSTEM_EXIT ) {
        return ( 1);
    } else if ( nParameter == WEP_FREE_DLL ) {
        return (1);
    } else {
        return (1);
    }
}
#endif

/*
$Header:   P:/PAT/INCLUDE/VCS/LIBMAIN.C_V   1.3   18 Apr 1994 12:10:12   RICHM  $
*/

