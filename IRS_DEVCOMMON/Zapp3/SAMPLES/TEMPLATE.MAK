ROGUE=D:\Rogue\workspaces\WINNT4\MSVC50\0s
RWCOMPVERSION=44C
!ifndef COMPILER
COMPILER=vcnt
!endif

!ifndef RWSHARED
RWSHARED=0
!endif
!ifndef RWMULTI
RWMULTI=0
!endif
!ifndef RWDEBUG
RWDEBUG=0
!endif
!ifndef RWSYMBDEBUG
RWSYMBDEBUG=0
!endif
!ifndef RWSTDLIB
RWSTDLIB=0
!endif

# Determine proper RW Defines from SPM build flags
!if $(RWSHARED)
ZRWSHARED_FLAGS=-D_RWBUILDSHARED
ZAPPDLL=1
!endif
!if $(RWSYMBDEBUG)
#ZRWSYMBDEBUG_FLAGS=-g
DEBUG=1
#ZAPPDEBUG=1

!endif
!if $(RWDEBUG)
ZRWDEBUG_FLAGS=-DRWDEBUG=1
#DEBUG=1
ZAPPDEBUG=1

!endif
!if $(RWMULTI)
ZRWMULTI_FLAGS=-DRW_MULTI_THREAD -D_REENTRANT 
THRNAME=mt
!endif

!if "$(RWSTDLIB)"=="1"
SLDEF=-DRW_RWV12_STDLIB=1
!if $(RWSHARED)
SLDEF =$(SLDEF) -D_RWSTDDLL
!endif
!endif


RWFLAGS = $(ZRWSHARED_FLAGS) $(ZRWSYMBDEBUG_FLAGS) $(ZRWDEBUG_FLAGS) $(ZRWMULTI_FLAGS) $(SLDEF) -DNOMINMAX $(RWCCOPTS)

# Determine build type from SPM build flags
!if !$(RWSTDLIB)
!if !$(RWMULTI)
!if !$(RWDEBUG)
!if !$(RWSYMBDEBUG) 
BUILDTYPE=0
!else
BUILDTYPE=1
!endif
!else
!if !$(RWSYMBDEBUG) 
BUILDTYPE=2
!else
BUILDTYPE=3
!endif
!endif
!else
!if !$(RWDEBUG)
!if !$(RWSYMBDEBUG) 
BUILDTYPE=4
!else
BUILDTYPE=5
!endif
!else
!if !$(RWSYMBDEBUG) 
BUILDTYPE=6
!else
BUILDTYPE=7
!endif
!endif
!endif
!else
!if !$(RWMULTI)
!if !$(RWDEBUG)
!if !$(RWSYMBDEBUG) 
BUILDTYPE=8
!else
BUILDTYPE=9
!endif
!else
!if !$(RWSYMBDEBUG) 
BUILDTYPE=10
!else
BUILDTYPE=11
!endif
!endif
!else
!if !$(RWDEBUG)
!if !$(RWSYMBDEBUG) 
BUILDTYPE=12
!else
BUILDTYPE=13
!endif
!else
!if !$(RWSYMBDEBUG) 
BUILDTYPE=14
!else
BUILDTYPE=15
!endif
!endif
!endif
!endif



!if "$(RWSTDLIB)"=="1"
STDLIB=Y
STDLIB_INCLUDE=$(ROGUE)\include
STDLIB_LIB=$(ROGUE)\lib
!endif



!if $(RWSHARED)
BUILDTYPE=$(BUILDTYPE)d$(RWUSEREXT)
!else
BUILDTYPE=$(BUILDTYPE)s$(RWUSEREXT)
!endif
ZAPPDIR=$(ROGUE)

!if "$(RWSTDLIB)"=="1"
SL=$(STDLIB_LIB)\std$(BUILDTYPE).lib
!endif


RWLINKS = $(RWLINKOPTS)


###############################################
#
# Assign DBASEON=1 to turn on Data Pack and DBTools.
#
#
# assign a value to one or more of these in makefile that includes template.mak:
#
# ZDP_Sybase_DB_Static
# ZDP_Sybase_CT_Static
# ZDP_Oracle_7_3_Static
# ZDP_Oracle_7_2_Static
# ZDP_Oracle_7_1_Static
# ZDP_ODBC_Static
# ZDP_Ingres_Static
# ZDP_Infx_7_1_Static
# ZDP_Infx_7_2_Static
# ZDP_Microsoft_DB_Static
#
#
# EXAMPLE:
# ZDP_Sybase_DB_Static=1
# ZDP_Sybase_CT_Static=2
# !include "..\template.mak"
#
###############################################

#!ifndef DATAPACK
#DATAPACK = m:\merdos\zapp_dev\zdp
#!endif

!ifndef DBASEON
DBASEON=0
!endif

#!ifndef ZAPPDIR
#ZAPPDIR = m:\merdos\zapp_dev
#!endif

#!ifndef COMPILER
#COMPILER = vcnt
#!endif


#!ifndef ROGUE
#ROGUE = e:\rogue
#!endif

RWLIB = $(ROGUE)\lib

#****************************************************
#Everything above this line can be set by the user


LIBMESPER=Unknown Persistence Option
LIBMESDEB=Unknownd ZAPPDEBUG Option
LIBMESCOMP=Unknown Compiler
LIBMESLIB=Unknown Library Type
LIBMESCTL3D=Unknown CTL3D Option

!ifndef CTRL3D
CTRL3D=0
!endif

!ifndef CTL3D
CTL3D=0
!endif

!if "$(DBASEON)"=="1"
INTPACK=1
!endif


!if "$(CTRL3D)"=="1"
CTL3D=1
!endif

!if $(CTL3D)
C3D=-D__CTL3D__
LIBMESCTL3D=, CTL3D Activated
!if "$(COMPILER)"=="bc5"
C3DL=CTL3D32.LIB
!else
!if "$(COMPILER)"=="bc45"
C3DL=CTL3D32.LIB+
!else
C3DL=CTL3D32.LIB
!endif
!endif
!else
C3D=
C3DL=
LIBMESCTL3D=, CTL3D NOT Active
!endif

!if "$(RWSTDLIB)"=="1"
STDLIB=Y
STDLIB_INCLUDE=$(ROGUE)\include
STDLIB_LIB=$(ROGUE)\lib
!endif


!ifdef DATAPACK
DPHEAD=-I$(DATAPACK)
!endif


!if "$(STDLIB)"=="Y"
!ifdef STDLIB_INCLUDE
INC=-I$(STDLIB_INCLUDE) -I$(ROGUE) $(DPHEAD) $(RWFLAGS)
!else
INC=-I$(ROGUE) $(DPHEAD) $(RWFLAGS)
!endif
USESTL=1
!else
INC=-I$(ROGUE) $(DPHEAD) -DRW_NO_STL=1  $(RWFLAGS)
USESTL=0
!endif

!ifndef ZUSETEMP
ZUSETEMP = 1
!endif

!ifndef PERSIST
PERSIST=0
!endif

!ifndef ZAPPDEBUG
ZAPPDEBUG=0
!endif

!ifndef INTPACK
INTPACK=0
!endif


!ifndef DEBUG
DEBUG=0
!endif

!ifndef OPTIMIZE
OPTIMIZE = 1
!endif

!ifndef ZAPPDLL
ZAPPDLL=0
BINDING=STATIC
!else
BINDING=DLL
!endif

!if "$(ZAPPDLL)"=="0"
BINDING=STATIC
!endif

!if "$(ZAPPDLL)"=="1"
BINDING=DLL
!endif


!ifndef MAKEDLL
MAKEDLL=0
!endif

!if $(MAKEDLL)
ZAPPDLL=1
!if "$(COMPILER)"=="vcnt"
ZDLLOPTS=-DZ_CLIDLL /LD /MD
!else
ZDLLOPTS=-DZ_CLIDLL
!endif
LIBMESOUTTYPE=DLL
!else
ZDLLOPTS=
LIBMESOUTTYPE=application
!endif

!ifdef Z_NATIVE_NBD
COMCTLLIB=comctl32.lib
COPT=$(COPT) -DZ_NATIVE_NBD=1
!else
COMCTLLIB=
!endif


