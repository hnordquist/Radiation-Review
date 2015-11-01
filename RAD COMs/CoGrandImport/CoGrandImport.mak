# Microsoft Developer Studio Generated NMAKE File, Based on CoGrandImport.dsp
!IF "$(CFG)" == ""
CFG=CoGrandImport - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CoGrandImport - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CoGrandImport - Win32 Debug" && "$(CFG)" != "CoGrandImport - Win32 Release MinDependency"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CoGrandImport - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\irs\comlibs\CoGrandImport.dll" ".\CoGrandImport.tlb" ".\CoGrandImport.h" ".\CoGrandImport_i.c" "$(OUTDIR)\CoGrandImport.pch" "$(OUTDIR)\CoGrandImport.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\BaseFacCfgWrap.obj"
	-@erase "$(INTDIR)\BaseFacCfgWrap.sbr"
	-@erase "$(INTDIR)\CoGrandImport.obj"
	-@erase "$(INTDIR)\CoGrandImport.pch"
	-@erase "$(INTDIR)\CoGrandImport.res"
	-@erase "$(INTDIR)\CoGrandImport.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\ErrorProcessing.obj"
	-@erase "$(INTDIR)\ErrorProcessing.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
	-@erase "$(INTDIR)\GrandData.obj"
	-@erase "$(INTDIR)\GrandData.sbr"
	-@erase "$(INTDIR)\GrandDataFile.obj"
	-@erase "$(INTDIR)\GrandDataFile.sbr"
	-@erase "$(INTDIR)\GrandDbVista.obj"
	-@erase "$(INTDIR)\GrandDbVista.sbr"
	-@erase "$(INTDIR)\GrandFacCfgWrap.obj"
	-@erase "$(INTDIR)\GrandFacCfgWrap.sbr"
	-@erase "$(INTDIR)\GrandImport.obj"
	-@erase "$(INTDIR)\GrandImport.sbr"
	-@erase "$(INTDIR)\GrandImportSupport.obj"
	-@erase "$(INTDIR)\GrandImportSupport.sbr"
	-@erase "$(INTDIR)\MyDateTime.obj"
	-@erase "$(INTDIR)\MyDateTime.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VistaBaseData.obj"
	-@erase "$(INTDIR)\VistaBaseData.sbr"
	-@erase "$(INTDIR)\VistaDataFile.obj"
	-@erase "$(INTDIR)\VistaDataFile.sbr"
	-@erase "$(INTDIR)\VistaDb.obj"
	-@erase "$(INTDIR)\VistaDb.sbr"
	-@erase "$(OUTDIR)\CoGrandImport.bsc"
	-@erase "$(OUTDIR)\CoGrandImport.exp"
	-@erase "$(OUTDIR)\CoGrandImport.lib"
	-@erase "..\..\..\irs\comlibs\CoGrandImport.dll"
	-@erase "..\..\..\irs\comlibs\CoGrandImport.ilk"
	-@erase "..\..\..\irs\comlibs\CoGrandImport.pdb"
	-@erase ".\CoGrandImport.h"
	-@erase ".\CoGrandImport.tlb"
	-@erase ".\CoGrandImport_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W2 /Gm /GX /ZI /Od /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "G:\Program Files\MSXML 4.0\inc" /D "_DEBUG" /D "_MBCS" /D "MINIGRAND" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
MTL_PROJ=
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoGrandImport.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoGrandImport.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BaseFacCfgWrap.sbr" \
	"$(INTDIR)\CoGrandImport.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\ErrorProcessing.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\GrandData.sbr" \
	"$(INTDIR)\GrandDataFile.sbr" \
	"$(INTDIR)\GrandDbVista.sbr" \
	"$(INTDIR)\GrandFacCfgWrap.sbr" \
	"$(INTDIR)\GrandImport.sbr" \
	"$(INTDIR)\GrandImportSupport.sbr" \
	"$(INTDIR)\MyDateTime.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\VistaBaseData.sbr" \
	"$(INTDIR)\VistaDataFile.sbr" \
	"$(INTDIR)\VistaDb.sbr"

"$(OUTDIR)\CoGrandImport.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=nafxcwd.lib imagehlp.lib ../../IRS_DEVCOMMON\Winlib32\gui32.lib ../../IRS_DEVCOMMON\Check_Signature\Check_Signature.lib ../../IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"c:/irs/comlibs/CoGrandImport.pdb" /debug /machine:I386 /def:".\CoGrandImport.def" /out:"c:/irs/comlibs\CoGrandImport.dll" /implib:"$(OUTDIR)\CoGrandImport.lib" /MAPINFO:EXPORTS /MAPINFO:LINES 
LINK32_OBJS= \
	"$(INTDIR)\BaseFacCfgWrap.obj" \
	"$(INTDIR)\CoGrandImport.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\ErrorProcessing.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\GrandData.obj" \
	"$(INTDIR)\GrandDataFile.obj" \
	"$(INTDIR)\GrandDbVista.obj" \
	"$(INTDIR)\GrandFacCfgWrap.obj" \
	"$(INTDIR)\GrandImport.obj" \
	"$(INTDIR)\GrandImportSupport.obj" \
	"$(INTDIR)\MyDateTime.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VistaBaseData.obj" \
	"$(INTDIR)\VistaDataFile.obj" \
	"$(INTDIR)\VistaDb.obj" \
	"$(INTDIR)\CoGrandImport.res"

