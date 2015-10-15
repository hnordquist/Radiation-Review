RAIMA OBJECT MANAGER (ROM)
VERSION 3.0 RELEASE NOTES
August, 1996

===========================================================================
QUICK TIPS!!
===========================================================================
-- If you already have ROM 3.0 Beta 1, you'll probably only be interested
   in Section 3 and the first part of Section 4 of this document.

-- The changes since Beta 1 are covered in the first part of Section 4.

-- Porting issues are covered in Section 5.

-- Accompanying this version of ROM is an add-on class library, the Server 
   Extension Toolkit (SETK).  It provides a C++ interface that makes 
   it quite easy to create a Velocis Server Extension.
   With this capability, you can move your database intensive C/C++
   application code to the server.  SETK is in pre-release form and as such,
   is somewhat raw.  We wanted to get it into your hands now to let you start
   working with it and to help give us an idea of what other 
   features/directions you'd like to see it go.

   The SETK is covered in the last section of this document, Section 9.

===========================================================================
TABLE OF CONTENTS
===========================================================================

1.  INTRODUCTION

2.  COMPILER and PLATFORM INFORMATION
    -- ROM with Raima Database Manager
       -- DOS/Win16
       -- Win32
       -- OS/2
       -- Unix
    -- ROM with Velocis (Raima Database Server)
       -- DOS/Win16
       -- Win32
       -- OS/2
       -- Unix
    -- ROM 3.0 Server Extension
       -- Windows 95/NT
       -- OS/2
       -- Netware NLM
       -- Unix

3.  BUILDING ROM 3.0
    -- For DOS, Win16, Win32, OS/2
    -- For Unix
    -- The Server Extension (for Velocis)
       -- Windows 95/NT
       -- OS/2
       -- Netware NLM
       -- Unix
    -- Additional notes on building

4.  SUMMARY OF CHANGES
    -- Changes since Beta 1
    -- Changes since ROM 2.0
       -- Architectural
       -- Macros
       -- General
    -- New features since ROM 2.0
    -- New features in ROM 2.x not documented in manuals

5.  REQUIRED CHANGES FOR USING ROM 3.0

6.  WRITING CODE THAT WORKS WITH BOTH RDM AND VELOCIS

7.  ADDITIONAL INFO
    -- Velocis Instance Locking
    -- Velocis Implicit Locking
    -- DDLP generated header files

8.  NOTES CONCERNING RELATIONSHIPS BETWEEN OBJECTS AND DATABASES
    -- The setup
    -- ROM 2.x solution
    -- Miscellaneous

9.  THE SERVER EXTENSION TOOLKIT

===========================================================================
1.  INTRODUCTION
===========================================================================

Raima Object Manager (ROM) 3.0 supports both Raima Database Manager (RDM) 
3.x and Velocis Database Server 1.x (formerly Raima Database Server). 
Any reference to RDS in this document or any ROM document refers to Velocis 
Database Server.

ROM 3.0 for Velocis has been implemented as a Server Extension in
order to reduce the amount of network traffic between the client and
the server.  ROM 3.0 for Velocis will also run with Velocis Windows 
Stand-Alone.

ROM 3.0 is being distributed with runtime libraries.  The ROM Server Extension for
AIX,HP_UX, NT, Netware, SCO Solaris  have been included in the Installalation diskette and
could be installed/registred during installion process.However if you wish have to register 
the ROM Server Extension manually with the Velocis Server you can then copy the desired
ROM SE to the device of your Server installation.  If you don't know how to do these things, refer
to your Velocis Administrator's Guide for more information.

You may to build ROM for whatever compiler, OS, and database engine 
you desire.  Instructions for this are covered in section 3 of this document.


===========================================================================
2.  COMPILER AND PLATFORM INFORMATION
===========================================================================

ROM 3.0 with RDM
----------------

  DOS/Win16
  -----------
  ROM 3.0 supports both Borland 4.5 and MSVC 1.5x for creating 
  DOS and Win16 applications using RDM as the repository.  

  For a DOS application, ROM 3.0 is a large-model static
  library that is linked with the RDM large-model context-switching 
  library (VISTALC.LIB) and can also be linked with the db_QUERY large-model 
  context-switching library (QUERYLC.LIB) to give you SQL-like access to 
  your database.

  For a Win16 application, ROM 3.0 is provided as a large-model
  Dynamic Link Library (DLL) that is linked with the RDM large-model 
  DLL and can also be linked with the db_QUERY large-model 
  DLL to give you SQL-like access to your database.


  Win32
  -----
  ROM 3.0 supports both MSVC 4.x and Borland 4.5 with the Win32 version of 
  RDM.  ROM is provided as a 32-bit DLL that is linked with the RDM 32-bit 
  DLL, RDMWIN32.DLL.
  

  ??? OS/2
  ----
  ROM 3.0 supports both the IBM C/Set and Borland C++ compiler for OS/2 with 
  the 32-bit OS/2 version of RDM.  ROM is provided as a 32-bit DLL that is 
  linked with the RDM 32-bit DLL, RDM330.DLL.


  ??? Unix
  ----
  ROM 3.0 comes with makefiles .  Contact your salesperson for 
  information on what is currently available.


ROM 3.0 with Velocis
--------------------
  
  DOS/Win16
  -----------
  ROM 3.0 supports both MSVC 1.5x and Borland C++ 4.5 for creating 
  DOS and Win16 applications using Velocis as the repository.  

  For a DOS client/server application, ROM 3.0 is a large-model static library 
  that is linked with the Velocis client library, RDSCLI.LIB.

  For a Win16 Stand-Alone application, ROM 3.0 is a large-model DLL that is 
  linked with the Velocis Windows Stand-Alone library, RDSSRV.LIB.

  For a Win16 client/server application, ROM 3.0 is a large-model DLL that is 
  linked with the Velocis client library, RDSCLI.LIB.


  Win32
  -----
  ROM 3.0 supports both MSVC 4.x and Borland C++ 4.5 for creating 32-bit 
  applications with Velocis.  


???  OS/2
  ----
  ROM 3.0 supports both Borland C++ for OS/2 and IBM C/Set 2.0 for
  creating 32-bit OS/2 applications with Velocis and is provided as
  a DLL that is linked with the Velocis client library.

  ??? Unix
  ----
  ROM 3.0 comes with makefiles for HPUX and AIX . 


ROM 3.0 Server Extension
------------------------
  
  Windows 95/NT
  -------------
  ROM 3.0 supports the MSVC 4.x compiler for building a Windows 95/NT 
  Server Extension.  


???  OS/2
  ----
  ROM 3.0 supports the IBM C/Set compiler for building an OS/2 
  Server Extension.  


  Netware NLM
  -----------
  ROM 3.0 supports the Watcom C/C++ 10.0 compiler for building a Netware NLM
  Server Extension.  Because some people don't have a Watcom compiler and thus 
  don't have the capability to build a Netware NLM, the NLM Server Extension 
  is being included in this package.  Filename: SE\NLM\_ROMSE.NLM


  Unix
  ----
  ROM 3.0 supports the native C/C++ compilers for building Unix Server 
  Extensions.


