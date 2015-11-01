# Microsoft Developer Studio Generated NMAKE File, Based on CoLinearTimeAlign.dsp
!IF "$(CFG)" == ""
CFG=CoLinearTimeAlign - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CoLinearTimeAlign - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CoLinearTimeAlign - Win32 Debug" && "$(CFG)" != "CoLinearTimeAlign - Win32 Release MinDependency"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\irs\comlibs\CoLinearTimeAlign.dll" ".\CoLinearTimeAlign.tlb" ".\CoLinearTimeAlign.h" ".\CoLinearTimeAlign_i.c" "$(OUTDIR)\CoLinearTimeAlign.bsc" ".\Debug\regsvr32.trg"

!ELSE 

ALL : "CoVifmImport - Win32 Debug" "CoGrandImport - Win32 Debug" "CoBinaryImport - Win32 Debug" "..\..\..\irs\comlibs\CoLinearTimeAlign.dll" ".\CoLinearTimeAlign.tlb" ".\CoLinearTimeAlign.h" ".\CoLinearTimeAlign_i.c" "$(OUTDIR)\CoLinearTimeAlign.bsc" ".\Debug\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"CoBinaryImport - Win32 DebugCLEAN" "CoGrandImport - Win32 DebugCLEAN" "CoVifmImport - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CoLinearTimeAlign.obj"
	-@erase "$(INTDIR)\CoLinearTimeAlign.pch"
	-@erase "$(INTDIR)\CoLinearTimeAlign.res"
	-@erase "$(INTDIR)\CoLinearTimeAlign.sbr"
	-@erase "$(INTDIR)\DbIni.obj"
	-@erase "$(INTDIR)\DbIni.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TimeAdjust.obj"
	-@erase "$(INTDIR)\TimeAdjust.sbr"
	-@erase "$(INTDIR)\TimeAlign.obj"
	-@erase "$(INTDIR)\TimeAlign.sbr"
	-@erase "$(INTDIR)\TimeAlignIni.obj"
	-@erase "$(INTDIR)\TimeAlignIni.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.bsc"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.exp"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.lib"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.pdb"
	-@erase "..\..\..\irs\comlibs\CoLinearTimeAlign.dll"
	-@erase "..\..\..\irs\comlibs\CoLinearTimeAlign.ilk"
	-@erase ".\CoLinearTimeAlign.h"
	-@erase ".\CoLinearTimeAlign.tlb"
	-@erase ".\CoLinearTimeAlign_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\commonimportcom" /I "..\..\IRS_DEVCOMMON\rdm45\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoLinearTimeAlign.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoLinearTimeAlign.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoLinearTimeAlign.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CoLinearTimeAlign.sbr" \
	"$(INTDIR)\DbIni.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TimeAdjust.sbr" \
	"$(INTDIR)\TimeAlign.sbr" \
	"$(INTDIR)\TimeAlignIni.sbr"

"$(OUTDIR)\CoLinearTimeAlign.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imagehlp.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoLinearTimeAlign.pdb" /debug /machine:I386 /def:".\CoLinearTimeAlign.def" /out:"c:/irs/comlibs\CoLinearTimeAlign.dll" /implib:"$(OUTDIR)\CoLinearTimeAlign.lib" /pdbtype:sept 
DEF_FILE= \
	".\CoLinearTimeAlign.def"
LINK32_OBJS= \
	"$(INTDIR)\CoLinearTimeAlign.obj" \
	"$(INTDIR)\DbIni.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimeAdjust.obj" \
	"$(INTDIR)\TimeAlign.obj" \
	"$(INTDIR)\TimeAlignIni.obj" \
	"$(INTDIR)\CoLinearTimeAlign.res" \
	"..\CoBinaryImport\Debug\CoBinaryImport.lib" \
	"..\CoGrandImport\Debug\CoGrandImport.lib" \
	"..\CoVifmImport\Debug\CoVifmImport.lib"

"..\..\..\irs\comlibs\CoLinearTimeAlign.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=\irs\comlibs\CoLinearTimeAlign.dll
InputPath=\irs\comlibs\CoLinearTimeAlign.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

OUTDIR=.\ReleaseMinDependency
INTDIR=.\ReleaseMinDependency
# Begin Custom Macros
OutDir=.\ReleaseMinDependency
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\irs\comlibs\CoLinearTimeAlign.dll" "$(OUTDIR)\CoLinearTimeAlign.bsc" ".\ReleaseMinDependency\regsvr32.trg"

!ELSE 

