/* for Dos Library only

 Note: I tried making this a .cpp file and just including vpp.hpp, 
  but that required a change to RDM header files where they have
  the global variable lockcomm_type defined to be CONST.  C++ makes
  that global local and so you get lockcomm_type undefined external 
  when you create your application.
*/

#include <stdio.h>

#define MULTI_TASK
#define LOCKCOMM_DATA
#define NO_WIN_H

#include "vpp.hpp"


void dummy()
{
}

