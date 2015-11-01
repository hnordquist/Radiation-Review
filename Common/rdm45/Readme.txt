
This file provides information concerning the contents of the delivery
diskettes, how to get help, and important information on new features or
changes in the Raima Database Manager (RDM) System that are currently 
not contained in the RDM manuals. It is important that you study this
document thoroughly as it includes information that is vital to the 
proper usage of Raima Database Manager on your system.

This file supplements the manuals and contains the most up-to-date
information on RDM 4.5, QUERY 4.5, and REVISE 4.5. Refer to the section 
on bug fixes for a description of the changes for each patch release.

Although this file refers to RDM 4.5 [Build 17], some of the new features
mentioned here were in fact added in RDM 4.0.  If you already have a
16-bit version of RDM 4.0 you may be familiar with some of the items
described in this file.

The sections of this file covering Query and Revise also describe some
features added in RDM 3.30 which are not yet documented in the manuals.

----------------------------------------------------------------------------


==========================================================================
TABLE OF CONTENTS
==========================================================================

The following is a list of the major topics found in this file.  While this
document is not broken into pages, these headings appear in the following
order and can be used in text searches.

GETTING HELP                  How to contact Raima Technical Support,
                              general warnings, and recommendations
                              
GENERAL INFO                  General warnings and reminders
                              
RAIMA DATABASE MANAGER 4.5    API changes, functional enhancements, new
                              error codes, required application changes
                              
QUERY 4.5                     Changes from 3.21
                              
REVISE 4.5                    Changes from 3.21
                              
LOCK MANAGER COMMUNICATION    The differences and uses of the lock 
                              manager for each platform
                              
OBJECT MANAGER                Notes on ROM-RDM compatibility
                              
BUG FIXES                     Bug fixes for each product
                              

==========================================================================
GETTING HELP
==========================================================================

SUPPORT SERVICES
----------------

     If you need assistance or have questions, please use one of the
     following support addresses / numbers:

     E-mail, Technical Support:     raimatech@raima.com
     Phone, Technical Support:      206-748-5333
     Fax, Technical Support:        206-749-0661

     If you are evaluating a beta version, please use this address:

     E-mail, Beta Test:             rdmbeta@raima.com
     
INTERNET
--------

     You can find us on the Internet at http://www.raima.com.  Our Internet
     site also includes FTP capabilities;  these are available through our
     web server.
     

==========================================================================
GENERAL INFO
==========================================================================

SUMMARY
-------

     1)  Directory Structure
     2)  Build Procedure
     3)  Compiler Constants
     4)  RDM DLL and Static Library
     5)  Debug Build
     6)  Installation Test
     7)  Compiler Support
     8)  Compatibility
     9)  Long File Names
     10) Change to ddlp
     11) Multi-Threaded Applications
     
DIRECTORY STRUCTURE
-------------------

     The directory structure for RDM for Win32 is as follows.
     Note that all directories may not be installed, depending
     on what you have purchased.
     
     bin            All the pre-made RDM utilities.
                    
     lib            The pre-made DLLs and static library.  The
                    subdirectories 'vwin' and 'qwin' contain the
                    import libraries and DLLs for RDM and QUERY
                    respectively; 'msvc' contains the RDM static
                    library.
                    
     examples       Subdirectories of sample programs and databases.
                    
     revise         REVISE libraries and program.  If the REVISE
                    library source is installed it is placed here.
                    
     include        Header files.
                    
     query          W_QUERY example program sources and executables.
                    If the QUERY library source is installed, it will
                    be placed here.
                    
     runtime        Source code for the RDM libraries.
                    
     wida           Source code for the Windows Interactive Database
                    Access utility.
                    
     lockmgr        Source code for the lock manager and remote console
                    utilities.
                    
     dal            Source for Database Access Language utility.
                    
     dbimp          Source for Database Import utility.
                    
     utility        Source for the RDM utilities.
                    
     dbedit         Source for the Database Editor utility.

BUILD PROCEDURE
---------------

     There are batch files in each directory for building the RDM
     libraries, executables and examples.  These are listed below.
     Note that several of the files cannot be run successfully until
     you have built the RDM libraries (with mkdll.bat and mklib.bat).

        Batch File:                     Generates:

        runtime\mkdll.bat               rdm45w32.dll
        runtime\mklib.bat               rdm45mu.lib
        dal\mkdal.bat                   dal.exe
                                        (run mklib.bat first)
        dal\mkedit.bat                  dbedit.exe
                                        (run mklib.bat and mkutils.bat first)
        dbimp\mkimp.bat                 dbimp.exe
                                        (run mklib.bat first)
        examples\sales\mkdbase.bat      sales example
                                        (run mklib.bat and mkutils.bat first)
        examples\tims\mktims.bat        tims console example
                                        (run mklib.bat and mkutils.bat first)
        examples\wtims\mkwtims.bat      wtims example
                                        (run mkdll.bat, mklib.bat and 
                                        mkutils.bat first)
        query\mkwquery.bat              rdm45w32.dll with qt_ functions
                                        (run mkdll.bat first)
        query\mkwdbqry.bat              w_query.exe
                                        (run mkwquery.bat first)
        revise\mkrevise.bat             dbrev.exe
                                        (run mklib.bat first)
        utility\mkutils.bat             utility programs and nolib.lib
                                        (run mklib.bat first)
        wida\mkwida32.bat               wida.exe
                                        (run mkdll.bat first)

     If you are rebuilding RDM completely, because of dependencies
     between the components, we suggest building in the following
     order:

     1. RDM libraries, rdm45w32.dll and rdm45mu.lib
        (mkdll.bat and mklib.bat in the runtime directory)

     2. Query library rdm45w32.dll and w_query.exe
        (mkwquery.bat and mkwdbqry.bat in the query directory)

     3. Utilities and dbimp.exe
        (mkutils.bat in the utility directory and mkimp.bat in
        the dbimp directory)

     4. Everything else

     Raima used MS Developer Studio, Visual C++ 4.0 to build the RDM
     libraries and executables - currently we do not have make files
     for other compilers.

     If you want to include support for the lock manager types for
     Novell networks (LMC_SPX and LMC_NOVELL) in the RDM DLL and static
     library, define the environment variable NOVELL before running the
     batch files, as follows:

          set NOVELL=1
          mkdll msvc \rdm45
          mklib msvc \rdm45

     The modules for these lock manager types are not included in the
     DLL or static library by default, because compiling them requires
     the Novell SDK.  If you do not have this, do not define NOVELL
     when rebuilding the libraries.  If you do have the Novell SDK,
     you will need to put the Novell include directory in the INCLUDE
     path definition for your compiler.

     The executable files delivered with this release were built with
     Novell SDK 9.

COMPILER CONSTANTS
------------------

     You will notice that the makefiles supplied for building the RDM
     libraries and utilities make use of certain compiler constants:

     DB_DLL       Define this if you are compiling the RDM source to
                  build a DLL

     DB_LIB       Define this if you are compiling the RDM source to
                  build a static library

     USE_DB_DLL   Define this if you are compiling application code
                  that includes vista.h and uses functions from the
                  RDM DLL

     USE_DB_LIB   Define this if you are compiling application code
                  that includes vista.h and uses functions from the
                  RDM static library

     If you do not define any of these constants, USE_DB_DLL will be
     defined by default in vista.h, i.e., it is assumed you are
     compiling an application that uses the RDM DLL.

     If DB_DLL or USE_DB_DLL are defined, then vista.h will automatically
     define MULTI_TASK, which ensures that the dt_ (context switching)
     functions are defined.  The RDM DLL only contains dt_ functions.

     If DB_LIB or USE_DB_LIB are defined then MULTI_TASK will not be
     defined.  The RDM static library only contains d_ (non-context-
     switching) functions.

RDM DLL AND STATIC LIBRARY
--------------------------

     The static library rdm45mu.lib is intended for use in command line
     programs such as the RDM utilities or the tims example, which do not
     require multiple contexts (multiple DB_TASKs).  Note that this library
     is not multi-threaded.

     For your applications you should use the DLL rdm45w32.dll, which
     supports multiple threads and multiple DB_TASKs.

     The name of the DLL has been changed to rdm45w32.dll so as to avoid
     confusion with earlier RDM versions, or with 16-bit DLLs.  The import
     library for the RDM DLL is rdm45w32.lib.