===========================================================================
3.  BUILDING ROM 3.0
===========================================================================

  For DOS, Win16, Win32, OS/2
  -----------------------------
  When building ROM for DOS, Win16, Win32, or OS/2, you have two options, 
  either use the project files that are supplied for building the ROM 
  libraries or use the MK.BAT system from the OMSRC directory.  If you choose
  to use the MK.BAT system, you'll first have to modify the file BASES.GEN 
  (located in the ROM root directory) to reflect the location and version of 
  the database engine (RDM or Velocis) that you wish to use.  A number of 
  macros are present in the BASES.GEN file that are used to build both the 
  ROM runtime libraries and example programs. You must have these macros 
  correct or the system will not build.

     ROMBASE: The root directory of ROM, for example:
              ROMBASE=c:\rom30

     RDMBASE: The root directory of RDM, for example:
              RDMBASE=c:\rdm40

     RDMVER:  This tells ROM which RDM library to link with.
                321 for RDM 3.21a or before
                330 for RDM 3.30 or 3.30a
                331 for RDM 3.31
                400 for RDM 4.0
                W32 for RDM for Win32
                for example:
              RDMVER=400

     RDSBASE: The root directory of Velocis, for example:
              RDSBASE=c:\velocis

     RDSVER:  This tells ROM which Velocis library to link with.
                SA  for Stand-Alone ROM libraries
                CS  for Client/Server client-side ROM libraries
                for example:
              RDSVER=CS

     DEBUG:   This flag (if uncommented) will cause ROM to be built with debug
              info:
              DEBUG=1

  After you have modified the BASES.GEN, you need to CD to the OMSRC directory
  and execute the MK.BAT (MK.CMD for OS/2) file, passing the appropriate command 
  line options.  Simply executing MK.BAT without any options will display a help 
  screen for you.  

  After the make process is finished (successfully), the newly created library
  will be copied into the LIB/COMPILER directory off of the ROM root directory.

  For ROM 3.0, the naming convention for the runtime libraries has changed.
  The following lists the ROM libraries that can be built:

    Files Created for use with RDM
    ------------------------------
      Platform    Library          Description
      --------    -------          -----------
      DOS         omdosrdm.lib     DOS Library
      Win16       omrdm16.dll      16-bit DLL
      Win16       omrdm16.lib      16-bit import library
      Win32       omrdm32.dll      32-bit DLL
      Win32       omrdm32.lib      32-bit import library
      OS/2        omrdm32.dll      32-bit DLL
      OS/2        omrdm32.lib      32-bit import library

    Files Created for use with Velocis
    ----------------------------------
      Platform    Library          Description
      --------    -------          -----------
      DOS         omdosrds.lib     DOS Library
      Win16       omrds16c.dll     16-bit client DLL
      Win16       omrds16c.lib     16-bit client import library
      Win16       omrds16s.dll     16-bit stand-alone DLL
      Win16       omrds16s.lib     16-bit stand-alone import library
      Win32       omrds32c.dll     32-bit client-side DLL
      Win32       omrds32c.lib     32-bit client-side import library
      Win32       omrds32s.dll     32-bit stand-alone DLL
      Win32       omrds32s.lib     32-bit stand-alone import library
      OS/2        omrds32c.dll     32-bit client-side DLL
      OS/2        omrds32c.lib     32-bit client-side import library
      OS/2        omrds32s.dll     32-bit stand-alone DLL
      OS/2        omrds32s.lib     32-bit stand-alone import library


  For Unix
  --------
  To build ROM 3.0 for Unix, you must first update the MAKEGEN.ROM and 
  MAKEGEN.RDM (for RDM) or MAKEGEN.RDS (for Velocis) files located in the 
  root directory of your ROM installation to reflect the location 
  and version of the RDM or Velocis installation that you wish to use.
  
    You should only have to change the ROM_HOME macro in the MAKEGEN.ROM file.
    
    For RDM, you'll have to change the RDX_HOME, RDX_VER, and RDX_QOBJ macros 
    in the MAKEGEN.RDM file to indicate the home directory for your RDM 
    installation, the version of you are using, and whether or not you are 
    using db_QUERY with ROM, respectively.

    For Velocis, you should only have to change the RDX_HOME macro in the 
    MAKEGEN.RDS file to point to the home directory of your Velocis 
    installation.

  You might also want/have to modify the MAKEGEN.HP file for a HPUX build, 
  MAKEGEN.AIX for an AIX build, ... (you get the idea) to reflect any special 
  compile options you would like to use when building ROM.  ROM's makefiles 
  currently have generic compile options and could/should be tinkered with to 
  optimize the ROM runtime libraries that are built.  
  
  Next, in the OMSRC directory, execute a MAKE on MAKERDM.HP for an RDM HPUX 
  build, MAKERDS.HP for a Velocis HPUX build, MAKERDM.AIX for an RDM AIX build, 
  MAKERDS.AIX for a Velocis AIX build, ... (once again, you get the idea). 

  For ROM 3.0, the naming convention for the runtime libraries has changed.
  The following lists the ROM libraries that can be built:

    Files Created for use with RDM
    ------------------------------
      Platform    Library          Description
      --------    -------          -----------
      Unix        libomrdm.a       Unix runtime library 

    Files Created for use with Velocis
    ----------------------------------
      Platform    Library          Description
      --------    -------          -----------
      Unix        libomrdsc.a      Unix client-side runtime library 
