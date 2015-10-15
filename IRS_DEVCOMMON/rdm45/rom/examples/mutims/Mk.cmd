@echo off

rem  Batch File to build Raima Object Manager example program

rem  Arguments
rem  %1 - Database Engine
rem  %2 - Compiler


if "X%1" == "X" goto args
if "X%2" == "X" goto args


rem check for valid arg 1
if "%1" == "RDM" goto rdm
if "%1" == "rdm" goto rdm
if "%1" == "RDS" goto rds
if "%1" == "rds" goto rds
goto args

:rdm
set XRDM=RDM
goto check2

:rds
set XRDM=RDS
goto check2


:check2
if "%2" == "ICC" goto icc
if "%2" == "icc" goto icc
if "%2" == "BCC" goto bcc
if "%2" == "bcc" goto bcc
goto args

:icc
set XCOMP=ICC
goto makeexe

:bcc
set XCOMP=BCC
goto makeexe


:makeexe
set XTYPE=OS2APP
copy ..\..\bases.gen + ..\..\build\build.gen + ..\..\build\%2.cfg + mk.gen makefile

if "%1" == "ICC" goto ibm
if "%1" == "icc" goto ibm
if "%1" == "BCC" goto borland
if "%1" == "bcc" goto borland


:ibm
nmake RDX=%XRDM% TARGET_TYPE=%XTYPE% COMPILER=%XCOMP% 
goto exit


:borland
make -DRDX=%XRDM% -DTARGET_TYPE=%XTYPE% -DCOMPILER=%XCOMP% 
goto exit


:args
echo.
echo * Usage:     MK {RDM/RDS} {ICC/BCC}
echo *
echo * Example:   MK RDS ICC 
echo * ---------------------------------------------------------------------------
echo * Compilers:
echo *            ICC      IBM VisualAge C++ for OS2
echo *            BCC      Borland C++ for OS2
echo * ---------------------------------------------------------------------------
echo * 
echo.

:exit
set XTYPE=
set XCOMP=
set XRDM=
