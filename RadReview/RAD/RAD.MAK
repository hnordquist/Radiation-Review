# Microsoft Developer Studio Generated NMAKE File, Based on RAD.DSP
!IF "$(CFG)" == ""
CFG=RAD - Win32 Debug
!MESSAGE No configuration specified. Defaulting to RAD - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RAD - Win32 Release" && "$(CFG)" != "RAD - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "RAD - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\irs\rad\RAD.exe" "$(OUTDIR)\RAD.bsc"

!ELSE 

ALL : "..\..\..\irs\rad\RAD.exe" "$(OUTDIR)\RAD.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AdjustTime.obj"
	-@erase "$(INTDIR)\AdjustTime.sbr"
	-@erase "$(INTDIR)\BinaryImport.obj"
	-@erase "$(INTDIR)\BinaryImport.sbr"
	-@erase "$(INTDIR)\BuildBandaid.obj"
	-@erase "$(INTDIR)\BuildBandaid.sbr"
	-@erase "$(INTDIR)\BuildFilename.obj"
	-@erase "$(INTDIR)\BuildFilename.sbr"
	-@erase "$(INTDIR)\CopyDir.obj"
	-@erase "$(INTDIR)\CopyDir.sbr"
	-@erase "$(INTDIR)\DateTimeUtils.obj"
	-@erase "$(INTDIR)\DateTimeUtils.sbr"
	-@erase "$(INTDIR)\DbDay.obj"
	-@erase "$(INTDIR)\DbDay.sbr"
	-@erase "$(INTDIR)\DbDefault.obj"
	-@erase "$(INTDIR)\DbDefault.sbr"
	-@erase "$(INTDIR)\DbEvent.obj"
	-@erase "$(INTDIR)\DbEvent.sbr"
	-@erase "$(INTDIR)\DbGeneralDbSupport.obj"
	-@erase "$(INTDIR)\DbGeneralDbSupport.sbr"
	-@erase "$(INTDIR)\DbImport.obj"
	-@erase "$(INTDIR)\DbImport.sbr"
	-@erase "$(INTDIR)\DbInterval.obj"
	-@erase "$(INTDIR)\DbInterval.sbr"
	-@erase "$(INTDIR)\DbLimit.obj"
	-@erase "$(INTDIR)\DbLimit.sbr"
	-@erase "$(INTDIR)\DbMark.obj"
	-@erase "$(INTDIR)\DbMark.sbr"
	-@erase "$(INTDIR)\DbOperator.obj"
	-@erase "$(INTDIR)\DbOperator.sbr"
	-@erase "$(INTDIR)\DbReadArray.obj"
	-@erase "$(INTDIR)\DbReadArray.sbr"
	-@erase "$(INTDIR)\DbReadStruct.obj"
	-@erase "$(INTDIR)\DbReadStruct.sbr"
	-@erase "$(INTDIR)\DbTime.obj"
	-@erase "$(INTDIR)\DbTime.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Display.sbr"
	-@erase "$(INTDIR)\DlgBackupCfg.obj"
	-@erase "$(INTDIR)\DlgBackupCfg.sbr"
	-@erase "$(INTDIR)\DlgBackupDb.obj"
	-@erase "$(INTDIR)\DlgBackupDb.sbr"
	-@erase "$(INTDIR)\DlgCopyLogs.obj"
	-@erase "$(INTDIR)\DlgCopyLogs.sbr"
	-@erase "$(INTDIR)\DlgDaysSummary.obj"
	-@erase "$(INTDIR)\DlgDaysSummary.sbr"
	-@erase "$(INTDIR)\DlgDirection.obj"
	-@erase "$(INTDIR)\DlgDirection.sbr"
	-@erase "$(INTDIR)\DlgDirectionSummary.obj"
	-@erase "$(INTDIR)\DlgDirectionSummary.sbr"
	-@erase "$(INTDIR)\DlgEventSummary.obj"
	-@erase "$(INTDIR)\DlgEventSummary.sbr"
	-@erase "$(INTDIR)\DlgExport.obj"
	-@erase "$(INTDIR)\DlgExport.sbr"
	-@erase "$(INTDIR)\DlgFindEvent.obj"
	-@erase "$(INTDIR)\DlgFindEvent.sbr"
	-@erase "$(INTDIR)\DlgFindMeas.obj"
	-@erase "$(INTDIR)\DlgFindMeas.sbr"
	-@erase "$(INTDIR)\DlgImportAll.obj"
	-@erase "$(INTDIR)\DlgImportAll.sbr"
	-@erase "$(INTDIR)\DlgimportLatest.obj"
	-@erase "$(INTDIR)\DlgimportLatest.sbr"
	-@erase "$(INTDIR)\DlgImportSome.obj"
	-@erase "$(INTDIR)\DlgImportSome.sbr"
	-@erase "$(INTDIR)\DlgIntegrity.obj"
	-@erase "$(INTDIR)\DlgIntegrity.sbr"
	-@erase "$(INTDIR)\DlgMarkSummary.obj"
	-@erase "$(INTDIR)\DlgMarkSummary.sbr"
	-@erase "$(INTDIR)\DlgMeasSummary.obj"
	-@erase "$(INTDIR)\DlgMeasSummary.sbr"
	-@erase "$(INTDIR)\DlgRawData.obj"
	-@erase "$(INTDIR)\DlgRawData.sbr"
	-@erase "$(INTDIR)\DlgRestoreCfg.obj"
	-@erase "$(INTDIR)\DlgRestoreCfg.sbr"
	-@erase "$(INTDIR)\DlgRestoreDb.obj"
	-@erase "$(INTDIR)\DlgRestoreDb.sbr"
	-@erase "$(INTDIR)\DlgReviewPeriod.obj"
	-@erase "$(INTDIR)\DlgReviewPeriod.sbr"
	-@erase "$(INTDIR)\DlgSelectFacility.obj"
	-@erase "$(INTDIR)\DlgSelectFacility.sbr"
	-@erase "$(INTDIR)\EOSSImport.obj"
	-@erase "$(INTDIR)\EOSSImport.sbr"
	-@erase "$(INTDIR)\EventNetArea.obj"
	-@erase "$(INTDIR)\EventNetArea.sbr"
	-@erase "$(INTDIR)\EventSummary.obj"
	-@erase "$(INTDIR)\EventSummary.sbr"
	-@erase "$(INTDIR)\EventTypeLists.obj"
	-@erase "$(INTDIR)\EventTypeLists.sbr"
	-@erase "$(INTDIR)\FacilityConfig.obj"
	-@erase "$(INTDIR)\FacilityConfig.sbr"
	-@erase "$(INTDIR)\FindEvent.obj"
	-@erase "$(INTDIR)\FindEvent.sbr"
	-@erase "$(INTDIR)\FindMeas.obj"
	-@erase "$(INTDIR)\FindMeas.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
	-@erase "$(INTDIR)\GETFILE.obj"
	-@erase "$(INTDIR)\GETFILE.sbr"
	-@erase "$(INTDIR)\GrandImport.obj"
	-@erase "$(INTDIR)\GrandImport.sbr"
	-@erase "$(INTDIR)\GUI_Glue.obj"
	-@erase "$(INTDIR)\GUI_Glue.sbr"
	-@erase "$(INTDIR)\GUI_PLOT.OBJ"
	-@erase "$(INTDIR)\GUI_PLOT.SBR"
	-@erase "$(INTDIR)\Initfile.obj"
	-@erase "$(INTDIR)\Initfile.sbr"
	-@erase "$(INTDIR)\InputDataFiles.obj"
	-@erase "$(INTDIR)\InputDataFiles.sbr"
	-@erase "$(INTDIR)\Interval.obj"
	-@erase "$(INTDIR)\Interval.sbr"
	-@erase "$(INTDIR)\MainDbCfg.obj"
	-@erase "$(INTDIR)\MainDbCfg.sbr"
	-@erase "$(INTDIR)\MyDateTime.obj"
	-@erase "$(INTDIR)\MyDateTime.sbr"
	-@erase "$(INTDIR)\OleTimeUtils.obj"
	-@erase "$(INTDIR)\OleTimeUtils.sbr"
	-@erase "$(INTDIR)\Patches.obj"
	-@erase "$(INTDIR)\Patches.sbr"
	-@erase "$(INTDIR)\PerformTimer.obj"
	-@erase "$(INTDIR)\PerformTimer.sbr"
	-@erase "$(INTDIR)\PIPE.OBJ"
	-@erase "$(INTDIR)\PIPE.SBR"
	-@erase "$(INTDIR)\PLOT.OBJ"
	-@erase "$(INTDIR)\PLOT.SBR"
	-@erase "$(INTDIR)\PLT_DB.OBJ"
	-@erase "$(INTDIR)\PLT_DB.SBR"
	-@erase "$(INTDIR)\PLT_DB_EOSS.OBJ"
	-@erase "$(INTDIR)\PLT_DB_EOSS.SBR"
	-@erase "$(INTDIR)\PLT_DLG.OBJ"
	-@erase "$(INTDIR)\PLT_DLG.SBR"
	-@erase "$(INTDIR)\Plt_evnt.obj"
	-@erase "$(INTDIR)\Plt_evnt.sbr"
	-@erase "$(INTDIR)\PLT_PANE.OBJ"
	-@erase "$(INTDIR)\PLT_PANE.SBR"
	-@erase "$(INTDIR)\PrintFacilityHeader.obj"
	-@erase "$(INTDIR)\PrintFacilityHeader.sbr"
	-@erase "$(INTDIR)\ProgressBars.obj"
	-@erase "$(INTDIR)\ProgressBars.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\Questions.obj"
	-@erase "$(INTDIR)\Questions.sbr"
	-@erase "$(INTDIR)\RAD.res"
	-@erase "$(INTDIR)\RadError.obj"
	-@erase "$(INTDIR)\RadError.sbr"
	-@erase "$(INTDIR)\RADEventList.obj"
	-@erase "$(INTDIR)\RADEventList.sbr"
	-@erase "$(INTDIR)\RADEventsXML.obj"
	-@erase "$(INTDIR)\RADEventsXML.sbr"
	-@erase "$(INTDIR)\RadImport.obj"
	-@erase "$(INTDIR)\RadImport.sbr"
	-@erase "$(INTDIR)\RadInit.obj"
	-@erase "$(INTDIR)\RadInit.sbr"
	-@erase "$(INTDIR)\RadMain.obj"
	-@erase "$(INTDIR)\RadMain.sbr"
	-@erase "$(INTDIR)\RadReanalyze.obj"
	-@erase "$(INTDIR)\RadReanalyze.sbr"
	-@erase "$(INTDIR)\RadReview.obj"
	-@erase "$(INTDIR)\RadReview.sbr"
	-@erase "$(INTDIR)\RadSetup.obj"
	-@erase "$(INTDIR)\RadSetup.sbr"
	-@erase "$(INTDIR)\ReadCfg.obj"
	-@erase "$(INTDIR)\ReadCfg.sbr"
	-@erase "$(INTDIR)\RevwTool.obj"
	-@erase "$(INTDIR)\RevwTool.sbr"
	-@erase "$(INTDIR)\RTERROR.OBJ"
	-@erase "$(INTDIR)\RTERROR.SBR"
	-@erase "$(INTDIR)\RTIMPORT.OBJ"
	-@erase "$(INTDIR)\RTIMPORT.SBR"
	-@erase "$(INTDIR)\RTINIT.OBJ"
	-@erase "$(INTDIR)\RTINIT.SBR"
	-@erase "$(INTDIR)\RTPIPE.OBJ"
	-@erase "$(INTDIR)\RTPIPE.SBR"
	-@erase "$(INTDIR)\RTReanalyze.obj"
	-@erase "$(INTDIR)\RTReanalyze.sbr"
	-@erase "$(INTDIR)\Rtreview.obj"
	-@erase "$(INTDIR)\Rtreview.sbr"
	-@erase "$(INTDIR)\RTSETUP.OBJ"
	-@erase "$(INTDIR)\RTSETUP.SBR"
	-@erase "$(INTDIR)\RTTITLE.OBJ"
	-@erase "$(INTDIR)\RTTITLE.SBR"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\SEBBoxes.obj"
	-@erase "$(INTDIR)\SEBBoxes.sbr"
	-@erase "$(INTDIR)\TextFile.obj"
	-@erase "$(INTDIR)\TextFile.sbr"
	-@erase "$(INTDIR)\TimeAlign.obj"
	-@erase "$(INTDIR)\TimeAlign.sbr"
	-@erase "$(INTDIR)\TS.obj"
	-@erase "$(INTDIR)\TS.sbr"
	-@erase "$(INTDIR)\TxtWrdb.obj"
	-@erase "$(INTDIR)\TxtWrdb.sbr"
	-@erase "$(INTDIR)\Update.obj"
	-@erase "$(INTDIR)\Update.sbr"
	-@erase "$(INTDIR)\UpperLayerGlue.obj"
	-@erase "$(INTDIR)\UpperLayerGlue.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VifmImport.obj"
	-@erase "$(INTDIR)\VifmImport.sbr"
	-@erase "$(INTDIR)\WriteNCCFile.obj"
	-@erase "$(INTDIR)\WriteNCCFile.sbr"
	-@erase "$(INTDIR)\WriteXML.obj"
	-@erase "$(INTDIR)\WriteXML.sbr"
	-@erase "$(OUTDIR)\RAD.bsc"
	-@erase "$(OUTDIR)\RAD.pdb"
	-@erase "..\..\..\irs\rad\RAD.exe"
	-@erase "..\..\..\irs\rad\RAD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Zi /O2 /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "c:\Program Files\MSXML 6.0\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "Z_NP" /D "RW_NO_STL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\RAD.res" /i "..\..\IRS_DEVCOMMON\UpperLayer" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RAD.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AdjustTime.sbr" \
	"$(INTDIR)\BinaryImport.sbr" \
	"$(INTDIR)\BuildBandaid.sbr" \
	"$(INTDIR)\BuildFilename.sbr" \
	"$(INTDIR)\CopyDir.sbr" \
	"$(INTDIR)\DateTimeUtils.sbr" \
	"$(INTDIR)\DbDay.sbr" \
	"$(INTDIR)\DbDefault.sbr" \
	"$(INTDIR)\DbEvent.sbr" \
	"$(INTDIR)\DbGeneralDbSupport.sbr" \
	"$(INTDIR)\DbImport.sbr" \
	"$(INTDIR)\DbInterval.sbr" \
	"$(INTDIR)\DbLimit.sbr" \
	"$(INTDIR)\DbMark.sbr" \
	"$(INTDIR)\DbOperator.sbr" \
	"$(INTDIR)\DbReadArray.sbr" \
	"$(INTDIR)\DbReadStruct.sbr" \
	"$(INTDIR)\DbTime.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\Display.sbr" \
	"$(INTDIR)\DlgBackupCfg.sbr" \
	"$(INTDIR)\DlgBackupDb.sbr" \
	"$(INTDIR)\DlgCopyLogs.sbr" \
	"$(INTDIR)\DlgDaysSummary.sbr" \
	"$(INTDIR)\DlgDirection.sbr" \
	"$(INTDIR)\DlgDirectionSummary.sbr" \
	"$(INTDIR)\DlgEventSummary.sbr" \
	"$(INTDIR)\DlgExport.sbr" \
	"$(INTDIR)\DlgFindEvent.sbr" \
	"$(INTDIR)\DlgFindMeas.sbr" \
	"$(INTDIR)\DlgImportAll.sbr" \
	"$(INTDIR)\DlgimportLatest.sbr" \
	"$(INTDIR)\DlgImportSome.sbr" \
	"$(INTDIR)\DlgIntegrity.sbr" \
	"$(INTDIR)\DlgMarkSummary.sbr" \
	"$(INTDIR)\DlgMeasSummary.sbr" \
	"$(INTDIR)\DlgRawData.sbr" \
	"$(INTDIR)\DlgRestoreCfg.sbr" \
	"$(INTDIR)\DlgRestoreDb.sbr" \
	"$(INTDIR)\DlgReviewPeriod.sbr" \
	"$(INTDIR)\DlgSelectFacility.sbr" \
	"$(INTDIR)\EOSSImport.sbr" \
	"$(INTDIR)\EventNetArea.sbr" \
	"$(INTDIR)\EventSummary.sbr" \
	"$(INTDIR)\EventTypeLists.sbr" \
	"$(INTDIR)\FacilityConfig.sbr" \
	"$(INTDIR)\FindEvent.sbr" \
	"$(INTDIR)\FindMeas.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\GETFILE.sbr" \
	"$(INTDIR)\GrandImport.sbr" \
	"$(INTDIR)\GUI_Glue.sbr" \
	"$(INTDIR)\GUI_PLOT.SBR" \
	"$(INTDIR)\Initfile.sbr" \
	"$(INTDIR)\InputDataFiles.sbr" \
	"$(INTDIR)\Interval.sbr" \
	"$(INTDIR)\MainDbCfg.sbr" \
	"$(INTDIR)\MyDateTime.sbr" \
	"$(INTDIR)\OleTimeUtils.sbr" \
	"$(INTDIR)\Patches.sbr" \
	"$(INTDIR)\PerformTimer.sbr" \
	"$(INTDIR)\PIPE.SBR" \
	"$(INTDIR)\PLOT.SBR" \
	"$(INTDIR)\PLT_DB.SBR" \
	"$(INTDIR)\PLT_DB_EOSS.SBR" \
	"$(INTDIR)\PLT_DLG.SBR" \
	"$(INTDIR)\Plt_evnt.sbr" \
	"$(INTDIR)\PLT_PANE.SBR" \
	"$(INTDIR)\PrintFacilityHeader.sbr" \
	"$(INTDIR)\ProgressBars.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\Questions.sbr" \
	"$(INTDIR)\RadError.sbr" \
	"$(INTDIR)\RADEventList.sbr" \
	"$(INTDIR)\RADEventsXML.sbr" \
	"$(INTDIR)\RadImport.sbr" \
	"$(INTDIR)\RadInit.sbr" \
	"$(INTDIR)\RadMain.sbr" \
	"$(INTDIR)\RadReanalyze.sbr" \
	"$(INTDIR)\RadReview.sbr" \
	"$(INTDIR)\RadSetup.sbr" \
	"$(INTDIR)\ReadCfg.sbr" \
	"$(INTDIR)\RevwTool.sbr" \
	"$(INTDIR)\RTERROR.SBR" \
	"$(INTDIR)\RTIMPORT.SBR" \
	"$(INTDIR)\RTINIT.SBR" \
	"$(INTDIR)\RTPIPE.SBR" \
	"$(INTDIR)\RTReanalyze.sbr" \
	"$(INTDIR)\Rtreview.sbr" \
	"$(INTDIR)\RTSETUP.SBR" \
	"$(INTDIR)\RTTITLE.SBR" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\SEBBoxes.sbr" \
	"$(INTDIR)\TextFile.sbr" \
	"$(INTDIR)\TimeAlign.sbr" \
	"$(INTDIR)\TS.sbr" \
	"$(INTDIR)\TxtWrdb.sbr" \
	"$(INTDIR)\Update.sbr" \
	"$(INTDIR)\UpperLayerGlue.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\VifmImport.sbr" \
	"$(INTDIR)\WriteNCCFile.sbr" \
	"$(INTDIR)\WriteXML.sbr"