???   Unix        libomrdss.a      Unix stand-alone runtime library 


  The Server Extension (for Velocis)
  ----------------------------------
    The ROM Server Extension can be built as either a Win32 DLL, an OS/2 DLL,
    a Novell NLM, or a Unix shared library (this may vary depending on the flavor
    of Unix being built).

    Windows 95/NT
    -------------
    To build the ROM Server Extension for a Windows 95/NT server, a project file 
    is used.  _ROMSE.MDP is located in the SE\MSVC4x directory off of the ROM 
    home directory.  If you are unfamiliar with the operation of the MSVC 4.x 
    IDE or have trouble building the Server Extension, please contact Technical 
    Support.  Once the Server Extension (_ROMSE.DLL) is built, you will have to 
    1) copy it to the RDSDLLS device of the Velocis server you wish to use it 
    with, and 2) register the Server Extension with the Velocis server.  If you
    are unfamiliar with how to do this, either refer to your Velocis 
    Administrator's Guide or contact Technical Support.  When your application 
    instantiates a Task object, the Server Extension will automatically be loaded.

 ???   OS/2
    ----
    To build the ROM Server Extension for an OS/2 server, a C/Set makefile 
    is used.  _ROMSE.MAK is located in the SE\ICC directory off of the ROM 
    home directory.  You will have to modify the _ROMSE.MAK file to indicate 
    the location of your Velocis Server installation and libraries.  After doing
    so, you can execute a make on the file to produce the _ROMSE.DLL Server 
    Extension.  If you are unfamiliar with this operation or have trouble building 
    the Server Extension, please contact Technical Support.  Once the Server 
    Extension (_ROMSE.DLL) is built, you will have to 1) copy it to the RDSDLLS 
    device of the Velocis server you wish to use it with, and 2) register the 
    Server Extension with the Velocis server.  If you are unfamiliar with how 
    to do this, either refer to your Velocis Administrator's Guide or contact 
    Technical Support.  When your application instantiates a Task object, the 
    Server Extension will automatically be loaded.

    Netware NLM
    -----------
    To build the ROM Server Extension for a Netware NLM, a Watcom C 10.0 makefile 
    is used.  _ROMSE.WMK is located in the SE\NLM directory off of the ROM 
    home directory.  You will have to modify the _ROMSE.WMK file to indicate 
    the location of your Velocis Server installation and libraries.  After doing
    so, you can execute a make on the file to produce the _ROMSE.NLM Server 
    Extension.  If you are unfamiliar with this operation or have trouble building 
    the Server Extension, please contact Technical Support.  Once the Server 
    Extension (_ROMSE.NLM) is built, you will have to 1) copy it to the RDSDLLS 
    device of the Velocis server you wish to use it with, and 2) register the 
    Server Extension with the Velocis server.  If you are unfamiliar with how 
    to do this, either refer to your Velocis Administrator's Guide or contact 
    Technical Support.  When your application instantiates a Task object, the 
    Server Extension will automatically be loaded.

  ???
    Unix
    ----
    The ROM Server Extension is automatically built when you create the ROM 
	runtime libraries.  The Server Extension is also copied into the RDSDLLS 
    directory of your Velocis installation.  You will only have to register the 
    Server Extension with the Velocis server.  If you are unfamiliar with how 
    to do this, either refer to your Velocis Administrator's Guide or contact 
    Technical Support.  When your application instantiates a Task object, the 
    Server Extension will automatically be loaded.


  Additional notes on building
  ----------------------------
  -- If you are trying to use Borland C++ 3.1 to compile ROM for Velocis 
     (a combination that is unsupported), you might need to modify the
     file rdm.h in the Velocis INCLUDE directory.  Search for _GROUPLOCK
     in the file and you should find this section of code:

        typedef struct _GROUPLOCK {
            short ftype;   /* function type - CRLOCK, CMLOCK, COLOCK,
                       CSLOCK, CRSLOCK, DBALOCK,
                       RTLOCK, STLOCK */
            short rstype;   /* record or set constant */
            DB_ADDR dba;   /* database address (if required) */
            RDM_DB hdb;      /* database number of entity */
            unsigned short odnum;   /* open database number (reserved) */
            char lmode;         /* lock mode, 'w' or 'r' */
        } GROUPLOCK;

     If there isn't an OM_EXPORT macro definition right before it, 
     you will have to modify it to be:

        #ifndef OM_EXPORT
        #define OM_EXPORT /**/
        #endif

        typedef struct OM_EXPORT _GROUPLOCK {
            short ftype;   /* function type - CRLOCK, CMLOCK, COLOCK,
                       CSLOCK, CRSLOCK, DBALOCK,
                       RTLOCK, STLOCK */
            short rstype;   /* record or set constant */
            DB_ADDR dba;   /* database address (if required) */
            RDM_DB hdb;      /* database number of entity */
            unsigned short odnum;   /* open database number (reserved) */
            char lmode;         /* lock mode, 'w' or 'r' */
        } GROUPLOCK;

  -- If you are trying to use Borland C++ 4.5 to compile ROM for Velocis 
     as a 32-bit DLL, you might need to modify the file platform.h in the 
     Velocis INCLUDE directory. 
     Search for MSC in the file and you should either find this section of 
     code:

        #ifdef MSC
        #define REXTERNAL     APIENTRY
        #define REXTVARARG
        #define RINTERNAL
        #define RNEARPROC
        #define RTHREAD
        #endif

     or this section of code:

        #if defined(MSC) || defined(__BORLANDC__)
        #define REXTERNAL     APIENTRY
        #define REXTVARARG
        #define RINTERNAL
        #define RNEARPROC
        #define RTHREAD
        #endif

     if you found the first section, add the BORLANDC part to it to make it
     like the second section.

     You will also have to create an import library that tlink32 will accept.
     There is a file on the BBS and on Raima's FTP site that will do this for 
     you.  The file is called MKBLIB.BAT.  If you can not locate it in either 
     of those places, contact Technical Support.

  -- It is best to compile your application with byte alignment. 
     This should be done via compiler options.  Doing this will also
     ensure that you will not have alignment problems between RDM and
     Velocis (Velocis requires byte alignment, RDM doesn't).

  -- With Velocis, an 'int' type is (by default) explicitly stored as 
     a 'short'.  If the -i switch is used when creating your database
     with the ddlproc.exe utility, then an 'int' type is stored as a 
     'long'.  So, it would be wise to simply not use the 'int' type
     and specify whether you want a 'short' or a 'long' instead.

  -- The example programs can each be built by calling the MK.BAT 
     batch file located in each example program's root directory.  
     When building the examples, the make process assumes that your 
     environment is set up correctly for your compiler and that your 
     RDM BIN directory is located somewhere in your PATH.
     If you are using Velocis, you will have to (pain-stakingly)
     register all (or whichever you wish to use) of the example
     databases with your Velocis Server.  

  -- A new version of the utility DDLP.EXE should be included with your
     ROM installation.  If you did not receive a DDLP.EXE, contact 
     Technical Support.  This new DDLP.EXE provides improved support 
     for ROM, and supercedes any previous versions of DDLP.EXE.  You 
     should make a backup copy of the original DDLP.EXE and copy the new 
     DDLP.EXE over the original.  If you are using Velocis, you should still 
     use this new DDLP.EXE to create the ddlp-generated header files, 
     but of course you will have to use either DDLPROC.EXE or SDDLP.EXE to 
     create your database.  Be careful to use the hpp file created from DDLP.EXE 
     and not one that might be created by DDLPROC.EXE or SDDLP.EXE.


===========================================================================
4.  SUMMARY OF CHANGES
===========================================================================

CHANGES SINCE ROM 3.0 BETA 1
----------------------------

  Architectural
  -------------
    -- VistaTask and VistaDb have been removed from the class hierarchy.
       Macros have been created to allow your application to still compile
       correctly without having to change references to them.

    -- All StoreObj navigation functionality takes place in the Navigator
       classes now.  The strange Goto2 functions have been renamed more 
       appropriately to Navigate.

    -- All d_ and c_ mapping functions in RDXDb and RDXTask have been moved
       to be out-of-line.  This should make it so you only have to link with 
       the ROM library you are using and not the RDM or Velocis library as 
       well.

    -- Although the ROM build structure and library naming convention doesn't 
       really facilitate it, with Velocis you can interchange the two types of
       ROM libraries (stand-alone and client/server) to run in either 
	   stand-alone mode or client/server mode without having to recompile
       your application.  To do this, it currently requires some trickery on
       your part as far as the DLL naming goes.  First, compile and link your
       application with whichever library you wish (either stand-alone or
       client/server).  Lets assume you link with OMRDS32C.LIB.  Your application
       will require that the OMRDS32C.DLL file is in your path somewhere.  What
       it doesn't require is that it actually is the OMRDS32C.DLL.  So, to 
       quickly change to stand-alone mode, you'd simply have to rename the 
       OMRDS32S.DLL file to be OMRDS32C.DLL (since that is what the application
       is still looking for) and replace the REAL OMRDS32C.DLL.  Now, when your
       application runs, it'll be running in stand-alone mode.  Obviously, we 
       need to do some work on this end to make this more of a seamless 
       operation for you, but heck, at least it works.

  Object Status
  -------------
    -- The status of an object upon construction is now set to S_OKAY.  
       An object is thought to be Okay as long as its status
       is S_OKAY -- which means that no database errors were encountered 
       while the object was last being manipulated.  

    -- StoreObj::NotRead now returns True if the object's status is not 
       Okay (not S_OKAY) or it has a null DB_ADDR.

    -- An object's status is now set to S_OKAY upon deletion (rather than
       S_DELETED).  This enables you to check for an Okay() status after
       a deletion.  Also, the object's DB_ADDR is now set to NULL_DBA after 
       deletion.


  Server Extension (Velocis only)
  -------------------------------
    -- Velocis c_ dictionary calls have all been implemented in the RDXDb 
       class (base class for StoreDb).  Other dictionary calls being made
       in the ROM library now make calls to these instead of d_dict.
       
    -- Information is being sent both to the Server Extension and from the 
       Server Extension heterogenously.  Previously, only data flowing to
       the server was being sent heterogenously.

    -- Multiple databases can now be opened.

    -- Multiple logins by the same client work correctly now.

    -- hlf_DirectNavigation was missing a call to d_recset.  It has been 
	   found.

    -- The Server Extension now cleans up memory for dead users.
	
	-- Various field types were not being passed to the Server Extension 
       correctly when operating in heterogenous mode.  This was noticable
	   when doing searches on indexed fields.


  Implicit Locking (Velocis only)
  -------------------------------
    -- Implicit locking now ignores the S_LOCKED and S_ILLDOWNG messages 
       allowing for an implicit lock attempt to be made multiple times on an 
       object.

    -- In Beta 1, if you were navigating to an object and that object was 
       already locked by someone else, you'd either have to wait until it was 
       available or implement a convoluted work-around to allow you to skip 
       it and simply navigate to the next object.  A new feature has been 
       added to allow this skip and navigation to happen naturally.  When the 
       object's implicit lock fails because someone else has it locked, the 
       object is actually navigated to but is not read.  So, if the navigation 
       fails with a status of S_UNAVAIL, you can either skip it and navigate to 
       the next object or go into a loop trying to StoreObj::ReRead the object 
       until the status is S_OKAY.


  StoreObj Class
  --------------
    -- NewObj, Write, Read, and Delete all return a RStoreObj now.  This allows 
       you to write your code as follows:

         if (!obj.Write().Okay())
            dosomething();

    -- The copy constructor and assignment operator both now adopt the 
       source object's navigation method, status, DB_ADDR, and PStoreDb.

    -- StoreObj::ReRead has been added that (as it implies) rereads an object
       from the database.

    -- RLBSet, RLBClear, and IsRLBSet have been added to StoreObj for support 
       of RDM's record lock bit functionality.


  OmBlob Class
  ------------
    -- A pointer was not being cleaned up when writing a Blob with Velocis.
	
	
  TransAction Class
  -----------------
    -- The TransAction class can now be used as if it supported nested 
       transactions.  Reference counting is done in the StoreTask the 
       TransAction is operating on.  If a nested transaction aborts, the 
       entire transaction will abort when the outermost transaction either 
       ends or aborts.

    -- The TransAction status 'NoTask' has been changed to 'TrNoTask' to match
       the other status names.

    -- The TransAction class now utilizes it's data member TrName.


  GroupLock Class
  ---------------
    -- The GroupLock class now supports lock request upgrading.  It keeps 
       track of records and sets that are both currently locked and in the 
       lock request queue.  Simply add any new lock requests to the GroupLock 
       object and reissue the Lock() command.  Neat.

    -- Setting a lock on set SET_TYPE with a GroupLock will now lock
       the owner, member, and set instance for Velocis.  This is the 
       default for RDM and in order to make your cross-database locking 
       seamless, this has also been done for Velocis.

    -- GroupLock::IsInRequest now only returns True if the request has been 
       issued but not yet granted.

    -- GroupLock::IsLocked now only returns True if the request has been 
       issued and granted.

    -- There is a difference in behavior between using RDM and Velocis when
       the UnLock function is called.  With RDM, only the locks that have been
       placed by the GroupLock will be unlocked -- meaning that you could have 
       set an individual ReadLock on a record type before using the GroupLock
       and it would persist the UnLocking of the GroupLock.  With Velocis, the
       UnLock will unlock all locks held by the associated Session.  This is 
       currently being done for performance reasons.  Until the 
       GroupLock::Unlock function is implemented in the Server Extension, it
       will remain this way.  RDXGroupLock::sUnlock is where the actual 
       implementation is, so if you don't like this behavior and feel 
       comfortable with changing the ROM source code, ...

    -- GroupLock::Reset has been added to clear both the lock request queue 
       and the held locks list, but does not, however, unlock any held locks.


  Error Handling
  --------------
    -- An Error Handler has been added to ROM 3.0 that has all error
       messages (RDM, RDS, and ROM) routed thru it.  If you set your
       own error handler, the error will first go thru the ROM error
       handler and then be passed on to your handler.  ROM-specific 
       errors have been designated to fall between -6000 and -6999. 
       The file romerr.h in the OMINC directory has the information
       about all possible ROM-specific errors.  Also, errors that
       occur in Velocis will actually cause the error handler to 
       trigger and display a message.  In ROM 2.0, if you hadn't 
       specified an error handler of your own, these errors went 
       undetected.

    -- RDXTask::SetErrorHandler has been added to allow you to set your error
       handler.

    -- Your error handler should have the following prototype now:
          void OMEXTERNAL MyDberr(ROM_ERRINFO OM_FAR &);
       where ROM_ERRINFO has the following structure:
          typedef struct ROMErrInfo_S
          {
             short errnum;
             char  errtxt[256];
          }


  Macros
  ------
    -- The following functions have been added to the OWNEROF macro to help
       increase the type-checking for the set parameter:

          T_F MakeOwnerOf(OBJ OM_FAR &member, SET_TYPE set = SET);
          T_F IsOwnerOf(OBJ OM_FAR &member, SET_TYPE set = SET);

       where IsOwnerOf makes a call to a new StoreObj::IsOwner function that 
       checks to see if the passed member object is a member of this owner 
       object.  The original StoreObj::IsOwner, which simply checks to see if 
       the passed member object is a member of the set (not the owner), remains 
       available.

    -- The following functions have been added to the MEMBEROF macro to help
       increase the type-checking for the set parameter:

          T_F NewMemberOf(OBJ OM_FAR &owner, SET_TYPE set = SET);
          T_F IsMemberOf(OBJ OM_FAR &owner, SET_TYPE set = SET);

       where IsMemberOf makes a call to a new StoreObj::IsMember function that 
       checks to see if the this member object is a member of the passed owner 
       object. The original StoreObj::IsMember, which simply checks to see if 
       this member object is a member of the set (not the owner) remains 
       available.

       NewMemberOf creates a new member object (with NewObj) and Connects it
       to the passed in owner via SET.

       Additionally, the GotoSet function from the MEMBEROF macro has been 
       renamed to be a more appropriate GotoMemberOf.

    -- The OM_OLDNAMES macro that was added for Beta 1 has been removed.  The 
       necessity for it has also been removed.  The protected data members it
       surrounded have been left as they were in ROM 2.0 and not moved to be
       private members.



