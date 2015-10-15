/***********************************************************************************************\
*																								*
*	PROGRAM: 	VIFM CDM ANALYSIS TOOL  														*
*																								*
*	MODULE: 	VIFM FILE READ.C  -  source code for VIFM file procedures and housekeeping   	*
*																								*
*	Author: 	C. Liguori - IAEA - SGTIE   													*
*																								*
*	This program has been developed using National Instruments CVI ver 6.0  					*
*	reference files:    																		*
*				VIFC Analysis.prj		Project file											*
*				VIFC Analysis.c  		main module - user interface call back functions		*
*				VIFC Data Analyze.c 	Data Analysis source code								*
*				VIFC Setup.c    		Parameter Setup Procedures source code  				*
*				VIFC Analysis.uir   	User interface resource file							*
*				VIFC Analysis.h 		User interface resource header  						*
*																								*
*																								*
*	Last Edit: 	2003-03-20		  																*
*																								*
*	History:	2002-10-07 - release of version 1.0 											*
*				2002-11-28:	hide second progress indicator during file index creation			*
*				2003-03-20:	hide second progress indicator during file reading for graph 		*
*							display																*
*																								*
\***********************************************************************************************/

#include <ansi_c.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 0

#define MAX_PATHNAME_LEN 256
#define BURST_RECORD_LENGTH 35000	  // number of records read in each file read operation
#define MAX_RECORD 300000			  // max records a file can contain */
#define VIFM_TO_LANL_DATE_OFFSET (4*365+2)*24*3600   /* base for VIFM system is 1904-01-01 00:00:00 
											and not 1899-12-30 00:00 as for COleDateTime class format */ 
#define BASE_TIME (4*365)*24*3600	  /* base for VIFM system is 1904-01-01 00:00:00 
											and not 1900 as for time_t class */ 
// prototypes
void TimeToString (double, char *);
static void IntByteSwap    (unsigned char * , int);
static void DoubleByteSwap (unsigned char * , int);
int  FileIndexCreate    (char *);
static void CheckTimeInterval  (void);
void SetInfo            (void);
int ReadDataFile (char * inputFileName, int * station_ID, int * recordsNumber, double * samplingInterval);
static void SpikeCut (double samplingInterval, int recordNumber);

// local variables
static char readBuffer [BURST_RECORD_LENGTH * 40];
static unsigned int countBuffer [MAX_RECORD] [8];
static double timeStamp [MAX_RECORD];

static FILE * fileIn; 


static char inputFileName [MAX_PATHNAME_LEN];
static char inputFileExtension [4];
static double inputTimeStamp;
static long lTemp;
static long maxInputFileSize;
static long maxInputFileRecords;
static int nFile;
static int currentFileIdx;

static int recordIndex;

static double firstTime, lastTime;

typedef struct 
{
    char    name [MAX_PATHNAME_LEN];
    double  iniTime;
    long    recordNumber;
    double  samplingInterval;
} inputFileElement;

static inputFileElement inputFileIndex [1000];
static inputFileElement rTemp;



/*----------------------------------------------------------------

------------------     DataReadCommand      ----------------------

Top level procedure for the VIFM data read function

-----------------------------------------------------------------*/

/*
void DataReadCommand (void)
{
       	if (!FileIndexCreate ("c:\\aaa_sl1.000")) return;
        
	    currentFileIdx = 0;
	    recordIndex = 0;

        do
        {
            if (ReadDataFile () == FALSE) break;
            CheckTimeInterval ();
// insert here the function to append data into the data base

		    recordIndex = 0;
// Note: ReadDataFile is appending data to the input buffers at recordIndex position:
// if it is necessary to keep an overlap with the previously read data, a portion should
// be moved to the beginning of the input buffers and record index loaded accordingly

            
        } while (TRUE);
        
}
            
*/