!ifndef PRECOMP
PRECOMP = 0
!endif

!ifndef OFILES
OFILES=$(NAME).obj
!endif

!ifndef LFILES
LFILES = $(NAME).obj
!endif

!ifndef NTLFILES
NTLFILES=$(OFILES)
!endif

DDELIB =
DB=

!ifdef HELPSRC
INCHELP=1
!endif

!ifdef NATIVEHELPSRC
INCHELP=1
!endif

!ifdef INCHELP
DDE=1
HDB=

!if $(PERSIST)
!if $(ZAPPDEBUG)
HDB=x
!else
HDB=p
!endif
!endif

!if $(ZAPPDEBUG)
!if $(PERSIST)
HDB=x
!else
HDB=d
!endif
!endif


!if $(ZAPPDLL)
HDLL=d
!else
HDLL=s
!endif

!if "$(COMPILER)"=="bc45"
HCOMP=32b9
!endif
!if "$(COMPILER)"=="vcnt"
HCOMP=32vc
!endif
!if "$(COMPILER)"=="bc5"
HCOMP=32b5
!endif


HLIBNAME=zhp$(BUILDTYPE).lib

!if "$(COMPILER)"=="vcnt"
!ifdef ADDLIB
ADDLIB=$(ZAPPDIR)\lib\$(HLIBNAME) $(ADDLIB)
!else
ADDLIB=$(ZAPPDIR)\lib\$(HLIBNAME)
!endif
!else
ADDLIB=$(ZAPPDIR)\lib\$(HLIBNAME) $(ADDLIB)
!endif

!if "$(COMPILER)"=="bc45"
EXENAME=bc45exe
!endif
!if "$(COMPILER)"=="vcnt"
EXENAME=vcnt
!endif
!if "$(COMPILER)"=="bc5"
EXENAME=bc5exe
!endif

!ifdef HELPSRC
!ifdef NATIVEHELPSRC
thehelp: thehelp1 thehelp2
!else
thehelp: thehelp1
!endif
!else
!ifdef NATIVEHELPSRC
thehelp: thehelp2
!endif
!endif

!ifdef HELPSRC

thehelp1: $(EXENAME) $(NAME).zhp

$(NAME).zhp: $(HELPSRC)
	$(ZAPPDIR)\bin\zhc $(HELPOPT) $(HELPSRC)

!endif

!ifdef NATIVEHELPSRC

!if "$(COMPILER)"=="vcnt"
HCNAME=hcrtf /xn
!else
HCNAME=hc31
!endif


thehelp2: $(EXENAME) $(NAME).hlp

$(NAME).hlp: $(NATIVEHELPSRC)
	$(HCNAME) $(NATIVEHELPOPT)  $(NATIVEHELPSRC)

!endif
!endif

!ifndef DDE
DDE=0
!endif
!ifndef COMMDLG
COMMDLG=0
!endif

!if $(PERSIST)
LIBMESPER=, Persistence On
!if $(ZAPPDEBUG)
DB=x
!else
DB=p
PFLAGS=
!endif
!else
PFLAGS=-DZ_NP=1
LIBMESPER=, Persistence Off
!endif

!if $(ZAPPDEBUG)
LIBMESDEB=, ZAPPDEBUG On
DBGFLAGS=-DZ_DB=1
!if $(PERSIST)
DB=x
!else
DB=d
!endif
!else
LIBMESDEB=, ZAPPDEBUG Off
!endif

!ifndef RES
!if $(MAKEDLL)
RES = $(NAME).dll
RESEXE = $(NAME).dll
!else
RES = $(NAME).exe
RESRES =
!if "$(COMPILER)"=="bc5"
RESEXE=
!else
!if "$(COMPILER)"=="bc45"
RESEXE=
!else
RESEXE = $(NAME).exe
!endif
!endif
!endif
BORRES=
!else
RESRES = $(NAME).res
!if $(MAKEDLL)
RESEXE = $(NAME).dll
!else
RESEXE = $(NAME).res
!endif
BORRES = BRCC32 -w32 -r -I$(ZAPPDIR)\zapph
!endif

!if $(ZAPPDLL)
LIBMESLIB=, DLL Linkage
!else
LIBMESLIB=, Static Linkage
!endif


!if "$(COMPILER)"=="vcnt"
!include <ntwin32.mak>

LIBMESCOMP=Microsoft C++

CC = cl
NTLINKER=LINK

!if "$(DBASEON)"=="1"
!if $(ZAPPDLL)
DPLIB = $(ZAPPDIR)\LIB\zdp$(BUILDTYPE).LIB
!else
DPLIB = $(ZAPPDIR)\LIB\zdp$(BUILDTYPE).LIB
!endif
!endif


!if $(INTPACK)
!if $(ZAPPDLL)
INTPACKLIB = $(ZAPPDIR)\LIB\zip$(BUILDTYPE).LIB
!else
INTPACKLIB = $(ZAPPDIR)\lib\zip$(BUILDTYPE).lib
!endif
!else
INTPACKLIB =
!endif


!if $(PRECOMP)
POPT = -YX
!else
POPT =
!endif

!if $(DEBUG)
DFLAGS=-Zi
!if $(MAKEDLL)
LFLAGS=-debug:full /SUBSYSTEM:windows -DLL /MAP
!else
LFLAGS=-debug:full
!endif
OPTIMIZE=0
!else
DFLAGS=
!if $(MAKEDLL)
LFLAGS=-debug:none /SUBSYSTEM:windows -DLL /MAP
!else
LFLAGS=-debug:none
!endif
!endif



# compare compversion to Microsoft 10.10 and 10.00
!if "$(RWCOMPVERSION)"!="3F2" && "$(RWCOMPVERSION)"!="3E8"


!if "$(DEBUG)"=="1"
libc = libcd.lib oldnames.lib
libcmt = libcmtd.lib oldnames.lib
libcdll = msvcrtd.lib oldnames.lib
MDEB = d
!endif

# The next section choosed the correct iostream library for
# Microsoft's tools to use.  By default, the library for the
# old iostream is used.  To use the new standard library iostream,
# define MSVCSTDLIB=1.

!if "$(MSVCSTDLIB)"=="1"
IO=p
!else
IO=i
!endif

!if "$(MSVCSTDLIB)"!="1"
!if $(ZAPPDLL)
MSIOLIB=msvc$(IO)rt$(MDEB).lib
!else
MSIOLIB=libc$(IO)$(THRNAME)$(MDEB).LIB
!endif
!endif

#endif for version check
!endif

!if $(ZAPPDLL)
LNKOPTS=$(ALIB1) $(ROGUE)\lib\tls$(BUILDTYPE).lib $(SL) $(MSIOLIB) $(olelibsdll) shell32.lib $(LNKOPT) $(LFLAGS)
!else
LNKOPTS=$(ZAPPDIR)\lib\zap$(BUILDTYPE).lib $(ALIB1) $(olelibs) $(ROGUE)\lib\tls$(BUILDTYPE).lib $(SL) $(MSIOLIB) $(guilibs) shell32.lib $(LNKOPT) $(LFLAGS)
!endif


!if $(ZAPPDLL)
!if $(MAKEDLL)
DLLOPT = -DZ_NTUSEDLL $(cflags) -Gs $(ZDLLOPTS)
DLLFLAGS = $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib

!else
DLLOPT = -DZ_NTUSEDLL
DLLFLAGS = $(ZAPPDIR)\lib\zap$(BUILDTYPE).obj $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib
!endif
!else
DLLOPT=
DLLFLAGS=
!endif


!ifdef Z_NATIVE_NBD
DLLFLAGS=$(DLLFLAGS) comctl32.lib
!endif