CHANGES SINCE ROM 2.0 (ie. changes made for ROM 3.0 Beta 1)
-----------------------------------------------------------
  Architectural
  -------------
    -- VistaObj is now the base class from which all storable objects
       derive.  It provides an object with a status and a pointer to
       the VistaDb (PVistaDb) whose database the object is stored in.
       
    -- StoreObj derives from both VistaObj and DbAddr. Although 
       conceptually it would be better for StoreObj to have DbAddr 
       as a data member -- thus giving it a 'has a' relationship -- 
       it would munge the code too much to do this and still attempt 
       to provide the DbAddr interface that is historically 
       accessable thru StoreObj.

    -- Polymorph now derives from a new class, GenericMemObj, that
       can iterate over multi-member sets with disregard for the
       type of the member object it is reading.

    -- OmBlob no longer derives from Polymorph but rather derives
       solely from VistaObj.  A Blob is not a Polymorph nor a
       StoreObj -- you can't iterate from one to the next, there is 
       only one navigation method associated with a Blob, ...  
       A Blob does however USE a StoreObj (a GenericMemObj in
       fact) to read/write all the members of the set.

    -- The semantics of OmBlob have also been changed.  Previously,
       the OmBlob class  could be thought of as more of a vessel for
       reading/writing blobs rather than a self-contained object. 
       This blob object could be used to Delete (for example) any 
       other blob in the database by simply passing Delete() the 
       owner of the blob to delete.  The blob that was used to Delete 
       the other blob would remain loaded in memory and unchanged.  
       This would seem to break the object-oriented paradigm.  Now, 
       there is tighter coupling between a blob and it's owner 
       in that the blob actually knows who it's owner is and will
       only act on the members of that owner.  In order to delete a
       different blob, either a new blob must be created (to leave
       the original blob loaded and unchanged) or the original blob
       will get a new owner and delete it's members.  The
       SetOwner(RStoreObj) function re-assigns the blob's owner and 
       uses this owner when trying to Read, Write, or Delete the blob.
       Your code will not have to be modified unless it is dependant
       on the blob not changing in this scenario.

    -- Class VariMemb is no longer used by either Polymorph or
       OmBlob and has subsequently been removed.

    -- Unfortunately, the semantics of the virtual mechanism on
       StoreObj::Read() has been broken for Velocis, thus making
       it unsafe to override it.  The Read() function, which can be 
       called from several places in StoreObj, will now only get called 
       if you explicitly call it from a derived class. Otherwise, it 
       will simply be executed in the Server Extension. To allow you to 
       do whatever processing you wish to do when an object is loaded,
       four new virtual functions have been added to facilitate this:
       StoreObj::OnRead(), StoreObj::OnWrite(), StoreObj::OnDelete(), 
       and StoreObj::OnNewObj().  After an object gets read into
       memory, the OnRead() function will be called.  These same
       functions are also available for OmBlob and Polymorph.

    -- Const-ness has been applied to all classes of ROM.  Because of
       this, you will need to check ALL virtual functions that you
       are overriding from ROM to make sure the signatures match. 
       The most common ones are RecType() and RecName().  RecData()
       has been left as a non-const member function for the time
       being due to a couple of compiler bugs that will return the
       wrong pointer if it is const.

  Macros
  ------            
    -- The only macros that are required to build your application
       with ROM 3.0 are your choice of OM_RDM or OM_RDS.  All other
       macros that were needed in previous releases (WINDOWS, TURBO, 
       MSC, etc.) are no long needed as long as you use the correct
       compiler options to tell it whether you are making a Windows
       app, a Dos app, or a DLL.  If you are making a DLL that uses
       ROM, you will also need to define USE_DLL.

    -- OM_FAR is a new macro to accomodate the Velocis RFAR and the 
       RDM DB_FAR macros.

    -- OM_NEAR is a new macro to accomodate the Velocis RNEAR and the 
       RDM DB_NEAR macros.

    -- OM_COMMONDB is a macro that you can define (when compiling 
       your application) that will limit the API to only those functions 
       and data members that are common to both RDM and Velocis for 
       cross-database deployment.  This should be attempted
       periodically to ensure that you are only using API that is
       intended for both database engines.  This macro is only to be
       used as a checking device as your application will not run
       correctly if it is compiled with this option.

    -- REC_TYPE, SET_TYPE, FLD_TYPE, and INDX_TYPE are macros used to
       hide the fact that Velocis and RDM don't always use the same
       data types for the same information.  For example, the record
       type for Velocis is a short and an int for RDM.  REC_TYPE is
       used to hide this.

    -- OWNEROFBLOB and BLOBMEMBEROF are new macros that must be used
       instead of OWNEROF and MEMBEROF for the owner of a blob and
       the blob itself.  Because OmBlob no longer derives from
       StoreObj, new macros had to be created to give the owner
       object and blob object the navigation methods they need.

    -- OMEXTERNAL is a new macro that has been created to mask the
       differences between REXTERNAL for Velocis and EXTERNAL_FIXED 
       for RDM.  These defines are used for callback functions like
       your error handler.  If you currently have an error handler,
       you should change whatever external macro you are using with
       OMEXTERNAL.

  General
  -------
    -- StoreObj::RecData() has remained the same because it works for
       most of the platforms we support.  However, because it is not 
       100% portable, the following is an example of what you can do 
       to ensure that RecData() will be portable:
          class Author : public StoreObj, public author
          {
             ...
             Pvoid RecData() { return (Pvoid)((author OM_FAR *)this); }
             ...
          }

    -- StoreObj::RecType() is now a const pure virtual function rather 
       than giving a runtime error if it is not redefined.  It now also 
       returns a type of REC_TYPE for both RDM and Velocis because RDM 
       uses an int and Velocis uses a short as the data type for a record 
       constant.  Therefore, if you have redefined RecType(), you will 
       need to make sure it returns a type of REC_TYPE and that it is 
       const.

    -- StoreObj::CrSet(), StoreObj::CrGet(), and StoreObj::CsmSet() all
       update VistaObj::status now.

    -- gen.hpp and gen.cpp (and others) have been removed from
       distribution because they were extra files that were not used in 
       creating the ROM libraries.

    -- The problem with Far Virtual Tables and Borland C++ has been 
       fixed.

    -- Blobs > 64k now work again with both RDM and Velocis.

    -- OmBlob::Import() and OmBlob::Export() now load the blob they
       operate on, whereas in ROM 2.0 they simply acted as a means
       to read/write a blob from/to a file without disturbing the
       contents of the blob they acted on.  The protected data members
       pFile and pTmpBuff have also been removed as they are no longer
       needed.

    -- The AutoLoad class has been renamed to OmAutoLoad due to a
       naming conflict with Visual C++ 2.0.  At some point, all class
       names will have to be made consistent.

    -- DDLP's class header creation has undergone many changes and fixes.
       Look at the section discussing DDLP for more information.

    -- The RDXDb::iClose() name has been changed to RDXDb::IncrClose() 
       for naming consistency.

    -- The StoreObj::TotalSize() name has been changed to 
       StoreObj::RecDataSize() for naming consistency.  It now returns 
       the total RecData size.


