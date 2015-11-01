// Shirley Klosterbuer
// Los Alamos National Laboratory
// NIS-5
// 10/01/96

// RAD.H
// Class definition for Radiation Review
             


#ifndef RAD_H

	#define RAD_H

	#include <gui_lib.h>
#ifndef WIN32
	#include <nscdefs.h>
#endif
	#include <stdlib.h> 
	#include <stdio.h>
	#include <io.h>
	#include <string.h>
	#include "vista.h"

	#include "RADSTRS.H"
	#include "RADERROR.H"
	#include "RADDEFS.H"

	#pragma pack(1)
	#include "database\db_main.h"
	#include "database\db_dflt.h"    
	#include "database\db_oper.h"    
	#pragma pack(8)
	
 	#include "gui_dde.h" 
	#include "resource.h"
#endif


		
