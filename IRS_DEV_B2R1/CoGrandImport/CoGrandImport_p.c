
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Wed Nov 12 09:27:07 2014
 */
/* Compiler settings for CoGrandImport.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "CoGrandImport.h"

#define TYPE_FORMAT_STRING_SIZE   1071                              
#define PROC_FORMAT_STRING_SIZE   1037                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IGrandImport_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IGrandImport_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure AddData */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 10 */	NdrFcShort( 0xc ),	/* 12 */
/* 12 */	NdrFcShort( 0x22 ),	/* 34 */
/* 14 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter sFacilityID */

/* 16 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 18 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 20 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrFilename */

/* 22 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 24 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 26 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bOverwrite */

/* 28 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 30 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 32 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrResult */

/* 34 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 36 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 38 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter pbResult */

/* 40 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 42 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 44 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 46 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 48 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 50 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure BackupDataStore */

/* 52 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 54 */	NdrFcLong( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x8 ),	/* 8 */
/* 60 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 62 */	NdrFcShort( 0x12 ),	/* 18 */
/* 64 */	NdrFcShort( 0x22 ),	/* 34 */
/* 66 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter sFacilityID */

/* 68 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 70 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 72 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrDestinationPath */

/* 74 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 76 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 78 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bQuestion */

/* 80 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 82 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 84 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bDefault */

/* 86 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 88 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 90 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 92 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 94 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 96 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 98 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 100 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 102 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloseDataStore */

/* 104 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 106 */	NdrFcLong( 0x0 ),	/* 0 */
/* 110 */	NdrFcShort( 0x9 ),	/* 9 */
/* 112 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 116 */	NdrFcShort( 0x8 ),	/* 8 */
/* 118 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 120 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 122 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateNewDataStore */

/* 126 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 128 */	NdrFcLong( 0x0 ),	/* 0 */
/* 132 */	NdrFcShort( 0xa ),	/* 10 */
/* 134 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 136 */	NdrFcShort( 0x6 ),	/* 6 */
/* 138 */	NdrFcShort( 0x22 ),	/* 34 */
/* 140 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter sFacilityID */

/* 142 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 144 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 146 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 148 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 150 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 152 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 154 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 156 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 158 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteData */

/* 160 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 162 */	NdrFcLong( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0xb ),	/* 11 */
/* 168 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 170 */	NdrFcShort( 0x2e ),	/* 46 */
/* 172 */	NdrFcShort( 0x22 ),	/* 34 */
/* 174 */	0x4,		/* Oi2 Flags:  has return, */
			0x6,		/* 6 */

	/* Parameter sFacilityID */

/* 176 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 178 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 180 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 182 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 184 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dtStartDate */

/* 188 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 190 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 192 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter dtEndDate */

/* 194 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 196 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 198 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pbStatus */

/* 200 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 202 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 204 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 206 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 208 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 210 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteDataStore */

/* 212 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 214 */	NdrFcLong( 0x0 ),	/* 0 */
/* 218 */	NdrFcShort( 0xc ),	/* 12 */
/* 220 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 222 */	NdrFcShort( 0x6 ),	/* 6 */
/* 224 */	NdrFcShort( 0x22 ),	/* 34 */
/* 226 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter sFacilityID */

/* 228 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 230 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 232 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbStatus */

/* 234 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 236 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 238 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 240 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 242 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 244 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataByChannel */

/* 246 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 248 */	NdrFcLong( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0xd ),	/* 13 */
/* 254 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 256 */	NdrFcShort( 0x34 ),	/* 52 */
/* 258 */	NdrFcShort( 0x22 ),	/* 34 */
/* 260 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter sFacilityID */

/* 262 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 264 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 266 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 268 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 270 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 272 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter sChannelID */

/* 274 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 276 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 278 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter StartTime */

/* 280 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 282 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 284 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter EndTime */

/* 286 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 288 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 290 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pvData */

