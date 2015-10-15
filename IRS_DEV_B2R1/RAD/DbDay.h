
/* DbDay.H  Function Prototypes */

#ifndef DbDay_H
	#define DbDay_H

class CDbDay
{
public:
	CDbDay(){;}
	~CDbDay(){;}
	int FindDayRecInMainDb(short sSta, DATE dTime);
	int CreateDayRecInMainDb(short sSta, DATE dDay);
	int ReadDayRecInMainDb(short sSta, DATE dDay, struct db_day_rec *dbDay);    
	int DeleteDbDayInMainDb(struct IDStruct StaID, DATE dDay);
};

#endif