"..\..\..\irs\comlibs\CoGrandImport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=\irs\comlibs\CoGrandImport.dll
InputPath=\irs\comlibs\CoGrandImport.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "CoGrandImport - Win32 Release MinDependency"

OUTDIR=.\ReleaseMinDependency
INTDIR=.\ReleaseMinDependency
# Begin Custom Macros
OutDir=.\ReleaseMinDependency
# End Custom Macros

ALL : "..\..\..\irs\comlibs\CoGrandImport.dll" "$(OUTDIR)\CoGrandImport.pch" "$(OUTDIR)\CoGrandImport.bsc" ".\ReleaseMinDependency\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\BaseFacCfgWrap.obj"
	-@erase "$(INTDIR)\BaseFacCfgWrap.sbr"
	-@erase "$(INTDIR)\CoGrandImport.obj"
	-@erase "$(INTDIR)\CoGrandImport.pch"
	-@erase "$(INTDIR)\CoGrandImport.res"
	-@erase "$(INTDIR)\CoGrandImport.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\ErrorProcessing.obj"
	-@erase "$(INTDIR)\ErrorProcessing.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
	-@erase "$(INTDIR)\GrandData.obj"
	-@erase "$(INTDIR)\GrandData.sbr"
	-@erase "$(INTDIR)\GrandDataFile.obj"
	-@erase "$(INTDIR)\GrandDataFile.sbr"
	-@erase "$(INTDIR)\GrandDbVista.obj"
	-@erase "$(INTDIR)\GrandDbVista.sbr"
	-@erase "$(INTDIR)\GrandFacCfgWrap.obj"
	-@erase "$(INTDIR)\GrandFacCfgWrap.sbr"
	-@erase "$(INTDIR)\GrandImport.obj"
	-@erase "$(INTDIR)\GrandImport.sbr"
	-@erase "$(INTDIR)\GrandImportSupport.obj"
	-@erase "$(INTDIR)\GrandImportSupport.sbr"
	-@erase "$(INTDIR)\MyDateTime.obj"
	-@erase "$(INTDIR)\MyDateTime.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VistaBaseData.obj"
	-@erase "$(INTDIR)\VistaBaseData.sbr"
	-@erase "$(INTDIR)\VistaDataFile.obj"
	-@erase "$(INTDIR)\VistaDataFile.sbr"
	-@erase "$(INTDIR)\VistaDb.obj"
	-@erase "$(INTDIR)\VistaDb.sbr"
	-@erase "$(OUTDIR)\CoGrandImport.bsc"
	-@erase "$(OUTDIR)\CoGrandImport.exp"
	-@erase "$(OUTDIR)\CoGrandImport.lib"
	-@erase "$(OUTDIR)\CoGrandImport.map"
	-@erase "$(OUTDIR)\CoGrandImport.pdb"
	-@erase "..\..\..\irs\comlibs\CoGrandImport.dll"
	-@erase "..\..\..\irs\comlibs\CoGrandImport.ilk"
	-@erase ".\CoGrandImport.h"
	-@erase ".\CoGrandImport.tlb"
	-@erase ".\CoGrandImport_i.c"
	-@erase ".\ReleaseMinDependency\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W2 /GX /Zi /O2 /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "G:\Program Files\MSXML 4.0\inc" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "MINIGRAND" /D "_WINDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoGrandImport.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoGrandImport.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BaseFacCfgWrap.sbr" \
	"$(INTDIR)\CoGrandImport.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\ErrorProcessing.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\GrandData.sbr" \
	"$(INTDIR)\GrandDataFile.sbr" \
	"$(INTDIR)\GrandDbVista.sbr" \
	"$(INTDIR)\GrandFacCfgWrap.sbr" \
	"$(INTDIR)\GrandImport.sbr" \
	"$(INTDIR)\GrandImportSupport.sbr" \
	"$(INTDIR)\MyDateTime.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\VistaBaseData.sbr" \
	"$(INTDIR)\VistaDataFile.sbr" \
	"$(INTDIR)\VistaDb.sbr"

