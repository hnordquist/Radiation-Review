/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
   conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
   conditions and the following disclaimer in the documentation and/or other materials provided
   with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
   may be used to endorse or promote products derived from this software without specific prior
   written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
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
// more comments added to this RAD to FDMS exchange mechanism
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