"$(OUTDIR)\RAD.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\IRS_DEVCOMMON\Winlib32\plot.lib nafxcw.lib imagehlp.lib ..\..\IRS_DEVCOMMON\Winlib32\gen32.lib ..\..\IRS_DEVCOMMON\Winlib32\gui32.lib ..\..\IRS_DEVCOMMON\Check_Signature\Check_Signature.lib ..\..\IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\RAD.pdb" /debug /machine:I386 /nodefaultlib:"nafxcw.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /out:"c:\irs\rad\RAD.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AdjustTime.obj" \
	"$(INTDIR)\BinaryImport.obj" \
	"$(INTDIR)\BuildBandaid.obj" \
	"$(INTDIR)\BuildFilename.obj" \
	"$(INTDIR)\CopyDir.obj" \
	"$(INTDIR)\DateTimeUtils.obj" \
	"$(INTDIR)\DbDay.obj" \
	"$(INTDIR)\DbDefault.obj" \
	"$(INTDIR)\DbEvent.obj" \
	"$(INTDIR)\DbGeneralDbSupport.obj" \
	"$(INTDIR)\DbImport.obj" \
	"$(INTDIR)\DbInterval.obj" \
	"$(INTDIR)\DbLimit.obj" \
	"$(INTDIR)\DbMark.obj" \
	"$(INTDIR)\DbOperator.obj" \
	"$(INTDIR)\DbReadArray.obj" \
	"$(INTDIR)\DbReadStruct.obj" \
	"$(INTDIR)\DbTime.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\DlgBackupCfg.obj" \
	"$(INTDIR)\DlgBackupDb.obj" \
	"$(INTDIR)\DlgCopyLogs.obj" \
	"$(INTDIR)\DlgDaysSummary.obj" \
	"$(INTDIR)\DlgDirection.obj" \
	"$(INTDIR)\DlgDirectionSummary.obj" \
	"$(INTDIR)\DlgEventSummary.obj" \
	"$(INTDIR)\DlgExport.obj" \
	"$(INTDIR)\DlgFindEvent.obj" \
	"$(INTDIR)\DlgFindMeas.obj" \
	"$(INTDIR)\DlgImportAll.obj" \
	"$(INTDIR)\DlgimportLatest.obj" \
	"$(INTDIR)\DlgImportSome.obj" \
	"$(INTDIR)\DlgIntegrity.obj" \
	"$(INTDIR)\DlgMarkSummary.obj" \
	"$(INTDIR)\DlgMeasSummary.obj" \
	"$(INTDIR)\DlgRawData.obj" \
	"$(INTDIR)\DlgRestoreCfg.obj" \
	"$(INTDIR)\DlgRestoreDb.obj" \
	"$(INTDIR)\DlgReviewPeriod.obj" \
	"$(INTDIR)\DlgSelectFacility.obj" \
	"$(INTDIR)\EOSSImport.obj" \
	"$(INTDIR)\EventNetArea.obj" \
	"$(INTDIR)\EventSummary.obj" \
	"$(INTDIR)\EventTypeLists.obj" \
	"$(INTDIR)\FacilityConfig.obj" \
	"$(INTDIR)\FindEvent.obj" \
	"$(INTDIR)\FindMeas.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\GETFILE.obj" \
	"$(INTDIR)\GrandImport.obj" \
	"$(INTDIR)\GUI_Glue.obj" \
	"$(INTDIR)\GUI_PLOT.OBJ" \
	"$(INTDIR)\Initfile.obj" \
	"$(INTDIR)\InputDataFiles.obj" \
	"$(INTDIR)\Interval.obj" \
	"$(INTDIR)\MainDbCfg.obj" \
	"$(INTDIR)\MyDateTime.obj" \
	"$(INTDIR)\OleTimeUtils.obj" \
	"$(INTDIR)\Patches.obj" \
	"$(INTDIR)\PerformTimer.obj" \
	"$(INTDIR)\PIPE.OBJ" \
	"$(INTDIR)\PLOT.OBJ" \
	"$(INTDIR)\PLT_DB.OBJ" \
	"$(INTDIR)\PLT_DB_EOSS.OBJ" \
	"$(INTDIR)\PLT_DLG.OBJ" \
	"$(INTDIR)\Plt_evnt.obj" \
	"$(INTDIR)\PLT_PANE.OBJ" \
	"$(INTDIR)\PrintFacilityHeader.obj" \
	"$(INTDIR)\ProgressBars.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\Questions.obj" \
	"$(INTDIR)\RadError.obj" \
	"$(INTDIR)\RADEventList.obj" \
	"$(INTDIR)\RADEventsXML.obj" \
	"$(INTDIR)\RadImport.obj" \
	"$(INTDIR)\RadInit.obj" \
	"$(INTDIR)\RadMain.obj" \
	"$(INTDIR)\RadReanalyze.obj" \
	"$(INTDIR)\RadReview.obj" \
	"$(INTDIR)\RadSetup.obj" \
	"$(INTDIR)\ReadCfg.obj" \
	"$(INTDIR)\RevwTool.obj" \
	"$(INTDIR)\RTERROR.OBJ" \
	"$(INTDIR)\RTIMPORT.OBJ" \
	"$(INTDIR)\RTINIT.OBJ" \
	"$(INTDIR)\RTPIPE.OBJ" \
	"$(INTDIR)\RTReanalyze.obj" \
	"$(INTDIR)\Rtreview.obj" \
	"$(INTDIR)\RTSETUP.OBJ" \
	"$(INTDIR)\RTTITLE.OBJ" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\SEBBoxes.obj" \
	"$(INTDIR)\TextFile.obj" \
	"$(INTDIR)\TimeAlign.obj" \
	"$(INTDIR)\TS.obj" \
	"$(INTDIR)\TxtWrdb.obj" \
	"$(INTDIR)\Update.obj" \
	"$(INTDIR)\UpperLayerGlue.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\VifmImport.obj" \
	"$(INTDIR)\WriteNCCFile.obj" \
	"$(INTDIR)\WriteXML.obj" \
	"$(INTDIR)\RAD.res"

