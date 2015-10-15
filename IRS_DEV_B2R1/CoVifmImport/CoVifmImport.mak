# Microsoft Developer Studio Generated NMAKE File, Based on CoVifmImport.dsp
!IF "$(CFG)" == ""
CFG=CoVifmImport - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CoVifmImport - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CoVifmImport - Win32 Debug" && "$(CFG)" != "CoVifmImport - Win32 Release MinDependency"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CoVifmImport - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\irs\comlibs\CoVifmImport.dll" ".\CoVifmImport.tlb" ".\CoVifmImport.h" ".\CoVifmImport_i.c" "$(OUTDIR)\CoVifmImport.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\CoVifmImport.obj"
	-@erase "$(INTDIR)\CoVifmImport.pch"
	-@erase "$(INTDIR)\CoVifmImport.res"
	-@erase "$(INTDIR)\CoVifmImport.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\FacCfgWrap.obj"
	-@erase "$(INTDIR)\FacCfgWrap.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
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
	-@erase "$(INTDIR)\VifmData.obj"
	-@erase "$(INTDIR)\VifmData.sbr"
	-@erase "$(INTDIR)\VifmDataFile.obj"
	-@erase "$(INTDIR)\VifmDataFile.sbr"
	-@erase "$(INTDIR)\VifmDbVista.obj"
	-@erase "$(INTDIR)\VifmDbVista.sbr"
	-@erase "$(INTDIR)\VifmImport.obj"
	-@erase "$(INTDIR)\VifmImport.sbr"
	-@erase "$(INTDIR)\VifmImportSupport.obj"
	-@erase "$(INTDIR)\VifmImportSupport.sbr"
	-@erase "$(INTDIR)\VistaBaseData.obj"
	-@erase "$(INTDIR)\VistaBaseData.sbr"
	-@erase "$(INTDIR)\VistaDataFile.obj"
	-@erase "$(INTDIR)\VistaDataFile.sbr"
	-@erase "$(INTDIR)\VistaDb.obj"
	-@erase "$(INTDIR)\VistaDb.sbr"
	-@erase "$(OUTDIR)\CoVifmImport.bsc"
	-@erase "$(OUTDIR)\CoVifmImport.exp"
	-@erase "$(OUTDIR)\CoVifmImport.lib"
	-@erase "$(OUTDIR)\CoVifmImport.pdb"
	-@erase "..\..\..\irs\comlibs\CoVifmImport.dll"
	-@erase "..\..\..\irs\comlibs\CoVifmImport.ilk"
	-@erase ".\CoVifmImport.h"
	-@erase ".\CoVifmImport.tlb"
	-@erase ".\CoVifmImport_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonImportCom" /I "..\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Check_Signature" /D "_DEBUG" /D "_MBCS" /D "VIFM" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoVifmImport.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoVifmImport.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoVifmImport.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CoVifmImport.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\FacCfgWrap.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\MyDateTime.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\VifmData.sbr" \
	"$(INTDIR)\VifmDataFile.sbr" \
	"$(INTDIR)\VifmDbVista.sbr" \
	"$(INTDIR)\VifmImport.sbr" \
	"$(INTDIR)\VifmImportSupport.sbr" \
	"$(INTDIR)\VistaBaseData.sbr" \
	"$(INTDIR)\VistaDataFile.sbr" \
	"$(INTDIR)\VistaDb.sbr"

"$(OUTDIR)\CoVifmImport.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib imagehlp.lib ..\..\IRS_DEVCOMMON\Check_Signature\Check_Signature.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoVifmImport.pdb" /debug /machine:I386 /def:".\CoVifmImport.def" /out:"c:/irs/comlibs\CoVifmImport.dll" /implib:"$(OUTDIR)\CoVifmImport.lib" /pdbtype:sept /libpath:"..\..\IRS_DEVCOMMON\rdm45\lib" 
LINK32_OBJS= \
	"$(INTDIR)\CoVifmImport.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\FacCfgWrap.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\MyDateTime.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VifmData.obj" \
	"$(INTDIR)\VifmDataFile.obj" \
	"$(INTDIR)\VifmDbVista.obj" \
	"$(INTDIR)\VifmImport.obj" \
	"$(INTDIR)\VifmImportSupport.obj" \
	"$(INTDIR)\VistaBaseData.obj" \
	"$(INTDIR)\VistaDataFile.obj" \
	"$(INTDIR)\VistaDb.obj" \
	"$(INTDIR)\CoVifmImport.res"