/*----------------------------------------------------------------

------------------     FileIndexCreate      ----------------------

This procedure looks into the folder specified and creates a file list
in chronological order of all the files present in the
folder with a name matching the one specified.
Input:
	char * inputFileRoot : specifies the full pathname 
						of an input file with no extension
						(e.g. "d:\VIFMdata\FacId sl1\FacId sl1")
						
Output:
	returns FALSE if no files found or invalid data path
	returns TRUE  if at least one file was found and catalogued
	
common variables:
	nFile : output : contains the number of files found
	inputFileIndex: output : array of struct inputFileElement
    (	char    name [MAX_PATHNAME_LEN];
    	double  iniTime;
    	long    recordNumber;
    	double  samplingInterval;
    )

-----------------------------------------------------------------*/

int FileIndexCreate (char * inputFileRoot)
{
int i;

    for (nFile = i = 0; i < 1000; i++)
    {
        sprintf (inputFileExtension, "%3.3d", i);
        strcpy (inputFileName, inputFileRoot);
        strcat (inputFileName, inputFileExtension);
//        if (GetFileInfo (inputFileName, &lTemp) == 1)
            if (fileIn = fopen (inputFileName, "rb"))
            {
                if (fread (&inputTimeStamp, 8, 1, fileIn))
                {
                    strcpy (inputFileIndex [nFile].name, inputFileName);
                    DoubleByteSwap ((unsigned char *) &inputTimeStamp, 1);
                    inputFileIndex [nFile].iniTime = inputTimeStamp;
                    inputFileIndex [nFile].recordNumber = lTemp / 40;
                    nFile++;
                    if (lTemp > maxInputFileSize) 
                        maxInputFileSize = lTemp;
                }
                fclose (fileIn);
            }
    }
    
    if (nFile == 0) 		// NO VIFM file in the selected directory!!
        return FALSE;
    
    if (nFile > 1) 			// sort files in the provisional list according to start time
    {
        for (i = 0; i < nFile - 1; i++)
        {
            if (inputFileIndex [i].iniTime > inputFileIndex [i + 1].iniTime)
            {
                rTemp = inputFileIndex [i];
                inputFileIndex [i] = inputFileIndex [i + 1];
                inputFileIndex [i + 1] = rTemp;
                i = -1;
            }
        }
    }
    
    maxInputFileRecords = maxInputFileSize / 40;
    firstTime = inputFileIndex [0].iniTime;
    return TRUE;
}


/*----------------------------------------------------------------

--------------------     IntByteSwap      ------------------------

This static procedure converts from Motorola notation to Intel notation
an array of elementNumber 32-bit integers. The conversion is 
accomplished 'in place'.
Input:
	unsigned void * pointer : pointer to the integer array
	int elementNumber : number of element in the array
						
Output:
	none

-----------------------------------------------------------------*/

static void IntByteSwap (unsigned char * pointer, int elementNumber)
{
int j;
unsigned char tempStorage1;

    for (j = 0; j < elementNumber; j++)
    {   
        tempStorage1 = pointer [j * 4];
        pointer [j * 4] = pointer [j * 4 + 3];
        pointer [j * 4 + 3] = tempStorage1;
        
        tempStorage1 = pointer [j * 4 + 1];
        pointer [j * 4 + 1] = pointer [j * 4 + 2];
        pointer [j * 4 + 2] = tempStorage1;
    
    }
}


/*----------------------------------------------------------------

------------------     DoubleByteSwap      -----------------------

This static procedure converts from Motorola notation to Intel notation
an array of elementNumber 64-bit floating point. The conversion is 
accomplished 'in place'.
Input:
	unsigned void * pointer : pointer to the double array
	int elementNumber : number of element in the array
						
Output:
	none

-----------------------------------------------------------------*/