!if "$(OPTIMIZE)"=="1"
!if $(ZAPPDLL)
CPPFLAGS= -Gd -GX -W3 -D_X86_=1 $(COPTS) $(cvarsdll) -D_RWTOOLSDLL -Ox $(C3D) $(DFLAGS) $(POPT) $(COPT) $(CDEF) $(DLLOPT) -DOEMRESOURCE
!else
CPPFLAGS= -Gd -GX -W3 -D_X86_=1 $(COPTS) $(cvars) -Ox $(C3D) $(DFLAGS) $(POPT) $(COPT) $(CDEF) $(DLLOPT)
!endif
!else
!if $(ZAPPDLL)
CPPFLAGS= -Od -Gd -GX -W3 -D_X86_=1 $(COPTS) $(cvarsdll) -D_RWTOOLSDLL $(C3D) $(DFLAGS) $(POPT) $(COPT) $(CDEF) $(DLLOPT) -DOEMRESOURCE
!else
CPPFLAGS= -Od -Gd -GX -W3 -D_X86_=1 $(COPTS) $(cvars) $(C3D) $(DFLAGS) $(POPT) $(COPT) $(CDEF) $(DLLOPT)
!endif
!endif

!if $(DDE)
DDELIB =
!endif

!ifndef ADDLIB
ALIB1 =
!else
ALIB1 = $(ADDLIB)
!endif

!ifndef LIBFILES
ALIB2 =
!else
ALIB2 = $(LIBFILES)
!endif

!if $(MAKEDLL)
vcnt: message $(NAME).dll
!else
vcnt: message $(NAME).exe
!endif

!endif


!if "$(COMPILER)"=="bc45"

LIBMESCOMP=Borland C++ 4.5
CC = bcc32
!if $(INTPACK)
!if $(ZAPPDLL)
INTPACKLIB = $(ZAPPDIR)\lib\zip$(BUILDTYPE).lib
!else
INTPACKLIB = $(ZAPPDIR)\lib\zip$(BUILDTYPE).lib
!endif
!else
INTPACKLIB =
!endif


!if $(ZAPPDLL)
DLLOPT = -DZ_NTUSEDLL -D_RWTOOLSDLL -D_RTLDLL
DLLLIB = $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib
DLLOBJ = $(ZAPPDIR)\lib\zap$(BUILDTYPE).obj 
!else
DLLOPT=
DLLLIB=
DLLOBJ=
!endif



!if $(PRECOMP)
POPT = -H=$(ZAPPDIR)\zappbc45.sym
!else
POPT =
!endif

!if $(DEBUG)
DFLAGS=-v
LFLAGS=/v
OPTIMIZE=0
!else
DFLAGS=
!endif

!if $(MAKEDLL)
LFLAGS=$(LFLAGS) /Tpd /m
DLLOPT=$(DLLOPT) -tWD $(ZDLLOPTS)
!endif

!if $(RWMULTI)
!if $(ZAPPDLL)
BORCLIB=CW32MTI.LIB
!else
BORCLIB=CW32MT.LIB
!endif
!else
!if $(ZAPPDLL)
BORCLIB=CW32I.LIB
!else
BORCLIB=CW32.LIB
!endif
!endif



!if $(RWMULTI)
MFLAG = -WM
!else
MFLAG = -WM-
!endif



!if "$(OPTIMIZE)"=="1"
CPPFLAGS=-V -3 -Ox -d -X $(C3D) $(COPT) $(DFLAGS) $(CDEF) $(POPT) $(DLLOPT) $(MFLAG)
!else
CPPFLAGS=-V -3 -Od -d -X $(C3D) $(COPT) $(DFLAGS) $(CDEF) $(POPT) $(DLLOPT) $(MFLAG)
!endif


!if $(COMMDLG)
!if "$(RCOPT)"=="-30"
CDLGLIB =
!else
CDLGLIB =
!endif
!else
CDLGLIB =
!endif

!ifndef ADDLIB
ALIB1 =
!else
ALIB1 = $(ADDLIB) 
!endif

!ifndef LIBFILES
ALIB2 =
!else
ALIB2 = $(LIBFILES)+
!endif

bc45: message bc45exe

!endif


!if "$(COMPILER)"=="bc5"

LIBMESCOMP=Borland C++ 5.0

CC = bcc32

!if $(ILINK)
LINKER = ilink32
LIBMESLINK = , Incremental linker
!else
LINKER = tlink32
!endif


!if $(INTPACK)
!if $(ZAPPDLL)
INTPACKLIB = $(ZAPPDIR)\lib\zip$(BUILDTYPE).lib
!else
INTPACKLIB = $(ZAPPDIR)\lib\zip$(BUILDTYPE).lib
!endif
!else
INTPACKLIB =
!endif


!if $(ZAPPDLL)
DLLOPT = -DZ_NTUSEDLL -D_RWTOOLSDLL -D_RTLDLL
DLLLIB = $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib
DLLOBJ = $(ZAPPDIR)\lib\zap$(BUILDTYPE).obj 
!else
DLLOPT=
DLLLIB=
DLLOBJ=
!endif

!if "$(DBASEON)"=="1"
!if $(ZAPPDLL)
DPLIB = $(ZAPPDIR)\LIB\zdp$(BUILDTYPE).lib
!else
DPLIB = $(ZAPPDIR)\LIB\zdp$(BUILDTYPE).lib
!endif
!endif


!if $(PRECOMP)
POPT = -H=$(ZAPPDIR)\zapp5.sym
!else
POPT =
!endif

!if $(DEBUG)
DFLAGS=-v
LFLAGS=/v
OPTIMIZE=0
!else
DFLAGS=
!endif

!if $(CODEGUARD)
DFLAGS=$(DFLAGS) -vG
CGLIB=cg32.lib
!endif

!if $(MAKEDLL)
LFLAGS=$(LFLAGS) /Tpd /m
DLLOPT=$(DLLOPT) -tWD $(ZDLLOPTS)
!endif


!if $(RWMULTI)
!if $(ZAPPDLL)
BORCLIB=CW32MTI.LIB
!else
BORCLIB=CW32MT.LIB
!endif
!else
!if $(ZAPPDLL)
BORCLIB=CW32I.LIB
!else
BORCLIB=CW32.LIB
!endif
!endif



!if $(RWMULTI)
MFLAG = -WM
!else
MFLAG = -WM-
!endif



!if "$(RWSTDLIB)"=="1"
BORSTDINC=-I$(ROGUE)\include\bc5
!endif

!if "$(OPTIMIZE)"=="1"
CPPFLAGS=-V -3 -Ox -d -X $(C3D) $(BORSTDINC) $(COPT) $(DFLAGS) $(CDEF) $(POPT) $(DLLOPT) $(MFLAG) -DOEMRESOURCE
!else
CPPFLAGS=-V -3 -Od -d -X $(C3D) $(BORSTDINC) $(COPT) $(DFLAGS) $(CDEF) $(POPT) $(DLLOPT) $(MFLAG) -DOEMRESOURCE
!endif


!if $(COMMDLG)
!if "$(RCOPT)"=="-30"
CDLGLIB =
!else
CDLGLIB =
!endif
!else
CDLGLIB =
!endif

!ifndef ADDLIB
ALIB1 =
!else
ALIB1 = $(ADDLIB) 
!endif

!ifndef LIBFILES
ALIB2 =
!else
ALIB2 = $(LIBFILES) 
!endif

bc5: message bc5exe

!endif











!if "$(DBASEON)"=="1"

!if "$(COMPILER)"=="bc5"

#
#    Borland specific directives
#

#mer.SWAP
#mer.AUTODEPEND



###################################################################
#
#    Must do's:
#
#    1. Choose BINDING; DLL or STATIC
#    2. Choose ACCESS; see below
#
#    Things to check:
#
#    THREAD:        Uncomment if using multi-thread libraries; NT
#    DBG:           Uncomment if using debug libraries
#    CPPOPTIONS:    Modify if other options are necessary
#
####################################################################
#   Uncomment one or the other depending on wether you're linking
#   to DBtools.h++ built as a DLL or as a STATIC library.
#BINDING= DLL
#BINDING= STATIC



#
#   Uncomment one of the following Access Libraries
ACCESS=        SYBASE_DB_ACCESS
#ACCESS=        SYBASE_CT_ACCESS
#ACCESS=        ORACLE_ACCESS
#ACCESS=        ODBC_ACCESS
#ACCESS=        INGRES_ACCESS




#
#   Uncomment the following line if you are using the multi-thread
#   version of Rogue Wave libraries.  This only applies to Windows
#   NT applications.
#THREAD = MULTI

#
#   Uncomment the following line if you are using the self-debugging
#   versions of Rogue Wave libraries.  This assumes you are using
#   ALL or NO self-debugging libraries
#DBG=   g