DEBUG BUILD
-----------

     The batch files and makefiles supplied will build non-debugging
     executables by default.  If you wish to build executables for
     debugging, please define the environment variable DEBUG on the
     command line before running the batch files, e.g.,:

          set DEBUG=1
          mkdll msvc \rdm45

     As well as compiling and linking with command line switches for
     debugging, this will cause the compiler constant DB_DEBUG to be
     defined on the compiler command line.  This will cause some extra
     RDM diagnostic code to be included (code in #ifdef DB_DEBUG and
     #ifdef DB_TRACE sections in the source files) that may be useful
     in the event of database errors.  Please refer to the section on
     Debugging Support below.

     Note that you should either build all or none of the source files
     with DEBUG defined.  If some are compiled with debugging code and
     others without, the linker may report unresolved symbols, such as
     db_printf.

INSTALLATION TEST
-----------------

     When you have completed installing RDM you may test your installation
     by compiling and running the example databases and programs that are
     included with this delivery.  The examples (called TIMS and WTIMS)
     provide illustrations of several aspects of using RDM, including an
     example schema, source code, an import specification, compilation,
     linking, and using the DDLP, INITDB, and DBIMP utilities.
     
     The TIMS example is a console application, which is built using the
     RDM static library. The WTIMS example is a Windows application, which
     uses the RDM dynamic link library.
     
COMPILER SUPPORT
----------------

     This delivery supports Microsoft Developer Studio 4.x (Compiler
     version 10.x).
     
COMPATIBILITY
-------------

     Since RDM 3.30 there have been file format changes to the DBL file,
     as well as changes to the messages exchanged by the Runtime and Lock
     Manager.  Consequently, it is not possible in general to intermix
     applications built with different versions of RDM, or different lock
     manager versions.  In addition, it is necessary to delete any DBL
     files when upgrading.

     However, if you are using RDM 4.0.2 (16-bit), or the 16-bit
     implementation of RDM 4.5 (available soon), you will be able to use
     your 16-bit applications simultaneously with 32-bit applications
     built with this product, provided you are careful not to use the
     "int" data type in your database definition, and provided the same
     structure alignment is used in both applications.
     
     Certain 16-bit releases of RDM 3.30 created incorrect key files for
     large negative key values, and for keyed struct fields.  In these
     cases, you must do a keybuild on your database when upgrading.
     Otherwise, the format of DBD, data and key files has not changed.
     
LONG FILE NAMES
---------------

     Earlier versions of RDM imposed a limit on the length of all
     database filenames and directories of 47 characters.  This limit
     has been changed to 255 characters in RDM 4.5.

     The database format (including the DBD file format) has not been
     changed.  Because the DBD file contains the names of all data and
     key files in the database, as specified in the DDL schema, there is
     still a limit of 47 characters on the names in the DDL schema (and
     the DBD file).  Since it is not recommended to hard-wire paths into
     the database definition, we hope this will not prove restrictive.

     The path of database files can be specified in several ways, such as
     in the dt_open() call, through dt_dbdpath() or dt_dbfpath(), or through
     the rdm.ini file.  All of these methods allow a 255 character path.

     Note that if you have written any programs that read the RDM database
     dictionary (DBD file) directly, and you are using the FILE_ENTRY
     structure defined in dbtype.h, you will need to change your programs
     so that they use V3_FILE_ENTRY instead.

CHANGE TO DDLP
--------------

     Because data size and alignment incompatibilities between applications
     sharing the same database can lead to data corruption, Raima has
     modified the Database Design Language Processor (ddlp) in order to
     make size and alignment issues more explicit.

     ddlp has been modified as follows: it now requires you to specify
     how many bytes to use for alignment of record structures, by using
     the -b switch, e.g.,:

          ddlp -b4 database.ddl

     will cause ddlp to generate a dbd file with fields aligned to
     4-byte boundaries.  The value you specify with -b should be the
     same as the value you use when compiling your application, using
     the -Zp switch.  In RDM 3.30 for Win32, ddlp defaulted to 4-byte
     alignment, but all 16-bit versions defaulted to single-byte
     alignment.

     Note that ddlp now displays an incompatibility warning whenever
     it finds an "int" field in your database definition.

MULTI-THREADED APPLICATIONS
---------------------------

     Multi-threaded applications are supported, but if two threads
     call RDM functions at the same time, the second will be blocked
     until the first has returned from RDM.

     Do not use the same DB_TASK for two threads.  Multiple threads
     will only be handled correctly if each thread has its own DB_TASK
     (i.e., each thread must have a unique DBUSERID).


==========================================================================
Raima Data Manager 4.5
==========================================================================

SUMMARY
-------

     1)  API changes
     2)  Error messages
     3)  Error reporting
     4)  Required application changes
     5)  16-bit to 32-bit database conversion strategies
     6)  Performance
     7)  Alignment checking
     8)  Timeouts
     9)  Debugging support
     10) Cache allocation
     11) Win32 Native I/O
     12) File Commit, SYNCFILES and REMOTESYNC
     13) Truename
     14) Pathname Compatibility Problems
     15) Shared Directories
     16) Novell Search Mode
     17) Country Table
     18) RDM.INI path
     19) Environment variables
     20) Wida and W_query Failure to Locate DBD File
     21) Summary of new runtime options
     
API CHANGES
-----------

     dt_lmclear()

          This function provides the same functionality as the program
          lmclear.exe, enabling an application to clear a user from the
          Lock Manager's user table.

     dt_dberr()
     
          This function can be used by an application program to call
          the internal RDM error handler.
          
     dt_def_opt()
     
          This function will reset selected RDM runtime options to their
          default values.
          
     dt_set_alignment_proc()
     
          This function establishes a callback function for verifying
          database structure alignments.
          
     dt_check_alignment()
     
          This function can be used to explicitly check database structure
          alignments on an open database.
          
     dt_dbstat()
     
          This function retrieves various runtime statistics.  This
          requires that the RDM library be compiled with #define DBSTAT,
          which is the default for the RDM 4.5 release.
          
     dt_rdmver()

          This function returns a version string that describes the
          particular version of the RDM library or DLL in use by the
          application.  Note that any program or utility which directly
          accesses the RDM version number string defined by db_VERSION
          in 'vista.h' must be changed to use this new function.
          
     See the enclosed manual pages for detailed descriptions of these
     functions.
     
     There have been many changes to the Raima header files;  therefore it
     is necessary to recompile all source files that reference RDM.
     
ERROR MESSAGES
--------------

     -901  S_SYSERR      This error now displays a message more accurately
                         describing the internal problem.  If this error is
                         encountered, it is imperative that the actual
                         error message be communicated to Raima when
                         calling Technical Support.
                         
     -903  S_NOWORK      The QUERY dictionary cannot be found.
                         
     -926  S_TTSERROR    An error occurred in a Novell TTS function.  The
                         error message will specify the name of the
                         function and the one-byte hexadecimal error code.
                         
     -946  S_ALIGNMENT   The compiler alignment used when compiling the
                         program does not match the alignment of the
                         database.  This is serious, as memory overwrites
                         and/or database corruption will result.
                         
     -947  S_REENTER     An attempt was made to call RDM re-entrantly from
                         inside the application's dt_set_dberr() handler.
          
ERROR REPORTING
---------------
     
     When an S_LMCERROR (-925) is generated, meaning that lock manager
     communication has been lost, the error will only be reported once, and
     doing a d_close() will close the database with no further error
     messages.  Calling any other lock manager communication function will
     continue to return the error, but without calling dberr().
     
     When RDM encounters an error, it now postpones calling the application
     defined error handler until after it has done any necessary internal
     cleanup.  In this way, it is now safe to do database operations, or do
     a longjmp() or a throw from inside the error handler for all errors
     except S_NOSPACE, S_RECOVERY, S_REENTER, and S_DEBUG.  If RDM is
     called re-entrantly from any of these error conditions, the new error
     code S_REENTER (-947) will be returned (but dberr will not be called).
                         
