// DbStation.h: interface for the CGrandDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBSTATION_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_)
#define AFX_DBSTATION_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int GetStationRecord(short sFacDbNum, const char *szStaName, struct db_sta_rec *pdbSta);

#endif // !defined(AFX_DBSTATION_H__2D7602C5_ED7D_11D5_816F_00C04F60E89B__INCLUDED_)
