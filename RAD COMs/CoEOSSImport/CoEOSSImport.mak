# Microsoft Developer Studio Generated NMAKE File, Based on CoEOSSImport.dsp
!IF "$(CFG)" == ""
CFG=CoEOSSImport - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CoEOSSImport - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CoEOSSImport - Win32 Debug" && "$(CFG)" != "CoEOSSImport - Win32 Release MinDependency"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CoEOSSImport - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\irs\comlibs\coeossimport.dll" ".\CoEOSSImport.tlb" ".\CoEOSSImport.h" ".\CoEOSSImport_i.c" "$(OUTDIR)\CoEOSSImport.pch" "$(OUTDIR)\CoEOSSImport.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AccessDataFile.obj"
	-@erase "$(INTDIR)\AccessDataFile.sbr"
	-@erase "$(INTDIR)\AccessDb.obj"
	-@erase "$(INTDIR)\AccessDb.sbr"
	-@erase "$(INTDIR)\AccessEOSSDb.obj"
	-@erase "$(INTDIR)\AccessEOSSDb.sbr"
	-@erase "$(INTDIR)\AdoDb.obj"
	-@erase "$(INTDIR)\AdoDb.sbr"
	-@erase "$(INTDIR)\CoEOSSImport.obj"
	-@erase "$(INTDIR)\CoEOSSImport.pch"
	-@erase "$(INTDIR)\CoEOSSImport.res"
	-@erase "$(INTDIR)\CoEOSSImport.sbr"
	-@erase "$(INTDIR)\CreateAccessDatabase.obj"
	-@erase "$(INTDIR)\CreateAccessDatabase.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\EOSSCreateAccessDatabase.obj"
	-@erase "$(INTDIR)\EOSSCreateAccessDatabase.sbr"
	-@erase "$(INTDIR)\EOSSData.obj"
	-@erase "$(INTDIR)\EOSSData.sbr"
	-@erase "$(INTDIR)\EOSSDataFile.obj"
	-@erase "$(INTDIR)\EOSSDataFile.sbr"
	-@erase "$(INTDIR)\EOSSImport.obj"
	-@erase "$(INTDIR)\EOSSImport.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CoEOSSImport.bsc"
	-@erase "$(OUTDIR)\coeossimport.exp"
	-@erase "$(OUTDIR)\coeossimport.lib"
	-@erase "$(OUTDIR)\CoEOSSImport.map"
	-@erase "$(OUTDIR)\CoEOSSImport.pdb"
	-@erase "..\..\..\irs\comlibs\coeossimport.dll"
	-@erase "..\..\..\irs\comlibs\coeossimport.ilk"
	-@erase ".\CoEOSSImport.h"
	-@erase ".\CoEOSSImport.tlb"
	-@erase ".\CoEOSSImport_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W2 /Gm /GX /ZI /Od /I "..\CommonImportCom" /I "c:\projects\RAD_b2R1\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON" /D "_DEBUG" /D "_MBCS" /D "EOSS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoEOSSImport.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoEOSSImport.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AccessDataFile.sbr" \
	"$(INTDIR)\AccessDb.sbr" \
	"$(INTDIR)\AccessEOSSDb.sbr" \
	"$(INTDIR)\AdoDb.sbr" \
	"$(INTDIR)\CoEOSSImport.sbr" \
	"$(INTDIR)\CreateAccessDatabase.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\EOSSCreateAccessDatabase.sbr" \
	"$(INTDIR)\EOSSData.sbr" \
	"$(INTDIR)\EOSSDataFile.sbr" \
	"$(INTDIR)\EOSSImport.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\CoEOSSImport.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imagehlp.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoEOSSImport.pdb" /map:"$(INTDIR)\CoEOSSImport.map" /debug /machine:I386 /def:".\CoEOSSImport.def" /out:"c:/irs/comlibs/coeossimport.dll" /implib:"$(OUTDIR)\coeossimport.lib" /MAPINFO:EXPORTS /MAPINFO:LINES 
LINK32_OBJS= \
	"$(INTDIR)\AccessDataFile.obj" \
	"$(INTDIR)\AccessDb.obj" \
	"$(INTDIR)\AccessEOSSDb.obj" \
	"$(INTDIR)\AdoDb.obj" \
	"$(INTDIR)\CoEOSSImport.obj" \
	"$(INTDIR)\CreateAccessDatabase.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\EOSSCreateAccessDatabase.obj" \
	"$(INTDIR)\EOSSData.obj" \
	"$(INTDIR)\EOSSDataFile.obj" \
	"$(INTDIR)\EOSSImport.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CoEOSSImport.res"