static void DoubleByteSwap (unsigned char * pointer, int elementNumber)
{
int j;
unsigned char tempStorage1;

    for (j = 0; j < elementNumber; j++)
    {   
        tempStorage1 = pointer [j * 8];
        pointer [j * 8] = pointer [j * 8 + 7];
        pointer [j * 8 + 7] = tempStorage1;
        
        tempStorage1 = pointer [j * 8 + 1];
        pointer [j * 8 + 1] = pointer [j * 8 + 6];
        pointer [j * 8 + 6] = tempStorage1;
        
        tempStorage1 = pointer [j * 8 + 2];
        pointer [j * 8 + 2] = pointer [j * 8 + 5];
        pointer [j * 8 + 5] = tempStorage1;
        
        tempStorage1 = pointer [j * 8 + 3];
        pointer [j * 8 + 3] = pointer [j * 8 + 4];
        pointer [j * 8 + 4] = tempStorage1;
    
    }
}


/*----------------------------------------------------------------

--------------------     ReadDataFile      -----------------------

This procedure reads in the file specified in the file index table 
at the index currentFileIdx. 

Output:
	returns FALSE if no file exists in the specified index table entry
	returns TRUE  if successful
	
currentFileIdx is incremented when the procedure is successfully completed.
	
-----------------------------------------------------------------*/

int ReadDataFile (char * inputFileName, int * station_ID, int * recordsNumber, double * samplingInterval)
{
int i, k, numrec;
HANDLE hFile;
DWORD fileSize;

	hFile = CreateFile (inputFileName,  // file to open
                   		GENERIC_READ,   // open for reading
                   		FILE_SHARE_READ | FILE_SHARE_WRITE,  // share mode
                   		NULL,    		// default security attributes
                   		OPEN_EXISTING,  // disposition
                   		0,       		// file attributes
                   		NULL);   		// don't copy any file's attributes

	if (hFile == INVALID_HANDLE_VALUE) {        // if we can't open the file...
		return FALSE;
	}
	
	fileSize = GetFileSize (hFile, 0);
	CloseHandle (hFile);
	if (fileSize == -1)
	{
		return FALSE;
	}
		
	
    fileIn = fopen (inputFileName, "rb");
    if (fileIn == NULL) 
    	return FALSE;
    	
    for (i = 0; !feof (fileIn); i+=k)
    {
        numrec = fread (readBuffer, 40, BURST_RECORD_LENGTH, fileIn);
        
        for (k = 0; k < numrec; k++)
        {
            if (recordIndex >= MAX_RECORD) break;
        
            memcpy (&timeStamp[recordIndex], &readBuffer [k * 40], 8);
            memcpy (&countBuffer[recordIndex][0], &readBuffer [k * 40 + 8], 32);
            
            DoubleByteSwap ((unsigned char *) &timeStamp[recordIndex], 1);
            IntByteSwap ((unsigned char *) &countBuffer[recordIndex][0], 8);
	// converts now from VIFM based time (1904-1-1) to OLE format (base 1899-12-30 00:00)
            timeStamp [recordIndex] += VIFM_TO_LANL_DATE_OFFSET;  
	// converts from sec to day based time&date            
			timeStamp [recordIndex] /= 24*3600;	

            recordIndex++;

// here an option to cancel the action by a user interface command like 'Cancel'
/*
            if (((i+k+1) % 3000) == 0)
            {
    
                if (GetUserEvent (0, &dum1, &dum2) == 1)
                {
                    HidePanel (fileReadInfo);
                    fclose (fileIn);
                    recordIndex = 0;
                    return FALSE;
                }
            }
*/
        }
    }

    fclose (fileIn);
    
    (*recordsNumber) = recordIndex - 1;
    
    for (k = recordIndex - 3; k >= 0; k--)
    {
        (*samplingInterval) = timeStamp[k] - timeStamp[k - 1];
        if (((*samplingInterval) < 2) && ((*samplingInterval) > .1)) 
            break;
    }

    SpikeCut (*samplingInterval, *recordsNumber);

    return TRUE;
}