REQUIRED APPLICATION CHANGES
----------------------------

     If you have an application built with RDM 3.30 for Win32 you
     will find that you need to make some changes to the compiler and
     linker command lines to build your application.

     If you are using the RDM DLL (as opposed to the static library)
     make sure that LOCKCOMM_DATA is not defined in your application
     source files.  The RDM DLL includes the code for all lock manager
     types, so there is no need to define LOCKCOMM_DATA - in fact defining
     it will cause link errors.

     Applications built with the static library should have LOCKCOMM_DATA
     defined in one source file only (before the line where vista.h is
     included).  You do not need to define the lock manager types you
     are using (i.e., do not use #define LMC_NETBIOS, #define LMC_SPX, etc.),
     as all of these are now defined by default.  However, if you wish to
     exclude certain lock manager types, you can do so by defining symbols
     such as NO_LMC_NETBIOS, NO_LMC_SPX etc.  See examples listed below.

          #define LOCKCOMM_DATA
          #define NO_LMC_NETBIOS
          #define NO_LMC_SPX
          #define NO_LMC_NOVELL
          #define NO_LMC_GENERAL
          #define NO_LMC_INTERNAL
          #include "vista.h"

     The above definitions will mean that only the TCP lock manager type
     will be valid in your application.  You will still have to call
     d_lockcomm:

          dt_lockcomm(LMC_TCP);

     Source code changes will also be necessary if you are using the
     functions:

          dt_rdcurr
          dt_wrcurr

     The type of the first argument of both of these two functions is now 
     defined as:

          DB_ADDR **

     Whereas in RDM 3.30 for Win32 it was defined as:

          DB_ADDR_P *

     Also, see the section on Query in this file, which describes a change
     in printer and display function definitions.

     Otherwise, you should not need to make any changes to your application
     source code, as all the functions in RDM 3.30 are still supported in 
     RDM 4.5.  You may wish to change your source code to take advantage of 
     new features such as the new lock manager types, but these will be 
     minor changes.

     When building your application you should consider the following
     points:

          - There is no longer a 'winclude' directory. The include files
            are in "include", so you will need to modify your makefiles
            accordingly.

          - The name of the RDM library has changed.

          - Although you do not need to run ddlp, if you do run it,
            make sure that the structure alignment matches your existing
            database.  To check the structure alignment run prdbd and
            examine the field offsets in the Field Table.

          - Make sure that the structure alignment you specify when
            compiling also matches your existing database.

          - You may need to make changes to your database to avoid
            16-bit / 32-bit imcompatibilities - see next section.

16-BIT TO 32-BIT DATABASE CONVERSION STRATEGIES
-----------------------------------------------

     When you use a 32-bit RDM application on a database that was created
     with 16-bit utilities / applications, there are two types of problem
     you are likely to encounter:

     1. Different sizes of certain data types
     2. Different alignment of structure elements

     The size problem affects int and unsigned int fields. In a 16-bit
     database, these will be 2 bytes in size - ddlp will allocate 2 bytes
     in the record slot, and the (16-bit) compiler you use to compile your
     application will allocate 2 bytes in memory for ints.

     When you recompile your application with a 32-bit compiler, any record
     structures that contain ints will change in size - the compiler will
     allocate 4 bytes for ints.  Your existing 16-bit database will no longer
     match the structures in your application's memory.  If you do not wish
     to keep this existing data, you can just create a new, 32-bit database
     by running the 32-bit versions of ddlp and initdb.  Normally, however,
     you will want to go on using your existing database.

     The easiest way to solve this problem is to change all ints to shorts
     and change all unsigned ints to unsigned shorts in the record
     definitions in your database schema, and also in any other structures
     used by your application when accessing the database.  The short and
     unsigned short data types are 2 bytes in size on both 16-bit and 32-bit
     environments.  You must therefore create a new database dictionary that
     is identical to the old one, except that ints are replaced by shorts.
     You do not need to change the data and key files in your database.  This
     strategy is discussed below as "Solution 1".

     An alternative solution is to continue using ints in your records, and
     to revise the contents of your database to reflect the change in record
     size.  This strategy is described below as "Solution 2".

     To solve the alignment problem mentioned above, you must use either
     compiler switches or "#pragma pack()" directives to force the compiler
     to use the alignment used in your database.  See the section below
     on the #pragma pack() directive.

     SOLUTION 1 - REPLACE INTS BY SHORTS

     If you are going to use shorts (as opposed to ints) in your schema
     and application then do the following :

     1. Change all ints to shorts in both schema and application.
        This includes changing unsigned ints to unsigned shorts.

     2. Run the 16-bit ddlp (NOT the 32-bit ddlp) on the new schema
        to get a 16-bit dbd.  Be sure to use the same options for ddlp
        that were used to create the original dictionary.  

     3. The above steps should be sufficient assuming that no other
        changes (besides ints to shorts) are made to the schema.  If
        you have also made other changes you will need to revise the
        database as well.

     4. Your application (16-bit or 32-bit) will have to be compiled
        with the same alignment as specified in the 16-bit ddlp 
        (in the 16-bit implementations of RDM 3.30 and RDM 4.0 ddlp
        defaulted to 1-byte alignment).

     5. Now the database can be used by 16-bit applications as well
        as 32-bit applications.

     SOLUTION 2 - REVISE THE DATABASE

     If you have a pre-existing application that uses ints and you don't
     want to change ints to shorts in your application then you must use
     db_Revise to convert your database from 16-bit to 32-bit.  You will
     need to do the following:

     1. Make a copy of your database schema and change all ints to shorts
        in this copy.

     2. Run the 16-bit ddlp on the schema with shorts. 

     3. In a different directory, run the 32-bit ddlp on the schema with
        ints.  Initialize this database with the 32-bit initdb.

     4. Run the 32-bit ddlp on the db_Revise temporary database, dbwork.ddl,
        and move the dbwork.dbd file created to the same directory as the
        original database.

     5. Run the 32-bit dbrev on the two databases, with the 16-bit database
        (containing shorts) as source and the 32-bit database (containing
        ints) as destination.  This will convert the 16-bit database to a
        32-bit database, and you will be able to use ints in your 32-bit
        application.

     6. 16-bit applications will not be able to use this database unless
        they are modified.

     7. Your 32-bit application will have to be compiled with the same
        alignment as specified with the -b option in the 32-bit ddlp (in
        the 32-bit implementation of RDM 3.30 ddlp defaulted to 4-byte
        alignment, in RDM 4.5 you are forced to specify the alignment
        explicitly with the -b option).

     It is recommended that you run keybuild after completing either of
     solutions 1 or 2.

     THE "#pragma pack()" DIRECTIVE

     If you want to continue using your old 16-bit dbd file (with 1-byte
     or 2-byte alignment) and you do not want to force 1-byte or 2-byte
     alignment throughout the whole of your 32-bit application then you
     can use the #pragma pack() directive.

     In your application add the following lines, assuming that the
     database header file generated by ddlp is called "database.h":

          #pragma pack(1)         // or pack(2) for 2-byte alignment
          #include "database.h"   // header produced by ddlp
          #pragma pack()          // back to default packing

     It is recommended that you run prdbd on both your old and your new
     database dictionaries, and compare the output to ensure that the
     field positions and sizes are as expected.
     
PERFORMANCE
-----------

     Whereas in RDM 3.30 the runtime option SYNCFILES was switched off
     by default, in RDM 4.5 it is switched on by default.  This change
     may have a significant effect on the performance of your applications.

     If you have been using RDM 3.30 for Win32 with the default runtime
     options (i.e. with SYNCFILES switched off) and you port your 
     applications to RDM 4.5, again using the default runtime options
     (i.e. with SYNCFILES switched on) you may find that your applications
     run slower than they did.

     The SYNCILES option ensures that files that are updated during a
     transaction are committed to disk at the end of the transaction (i.e.
     a disk-write is forced, as opposed to the updates remaining in the OS
     write cache).  This option is required to guarantee data integrity
     in the event of a power failure or system crash.

     Apart from the effects of this change in the default runtime options,
     you should find that RDM 4.5 is slightly faster than RDM 3.30.

ALIGNMENT CHECKING
------------------

     A mechanism has been added to RDM (version 4.0 and later versions)
     which lets it check that the structure alignment of database records
     in memory (as defined by the compiler switches used when compiling
     your application) matches that of the database definition (.dbd file).
     A mismatch could result in database corruption, or in corruption of
     the application's stack or heap.  Previous versions of RDM could not
     detect such mismatches.  There is a particular danger of this problem
     if you change compilers, or share a database between 16-bit and 32-bit
     applications.
     
     To use this feature, you must register the alignment checking code
     as a callback function with the DLL, as explained in the new manual
     page for dt_set_alignment_proc().  Note that you do not have to write
     an alignment checking function yourself.  The function is supplied
     in align.h, so that it will be compiled as part of your application,
     with the same structure alignment as your application.
     
     You will need to define the following symbols in one of your source
     files prior to including "vista.h":
     
     Pre-processor symbol     Meaning
     ---------------------    ---------------------------------------------
     ALIGNMENT_CHECK          This symbol should be defined in exactly one
                              source module where it will include support
                              for alignment checking.
                              
     ALIGNMENT_PROC_NAME      The alignment checking function must be
                              given a name and then registered with the
                              RDM DLL.  This symbol must be defined along
                              with ALIGNMENT_CHECK, as well as in the module
                              that calls dt_set_alignment_proc.  See the new
                              manual page on dt_set_alignment_proc() for
                              additional details.
                              
TIMEOUTS
--------

     More control has been added to network timeouts.  The dt_timeout()
     parameter can now be specified in the rdm.ini file, and there is no
     restriction on its value.  A problem in version 3.30, that prevented
     the value from being longer than 30 seconds, has been fixed.
     
     In addition, while waiting for a lock to be granted, RDM now checks
     periodically to see that the Lock Manager is still alive.  The
     frequency of checking is specified by the new 'net_timeout' parameter
     in rdm.ini.
     
     These values are specified as follows:

          [lockmgr]
          timeout=<seconds>        ; defaults to 10 seconds
          net_timeout=<seconds>    ; defaults to 30 seconds

     In RDM 4.5 for Win32 another timeout variable has been added for
     thread synchronization.  To protect its internal, thread-specific
     data, RDM prevents multiple threads from executing its functions
     simultaneously.  It uses a Mutex to ensure that no more than one
     thread is in an RDM function at any time.

     This means that, if Thread B calls an RDM function while Thread A
     is executing an RDM function, Thread B will be blocked until Thread A
     returns.  Normally, this will only be for a fraction of a second.
     Even if Thread A is waiting for a response from the Lock Manager
     the delay should be less than a second, as RDM releases the
     Mutex while waiting for Lock Manager responses.

     The application can determine how long a thread should wait for
     the Mutex before timing out, by setting the "thread_timeout"
     parameter in rdm.ini:

          [rdm]
          thread_timeout=<seconds>    ; defaults to 30 seconds
     
DEBUGGING SUPPORT
-----------------

     RDM 4.5 contains several debugging switches to assist Raima in
     isolating any problems that could be related to the runtime library.
     They are enabled when the library or DLL is re-compiled with the
     DB_DEBUG define.
     
      THESE DEBUGGING OPTIONS ARE PROVIDED SOLELY TO ASSIST RAIMA IN ITS
      DEVELOPMENT EFFORTS, AND ARE SUBJECT TO CHANGE WITHOUT NOTICE.
                                     
     When DB_DEBUG is defined, the following runtime options become
     available by calling dt_on_opt() or by adding entries in the rdm.ini
     file in the [debug] section:
     
     Option           Description
     -----------      ----------------------------------------------------
     PZVERIFY         Upon access to a database file, the file's next_slot
                      and delete_chain values are compared against the
                      actual file size to ensure that they do not point
                      past the end of the file.  This will help detect an
                      exploding file size problem.
                      
     PAGE_CHECK       Each database page read into the cache will be
                      marked with an identifying number so that it can be
                      tracked in memory and validated upon write.  This
                      provides only minimal verification, and is not
                      expected to be of much use for a customer.
                      
     LOCK_CHECK       Upon each file-lock-protected access to a database
                      file or page in the cache, the last modified date of
                      the file is retrieved, and the last modified time in
                      the first 4 bytes of the page are read from the disk
                      to ensure that another process has not illegally
                      updated the file since it was locked.  This check
                      can detect improper lock arbitration due to
                      misnaming of the TAF file, Lock Manager, or database
                      filenames.  It will detect the common problem of
                      the Lock Manager seeming to have duplicate file
                      names when not all processes are using the TRUENAME
                      option in the same way.
                      
                      !!! WARNING !!!  This option performs disk I/O on
                      each access to the internal cache and so will
                      greatly decrease your program's performance.
                      
     CACHE_CHECK      The internal cache data structures are checked for
                      consistency at the beginning of each d_ or dt_
                      function, and after each cache reference.
                      
     In addition, DB_DEBUG also defines DB_TRACE, which enables the runtime
     library to write various trace output to a disk file.  The name of the
     file is hard-coded to be "vista.out".  The trace file has been designed
     to be shared by multiple processes on the network.  It is arbitrated
     between processes by the use of share locks, and each time a different
     DB_TASK writes to it, a line identifying the process and task is also
     written.
     
     Option           Description
     -----------      ----------------------------------------------------
     TRACE_DBERR      If this or any other trace option is on, all
                      database errors will be logged to the trace file
                      immediately prior to calling your dberr handler.
                      
     TRACE_API        Each d_ function will be written.  Since the rdm.ini
                      file is not processed until the first call to
                      d_open, any pre-open d_ function will not be traced
                      unless the DBTRACE environment variable is defined.
                      If defined, it will turn on all trace options until
                      overridden by the rdm.ini file.
                      
     TRACE_LOCKS      An entry will be written each time a file is locked
                      or freed.
                      
     Note that the trace file will be physically committed to the disk on
     each write if the SYNCFILES option is turned on (the default).  This
     prevents any output from being lost if your program terminates with a
     GP fault or a hung machine, but causes a tremendous amount of disk
     overhead.
     
     A new section has been added to the rdm.ini file for specifying the
     debug and trace options:
     
          [debug]
          pzverify=1     ; verify pgzero against filelength on each access
          page_check=1   ; verify cache page identity
          lock_check=1   ; detect unarbitrated file updates
          cache_check=1  ; verify cache consistency
          trace_dberr=1  ; output dberr messages to trace file
          trace_api=1    ; write all d_ api functions to trace file
          trace_locks=1  ; write file locks and frees to trace file
          
     These options can also be set with dt_on_opt() and dt_off_opt(),
     however, the debug options and the trace options cannot be mixed with
     each other or with any other options in the same dt_on_opt() or
     dt_off_opt() call.
     
          dt_on_opt(PZVERIFY, &Task);
          dt_on_opt(TRACE_DBERR | TRACE_API, &Task); /*must be separate call*/
     
CACHE ALLOCATION
----------------

     RDM 4.0 and later versions allocate memory for cache pages only when
     they are first needed.  Whereas RDM 3.30 used to allocate the whole
     cache as soon as the first database was opened, the default behavior
     in RDM 4.0 and RDM 4.5 is to wait till the cache is actually needed.

     If you want to force RDM to allocate all the cache memory it needs 
     immediately on the first database open (i.e., the old RDM 3.30
     behavior) you can do so.  You can switch on the runtime option
     PREALLOC_CACHE, that is, call dt_on_opt(PREALLOC_CACHE, &Task)
     in your application, or set prealloc_cache=1 in the [rdm]
     section of the rdm.ini file.

WIN32 NATIVE I/O
----------------

     The RDM DLL in RDM 4.5 for Win32 uses Win32 native I/O functions
     instead of the ANSI functions used in previous versions.  The Win32
     native functions are slightly faster.  If you wish to use the ANSI
     functions, comment out the line in vista.h which defines
     WIN32_IO and rebuild rdm45w32.dll.

     Note that in this release the RDM utilities do not use Win32 native
     I/O functions.
     
FILE COMMIT, SYNCFILES and REMOTESYNC
-------------------------------------

     When a transaction is completed, RDM must ensure that all the database
     changes have been physically written to disk before returning back to
     the application.  Provided that the RDM runtime option SYNCFILES is
     enabled (which it is by default), files will be committed to disk using
     one of two functions, _commit() or FlushFileBuffers(), at the proper
     times.  This will ensure that, by the time RDM starts updating the
     data and key files in a transaction, the log file is committed to disk.
     Likewise, when RDM has finished writing to the data and key files, it
     will commit these to disk.

     However, if the database is located on a remote drive, the commit
     functionality described above will only ensure that data is not left
     in the local machine's operating system cache.  It will not flush the
     remote machine's cache, so that if the remote machine crashes, data
     loss and database corruption may occur.

     To flush the operating system cache on a remote workstation, the
     RDM runtime option REMOTESYNC must be enabled. This is not enabled
     by default, but can be switched on either by calling

          dt_on_opt(REMOTESYNC, &Task);

     or by adding the following entry to the rdm.ini file:
     
          [rdm]
          remotesync=1

TRUENAME
--------

     The RDM Lock Manager works by maintaining a list of files that are
     used by the RDM applications communicating with it.  It is
     therefore necessary that all applications use the same names to
     identify these files.

     On a network, different workstations may use different drive letters
     to refer to the same directory on a server, or on another workstation.
     Filenames consisting of a drive letter and a path are therefore not
     suitable for identifying files to the Lock Manager.

     RDM therefore uses UNC (Universal Naming Convention) filenames by
     default.  These consist of a machine name followed by a path, in the
     form:

          \\machine_name\directory\filename

     The use of UNC filenames is enabled through the RDM runtime option
     LMC_OPT_TRUENAME, which is on by default.  Switching this option off
     will cause RDM to form filenames with drive letters instead.

     If you are accessing the same database files from 16-bit and 32-bit
     applications, we recommend you check that the applications are all
     using the same filenames.  You can do this by looking at the Lock
     Manager's list of files, and checking that each filename appears only
     once.  You can see the full path of any filename in the Lock Manager's
     list by selecting the file and clicking on the "View File" button.

PATHNAME COMPATIBILITY PROBLEMS
-------------------------------

     On Windows NT and Windows 95, directory and filenames may be longer
     than 8 characters.  Long directory and file names have "short name"
     equivalents.  For compatibility with 16-bit versions, which do not
     support long names, RDM uses the short name equivalents by default.
     This forces the names sent to the lock manager to match, the
     importance of which is discussed in the TRUENAME section above.

     During testing Raima found that in some situations the generation
     of short names did not work correctly, resulting in a return code
     of S_UNAVAIL from dt_open().  Specifically, if an application
     running on Windows NT opens a database on Windows 95 at a path
     containing spaces or long names, the short name translation will
     fail.  To cure this problem, either turn off short name translation,
     rename the directory, or define the share so as to eliminate all
     long components from the path.

     For example, on a Windows 95 workstation with a directory name of:

          c:\My Application\data

     16-bit applications, and 32-bit applications with short name
     translation on, will both translate this, assuming that the root of
     drive C: is shared as DRIVE_C, to:

          \\MACHINE\DRIVE_C\MYAPPL~1\DATA

     Unfortunately NT applications will fail to do the translation
     correctly.

     Short name translation in 32-bit applications is controlled by the
     RDM runtime option LMC_OPT_SHORTNAME.  LMC_OPT_SHORTNAME is undefined
     for 16-bit applications.  You can turn off LMC_OPT_SHORTNAME either
     by calling

          dt_off_opt(LMC_OPT_SHORTNAME, &Task);

     or by adding the following entry to the rdm.ini file:
     
          [rdm]
          shortname=0

     If you turn off LMC_OPT_SHORTNAME the directory name used is:

          \\MACHINE\DRIVE_C\MY APPLICATION\DATA

     However, if a 16-bit application (which cannot use long filenames)
     opens the database at the same time, the lock manager will think two
     different sets of files are being opened, and database corruption
     may result.

     Renaming the directory from "My Application" to "My_App" would
     also solve the problem, allowing simultaneous 16-bit and 32-bit
     access:

          \\MACHINE\DRIVE_C\MY_APP\DATA

     Finally, sharing the "My Application" directory as "APPSHARE" instead
     of the root of drive C: eliminates the long component from the path:
     
          \\MACHINE\APPSHARE\DATA

     This also allows 16-bit and 32-bit applications to use the database
     concurrently.


SHARED DIRECTORIES
------------------

     Earlier versions of RDM did not handle shared directories on Windows
     NT and Windows 95 correctly.  Under some conditions, applications
     running locally on a workstation where a database was located might
     refer to the database files with names different from those used by
     applications running on remote workstations.

     In RDM 4.5 this problem is corrected. RDM 4.5 applications accessing 
     a database in a shared directory (either on the local machine or on 
     a remote one) will correctly convert the database file names to UNC 
     format before any communication with the Lock Manager.  See example
     below:
          \\machine_name\share_name\directory\filename

     Provided you do not switch off the runtime option LMC_OPT_TRUENAME
     (which is on by default) you should find that, if you click on
     "View File" in the Lock Manager main window, the filename is always
     displayed in UNC format at the top of the window.

     If you are using 16-bit versions of RDM as well, we recommend you check
     that the versions are generating filenames in the same format.

NOVELL SEARCH MODE
------------------

     If you are on a Novell network and your application gets -905 S_NOFILE
     errors when accessing the database, make sure that the Novell search
     mode is set to 2.  This will force Novell to look where you specify,
     rather than search blindly throughout your paths and network drives.
     
COUNTRY TABLE
-------------

     Raima strongly advises that you use a country table, rather than call
     d_mapchar().  The reason is that the RDM utilities such as IDA and
     dbcheck will not be aware of the character mappings, and so will
     either be unable to find data using keys, or will update key values
     incorrectly, thereby corrupting the key files. The dbcheck utility
     in particular will report that the key files are corrupted.
     
     This also applies to using dt_on_opt(IGNORECASE, &Task).  Use the
     rdm.ini file instead:
     
          [rdm]
          ignorecase=1
     
     In either case, the key files can be repaired by running keybuild
     after you have created a country table or updated the rdm.ini file.
     However, if you have created sorted sets with this problem, you will
     have to disconnect and reconnect them all.
          
RDM.INI PATH
------------

     In version 4.0 and later versions, applications built with the RDM
     static library use the environment variable RDMINI to determine the
     path of the rdm.ini file.  This may be useful with the RDM utilities,
     since all of them now use the rdm.ini file.
     
ENVIRONMENT VARIABLES
---------------------

     Note that a DLL does not inherit the environment variables of the
     calling process, so RDMINI and all other environment variables will
     be ignored by the RDM DLL.

WIDA AND W_QUERY FAILURE TO LOCATE DBD FILE
-------------------------------------------

     If you run either of the programs wida.exe or w_query.exe on a
     Windows NT workstation, and attempt to open a database located
     in a shared directory on a Windows 95 workstation, you may find
     that you are unable to do so, because the database dictionary
     (DBD file) is not displayed in the file list of the database open
     dialog.  To correct this problem, make sure that the archive
     attribute is set on the database dictionary file.

SUMMARY OF NEW RUNTIME OPTIONS
------------------------------

     The options below were added or changed in RDM 4.5 or RDM 4.0. Each
     of these options is discussed under the appropriate product section.
     
     rdm.ini                  dt_on/off_opt
     -----------------------  ---------------------
     [rdm]
     alignment_check=0        ALIGNMENT_CHECK
     syncfiles=0              SYNCFILES      (note default value change)
     remotesync=1             REMOTESYNC
     shortname=0              LMC_OPT_SHORTNAME
     thread_timeout=<seconds>
     prealloc_cache=1         PREALLOC_CACHE

     [lockmgr]
     timeout=<seconds>
     net_timeout=<seconds>
     lana=<number>            LMC_OPT_LANA0, LMC_OPT_LANA1 etc.
     
     [db_query]
     dateFrom1970=1           QO_DATEFROM1970
     queryplan=1
     
     [db_revise]
     workpath=<path>
     
     [debug]
     pzverify=1               PZVERIFY
     page_check=1             PAGE_CHECK
     lock_check=1             LOCK_CHECK
     cache_check=1            CACHE_CHECK
     trace_dberr=1            TRACE_DBERR
     trace_api=1              TRACE_API
     trace_locks=1            TRACE_LOCKS


==========================================================================
QUERY 4.5
==========================================================================

Because the QUERY manual does not yet include 3.3 update information, the
following descriptions include changes for version 3.3 and for 4.5.

SUMMARY
-------

     1)  API changes
     2)  Required application changes
     