NEW FEATURES SINCE ROM 2.0 (new features added for ROM 3.0 Beta 1)
------------------------------------------------------------------
    -- ROM for Velocis is implemented as a Server Extension.  You
       will need to register the ROM Server Extension with your
       server using the admin utility as well as copy the ROM Server
       Extension into the RDSDLLS device.

    -- The full d_ API of both RDM and Velocis is now provided.  Note
       that using database-specific functions will not allow you to
       cross-compile between RDM and Velocis.  Look in OMINC\dbrdx.hpp
       and OMINC\taskrdx.hpp for the complete listings.

    -- Because the full Velocis d_ API is now provided, Instance Locking
       capabilities are now available.  Look at OMINC\storeobj.hpp or 
       the section on Instance Locking for more information.

    -- ROM is now capable of performing Implicit Locking when used
       with Velocis.  New functions have been added to the RDXTask
       class (in OMINC\taskrdx.hpp) to facilitate this.  New functions 
       have also been added to facilitate the Dirty Reads and
       Freeable Read Locks capabilities of Velocis.  Refer to the
       information on Implicit Locking in section 7 of this document.

    -- A new function Qtask::SetQueryPaths() now allows you to
       specify a qdbpath, qdfpath, and qrfpath.  Refer to your db_QUERY
       manual for more information on these user-defined paths.

    -- You are now able to switch the navigation method of a storable
       object via the StoreObj::SwitchNav() functions.  SwitchNav
       takes either no params, a RKeyObj, or a RSetObj to make
       the storable object have direct, relational, or network access
       respectively.

    -- You are now also able to use the First, Last, Next, Prev, and Goto
       operators that take a RKeyObj on any storable object. The
       relational access only applies to that specific function call and
       does not change the default navigation method. Previously, this
       did not work and caused dynalink errors if the object's default 
       navigation method was not relational.

    -- A StoreObj is now able to iterate over a multi-member set
       correctly.  It skips over member records that don't have the
       same RecType and reads into memory only those that do.  The 
       polymorph example demonstrates this.

    -- StoreObj::NewMember(RStoreObj, SET_TYPE) has been added that 
       simply creates a new object and connects it to the passed in
       StoreObj thru set SET_TYPE.