ALL : "CoVifmImport - Win32 Release MinDependency" "CoGrandImport - Win32 Release MinDependency" "CoBinaryImport - Win32 Release MinDependency" "..\..\..\irs\comlibs\CoLinearTimeAlign.dll" "$(OUTDIR)\CoLinearTimeAlign.bsc" ".\ReleaseMinDependency\regsvr32.trg"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"CoBinaryImport - Win32 Release MinDependencyCLEAN" "CoGrandImport - Win32 Release MinDependencyCLEAN" "CoVifmImport - Win32 Release MinDependencyCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CoLinearTimeAlign.obj"
	-@erase "$(INTDIR)\CoLinearTimeAlign.pch"
	-@erase "$(INTDIR)\CoLinearTimeAlign.res"
	-@erase "$(INTDIR)\CoLinearTimeAlign.sbr"
	-@erase "$(INTDIR)\DbIni.obj"
	-@erase "$(INTDIR)\DbIni.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\GenDateTime.obj"
	-@erase "$(INTDIR)\GenDateTime.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TimeAdjust.obj"
	-@erase "$(INTDIR)\TimeAdjust.sbr"
	-@erase "$(INTDIR)\TimeAlign.obj"
	-@erase "$(INTDIR)\TimeAlign.sbr"
	-@erase "$(INTDIR)\TimeAlignIni.obj"
	-@erase "$(INTDIR)\TimeAlignIni.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.bsc"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.exp"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.lib"
	-@erase "$(OUTDIR)\CoLinearTimeAlign.pdb"
	-@erase "..\..\..\irs\comlibs\CoLinearTimeAlign.dll"
	-@erase "..\..\..\irs\comlibs\CoLinearTimeAlign.ilk"
	-@erase ".\CoLinearTimeAlign.h"
	-@erase ".\CoLinearTimeAlign.tlb"
	-@erase ".\CoLinearTimeAlign_i.c"
	-@erase ".\ReleaseMinDependency\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Zi /O2 /I "../common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\commonimportcom" /I "..\..\IRS_DEVCOMMON\rdm45\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoLinearTimeAlign.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoLinearTimeAlign.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoLinearTimeAlign.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CoLinearTimeAlign.sbr" \
	"$(INTDIR)\DbIni.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\GenDateTime.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TimeAdjust.sbr" \
	"$(INTDIR)\TimeAlign.sbr" \
	"$(INTDIR)\TimeAlignIni.sbr"

"$(OUTDIR)\CoLinearTimeAlign.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imagehlp.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoLinearTimeAlign.pdb" /debug /machine:I386 /def:".\CoLinearTimeAlign.def" /out:"c:/irs/comlibs/CoLinearTimeAlign.dll" /implib:"$(OUTDIR)\CoLinearTimeAlign.lib" 
DEF_FILE= \
	".\CoLinearTimeAlign.def"
LINK32_OBJS= \
	"$(INTDIR)\CoLinearTimeAlign.obj" \
	"$(INTDIR)\DbIni.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\GenDateTime.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimeAdjust.obj" \
	"$(INTDIR)\TimeAlign.obj" \
	"$(INTDIR)\TimeAlignIni.obj" \
	"$(INTDIR)\CoLinearTimeAlign.res" \
	"..\CoBinaryImport\ReleaseMinDependency\CoBinaryImport.lib" \
	"..\CoGrandImport\ReleaseMinDependency\CoGrandImport.lib" \
	"..\CoVifmImport\ReleaseMinDependency\CoVifmImport.lib"

"..\..\..\irs\comlibs\CoLinearTimeAlign.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\CoLinearTimeAlign.dll
InputPath=\irs\comlibs\CoLinearTimeAlign.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CoLinearTimeAlign.dep")
!INCLUDE "CoLinearTimeAlign.dep"
!ELSE 
!MESSAGE Warning: cannot find "CoLinearTimeAlign.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CoLinearTimeAlign - Win32 Debug" || "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"
SOURCE=.\CoLinearTimeAlign.cpp

"$(INTDIR)\CoLinearTimeAlign.obj"	"$(INTDIR)\CoLinearTimeAlign.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"


SOURCE=.\CoLinearTimeAlign.idl

!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

MTL_SWITCHES=/tlb ".\CoLinearTimeAlign.tlb" /h "CoLinearTimeAlign.h" /iid "CoLinearTimeAlign_i.c" /Oicf 

".\CoLinearTimeAlign.tlb"	".\CoLinearTimeAlign.h"	".\CoLinearTimeAlign_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

MTL_SWITCHES=/tlb ".\CoLinearTimeAlign.tlb" /h "CoLinearTimeAlign.h" /iid "CoLinearTimeAlign_i.c" /Oicf 

".\CoLinearTimeAlign.tlb"	".\CoLinearTimeAlign.h"	".\CoLinearTimeAlign_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CoLinearTimeAlign.rc