API CHANGES
-----------

     q_set_workingproc()
     qt_set_workingproc()
     
          Version 3.30 introduced a new call-back function that allows the
          user to see how much work has been done.  q[t]_set_workingproc()
          registers your call-back function with QUERY.  Your call-back
          function must be of the form:
          
               void QEXTERNAL_FIXED my_working(
                         unsigned long current, unsigned long maximum)
          
          where "current" is the current number of root records read for
          this select statement, and "maximum" is the maximum number of
          root records possible.  If multiple record types share the same
          file or if key optimization is done, "current" may not start at
          the beginning, or end at "maximum."  However, when the report is
          finished, the callback function will be called one last time with
          "current" set equal to "maximum" to signal the end of the report.
          If root record instances are added after the report is started,
          "current" may exceed "maximum."
          
     q[t]_on_opt()
     
          Version 3.30 introduced a new option, QO_DATEFROM1970, because
          the Microsoft C/C++ 7.0 compiler uses 1900 as the base date for
          system dates, rather than the standard of 1970.  If a database
          contains dates of this form, QUERY would misinterpret them.  By
          default this option is on, which specifies that dates are based
          on the 1970 standard.  If this option is turned off, QUERY will
          adjust all date fields in the database during comparisons and
          display.
          
     int q[t]_set_displayfunc(...)
     int q[t]_set_printerfunc(...)
     
          Version 3.30 extends these functions to every platform, not just
          Windows.  They can be used in place of calling q_nextrow() or
          q_nextval().

          Note that in version 4.5 the third parameter of both of these
          functions (the line number) is defined as an unsigned long, not
          an int, as stated in the manual.
          
     A new QUERY option has been added to the rdm.ini file that will cause
     the query optimization plan to be printed at the head of the report:
     
          [query]
          queryplan=1
     
