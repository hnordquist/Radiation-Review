#ifndef ADJUSTTIME_H
#define ADJUSTTIME_H

void AdjustStationTimeMenuOption();

double AdjustTimestampZeroBased(IDStruct StaID, double dOriginalTimestamp, double dStart, double dEnd);
double AdjustTimestamp(IDStruct StaID, double dOriginalTimestamp);
double UnadjustTimestamp(IDStruct StaID, double dAdjustedTimestamp);
void CleanUpAdjust();

#endif // ADJUSTTIME_H