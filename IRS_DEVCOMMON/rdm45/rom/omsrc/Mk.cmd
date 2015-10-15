@echo off

rem  Batch File to build Raima Object Manager libraries

rem  Arguments
rem  %1 - Database
rem  %2 - Compiler
rem  %3 - Query option


if "X%1" == "X" goto args
if "X%2" == "X" goto args

:mkdirs
if not exist %2\*.*       mkdir %2
if not exist %2\%1\*.*    mkdir %2\%1
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
if "%2" == "ICC"     goto icc
if "%2" == "icc"     goto icc
if "%2" == "BCC"     goto bcc
if "%2" == "bcc"     goto bcc
echo.
echo The second argument must be either ICC or BCC.
echo.
goto args


:icc
set XCOMP=ICC
goto check3


:bcc
set XCOMP=BCC
goto check3


:check3
rem check for valid arg 3
if "%3" == "Q" goto query
if "%3" == "q" goto query
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
copy ..\bases.gen+..\build\build.gen+..\build\%2.cfg+mk.gen makefile
set XTYPE=OS2DLL

if "%1" == "ICC" goto iccmake
if "%1" == "icc" goto iccmake
if "%1" == "BCC" goto bccmake
if "%1" == "bcc" goto bccmake


:iccmake
nmake RDX=%XRDM% TARGET_TYPE=%XTYPE% COMPILER=%XCOMP% DB_QUERY=%XQRY%
goto exit


:bccmake
make -DRDX=%XRDM% -DTARGET_TYPE=%XTYPE% -DCOMPILER=%XCOMP% -DDB_QUERY=%XQRY%
goto exit


:args
echo.
echo * Usage:     MK {RDM/RDS} {ICC/BCC} [Q]
echo *
echo * Example:   MK ICC Q
echo * ---------------------------------------------------------------------------
echo * Database:
echo *            RDM      Raima Database Manager 3.20 and greater 
echo *            RDS      Velocis Database Server 1.3.1 (build 35) and greater
echo * ---------------------------------------------------------------------------
echo * Compilers:
echo *            ICC      IBM VisualAge C++ for OS/2
echo *            BCC      Borland C++ for OS/2
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