REQUIRED APPLICATION CHANGES
----------------------------

     Passing of parameters to nested QRF's has changed.  Previously, the
     inner QRF implicitly received the parameters from the outer QRF.  Now
     the parameters to the inner QRF must be explicitly passed in from the
     outer QRF.
     
     The default callback functions q_ctod(), q_translate(), and
     q_function() have been removed from the libraries because they are
     example functions which should not be used without customization.  The
     application must use the appropriate q_set_*() function to register
     your customized version as a callback function.  Attempts to use the
     callbacks without registering them will return the error Q_PROCNEEDED.
     
     Any application source file which uses the VALUE structure must be
     recompiled.

     If you are using either of the functions:

          qt_set_displayfunc
          qt_set_printerfunc

     please note that the definition of the callback functions passed to
     both of these has been slightly changed: the third parameter (the
     line number) has been changed from an int to an unsigned long.


==========================================================================
REVISE 4.5
==========================================================================

SUMMARY
-------

     1)  16-bit to 32-bit Compatibility
     2)  Multi-user considerations
     3)  Sample database
     4)  Customizing convert.c
     5)  Duplicate field names
     6)  Dot syntax
     7)  Miscellaneous
     
16-BIT TO 32-BIT COMPATIBILITY
------------------------------

     If you are using 16-bit and 32-bit versions of REVISE, be careful
     not to mix the database dictionaries for the REVISE working database.
     dbwork.dbd.  The 16-bit and 32-bit versions of this file are different.
     Using the wrong file will probably cause REVISE to fail - even if it
     does not fail, the set connections in the destination database will 
     be invalid.

MULTI-USER CONSIDERATIONS
-------------------------

     REVISE does not perform any file locking.  The user must eliminate any
     possibility of update to the source database during the time REVISE is
     executing.
     
SAMPLE DATABASE
---------------

     In subdirectory SRC (below the DBREV subdirectory) is a sample
     database that can be used to test the installation of REVISE.  The
     database can be re-created with the following commands:
     
          cd src
          ddlp -b4 tims.ddl
          initdb tims
          dbimp tims.imp
          
     The DEST directory contains a changed schema (note the differences).
     To perform the sample revision, enter the following commands from the
     REVISE home directory:
     
          cd ..\dest
          ddlp -b4 tims.ddl
          initdb tims
          cd ..
          dbrev  -r  -v  src\tims  dest\tims
          
CUSTOMIZING CONVERT.C
---------------------

     An object code library of REVISE is supplied with this delivery.  This
     may be used to link a new REVISE containing your own customized field
     conversion function (see Section 4.4.2.2 in the REVISE User's Guide).
     The file convert.c is provided as a template for creating your own
     conversions.
     
     To build a new REVISE, you must modify convert.c, compile it, and link
     it with the DBREV.LIB appropriate to your compiler.  If you are using
     Microsoft Developer Studio 4.x, the commands would be:
     
          cl -c -Zp4 -YX -ML -I\rdm45\include -DUSE_DB_LIB convert.c
          link /OUT:dbrev.exe convert.obj dbrev.lib advapi32.lib mpr.lib
          
DUPLICATE FIELD NAMES
---------------------

     If a database contains duplicate field names (created with the -d
     option of ddlp), REVISE will now correlate the field names with the
     record names to establish a correspondence between record types in the
     source and destination databases.  Previously, the user had to
     supply a script file that would project the record onto itself and
     thus remove the duplicate field name ambiguity.
     
DOT SYNTAX
----------

     The dot syntax in a field name specification is now supported.  This
     may be necessary when specifying the field to a field conversion
     statement.
     
MISCELLANEOUS
-------------

     Doing a static revision prior to running REVISE with structural
     changes can make the revision run faster.  It will also prevent
     fields from being converted in unexpected or unwanted ways.  See
     section 4.2 in the REVISE manual for more information about static
     revisions.
     
     REVISE will use the DBDPATH environment variable for locating the
     "dbwork" database.  It can also be specified in the rdm.ini file under
     the "[db_REVISE]" section with the keyword "workpath=".
     
     As of version 3.30, database address fields that point to records
     within the same database are now automatically adjusted to account for
     the record being moved during the revise process.
     

===========================================================================
LOCK MANAGER COMMUNICATION
===========================================================================

SUMMARY
-------

     1)  Compatibility
     2)  New Command Line Switches
     3)  LMC_TCP
     4)  LMC_NETBIOS
     5)  NetBIOS Name Table
     6)  ADSTATUS Utility for NetBIOS
     7)  LMC_SPX
     8)  Lock Manager Name and IPX Address
     9)  LMC_NOVELL

COMPATIBILITY
-------------

     The 32-bit RDM 4.5 Lock Manager will recognize 16-bit RDM 4.5 and
     RDM 4.0.2 applications.  Note, however, that the 16-bit RDM 4.0.2
     Lock Manager will not recognize RDM 4.5 applications (the application
     will get an S_UNAVAIL return code when it calls dt_open).

     The 16-bit and 32-bit versions of RDM 4.5 are fully compatible with
     each other.  You can use either a 16-bit or a 32-bit RDM 4.5 lock
     manager with both 16-bit and 32-bit RDM 4.5 applications.

NEW COMMAND LINE SWITCHES
-------------------------

     The Lock Manager's command line switches have been changed in RDM
     4.5 for Win32.  The switches documented in the RDM Manuals do not
     apply to version 4.5.  The following command line switches are
     recognized by this version of the Lock Manager:

     -a  followed by lock manager name, e.g. -atest or -a test to
         specify a lock manager name of "test".  Default name is
         "lockmgr".

     -c  (NetBIOS only) followed by the number of NetBIOS NCBs to be
         used in listening for connections on each lan adapter, e.g.
         -c4 to specify 4 listen NCBs on each lan adapter.  Default
         is 2.

     -d  (SPX only) causes lock manager socket number to be allocated
         dynamically.  By default the Lock Manager listens for connections
         on IPX socket number 823f, but in case this conflicts with other
         software you can force the Lock Manager to allow SPX to allocate
         a socket number to it at runtime.

     -f  followed by the number of entries required in the Lock Manager's
         file table, e.g. -f1024 for 1024 file table entries.  This number
         should be at least as large as the total number of files in all
         databases managed concurrently by the Lock Manager.  If the file
         table is too small your application will get an S_UNAVAIL return
         code when attempting to open a database.  Default is 256.

     -l  optionally followed by a log file name, e.g. -llockmgr.txt or
         -l lockmgr.txt to make the Lock Manager output debugging
         information to a log file called "lockmgr.txt".  If no filename
         is specified, the log file will be called "lockmgr.log".

     -m  followed by b, c or s, to specify the Lock Manager protocol,
         where:
                   -mb  specifies NetBIOS,
                   -mc  specifies TCP/IP and
                   -ms  specifies SPX.

         If you use -mb you may force the Lock Manager to listen on only
         one lan adapter by specifying the lana number, e.g. -mb0 forces
         use of lana zero only; otherwise the Lock Manager will listen on
         all available lan adapters.

     -n  followed by the number of message buffers to be allocated by the
         Lock Manager, e.g. -n128 to allocate 128 buffers. Message buffers
         are required for incoming messages and for replies.  The lock
         manager will never need more message buffers than there are active
         connections.  If there are too few message buffers, the Lock Manager
         may become sluggish in responding to incoming requests.  Default
         is 32.

     -q  followed by the number of entries in the lock request queue, e.g.
         -q256 to specify a lock request queue of 256 entries.  You can see
         how many queue entries are being used at any time; this is 
         displayed in the main window.  If the queue is too small the lock
         manager will discard incoming lock requests (causing -925 errors).
         Default is 128.

     -r  (NetBIOS and SPX only) followed by the number of NetBIOS NCBs
         or SPX TCBs to be used for receiving incoming packets from each
         connection, e.g. -r4 for 4 NCBs / TCBs.  It is very unlikely that
         a value greater than 2 would ever be required, as an incoming
         message will almost never consist of more than 2 packets, and
         there can only be one message from a connection at a time.
         Default is 2.

     -s  (NetBIOS and SPX only) followed by the number of NetBIOS NCBs
         or SPX TCBs to be used for sending outgoing packets, e.g. -s32
         for 32 NCBs / TCBs.  Insufficient send NCBs / TCBs will cause
         the Lock Manager to be sluggish in responding to incoming
         requests.  Default is 16.

     -u  followed by the number of entries required in the Lock Manager's
         user table, e.g. -u32 for 32 user table entries.  This number
         should be at least the largest number of database tasks (DB_TASKs)
         concurrently using the Lock Manager.  If the user table is too small
         your application will get an S_UNAVAIL return code when attempting
         to open a database.  Default is 16.

     -x  followed by the size in bytes of the largest message buffer to be
         allocated, e.g. -x2048 to set a maximum message size of 2048 bytes.
         Most lock manager messages are less than 256 bytes long.  The
         longest messages are those sent to the remote console, WCONSOLE.EXE,
         and the size of these depends on the number of users and files that
         are active.  If the maximum message buffer is too small then -925
         errors will occur when you run the remote console.  Default is 4096.

