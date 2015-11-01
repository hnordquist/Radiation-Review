@echo off

rem  Batch File to build Raima Object Manager libraries

rem  Arguments
rem  %1 - Database
rem  %2 - Compiler
rem  %3 - Library Type
rem  %4 - Query option


if "X%1" == "X" goto args
if "X%2" == "X" goto args
if "X%3" == "X" goto args

:mkdirs
if not exist %2\*.*       mkdir %2
if not exist %2\%1\*.*    mkdir %2\%1
if not exist %2\%1\%3\*.* mkdir %2\%1\%3
if not exist ..\lib\*.*   mkdir ..\lib
if not exist ..\lib\%2    mkdir ..\lib\%2


:check1
rem check for valid arg 1
if "%1" == "RDM" goto rdm
if "%1" == "rdm" goto rdm
if "%1" == "RDS" goto rds
if "%1" == "rds" goto rds
echo.
echo The first argument must be either RDM or RDS.
echo.
goto args

:rdm
set XRDM=RDM
goto check2

:rds
set XRDM=RDS
goto check2


:check2
rem check for valid arg 2
if "%2" == "BOR"   goto bor
if "%2" == "bor"   goto bor
if "%2" == "MSVC"  goto msvc
if "%2" == "msvc"  goto msvc
echo.
echo The second argument must be one of BOR45, MSVC15X, or MSVC4x.
echo.
goto args


:bor
set XCOMP=BOR
set XCFG=bor
goto check3

:msvc
set XCOMP=MSVC
if "%3" == "dll16"   goto msvc16bit
if "%3" == "DLL16"   goto msvc16bit
if "%3" == "dll32"  goto msvc32bit
if "%3" == "DLL32"  goto msvc32bit
goto args

:msvc16bit
set XCFG=msvc16
goto check3

:msvc32bit
set XCFG=msvc32
goto check3

:check3
rem check for valid arg 3
if "%3" == "DOS"   goto mkdos
if "%3" == "dos"   goto mkdos
if "%3" == "DLL16" goto mkdll16
if "%3" == "dll16" goto mkdll16
if "%3" == "DLL32" goto mkdll32
if "%3" == "dll32" goto mkdll32
echo.
echo The third argument must be one of DOS, DLL16, or DLL32.
echo.
goto args


:mkdos
set XTYPE=DOSLIB
goto check4

:mkdll16
set XTYPE=WINDLL
goto check4

:mkdll32
set XTYPE=WIN32DLL
goto check4

:check4
rem check for valid arg 4
if "%4" == "Q" goto query
if "%4" == "q" goto query
set XQRY=NO
goto build

:query
if "%1" == "RDS" goto noquery
if "%1" == "rds" goto noquery
set XQRY=YES
goto build

:noquery
echo.
echo ROM can only be built to use db_QUERY when being built for RDM.
echo.
goto args


:build
copy ..\bases.gen+..\build\build.gen+..\build\%XCFG%.cfg+mk.gen makefile

if "%2" == "BOR"   goto borland
if "%2" == "bor"   goto borland
if "%2" == "MSVC"  goto microsoft
if "%2" == "msvc"  goto microsoft

:microsoft
nmake RDX=%XRDM% TARGET_TYPE=%XTYPE% CTYPE=%XCFG% COMPILER=%XCOMP% DB_QUERY=%XQRY%
goto exit

:borland
make -DRDX=%XRDM% -DTARGET_TYPE=%XTYPE% -DCTYPE=%XCFG% -DCOMPILER=%XCOMP% -DDB_QUERY=%XQRY%
goto exit


:args
echo.
echo * Usage:     MK {RDM/RDS} {BOR/MSVC} {DOS/DLL16/DLL32} [Q]
echo *
echo * Example:   MK RDM MSVC DLL32 Q
echo * ---------------------------------------------------------------------------
echo * Database:
echo *            RDM      Raima Database Manager 3.20 and greater 
echo *            RDS      Velocis Database Server 1.3.1 (build 35) and greater
echo * ---------------------------------------------------------------------------
echo * Compilers:
echo *            BOR45    Borland C++                         {DOS/DLL16/DLL32}
echo *            MSVC     Microsoft Visual C++(1.5)           {DOS/DLL16}
echo *            MSVC     Microsoft Visual C++(2.x and above) {DLL32}
echo * ---------------------------------------------------------------------------
echo * Libraries:
echo *            DOS      16-bit Large Model DOS Library
echo *            DLL16    16-bit Large Model Windows DLL and Import Library
echo *            DLL32    32-bit Windows DLL and Import Library
echo * ---------------------------------------------------------------------------
echo * Query: ** OPTIONAL **
echo *            Q        Optional support for db_QUERY (RDM only)
echo * ---------------------------------------------------------------------------
echo * 

:exit
set XTYPE=
set XCOMP=
set XQRY=
set XRDM=