####################################################################
# Check the MUST choices
####################################################################
!ifndef BINDING
!  error Must choose BINDING; either DLL or STATIC
!endif

!if $(BINDING) != DLL && $(BINDING) != STATIC
!  error Illegal value for BINDING option, must be DLL or STATIC
!endif



!ifndef ACCESS
!  error Must choose ACCESS library option
!endif



####################################################################
# Set ENVIRON
#

!ifndef ENVIRON
!  if $d(__OS2__)
ENVIRON = OS2
!  endif
#!  if $(OS) == Windows_NT
ENVIRON = WIN32
#!  endif
!endif



#
# Choose ENVIRON based on operating system
#
#ENVIRON=WIN16          # Windows 3.1
#ENVIRON=WIN32          # Windows 95

!ifndef ENVIRON
! error Must set ENVIRON in this makefile; WIN16 or WIN32
!endif

!if $(ENVIRON) != WIN32 && $(ENVIRON) != WIN16
!  error Illegal value for ENVIRON option, must be WIN16 or WIN32
!endif



####################################################################
#
#   Search path for Rogue Wave files
#
RWINCDIR = ..
RWLIBDIR = $(ROGUE)\lib

#
#   Rogue Wave defines to link DLLs
#
!if $(BINDING) == DLL
DEFINES = -D_RWDBDLL -D_RWTOOLSDLL -D_RTLDLL
!  if $(STDLIB) == Y
BINDDEF= $(BINDDEF) -D_RWSTDDLL
!  endif
!endif

#Defines about using Standard Library.
!ifndef STDLIB
STDLIB=N
!endif

!if $(STDLIB) != Y 
DEFINES = $(DEFINES) -DRW_NO_STL=1
!else

# Tools and Stdlib WINNAMES are inconsistent, hence SLWINNAME:
!if $(ENVIRON) == WIN32
!  if $(BINDING) == DLL
SLWINNAME = d
!  else
SLWINNAME = w
!  endif
!endif

!  ifndef STDLIB_INCLUDE
STDLIB_INCLUDE_DIR=
!    else
STDLIB_INCLUDE_DIR= -I$(STDLIB_INCLUDE)
!  endif


!  ifndef STDLIB_LIB
STDLIBNAME= 
!    else
STDLIBNAME= std$(BUILTYPE).lib
STDLIBNAME= $(STDLIB_LIB)\$(STDLIBNAME)
!  endif
!endif




#Defines on using Threads.
!ifndef THREAD
THREAD = SINGLE
!endif

!if $(ENVIRON) == WIN32
!  if $(THREAD) == MULTI
THRNAME = mt
MTOPT = -WM
!  endif
!else
MEMOPT=l
!endif

!if $(ENVIRON) == WIN32
!  if $(BINDING) == DLL
WINNAME=d
!  else
WINNAME=
!  endif
!else
!  if $(BINDING) == DLL
WINNAME=d
!  else
WINNAME=w
!  endif
!endif



#
# Compiler flags
# --------------
#       -v        for source code debugging
#       -O1       to optimize for space
#       -O2       to optimize for speed
#       -2        to generate 80286 code
#       -ml       to generate large model code
#       -WS       to generate smart call backs for DLLs
#       -WC       to generate a 32-bit console mode application (winnt)
#       -WM       to make a multi-threaded application
#

#
#   Set compiler flags from the above list here:
#
!if $(ENVIRON) == WIN32
CPPOPTS = -WC $(MTOPT) $(DEFINES)
DBTCPPFLAGS = $(MTOPT) $(DEFINES)
!else
CPPOPTS = -m$(MEMOPT) -WS $(DEFINES)
DBTCPPFLAGS = $(DEFINES)
!endif




#
#   Set to your Borland root directory, e.g. "c:\bc45".
#
BCCDIR = C:\DEV\BORLAND\BC5

!if $(ENVIRON) == WIN32
SUFFIX = 32
!else
SUFFIX =
!endif

#
#   C++ compiler
#
CPP = bcc$(SUFFIX)

#
#   Linker
#
LINKER = tlink$(SUFFIX)

#
# Linker flags
# ------------
#       -Twe  build a Windows .exe file
#       -Tpe  build a protected mode 32-bit .exe file (winnt)
#       -v    include full symbolic debug info in .exe file
#       -ap   target Windows-compatible character mode applications
#

#
#   Set linker flags from the above list here:
#
!if $(ENVIRON) == WIN32
LINKFLAGS = -L$(BCCDIR)\lib;$(RWLIBDIR) -Tpe -v -ap
!else
LINKFLAGS = -L$(BCCDIR)\lib;$(RWLIBDIR) -Twe -v
!endif





##################################################################
#   Access Libraries
#

#
#   Use ODBC access library
#

!if "$(ZDP_ODBC_Static)"=="1"
# Set ODBC to directory of the installation 
! ifndef ODBC
#ODBC=
! endif
ODBCVENLIB=$(ODBC)\blib\odbc$(SUFFIX).lib
ODBCRWLIB=$(RWLIB)\odb$(BUILDTYPE).lib 
ODBCRWOBJ=$(RWLIB)\bdbodlib.obj 
!endif

#
#   Use Oracle access library
#

!if "$(ZDP_Oracle_8_03_Static)"=="1"
# Set ORACLE to directory of the installation 
! ifndef ORACLE
#ORACLE=
! endif
ORA803VENLIB=$(ORACLE)\oci80\lib\borland\ociw32.lib
ORARWOBJ=$(RWLIB)\bdborlib.obj  
ORARWLIB=$(RWLIB)\ora$(BUILDTYPE).lib 
!endif


!if "$(ZDP_Oracle_7_3_Static)"=="1"
# Set ORACLE to directory of the installation 
! ifndef ORACLE
#ORACLE=
! endif
ORA73VENLIB=$(ORACLE)\oci73\lib\borland\ociw32.lib
ORARWOBJ=$(RWLIB)\bdborlib.obj  
ORARWLIB=$(RWLIB)\ora$(BUILDTYPE).lib 
!endif

!if "$(ZDP_Oracle_7_2_Static)"=="1"
# Set ORACLE to directory of the installation 
! ifndef ORACLE
#ORACLE=
! endif
ORA72VENLIB=$(ORACLE)\OCI72\BLIB\OCIW32.LIB
ORARWOBJ=$(RWLIB)\bdborlib.obj  
ORARWLIB=$(RWLIB)\ora$(BUILDTYPE).lib 
!endif


!if "$(ZDP_Oracle_7_1_Static)"=="1"
# Set ORACLE to directory of the installation 
! ifndef ORACLE
#ORACLE=
! endif
ORA71VENLIB=$(ORACLE)\pro16\blib\ora7nt.lib
ORARWOBJ=$(RWLIB)\bdborlib.obj  
ORARWLIB=$(RWLIB)\ora$(BUILDTYPE).lib 
!endif


#
#   Use Sybase access library for DB library
#

!if "$(ZDP_Sybase_DB_Static)"=="1"
# Set SYBASE to directory of the installation
! ifndef SYBASE
#SYBASE=
! endif
SYBDBRWLIB=$(RWLIB)\sdb$(BUILDTYPE).lib 
SYBDBRWOBJ=$(RWLIB)\bdbsdlib.obj 
SDBLIBNAME=$(SYBASE)\blib\libsybdb.lib
!endif




#
#   Use Sybase access library for CT library
#

!if "$(ZDP_Sybase_CT_Static)"=="1"
# Set SYBASE to directory of the installation
! ifndef SYBASE
SYBASE=e:\SYBASE
! endif
SYBCTVENLIB=$(SYBASE)\blib\libcomn.lib $(SYBASE)\blib\libcs.lib $(SYBASE)\blib\libct.lib
SYBCTRWLIB=$(RWLIB)\ctl$(BUILDTYPE).lib
SYBCTRWOBJ=$(RWLIB)\bdbctlib.obj 
!endif




#
#   Use Ingres access library
#