/*----------------------------------------------------------------

--------------------     TimeToString      -----------------------

This procedure coverts the VIFM date and time stamp value into a text 
string in the format YYYY-MM-DD hh:mm:ss.n

-----------------------------------------------------------------*/



void TimeToString (double timeStamp, char * timeString)
{
struct tm tm1;
time_t time1;

    timeStamp *= 24*3600;
    time1 = (time_t) (timeStamp - VIFM_TO_LANL_DATE_OFFSET + BASE_TIME);
    tm1 = (*gmtime (&time1));
    timeStamp = timeStamp - (time_t) timeStamp;
	strftime (timeString, 40, "%Y-%b-%d %H:%M:%S", &tm1);
	sprintf (&timeString [strlen (timeString)], ".%d", (int) (timeStamp * 10.));
}


/*----------------------------------------------------------------

--------------------     SpikeCut      -----------------------

This procedure removes from the raw data files the single point spikes
usually due to the well known ADAM deficiency in handling the counter 
reading when close to a 10k boundary for the counter content

-----------------------------------------------------------------*/

#define STATISTIC_CUT 4.5

/*

First I check if data are on a plateau (first two points).
If so and if third point shows a difference greater than 8000 counts 
and if the fourth point is still on a line with the first two points
then replace the third point with the averaged content of the two adiacent points.
In case of count rates > 2e5 I don't care removing any spike

*/

static void SpikeCut (double samplingInterval, int recordNumber)
{
int i, j;
int sigma, remainder, delta;
    
    for (j = 0; j < 8; j++)
    {
	    for (i = 0; i < recordNumber - 4; i++)
	    {
    
	        delta =  abs ((int) countBuffer [i + 2] [j] - (int) countBuffer [i] [j]);
        
	        if (delta > (8000/samplingInterval))
	        {
	            sigma = (unsigned int) (sqrt (countBuffer [i + 1] [j] * samplingInterval) 
	            	* STATISTIC_CUT / samplingInterval);
	            remainder = abs (((countBuffer [i + 3] [j] + (int) (5000 / samplingInterval)) % 
	            	(int) (10000 / samplingInterval)) - (5000 / samplingInterval));
	            if ((abs ((int) countBuffer [i + 1] [j] - (int) countBuffer [i] [j]) < sigma) && 
	                (countBuffer [i + 1] [j] < 200000) &&
	                ((remainder < sigma) || (delta > 20000)) &&
	                (abs ((int) countBuffer [i + 3] [j] - (int) countBuffer [i + 1] [j]) < sigma))
	            {
	                    countBuffer [i + 2] [j] = (countBuffer [i + 1] [j] 
	                    			+ countBuffer [i + 3] [j]) / 2;
                    
	            }
	        }
	    }
    }
    
    return;
}


/*
static void CheckTimeInterval (void)
{
double timeInterval, diff;
int j;
    
    timeInterval = timeStamp [1] - timeStamp [0];
    if (timeInterval <= 0)
    {
        return;
    }
    
    timeInterval *= 5.;
    
    for (j = 2; j < recordNumber; j++)
    {
        diff = (timeStamp [j] - timeStamp [j - 1]);
        if ((diff < 0) || (diff > timeInterval))
        {
/*            j0 = (int) ((diff * 5.) / timeInterval);
            if ((j0 > 0) && (j0 < 257))
                timeAnomalDistribution [j0]++;
            else if (++anomalies < 21)
            {
                TimeToString (timeStamp [j], timeString);
                fprintf (fileRep, "\n  Time anomaly at %s (record %d - delta %.2f s)", timeString,
                    j - REGION_SWAP, (timeStamp [j] - timeStamp [j - 1]));
//              j += 10;
            }
*//*
            if ((timeStamp [j] - timeStamp [0] ) < 0)
            {
//                MessagePopup ("Warning", 
//                    "An invalid time stamp has been found!\n File content has been corrected!");
                {
                    do
                    {
                        timeStamp [j] = timeStamp [j - 1] + timeInterval / 5.;
                        j++;
                        if (j >= recordNumber) break;
            
                    } while ((timeStamp [j] - timeStamp [0]) < 0);
                }
            }
        }
    }       
}
*/