"..\..\..\irs\rad\RAD.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RAD - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\irs\rad\RAD.exe" "$(OUTDIR)\RAD.bsc"

!ELSE 

ALL : "CoLinearTimeAlign - Win32 Debug" "CoGrandImport - Win32 Debug" "CoBinaryImport - Win32 Debug" "CoVifmImport - Win32 Debug" "..\..\..\irs\rad\RAD.exe" "$(OUTDIR)\RAD.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"CoVifmImport - Win32 DebugCLEAN" "CoBinaryImport - Win32 DebugCLEAN" "CoGrandImport - Win32 DebugCLEAN" "CoLinearTimeAlign - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AdjustTime.obj"
	-@erase "$(INTDIR)\AdjustTime.sbr"
	-@erase "$(INTDIR)\BinaryImport.obj"
	-@erase "$(INTDIR)\BinaryImport.sbr"
	-@erase "$(INTDIR)\BuildBandaid.obj"
	-@erase "$(INTDIR)\BuildBandaid.sbr"
	-@erase "$(INTDIR)\BuildFilename.obj"
	-@erase "$(INTDIR)\BuildFilename.sbr"
	-@erase "$(INTDIR)\CopyDir.obj"
	-@erase "$(INTDIR)\CopyDir.sbr"
	-@erase "$(INTDIR)\DateTimeUtils.obj"
	-@erase "$(INTDIR)\DateTimeUtils.sbr"
	-@erase "$(INTDIR)\DbDay.obj"
	-@erase "$(INTDIR)\DbDay.sbr"
	-@erase "$(INTDIR)\DbDefault.obj"
	-@erase "$(INTDIR)\DbDefault.sbr"
	-@erase "$(INTDIR)\DbEvent.obj"
	-@erase "$(INTDIR)\DbEvent.sbr"
	-@erase "$(INTDIR)\DbGeneralDbSupport.obj"
	-@erase "$(INTDIR)\DbGeneralDbSupport.sbr"
	-@erase "$(INTDIR)\DbImport.obj"
	-@erase "$(INTDIR)\DbImport.sbr"
	-@erase "$(INTDIR)\DbInterval.obj"
	-@erase "$(INTDIR)\DbInterval.sbr"
	-@erase "$(INTDIR)\DbLimit.obj"
	-@erase "$(INTDIR)\DbLimit.sbr"
	-@erase "$(INTDIR)\DbMark.obj"
	-@erase "$(INTDIR)\DbMark.sbr"
	-@erase "$(INTDIR)\DbOperator.obj"
	-@erase "$(INTDIR)\DbOperator.sbr"
	-@erase "$(INTDIR)\DbReadArray.obj"
	-@erase "$(INTDIR)\DbReadArray.sbr"
	-@erase "$(INTDIR)\DbReadStruct.obj"
	-@erase "$(INTDIR)\DbReadStruct.sbr"
	-@erase "$(INTDIR)\DbTime.obj"
	-@erase "$(INTDIR)\DbTime.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Display.sbr"
	-@erase "$(INTDIR)\DlgBackupCfg.obj"
	-@erase "$(INTDIR)\DlgBackupCfg.sbr"
	-@erase "$(INTDIR)\DlgBackupDb.obj"
	-@erase "$(INTDIR)\DlgBackupDb.sbr"
	-@erase "$(INTDIR)\DlgCopyLogs.obj"
	-@erase "$(INTDIR)\DlgCopyLogs.sbr"
	-@erase "$(INTDIR)\DlgDaysSummary.obj"
	-@erase "$(INTDIR)\DlgDaysSummary.sbr"
	-@erase "$(INTDIR)\DlgDirection.obj"
	-@erase "$(INTDIR)\DlgDirection.sbr"
	-@erase "$(INTDIR)\DlgDirectionSummary.obj"
	-@erase "$(INTDIR)\DlgDirectionSummary.sbr"
	-@erase "$(INTDIR)\DlgEventSummary.obj"
	-@erase "$(INTDIR)\DlgEventSummary.sbr"
	-@erase "$(INTDIR)\DlgExport.obj"
	-@erase "$(INTDIR)\DlgExport.sbr"
	-@erase "$(INTDIR)\DlgFindEvent.obj"
	-@erase "$(INTDIR)\DlgFindEvent.sbr"
	-@erase "$(INTDIR)\DlgFindMeas.obj"
	-@erase "$(INTDIR)\DlgFindMeas.sbr"
	-@erase "$(INTDIR)\DlgImportAll.obj"
	-@erase "$(INTDIR)\DlgImportAll.sbr"
	-@erase "$(INTDIR)\DlgimportLatest.obj"
	-@erase "$(INTDIR)\DlgimportLatest.sbr"
	-@erase "$(INTDIR)\DlgImportSome.obj"
	-@erase "$(INTDIR)\DlgImportSome.sbr"
	-@erase "$(INTDIR)\DlgIntegrity.obj"
	-@erase "$(INTDIR)\DlgIntegrity.sbr"
	-@erase "$(INTDIR)\DlgMarkSummary.obj"
	-@erase "$(INTDIR)\DlgMarkSummary.sbr"
	-@erase "$(INTDIR)\DlgMeasSummary.obj"
	-@erase "$(INTDIR)\DlgMeasSummary.sbr"
	-@erase "$(INTDIR)\DlgRawData.obj"
	-@erase "$(INTDIR)\DlgRawData.sbr"
	-@erase "$(INTDIR)\DlgRestoreCfg.obj"
	-@erase "$(INTDIR)\DlgRestoreCfg.sbr"
	-@erase "$(INTDIR)\DlgRestoreDb.obj"
	-@erase "$(INTDIR)\DlgRestoreDb.sbr"
	-@erase "$(INTDIR)\DlgReviewPeriod.obj"
	-@erase "$(INTDIR)\DlgReviewPeriod.sbr"
	-@erase "$(INTDIR)\DlgSelectFacility.obj"
	-@erase "$(INTDIR)\DlgSelectFacility.sbr"
	-@erase "$(INTDIR)\EOSSImport.obj"
	-@erase "$(INTDIR)\EOSSImport.sbr"
	-@erase "$(INTDIR)\EventNetArea.obj"
	-@erase "$(INTDIR)\EventNetArea.sbr"
	-@erase "$(INTDIR)\EventSummary.obj"
	-@erase "$(INTDIR)\EventSummary.sbr"
	-@erase "$(INTDIR)\EventTypeLists.obj"
	-@erase "$(INTDIR)\EventTypeLists.sbr"
	-@erase "$(INTDIR)\FacilityConfig.obj"
	-@erase "$(INTDIR)\FacilityConfig.sbr"
	-@erase "$(INTDIR)\FindEvent.obj"
	-@erase "$(INTDIR)\FindEvent.sbr"
	-@erase "$(INTDIR)\FindMeas.obj"
	-@erase "$(INTDIR)\FindMeas.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
	-@erase "$(INTDIR)\GETFILE.obj"
	-@erase "$(INTDIR)\GETFILE.sbr"
	-@erase "$(INTDIR)\GrandImport.obj"
	-@erase "$(INTDIR)\GrandImport.sbr"
	-@erase "$(INTDIR)\GUI_Glue.obj"
	-@erase "$(INTDIR)\GUI_Glue.sbr"
	-@erase "$(INTDIR)\GUI_PLOT.OBJ"
	-@erase "$(INTDIR)\GUI_PLOT.SBR"
	-@erase "$(INTDIR)\Initfile.obj"
	-@erase "$(INTDIR)\Initfile.sbr"
	-@erase "$(INTDIR)\InputDataFiles.obj"
	-@erase "$(INTDIR)\InputDataFiles.sbr"
	-@erase "$(INTDIR)\Interval.obj"
	-@erase "$(INTDIR)\Interval.sbr"
	-@erase "$(INTDIR)\MainDbCfg.obj"
	-@erase "$(INTDIR)\MainDbCfg.sbr"
	-@erase "$(INTDIR)\MyDateTime.obj"
	-@erase "$(INTDIR)\MyDateTime.sbr"
	-@erase "$(INTDIR)\OleTimeUtils.obj"
	-@erase "$(INTDIR)\OleTimeUtils.sbr"
	-@erase "$(INTDIR)\Patches.obj"
	-@erase "$(INTDIR)\Patches.sbr"
	-@erase "$(INTDIR)\PerformTimer.obj"
	-@erase "$(INTDIR)\PerformTimer.sbr"
	-@erase "$(INTDIR)\PIPE.OBJ"
	-@erase "$(INTDIR)\PIPE.SBR"
	-@erase "$(INTDIR)\PLOT.OBJ"
	-@erase "$(INTDIR)\PLOT.SBR"
	-@erase "$(INTDIR)\PLT_DB.OBJ"
	-@erase "$(INTDIR)\PLT_DB.SBR"
	-@erase "$(INTDIR)\PLT_DB_EOSS.OBJ"
	-@erase "$(INTDIR)\PLT_DB_EOSS.SBR"
	-@erase "$(INTDIR)\PLT_DLG.OBJ"
	-@erase "$(INTDIR)\PLT_DLG.SBR"
	-@erase "$(INTDIR)\Plt_evnt.obj"
	-@erase "$(INTDIR)\Plt_evnt.sbr"
	-@erase "$(INTDIR)\PLT_PANE.OBJ"
	-@erase "$(INTDIR)\PLT_PANE.SBR"
	-@erase "$(INTDIR)\PrintFacilityHeader.obj"
	-@erase "$(INTDIR)\PrintFacilityHeader.sbr"
	-@erase "$(INTDIR)\ProgressBars.obj"
	-@erase "$(INTDIR)\ProgressBars.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\Questions.obj"
	-@erase "$(INTDIR)\Questions.sbr"
	-@erase "$(INTDIR)\RAD.res"
	-@erase "$(INTDIR)\RadError.obj"
	-@erase "$(INTDIR)\RadError.sbr"
	-@erase "$(INTDIR)\RADEventList.obj"
	-@erase "$(INTDIR)\RADEventList.sbr"
	-@erase "$(INTDIR)\RADEventsXML.obj"
	-@erase "$(INTDIR)\RADEventsXML.sbr"
	-@erase "$(INTDIR)\RadImport.obj"
	-@erase "$(INTDIR)\RadImport.sbr"
	-@erase "$(INTDIR)\RadInit.obj"
	-@erase "$(INTDIR)\RadInit.sbr"
	-@erase "$(INTDIR)\RadMain.obj"
	-@erase "$(INTDIR)\RadMain.sbr"
	-@erase "$(INTDIR)\RadReanalyze.obj"
	-@erase "$(INTDIR)\RadReanalyze.sbr"
	-@erase "$(INTDIR)\RadReview.obj"
	-@erase "$(INTDIR)\RadReview.sbr"
	-@erase "$(INTDIR)\RadSetup.obj"
	-@erase "$(INTDIR)\RadSetup.sbr"
	-@erase "$(INTDIR)\ReadCfg.obj"
	-@erase "$(INTDIR)\ReadCfg.sbr"
	-@erase "$(INTDIR)\RevwTool.obj"
	-@erase "$(INTDIR)\RevwTool.sbr"
	-@erase "$(INTDIR)\RTERROR.OBJ"
	-@erase "$(INTDIR)\RTERROR.SBR"
	-@erase "$(INTDIR)\RTIMPORT.OBJ"
	-@erase "$(INTDIR)\RTIMPORT.SBR"
	-@erase "$(INTDIR)\RTINIT.OBJ"
	-@erase "$(INTDIR)\RTINIT.SBR"
	-@erase "$(INTDIR)\RTPIPE.OBJ"
	-@erase "$(INTDIR)\RTPIPE.SBR"
	-@erase "$(INTDIR)\RTReanalyze.obj"
	-@erase "$(INTDIR)\RTReanalyze.sbr"
	-@erase "$(INTDIR)\Rtreview.obj"
	-@erase "$(INTDIR)\Rtreview.sbr"
	-@erase "$(INTDIR)\RTSETUP.OBJ"
	-@erase "$(INTDIR)\RTSETUP.SBR"
	-@erase "$(INTDIR)\RTTITLE.OBJ"
	-@erase "$(INTDIR)\RTTITLE.SBR"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\SEBBoxes.obj"
	-@erase "$(INTDIR)\SEBBoxes.sbr"
	-@erase "$(INTDIR)\TextFile.obj"
	-@erase "$(INTDIR)\TextFile.sbr"
	-@erase "$(INTDIR)\TimeAlign.obj"
	-@erase "$(INTDIR)\TimeAlign.sbr"
	-@erase "$(INTDIR)\TS.obj"
	-@erase "$(INTDIR)\TS.sbr"
	-@erase "$(INTDIR)\TxtWrdb.obj"
	-@erase "$(INTDIR)\TxtWrdb.sbr"
	-@erase "$(INTDIR)\Update.obj"
	-@erase "$(INTDIR)\Update.sbr"
	-@erase "$(INTDIR)\UpperLayerGlue.obj"
	-@erase "$(INTDIR)\UpperLayerGlue.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VifmImport.obj"
	-@erase "$(INTDIR)\VifmImport.sbr"
	-@erase "$(INTDIR)\WriteNCCFile.obj"
	-@erase "$(INTDIR)\WriteNCCFile.sbr"
	-@erase "$(INTDIR)\WriteXML.obj"
	-@erase "$(INTDIR)\WriteXML.sbr"
	-@erase "$(OUTDIR)\RAD.bsc"
	-@erase "$(OUTDIR)\RAD.pdb"
	-@erase "..\..\..\irs\rad\RAD.exe"
	-@erase "..\..\..\irs\rad\RAD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "c:\Program Files\MSXML 6.0\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "Z_NP" /D "RW_NO_STL" /D "MSC" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\RAD.res" /i "..\..\IRS_DEVCOMMON\UpperLayer" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RAD.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AdjustTime.sbr" \
	"$(INTDIR)\BinaryImport.sbr" \
	"$(INTDIR)\BuildBandaid.sbr" \
	"$(INTDIR)\BuildFilename.sbr" \
	"$(INTDIR)\CopyDir.sbr" \
	"$(INTDIR)\DateTimeUtils.sbr" \
	"$(INTDIR)\DbDay.sbr" \
	"$(INTDIR)\DbDefault.sbr" \
	"$(INTDIR)\DbEvent.sbr" \
	"$(INTDIR)\DbGeneralDbSupport.sbr" \
	"$(INTDIR)\DbImport.sbr" \
	"$(INTDIR)\DbInterval.sbr" \
	"$(INTDIR)\DbLimit.sbr" \
	"$(INTDIR)\DbMark.sbr" \
	"$(INTDIR)\DbOperator.sbr" \
	"$(INTDIR)\DbReadArray.sbr" \
	"$(INTDIR)\DbReadStruct.sbr" \
	"$(INTDIR)\DbTime.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\Display.sbr" \
	"$(INTDIR)\DlgBackupCfg.sbr" \
	"$(INTDIR)\DlgBackupDb.sbr" \
	"$(INTDIR)\DlgCopyLogs.sbr" \
	"$(INTDIR)\DlgDaysSummary.sbr" \
	"$(INTDIR)\DlgDirection.sbr" \
	"$(INTDIR)\DlgDirectionSummary.sbr" \
	"$(INTDIR)\DlgEventSummary.sbr" \
	"$(INTDIR)\DlgExport.sbr" \
	"$(INTDIR)\DlgFindEvent.sbr" \
	"$(INTDIR)\DlgFindMeas.sbr" \
	"$(INTDIR)\DlgImportAll.sbr" \
	"$(INTDIR)\DlgimportLatest.sbr" \
	"$(INTDIR)\DlgImportSome.sbr" \
	"$(INTDIR)\DlgIntegrity.sbr" \
	"$(INTDIR)\DlgMarkSummary.sbr" \
	"$(INTDIR)\DlgMeasSummary.sbr" \
	"$(INTDIR)\DlgRawData.sbr" \
	"$(INTDIR)\DlgRestoreCfg.sbr" \
	"$(INTDIR)\DlgRestoreDb.sbr" \
	"$(INTDIR)\DlgReviewPeriod.sbr" \
	"$(INTDIR)\DlgSelectFacility.sbr" \
	"$(INTDIR)\EOSSImport.sbr" \
	"$(INTDIR)\EventNetArea.sbr" \
	"$(INTDIR)\EventSummary.sbr" \
	"$(INTDIR)\EventTypeLists.sbr" \
	"$(INTDIR)\FacilityConfig.sbr" \
	"$(INTDIR)\FindEvent.sbr" \
	"$(INTDIR)\FindMeas.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\GETFILE.sbr" \
	"$(INTDIR)\GrandImport.sbr" \
	"$(INTDIR)\GUI_Glue.sbr" \
	"$(INTDIR)\GUI_PLOT.SBR" \
	"$(INTDIR)\Initfile.sbr" \
	"$(INTDIR)\InputDataFiles.sbr" \
	"$(INTDIR)\Interval.sbr" \
	"$(INTDIR)\MainDbCfg.sbr" \
	"$(INTDIR)\MyDateTime.sbr" \
	"$(INTDIR)\OleTimeUtils.sbr" \
	"$(INTDIR)\Patches.sbr" \
	"$(INTDIR)\PerformTimer.sbr" \
	"$(INTDIR)\PIPE.SBR" \
	"$(INTDIR)\PLOT.SBR" \
	"$(INTDIR)\PLT_DB.SBR" \
	"$(INTDIR)\PLT_DB_EOSS.SBR" \
	"$(INTDIR)\PLT_DLG.SBR" \
	"$(INTDIR)\Plt_evnt.sbr" \
	"$(INTDIR)\PLT_PANE.SBR" \
	"$(INTDIR)\PrintFacilityHeader.sbr" \
	"$(INTDIR)\ProgressBars.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\Questions.sbr" \
	"$(INTDIR)\RadError.sbr" \
	"$(INTDIR)\RADEventList.sbr" \
	"$(INTDIR)\RADEventsXML.sbr" \
	"$(INTDIR)\RadImport.sbr" \
	"$(INTDIR)\RadInit.sbr" \
	"$(INTDIR)\RadMain.sbr" \
	"$(INTDIR)\RadReanalyze.sbr" \
	"$(INTDIR)\RadReview.sbr" \
	"$(INTDIR)\RadSetup.sbr" \
	"$(INTDIR)\ReadCfg.sbr" \
	"$(INTDIR)\RevwTool.sbr" \
	"$(INTDIR)\RTERROR.SBR" \
	"$(INTDIR)\RTIMPORT.SBR" \
	"$(INTDIR)\RTINIT.SBR" \
	"$(INTDIR)\RTPIPE.SBR" \
	"$(INTDIR)\RTReanalyze.sbr" \
	"$(INTDIR)\Rtreview.sbr" \
	"$(INTDIR)\RTSETUP.SBR" \
	"$(INTDIR)\RTTITLE.SBR" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\SEBBoxes.sbr" \
	"$(INTDIR)\TextFile.sbr" \
	"$(INTDIR)\TimeAlign.sbr" \
	"$(INTDIR)\TS.sbr" \
	"$(INTDIR)\TxtWrdb.sbr" \
	"$(INTDIR)\Update.sbr" \
	"$(INTDIR)\UpperLayerGlue.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\VifmImport.sbr" \
	"$(INTDIR)\WriteNCCFile.sbr" \
	"$(INTDIR)\WriteXML.sbr"

