//Check_Signature.h

#ifndef CHECK_SIGNATURE_H
#define CHECK_SIGNATURE_H

/*
  pjm 23-Jan-06  Added comments and compilation guards.

Return values of CheckSignatureEx():

#define FT_LIBRARY_ERROR 			-1
#define FT_NO_CARD 					-2
#define FT_CARD_ERROR 				-3
#define FT_NO_PERSONALITY 			-4
#define FT_NO_INPUT_FILE 			-10
#define FT_EMPTY_INPUT_FILE 		-11
#define FT_INPUT_FILE_IO_ERROR 		-12
#define FT_OUTPUT_FILE_IO_ERROR 	-13
#define FT_INPUT_FILE_INVALID 		-14

char * errorMessages [] = {
	"Success",
	"Crypto Library Error",	// -1
	"N/A",					// -2
	"N/A",					// -3
	"N/A",					// -4
	"N/A",
	"N/A",
	"N/A",
	"N/A",
	"N/A",
	"No input file",		// -10
	"Empty input file",		// -11
	"Error reading from input file",	// -12
	"Error writing to output file",		// -13
	"Input file has no S/MIME format",	// -14
	"Unknown Error"
};
*/



#ifdef _CVI_
	#define CHECK_SIGNATURE_API int __stdcall
	#define CHECK_SIGNATURE_MAIN int __stdcall

#else
	#ifdef CHECK_SIGNATURE_V2_EXPORTS
		#define CHECK_SIGNATURE_API __declspec(dllexport)
	#else
		#define CHECK_SIGNATURE_API __declspec(dllimport)
	#endif
	#define CHECK_SIGNATURE_MAIN BOOL APIENTRY
#endif

CHECK_SIGNATURE_API int GetExtraInfos ( unsigned char ** keyFingerprint, unsigned char ** rootKeyFingerprint,
								   char ** commonName, char ** CA_CommonName,
								   char ** extraComment, unsigned char ** timeStamp );
CHECK_SIGNATURE_API int Check_Signature ( const char * inputFilePath, long * fileOffset, long * fileLength );
CHECK_SIGNATURE_API int CheckSignature ( const char * inputFilePath, long * fileOffset, long * fileLength );
CHECK_SIGNATURE_API int CheckSignatureEx ( const char * inputFilePath, long * fileOffset, long * fileLength, 
				unsigned char ** fingerPrint, unsigned char ** timeStamp );
CHECK_SIGNATURE_API int Check_Signature_Ex ( const char * inputFilePath, long * fileOffset, long * fileLength, 
					unsigned char ** publicKey, unsigned char ** timeStamp );
CHECK_SIGNATURE_API int GetDeenvStream ( const char * inputFilePath, long * fileOffset, long * fileLength );

// to enable backward compatibility with the old version 1 custom format
//#define VER1_FORMAT_COMPATIBILITY 1

// to disable backward compatibility with the old version 1 custom format
#define VER1_FORMAT_COMPATIBILITY 0 

#endif //CHECK_SIGNATURE_H

