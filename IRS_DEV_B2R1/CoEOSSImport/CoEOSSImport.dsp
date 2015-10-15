# Microsoft Developer Studio Project File - Name="CoEOSSImport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CoEOSSImport - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoEOSSImport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoEOSSImport.mak" CFG="CoEOSSImport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoEOSSImport - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CoEOSSImport - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Radiation Review/CoEOSSImport/CoEOSSImport", MDDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoEOSSImport - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W2 /Gm /GX /ZI /Od /I "..\CommonImportCom" /I "c:\projects\RAD_b2R1\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON" /D "_DEBUG" /D "_MBCS" /D "EOSS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imagehlp.lib /nologo /subsystem:windows /dll /pdb:"./Debug/CoEOSSImport.pdb" /map:"./Debug/CoEOSSImport.map" /debug /machine:I386 /def:".\CoEOSSImport.def" /out:"c:/irs/comlibs/coeossimport.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\irs\comlibs\coeossimport.dll
InputPath=\irs\comlibs\coeossimport.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CoEOSSImport - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W2 /GX /Od /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "EOSS" /D "_WINDLL" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 imagehlp.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"./ReleaseMinDependency/CoEOSSImport.pdb" /map:"./Debug/CoEOSSImport.map" /debug /machine:I386 /def:".\CoEOSSImport.def" /out:"c:/irs/comlibs/coeossimport.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\coeossimport.dll
InputPath=\irs\comlibs\coeossimport.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CoEOSSImport - Win32 Debug"
# Name "CoEOSSImport - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CommonImportCom\AccessDataFile.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\AccessDb.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessEOSSDb.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\AdoDb.cpp
# End Source File
# Begin Source File

SOURCE=.\CoEOSSImport.cpp
# End Source File
# Begin Source File

SOURCE=.\CoEOSSImport.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CoEOSSImport.idl
# ADD MTL /tlb ".\CoEOSSImport.tlb" /h "CoEOSSImport.h" /iid "CoEOSSImport_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\CreateAccessDatabase.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\EOSSCreateAccessDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\EOSSData.cpp
# End Source File
# Begin Source File

SOURCE=.\EOSSDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\EOSSImport.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SafeArrayUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\CommonImportCom\AccessDataFile.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\AccessDb.h
# End Source File
# Begin Source File

SOURCE=.\AccessEOSSDb.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\AdoDb.h
# End Source File
# Begin Source File

SOURCE=.\CoEOSSImport.h
# End Source File
# Begin Source File

SOURCE=..\Common\commondefines.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\CreateAccessDatabase.h
# End Source File
# Begin Source File

SOURCE=.\EOSSCreateAccessDatabase.h
# End Source File
# Begin Source File

SOURCE=.\EOSSData.h
# End Source File
# Begin Source File

SOURCE=.\EOSSDataFile.h
# End Source File
# Begin Source File

SOURCE=.\eossdefs.h
# End Source File
# Begin Source File

SOURCE=.\EOSSImport.h
# End Source File
# Begin Source File

SOURCE=..\RAD\imports.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CoEOSSImport.rc
# End Source File
# Begin Source File

SOURCE=.\EOSSImport.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=..\Common\CommonErrorMessages.ini
# End Source File
# End Target
# End Project