"$(OUTDIR)\RAD.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\IRS_DEVCOMMON\Winlib32\plot.lib nafxcwd.lib imagehlp.lib ..\..\IRS_DEVCOMMON\Winlib32\gen32.lib ..\..\IRS_DEVCOMMON\Winlib32\gui32.lib ..\..\IRS_DEVCOMMON\Check_Signature\Check_Signature.lib ..\..\IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\RAD.pdb" /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrtd.lib" /out:"c:\irs\rad\RAD.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AdjustTime.obj" \
	"$(INTDIR)\BinaryImport.obj" \
	"$(INTDIR)\BuildBandaid.obj" \
	"$(INTDIR)\BuildFilename.obj" \
	"$(INTDIR)\CopyDir.obj" \
	"$(INTDIR)\DateTimeUtils.obj" \
	"$(INTDIR)\DbDay.obj" \
	"$(INTDIR)\DbDefault.obj" \
	"$(INTDIR)\DbEvent.obj" \
	"$(INTDIR)\DbGeneralDbSupport.obj" \
	"$(INTDIR)\DbImport.obj" \
	"$(INTDIR)\DbInterval.obj" \
	"$(INTDIR)\DbLimit.obj" \
	"$(INTDIR)\DbMark.obj" \
	"$(INTDIR)\DbOperator.obj" \
	"$(INTDIR)\DbReadArray.obj" \
	"$(INTDIR)\DbReadStruct.obj" \
	"$(INTDIR)\DbTime.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\DlgBackupCfg.obj" \
	"$(INTDIR)\DlgBackupDb.obj" \
	"$(INTDIR)\DlgCopyLogs.obj" \
	"$(INTDIR)\DlgDaysSummary.obj" \
	"$(INTDIR)\DlgDirection.obj" \
	"$(INTDIR)\DlgDirectionSummary.obj" \
	"$(INTDIR)\DlgEventSummary.obj" \
	"$(INTDIR)\DlgExport.obj" \
	"$(INTDIR)\DlgFindEvent.obj" \
	"$(INTDIR)\DlgFindMeas.obj" \
	"$(INTDIR)\DlgImportAll.obj" \
	"$(INTDIR)\DlgimportLatest.obj" \
	"$(INTDIR)\DlgImportSome.obj" \
	"$(INTDIR)\DlgIntegrity.obj" \
	"$(INTDIR)\DlgMarkSummary.obj" \
	"$(INTDIR)\DlgMeasSummary.obj" \
	"$(INTDIR)\DlgRawData.obj" \
	"$(INTDIR)\DlgRestoreCfg.obj" \
	"$(INTDIR)\DlgRestoreDb.obj" \
	"$(INTDIR)\DlgReviewPeriod.obj" \
	"$(INTDIR)\DlgSelectFacility.obj" \
	"$(INTDIR)\EOSSImport.obj" \
	"$(INTDIR)\EventNetArea.obj" \
	"$(INTDIR)\EventSummary.obj" \
	"$(INTDIR)\EventTypeLists.obj" \
	"$(INTDIR)\FacilityConfig.obj" \
	"$(INTDIR)\FindEvent.obj" \
	"$(INTDIR)\FindMeas.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\GETFILE.obj" \
	"$(INTDIR)\GrandImport.obj" \
	"$(INTDIR)\GUI_Glue.obj" \
	"$(INTDIR)\GUI_PLOT.OBJ" \
	"$(INTDIR)\Initfile.obj" \
	"$(INTDIR)\InputDataFiles.obj" \
	"$(INTDIR)\Interval.obj" \
	"$(INTDIR)\MainDbCfg.obj" \
	"$(INTDIR)\MyDateTime.obj" \
	"$(INTDIR)\OleTimeUtils.obj" \
	"$(INTDIR)\Patches.obj" \
	"$(INTDIR)\PerformTimer.obj" \
	"$(INTDIR)\PIPE.OBJ" \
	"$(INTDIR)\PLOT.OBJ" \
	"$(INTDIR)\PLT_DB.OBJ" \
	"$(INTDIR)\PLT_DB_EOSS.OBJ" \
	"$(INTDIR)\PLT_DLG.OBJ" \
	"$(INTDIR)\Plt_evnt.obj" \
	"$(INTDIR)\PLT_PANE.OBJ" \
	"$(INTDIR)\PrintFacilityHeader.obj" \
	"$(INTDIR)\ProgressBars.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\Questions.obj" \
	"$(INTDIR)\RadError.obj" \
	"$(INTDIR)\RADEventList.obj" \
	"$(INTDIR)\RADEventsXML.obj" \
	"$(INTDIR)\RadImport.obj" \
	"$(INTDIR)\RadInit.obj" \
	"$(INTDIR)\RadMain.obj" \
	"$(INTDIR)\RadReanalyze.obj" \
	"$(INTDIR)\RadReview.obj" \
	"$(INTDIR)\RadSetup.obj" \
	"$(INTDIR)\ReadCfg.obj" \
	"$(INTDIR)\RevwTool.obj" \
	"$(INTDIR)\RTERROR.OBJ" \
	"$(INTDIR)\RTIMPORT.OBJ" \
	"$(INTDIR)\RTINIT.OBJ" \
	"$(INTDIR)\RTPIPE.OBJ" \
	"$(INTDIR)\RTReanalyze.obj" \
	"$(INTDIR)\Rtreview.obj" \
	"$(INTDIR)\RTSETUP.OBJ" \
	"$(INTDIR)\RTTITLE.OBJ" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\SEBBoxes.obj" \
	"$(INTDIR)\TextFile.obj" \
	"$(INTDIR)\TimeAlign.obj" \
	"$(INTDIR)\TS.obj" \
	"$(INTDIR)\TxtWrdb.obj" \
	"$(INTDIR)\Update.obj" \
	"$(INTDIR)\UpperLayerGlue.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\VifmImport.obj" \
	"$(INTDIR)\WriteNCCFile.obj" \
	"$(INTDIR)\WriteXML.obj" \
	"$(INTDIR)\RAD.res" \
	"..\CoVifmImport\Debug\CoVifmImport.lib" \
	"..\CoBinaryImport\Debug\CoBinaryImport.lib" \
	"..\CoGrandImport\Debug\CoGrandImport.lib" \
	"..\CoLinearTimeAlign\Debug\CoLinearTimeAlign.lib"