/* 292 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 294 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 296 */	NdrFcShort( 0x406 ),	/* Type Offset=1030 */

	/* Parameter pbResult */

/* 298 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 300 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 302 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 304 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 306 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 308 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataByStation */

/* 310 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 312 */	NdrFcLong( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0xe ),	/* 14 */
/* 318 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 320 */	NdrFcShort( 0x2e ),	/* 46 */
/* 322 */	NdrFcShort( 0x22 ),	/* 34 */
/* 324 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x7,		/* 7 */

	/* Parameter sFacilityID */

/* 326 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 328 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 330 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 332 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 334 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 336 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter StartTime */

/* 338 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 342 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter EndTime */

/* 344 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 346 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 348 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pvData */

/* 350 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 352 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 354 */	NdrFcShort( 0x406 ),	/* Type Offset=1030 */

	/* Parameter pbResult */

/* 356 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 358 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 360 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 364 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataStatistics */

/* 368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 374 */	NdrFcShort( 0xf ),	/* 15 */
/* 376 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
/* 378 */	NdrFcShort( 0x34 ),	/* 52 */
/* 380 */	NdrFcShort( 0x86 ),	/* 134 */
/* 382 */	0x4,		/* Oi2 Flags:  has return, */
			0xa,		/* 10 */

	/* Parameter sFacilityID */

/* 384 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 388 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 390 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 392 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter sChannelID */

/* 396 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 398 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 400 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter StartTime */

/* 402 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 404 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 406 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter EndTime */

/* 408 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 410 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 412 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pActualStartTime */

/* 414 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 416 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 418 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pActualEndTime */

/* 420 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 422 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 424 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter plNumMembers */

/* 426 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 428 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 432 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 434 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 436 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 440 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataStorePath */

/* 444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x10 ),	/* 16 */
/* 452 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 454 */	NdrFcShort( 0x6 ),	/* 6 */
/* 456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 458 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x3,		/* 3 */

	/* Parameter sFacilityID */

/* 460 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 462 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 464 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrStorePath */

/* 466 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 468 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 470 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 472 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 474 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataStoreSize */

/* 478 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 480 */	NdrFcLong( 0x0 ),	/* 0 */
/* 484 */	NdrFcShort( 0x11 ),	/* 17 */
/* 486 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 488 */	NdrFcShort( 0x6 ),	/* 6 */
/* 490 */	NdrFcShort( 0x46 ),	/* 70 */
/* 492 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter sFacilityID */

/* 494 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 496 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 498 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pdSizeInBytes */

/* 500 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 502 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 504 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 506 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 508 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 510 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 512 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 514 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 516 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataStoreStatus */

/* 518 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 520 */	NdrFcLong( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x12 ),	/* 18 */
/* 526 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 528 */	NdrFcShort( 0x6 ),	/* 6 */
/* 530 */	NdrFcShort( 0x24 ),	/* 36 */
/* 532 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter sFacilityID */

/* 534 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 536 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 538 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter plStatus */

/* 540 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 542 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 546 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDataStoreTimeRange */

/* 552 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 554 */	NdrFcLong( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x13 ),	/* 19 */
/* 560 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 562 */	NdrFcShort( 0x14 ),	/* 20 */
/* 564 */	NdrFcShort( 0x6a ),	/* 106 */
/* 566 */	0x4,		/* Oi2 Flags:  has return, */
			0x7,		/* 7 */

	/* Parameter sFacilityID */

/* 568 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 570 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 572 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 574 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 576 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 578 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bErrDisplay */

/* 580 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 582 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 584 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter FirstTimestamp */

/* 586 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 588 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 590 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter LastTimestamp */

/* 592 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 594 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 596 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 598 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 600 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 602 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 604 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 606 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDayStatistics */

/* 610 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 616 */	NdrFcShort( 0x14 ),	/* 20 */
/* 618 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
/* 620 */	NdrFcShort( 0x2e ),	/* 46 */
/* 622 */	NdrFcShort( 0x86 ),	/* 134 */
/* 624 */	0x4,		/* Oi2 Flags:  has return, */
			0x9,		/* 9 */

	/* Parameter sFacilityID */

/* 626 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 628 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 630 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 632 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 634 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter StartTime */

/* 638 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 640 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 642 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter EndTime */

/* 644 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 646 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 648 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pActualStartTime */

/* 650 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 652 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 654 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pActualEndTime */

/* 656 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 658 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 660 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter plNumMembers */

/* 662 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 664 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 666 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 668 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 670 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 672 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 674 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 676 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
/* 678 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetDaySummaryData */

/* 680 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 682 */	NdrFcLong( 0x0 ),	/* 0 */
/* 686 */	NdrFcShort( 0x15 ),	/* 21 */
/* 688 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 690 */	NdrFcShort( 0x2e ),	/* 46 */
/* 692 */	NdrFcShort( 0x22 ),	/* 34 */
/* 694 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x7,		/* 7 */

	/* Parameter sFacilityID */

/* 696 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 698 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 700 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter lStationID */

/* 702 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 704 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter StartTime */

/* 708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 710 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 712 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter EndTime */

/* 714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 716 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 718 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pvDay */

/* 720 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 722 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 724 */	NdrFcShort( 0x406 ),	/* Type Offset=1030 */

	/* Parameter pbResult */

/* 726 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 728 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 730 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 732 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 734 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFilesList */

/* 738 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 740 */	NdrFcLong( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x16 ),	/* 22 */
/* 746 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 748 */	NdrFcShort( 0x12 ),	/* 18 */
/* 750 */	NdrFcShort( 0x8 ),	/* 8 */
/* 752 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter sFacilityID */

/* 754 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 756 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 758 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrRootDir */

/* 760 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 762 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 764 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bIncludeSubDirs */

/* 766 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 768 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 770 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bMostRecent */

/* 772 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 774 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 776 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter psaFilesList */

/* 778 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 780 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 782 */	NdrFcShort( 0x406 ),	/* Type Offset=1030 */

	/* Return value */

/* 784 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 786 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 788 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetLastError */

/* 790 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 792 */	NdrFcLong( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x17 ),	/* 23 */
/* 798 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 800 */	NdrFcShort( 0x6 ),	/* 6 */
/* 802 */	NdrFcShort( 0x24 ),	/* 36 */
/* 804 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter sFacilityID */

/* 806 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 808 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 810 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbstrErrortext */

/* 812 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 814 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 816 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter plError */

/* 818 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 820 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 824 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 826 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetParameters */

/* 830 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 832 */	NdrFcLong( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x18 ),	/* 24 */
/* 838 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 840 */	NdrFcShort( 0x6 ),	/* 6 */
/* 842 */	NdrFcShort( 0x22 ),	/* 34 */
/* 844 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x4,		/* 4 */

	/* Parameter sFacilityID */

/* 846 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 848 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 850 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pvParms */

/* 852 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 854 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 856 */	NdrFcShort( 0x406 ),	/* Type Offset=1030 */

	/* Parameter pbResult */

/* 858 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 860 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 862 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 866 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InitializeDataStore */

/* 870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x19 ),	/* 25 */
/* 878 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 880 */	NdrFcShort( 0xc ),	/* 12 */
/* 882 */	NdrFcShort( 0x22 ),	/* 34 */
/* 884 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter sFacilityID */

/* 886 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 888 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 890 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bQuestion */

/* 892 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 894 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 896 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbSuccess */

/* 898 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 900 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 902 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 904 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 906 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 908 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenDataStore */

/* 910 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 912 */	NdrFcLong( 0x0 ),	/* 0 */
/* 916 */	NdrFcShort( 0x1a ),	/* 26 */
/* 918 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 920 */	NdrFcShort( 0x6 ),	/* 6 */
/* 922 */	NdrFcShort( 0x22 ),	/* 34 */
/* 924 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter sFacilityID */