!if "$(ZDP_Ingres_Static)"=="1"
# Set II_SYSTEM to directory of the installation 
! ifndef II_SYSTEM
#II_SYSTEM=
! endif
INGRESVENLIB=$(II_SYSTEM)\ingres\lib\iiw3libq.lib
INGRESRWLIB=$(RWLIB)\ing$(BUILDTYPE).obj
INGRESRWOBJ=$(RWLIB)\bdbinlib.obj
!endif



!if $(BINDING) == DLL
! if $(ENVIRON) == WIN16
MISCOBJ= $(RWLIBDIR)\bdbmgr.obj
! else
MISCOBJ=
! endif
!else
MISCOBJ= $(DBLIBOBJ)
!endif


##################################################################
#       From here on, it's pretty much boilerplate


!if $(BINDING) == DLL
#
# Dynamic Binding
#
!  if $(ENVIRON) == WIN32
BCCLIB  = cw32$(THRNAME)i.lib
BCCOBJ  = $(BCCDIR)\lib\c0x32.obj
DBLIB   = $(ROGUE)\LIB\dbt$(BUILDTYPE).lib
TOOLIB  = btld$(DBG)$(THRNAME).lib
!  else
BCCLIB  = crtldll.lib
BCCOBJ  = $(BCCDIR)\lib\c0wl.obj
DBLIB   = $(ROGUE)\lib\bd1ld.lib+$(ROGUE)\lib\bd2ld.lib+$(ROGUE)\lib\bd3ld.lib
TOOLIB  = btlld.lib
!  endif
!endif

!if $(BINDING) == STATIC
#
# Static Binding
#
!  if $(ENVIRON) == WIN32
BCCLIB  = cw32.lib
BCCOBJ  = $(BCCDIR)\lib\c0x32.obj
DBLIB   = $(ROGUE)\lib\dbt$(BUILDTYPE).lib
TOOLIB  = btl$(DBG)$(THRNAME).lib+$(VENDORLIB)
!  else
BCCLIB  = mathwl.lib+cwl.lib
BCCOBJ  = $(BCCDIR)\lib\c0wl.obj
DBLIB   = $(ROGUE)\lib\bdlw.lib
TOOLIB  = btllw.lib+$(VENDORLIB)
!  endif
!endif

#borland
!endif




!if "$(COMPILER)"=="vcnt"


#
#==========================================================================
#                  
#    Must do's:
#
#    1. Choose BINDING; DLL or STATIC
#    2. Choose ACCESS; see below
#    3. Choose THREAD; SINGLE or MULTI
#
#    Things to check:
#
#    DBG:               Uncomment if using debug libraries
#    CPPOPTIONS:        Modify if other options are necessary
#
####################################################################
#   Uncomment one or the other depending on wether you're linking
#   to DBtools.h++ built as a DLL or as a STATIC library.
#BINDING= DLL
#BINDING= STATIC

#
#   Uncomment one of the following Access Libraries
#ACCESS=        SYBASE_DB_ACCESS
#ACCESS=        SYBASE_CT_ACCESS
#ACCESS=        ORACLE_ACCESS
#ACCESS=        ODBC_ACCESS
#ACCESS=        INGRES_ACCESS
#ACCESS=        INFORMIX_ACCESS

#
#   Uncomment the following line if you are using the debug versions
#   of Rogue Wave libraries.  This assumes you are using ALL debug
#   libraries or NO debug libraries
#DBG=   g

#   Uncomment the following line if you are using the multithreaded 
#   versions of Rogue Wave libraries.  This assumes you are using 
#   ALL multithreaded libraries or NO multithreaded libraries.
#THREAD=   MULTI

####################################################################
# Set ENVIRON
#
!ifndef ENVIRON
#!  if "$(OS)" == "Windows_NT"
ENVIRON=WIN32
#!  endif
!endif

#
# Choose ENVIRON based on operating system
#
#ENVIRON=WIN16          # Windows 3.1
#ENVIRON=WIN32          # Windows 95

!ifndef ENVIRON
! error Must set ENVIRON in this makefile; WIN16 or WIN32
!endif



####################################################################
#   Vendor-supplied libraries & their directories.
####################################################################

#   MicroSoft - DB-Library

!if "$(ZDP_Microsoft_DB_Static)"=="1"
# Set SQLSERVER to directory of the installation
!ifndef SQLSERVER
#SQLSERVER=
!endif
MSDBVENLIB=$(SQLSERVER)\lib\ntwdblib.lib
!endif

#
#   Sybase - DB-Library
#

!if "$(ZDP_Sybase_DB_Static)"=="1"
# Set SYBASE to directory of the installation
!ifndef SYBASE
#SYBASE=
!endif
! if "$(ENVIRON)" == "WIN32"
SDBLIBNAME=   libsybdb.lib
! else
# for DB-Library 10.0.x
SDBLIBNAME=   wsybdb.lib
# for DB-Library 4.2.x
#SDBLIBNAME=  w3dblib.lib
! endif
SDBLIBDIR=  $(SYBASE)\lib
SYBDBVENLIB=$(SDBLIBDIR)\$(SDBLIBNAME)
!endif


#
#   Sybase - CT-Library
#

!if "$(ZDP_Sybase_CT_Static)"=="1"
# Set SYBASE to directory of the installation
! ifndef SYBASE
SYBASE=e:\SYBASE
! endif
! if "$(ENVIRON)" == "WIN32"
SYBCTVENLIB=$(SYBASE)\lib\libct.lib $(SYBASE)\lib\libcs.lib
! endif
!endif


#
#   Oracle
#


!if "$(ZDP_Oracle_8_03_Static)"=="1"
# Set ORACLE to directory of the installation 
!ifndef ORACLE
#ORACLE=
!endif
ORA803VENLIB=$(ORACLE)\oci80\lib\msvc\ociw32.lib
!endif


!if "$(ZDP_Oracle_7_3_Static)"=="1"
# Set ORACLE to directory of the installation 
!ifndef ORACLE
#ORACLE=
!endif
ORA73VENLIB=$(ORACLE)\oci73\lib\msvc\ociw32.lib
!endif

!if "$(ZDP_Oracle_7_2_Static)"=="1"
# Set ORACLE to directory of the installation 
!ifndef ORACLE
#ORACLE=
!endif
ORA72VENLIB=$(ORACLE)\oci72\lib\old\ora72.lib
!endif

!if "$(ZDP_Oracle_7_1_Static)"=="1"
# Set ORACLE to directory of the installation 
!ifndef ORACLE
#ORACLE=
!endif
ORA71VENLIB=$(ORACLE)\pro16\lib\orant71.lib
!endif



#
# IBM DB2
#
!if "$(ZDP_DB2_Static)" == "1"
#DB2PATH=
DB2VENLIB=$(DB2PATH)\lib\db2cli.lib
!endif



#
# ODBC
#

!if "$(ZDP_ODBC_Static)"=="1"
# Set ODBCSDK to directory of the installation 
! ifndef ODBCSDK
#ODBCSDK=
! endif
ODBCVENLIB=$(ODBCSDK)\lib\odbc32.lib
!endif



#   Ingres

!if "$(ZDP_Ingres_Static)"=="1"
# Set II_SYSTEM to directory of the installation 
! ifndef II_SYSTEM
#II_SYSTEM=
! endif
INGRESVENLIB=$(II_SYSTEM)\ingres\lib\iiw3libq.lib
!endif

#   Informix
#


!if "$(ZDP_Infx_7_2_Static)"=="1"
# Set INFORMIXDIR to directory of the installation 
! ifndef INFORMIXDIR
#INFORMIXDIR=
! endif
INFORMIX72VENLIB=$(INFORMIXDIR)\lib\isqlT07c.lib
!endif


!if "$(ZDP_Infx_7_1_Static)"=="1"
# Set INFORMIXDIR to directory of the installation 
! ifndef INFORMIXDIR
#INFORMIXDIR=
! endif
INFORMIXVENLIB=$(INFORMIXDIR)\lib\isqli710.lib
!endif


####################################################################
#   Reconcile differences in library names
####################################################################
#set default values


!ifndef THREAD
!  if "$(BINDING)" == "DLL"
THREAD= MULTI
!  else
THREAD= SINGLE
!  endif
!endif

!if "$(BMODE)" == "DEBUG"
DBG=g
!  else
BMODE=RELEASE
DBG=
!endif

