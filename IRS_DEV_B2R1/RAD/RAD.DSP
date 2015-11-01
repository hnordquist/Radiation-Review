# Microsoft Developer Studio Project File - Name="RAD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RAD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RAD.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RAD.MAK" CFG="RAD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RAD - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RAD - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Rad Review Test/RAD", WABAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RAD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\..\Rad_Coms" /I "..\Common" /I "..\CommonImportCom" /I "..\..\Common\winclude32" /I "..\..\Common\zapp3" /I "..\..\Common\rdm45\include" /I "..\..\Common\Zapp3\ZAPPH" /I "..\..\Common\Check_Signature" /I "\Program Files\Common Files\System\ado" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "Z_NP" /D "RW_NO_STL" /Fr /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\IRS_DEVCOMMON\UpperLayer" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 nafxcw.lib ..\..\common\Winlib32\plot.lib imagehlp.lib ..\..\Common\Winlib32\gen32.lib ..\..\Common\Winlib32\gui32.lib ..\..\Common\Check_Signature\Check_Signature.lib ..\..\Common\rdm45\lib\vwin\rdm45w32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"nafxcw.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
# SUBTRACT LINK32 /pdb:none /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "RAD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\rad_coms" /I "..\Common" /I "..\CommonImportCom" /I "..\..\Common\winclude32" /I "..\..\Common\zapp3" /I "..\..\Common\rdm45\include" /I "..\..\Common\Zapp3\ZAPPH" /I "..\..\Common\Check_Signature" /I "\Program Files\Common Files\System\ado" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "Z_NP" /D "RW_NO_STL" /D "MSC" /FR /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\IRS_DEVCOMMON\UpperLayer" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib ..\..\common\Winlib32\plot.lib imagehlp.lib ..\..\Common\Winlib32\gen32.lib ..\..\Common\Winlib32\gui32.lib ..\..\Common\Check_Signature\Check_Signature.lib ..\..\Common\rdm45\lib\vwin\rdm45w32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrtd.lib"
# SUBTRACT LINK32 /pdb:none /map

!ENDIF 

# Begin Target

# Name "RAD - Win32 Release"
# Name "RAD - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\AdjustTime.cpp
# End Source File
# Begin Source File

SOURCE=.\BinaryImport.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildBandaid.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildFilename.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyDir.cpp
# End Source File
# Begin Source File

SOURCE=.\DateTimeUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\DbDay.cpp
# End Source File
# Begin Source File

SOURCE=.\DbDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\DbEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DbGeneralDbSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\DbImport.cpp
# End Source File
# Begin Source File

SOURCE=.\DbInterval.cpp
# End Source File
# Begin Source File

SOURCE=.\DbLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\DbMark.cpp
# End Source File
# Begin Source File

SOURCE=.\DbOperator.CPP
# End Source File
# Begin Source File

SOURCE=.\DbReadArray.cpp
# End Source File
# Begin Source File

SOURCE=.\DbReadStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\DbTime.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\DirUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBackupCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBackupDb.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCopyLogs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDaysSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDirection.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDirectionSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEventSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExport.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindMeas.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImportAll.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgimportLatest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImportSome.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIntegrity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMarkSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMeasSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRawData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRestoreCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRestoreDb.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReviewPeriod.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectFacility.cpp
# End Source File
# Begin Source File

SOURCE=.\EOSSImport.cpp
# End Source File
# Begin Source File

SOURCE=.\EventNetArea.cpp
# End Source File
# Begin Source File

SOURCE=.\EventSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\EventTypeLists.cpp
# End Source File
# Begin Source File

SOURCE=.\FacilityConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\FindEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\FindMeas.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\GETFILE.cpp
# End Source File
# Begin Source File

SOURCE=.\GrandImport.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI_Glue.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI_PLOT.CPP
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\Initfile.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDataFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Interval.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDbCfg.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\MyDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\OleTimeUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Patches.cpp
# End Source File
# Begin Source File

SOURCE=.\PerformTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\PIPE.CPP
# End Source File
# Begin Source File

SOURCE=.\PLOT.CPP
# End Source File
# Begin Source File

SOURCE=.\PLT_DB.CPP
# End Source File
# Begin Source File

SOURCE=.\PLT_DB_EOSS.CPP
# End Source File
# Begin Source File

SOURCE=.\PLT_DLG.CPP
# End Source File
# Begin Source File

SOURCE=.\Plt_evnt.cpp
# End Source File
# Begin Source File

SOURCE=.\PLT_PANE.CPP
# End Source File
# Begin Source File

SOURCE=.\PrintFacilityHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBars.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Questions.cpp
# End Source File
# Begin Source File

SOURCE=.\RadError.cpp
# End Source File
# Begin Source File

SOURCE=.\RADEventList.cpp
# End Source File
# Begin Source File

SOURCE=.\RADEventsXML.cpp
# End Source File
# Begin Source File

SOURCE=.\RadImport.cpp
# End Source File
# Begin Source File

SOURCE=.\RadInit.cpp
# End Source File
# Begin Source File

SOURCE=.\RadMain.cpp
# End Source File
# Begin Source File

SOURCE=.\RadReanalyze.cpp
# End Source File
# Begin Source File

SOURCE=.\RadReview.cpp
# End Source File
# Begin Source File