LMC_TCP
-------

     The TCP/IP lock manager protocol is new in this release.  The
     identifier for the TCP Lock Manager is LMC_TCP, for purposes of
     the call to d_lockcomm().  The letter that indicates TCP/IP as the
     transport for the WIDA, LMW and WCONSOLE programs is 'c' thus:

          LMW -mc

     This informs LMW that it should use the TCP/IP transport.

     TCP/IP creates connections by specifying a target machine and a socket
     number.  By default, RDM uses socket number 1523 (subject to change).
     At the lock manager end, RDM on a multi-homed machine will use the
     first or primary IP address.  This cannot be altered.  RDM uses the
     default socket number unless told otherwise.  Use the -A parameter to
     LMW to alter the socket number. Precede the number with a colon.  For
     example, to start LMW as above but at socket number 777, do this:

          LMW -mc -a :777

     The name parameter is not otherwise used by the Lock Manager.

     The Lock Manager displays its own IP address in its caption bar.
     If LMW is run on a machine foo.bar.net at IP address 1.2.3.4, its
     caption bar will read as follows:

          Lock Manager - '1.2.3.4 foo.bar.net'

     If the Lock Manager is running at other than the default port, the
     port number will appear as well, thus:

          Lock Manager - '1.2.3.4:777 foo.bar.net:777'

     The name parameter at the runtime end is a bit different.  You must
     enter the machine's name as the lock manager name.  A socket number
     will also be needed if the default is not being used.  In the first
     example above, where the default port is in use, any of the following
     may be used as the lock manager name (or the -A parameter to WCONSOLE
     or WIDA):

          1.2.3.4
          foo.bar.net
          foo           (assuming the runtime is also somewhere in bar.net)

     The WIDA command lines would then be as follows:

          WIDA -mc -a 1.2.3.4
          WIDA -mc -a foo.bar.net
          WIDA -mc -a foo                  (same assumption as above)

     If the socket was altered to 777 as described earlier in this example,
     the lock manager name would need to be altered as follows:

          1.2.3.4:777
          foo.bar.net:777
          foo:777       (assuming the runtime is also somewhere in bar.net)

     And similarly, the WIDA command line would change as follows:

          WIDA -mc -a 1.2.3.4:777
          WIDA -mc -a foo.bar.net:777
          WIDA -mc -a foo:777              (same assumption as above)

     While each of these choices, dotted decimal address (1.2.3.4),
     fully qualified domain name (foo.bar.net), or machine name (foo)
     will work, you cannot mix them.  That is, you cannot use dotted
     decimal from one machine and FQDN from another.  If you do, you
     will get a -944 TAF-lockmgr synchronization error.  This is because
     lock manager names written to the TAF files will not match.  Choose
     one representation for the lock manager name and stick to it.
     Dotted decimal may be a tiny bit faster, but if your IP addresses
     are allocated dynamically, they may not be a practical alternative.

LMC_NETBIOS
-----------

     Previous versions of RDM only used NetBIOS lan adapter zero (lana 0).
     Under Windows NT and Windows 95 there are typically several (virtual)
     lan adapters, one for each protocol that has a binding to NetBIOS.
     Under Windows NT you can set the lana number for each protocol.  Under
     Windows 95 you can determine which protocol uses lana 0, as the default
     protocol will always be assigned to lana 0, but otherwise you have no
     control over lana numbers.

     In this release of RDM the NetBIOS Lock Manager listens for connections
     on all available lan adapters.  Therefore, you may connect to the same
     lock manager with multiple applications using different protocols.  You
     can see which lana number each connection is using by clicking on "View
     User" in the Lock Manager main window.  Also, you can start the Lock
     Manager with the -l switch to get it to output debugging information to
     a log file, and this debugging information will include the lana number
     for each connection.

     Because there is a small processing overhead associated with handling
     multiple lan adapters, you can force the Lock Manager to listen on one
     lan adapter only, provided you know which lana number will be used. For
     example, to force the Lock Manager to listen on lana number 3 only, use
     the -mb switch as follows:

          lmw -mb3

     The RDM DLL always only uses one lana number.  By default it uses
     lana 0, but you can force it to use any valid lana number by calling
     dt_on_opt(), or by adding an entry to the rdm.ini file.  For example,
     to force the DLL to use lana 3, do one of the following:

     1. Add this call to your code (before dt_open):

          dt_on_opt(LMC_OPT_LANA3, &Task);

     or

     2. Add the following lines to your rdm.ini file:

          [lockmgr]
          lana=3

     Remember that the second method will cause all RDM applications that
     read the rdm.ini file to use lana 3.  If you use the first method
     above, you will find that constants are defined in vista.h for lana
     numbers 0 to 7, but RDM will accept any lana number from 0 to 254.

NETBIOS NAME TABLE
------------------

     If you are using the NetBIOS Lock Manager, opening a database in
     shared mode will cause your DBUSERID to be added to the NetBIOS
     adapter's name table.  Closing the database, however, will not remove
     it from the name table.  This is because adding a name to the name
     table is very slow, and applications that repeatedly open and close
     databases, using the same DBUSERID, would run noticeably slower if the
     DBUSERID were added to, and removed from, the name table each time.
     
     However, you may sometimes experience S_DUPUSERID (-921) errors when
     attempting to reuse the same DBUSERID on a different workstation after
     it is no longer in use on the first.  You can solve this problem by
     calling dt_on_opt(LMC_OPT_DELNAME, &Task) in your application, or by
     setting delname=1 in the [rdm] section of the rdm.ini file.  This will
     cause the DBUSERID to be removed from the NetBIOS adapter's name table
     when the last database is closed.

     Defining a DBUSERID the same as the computer name of the Windows NT
     or Windows 95 workstation where the application is running will cause
     an error -925 (S_LMCERROR).  Likewise, defining the lock manager name
     the same as the computer name of the workstation where it is running
     will cause the following error message:

          NetBIOS NCBADDNAME failed (lana 0) - error D

ADSTATUS UTILITY FOR NETBIOS
----------------------------

     RDM 4.5 includes a new version of the adstatus utility (first included
     in Version 3.30 of RDM).  This program can be used to display the
     contents of the NetBIOS name table.  Note that it does not communicate
     with the Lock Manager, nor does it use the RDM DLL or any other
     components of RDM.

     You can use adstatus to display any name in the NetBIOS name table -
     these include the RDM Lock Manager name and dbuserid(s), as well as
     your computer name and other operating system entities.  On Windows 95
     you can also use adstatus to see the status of NetBIOS sessions, and
     to delete NetBIOS names that are not in use (these features do not work
     on Windows NT).

     The command line options are as follows:

          adstatus [/H] [/A name] [/Cn] [/Nn] [/R s n] [/D name]

            /H        = Prints command line options and quits.
            /A <name> = Prints status of NetBIOS name on the network.
            /Cn       = Continuous polling every 'n' seconds
                        (Defaults to 4 seconds).
            /Nn       = Specifies the LAN adapter number
                        (Defaults to all LAN adapters).
            /R s n    = Reset the LAN adapter with 's' = sessions,
                        'n' = names (By default the LAN adapter is
                        reset without specifying number of names and
                        sessions).
            /D <name> = Deletes the specified name from all local LAN
                        adapters.

     Note that adstatus will not display the contents of the name table
     unless you use the /A option.  If the specified name is found in the
     NetBIOS name table, the whole name table will be displayed.

LMC_SPX
-------

     Note that the lock manager name is case-insensitive when you use
     the SPX Lock Manager. With all other lock manager types, the name
     is case-sensitive.

     To use the LMC_SPX lock manager type you must have Novell's Client32
     loaded on all workstations which run the Lock Manager or use the RDM
     DLL. Novell's Client32 is available on Novell's web site for both
     Windows NT and Windows 95.

     To build the RDM DLL, if you want to include the SPX module, please
     make sure that you define the environment variable NOVELL before
     running the batch file mkdll.bat:

          set NOVELL=1
          mkdll msvc \rdm45

     You will need to have the Novell SDK to build the RDM DLL if you wish
     to include the SPX module.  RDM for Win32 uses Novell's SIPX 32 API.