"..\..\..\irs\comlibs\CoVifmImport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=\irs\comlibs\CoVifmImport.dll
InputPath=\irs\comlibs\CoVifmImport.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "CoVifmImport - Win32 Release MinDependency"

OUTDIR=.\ReleaseMinDependency
INTDIR=.\ReleaseMinDependency
# Begin Custom Macros
OutDir=.\ReleaseMinDependency
# End Custom Macros

ALL : "..\..\..\irs\comlibs\CoVifmImport.dll" "$(OUTDIR)\CoVifmImport.bsc" ".\ReleaseMinDependency\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\CoVifmImport.obj"
	-@erase "$(INTDIR)\CoVifmImport.pch"
	-@erase "$(INTDIR)\CoVifmImport.res"
	-@erase "$(INTDIR)\CoVifmImport.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\FacCfgWrap.obj"
	-@erase "$(INTDIR)\FacCfgWrap.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
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
	-@erase "$(INTDIR)\VifmData.obj"
	-@erase "$(INTDIR)\VifmData.sbr"
	-@erase "$(INTDIR)\VifmDataFile.obj"
	-@erase "$(INTDIR)\VifmDataFile.sbr"
	-@erase "$(INTDIR)\VifmDbVista.obj"
	-@erase "$(INTDIR)\VifmDbVista.sbr"
	-@erase "$(INTDIR)\VifmImport.obj"
	-@erase "$(INTDIR)\VifmImport.sbr"
	-@erase "$(INTDIR)\VifmImportSupport.obj"
	-@erase "$(INTDIR)\VifmImportSupport.sbr"
	-@erase "$(INTDIR)\VistaBaseData.obj"
	-@erase "$(INTDIR)\VistaBaseData.sbr"
	-@erase "$(INTDIR)\VistaDataFile.obj"
	-@erase "$(INTDIR)\VistaDataFile.sbr"
	-@erase "$(INTDIR)\VistaDb.obj"
	-@erase "$(INTDIR)\VistaDb.sbr"
	-@erase "$(OUTDIR)\CoVifmImport.bsc"
	-@erase "$(OUTDIR)\CoVifmImport.exp"
	-@erase "$(OUTDIR)\CoVifmImport.lib"
	-@erase "$(OUTDIR)\CoVifmImport.map"
	-@erase "$(OUTDIR)\CoVifmImport.pdb"
	-@erase "..\..\..\irs\comlibs\CoVifmImport.dll"
	-@erase "..\..\..\irs\comlibs\CoVifmImport.ilk"
	-@erase ".\CoVifmImport.h"
	-@erase ".\CoVifmImport.tlb"
	-@erase ".\CoVifmImport_i.c"
	-@erase ".\ReleaseMinDependency\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Zi /O2 /I "..\CommonImportCom" /I "..\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Check_Signature" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "VIFM" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoVifmImport.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoVifmImport.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoVifmImport.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CoVifmImport.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\FacCfgWrap.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\MyDateTime.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\VifmData.sbr" \
	"$(INTDIR)\VifmDataFile.sbr" \
	"$(INTDIR)\VifmDbVista.sbr" \
	"$(INTDIR)\VifmImport.sbr" \
	"$(INTDIR)\VifmImportSupport.sbr" \
	"$(INTDIR)\VistaBaseData.sbr" \
	"$(INTDIR)\VistaDataFile.sbr" \
	"$(INTDIR)\VistaDb.sbr"

"$(OUTDIR)\CoVifmImport.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\IRS_DEVCOMMON\rdm45\lib\vwin\rdm45w32.lib imagehlp.lib ..\..\IRS_DEVCOMMON\Check_Signature\Check_Signature.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoVifmImport.pdb" /map:"$(INTDIR)\CoVifmImport.map" /debug /machine:I386 /def:".\CoVifmImport.def" /out:"c:/irs/comlibs/CoVifmImport.dll" /implib:"$(OUTDIR)\CoVifmImport.lib" /libpath:"..\..\IRS_DEVCOMMON\rdm45\lib" 
DEF_FILE= \
	".\CoVifmImport.def"