"..\..\..\irs\comlibs\coeossimport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=\irs\comlibs\coeossimport.dll
InputPath=\irs\comlibs\coeossimport.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "CoEOSSImport - Win32 Release MinDependency"

OUTDIR=.\ReleaseMinDependency
INTDIR=.\ReleaseMinDependency
# Begin Custom Macros
OutDir=.\ReleaseMinDependency
# End Custom Macros

ALL : "..\..\..\irs\comlibs\coeossimport.dll" "$(OUTDIR)\CoEOSSImport.pch" "$(OUTDIR)\CoEOSSImport.bsc" ".\ReleaseMinDependency\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AccessDataFile.obj"
	-@erase "$(INTDIR)\AccessDataFile.sbr"
	-@erase "$(INTDIR)\AccessDb.obj"
	-@erase "$(INTDIR)\AccessDb.sbr"
	-@erase "$(INTDIR)\AccessEOSSDb.obj"
	-@erase "$(INTDIR)\AccessEOSSDb.sbr"
	-@erase "$(INTDIR)\AdoDb.obj"
	-@erase "$(INTDIR)\AdoDb.sbr"
	-@erase "$(INTDIR)\CoEOSSImport.obj"
	-@erase "$(INTDIR)\CoEOSSImport.pch"
	-@erase "$(INTDIR)\CoEOSSImport.res"
	-@erase "$(INTDIR)\CoEOSSImport.sbr"
	-@erase "$(INTDIR)\CreateAccessDatabase.obj"
	-@erase "$(INTDIR)\CreateAccessDatabase.sbr"
	-@erase "$(INTDIR)\DirUtilities.obj"
	-@erase "$(INTDIR)\DirUtilities.sbr"
	-@erase "$(INTDIR)\EOSSCreateAccessDatabase.obj"
	-@erase "$(INTDIR)\EOSSCreateAccessDatabase.sbr"
	-@erase "$(INTDIR)\EOSSData.obj"
	-@erase "$(INTDIR)\EOSSData.sbr"
	-@erase "$(INTDIR)\EOSSDataFile.obj"
	-@erase "$(INTDIR)\EOSSDataFile.sbr"
	-@erase "$(INTDIR)\EOSSImport.obj"
	-@erase "$(INTDIR)\EOSSImport.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\SafeArrayUtil.obj"
	-@erase "$(INTDIR)\SafeArrayUtil.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\CoEOSSImport.bsc"
	-@erase "$(OUTDIR)\coeossimport.exp"
	-@erase "$(OUTDIR)\coeossimport.lib"
	-@erase "$(OUTDIR)\CoEOSSImport.pdb"
	-@erase "..\..\..\irs\comlibs\coeossimport.dll"
	-@erase "..\..\..\irs\comlibs\coeossimport.ilk"
	-@erase ".\CoEOSSImport.h"
	-@erase ".\CoEOSSImport.tlb"
	-@erase ".\CoEOSSImport_i.c"
	-@erase ".\Debug\CoEOSSImport.map"
	-@erase ".\ReleaseMinDependency\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W2 /GX /Od /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "EOSS" /D "_WINDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CoEOSSImport.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CoEOSSImport.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AccessDataFile.sbr" \
	"$(INTDIR)\AccessDb.sbr" \
	"$(INTDIR)\AccessEOSSDb.sbr" \
	"$(INTDIR)\AdoDb.sbr" \
	"$(INTDIR)\CoEOSSImport.sbr" \
	"$(INTDIR)\CreateAccessDatabase.sbr" \
	"$(INTDIR)\DirUtilities.sbr" \
	"$(INTDIR)\EOSSCreateAccessDatabase.sbr" \
	"$(INTDIR)\EOSSData.sbr" \
	"$(INTDIR)\EOSSDataFile.sbr" \
	"$(INTDIR)\EOSSImport.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\SafeArrayUtil.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\CoEOSSImport.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imagehlp.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CoEOSSImport.pdb" /map:"./Debug/CoEOSSImport.map" /debug /machine:I386 /def:".\CoEOSSImport.def" /out:"c:/irs/comlibs/coeossimport.dll" /implib:"$(OUTDIR)\coeossimport.lib" /MAPINFO:EXPORTS /MAPINFO:LINES 