"..\..\..\irs\rad\RAD.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RAD.DEP")
!INCLUDE "RAD.DEP"
!ELSE 
!MESSAGE Warning: cannot find "RAD.DEP"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RAD - Win32 Release" || "$(CFG)" == "RAD - Win32 Debug"
SOURCE=.\AdjustTime.cpp

"$(INTDIR)\AdjustTime.obj"	"$(INTDIR)\AdjustTime.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BinaryImport.cpp

"$(INTDIR)\BinaryImport.obj"	"$(INTDIR)\BinaryImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BuildBandaid.cpp

"$(INTDIR)\BuildBandaid.obj"	"$(INTDIR)\BuildBandaid.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BuildFilename.cpp

"$(INTDIR)\BuildFilename.obj"	"$(INTDIR)\BuildFilename.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CopyDir.cpp

"$(INTDIR)\CopyDir.obj"	"$(INTDIR)\CopyDir.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DateTimeUtils.cpp

"$(INTDIR)\DateTimeUtils.obj"	"$(INTDIR)\DateTimeUtils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbDay.cpp

"$(INTDIR)\DbDay.obj"	"$(INTDIR)\DbDay.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbDefault.cpp

"$(INTDIR)\DbDefault.obj"	"$(INTDIR)\DbDefault.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbEvent.cpp