!if "$(BINDING)" == "DLL"
BIND=  d
!else
BINDING= STATIC
BIND=  w
!endif

!if "$(ENVIRON)" == "WIN32"
MODEL=
! if "$(BINDING)" == "STATIC"
BIND=
! endif
!else
MODEL= l
!endif

THRNAME=
!if "$(ENVIRON)" == "WIN32"
! if "$(THREAD)"=="MULTI"
! if "$(BINDING)" == "DLL"
THRNAME= 	  mt
!   else
THRNAME= mt
!   endif
! endif
!endif

!ifndef STDLIB
STDLIB=N
!endif

!if "$(STDLIB)" != "Y"
CCOPTS= $(CCOPTS) -DRW_NO_STL=1
!else

# Tools and Stdlib WINNAMES are inconsistent, hence SLWINNAME:
!if "$(ENVIRON)" == "WIN32"
!  if "$(BINDING)" == "DLL"
SLWINNAME = d
!  else
SLWINNAME = w
!  endif
!endif

!  ifndef STDLIB_INCLUDE
STDLIB_INCLUDE_DIR=
!    else
STDLIB_INCLUDE_DIR= -I$(STDLIB_INCLUDE)
!  endif

!  ifndef STDLIB_LIB
STDLIBNAME= 
!    else
STDLIBNAME= std$(BUILDTYPE).lib
STDLIBNAME= $(STDLIB_LIB)\$(STDLIBNAME)
!  endif
!endif

####################################################################
#   DBtools.h++
####################################################################
#
DBNAME=         dbt$(BUILDTYPE).lib
DBINCL=         ..

####################################################################
#   Tools.h++
####################################################################
# 
TOOLSNAME=      tls$(BUILDTYPE).lib
RWINCL=         ..
RWLIB=          $(ROGUE)

####################################################################
#       Access Libraries
#       Uncomment one section.
####################################################################

!if "$(BINDING)" == "STATIC"

!if "$(ZDP_Microsoft_DB_Static)"=="1"
MSDBRWLIB=$(RWLIB)\lib\msq$(BUILDTYPE).lib
MSDBRWOBJ=$(RWLIB)\lib\mdbmqlib.obj
!endif


!if "$(ZDP_Sybase_DB_Static)"=="1"
SYBDBRWLIB=$(RWLIB)\lib\sdb$(BUILDTYPE).lib
SYBDBRWOBJ=$(RWLIB)\lib\mdbsdlib.obj
!endif

# Access Library for Sybase CT-Library

!if "$(ZDP_Sybase_CT_Static)"=="1"
SYBCTRWLIB=$(RWLIB)\lib\ctl$(BUILDTYPE).lib
SYBCTRWOBJ=$(RWLIB)\lib\mdbctlib.obj
!endif

# Access Library for Oracle OCI library


!if "$(ZDP_Oracle_8_03_Static)"=="1"
ORARWOBJ=$(RWLIB)\lib\mdborlib.obj
ORARWLIB=$(RWLIB)\lib\ora$(BUILDTYPE).lib
!endif


!if "$(ZDP_Oracle_7_3_Static)"=="1"
ORARWOBJ=$(RWLIB)\lib\mdborlib.obj
ORARWLIB=$(RWLIB)\lib\ora$(BUILDTYPE).lib
!endif

!if "$(ZDP_Oracle_7_2_Static)"=="1"
ORARWOBJ=$(RWLIB)\lib\mdborlib.obj
ORARWLIB=$(RWLIB)\lib\ora$(BUILDTYPE).lib
!endif

!if "$(ZDP_Oracle_7_1_Static)"=="1"
ORARWOBJ=$(RWLIB)\lib\mdborlib.obj
ORARWLIB=$(RWLIB)\lib\ora$(BUILDTYPE).lib
!endif

# Access Library for DB2

!if "$(ZDP_DB2_Static)"=="1"
DB2RWOBJ=$(RWLIB)\lib\mdbd2lib.obj
DB2RWLIB=$(RWLIB)\lib\dbw$(BUILDTYPE).lib
!endif

# Access Library for ODBC library

!if "$(ZDP_ODBC_Static)"=="1"
ODBCRWOBJ=$(RWLIB)\lib\mdbodlib.obj
ODBCRWLIB=$(RWLIB)\lib\odb$(BUILDTYPE).lib
!endif

#       Access Library for Ingres

!if "$(ZDP_Ingres_Static)"=="1"
INGRESRWOBJ=$(RWLIB)\lib\mdbinlib.obj
INGRESRWLIB=$(RWLIB)\lib\mdin$(MODEL)$(BIND)$(DBG)$(THRNAME).lib
!endif

#       Access Library for Informix

!if "$(ZDP_Infx_7_2_Static)"=="1"
INFORMIXRWOBJ=$(RWLIB)\lib\mdbiflib.obj
INFORMIXRWLIB=$(RWLIB)\lib\inf$(BUILDTYPE).lib
!endif


#       Access Library for Informix

!if "$(ZDP_Infx_7_1_Static)"=="1"
INFORMIXRWOBJ=$(RWLIB)\lib\mdbiflib.obj
INFORMIXRWLIB=$(RWLIB)\lib\inf$(BUILDTYPE).LIB
!endif

#MER
DBACCESSNAME = $(ROGUE)\LIB\$(DBACCESSNAME)

!else
# Dynamic loading of access library
DBACCESSOBJ=    
DBACCESSNAME=
VENDORNAME=
!endif

####################################################################
#   Set and uncomment the following if the standard environment
#   variables have not been set; INCLUDE and LIB.
####################################################################
#MSVCDIR =      c:\msvc15
#MSVCINCL=      $(MSVCDIR)\include
#MSVCLIB =      $(MSVCDIR)\lib

####################################################################
#   Prepend to the LIB environment variable.
####################################################################
LIB=            ..\lib;$(VENDORLIBDIR);$(MSVCLIB);$(ROGUE)\lib;$(LIB) 

##################################################################
#
#   From here on, it's pretty much boilerplate.
#
##################################################################
#   Options to be passed to your C++ compiler:
!if "$(ENVIRON)" == "WIN32"
OSOPTS=        -GX -DWIN32
CPPOPTIONS= $(CPPOPTIONS) $(CCOPTS)
SEGOPT=
SUBSYS=        -SUBSYSTEM:console
!else
OSOPTS=        /DWIN16
CPPOPTIONS=   $(CPPOPTIONS) /AL $(CCOPTS)
SEGOPT=        /SEG:256
!endif
#Choose the correct runtime library
DEBUGLIB=
!ifdef MSDEBUG
DBTCPPFLAGS=$(DBTCPPFLAGS) -Zi
DEBUGLIB=d
!endif

#    Linking directives
LINKFLAGS= /nologo $(SUBSYS) /NOD $(SEGOPT)
# GOOD MER LINKLIBS = $(DBACCESSNAME) $(TOOLSNAME) $(DBNAME) $(STDLIBNAME) $(VENDORNAME) 
!if "$(DBASEON)"=="1"
LINKLIBS = $(TOOLSNAME) $(DBNAME) $(STDLIBNAME) $(SYBDBRWOBJ) $(SYBDBRWLIB) \
		   $(SYBDBVENLIB) $(SYBCTRWOBJ) $(SYBCTRWLIB) $(SYBCTVENLIB) $(ORARWOBJ) \
		   $(ORARWLIB) $(ORA71VENLIB) $(ORA72VENLIB) $(ORA73VENLIB) $(ODBCVENLIB) $(ODBCRWOBJ) \
		   $(ODBCRWLIB) $(INGRESVENLIB) $(INGRESRWOBJ) $(INGRESRWLIB) \
		   $(INFORMIXRWOBJ) $(INFORMIXRWLIB) $(INFORMIXVENLIB) $(DB2VENLIB) $(DB2RWLIB) $(DB2RWOBJ) \
		   $(MSDBRWLIB) $(MSDBRWOBJ) $(MSDBVENLIB) $(ORA73VENLIB)  $(ORA803VENLIB) \
		   $(INFORMIX72VENLIB)

!else
LINKLIBS = $(TOOLSNAME) $(STDLIBNAME)
!endif

#    Additional directives for C++ compiler
!if "$(BINDING)" == "DLL"
BINDDEF=       -D_RWDBDLL -D_RWTOOLSDLL  
!endif