LOCK MANAGER NAME AND IPX ADDRESS
---------------------------------

     The Lock Manager displays its own IPX address when it runs, along with
     its name.  Your application can use the address as an alternative to
     the lock manager name, to specify a particular lock manager to connect
     to.

     The IPX address consists of three parts, a network number, node number
     and socket number.  For example, if you start the Lock Manager with
     name "TEST", you might find that it displayed its name and address as
     below:

          TEST@1.c0bd4fc4.823f

     The IPX address is prefixed by an @ character.  If you use the IPX
     address to identify the Lock Manager the socket number does not
     normally need to be supplied, since it defaults to 823f.  In your
     application, you could identify this lock manager by IPX address as
     follows:

          d_lockmgr("@1.c0bd4fc4");

     or

          d_lockmgr("@1.c0bd4fc4.823f");

     The @ symbol tells RDM that you are using an address, not a name.
     Either of the above function calls will be equivalent to:

          d_lockmgr("TEST");

     The same convention applies to lock manager names specified in the
     rdm.ini file, e.g.

          [lockmgr]
          name=@1.c0bd4fc4
    
     The socket number used by the RDM Lock Manager is 823f by default. 
     This is a pre-assigned constant reserved by Novell for use by Raima. 
     Use of this constant implies that only one SPX lock manager can be 
     running on a workstation, as the IPX address of each must be unique 
     across the network.

     The lock manager command line switch -d allows you to run more than
     one lock manager on one workstation, by dynamically allocating socket
     numbers instead of using the constant described above.  For example, 
     you could start two SPX lock managers with the following command lines:

          lmw -a lockmgr1            Uses socket number hex 823f
          lmw -a lockmgr2 -d         Socket number determined at runtime
 
LMC_NOVELL
----------

     The Novell Lock Manager, LMC_NOVELL, was added in Version 3.30 of
     Raima Database Manager, although it was not supported in RDM 3.30
     for Win32.  The Novell Lock Manager uses Novell's native locking and
     the Transaction Tracking System (TTS).  No lock manager program is
     required if you use LMC_NOVELL.
     
     Use of LMC_NOVELL requires that TTS be enabled on the file server.
     RDM turns off implicit transactions on the server.  It also flags the
     database files as transactional, if this has not already been done.

     To use the LMC_NOVELL lock manager type you must have Novell's
     Client32 loaded on all workstations that use the RDM DLL.  Novell's
     Client32 is available on Novell's web site for both Windows NT and
     Windows 95.

     To build the RDM DLL, if you want to include the LMC_NOVELL module,
     please ensure that you define the environment variable NOVELL before
     running the batch file mkdll.bat:

          set NOVELL=1
          mkdll msvc \rdm45

     You will need the Novell SDK to build the RDM DLL if you wish
     to include the LMC_NOVELL module.
     

==========================================================================
OBJECT MANAGER
==========================================================================

COMPATIBILITY
-------------

     ROM 3.0 is compatible with RDM 4.5.  However, you must recompile ROM,
     and, under Windows, change the makefiles to reference the new DLL
     name.
     

==========================================================================
BUG FIXES
==========================================================================

SUMMARY
-------

     1)  Version nomenclature
     2)  Bugs
     3)  Raima Database Manager
     4)  Query
     5)  Revise
     6)  Lock Manager
     
VERSION NOMENCLATURE
--------------------

     The Raima product release is identified by a version, revision, patch,
     and build number.

     The version number is incremented when the product is being released
     across all supported platforms.  For example, the current version is 
     '4' due to our intent to release across DOS, Windows, Win32, UNIX, 
     QNX, and OS/2.

     The revision number is incremented when new features are added, and as
     we release each additional platform.  '.0' is the base release for DOS 
     and 16-bit Windows.

     The patch number is incremented when we incorporate bug fixes as part 
     of the released product.

     The build number is continuously incremented.  Bug fixes on our
     FTP site may appear in advance of their incorporation in the
     version being shipped on physical media.  Each bug fix build is
     identified by a different build number.

BUGS
----

     The remaining sections list, by product version number, RDM bugs that
     were found and fixed.  The bugs listed under each version number were
     fixed in that version, e.g., those under the heading [4.5] were fixed
     in RDM 4.5.

     Please note that some of these bugs may refer to specific operating
     systems or platforms.

RAIMA DATABASE MANAGER
----------------------

[4.5]

     Auto-recovery for the General and Internal Lock Managers now works
     correctly.

     A command line switch has been added to dbcheck to ignore key
     files (it will not test for their existence).

     The dbcheck utility now checks that keys defined as unique do not have
     any duplicate values.

     The dbcheck utility now checks that there are no "orphaned" set
     members.  That is, set members that are not in any set instance's linked
     list, but whose member pointers indicate that they are connected to an
     owner.

     When dbcheck is invoked and a file list is specified on the command
     line, all other files are now ignored (dbcheck does not test for
     their existence).

     The keybuild utility now correctly reports the number of records
     processed after 9,999,999 records have processed in a given file.

     Setting the TAF or LOG file path in wida.exe now works correctly.

     The wida.exe file will no longer crash if it runs out of memory.

     Writing to a database with and without transactions from the same
     DB_TASK no longer will cause database corruption.

     Failure of the read() function is now handled correctly.  Previously,
     read failures (e.g. due to network errors or media errors) were
     sometimes followed by -901 errors or database corruption.

     Invoking dbimp with its full pathname will no longer cause it to
     display an "invalid option" error.

     The command line switches for dbimp were not handled as stated in
     the manual.  The switches -s, -e, -k, and -p now work as stated.

     If dt_iopen() failed because of insufficient memory, it did not
     clean up correctly, possibly causing protection faults / core dumps 
     afterwards.  This has been fixed.

     The functions dt_csmwrite() and dt_csowrite() will no longer cause a
     protection fault / core dump on 32-bit platforms.

     The functions dt_crtype(), dt_findnm(), dt_findpm(), dt_findlm() and
     dt_findfm() will now test whether the current record was deleted, and
     will return S_DELETED if it was.

     The 8.3 file format restriction has been removed (for Win32).

     When a record lock bit was set outside a transaction, the data file
     was not committed.  Thus the lock might not be seen by applications
     running on other workstations.  This has been fixed.

     Using a cache of more than 512 pages will no longer cause RDM to
     generate an error -901 "Invalid LRU Page" or a protection fault.

     If the RDM library is built with debugging code included it will no
     longer generate spurious -901 "IX-cache not reset after trans" errors.

     If the TRLOGGING runtime option was switched off a log file was still
     created, and database errors might also occur.  This has been fixed.

     If the current owner of a set and the current member were inconsistent
     (i.e. the current member of the set did not point to the current owner)
     the functions dt_findnm() and dt_findpm() returned an error -902 
     (S_FAULT).  It now returns the correct error code 10 (S_SETCLASH).

[4.0.2 Build 10]

     Closing a database with dt_iclose() might be followed by -901 errors
     under some conditions.  This no longer occurs.

     The dbcheck utility will no longer output multiple -901 errors while
     processing data files containing more than 130K records.

     Auto recovery now works in RDM 4.0 and 4.0.1

[4.0.1 Build 8]

     Opening a local database under Windows for Workgroups did not work
     correctly if the database was being shared using "share as" and the
     share name did not include a complete path to the files.  This has
     been fixed.

     The dbcheck utility sometimes failed on data files containing more than
     130K records.  The symptoms included hanging, spurious reports of
     "Record deleted, but not on delete chain", and -904 Out Of Memory
      errors.  This has been fixed.

     Under Windows, the General Lock Manager caused occasional GP faults
     and unusual behavior indicative of memory overwrites.  This has been
     traced to the use of the C library functions stat() and ftime(), which
     were corrupting the runtime memory.  Although the documentation, states
     otherwise, these functions are not safe to use from a DLL as they
     require SS=DS.

     When a literal string was passed to d_keyfind(), RDM could access memory
     beyond the end of the string, thus causing memory faults in protected
     mode.  This occurred because RDM was using the field width defined in
     the schema instead of looking for the null terminator.  This same memory
     access problem also occurred for d_crwrite() and d_fillnew().  The
     problem is now fixed for all three functions.

     When a string field was not null-terminated and was used as a descending
     field in a compound key, then a null was placed in the last byte.
     This would lead to duplicate keys if the strings were the same
     in all but the last byte.  This problem is fixed.
     WARNING:  If your database contains duplicate keys of the type
     described, it will be necessary to run keybuild on the database.

     A new API function, d_rdmver(), was added to access the runtime 
     library/DLL version information.

[4.0]

     Improvements have been made in low level file I/O for out-of-disk
     conditions, TTS server busy, and Windows error handling.
     
     Opening a database in one-user mode now ignores any lock manager
     settings in environment variables and in the rdm.ini file.
     
     Passing in a NULL pointer to the d_rdmini() function will now prevent
     use of an rdm.ini file.  Note that by default, RDM uses an rdm.ini
     file in the current working directory, or the directory specified by
     the RDMINI environment variable.

     Most database options cannot be changed once a database has been
     opened.  Those options that can be changed with the database opened
     are DCHAINUSE, TXTEST, IGNORECASE, and CLOSEFILES.  The dt_on_opt
     and dt_off_opt functions now return S_DBOPEN if any changes are
     attempted other than the four listed here.
     
     The d_keylock function will no longer fail if the number of fields in
     all open databases exceeded 2767.
     
     Negative values for key fields were inserted into the key file
     incorrectly in some 3.30 patch releases.  This was fixed.
     
     Version 3.30 of ddlp ignored unique and optional attributes on struct
     keys.  This caused it to generate an incorrect dictionary.  A frequent
     symptom was that duplicate values could be added to the database.  This
     has been fixed.

     If either LMCLEAR or the remote console was first to connect to the
     Lock Manager, they attempted to do external recovery but were unable to
     do so.  This was the case for all lock managers other than General Lock
     Manager.  Neither LMCLEAR nor the remote console should attempt external
     recovery.  They no longer try.

     Doing incremental opens with databases that contain a SYSTEM record
     will no longer cause currency problems.
     
     WIDA will now use an rdm.ini file located in the directory of the
     database being opened.
     
     The DOS console now displays errors on the bottom line of the screen,
     and indicates if the lock manager connection has been dropped.
     
     Truename strings maintained in the Lock Manager use prefix compression
     to minimize memory space.
     
     Versions of RDM since 3.21a have used the srand() and rand() C runtime
     library functions.  This could cause problems for applications that
     need a repeatable random sequence.  RDM now implements its own rand()
     function.
     
     Under Windows, a filename with no path causes a search for that file.
     The search looks in the current directory, the Windows directory, the
     Windows system directory, the executable directory, all the directories
     in the PATH, then all the mapped network drives.  It often happened that
     a log, TAF, or DBL file with that filename existed in one of the
     searched directories, but was most likely not the correct one.  This led
     to many bizarre and difficult to isolate problems.  RDM now precedes all
     relative files with a '.\ ' thus preventing the search.
     
     The Windows DLL now correctly handles files in the root directory.
     
     The rdm.ini file now ignores white space around the '=', and is
     indifferent to the case of the word "ignorecase".
     
     d_initfile no longer generates spurious S_NOFILE errors.
     