"$(INTDIR)\DbEvent.obj"	"$(INTDIR)\DbEvent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbGeneralDbSupport.cpp

"$(INTDIR)\DbGeneralDbSupport.obj"	"$(INTDIR)\DbGeneralDbSupport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbImport.cpp

"$(INTDIR)\DbImport.obj"	"$(INTDIR)\DbImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbInterval.cpp

"$(INTDIR)\DbInterval.obj"	"$(INTDIR)\DbInterval.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbLimit.cpp

"$(INTDIR)\DbLimit.obj"	"$(INTDIR)\DbLimit.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbMark.cpp

"$(INTDIR)\DbMark.obj"	"$(INTDIR)\DbMark.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbOperator.CPP

"$(INTDIR)\DbOperator.obj"	"$(INTDIR)\DbOperator.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbReadArray.cpp

"$(INTDIR)\DbReadArray.obj"	"$(INTDIR)\DbReadArray.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbReadStruct.cpp

"$(INTDIR)\DbReadStruct.obj"	"$(INTDIR)\DbReadStruct.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DbTime.cpp

"$(INTDIR)\DbTime.obj"	"$(INTDIR)\DbTime.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\DirUtilities.cpp

"$(INTDIR)\DirUtilities.obj"	"$(INTDIR)\DirUtilities.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Display.cpp