"$(INTDIR)\CoLinearTimeAlign.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DbIni.cpp

"$(INTDIR)\DbIni.obj"	"$(INTDIR)\DbIni.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"


SOURCE=..\Common\DirUtilities.cpp

"$(INTDIR)\DirUtilities.obj"	"$(INTDIR)\DirUtilities.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\GenDateTime.cpp

"$(INTDIR)\GenDateTime.obj"	"$(INTDIR)\GenDateTime.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\ProgressWnd.cpp

"$(INTDIR)\ProgressWnd.obj"	"$(INTDIR)\ProgressWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\SafeArrayUtil.cpp

"$(INTDIR)\SafeArrayUtil.obj"	"$(INTDIR)\SafeArrayUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\commonimportcom" /I "..\..\IRS_DEVCOMMON\rdm45\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoLinearTimeAlign.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoLinearTimeAlign.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

CPP_SWITCHES=/nologo /MT /W3 /GX /Zi /O2 /I "../common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\commonimportcom" /I "..\..\IRS_DEVCOMMON\rdm45\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoLinearTimeAlign.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoLinearTimeAlign.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TimeAdjust.cpp

"$(INTDIR)\TimeAdjust.obj"	"$(INTDIR)\TimeAdjust.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"


SOURCE=.\TimeAlign.cpp

"$(INTDIR)\TimeAlign.obj"	"$(INTDIR)\TimeAlign.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"


SOURCE=.\TimeAlignIni.cpp

"$(INTDIR)\TimeAlignIni.obj"	"$(INTDIR)\TimeAlignIni.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CoLinearTimeAlign.pch"


!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

"CoBinaryImport - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoBinaryImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoBinaryImport.mak CFG="CoBinaryImport - Win32 Debug" 
   cd "..\CoLinearTimeAlign"

"CoBinaryImport - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoBinaryImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoBinaryImport.mak CFG="CoBinaryImport - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CoLinearTimeAlign"

!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

"CoBinaryImport - Win32 Release MinDependency" : 
   cd "\projects\IRS_DEV_B2R1\CoBinaryImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoBinaryImport.mak CFG="CoBinaryImport - Win32 Release MinDependency" 
   cd "..\CoLinearTimeAlign"

"CoBinaryImport - Win32 Release MinDependencyCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoBinaryImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoBinaryImport.mak CFG="CoBinaryImport - Win32 Release MinDependency" RECURSE=1 CLEAN 
   cd "..\CoLinearTimeAlign"

!ENDIF 

!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

"CoGrandImport - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoGrandImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoGrandImport.mak CFG="CoGrandImport - Win32 Debug" 
   cd "..\CoLinearTimeAlign"

"CoGrandImport - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoGrandImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoGrandImport.mak CFG="CoGrandImport - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CoLinearTimeAlign"

!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

"CoGrandImport - Win32 Release MinDependency" : 
   cd "\projects\IRS_DEV_B2R1\CoGrandImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoGrandImport.mak CFG="CoGrandImport - Win32 Release MinDependency" 
   cd "..\CoLinearTimeAlign"

"CoGrandImport - Win32 Release MinDependencyCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoGrandImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoGrandImport.mak CFG="CoGrandImport - Win32 Release MinDependency" RECURSE=1 CLEAN 
   cd "..\CoLinearTimeAlign"

!ENDIF 

!IF  "$(CFG)" == "CoLinearTimeAlign - Win32 Debug"

"CoVifmImport - Win32 Debug" : 
   cd "\projects\IRS_DEV_B2R1\CoVifmImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoVifmImport.mak CFG="CoVifmImport - Win32 Debug" 
   cd "..\CoLinearTimeAlign"

"CoVifmImport - Win32 DebugCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoVifmImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoVifmImport.mak CFG="CoVifmImport - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CoLinearTimeAlign"

!ELSEIF  "$(CFG)" == "CoLinearTimeAlign - Win32 Release MinDependency"

"CoVifmImport - Win32 Release MinDependency" : 
   cd "\projects\IRS_DEV_B2R1\CoVifmImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoVifmImport.mak CFG="CoVifmImport - Win32 Release MinDependency" 
   cd "..\CoLinearTimeAlign"

"CoVifmImport - Win32 Release MinDependencyCLEAN" : 
   cd "\projects\IRS_DEV_B2R1\CoVifmImport"
   $(MAKE) /$(MAKEFLAGS) /F .\CoVifmImport.mak CFG="CoVifmImport - Win32 Release MinDependency" RECURSE=1 CLEAN 
   cd "..\CoLinearTimeAlign"

!ENDIF 


!ENDIF 