/* 926 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 928 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 930 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbSuccess */

/* 932 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 934 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 936 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 938 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 940 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RestoreDataStore */

/* 944 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 946 */	NdrFcLong( 0x0 ),	/* 0 */
/* 950 */	NdrFcShort( 0x1b ),	/* 27 */
/* 952 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 954 */	NdrFcShort( 0x12 ),	/* 18 */
/* 956 */	NdrFcShort( 0x22 ),	/* 34 */
/* 958 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter sFacilityID */

/* 960 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 962 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 964 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrSourcePath */

/* 966 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 970 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bQuestion */

/* 972 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 974 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 976 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bDefault */

/* 978 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 980 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 982 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbResult */

/* 984 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 986 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 988 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 990 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 992 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 994 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetParameters */

/* 996 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 998 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1002 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1004 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 1006 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1008 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1010 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter sFacilityID */

/* 1012 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1014 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1016 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter vParms */

/* 1018 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1020 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1022 */	NdrFcShort( 0x424 ),	/* Type Offset=1060 */

	/* Parameter pbResult */

/* 1024 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1026 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1028 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1030 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1032 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1034 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xc ),	/* Offset= 12 (16) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 16 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 18 */	NdrFcShort( 0x8 ),	/* 8 */
/* 20 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (6) */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 26 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */
/* 30 */	NdrFcShort( 0x4 ),	/* 4 */
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (2) */
/* 36 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 38 */	NdrFcShort( 0x6 ),	/* Offset= 6 (44) */
/* 40 */	
			0x13, 0x0,	/* FC_OP */
/* 42 */	NdrFcShort( 0xffffffe6 ),	/* Offset= -26 (16) */
/* 44 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x4 ),	/* 4 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (40) */
/* 54 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 56 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 58 */	
			0x11, 0x0,	/* FC_RP */
/* 60 */	NdrFcShort( 0x3ca ),	/* Offset= 970 (1030) */
/* 62 */	
			0x13, 0x0,	/* FC_OP */