===========================================================================
5.  REQUIRED CHANGES FOR USE WITH ROM 3.0
===========================================================================

  -- OWNEROFBLOB and BLOBMEMBEROF are new macros that must be used
     instead of OWNEROF and MEMBEROF for the owner of a blob and
     the blob itself.  Because OmBlob no longer derives from
     StoreObj, new macros had to be created to give the owner
     object and blob object the navigation methods they need.  This
     (obviously) applies to all OmBlob-derived classes like VariLenText, 
     OmBitmap and WinBlob.

  -- ALL overridden virtual functions should be looked at for
     const-ness and return types.  If your function signature doesn't
     match ROM's, your application will not work correctly.  Virtual 
     functions like StoreObj::Write, StoreObj::Read and so on have new
     return types.  

  -- If you're using Velocis with ROM, you should not override 
     StoreObj::Read() anymore, but rather override OnRead() to get the same 
	 affect.  This functions will be triggered AFTER the object has been 
	 read. The reason for this is mentioned above.

  -- Types REC_TYPE, SET_TYPE, FLD_TYPE, and INDX_TYPE are now being
     used for record, set, field, and index identifiers that are
     passed to the database engine because RDM and Velocis use 
     different data types for some of these.  You should do the same.

  -- OMEXTERNAL is a new macro that has been created to mask the
     differences between REXTERNAL for Velocis and EXTERNAL_FIXED 
     for RDM.  These defines are used for callback functions like
     your error handler.  If you currently have an error handler,
     you should change whatever external macro you are using with
     OMEXTERNAL.


===========================================================================
6.  WRITING CODE THAT WORKS WITH BOTH RDM AND Velocis
===========================================================================

  -- In ROM 2.0 every effort was taken to only provide those functions 
     that were common to both engines.  Unfortunately, in doing so, it 
     made it impossible for you to take advantage of some of the features 
     Velocis has that RDM doesn't.  Now, because the entire Velocis d_ API 
     is available, it is up to you to use only those functions that are 
     common to both Velocis and RDM.  
     
  -- The macro OM_COMMONDB can be used to ease the pain of doing 
     cross-database development.  In order to use the macro, every so often, 
     you should define OM_COMMONDB and try to recompile your application.  If
     you are using some ROM functionality that is not available to both RDM
     and Velocis, your compilation should fail.  This is only a checking 
     device to ensure that your application is only using API that is intended 
     for both database engines.  Your application will not run correctly if 
     compiled this way, but it will notify you of any mis-use of non 
     cross-database compatible API.

  -- OmQuery can only be used with RDM and therefore cannot be used
     if you wish to use both RDM and Velocis with one code base.  
     Eventually, there will be a cooresponding class to unleash 
     Velocis's SQL engine.


===========================================================================
7.  ADDITIONAL INFO
===========================================================================

Velocis Instance Locking
------------------------
In ROM 2.0, the idea was to allow you to create an application for
either RDM or Velocis with little or no code changes.  In this regard ROM
2.0 was a success.  Where it failed, though, was in not allowing you to
utilize any of the new features provided by Velocis -- the most important
of those probably being Instance Locks.  

The following functions have been added to the StoreObj class for
implementing Instance Locking:

   // record instance locking
   T_F  ILock()
   T_F  IReadLock()
   T_F  IWriteLock()
   T_F  IFreeLock()

   // set instance locking
   T_F  ILockSet()
   T_F  IFreeSet()

As you will notice they all return either True or False indicating
the success or failure of the call.


Velocis Implicit Locking
------------------------
Some believe that Implicit Locking should be implemented at the 
server level.  Unfortunately, Velocis does not do this, leaving
ROM to try to do it at the client-side instead.  Now that ROM is
implemented as a Server Extension, Implicit Locking is being done 
at the Server level again.

There are two types of Implicit Locking available: Read and
Read/Write.  As one would suspect, Read type Implicit Locking will
only implicitly set Read Instance Locks and Read/Write will (of course) 
set both Read and Write Instance Locks.  These locking types also
work in conjunction with the rdlockmodes function, so if you have
Dirty Reads enabled and Implicit Locking also enabled, Read Implicit
Locks will not be attempted.

