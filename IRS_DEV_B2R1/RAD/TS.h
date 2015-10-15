//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: TS.h $
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/18/04   Time: 3:29p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// more comments added to this RAD to FDMS exchnage mechanism
// 
// *****************  Version 2  *****************
// User: Longo        Date: 10/18/04   Time: 10:22a
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// comments galore
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// This file contains a class definition that converts a primitive string
// representation of a date and time into a COleDateTime object.
//
// The class TS ('TimeStamp') is a subclass of COleDateTime.
//
// To use TS, create an instance of a TS, optionally invoking the Convert() method.
// There are 2 styles of usage:
//		Use the bare constructor, then using Convert(char*) to present the source string and convert
//		Use the char* parameterized constructor; Convert() is invoked internally during construction
//
// After construction and conversion, a TS instance is used as a COleDateTime instance.
//
//////////////////////////////////////////////////////////////

#pragma once

class COleDateTime;

//////////////////////////////////////////////////////////////
// The class TS contains methods to convert strings to COleDateTime values.
//
// Derived from COleDateTime
//
// methods:
//	TS()			constructor
//	TS(char* ts)	constructor, calls Convert with parameter
//  ~TS(void);		destructor
//	void Convert(LPCTSTR str);
//	bool convert_from_hhmmss (char * input_string);
//	bool convert_from_yyyymmdd (char * input_string);
//
//////////////////////////////////////////////////////////////
class TS : public COleDateTime
{
public:
	TS(void);
	TS(char* ts);

	~TS(void);

	///////////////////////////////////////////////////////
	// Convert
	//
	// Transforms the instance into a COleDateTime representation
	// of the timestamp string parameter
	// 
	// Input:
	//	str		pointer to string representation of time
	///////////////////////////////////////////////////////
	void Convert(LPCTSTR str);

private:
	
	///////////////////////////////////////////////////////
	// convert_from_hhmmss
	//
	// Converts the hour, minute and seconds of the input string to integer values
	// 
	// Input:
	//	input_string		pointer to string representation of time
	//					(2 digit hour, 2 digit minutes, 2 digit seconds)
	// Output:
	//	hr, min, sec: traditional integer values for the named fields
	//
	///////////////////////////////////////////////////////
	bool convert_from_hhmmss (char * input_string, int& hr, int& min, int& sec);

	///////////////////////////////////////////////////////
	// convert_from_yyyymmdd
	//
	// Converts the year, month and day of the input string to integer values
	// 
	// Input:
	//	input_string		pointer to string representation of time
	//					(4 digit year, 2 digit month, 2 digit day)
	// Output:
	//	year, mon, day: traditional integer values for the named fields
	//
	///////////////////////////////////////////////////////
	bool convert_from_yyyymmdd (char * input_string, int& year, int& mon, int& day);

};