/* 64 */	NdrFcShort( 0x3b2 ),	/* Offset= 946 (1010) */
/* 66 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 68 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 70 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 72 */	NdrFcShort( 0x2 ),	/* Offset= 2 (74) */
/* 74 */	NdrFcShort( 0x10 ),	/* 16 */
/* 76 */	NdrFcShort( 0x2f ),	/* 47 */
/* 78 */	NdrFcLong( 0x14 ),	/* 20 */
/* 82 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 84 */	NdrFcLong( 0x3 ),	/* 3 */
/* 88 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 90 */	NdrFcLong( 0x11 ),	/* 17 */
/* 94 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 96 */	NdrFcLong( 0x2 ),	/* 2 */
/* 100 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 102 */	NdrFcLong( 0x4 ),	/* 4 */
/* 106 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 108 */	NdrFcLong( 0x5 ),	/* 5 */
/* 112 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 114 */	NdrFcLong( 0xb ),	/* 11 */
/* 118 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 120 */	NdrFcLong( 0xa ),	/* 10 */
/* 124 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 126 */	NdrFcLong( 0x6 ),	/* 6 */
/* 130 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (362) */
/* 132 */	NdrFcLong( 0x7 ),	/* 7 */
/* 136 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 138 */	NdrFcLong( 0x8 ),	/* 8 */
/* 142 */	NdrFcShort( 0xffffff9a ),	/* Offset= -102 (40) */
/* 144 */	NdrFcLong( 0xd ),	/* 13 */
/* 148 */	NdrFcShort( 0xdc ),	/* Offset= 220 (368) */
/* 150 */	NdrFcLong( 0x9 ),	/* 9 */
/* 154 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (386) */
/* 156 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 160 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (404) */
/* 162 */	NdrFcLong( 0x24 ),	/* 36 */
/* 166 */	NdrFcShort( 0x302 ),	/* Offset= 770 (936) */
/* 168 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 172 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (936) */
/* 174 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 178 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (940) */
/* 180 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 184 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (944) */
/* 186 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 190 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (948) */
/* 192 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 196 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (952) */
/* 198 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 202 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (956) */
/* 204 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 208 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (960) */
/* 210 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 214 */	NdrFcShort( 0x2da ),	/* Offset= 730 (944) */
/* 216 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 220 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (948) */
/* 222 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 226 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (964) */
/* 228 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 232 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (960) */
/* 234 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 238 */	NdrFcShort( 0x2da ),	/* Offset= 730 (968) */
/* 240 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 244 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (972) */
/* 246 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 250 */	NdrFcShort( 0x2d6 ),	/* Offset= 726 (976) */
/* 252 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 256 */	NdrFcShort( 0x2d4 ),	/* Offset= 724 (980) */
/* 258 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 262 */	NdrFcShort( 0x2d2 ),	/* Offset= 722 (984) */
/* 264 */	NdrFcLong( 0x10 ),	/* 16 */
/* 268 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 270 */	NdrFcLong( 0x12 ),	/* 18 */
/* 274 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 276 */	NdrFcLong( 0x13 ),	/* 19 */
/* 280 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 282 */	NdrFcLong( 0x15 ),	/* 21 */
/* 286 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 288 */	NdrFcLong( 0x16 ),	/* 22 */
/* 292 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 294 */	NdrFcLong( 0x17 ),	/* 23 */
/* 298 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 300 */	NdrFcLong( 0xe ),	/* 14 */
/* 304 */	NdrFcShort( 0x2b0 ),	/* Offset= 688 (992) */
/* 306 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 310 */	NdrFcShort( 0x2b4 ),	/* Offset= 692 (1002) */
/* 312 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 316 */	NdrFcShort( 0x2b2 ),	/* Offset= 690 (1006) */
/* 318 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 322 */	NdrFcShort( 0x26e ),	/* Offset= 622 (944) */
/* 324 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 328 */	NdrFcShort( 0x26c ),	/* Offset= 620 (948) */
/* 330 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 334 */	NdrFcShort( 0x26a ),	/* Offset= 618 (952) */
/* 336 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 340 */	NdrFcShort( 0x260 ),	/* Offset= 608 (948) */
/* 342 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 346 */	NdrFcShort( 0x25a ),	/* Offset= 602 (948) */
/* 348 */	NdrFcLong( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* Offset= 0 (352) */
/* 354 */	NdrFcLong( 0x1 ),	/* 1 */
/* 358 */	NdrFcShort( 0x0 ),	/* Offset= 0 (358) */
/* 360 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (359) */
/* 362 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 364 */	NdrFcShort( 0x8 ),	/* 8 */
/* 366 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 368 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 374 */	NdrFcShort( 0x0 ),	/* 0 */
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 378 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 380 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 382 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 384 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 386 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 388 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 394 */	NdrFcShort( 0x0 ),	/* 0 */
/* 396 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 398 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 400 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 402 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 404 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 406 */	NdrFcShort( 0x2 ),	/* Offset= 2 (408) */
/* 408 */	
			0x13, 0x0,	/* FC_OP */
/* 410 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (918) */
/* 412 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 414 */	NdrFcShort( 0x18 ),	/* 24 */
/* 416 */	NdrFcShort( 0xa ),	/* 10 */
/* 418 */	NdrFcLong( 0x8 ),	/* 8 */
/* 422 */	NdrFcShort( 0x58 ),	/* Offset= 88 (510) */
/* 424 */	NdrFcLong( 0xd ),	/* 13 */
/* 428 */	NdrFcShort( 0x78 ),	/* Offset= 120 (548) */
/* 430 */	NdrFcLong( 0x9 ),	/* 9 */
/* 434 */	NdrFcShort( 0x94 ),	/* Offset= 148 (582) */
/* 436 */	NdrFcLong( 0xc ),	/* 12 */
/* 440 */	NdrFcShort( 0xbc ),	/* Offset= 188 (628) */
/* 442 */	NdrFcLong( 0x24 ),	/* 36 */
/* 446 */	NdrFcShort( 0x114 ),	/* Offset= 276 (722) */
/* 448 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 452 */	NdrFcShort( 0x130 ),	/* Offset= 304 (756) */
/* 454 */	NdrFcLong( 0x10 ),	/* 16 */
/* 458 */	NdrFcShort( 0x148 ),	/* Offset= 328 (786) */
/* 460 */	NdrFcLong( 0x2 ),	/* 2 */
/* 464 */	NdrFcShort( 0x160 ),	/* Offset= 352 (816) */
/* 466 */	NdrFcLong( 0x3 ),	/* 3 */
/* 470 */	NdrFcShort( 0x178 ),	/* Offset= 376 (846) */
/* 472 */	NdrFcLong( 0x14 ),	/* 20 */
/* 476 */	NdrFcShort( 0x190 ),	/* Offset= 400 (876) */
/* 478 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (477) */
/* 480 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 482 */	NdrFcShort( 0x4 ),	/* 4 */
/* 484 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 490 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 492 */	NdrFcShort( 0x4 ),	/* 4 */
/* 494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 496 */	NdrFcShort( 0x1 ),	/* 1 */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	0x13, 0x0,	/* FC_OP */
/* 504 */	NdrFcShort( 0xfffffe18 ),	/* Offset= -488 (16) */
/* 506 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 508 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 510 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 514 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 516 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 518 */	NdrFcShort( 0x4 ),	/* 4 */
/* 520 */	NdrFcShort( 0x4 ),	/* 4 */
/* 522 */	0x11, 0x0,	/* FC_RP */
/* 524 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (480) */
/* 526 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 528 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 530 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 534 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 538 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 542 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 544 */	NdrFcShort( 0xffffff50 ),	/* Offset= -176 (368) */
/* 546 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 548 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 550 */	NdrFcShort( 0x8 ),	/* 8 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x6 ),	/* Offset= 6 (560) */
/* 556 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 558 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 560 */	
			0x11, 0x0,	/* FC_RP */
/* 562 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (530) */
/* 564 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 568 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 572 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 576 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 578 */	NdrFcShort( 0xffffff40 ),	/* Offset= -192 (386) */
/* 580 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 582 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 584 */	NdrFcShort( 0x8 ),	/* 8 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x6 ),	/* Offset= 6 (594) */
/* 590 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 592 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 594 */	
			0x11, 0x0,	/* FC_RP */
/* 596 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (564) */
/* 598 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 600 */	NdrFcShort( 0x4 ),	/* 4 */
/* 602 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 608 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 610 */	NdrFcShort( 0x4 ),	/* 4 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x1 ),	/* 1 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	0x13, 0x0,	/* FC_OP */
/* 622 */	NdrFcShort( 0x184 ),	/* Offset= 388 (1010) */
/* 624 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 626 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 628 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 630 */	NdrFcShort( 0x8 ),	/* 8 */
/* 632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 634 */	NdrFcShort( 0x6 ),	/* Offset= 6 (640) */
/* 636 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 638 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 640 */	
			0x11, 0x0,	/* FC_RP */
/* 642 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (598) */
/* 644 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 646 */	NdrFcLong( 0x2f ),	/* 47 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 656 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 658 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 660 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 662 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 664 */	NdrFcShort( 0x1 ),	/* 1 */
/* 666 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 668 */	NdrFcShort( 0x4 ),	/* 4 */
/* 670 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 672 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 674 */	NdrFcShort( 0x10 ),	/* 16 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0xa ),	/* Offset= 10 (688) */
/* 680 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 682 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 684 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (644) */
/* 686 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 688 */	
			0x13, 0x0,	/* FC_OP */
/* 690 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (662) */
/* 692 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 694 */	NdrFcShort( 0x4 ),	/* 4 */
/* 696 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 702 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 704 */	NdrFcShort( 0x4 ),	/* 4 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x1 ),	/* 1 */
/* 710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 714 */	0x13, 0x0,	/* FC_OP */
/* 716 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (672) */
/* 718 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 720 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 722 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 724 */	NdrFcShort( 0x8 ),	/* 8 */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x6 ),	/* Offset= 6 (734) */
/* 730 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 732 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 734 */	
			0x11, 0x0,	/* FC_RP */
