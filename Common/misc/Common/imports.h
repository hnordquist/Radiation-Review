#ifndef IMPORTS_H
#define IMPORTS_H

// imports.h

#include "HPDefines.h"

// The need for these defines has now been 
// replaced by use of COMLIBSLOCATION env var and
// addition of $(COMLIBSLOCATION) to VC++ options for paths to use when searching for include files while building a VC++ project.
// or the addition of \irs\comlibs to the PATH variable in the user's environment.


#import "CoFacilityMgr.exe"	no_namespace named_guids
#import "CoIRSLogger.dll"			no_namespace named_guids

#endif //IMPORTS_H
