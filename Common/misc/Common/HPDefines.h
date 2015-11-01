//HPDefines.h

// The need for these defines in VS 20xx has now been 
// replaced by use of ADOLOCATION env var and
// addition of $(ADOLOCATION) to VC++ options for paths to use when searching for include files while building a VC++ project.
//
// typical locations you need will include the source of the msxml header
// e.g. c:\Program Files\MSXML 6.0\inc

// Access support COMs:
// e.g. ADOLOCATION=C:\Program Files\Common Files\System\ado
// for VC 6, add the path to the compiler include attribute: e.g.\Program Files\Common Files\System\ado

// IRS Comlibs e.g. \irs\comlibs

//for VS 2005 and above
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996)