/* 736 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (692) */
/* 738 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 740 */	NdrFcShort( 0x8 ),	/* 8 */
/* 742 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 744 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 746 */	NdrFcShort( 0x10 ),	/* 16 */
/* 748 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 750 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 752 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (738) */
			0x5b,		/* FC_END */
/* 756 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 758 */	NdrFcShort( 0x18 ),	/* 24 */
/* 760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 762 */	NdrFcShort( 0xa ),	/* Offset= 10 (772) */
/* 764 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 766 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 768 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (744) */
/* 770 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 772 */	
			0x11, 0x0,	/* FC_RP */
/* 774 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (530) */
/* 776 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 778 */	NdrFcShort( 0x1 ),	/* 1 */
/* 780 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 784 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 786 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 790 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 792 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 794 */	NdrFcShort( 0x4 ),	/* 4 */
/* 796 */	NdrFcShort( 0x4 ),	/* 4 */
/* 798 */	0x13, 0x0,	/* FC_OP */
/* 800 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (776) */
/* 802 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 804 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 806 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 808 */	NdrFcShort( 0x2 ),	/* 2 */
/* 810 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 816 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 818 */	NdrFcShort( 0x8 ),	/* 8 */
/* 820 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 822 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 824 */	NdrFcShort( 0x4 ),	/* 4 */
/* 826 */	NdrFcShort( 0x4 ),	/* 4 */
/* 828 */	0x13, 0x0,	/* FC_OP */
/* 830 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (806) */
/* 832 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 834 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 836 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 838 */	NdrFcShort( 0x4 ),	/* 4 */
/* 840 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 846 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 850 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 852 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 854 */	NdrFcShort( 0x4 ),	/* 4 */
/* 856 */	NdrFcShort( 0x4 ),	/* 4 */
/* 858 */	0x13, 0x0,	/* FC_OP */
/* 860 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (836) */
/* 862 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 864 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 866 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 868 */	NdrFcShort( 0x8 ),	/* 8 */
/* 870 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 876 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 880 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 882 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 884 */	NdrFcShort( 0x4 ),	/* 4 */
/* 886 */	NdrFcShort( 0x4 ),	/* 4 */
/* 888 */	0x13, 0x0,	/* FC_OP */
/* 890 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (866) */
/* 892 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 894 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 896 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 898 */	NdrFcShort( 0x8 ),	/* 8 */
/* 900 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 902 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 904 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 910 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 912 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 914 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (896) */
/* 916 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 918 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 920 */	NdrFcShort( 0x28 ),	/* 40 */
/* 922 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (904) */
/* 924 */	NdrFcShort( 0x0 ),	/* Offset= 0 (924) */
/* 926 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 928 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 930 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 932 */	NdrFcShort( 0xfffffdf8 ),	/* Offset= -520 (412) */
/* 934 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 936 */	
			0x13, 0x0,	/* FC_OP */
