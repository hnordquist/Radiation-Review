# Microsoft Developer Studio Project File - Name="CoGrandImport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CoGrandImport - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoGrandImport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoGrandImport.mak" CFG="CoGrandImport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoGrandImport - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CoGrandImport - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Radiation Review/CoGrandImport/CoGrandImport", MDDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoGrandImport - Win32 Debug"

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
# ADD CPP /nologo /MTd /W2 /Gm /GX /ZI /Od /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "G:\Program Files\MSXML 4.0\inc" /D "_DEBUG" /D "_MBCS" /D "MINIGRAND" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib imagehlp.lib ../../IRS_DEVCOMMON\Winlib32\gui32.lib ../../IRS_DEVCOMMON\Check_Signature\Check_Signature.lib ../../IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib /nologo /subsystem:windows /dll /pdb:"c:/irs/comlibs/CoGrandImport.pdb" /debug /machine:I386 /def:".\CoGrandImport.def" /out:"c:/irs/comlibs\CoGrandImport.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\irs\comlibs\CoGrandImport.dll
InputPath=\irs\comlibs\CoGrandImport.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CoGrandImport - Win32 Release MinDependency"

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
# ADD CPP /nologo /MT /W2 /GX /Zi /O2 /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "G:\Program Files\MSXML 4.0\inc" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "MINIGRAND" /D "_WINDLL" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 nafxcw.lib imagehlp.lib ../../IRS_DEVCOMMON\Winlib32\gui32.lib ../../IRS_DEVCOMMON\Check_Signature\Check_Signature.lib ../../IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib /nologo /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /def:".\CoGrandImport.def" /out:"c:/irs/comlibs/CoGrandImport.dll" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\CoGrandImport.dll
InputPath=\irs\comlibs\CoGrandImport.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CoGrandImport - Win32 Debug"
# Name "CoGrandImport - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CommonBaseClasses\BaseFacCfgWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\CoGrandImport.cpp
# End Source File
# Begin Source File

SOURCE=.\CoGrandImport.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CoGrandImport.idl
# ADD MTL /tlb ".\CoGrandImport.tlb" /h "CoGrandImport.h" /iid "CoGrandImport_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ErrorProcessing.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandData.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandDbVista.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandFacCfgWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandImport.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandImportSupport.cpp
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

SOURCE=..\CommonBaseClasses\BaseFacCfgWrap.h
# End Source File
# Begin Source File

SOURCE=.\CoGrandImport.h
# End Source File
# Begin Source File

SOURCE=..\CommonImportCom\db_rmon.h
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.h
# End Source File
# Begin Source File

SOURCE=..\Common\ErrorProcessing.h
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.h
# End Source File
# Begin Source File

SOURCE=.\GrandData.h
# End Source File
# Begin Source File

SOURCE=.\GrandDataFile.h
# End Source File
# Begin Source File

SOURCE=.\GrandDbVista.h
# End Source File
# Begin Source File

SOURCE=.\GrandFacCfgWrap.h
# End Source File
# Begin Source File

SOURCE=.\GrandImport.h
# End Source File
# Begin Source File

SOURCE=.\GrandImportSupport.h
# End Source File
# Begin Source File

SOURCE=..\Common\imports.h
# End Source File
# Begin Source File

SOURCE=..\Common\MyDateTime.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
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

SOURCE=.\CoGrandImport.rc
# End Source File
# Begin Source File

SOURCE=.\GrandImport.rgs
# End Source File
# End Group
# Begin Source File

SOURCE="G:\Documents and Settings\186791\My Documents\ISVS-A.txt"
# End Source File
# End Target
# End Project