"$(OUTDIR)\CoGrandImport.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=nafxcw.lib imagehlp.lib ../../IRS_DEVCOMMON\Winlib32\gui32.lib ../../IRS_DEVCOMMON\Check_Signature\Check_Signature.lib ../../IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoGrandImport.pdb" /map:"$(INTDIR)\CoGrandImport.map" /debug /machine:I386 /def:".\CoGrandImport.def" /out:"c:/irs/comlibs/CoGrandImport.dll" /implib:"$(OUTDIR)\CoGrandImport.lib" /MAPINFO:EXPORTS /MAPINFO:LINES 
LINK32_OBJS= \
	"$(INTDIR)\BaseFacCfgWrap.obj" \
	"$(INTDIR)\CoGrandImport.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\ErrorProcessing.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\GrandData.obj" \
	"$(INTDIR)\GrandDataFile.obj" \
	"$(INTDIR)\GrandDbVista.obj" \
	"$(INTDIR)\GrandFacCfgWrap.obj" \
	"$(INTDIR)\GrandImport.obj" \
	"$(INTDIR)\GrandImportSupport.obj" \
	"$(INTDIR)\MyDateTime.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VistaBaseData.obj" \
	"$(INTDIR)\VistaDataFile.obj" \
	"$(INTDIR)\VistaDb.obj" \
	"$(INTDIR)\CoGrandImport.res"

"..\..\..\irs\comlibs\CoGrandImport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\CoGrandImport.dll
InputPath=\irs\comlibs\CoGrandImport.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CoGrandImport.dep")
!INCLUDE "CoGrandImport.dep"
!ELSE 
!MESSAGE Warning: cannot find "CoGrandImport.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CoGrandImport - Win32 Debug" || "$(CFG)" == "CoGrandImport - Win32 Release MinDependency"
SOURCE=..\CommonBaseClasses\BaseFacCfgWrap.cpp

"$(INTDIR)\BaseFacCfgWrap.obj"	"$(INTDIR)\BaseFacCfgWrap.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\CoGrandImport.cpp

"$(INTDIR)\CoGrandImport.obj"	"$(INTDIR)\CoGrandImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CoGrandImport.idl

!IF  "$(CFG)" == "CoGrandImport - Win32 Debug"

MTL_SWITCHES=/tlb ".\CoGrandImport.tlb" /h "CoGrandImport.h" /iid "CoGrandImport_i.c" /Oicf 

".\CoGrandImport.tlb"	".\CoGrandImport.h"	".\CoGrandImport_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoGrandImport - Win32 Release MinDependency"

MTL_SWITCHES=/tlb ".\CoGrandImport.tlb" /h "CoGrandImport.h" /iid "CoGrandImport_i.c" /Oicf 

".\CoGrandImport.tlb"	".\CoGrandImport.h"	".\CoGrandImport_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Common\DirUtilities.cpp

"$(INTDIR)\DirUtilities.obj"	"$(INTDIR)\DirUtilities.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\ErrorProcessing.cpp

"$(INTDIR)\ErrorProcessing.obj"	"$(INTDIR)\ErrorProcessing.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\GenDateTime.cpp

"$(INTDIR)\GenDateTime.obj"	"$(INTDIR)\GenDateTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\GrandData.cpp

"$(INTDIR)\GrandData.obj"	"$(INTDIR)\GrandData.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrandDataFile.cpp

"$(INTDIR)\GrandDataFile.obj"	"$(INTDIR)\GrandDataFile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrandDbVista.cpp

"$(INTDIR)\GrandDbVista.obj"	"$(INTDIR)\GrandDbVista.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrandFacCfgWrap.cpp

"$(INTDIR)\GrandFacCfgWrap.obj"	"$(INTDIR)\GrandFacCfgWrap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrandImport.cpp

"$(INTDIR)\GrandImport.obj"	"$(INTDIR)\GrandImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrandImportSupport.cpp

"$(INTDIR)\GrandImportSupport.obj"	"$(INTDIR)\GrandImportSupport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\MyDateTime.cpp

"$(INTDIR)\MyDateTime.obj"	"$(INTDIR)\MyDateTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\ProgressWnd.cpp

"$(INTDIR)\ProgressWnd.obj"	"$(INTDIR)\ProgressWnd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\SafeArrayUtil.cpp

"$(INTDIR)\SafeArrayUtil.obj"	"$(INTDIR)\SafeArrayUtil.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CoGrandImport - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W2 /Gm /GX /ZI /Od /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "G:\Program Files\MSXML 4.0\inc" /D "_DEBUG" /D "_MBCS" /D "MINIGRAND" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoGrandImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoGrandImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoGrandImport - Win32 Release MinDependency"

CPP_SWITCHES=/nologo /MT /W2 /GX /Zi /O2 /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /I "..\..\IRS_DEVCOMMON\zapp3" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Zapp3\ZAPPH" /I "..\..\IRS_DEVCOMMON\Check_Signature" /I "G:\Program Files\MSXML 4.0\inc" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "MINIGRAND" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoGrandImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoGrandImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\CommonImportCom\VistaBaseData.cpp

"$(INTDIR)\VistaBaseData.obj"	"$(INTDIR)\VistaBaseData.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonImportCom\VistaDataFile.cpp

"$(INTDIR)\VistaDataFile.obj"	"$(INTDIR)\VistaDataFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonImportCom\VistaDb.cpp

"$(INTDIR)\VistaDb.obj"	"$(INTDIR)\VistaDb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\CoGrandImport.rc

"$(INTDIR)\CoGrandImport.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

