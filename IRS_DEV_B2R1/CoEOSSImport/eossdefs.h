//-----------------------------------------------------------------------------
//
// EOSS - Electronic Optical Sealing System
//
// Sample Seal Reader
//
// (c) 2007 Dr. Neumann Consultants
// D-50259 Pulheim, Schiffgesweg 9, Germany
//
//-----------------------------------------------------------------------------
#define MAXNUMEOSS 32

#pragma once

#define CMD_LOGON 'L'
#define CMD_LOGOFF 'O'
#define CMD_TIME 'T'
#define CMD_STATUS 'S'
#define CMD_SETUP 'U'
#define CMD_LOGINFO 'I'
#define CMD_LOGPAGE 'P'
#define CMD_KEYSET 'K'
#define CMD_WRITEPAD 'W'
#define CMD_READPAD 'R'
#define CMD_ERASE 'E'
#define CMD_HASH 'H'

#define PCK_GETCHALLENGE 'G'
#define PCK_CHALLENGE 'C'
#define PCK_REQUEST 'Q'
#define PCK_REPLY 'R'
#define PCK_DENIAL 'D'
#define PCK_TRIGGER 'T'
#define PCK_INQUIRY 'I'
#define MAX_SEALS 32

#pragma pack(push)
#pragma pack(1)

typedef struct tagLANLHEADER
{
	char	SizeOfHeader[4];
	char	unused1[5];
	char	Version[5];
	char	StationId[3];
	char	Year[3];
	char	Month[3];
	char	Day[3];
	char	FullYear[4];
	char	unused2[43];
} LANLHEADER;

typedef unsigned long MYADDRESS;

typedef struct tagTIME {
    unsigned short HalfDays;
    unsigned short Seconds;
} EOSSTIME;

typedef struct tagTIMEREC
{
	double dt;
	COleDateTime::DateTimeStatus status;
	short sOffset;
	char cVersion[2];
} TIMEREC;

typedef unsigned char DESBLOCK[8];

typedef struct tagGETCHALLENGE {
    unsigned short Type;
} GETCHALLENGE;

typedef struct tagCHALLENGE {
    unsigned short Type;
    unsigned long Seal;         // seal serial number
    unsigned long KeySetNumber; // key-set number
    EOSSTIME      KeySetTime;   // date and time of key-set generation
    unsigned char Version1;     // major version number
    unsigned char Version2;     // minor version standard
    DESBLOCK Token;
} CHALLENGE;

typedef struct tagTIMEDATA {
    unsigned short HalfDays;
    unsigned short Seconds;
    unsigned long Reserved;
} TIMEDATA;

typedef struct tagINITIALS {
   unsigned short Operator1;  // ID of operator 1
   unsigned short Operator2;  // ID of operator 2 or zero
   unsigned short Reserved2;
   unsigned short Reserved3;
} INITIALS;

#define FLAG_FOOPEN  1
#define FLAG_FOFAIL  2
#define FLAG_HOPEN	4
#define FLAG_SHORT	8
#define FLAG_BROKEN 16

typedef struct tagSTATUS {
   MYADDRESS Seal;					//4		0
   EOSSTIME Time;					//2+2	4

   unsigned short Operator1;		//2		8
   unsigned short Operator2;		//2		10
   unsigned char Flags;				//1		12
   signed char Temperature;			//1		13
   unsigned char EmergencyBattery;	//1		14
   unsigned char MainBattery;		//1		15

   unsigned short FlashWarnings;	//2
   unsigned short FlashErrors;		//2
   unsigned short MajoWarnings;		//2
   unsigned short MajoErrors;		//2

   unsigned short EPROMCrc16;		//2
   unsigned char BatID[8];			//8
   char BatteryType[8];				//8
   unsigned short BatteryMfg;		//2
   unsigned long BatteryConsumed;	//4

   unsigned short EBatteryMfg;		//2
   unsigned long EBatteryConsumed;	//4
   unsigned short BatteryInstalled;	//2

   unsigned short EBatteryInstalled;//2
   signed short T0Offset;			//2
   signed short F0Offset;			//2
   unsigned short LaserThreshold;	//2

   unsigned short Laser500uW;		//2
   unsigned short LaserPeak;		//2
   unsigned short LaserAdjDate;		//2
   char LaserId[16];				//16
   unsigned long VacossAttachments;	//4
   unsigned char VacossEnabled;		//1

   unsigned char Reserved[5];		//5
} STATUS;

#define PAGE_SIZE 64
#define PAGES_TOTAL 8192 / PAGE_SIZE

typedef struct tagPAGEHEADER {
   unsigned short Seal;
   unsigned char Log;
   unsigned char Page;
   EOSSTIME Time;
   DESBLOCK Authenticator;
} PAGEHEADER;

typedef PAGEHEADER * PPAGEHEADER;

typedef EOSSTIME SEALEVENT;

