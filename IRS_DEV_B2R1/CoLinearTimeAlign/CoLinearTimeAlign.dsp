# Microsoft Developer Studio Project File - Name="CoLinearTimeAlign" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CoLinearTimeAlign - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoLinearTimeAlign.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoLinearTimeAlign.mak" CFG="CoLinearTimeAlign - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoLinearTimeAlign - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CoLinearTimeAlign - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Radiation Review/CoTimeAlign", EQDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\commonimportcom" /I "..\..\IRS_DEVCOMMON\rdm45\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imagehlp.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"c:/irs/comlibs\CoLinearTimeAlign.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\irs\comlibs\CoLinearTimeAlign.dll
InputPath=\irs\comlibs\CoLinearTimeAlign.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\commonimportcom" /I "..\..\IRS_DEVCOMMON\rdm45\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 imagehlp.lib /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /out:"c:/irs/comlibs/CoLinearTimeAlign.dll"
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\CoLinearTimeAlign.dll
InputPath=\irs\comlibs\CoLinearTimeAlign.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CoLinearTimeAlign - Win32 Debug"
# Name "CoLinearTimeAlign - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoLinearTimeAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\CoLinearTimeAlign.def
# End Source File
# Begin Source File

SOURCE=.\CoLinearTimeAlign.idl
# ADD MTL /tlb ".\CoLinearTimeAlign.tlb" /h "CoLinearTimeAlign.h" /iid "CoLinearTimeAlign_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\CoLinearTimeAlign.rc
# End Source File
# Begin Source File

SOURCE=.\DbIni.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.cpp
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

SOURCE=.\TimeAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeAlignIni.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DbIni.h
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.h
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\SafeArrayUtil.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TimeAdjust.h
# End Source File
# Begin Source File

SOURCE=.\TimeAlign.h
# End Source File
# Begin Source File

SOURCE=.\TimeAlignIni.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\TimeAlign.rgs
# End Source File
# End Group
# End Target
# End Project
