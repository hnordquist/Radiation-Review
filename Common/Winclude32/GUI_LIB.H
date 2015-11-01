
// GUI_LIB.h

#if !defined(GUI_LIB_H)
	#define GUI_LIB_H
	
	#define GUI_LIB_PRIVATE		// Shouldn't be necessary but... 8-Mar-95.
		
	#define OEMRESOURCE	1		// zApp's enumdef.h needs this

	// #define WIN_NT
	
	#include <windows.h>

	#include <zapp.hpp>  // should already be covered.

	#include <gui_defs.h>		// define RW_NO_STL before zApp include.

	#include <gui_app.h>
	#include <gui_dlg.h>
	#include <gui_dapp.h>
	#include <gui_fstr.h>
	#include <gui_menu.h>
	#include <gui_misc.h>
	#include <gui_pane.h>
	#include <gui_txwn.h>

#endif