SOURCE=.\RadSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfg.cpp
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RevwTool.cpp"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTERROR.CPP"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTIMPORT.CPP"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTINIT.CPP"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTPIPE.CPP"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTReanalyze.cpp"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\Rtreview.cpp"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTSETUP.CPP"
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\RTTITLE.CPP"
# End Source File
# Begin Source File

SOURCE=..\Common\SafeArrayUtil.cpp
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\SEBBoxes.CPP"
# End Source File
# Begin Source File

SOURCE=.\TextFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\TS.cpp
# End Source File
# Begin Source File

SOURCE=".\UPPERLAYER\TxtWrdb.CPP"
# End Source File
# Begin Source File

SOURCE=.\Update.cpp
# End Source File
# Begin Source File

SOURCE=.\UpperLayerGlue.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\VifmImport.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteNCCFile.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteXML.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h, *.txt"
# Begin Source File

SOURCE=.\AdjustTime.h
# End Source File
# Begin Source File

SOURCE=.\BinaryImport.h
# End Source File
# Begin Source File

SOURCE=..\Common\ComDataStructures.h
# End Source File
# Begin Source File

SOURCE=.\DateTimeUtils.h
# End Source File
# Begin Source File

SOURCE=.\DATABASE\db_main.h
# End Source File
# Begin Source File

SOURCE=.\DbDefault.h
# End Source File
# Begin Source File

SOURCE=.\DbImport.h
# End Source File
# Begin Source File

SOURCE=.\DbInterval.h
# End Source File
# Begin Source File

SOURCE=.\DbReadStruct.h
# End Source File
# Begin Source File

SOURCE=.\Display.h
# End Source File
# Begin Source File

SOURCE=.\DlgDirection.h
# End Source File
# Begin Source File

SOURCE=.\EOSSImport.h
# End Source File
# Begin Source File

SOURCE=.\FacilityConfig.h
# End Source File
# Begin Source File

SOURCE=..\Common\GenDateTime.h
# End Source File
# Begin Source File

SOURCE=.\GrandImport.h
# End Source File
# Begin Source File

SOURCE=.\GUI_Glue.h
# End Source File
# Begin Source File

SOURCE=.\GUI_PLOT.H
# End Source File
# Begin Source File

SOURCE=..\Common\HPDefines.h
# End Source File
# Begin Source File

SOURCE=.\imports.h
# End Source File
# Begin Source File

SOURCE=..\Common\MyDateTime.h
# End Source File
# Begin Source File

SOURCE=.\PLT_DB.H
# End Source File
# Begin Source File

SOURCE=.\PLT_DLG.H
# End Source File
# Begin Source File

SOURCE=.\plt_evnt.h
# End Source File
# Begin Source File

SOURCE=.\PLT_PANE.H
# End Source File
# Begin Source File

SOURCE=.\ProgressBars.h
# End Source File
# Begin Source File

SOURCE=..\Common\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\RAD.H
# End Source File
# Begin Source File

SOURCE=.\Raddefs.h
# End Source File
# Begin Source File

SOURCE=.\RadError.h
# End Source File
# Begin Source File

SOURCE=.\RADEventList.h
# End Source File
# Begin Source File

SOURCE=.\RADEventsXML.h
# End Source File
# Begin Source File

SOURCE=.\RadImport.h
# End Source File
# Begin Source File

SOURCE=.\RadInit.h
# End Source File
# Begin Source File

SOURCE=.\RadMain.h
# End Source File
# Begin Source File

SOURCE=.\RadReanalyze.H
# End Source File
# Begin Source File

SOURCE=.\RadReview.h
# End Source File
# Begin Source File

SOURCE=.\RadSetup.h
# End Source File
# Begin Source File

SOURCE=.\Radstrs.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTERROR.H
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTIMPORT.H
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTINIT.H
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTPIPE.H
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTReanalyze.h
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTREVIEW.H
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTSETUP.H
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\RTTITLE.H
# End Source File
# Begin Source File

SOURCE=..\Common\SafeArrayUtil.h
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\SEBBOXES.H
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Common\targetver.h
# End Source File
# Begin Source File

SOURCE=.\TimeAlign.h
# End Source File
# Begin Source File

SOURCE=.\TS.h
# End Source File
# Begin Source File

SOURCE=.\UPPERLAYER\TxtWrdb.H
# End Source File
# Begin Source File

SOURCE=..\Common\TypeDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\Utilities.h
# End Source File
# Begin Source File

SOURCE=.\VifmImport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "*.rc, *.rc2,*.cur, *.ico, *.bmp"
# Begin Source File

SOURCE=.\res\CURSOR1.CUR
# End Source File
# Begin Source File

SOURCE=.\res\CURSOR2.CUR
# End Source File
# Begin Source File

SOURCE=.\res\GRANDREV.ICO
# End Source File
# Begin Source File

SOURCE=.\res\horzch2.cur
# End Source File
# Begin Source File

SOURCE=.\res\ICON1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\NewRadIcon.ICO
# End Source File
# Begin Source File

SOURCE=.\res\QUESTION.ICO
# End Source File
# Begin Source File

SOURCE=.\RAD.RC
# End Source File
# Begin Source File

SOURCE=.\RES\RAD.RC2
# End Source File
# Begin Source File

SOURCE=.\res\RADTitle.bmp
# End Source File
# End Group
# End Target
# End Project