"$(INTDIR)\Display.obj"	"$(INTDIR)\Display.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgBackupCfg.cpp

"$(INTDIR)\DlgBackupCfg.obj"	"$(INTDIR)\DlgBackupCfg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgBackupDb.cpp

"$(INTDIR)\DlgBackupDb.obj"	"$(INTDIR)\DlgBackupDb.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgCopyLogs.cpp

"$(INTDIR)\DlgCopyLogs.obj"	"$(INTDIR)\DlgCopyLogs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgDaysSummary.cpp

"$(INTDIR)\DlgDaysSummary.obj"	"$(INTDIR)\DlgDaysSummary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgDirection.cpp

"$(INTDIR)\DlgDirection.obj"	"$(INTDIR)\DlgDirection.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgDirectionSummary.cpp

"$(INTDIR)\DlgDirectionSummary.obj"	"$(INTDIR)\DlgDirectionSummary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgEventSummary.cpp

"$(INTDIR)\DlgEventSummary.obj"	"$(INTDIR)\DlgEventSummary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgExport.cpp

"$(INTDIR)\DlgExport.obj"	"$(INTDIR)\DlgExport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgFindEvent.cpp

"$(INTDIR)\DlgFindEvent.obj"	"$(INTDIR)\DlgFindEvent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgFindMeas.cpp

"$(INTDIR)\DlgFindMeas.obj"	"$(INTDIR)\DlgFindMeas.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgImportAll.cpp

"$(INTDIR)\DlgImportAll.obj"	"$(INTDIR)\DlgImportAll.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgimportLatest.cpp

"$(INTDIR)\DlgimportLatest.obj"	"$(INTDIR)\DlgimportLatest.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgImportSome.cpp

"$(INTDIR)\DlgImportSome.obj"	"$(INTDIR)\DlgImportSome.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgIntegrity.cpp

"$(INTDIR)\DlgIntegrity.obj"	"$(INTDIR)\DlgIntegrity.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgMarkSummary.cpp

"$(INTDIR)\DlgMarkSummary.obj"	"$(INTDIR)\DlgMarkSummary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgMeasSummary.cpp

"$(INTDIR)\DlgMeasSummary.obj"	"$(INTDIR)\DlgMeasSummary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgRawData.cpp

"$(INTDIR)\DlgRawData.obj"	"$(INTDIR)\DlgRawData.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgRestoreCfg.cpp

"$(INTDIR)\DlgRestoreCfg.obj"	"$(INTDIR)\DlgRestoreCfg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgRestoreDb.cpp

"$(INTDIR)\DlgRestoreDb.obj"	"$(INTDIR)\DlgRestoreDb.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgReviewPeriod.cpp

"$(INTDIR)\DlgReviewPeriod.obj"	"$(INTDIR)\DlgReviewPeriod.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSelectFacility.cpp

"$(INTDIR)\DlgSelectFacility.obj"	"$(INTDIR)\DlgSelectFacility.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EOSSImport.cpp

"$(INTDIR)\EOSSImport.obj"	"$(INTDIR)\EOSSImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventNetArea.cpp

"$(INTDIR)\EventNetArea.obj"	"$(INTDIR)\EventNetArea.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventSummary.cpp

"$(INTDIR)\EventSummary.obj"	"$(INTDIR)\EventSummary.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventTypeLists.cpp

"$(INTDIR)\EventTypeLists.obj"	"$(INTDIR)\EventTypeLists.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FacilityConfig.cpp

"$(INTDIR)\FacilityConfig.obj"	"$(INTDIR)\FacilityConfig.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FindEvent.cpp

"$(INTDIR)\FindEvent.obj"	"$(INTDIR)\FindEvent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\FindMeas.cpp

"$(INTDIR)\FindMeas.obj"	"$(INTDIR)\FindMeas.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\GenDateTime.cpp

"$(INTDIR)\GenDateTime.obj"	"$(INTDIR)\GenDateTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\GETFILE.cpp

"$(INTDIR)\GETFILE.obj"	"$(INTDIR)\GETFILE.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrandImport.cpp

"$(INTDIR)\GrandImport.obj"	"$(INTDIR)\GrandImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GUI_Glue.cpp

"$(INTDIR)\GUI_Glue.obj"	"$(INTDIR)\GUI_Glue.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GUI_PLOT.CPP