typedef struct tagSYSTEMEVENT {
	EOSSTIME Time;
	unsigned short Code;
} SYSTEMEVENT;

typedef struct tagSOHRECORD {
      unsigned short Date;
      signed char MinTemperature;
      signed char MaxTemperature;
      unsigned char EmergencyBattery;
      unsigned char MainBattery;
      unsigned char OnBatteries;
      unsigned char MVEvents;
} SOHRECORD;

#define SEALEVENTS_PER_PAGE ((PAGE_SIZE - sizeof(PAGEHEADER)) / sizeof(SEALEVENT))
#define SYSTEMEVENTS_PER_PAGE ((PAGE_SIZE - sizeof(PAGEHEADER)) / sizeof(SYSTEMEVENT))
#define SOHRECORDS_PER_PAGE ((PAGE_SIZE - sizeof(PAGEHEADER)) / sizeof(SOHRECORD))

typedef struct {
	PAGEHEADER Header;
	SEALEVENT Entries[SEALEVENTS_PER_PAGE];
} SEALLOGPAGE;

typedef struct tagSYSTEMLOGPAGE {
	PAGEHEADER Header;
	SYSTEMEVENT Entries[SYSTEMEVENTS_PER_PAGE];
} SYSTEMLOGPAGE;

typedef struct tagSOHLOGPAGE {
	PAGEHEADER Header;
	SOHRECORD Entries[SOHRECORDS_PER_PAGE];
} SOHLOGPAGE;

typedef union {
	SEALLOGPAGE SealLogPage;
	SYSTEMLOGPAGE SystemLogPage;
	SOHLOGPAGE SoHLogPage; 
} LOGPAGE;

typedef struct tagLOGBOOKINFO {
   unsigned long SealEvents;			//4
   unsigned char SealLogFirstPage;		//1
   unsigned char SealLogLastPage;		//1
   unsigned char SealLogStartPage;		//1
   unsigned char SealLogCurrentPage;	//1

   unsigned long SystemEvents;			//4
   unsigned char SystemLogFirstPage;	//1	
   unsigned char SystemLogLastPage;		//1
   unsigned char SystemLogStartPage;	//1
   unsigned char SystemLogCurrentPage;	//1

   unsigned short SoHRecords;			//2
   unsigned char SoHLogFirstPage;		//1
   unsigned char SoHLogLastPage;		//1
   unsigned char SoHLogStartPage;		//1
   unsigned char SoHLogCurrentPage;		//1

   unsigned char TotalPadPages;			//1
   unsigned char ExStatusFlags;			//1

   unsigned long KeysetNumber;			//4
   EOSSTIME KeysetTime;					//4
} LOGINFO;

typedef union tagREQUEST_DATA {
    TIMEDATA Time;
    INITIALS Initials;
    unsigned char Page[PAGE_SIZE];
    DESBLOCK HashKey;   // the hash key to be used for program memory verification
} REQUEST_DATA;

typedef union tagREPLY_DATA {
    STATUS Status;
    LOGINFO LogInfo;
    unsigned char Page[PAGE_SIZE];
    DESBLOCK KeyedHash; // keyed hash value over the entire program memory
} REPLY_DATA;

typedef struct tagREQUEST {
   unsigned short Type;
   DESBLOCK Authorization;
   unsigned char Command;
   unsigned char Parameter;
   REQUEST_DATA Data;
} REQUEST;

typedef struct tagREPLY {
   unsigned short Type;
   DESBLOCK Token;
   DESBLOCK Authentication;
   REPLY_DATA Data;
} REPLY;

typedef struct tagDENIAL {
   unsigned short Type;
   DESBLOCK Token;
   unsigned short Error;
} DENIAL;

typedef struct tagTRIGGER {
   unsigned short Type;
   unsigned char Flags;
   unsigned char Reserved;
} TRIGGER;

typedef union tagPACKAGE {
    unsigned short Type;
    GETCHALLENGE GetChallenge;
    CHALLENGE Challenge;
    REQUEST Request;
    REPLY Reply;
    DENIAL Denial;
    TRIGGER Trigger;
} PACKAGE;

typedef struct tagHEADER {
   MYADDRESS Receiver;
   MYADDRESS Sender;
   unsigned short Size;
} HEADER;

typedef struct tagFRAME {
   HEADER Header;
   PACKAGE Package;
	short ______crc;	// for sizeof() only!
} FRAME;

typedef struct tagSPACKAGE {
	unsigned short usType;
	unsigned long ulLowest;
	unsigned long ulHighest;
	unsigned long iNodes[32];
} SPACKAGE;

typedef struct tagSFRAME {
	HEADER sHeader;
	SPACKAGE sPackage;
	short ______crc;
} SFRAME;

typedef struct tagIFRAME {
	HEADER sHeader;
	short ______crc;
} IFRAME;

#pragma pack(pop)

