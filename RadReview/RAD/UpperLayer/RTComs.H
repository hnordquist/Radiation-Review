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
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/11/97

// RT_Coms.H
// These are the messages which a Review Tool can understand


#ifndef RT_COMS_H

	#define RT_COMS_H

  	// Timestamp format is defined to be aaaaabbbbccddeeffgghhh where
	//		aaaaa:	5 digits for ID (12345)     NOT CURRENTLY USED
	//		bbbb:	4 digits for year (1997)
	//		cc:		2 digits for month (02)
	//		dd:		2 digits for day (05)
	//		ee:		2 digits for hour (12)
	//		ff:		2 digits for minute (45)
	//		gg:		2 digits for second (30)
	//		hhh:	3 digits for milliseconds (768)
	//	Example: 1234519720516012345678 = 12345 5/16/72 1:23:45.678


  	// Timerange format is defined to be aaaaabbbbccddeeffgghhh-iiiijjkkllmmnnooo where
	//		aaaaa:	5 digits for ID (12345)     NOT CURRENTLY USED
	//		bbbb:	4 digits for start year (1997)
	//		cc:		2 digits for start month (02)
	//		dd:		2 digits for start day (05)
	//		ee:		2 digits for start hour (12)
	//		ff:		2 digits for start minute (45)
	//		gg:		2 digits for start second (30)
	//		hhh:	3 digits for start milliseconds (768)
	//		bbbb:	4 digits for end year (1997)
	//		cc:		2 digits for end month (02)
	//		dd:		2 digits for end day (05)
	//		ee:		2 digits for end hour (12)
	//		ff:		2 digits for end minute (45)
	//		gg:		2 digits for end second (30)
	//		hhh:	3 digits for end milliseconds (768)
	//	Example: 1234519720516012345678-19720516014020555 = 12345 5/16/72 1:23:45.678 to 5/16/72 1:40:20.555
    
	// szPIPE_DATA, szPIPE_DATA_AREA, szPIPE_EXIT_ALL return BUSY in request (overlapping request message) if could not shutdown
	// Currently, all messages which receive a return string receive the exact same value back if successful


	////////////////////////////////////////////////////////////////////////////////////////////////
	// DDE COMMANDS - Messages which are sent/received as a DDE command (no reply)
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Sent to the clients (other tools) to let it know that your server has started and it can connect to it
	// Example: "PIPE_STARTED:DVR"
    #define szPIPE_STARTED				"PIPE_STARTED:"			// followed by sender tool pipe name

	// Sent to the setup commanding tool to let it know that your tool has completed setup
	// Example: "PIPE_SETUP_DONE:DVR"
    #define szPIPE_SETUP_DONE			"PIPE_SETUP_DONE"		// followed by sender tool pipe name

	// Sent to the import commanding tool to let it know that your tool has completed import
	// Example: "PIPE_IMPORT_DONE:DVR"
    #define szPIPE_IMPORT_DONE			"PIPE_IMPORT_DONE:"		// followed by sender tool pipe name

	// Sent to the reanalyze commanding tool to let it know that your tool has completed reanalyze
	// Example: "PIPE_REANALYZE:DVR"
    #define szPIPE_REANALYZE_DONE		"PIPE_REANALYZE_DONE:"	// followed by sender tool pipe name

	// Sent to all clients (other tools) to let them know you are exiting and your server will not be available
	// Example: "PIPE_EXITED:DVR"
    #define szPIPE_EXITED				"PIPE_EXITED:"			// followed by sender tool pipe name


	////////////////////////////////////////////////////////////////////////////////////////////////
	// DDE REQUESTS - Messages which are sent/received as a DDE request (reply required)
	////////////////////////////////////////////////////////////////////////////////////////////////

	// NOT CURRENTLY USED
    #define szPIPE_DATABASE				"PIPE_DATABASE:"		// followed by sender tool pipe name + database path+name  Ex. "PIPE_IMPORT_DONE:RAD+C:\IRS\RAD\RAD.DB"

	// NOT CURRENTLY USED
	#define szPIPE_DATA_UPDATE			"PIPE_DATA_UPDATE:"		// followed by start timestamp indicating event to read from DB to update

	// Sent to request a client (other tool) to display its matching data
	// Example: "PIPE_DATA:RAD+0000019720516123456789"
	// Reply: Succesful (Reply == Request): "PIPE_DATA:RAD+0000019720516123456789"
	// Reply: Tool busy: "BUSY"
	// Reply: Tool does not have data for location: "LOCATION"
	// Reply: Tool does not have matching data at time: "TIMESTAMP"
	// Reply: Failed (Reply != Request): "IPE_DATA:RAD+0000019720516123456789"
    #define szPIPE_DATA					"PIPE_DATA:"			// followed by sender tool pipe name + Timestamp

	// NOT CURRENTLY USED
	// Example:  "PIPE_DATA_AREA:RAD+0000019720516123456789"
    #define szPIPE_DATA_AREA			"PIPE_DATA_AREA:"		// followed by sender tool pipe name + Timestamp

	// Sent to request a client (other tool) to change its represented data to a different location
	// NOTE: This is not the same location as the 5 character location at the begining of a timestamp
	// NOTE: Some tools may require this to be sent every time before szPIPE_DATA and szPIPE_DATA_AREA is sent
	// Example: "PIPE_LOCATION:RAD+RokkashoCanalA"
	// Reply: Succesful (Reply == Request): "PIPE_LOCATION:RAD+RokkashoCanalA"
	// Reply: Tool busy: "BUSY"
	// Reply: Tool does not have location: "LOCATION"
	// Reply: Failed (Reply != Request): "IPE_DATA:RAD+RokkashoCanalA"
    #define szPIPE_LOCATION				"PIPE_LOCATION:"		// followed by sender tool pipe name + Location

	// Sent to request a client (the reanalyze commander tool) to initiate a reanalyzation of appropriate data for appropriate tools
	// Example: "PIPE_REANALYZE:RAD"
	// Reply: Succesful (Reply == Request): "PIPE_REANALYZE:RAD"
	// Reply: Tool busy: "BUSY"
	// Reply: Failed (Reply != Request): "IPE_REANALYZE:RAD"
    #define szPIPE_REANALYZE			"PIPE_REANALYZE:"		// followed by sender tool pipe name

	// NOT CURRENTLY USED
    #define szPIPE_SAVE_ALL				"PIPE_SAVE_ALL"

	// Sent to request ALL clients (other tools) to exit immediately, this allows all tools to exit as one
	// NOTE: If the tool needs to still save its data, the tool should reply successful and then display the appropriate data saving screens
	// Example: "PIPE_EXIT_ALL"
	// Reply: Succesful (Reply == Request): "PIPE_EXIT_ALL"
	// Reply: Tool busy: "BUSY"
	// Reply: Failed (Reply != Request): "IPE_EXIT_ALL"
    #define szPIPE_EXIT_ALL				"PIPE_EXIT_ALL"



    #define uiTIME_STAMP_LENGTH				22
    #define uiTIME_RANGE_LENGTH				45
	#define uiTIMESTAMP_LOCATION_LENGTH		5		// NOT CURRENTLY USED
    

#endif