As an object is read in, a Read Instance Lock is placed on the record. 
Before an object is about to be updated, the Read Instance Lock is 
upgraded to a Write Instance Lock.  In an update situation, all locks 
will be held until the Transaction has ended.  In a situation where data 
will only be accessed with Read Instance Locks, all locks will be held 
until you specifically release them with a call to one of the RDXTask
FreeLock calls.

Here is an example of the read-only situation:
   // assume no locks held at this point
   { 
      // enable Implicit R/W locking
      Task.EnableImplicitLocks();

      printf("Enter name of document to export: ");
      gets(tmp);

      // Find() applies a Read Instance Lock on document
      document.Find(KeyObj(NAME,tmp));
      if (!document.Okay())
      {
         if (document.GetStatus() == S_NOTLOCKED)
            printf("   someone else is updating this document\n");
         else
            printf("   document not found\n");
      }
      else
      {
         printf("Enter name of file to export to (create): ");
         gets(tmp);

         // >> navigation applies Read Inst. Locks to all members
         // of blob set
         if ((document >> text).Export(tmp) == True)
            printf("   File '%s' has been created.\n", tmp);
         else if (text.GetStatus() == S_NOTLOCKED)
            printf("   someone else is updating this text blob\n");
         else
            printf("   File '%s' could not be created.\n", tmp);
      }

      // explicitly release all locks after operation
      Task.FreeAllLocks();
      break;
   }

In the event an Implicit Lock request can not be granted, the object's 
status will be set to S_NOTLOCKED and the function called will return as 
if it had failed.  So in the example above, if the Implicit Read
Instance Lock could not have been granted, document's status would
have been set to S_NOTLOCKED and the object would not have been read
in.

The following functions have been added to the RDXTask class for 
implementing Implicit Locking:

   short     LockModeInfo() { return lockmodeinfo; }
   // ability to read info without read locks
   void      EnableDirtyReads()  { lockmodeinfo |= OMF_DIRTY_READS; }
   void      DisableDirtyReads() { lockmodeinfo &= ~OMF_DIRTY_READS; }
   T_F       DirtyReadsEnabled() const { return (T_F)(lockmodeinfo & OMF_DIRTY_READS); }
   
   // ability to free read locks inside a transaction
   void      EnableFreeableReadLocks()  { lockmodeinfo |= OMF_FREEABLE_READLOCKS; }
   void      DisableFreeableReadLocks() { lockmodeinfo &= ~OMF_FREEABLE_READLOCKS;}
   T_F       FreeableReadLocksEnabled() const { return (T_F)(lockmodeinfo & OMF_FREEABLE_READLOCKS); }
   
   // Implicit Read Instance locks only
   void      EnableImplicitReadLocks()  { lockmodeinfo |= OMF_IMPLICIT_READLOCKS; }
   void      DisableImplicitReadLocks() { lockmodeinfo &= ~OMF_IMPLICIT_READLOCKS;}
   T_F       ImplicitReadLocksEnabled() const { return (T_F)(lockmodeinfo & OMF_IMPLICIT_READLOCKS); }

   // Implicit Read and Write Instance locks
   void      EnableImplicitLocks()  { lockmodeinfo |= (OMF_IMPLICIT_WRITELOCKS | OMF_IMPLICIT_READLOCKS); }
   void      DisableImplicitLocks() { lockmodeinfo &= ~(OMF_IMPLICIT_WRITELOCKS | OMF_IMPLICIT_READLOCKS);}
   T_F       ImplicitLocksEnabled() const { return (T_F)(lockmodeinfo & (OMF_IMPLICIT_WRITELOCKS | OMF_IMPLICIT_READLOCKS)); }

   // Free Implicit (or other) Locks
   //  FreeWriteLocks and FreeAllLocks will only work in certain open modes
   //  refer to your Velocis User's guide for more info
   T_F       FreeReadLocks()  const { return T_F(release(RDLOCKS)); }
   T_F       FreeWriteLocks() const { return T_F(release(WRLOCKS)); }
   T_F       FreeAllLocks()   const { return T_F(release(RWLOCKS)); }

Refer to your Velocis User's Guide in section 7.6 regarding
Transaction Isolation for more information about Dirty Reads and
Freeable Read Locks.

DDLP Generated Header Files
---------------------------
The ddlp.exe utility has undergone more improvements in an attempt to
give you a kick-start on your class creation when creating a new
database.  

This is probably a good place for a disclaimer about the header file 
ddlp.exe produces:  
  -- It is typically a logical starting place for your class creation.

  -- If your database is not complex, the header file ddlp.exe
     produces will most likely be compilable and you will be able to
     simply derive from the classes it defines, add in your member
     functions, and shazam, the class definition process has been taken 
     care of for you.  Most of the ROM example programs have been created 
     in this way.

  -- In the event your database is complex and ddlp.exe has goofed, you 
     can simply use the header file as a quick template of what MACROS and 
     functions you should put in your own class definition.  It would
     be a good idea to also let Tech Support know about the problem
     you've run into so we can fix it.

Changes to ddlp.exe since ROM 2.0:
  -- The -ood flag has been removed.  With the -ood flag specified, DDLP
     attempted to create default constructors for you -- meaning that
     if your record only had one key and no sets attached to it, it
     would make the object have keyed access by default. 
     Unfortunately, in the real world this example very rarely
     applies and also unfortunately, without changing the grammar of
     DDLP a great deal, it would hardly ever guess what you really
     intended an objects default access to be.  It is a nice idea,
     but one that should be pursued in a different tool.

  -- Using the special bitmap, blob, and varilen key-words when
     specifying a multi-member set, seem to work fine now.  The
     naming convention has changed a little, but it is more intuitive
     now.  The name of your bitmap, blob, or varilen text object will
     now be the name of the multi-member set (rather than the name of
     the first member of the set).

  -- ddlp.exe now creates subclasses correctly when you have multiple
     sets between two records.

  -- It now creates classes to handle polymorphic relationships.

  -- It now applies const-ness to all functions it creates.

All in all, ddlp.exe seems to work well.  It only falls down when your 
database mixes "special" records and sets.


===========================================================================
8.  NOTES CONCERNING RELATIONSHIPS BETWEEN OBJECTS AND DATABASES
===========================================================================

The Setup
---------

First, consider the following ideal ROM architecture:

    data objects (StoreObj derived from VistaObj)
        belong to
    databases (StoreDb derived from RDXDb)
        which belong to
    tasks (StoreTask derived from RDXTask).

Prior to 2.x, the following class members and functions were used to help
facilitate the relationship between these objects:

* All tasks contain the common data member StoreTask::pCurrTask,
  which is a static StoreTask pointer that points to the "current"
  task in the application.  This "current" task is normally
  determined by the most recent instantiation of a task or the
  invocation of a task's StoreTask::MakeCurrent() member function.

* Each task maintains a list of active and inactive databases
  associated with it.  NOTE: Although each task may manage several
  databases, only one instance of a particular database type can
  be associated with a particular task.

