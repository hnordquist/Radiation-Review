# Microsoft Developer Studio Project File - Name="CoVifmImport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CoVifmImport - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoVifmImport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoVifmImport.mak" CFG="CoVifmImport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoVifmImport - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CoVifmImport - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Radiation Review/CoVifmImport", PUDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoVifmImport - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonImportCom" /I "..\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Check_Signature" /D "_DEBUG" /D "_MBCS" /D "VIFM" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib imagehlp.lib ..\..\IRS_DEVCOMMON\Check_Signature\Check_Signature.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\CoVifmImport.def" /out:"c:/irs/comlibs\CoVifmImport.dll" /pdbtype:sept /libpath:"..\..\IRS_DEVCOMMON\rdm45\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\irs\comlibs\CoVifmImport.dll
InputPath=\irs\comlibs\CoVifmImport.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CoVifmImport - Win32 Release MinDependency"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonImportCom" /I "..\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Check_Signature" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "VIFM" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\..\IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib imagehlp.lib ..\..\IRS_DEVCOMMON\Check_Signature\Check_Signature.lib /nologo /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /out:"c:/irs/comlibs/CoVifmImport.dll" /libpath:"..\..\IRS_DEVCOMMON\rdm45\lib"
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\CoVifmImport.dll
InputPath=\irs\comlibs\CoVifmImport.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CoVifmImport - Win32 Debug"
# Name "CoVifmImport - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoVifmImport.cpp
# End Source File
# Begin Source File

SOURCE=.\CoVifmImport.def

!IF  "$(CFG)" == "CoVifmImport - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoVifmImport - Win32 Release MinDependency"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoVifmImport.idl
# ADD MTL /tlb ".\CoVifmImport.tlb" /h "CoVifmImport.h" /iid "CoVifmImport_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\CoVifmImport.rc
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\FacCfgWrap.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\MyDateTime.cpp
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
# Begin Source File

SOURCE=.\VifmData.cpp
# End Source File
# Begin Source File

SOURCE=.\VifmDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\VifmDbVista.cpp
# End Source File
# Begin Source File

SOURCE=.\VifmImport.cpp
# End Source File
# Begin Source File

SOURCE=.\VifmImportSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\VistaBaseData.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\VistaDataFile.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\VistaDb.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FacCfgWrap.h
# End Source File
# Begin Source File

SOURCE=..\..\IRS_DEVCOMMON\Winclude32\GEN.H
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\GenDateTime.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\MyDateTime.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VifmData.h
# End Source File
# Begin Source File

SOURCE=.\VifmDataFile.h
# End Source File
# Begin Source File

SOURCE=.\VifmDbVista.h
# End Source File
# Begin Source File

SOURCE=.\VifmImport.h
# End Source File
# Begin Source File

SOURCE=.\VifmImportSupport.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\VistaBaseData.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\VistaDataFile.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\VistaDb.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\VifmImport.rgs
# End Source File
# End Group
# End Target
# End Project