LINK32_OBJS= \
	"$(INTDIR)\AccessDataFile.obj" \
	"$(INTDIR)\AccessDb.obj" \
	"$(INTDIR)\AccessEOSSDb.obj" \
	"$(INTDIR)\AdoDb.obj" \
	"$(INTDIR)\CoEOSSImport.obj" \
	"$(INTDIR)\CreateAccessDatabase.obj" \
	"$(INTDIR)\DirUtilities.obj" \
	"$(INTDIR)\EOSSCreateAccessDatabase.obj" \
	"$(INTDIR)\EOSSData.obj" \
	"$(INTDIR)\EOSSDataFile.obj" \
	"$(INTDIR)\EOSSImport.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\SafeArrayUtil.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CoEOSSImport.res"

"..\..\..\irs\comlibs\coeossimport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinDependency
TargetPath=\irs\comlibs\coeossimport.dll
InputPath=\irs\comlibs\coeossimport.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CoEOSSImport.dep")
!INCLUDE "CoEOSSImport.dep"
!ELSE 
!MESSAGE Warning: cannot find "CoEOSSImport.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CoEOSSImport - Win32 Debug" || "$(CFG)" == "CoEOSSImport - Win32 Release MinDependency"
SOURCE=..\CommonImportCom\AccessDataFile.cpp

"$(INTDIR)\AccessDataFile.obj"	"$(INTDIR)\AccessDataFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonImportCom\AccessDb.cpp

"$(INTDIR)\AccessDb.obj"	"$(INTDIR)\AccessDb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\AccessEOSSDb.cpp

"$(INTDIR)\AccessEOSSDb.obj"	"$(INTDIR)\AccessEOSSDb.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\CommonImportCom\AdoDb.cpp

"$(INTDIR)\AdoDb.obj"	"$(INTDIR)\AdoDb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\CoEOSSImport.cpp

"$(INTDIR)\CoEOSSImport.obj"	"$(INTDIR)\CoEOSSImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CoEOSSImport.idl

!IF  "$(CFG)" == "CoEOSSImport - Win32 Debug"

MTL_SWITCHES=/tlb ".\CoEOSSImport.tlb" /h "CoEOSSImport.h" /iid "CoEOSSImport_i.c" /Oicf 

".\CoEOSSImport.tlb"	".\CoEOSSImport.h"	".\CoEOSSImport_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoEOSSImport - Win32 Release MinDependency"

MTL_SWITCHES=/tlb ".\CoEOSSImport.tlb" /h "CoEOSSImport.h" /iid "CoEOSSImport_i.c" /Oicf 

".\CoEOSSImport.tlb"	".\CoEOSSImport.h"	".\CoEOSSImport_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\CommonImportCom\CreateAccessDatabase.cpp

"$(INTDIR)\CreateAccessDatabase.obj"	"$(INTDIR)\CreateAccessDatabase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\DirUtilities.cpp

"$(INTDIR)\DirUtilities.obj"	"$(INTDIR)\DirUtilities.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\EOSSCreateAccessDatabase.cpp

"$(INTDIR)\EOSSCreateAccessDatabase.obj"	"$(INTDIR)\EOSSCreateAccessDatabase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EOSSData.cpp

"$(INTDIR)\EOSSData.obj"	"$(INTDIR)\EOSSData.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EOSSDataFile.cpp

"$(INTDIR)\EOSSDataFile.obj"	"$(INTDIR)\EOSSDataFile.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EOSSImport.cpp

"$(INTDIR)\EOSSImport.obj"	"$(INTDIR)\EOSSImport.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Common\ProgressWnd.cpp

"$(INTDIR)\ProgressWnd.obj"	"$(INTDIR)\ProgressWnd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Common\SafeArrayUtil.cpp

"$(INTDIR)\SafeArrayUtil.obj"	"$(INTDIR)\SafeArrayUtil.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CoEOSSImport - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W2 /Gm /GX /ZI /Od /I "..\CommonImportCom" /I "c:\projects\RAD_b2R1\Common" /I "..\..\IRS_DEVCOMMON\Winclude32" /I "..\..\IRS_DEVCOMMON" /D "_DEBUG" /D "_MBCS" /D "EOSS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoEOSSImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoEOSSImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CoEOSSImport - Win32 Release MinDependency"

CPP_SWITCHES=/nologo /MT /W2 /GX /Od /I "..\CommonBaseClasses" /I "..\Common" /I "..\CommonImportCom" /I "..\..\IRS_DEVCOMMON\winclude32" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "EOSS" /D "_WINDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\CoEOSSImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\CoEOSSImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CoEOSSImport.rc

"$(INTDIR)\CoEOSSImport.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