/* 938 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (672) */
/* 940 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 942 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 944 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 946 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 948 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 950 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 952 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 954 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 956 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 958 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 960 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 962 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 964 */	
			0x13, 0x0,	/* FC_OP */
/* 966 */	NdrFcShort( 0xfffffda4 ),	/* Offset= -604 (362) */
/* 968 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 970 */	NdrFcShort( 0xfffffc5e ),	/* Offset= -930 (40) */
/* 972 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 974 */	NdrFcShort( 0xfffffda2 ),	/* Offset= -606 (368) */
/* 976 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 978 */	NdrFcShort( 0xfffffdb0 ),	/* Offset= -592 (386) */
/* 980 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 982 */	NdrFcShort( 0xfffffdbe ),	/* Offset= -578 (404) */
/* 984 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 986 */	NdrFcShort( 0x2 ),	/* Offset= 2 (988) */
/* 988 */	
			0x13, 0x0,	/* FC_OP */
/* 990 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1010) */
/* 992 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 994 */	NdrFcShort( 0x10 ),	/* 16 */
/* 996 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 998 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1000 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1002 */	
			0x13, 0x0,	/* FC_OP */
/* 1004 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (992) */
/* 1006 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1008 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1010 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1012 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1016 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1016) */
/* 1018 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1020 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1022 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1024 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1026 */	NdrFcShort( 0xfffffc40 ),	/* Offset= -960 (66) */
/* 1028 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1030 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1032 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1034 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1036 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1038 */	NdrFcShort( 0xfffffc30 ),	/* Offset= -976 (62) */
/* 1040 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1042 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1044 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1046 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1048 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1050 */	NdrFcShort( 0xffffffec ),	/* Offset= -20 (1030) */
/* 1052 */	
			0x11, 0x0,	/* FC_RP */