DBTCPPFLAGS = $(CPPOPTIONS) $(DBTCPPFLAGS) -nologo $(OSOPTS) $(BINDDEF) -D_WINDOWS -DNOMINMAX -c 

!if "$(MSVCINCL)" != ""
DBTCPPFLAGS = $(DBTCPPFLAGS) -I$(MSVCINCL)
!endif

!if "$(ENVIRON)" == "WIN32"

!  if "$(BINDING)" == "DLL"
MISCLIBS=      oldnames.lib msvcrt$(DEBUGLIB).lib kernel32.lib user32.lib  
DBTCPPFLAGS= $(DBTCPPFLAGS) -MD$(DEBUGLIB)
!  else
!    if "$(BINDING)" == "STATIC" && "$(THREAD)" == "SINGLE"
MISCLIBS=      oldnames.lib libc$(DEBUGLIB).lib kernel32.lib user32.lib  
DBTCPPFLAGS= $(DBTCPPFLAGS) -ML$(DEBUGLIB)
!    else 
!      if "$(BINDING)" == "STATIC" && "$(THREAD)" == "MULTI"
MISCLIBS=      oldnames.lib libcmt$(DEBUGLIB).lib kernel32.lib user32.lib  
DBTCPPFLAGS= $(DBTCPPFLAGS) -MT$(DEBUGLIB) 
!      endif
!    endif
!  endif

!else
		 
# for Windows 3.1
MISCLIBS=      libw.lib oldnames.lib llibcew.lib
DEFFILE=       tutor.def

!endif


#added by mer:

LNKOPTS=$(LNKOPTS) $(LINKFLAGS) $(LINKLIBS) $(DBACCESSOBJ)


#vcnt
!endif


#dbaseon
!endif






!if "$(COMPILER)"=="vcnt"

.SUFFIXES : .cpp

.cpp.obj:
	$(CC) $(CPPFLAGS) $(DBTCPPFLAGS) $(CCOPTS) $(PFLAGS) $(DBGFLAGS) $(INC) -D__NT__=1 -c $*.cpp

!else
!if $(MAKEDLL)

.SUFFIXES : .cpp

.cpp.obj:
	$(CC) @&&|
$(CPPFLAGS) $(DBTCPPFLAGS) $(PFLAGS) $(CCOPTS) $(DBGFLAGS) $(INC) -D__NT__=1 -c $*.cpp
|

!else

.SUFFIXES : .cpp

.cpp.obj:
	$(CC) @&&|
$(CPPFLAGS) $(DBTCPPFLAGS) $(PFLAGS) $(CCOPTS) $(DBGFLAGS) $(INC) -D__NT__=1 -c $*.cpp
|

!endif
!endif



!ifndef LFLAGS
LFLAGS =
!endif



!if "$(COMPILER)"=="bc5"

!if $(ZAPPDLL)
!if $(USESTL)
!ifdef STDLIB_LIB
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib $(SL)
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib $(SL)
!endif
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib

!endif

!else

!if $(USESTL)
!ifdef STDLIB_LIB
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib $(SL)
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib $(SL)
!endif
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib

!endif
!endif


!if $(ZUSETEMP)
bc5exe: $(OFILES) $(RESRES) $(NAME).def
	echo $(ROGUE)
	$(LINKER) /Tpe -c /aa $(LNKOPT)$(LFLAGS) $(RWLINKS) @&&~
!if $(MAKEDLL)
	c0d32.obj $(MISCOBJ)+
!else
	c0w32.obj $(DLLOBJ) $(MISCOBJ)+
!if "$(DBASEON)"=="1"
!if !$(ZAPPDLL)
	$(SYBCTRWOBJ) $(ODBCRWOBJ) $(ORARWOBJ) $(SYBDBRWOBJ) $(INGRESRWOBJ)+
!endif
!endif
!endif
	$(LFILES)
	$(NAME)
	$(NAME)
!if $(ZAPPDLL)
        $(CGLIB) $(VENDORLIB) $(DBLIB) $(DLLLIB)  $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB)+
        $(DPLIB) $(INTPACKLIB) $(RL) ole2w32.lib import32.lib $(BORCLIB)
!else
!if "$(DBASEON)"=="1"
!ifdef ZDP_Sybase_CT_Static
		$(SYBCTVENLIB) $(SYBCTRWLIB)+
!endif
!ifdef ZDP_ODBC_Static
		$(ODBCVENLIB) $(ODBCRWLIB)+
!endif
!ifdef ZDP_Oracle_8_03_Static
		$(ORA803VENLIB) $(ORARWLIB)+
!endif
!ifdef ZDP_Oracle_7_3_Static
		$(ORA73VENLIB) $(ORARWLIB)+
!endif
!ifdef ZDP_Oracle_7_2_Static
		$(ORA72VENLIB) $(ORARWLIB)+
!endif
!ifdef ZDP_Oracle_7_1_Static
		$(ORA71VENLIB) $(ORARWLIB)+
!endif
!ifdef ZDP_Sybase_DB_Static
		$(SYBDBRWLIB) $(SDBLIBNAME)+
!endif
!ifdef ZDP_Ingres_Static
		$(INGRESVENLIB) $(INGRESRWLIB)+
!endif
#dbaseon
!endif
        $(CGLIB) $(DBLIB)  $(ZAPPDIR)\LIB\zap$(BUILDTYPE).lib  $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB) $(DPLIB) $(INTPACKLIB) $(RL) ole2w32.lib import32.lib $(BORCLIB)
!endif
	$(NAME).def
	$(RESRES)
~
!if $(MAKEDLL)
	implib $(NAME).lib $(NAME).dll
!endif
!else

bc5exe: $(OFILES) makeb5.lnk $(RESRES) $(NAME).def
	$(LINKER) /aa $(LNKOPT)$(LFLAGS) $(RWLINKS) @makeb5.lnk
	del makeb5.lnk
!if $(MAKEDLL)
	implib $(NAME).lib $(NAME).dll
!endif

!endif

$(NAME).res: $(NAME).rc
	$(BORRES) $(NAME).rc

!else


!if "$(COMPILER)"=="bc45"

!if $(ZAPPDLL)
!if $(USESTL)
!ifdef STDLIB_LIB
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib+$(SL)
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib+$(SL)
!endif
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib

!endif

!else
!if $(USESTL)
!ifdef STDLIB_LIB
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib+$(SL)
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib+$(SL)
!endif
!else
RL=$(ROGUE)\lib\tls$(BUILDTYPE).lib

!endif
!endif

!if $(ZUSETEMP)
!if $(MAKEDLL)
bc45exe: $(OFILES) $(RESRES) $(NAME).def
	tlink32 /Tpd -c /aa $(LNKOPT)$(LFLAGS) $(RWLINKS) @&&~
	c0d32.obj+
!else
bc45exe: $(OFILES) $(RESRES) $(NAME).def
	tlink32 /Tpe -c /aa $(LNKOPT)$(LFLAGS) $(RWLINKS) @&&~
	c0w32.obj+$(DLLOBJ)+
!endif
	$(LFILES)
	$(NAME)
	$(NAME)
!if $(ZAPPDLL)
        $(DLLLIB)+ $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB)
        $(INTPACKLIB)+$(RL)+ole2w32.lib+import32.lib+$(BORCLIB)
!else
        $(ZAPPDIR)\LIB\zntb9$(DB).lib+ $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB)
        $(INTPACKLIB)+$(RL)+ole2w32.lib+import32.lib+$(BORCLIB)
!endif
	$(NAME).def
	$(RESRES)
~
!if $(MAKEDLL)
	implib $(NAME).lib $(NAME).dll
!endif
!else

bc45exe: $(OFILES) makeb9.lnk $(RESRES) $(NAME).def
	tlink32 /aa $(LNKOPT)$(LFLAGS) $(RWLINKS) @makeb9.lnk
	del makeb9.lnk
!if $(MAKEDLL)
	implib $(NAME).lib $(NAME).dll
!endif

!endif

$(NAME).res: $(NAME).rc
	$(BORRES) $(NAME).rc


!else

