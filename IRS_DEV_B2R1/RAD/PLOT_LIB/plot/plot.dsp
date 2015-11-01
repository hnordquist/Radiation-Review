# Microsoft Developer Studio Project File - Name="plot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=plot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plot.mak" CFG="plot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plot - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "plot - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/IRS_DEV_PLOT_LIB/Source/plot", IEFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Od /I "c:\IRS_DEVCOMMON\Winclude32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "plot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "c:\IRS_DEVCOMMON\Winclude32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "plot - Win32 Release"
# Name "plot - Win32 Debug"
# Begin Source File

SOURCE=..\PL.CPP
# End Source File
# Begin Source File

SOURCE=..\PL.H
# End Source File
# Begin Source File

SOURCE=..\PL_AREA.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_C.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_DRAW.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_E_2_M.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_OBJ.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_OVR.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_P_2_P.CPP
# End Source File
# Begin Source File

SOURCE=..\Pl_prt.cpp
# End Source File
# Begin Source File

SOURCE=..\PL_PVT.H
# End Source File
# Begin Source File

SOURCE=..\PL_R_2_Z.CPP
# End Source File
# Begin Source File

SOURCE=..\PL_WIN.CPP
# End Source File
# Begin Source File

SOURCE=..\SCAN.C
# End Source File
# Begin Source File

SOURCE=..\SCAN.H
# End Source File
# Begin Source File

SOURCE=..\TIME_SUB.C
# End Source File
# Begin Source File

SOURCE=..\TIME_SUB.H
# End Source File
# Begin Source File

SOURCE=..\WU.C
# End Source File
# Begin Source File

SOURCE=..\WU.H
# End Source File
# End Target
# End Project