* Every unique database type (derived from StoreDb) contains a
  static data member named <db_name>::DbPtr, which is a StoreDb
  pointer that points to the "current" database of that type.  As
  with tasks, the "current" database is normally determined by the
  most recent instantiation of this database type or the invocation
  of the database's <db_name>::MakeCurrent() member function.

* Each database also contains an internal pointer to its
  associated task.  Since there is no current means for specifying
  which task the database should belong to, the "current" task found
  in StoreTask::pCurrTask is used.  This is usually not a
  problem, though, since databases are normally instantiated as
  data members within a task - an event that will occur when the
  encapsulating task is instantiated and, hence, will be the
  "current" task.

* Each data object type (derived from StoreObj) contains
  the GetDb() member function defined by the STOREDIN() macro,
  which is responsible for returning a pointer to the object's
  associated database.

Since a task object normally encapsulates its associated
databases (as mentioned above), it is often useful for the user to
define a MakeCurrent() member function in the task object.  This
function normally invokes the task's StoreTask::MakeCurrent()
member function, as well as the <db_name>::MakeCurrent() member
function for each of its databases.


The Problem in ROM 1.1
----------------------

The most significant problem associated with multi-tasking was not related to
the tasks themselves, but due to the nonexistence of a direct relationship
between a data object and its database.  As noted above, this relationship
was provided by the GetDb() member function in the data object.  This function
actually returned the "current" value found in <db_name>::DbPtr (the "current"
database of that particular database type), which did not necessarily
reference the same database used when creating the data object.  Since most
of a data object's functionality is still performed internally by the task
object, the object->database->task relationship must be stable.

A multi-tasking problem that was directly related to a task object concerned
the GroupLock class.  This was due to GroupLock's direct use of the
StoreTask::pCurrTask pointer rather than making use of its own internal data
member that points to its related task.

Another result of this design was that no internal relationship existed
between GroupLocks and TransAction objects, and a separate instance of each
was required for each task object that they were related to.  Although the
RDM database implements locks and transactions based on tasks, there is
little reason for the user of an "object oriented" database to be concerned
with such issues.  In short, ROM has the ability to make tasks transparent to
the average user, and should at least provide the mechanisms for such.


ROM 2.x Solution
----------------

The solution is rather simple.  Data objects now contain an internal data
member, StoreObj::_pdb, which is a StoreDb pointer that points to the
database that the object is related to.  If this pointer is NULL, the data
object is related to the "current" database found in <db_name>::DbPtr (the
same behavior found in previous versions of ROM).

To maintain code compatibility, the name of the GetDb() member function
created by the STOREDIN() macro was changed to _GetDb(), and is still the
mechanism used to retrieve the "current" database (described above).  The new
GetDb() member function (which ROM 2.x uses) returns the internal data member
StoreObj::_pdb if it is non-NULL.  Otherwise it returns a call to _GetDb().

To initialize this new internal data member, the StoreObj constructors have
been modified to accept an additional parameter, a StoreDb pointer, that
defaults to NULL if unspecified.  This allows existing code to be compatible
with the previous versions of ROM.  New development will most likely take
advantage of this same mechanism when creating new data objects derived from
StoreObj, but will set this new default parameter to the appropriate
database's <db_name>::DbPtr.  The header files created by the new DDLP use
this mechanism, as in the following:

    class SomeObj : public StoreObj, public SomeRec {
        public:
            SomeObj(PStoreDb pdb=somedb::DbPtr) : StoreObj(pdb) {}
            SomeObj(KeyObj &key, PStoreDb pdb=somedb::DbPtr) :
                StoreObj(key, pdb) {}
            SomeObj(SetObj &set, PStoreDb pdb=somedb::DbPtr) :
                StoreObj(set, pdb) {}
        ...
    };

In the event that a data object should ever need to be related to another
database of the same type, a new member function, StoreObj::SetDb(), has been
added to reassign the value of the internal data member StoreObj::_pdb.
This function can also be used to set the data member to NULL if old ROM
behavior is desired.


Miscellaneous
-------------

The following issues are directly and/or indirectly related to multi-tasking
with ROM:

* Normally, TransAction objects are associated with the "current" task found
  in StoreTask::pCurrTask when they are instantiated.  To alter this behavior,
  pass the desired task to the constructor.

* A GroupLock object normally associates itself with the task related to the
  first data object passed to it.  If any subsequent data objects are passed
  to the GroupLock object that are not related to this same task, an error
  occurs.  Unlike the TransAction object, there is no constructor that can be
  used for assigning a particular task; however, the member function SetTask()
  can be used if the GroupLock object is still empty (right after
  instantiation, before any data objects have been passed to it).

* If global instances of task objects are to be created, and these tasks make
  references to the "current" databases (described above), be sure that the
  DB_INIT() macro has been invoked first.  Otherwise, the values found in
  <db_name>::DbPtr will be undefined.

* Global data objects (derived from StoreObj) can be instantiated before their
  associated tasks and databases have been created; however, the developer
  must be aware of the state of the <db_name>::DbPtr variables that they
  reference.  If the global data object is instantiated with a NULL parameter
  for the database pointer argument (whether explicitly or by default), the
  data object will provide the same behavior as previous versions of ROM, as
  described above.  If a non-NULL parameter is passed, be sure to invoke the
  DB_INIT() macro to prevent undefined values in the "current" database
  pointers, as described in the previous item.  Also, the order of
  instantiation for global objects is important, since any reference to these
  "current" database pointers in the data object's constructors will have the
  same effect as passing NULL if no instances of the desired database exist.


===========================================================================
9.  THE SERVER EXTENSION TOOLKIT
===========================================================================

  Introduction
  ------------
  The Server Extension Toolkit (SETK) is an addon tool for Velocis developers.
  It provides a streams-like interface to the rpc_ layer and ultimately allows 
  you to build a Server Extension using C++.  This, combined with a ROM 
  stand-alone library linked into your Server Extension, allows you to move 
  your ROM code into a Server Extension to help improve your application's 
  performance.

  Currently, the SETK is in pre-release form and as such, is a little raw.  
  It has only had minimal testing to confirm the proof of concept.
  We're giving it to you now to let you get a taste of it and to hopefully
  get some feedback from you as to where you'd like to see it go.

  We'll obviously be doing more work on this to further integrate it into
  ROM, but for now, it is 'as is'.


  Building the SETK
  -----------------
  Currently the SETK libraries can only be built with a MSVC 4.x project file. 
  The SETK libraries are in the form of static libraries.  To build a set of 
  ROM libraries that will work with the SETK you have to go through two steps:

    1) Build the SETK client and server static libraries producing SETK32C.LIB
	   and SETK32S.LIB.

    ??? 2) Build both a client/server ROM library and a stand-alone library.
       Make sure that both OM_SERVER and SETOOLKIT are defined when the ROM 
       source files are compiled and that the client/server ROM library links 
       with the SETK32C.LIB library and the stand-alone ROM library links with 
	   the SETK32S.LIB library.

  Using the SETK
  --------------
  Included in the SETOOLKT directory is a REGRESS directory that serves as a 
  test program for the SETK.  It demonstrates the class layout for creating 
  a Server Extension using the SETK and how to use the operators to stream
  information to and from the Server.

  
  




