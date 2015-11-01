@echo off

rem  Batch File to build Raima Object Manager example

rem  Arguments
rem  %1 - Database
rem  %2 - Compiler
rem  %3 - Library Type


if "X%1" == "X" goto args
if "X%2" == "X" goto args
if "X%3" == "X" goto args


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
echo The second argument must be one of BOR, or MSVC.
echo.
goto args


:bor
set XCOMP=BOR
set XCFG=BOR
goto check3

:msvc
set XCOMP=MSVC
if "%3" == "WIN16"   goto msvc16bit
if "%3" == "win16"   goto msvc16bit
if "%3" == "WIN32"  goto msvc32bit
if "%3" == "win32"  goto msvc32bit
goto check3

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
if "%3" == "WIN16" goto mkwin16
if "%3" == "win16" goto mkwin16
if "%3" == "WIN32" goto mkwin32
if "%3" == "win32" goto mkwin32
echo.
echo The third argument must be one of DOS, WIN16, or WIN32.
echo.
goto args


:mkdos
set XTYPE=DOSAPP
set XQW=
if "%2" == "bor" goto b45dos
if "%2" == "BOR" goto b45dos
if "%2" == "msvc" goto m80dos
if "%2" == "MSVC" goto m80dos
goto check4

:b45dos
set RDL=b45
goto check4

:m80dos
set RDL=m80
goto check4

:mkwin16
set XTYPE=WINAPP
set XQW=YES
goto check4

:mkwin32
set XTYPE=WIN32APP
set XQW=YES
goto check4

:check4
rem check for valid arg 4
if "%4" == "Q" goto query
if "%4" == "q" goto query
set XQRY=NO
goto build

:query
if "%1" == "RDM" goto noquery
if "%1" == "rdm" goto noquery
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
copy ..\..\bases.gen + ..\..\build\build.gen + ..\..\build\%XCFG%.cfg + mk.gen makefile

if "%2" == "BOR"   goto borland
if "%2" == "bor"   goto borland
if "%2" == "MSVC" goto microsoft
if "%2" == "msvc" goto microsoft

:microsoft
nmake RDX=%XRDM% TARGET_TYPE=%XTYPE% CTYPE=%XCFG% COMPILER=%XCOMP% RLIB=%RDL% QUICKWINAPP=%XQW%
goto exit

:borland
make -DRDX=%XRDM% -DTARGET_TYPE=%XTYPE% -DCTYPE=%XCFG% -DCOMPILER=%XCOMP% -DRLIB=%RDL% -DQUICKWINAPP=%XQW%
goto exit


:args
echo.
echo * Usage:     MK {RDM/RDS} {BOR/MSVC} {DOS/WIN16/WIN32} 
echo *
echo * Example:   MK RDM MSVC WIN32 
echo * ---------------------------------------------------------------------------
echo * Database:
echo *            RDM      Raima Database Manager 3.20 and greater 
echo *            RDS      Velocis Database Server 1.3.1 (build 35) and greater
echo * ---------------------------------------------------------------------------
echo * Compilers:
echo *            BOR      Borland C++                          {DOS/WIN16/WIN32}
echo *            MSVC     Microsoft Visual C++ 1.5x            {DOS/WIN16}
echo *            MSVC     Microsoft Visual C++ (2.x and above) {WIN32}
echo * ---------------------------------------------------------------------------
echo * Libraries:
echo *            DOS      16-bit Large Model DOS Application
echo *            WIN16    16-bit Large Model Windows Application 
echo *            WIN32    32-bit Windows APP 
echo * ---------------------------------------------------------------------------
echo * 

:exit
set XTYPE=
set XCOMP=
set XQRY=
set XRDM=