"$(INTDIR)\GUI_PLOT.OBJ"	"$(INTDIR)\GUI_PLOT.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UPPERLAYER\Initfile.cpp

"$(INTDIR)\Initfile.obj"	"$(INTDIR)\Initfile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\InputDataFiles.cpp

"$(INTDIR)\InputDataFiles.obj"	"$(INTDIR)\InputDataFiles.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Interval.cpp

"$(INTDIR)\Interval.obj"	"$(INTDIR)\Interval.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MainDbCfg.cpp

"$(INTDIR)\MainDbCfg.obj"	"$(INTDIR)\MainDbCfg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\MyDateTime.cpp

"$(INTDIR)\MyDateTime.obj"	"$(INTDIR)\MyDateTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\OleTimeUtils.cpp

"$(INTDIR)\OleTimeUtils.obj"	"$(INTDIR)\OleTimeUtils.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Patches.cpp

"$(INTDIR)\Patches.obj"	"$(INTDIR)\Patches.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PerformTimer.cpp

"$(INTDIR)\PerformTimer.obj"	"$(INTDIR)\PerformTimer.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UPPERLAYER\PIPE.CPP

"$(INTDIR)\PIPE.OBJ"	"$(INTDIR)\PIPE.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PLOT.CPP

"$(INTDIR)\PLOT.OBJ"	"$(INTDIR)\PLOT.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PLT_DB.CPP

"$(INTDIR)\PLT_DB.OBJ"	"$(INTDIR)\PLT_DB.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PLT_DB_EOSS.CPP

"$(INTDIR)\PLT_DB_EOSS.OBJ"	"$(INTDIR)\PLT_DB_EOSS.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PLT_DLG.CPP

"$(INTDIR)\PLT_DLG.OBJ"	"$(INTDIR)\PLT_DLG.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Plt_evnt.cpp

"$(INTDIR)\Plt_evnt.obj"	"$(INTDIR)\Plt_evnt.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PLT_PANE.CPP

"$(INTDIR)\PLT_PANE.OBJ"	"$(INTDIR)\PLT_PANE.SBR" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PrintFacilityHeader.cpp

"$(INTDIR)\PrintFacilityHeader.obj"	"$(INTDIR)\PrintFacilityHeader.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ProgressBars.cpp

"$(INTDIR)\ProgressBars.obj"	"$(INTDIR)\ProgressBars.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\ProgressWnd.cpp

"$(INTDIR)\ProgressWnd.obj"	"$(INTDIR)\ProgressWnd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Questions.cpp

"$(INTDIR)\Questions.obj"	"$(INTDIR)\Questions.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadError.cpp

"$(INTDIR)\RadError.obj"	"$(INTDIR)\RadError.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RADEventList.cpp

"$(INTDIR)\RADEventList.obj"	"$(INTDIR)\RADEventList.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RADEventsXML.cpp

"$(INTDIR)\RADEventsXML.obj"	"$(INTDIR)\RADEventsXML.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadImport.cpp

"$(INTDIR)\RadImport.obj"	"$(INTDIR)\RadImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadInit.cpp

"$(INTDIR)\RadInit.obj"	"$(INTDIR)\RadInit.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadMain.cpp

"$(INTDIR)\RadMain.obj"	"$(INTDIR)\RadMain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadReanalyze.cpp

"$(INTDIR)\RadReanalyze.obj"	"$(INTDIR)\RadReanalyze.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadReview.cpp

"$(INTDIR)\RadReview.obj"	"$(INTDIR)\RadReview.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadSetup.cpp

"$(INTDIR)\RadSetup.obj"	"$(INTDIR)\RadSetup.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ReadCfg.cpp

"$(INTDIR)\ReadCfg.obj"	"$(INTDIR)\ReadCfg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\UPPERLAYER\RevwTool.cpp"

"$(INTDIR)\RevwTool.obj"	"$(INTDIR)\RevwTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTERROR.CPP"

"$(INTDIR)\RTERROR.OBJ"	"$(INTDIR)\RTERROR.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTIMPORT.CPP"

"$(INTDIR)\RTIMPORT.OBJ"	"$(INTDIR)\RTIMPORT.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTINIT.CPP"

"$(INTDIR)\RTINIT.OBJ"	"$(INTDIR)\RTINIT.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTPIPE.CPP"

"$(INTDIR)\RTPIPE.OBJ"	"$(INTDIR)\RTPIPE.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTReanalyze.cpp"

"$(INTDIR)\RTReanalyze.obj"	"$(INTDIR)\RTReanalyze.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\Rtreview.cpp"

"$(INTDIR)\Rtreview.obj"	"$(INTDIR)\Rtreview.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTSETUP.CPP"

"$(INTDIR)\RTSETUP.OBJ"	"$(INTDIR)\RTSETUP.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\RTTITLE.CPP"

"$(INTDIR)\RTTITLE.OBJ"	"$(INTDIR)\RTTITLE.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\SafeArrayUtil.cpp

"$(INTDIR)\SafeArrayUtil.obj"	"$(INTDIR)\SafeArrayUtil.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=".\UPPERLAYER\SEBBoxes.CPP"

"$(INTDIR)\SEBBoxes.obj"	"$(INTDIR)\SEBBoxes.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\TextFile.cpp

"$(INTDIR)\TextFile.obj"	"$(INTDIR)\TextFile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TimeAlign.cpp

"$(INTDIR)\TimeAlign.obj"	"$(INTDIR)\TimeAlign.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TS.cpp

"$(INTDIR)\TS.obj"	"$(INTDIR)\TS.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\UPPERLAYER\TxtWrdb.CPP"

"$(INTDIR)\TxtWrdb.obj"	"$(INTDIR)\TxtWrdb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Update.cpp

"$(INTDIR)\Update.obj"	"$(INTDIR)\Update.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UpperLayerGlue.cpp

"$(INTDIR)\UpperLayerGlue.obj"	"$(INTDIR)\UpperLayerGlue.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Utilities.cpp

"$(INTDIR)\Utilities.obj"	"$(INTDIR)\Utilities.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VifmImport.cpp

"$(INTDIR)\VifmImport.obj"	"$(INTDIR)\VifmImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WriteNCCFile.cpp

"$(INTDIR)\WriteNCCFile.obj"	"$(INTDIR)\WriteNCCFile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WriteXML.cpp

"$(INTDIR)\WriteXML.obj"	"$(INTDIR)\WriteXML.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RAD.RC

"$(INTDIR)\RAD.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "RAD - Win32 Release"

!ELSEIF  "$(CFG)" == "RAD - Win32 Debug"

"CoVifmImport - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoVifmImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoVifmImport.mak CFG="CoVifmImport - Win32 Debug" 
   cd "..\RAD"

"CoVifmImport - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoVifmImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoVifmImport.mak CFG="CoVifmImport - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\RAD"

!ENDIF 

!IF  "$(CFG)" == "RAD - Win32 Release"

!ELSEIF  "$(CFG)" == "RAD - Win32 Debug"

"CoBinaryImport - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoBinaryImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoBinaryImport.mak CFG="CoBinaryImport - Win32 Debug" 
   cd "..\RAD"

"CoBinaryImport - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoBinaryImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoBinaryImport.mak CFG="CoBinaryImport - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\RAD"

!ENDIF 

!IF  "$(CFG)" == "RAD - Win32 Release"

!ELSEIF  "$(CFG)" == "RAD - Win32 Debug"

"CoGrandImport - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoGrandImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoGrandImport.mak CFG="CoGrandImport - Win32 Debug" 
   cd "..\RAD"

"CoGrandImport - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoGrandImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoGrandImport.mak CFG="CoGrandImport - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\RAD"

!ENDIF 

!IF  "$(CFG)" == "RAD - Win32 Release"

!ELSEIF  "$(CFG)" == "RAD - Win32 Debug"

"CoLinearTimeAlign - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoLinearTimeAlign"
   $(MAKE) /$(MAKEFLAGS) /F .\CoLinearTimeAlign.mak CFG="CoLinearTimeAlign - Win32 Debug" 
   cd "..\RAD"

"CoLinearTimeAlign - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoLinearTimeAlign"
   $(MAKE) /$(MAKEFLAGS) /F .\CoLinearTimeAlign.mak CFG="CoLinearTimeAlign - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\RAD"

!ENDIF 


!ENDIF 