$(NAME).dll: makefile $(OFILES) $(RESRES) $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib $(INTPACKLIB) $(DPLIB)
	$(NTLINKER) -map:$(NAME).map -out:$(NAME).dll $(LNKOPTS) $(RWLINKS) $(ALIB2) $(C3DL) $(CDLGLIB) $(DDELIB) $(INTPACKLIB) $(DPLIB) $(NTLFILES) $(RESRES) $(DLLFLAGS) -implib:$(NAME).lib
	$(ZAPPDIR)\bin\map2def $(NAME).map > $(NAME).def
	$(NTLINKER) -map:$(NAME).map -out:$(NAME).dll $(LNKOPTS) $(RWLINKS) $(ALIB2) $(C3DL) $(CDLGLIB) $(DDELIB) $(INTPACKLIB) $(DPLIB) $(NTLFILES) $(RESRES) $(DLLFLAGS) -implib:$(NAME).lib

!if $(ZAPPDLL)
$(NAME).exe: makefile $(OFILES) $(RESRES) $(NAME).def $(INTPACKLIB)
#$(NAME).exe: makefile $(OFILES) $(RESRES) $(NAME).def $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib $(INTPACKLIB) $(DPLIB)
!else
$(NAME).exe: makefile $(OFILES) $(RESRES) $(NAME).def $(ZAPPDIR)\lib\zap$(BUILDTYPE).lib $(INTPACKLIB) $(DPLIB)
!endif
#	$(NTLINKER) -map:$(NAME).map -out:$(NAME).exe $(LNKOPTS) $(RWLINKS) $(guiflags) $(ALIB2) $(C3DL) $(COMCTLLIB) $(CDLGLIB) $(DDELIB) $(INTPACKLIB) $(DPLIB) $(NTLFILES) $(RESRES) $(DLLFLAGS)
#	$(NTLINKER) -map:$(NAME).map -out:$(NAME).exe $(INTPACKLIB) $(RWLINKS) $(DPLIB) $(NTLFILES) $(RESRES) $(DLLFLAGS) $(LNKOPTS) $(guiflags) oldnames.lib $(ALIB2) $(C3DL) $(COMCTLLIB) $(CDLGLIB) $(DDELIB) $(RWLIB)\lib\mdbodlib.obj $(RWLIB)\lib\mdod$(MODEL)$(BIND)$(DBG)$(THRNAME).lib
	$(NTLINKER) -map:$(NAME).map -out:$(NAME).exe $(INTPACKLIB) $(DPLIB) $(NTLFILES) $(RESRES) $(DLLFLAGS) $(LNKOPTS) $(RWLINKS) $(guiflags) oldnames.lib $(ALIB2) $(C3DL) $(COMCTLLIB) $(CDLGLIB) $(DDELIB)



$(NAME).res: $(RES)
    rc -I$(ZAPPDIR)\zapph -r $(RES)


!endif
!endif

clean:
	del *.obj
	del *.res
	del $(NAME).exe
	del *.map
	del *.lnk
	del *.log
	del *.zhp
	del *.hlp
	del *.pdb
	del $(NAME).dll
	del $(NAME).lib
	del *.pch
	del $(ZAPPDIR)\*.sym
	del *.sym
	del lastmake.txt

run:
	$(NAME)


makeb5.lnk: $(ZAPPDIR)\samples\template.mak makefile
!if $(MAKEDLL)
	echo c0d32.obj $(MISCOBJ)+ > makeb5.lnk
!else
	echo c0w32.obj $(DLLOBJ) $(MISCOBJ)+ > makeb5.lnk
!endif
!if "$(DBASEON)"=="1"
!if !$(ZAPPDLL)
	echo 	$(SYBCTRWOBJ) $(ODBCRWOBJ) $(ORARWOBJ) $(SYBDBRWOBJ) $(INGRESRWOBJ)+ >>makeb5.lnk
!endif
!endif
	echo $(LFILES) >> makeb5.lnk
	echo $(NAME) >>makeb5.lnk
	echo $(NAME) >> makeb5.lnk
!if $(ZAPPDLL)
        echo $(CGLIB) $(DLLLIB)+ >>makeb5.lnk
        echo $(INTPACKLIB) $(RL)+ >>makeb5.lnk
!else
        echo $(CGLIB) $(ZAPPDIR)\LIB\zap$(BUILDTYPE).lib+ >>makeb5.lnk
        echo $(INTPACKLIB) $(RL)+ >>makeb5.lnk
!endif
!if "$(DBASEON)"=="1"
	echo $(DBLIB)+ >>makeb5.lnk
!ifdef ZDP_Sybase_CT_Static
		echo $(SYBCTVENLIB) $(SYBCTRWLIB)+ >>makeb5.lnk
!endif
!ifdef ZDP_ODBC_Static
		echo $(ODBCVENLIB) $(ODBCRWLIB)+ >>makeb5.lnk
!endif
!ifdef ZDP_Oracle_8_03_Static
		echo $(ORA803VENLIB) $(ORARWLIB)+ >>makeb5.lnk
!endif
!ifdef ZDP_Oracle_7_3_Static
		echo $(ORA73VENLIB) $(ORARWLIB)+ >>makeb5.lnk
!endif
!ifdef ZDP_Oracle_7_2_Static
		echo $(ORA72VENLIB) $(ORARWLIB)+ >>makeb5.lnk
!endif
!ifdef ZDP_Oracle_7_1_Static
		echo $(ORA71VENLIB) $(ORARWLIB)+ >>makeb5.lnk
!endif
!ifdef ZDP_Sybase_DB_Static
		echo $(SYBDBRWLIB) $(SDBLIBNAME)+ >> makeb5.lnk
!endif
!ifdef ZDP_Ingres_Static
		echo $(INGRESVENLIB) $(INGRESRWLIB)+ >>makeb5.lnk
!endif
		echo $(DPLIB)+ >>makeb5.lnk
#dbaseon
!endif
!if $(ZAPPDLL)
#        echo  $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB) >>makeb5.lnk
        echo $(C3DL) $(ALIB1) $(ALIB2) ole2w32.lib import32.lib $(BORCLIB) >>makeb5.lnk
!else
#        echo  $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB) >>makeb5.lnk
        echo $(C3DL) $(ALIB1) $(ALIB2) ole2w32.lib import32.lib $(BORCLIB) >>makeb5.lnk
!endif
		echo $(NAME).def >>makeb5.lnk
#!ifdef RESRES
!if "$(RESRES)"!=""
		echo $(RESRES) >>makeb5.lnk
!endif

makeb9.lnk: $(ZAPPDIR)\samples\template.mak makefile
!if $(MAKEDLL)
	echo c0d32.obj+ > makeb9.lnk
!else
	echo c0w32.obj+$(DLLOBJ)+ > makeb9.lnk
!endif
	echo $(LFILES) >> makeb9.lnk
	echo $(NAME) >>makeb9.lnk
	echo $(NAME) >> makeb9.lnk
!if $(ZAPPDLL)
        echo $(DLLLIB)+ $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB) >>makeb9.lnk
        echo $(INTPACKLIB) +ole2w32.lib+import32.lib+$(BORCLIB) >>makeb9.lnk
!else
        echo $(ZAPPDIR)\LIB\zap$(BUILDTYPE).lib+ $(C3DL) $(ALIB1) $(ALIB2) $(olelibs) $(CDLGLIB) >>makeb9.lnk
        echo $(INTPACKLIB)+$(RL)+ole2w32.lib+import32.lib+$(BORCLIB) >>makeb9.lnk
!endif
	echo $(NAME).def >>makeb9.lnk
	echo $(RESRES) >>makeb9.lnk


LIBMESSAGE1=Building zApp 3.0 for Win32 $(LIBMESOUTTYPE),,
LIBMESSAGE2=$(LIBMESCOMP)$(LIBMESLINK)$(LIBMESPER)$(LIBMESDEB)$(LIBMESCTL3D)$(LIBMESLIB)

message:
			@echo.
			@echo.
			@echo $(LIBMESSAGE1)
	        @echo $(LIBMESSAGE2)
			@echo.
			@echo.
			@echo $(LIBMESSAGE1) > lastmake.txt
			@echo $(LIBMESSAGE2) >> lastmake.txt