/* 1054 */	NdrFcShort( 0xfffffc0e ),	/* Offset= -1010 (44) */
/* 1056 */	
			0x12, 0x0,	/* FC_UP */
/* 1058 */	NdrFcShort( 0xffffffd0 ),	/* Offset= -48 (1010) */
/* 1060 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1062 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1064 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1068 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1056) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IGrandImport, ver. 0.0,
   GUID={0xBD000CB1,0x05DF,0x11D6,{0x81,0x76,0x00,0xC0,0x4F,0x60,0xE8,0x9B}} */

#pragma code_seg(".orpc")
static const unsigned short IGrandImport_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    52,
    104,
    126,
    160,
    212,
    246,
    310,
    368,
    444,
    478,
    518,
    552,
    610,
    680,
    738,
    790,
    830,
    870,
    910,
    944,
    996
    };

static const MIDL_STUBLESS_PROXY_INFO IGrandImport_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IGrandImport_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IGrandImport_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IGrandImport_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(29) _IGrandImportProxyVtbl = 
{
    &IGrandImport_ProxyInfo,
    &IID_IGrandImport,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::AddData */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::BackupDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::CloseDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::CreateNewDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::DeleteData */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::DeleteDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataByChannel */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataByStation */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataStatistics */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataStorePath */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataStoreSize */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataStoreStatus */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDataStoreTimeRange */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDayStatistics */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetDaySummaryData */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetFilesList */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetLastError */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::GetParameters */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::InitializeDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::OpenDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::RestoreDataStore */ ,
    (void *) (INT_PTR) -1 /* IGrandImport::SetParameters */
};


static const PRPC_STUB_FUNCTION IGrandImport_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IGrandImportStubVtbl =
{
    &IID_IGrandImport,
    &IGrandImport_ServerInfo,
    29,
    &IGrandImport_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x600015b, /* MIDL Version 6.0.347 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _CoGrandImport_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IGrandImportProxyVtbl,
    0
};

const CInterfaceStubVtbl * _CoGrandImport_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IGrandImportStubVtbl,
    0
};

PCInterfaceName const _CoGrandImport_InterfaceNamesList[] = 
{
    "IGrandImport",
    0
};

const IID *  _CoGrandImport_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _CoGrandImport_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _CoGrandImport, pIID, n)

int __stdcall _CoGrandImport_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_CoGrandImport_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo CoGrandImport_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _CoGrandImport_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _CoGrandImport_StubVtblList,
    (const PCInterfaceName * ) & _CoGrandImport_InterfaceNamesList,
    (const IID ** ) & _CoGrandImport_BaseIIDList,
    & _CoGrandImport_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