QUERY
-----

[4.5]

     SQL statements containing mixed signed and unsigned numeric
     field types were not handled correctly.  These often caused
     errors such as "invalid expression" or "function type mismatch".

     Double-quotes inside SQL strings were not allowed.  These are now
     allowed, provided they are preceded by a backslash, as in C.

     Switching on the READONLY runtime option caused a -939 error if
     Query was used.

     The Query library did not process the rdm.ini file correctly.

[4.0.2 Build 10]

     Spurious '*' characters were inserted at the start and end of field
     values generated from "values are" statements.

     Output was not sorted correctly when select distinct was used.

[4.0.1 Build 8]

     Opening in shared mode with LMC_GENERAL sometimes caused GP faults.

[4.0]

     Output of database fields containing percent (%) characters now works.
     NOTE:  If you did a work-around for this problem by putting two
     percent characters in your data, this fix will cause them both to be
     printed.
     
     Some problems experienced with long reports have been fixed.
     
     Footers did not always print correctly.
     
     Printing to file NULL did not work.
     
     Under Windows, the check for the QUERY DLL has been improved so that
     multiple errors are no longer generated.
     
     The QUERY work database is now checked for valid alignment.
     
     Several problems relating to date display were fixed.
     
     Comparisons using a "like" clause were not using the country table.
     
     Opening in shared mode now causes all queries to use the sort/group
     database to ensure cursor stability.  Without this, simultaneous
     changes by another process could cause incorrect results, or even
     GP faults.
     
     A bug in locking caused the wrong files to be locked when retries were
     required.
     
     There was a bug in navigating a path through a NULL database address
     field.
     
     QUERY occasionally experienced a divide by zero.
     
     Doing coded values on a binary array now works.
     
     Database address fields sometimes displayed incorrectly when doing a
     "group by" operation.
     
     Column expressions can now handle "translate using" of a string.
     
     Database address fields did not always compare correctly.
     
     Database addresses can now be displayed as an int or long.
     
     Unsigned long constants larger than LONG_MAX compared incorrectly.
     
     Escape characters in strings ("\") in query statements are now
     processed according to standard C rules.  However, file path strings
     will accept either single or double backslashes.
     
     Comparing values against INT_MAX gave different results under
     different compilers.
     
     Record, field, or set names >= 31 characters in length caused the
     dictionary to be scrambled in memory.
     
     The QUERY temporary files no longer need to be TTS flagged when using
     LMC_NOVELL.
     
     The report output file is now closed on a QBREAK.
     
     An int or long displayed as money did not sort correctly.
     
     Foreign character mappings and case-insensitive sorting were not used
     in evaluating "where" clauses in SQL "select" statements.
     
REVISE
------

[4.5]

     The dbrev executable would crash or hang when revising a database with no
     sets.

[4.0]

     Creating a new optional key field did not work correctly.
     
     Only the first element of an arrayed struct was being copied over.
     
     Newly added timestamped records or sets are initialized to 1.
     
     Converting a string field to a numeric field did not work for empty
     strings.
     
LOCK MANAGER
------------

[4.5]

     The number of users supported by lmw.exe was limited to 18, regardless
     of the value specified on the command line in the -u switch.

     The function dt_lmstat() always returned a status of 0 for any user,
     when used with the SPX or NetBIOS Lock Managers.

     For the SPX and NetBIOS Lock Managers, lmclear.exe did not work correctly
     (because the function dt_lmstat() did not work correctly).

     With wconsole.exe, dead users could not be "killed" when used
     with the General Lock Manager, as the "Kill User" button was
     disabled.  It has now been enabled.

     If you try to quit from Windows while the Lock Manager is running,
     a warning dialog appears, asking if you really want to quit. If you
     clicked on "No" (do not quit) this caused a protection fault in the
     Lock Manager later.

     Repeated unsuccessful attempts to open a database using the SPX
     Lock Manager would eventually cause -925 errors instead of the
     appropriate error codes (e.g. -921 for duplicate userid).

     When the LMC_NOVELL Lock Manager was used, and a record lock bit
     was set on a record, then a transaction began, and a write lock could
     not be placed on the file that contained that record.

     If the LMC_NOVELL Lock Manager was used, dt_open or dt_iopen might
     return an error -925 (S_LMCERROR) instead of a return code 5 (S_UNAVAIL)
     if the database was locked.

     The NetBIOS Lock Manager caused a -925 timeout error if a very long
     message packet was received (e.g. from wconsole.exe).

     The General Lock Manager would try to lock the DBL file for an
     excessively long time before timing out.  Retrying is now limited
     to the time specified in the net_timeout parameter in rdm.ini.

     On failure to lock the DBL file, the error message displayed by the
     General Lock Manager said "Unable to unlock DBL file".

     In RDM 3.30 for Win32, if dt_checkid() was called before dt_open
     using the General Lock Manager, errors -907 and -914 might occur (due 
     to failure to lock the DBL file).

     In RDM 3.30 for Win32, if dt_open() failed with an error -921, using
     the General Lock Manager, errors -907 and -914 might occur in other
     processes (due to failure to unlock the DBL file).

     In RDM 3.30 for Win32, if dt_checkid() was called when the General 
     Lock Manager was being used, RDM might cause a protection fault (due
     to failure to commit the DBL file).

     In RDM 3.30 for Win32, protection faults might occur during dt_close()
     if the General or Internal Lock Managers were used.

     In RDM 3.30 for Win32 the semaphore name used for thread synchronization
     was the same for all processes.  Thus multiple processes running on
     the same workstation might delay each other unnecessarily.

     The Internal Lock Manager miscalculated the amount of memory required
     for certain bitmaps, and allocated too much memory.

     The Lock Manager sometimes displayed the wrong user name, or a blank
     name, in its file list (when displaying users with queued lock requests).

     The Lock Manager might cause a protection fault if multiple TAF files
     were used.

[4.0.2 Build 10]

     Using the LMC_NOVELL Lock Manager, read-locking data in a DOS
     application actually caused the data to be write-locked if a VLM
     driver was being used.  This problem did not affect Netx drivers,
     and did not affect Windows applications.
     (Since the DBD file is read-locked under LMC_NOVELL when you open a
     database in shared mode, DOS applications might get S_UNAVAIL return
     codes from d_open because of the above error).

     The LMC_NOVELL Lock Manager freed all locks whenever any lock was
     freed.  This problem affected both DOS and Windows applications.

     The Novell DLLs supplied with RDM 4.0 and 4.0.1 were not current,
     and might cause errors when used with the LMC_NOVELL Lock Manager.

     The LMC_SPX Lock Manager and runtime could not communicate across
     routers. On Novell networks where the Lock Manager was on one side of
     a router, and the application on the other side, the application
     would get an error -920 from d_open.

[4.0]

     The console no longer requires "share.exe".
     
     The SPX Lock Manager now checks for another of the same name.
     
     The NetBIOS and SPX Lock Managers can handle slightly more users and
     files, and should be much more robust under high stress.
     
     Under version 3.30, the TSR Lock Managers were not handling all the
     MS-DOS reentrancy issues correctly.  This has been fixed.
     
     The TSR Lock Managers have a new -I option that will display a
     "working" indicator at the top right of the screen:  a "+" symbol will
     be flashed whenever a message is processed.  Note that the indicator
     is displayed using INT 10h services, therefore it could conflict with
     non-standard video drivers.
     
     The Lock Manager can only be shutdown using SHUTDOWN, not QUIT.
     
     SHUTDOWN from the remote console now works.
     
     The Lock Manager now verifies that each user of a database is using
     the same TAF file.  This is needed to ensure that a database is not
     being arbitrated by multiple lock managers, which could lead to
     database corruption.
     
     Setting the -u, -f, -r, or -q parameters too high for the Lock Manager
     gave no warning or error, but destroyed memory.
     
     Some versions of the 3.30 lock managers did not prevent other users
     from accessing an exclusively locked database.
     
     The Lock Manager sometimes corrupted memory if a user received an LMC
     timeout and then tried again.
     
     The General Lock Manager did not lock the DBL file during a
     d_checkid() function.  When several users attempted to open the
     database simultaneously, the first user would hang, and the rest would
     time out.
     
     The "-r" parameter to the lock manager console did not work.
     
     The Windows Lock Manager no longer GPF's if the display is showing a
     user or file detail and that entry is removed (because the user or
     last user of the file logs off).
     
     Using more than one remote console against a lock manager now works.
     
     Network filenames longer than 48 characters are no longer scrambled
     by the remote console.
     
     The Internal and General Lock Managers sometimes tried to free a NULL
     pointer.
     
     The remote console has been made more robust in handling out-of-memory
     and communications errors.
     

==========================================================================
END OF FILE
==========================================================================