/* functions not needed

int GetData (double iniTime, double timeInterval)
{
int i, loopFlag;
int dum1, dum2;
double endTime;
int k, numrec;

    endTime = iniTime + timeInterval;
    
    if (nFile > 1)
	    for (currentFileIdx= nFile - 1; currentFileIdx > 0; currentFileIdx--)
	    {
	        if (inputFileIndex [currentFileIdx].iniTime < iniTime) break;
	    }
	else currentFileIdx = 0;

    recordIndex = 0;
    loopFlag = TRUE;
    
loop:

    fileIn = fopen (inputFileIndex [currentFileIdx].name, "rb");
    
    SplitPath (inputFileIndex [currentFileIdx].name, inputFileDrive, inputFileDirectory,
               inputFileShortName);
    
    SetCtrlVal (fileReadInfo, FILE_INFOS_DATE, "");

    SetCtrlVal (fileReadInfo, FILE_INFOS_NAME, inputFileShortName);
    SetCtrlVal (fileReadInfo, FILE_INFOS_STATE, 0.);
    SetCtrlAttribute (fileReadInfo, FILE_INFOS_NAME, ATTR_LABEL_TEXT,
                  "Reading File");
    SetActiveCtrl (fileReadInfo, FILE_INFOS_DUMMY);
    SetCtrlVal (fileReadInfo, FILE_INFOS_STATE_2, 0.);
	SetCtrlAttribute (fileReadInfo, FILE_INFOS_STATE_2, ATTR_VISIBLE,
					  FALSE);
    SetCtrlVal (fileReadInfo, FILE_INFOS_TIME, "");
    
    DisplayPanel (fileReadInfo);

    fseek (fileIn,
       (long) ((iniTime - inputFileIndex [currentFileIdx].iniTime) *.9 / inputFileIndex [currentFileIdx].samplingInterval) * 40,
       SEEK_SET);
       
    for (i = 0; !feof (fileIn); i+=k)
    {
        numrec = fread (readBuffer, 40, BURST_RECORD_LENGTH, fileIn);
        
        for (k = 0; k < numrec; k++)
        {
        
            memcpy (&timeStamp[recordIndex], &readBuffer [k * 40], 8);
            memcpy (&countBuffer[recordIndex][0], &readBuffer [k * 40 + 8], 32);
            
            doubleByteSwap ((unsigned char *) &timeStamp[recordIndex], 1);
            intByteSwap ((unsigned char *) &countBuffer[recordIndex][0], 8);
    
            if (timeStamp[recordIndex] > endTime) 
            {
                loopFlag = FALSE;
                break;
            }
        
            if (timeStamp[recordIndex] > iniTime) 
                recordIndex++;
        
            if (((i+k) % 1000) == 0)
            {
                SetCtrlVal (fileReadInfo, FILE_INFOS_STATE, (double) (i+k) * 100 / (double) maxInputFileRecords);
                ProcessDrawEvents ();
                ProcessSystemEvents ();
    
                if (GetUserEvent (0, &dum1, &dum2) == 1)
                {
                    HidePanel (fileReadInfo);
                    fclose (fileIn);
                    return FALSE;
                }

            }
            
        }
            
    }
    
    fclose (fileIn);
    
    if (loopFlag)
    {
        if (++currentFileIdx >= nFile) 
        {   
            HidePanel (fileReadInfo);
            return FALSE;
        }
        goto loop;
    }
    
    HidePanel (fileReadInfo);
  	SetCtrlAttribute (fileReadInfo, FILE_INFOS_STATE_2, ATTR_VISIBLE, TRUE);
    recordNumber = recordIndex - 1;
    
    if (recordNumber > 2) return TRUE;
    else return FALSE;
}

*/