LINK32_OBJS= \
	"$(INTDIR)\CoVifmImport.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\FacCfgWrap.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\MyDateTime.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VifmData.obj" \
	"$(INTDIR)\VifmDataFile.obj" \
	"$(INTDIR)\VifmDbVista.obj" \
	"$(INTDIR)\VifmImport.obj" \
	"$(INTDIR)\VifmImportSupport.obj" \
	"$(INTDIR)\VistaBaseData.obj" \
	"$(INTDIR)\VistaDataFile.obj" \
	"$(INTDIR)\VistaDb.obj" \
	"$(INTDIR)\CoVifmImport.res"

"..\..\..\irs\comlibs\CoVifmImport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\CoVifmImport.dll
InputPath=\irs\comlibs\CoVifmImport.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CoVifmImport.dep")
!INCLUDE "CoVifmImport.dep"
!ELSE 
!MESSAGE Warning: cannot find "CoVifmImport.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CoVifmImport - Win32 Debug" || "$(CFG)" == "CoVifmImport - Win32 Release MinDependency"
SOURCE=.\CoVifmImport.cpp

"$(INTDIR)\CoVifmImport.obj"	"$(INTDIR)\CoVifmImport.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=.\CoVifmImport.idl

!IF  "$(CFG)" == "CoVifmImport - Win32 Debug"

MTL_SWITCHES=/tlb ".\CoVifmImport.tlb" /h "CoVifmImport.h" /iid "CoVifmImport_i.c" /Oicf 

".\CoVifmImport.tlb"	".\CoVifmImport.h"	".\CoVifmImport_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoVifmImport - Win32 Release MinDependency"

MTL_SWITCHES=/tlb ".\CoVifmImport.tlb" /h "CoVifmImport.h" /iid "CoVifmImport_i.c" /Oicf 

".\CoVifmImport.tlb"	".\CoVifmImport.h"	".\CoVifmImport_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CoVifmImport.rc

"$(INTDIR)\CoVifmImport.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\Common\DirUtilities.cpp

"$(INTDIR)\DirUtilities.obj"	"$(INTDIR)\DirUtilities.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\FacCfgWrap.cpp

"$(INTDIR)\FacCfgWrap.obj"	"$(INTDIR)\FacCfgWrap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=..\Common\GenDateTime.cpp

"$(INTDIR)\GenDateTime.obj"	"$(INTDIR)\GenDateTime.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\MyDateTime.cpp

"$(INTDIR)\MyDateTime.obj"	"$(INTDIR)\MyDateTime.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\ProgressWnd.cpp

"$(INTDIR)\ProgressWnd.obj"	"$(INTDIR)\ProgressWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\SafeArrayUtil.cpp

"$(INTDIR)\SafeArrayUtil.obj"	"$(INTDIR)\SafeArrayUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CoVifmImport - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonImportCom" /I "..\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Check_Signature" /D "_DEBUG" /D "_MBCS" /D "VIFM" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoVifmImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoVifmImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoVifmImport - Win32 Release MinDependency"

CPP_SWITCHES=/nologo /MT /W3 /GX /Zi /O2 /I "..\CommonImportCom" /I "..\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON\rdm45\include" /I "..\..\IRS_DEVCOMMON\Check_Signature" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "VIFM" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoVifmImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoVifmImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VifmData.cpp

"$(INTDIR)\VifmData.obj"	"$(INTDIR)\VifmData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=.\VifmDataFile.cpp

"$(INTDIR)\VifmDataFile.obj"	"$(INTDIR)\VifmDataFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=.\VifmDbVista.cpp

"$(INTDIR)\VifmDbVista.obj"	"$(INTDIR)\VifmDbVista.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=.\VifmImport.cpp

"$(INTDIR)\VifmImport.obj"	"$(INTDIR)\VifmImport.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=.\VifmImportSupport.cpp

"$(INTDIR)\VifmImportSupport.obj"	"$(INTDIR)\VifmImportSupport.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"


SOURCE=..\CommonImportCom\VistaBaseData.cpp

"$(INTDIR)\VistaBaseData.obj"	"$(INTDIR)\VistaBaseData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonImportCom\VistaDataFile.cpp

"$(INTDIR)\VistaDataFile.obj"	"$(INTDIR)\VistaDataFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonImportCom\VistaDb.cpp

"$(INTDIR)\VistaDb.obj"	"$(INTDIR)\VistaDb.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoVifmImport.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

