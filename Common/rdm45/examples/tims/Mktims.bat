@echo off

rem  Batch File to Build Raima Data Manager TIMS Example

rem  Arguments
rem    %1 - Compiler
rem    %2 - Home Directory for Raima Data Manager

rem Validate the compiler
set C=
if "%1" == "msvc" set C=MSVC
if "%1" == "MSVC" set C=MSVC
if "%C%" == "" echo The specified compiler is invalid.
if "%C%" == "" goto args

if "%2" == "" goto bad_home
if not exist %2\*.* goto bad_home

goto %C%


:MSVC
   nmake CMP=%C% VROOT=%2 -f makefile.msc
   goto exit

:bad_home
echo The specified home directory does not exist.

:args
echo *
echo *
echo * Usage:     MKTIMS  Compiler  Home_Dir
echo *
echo * Example:   MKTIMS  MSVC      \RAIMA
echo * ---------------------------------------------------------------------------
echo * Compiler:  MSVC - Microsoft Visual C++
echo * ---------------------------------------------------------------------------
echo * Home_Dir:  Root directory for Raima Data Manager.
echo * ---------------------------------------------------------------------------
echo *
echo *

rem Set errorlevel
replace .>nul

:exit
set C=

rem $Revision:   1.17  $

