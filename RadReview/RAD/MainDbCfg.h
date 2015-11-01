
#ifndef MainDbCfg_H

	#define MainDbCfg_H
// this file contains the function prototypes to access the data stored in the main dbVista
// database associated with Rad.  All these functions should disappear when all data is stored
// in databases associated with COMs

	int StationExistsInMainDb(short sSta);		// 3.0.3 with new FacMgr
	int FacilityExistsInMainDb(short sFac);		// 3.0.3 with new FacMgr

#endif