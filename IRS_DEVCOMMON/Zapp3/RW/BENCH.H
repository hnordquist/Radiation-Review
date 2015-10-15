#ifndef __RWBENCH_H__
#define __RWBENCH_H__

/*
 * RWBench:  A class to help in the running of benchmarks
 *
 * $Id: bench.h@#/main/12  07/18/96 04:15:21  hart (TOOLS707_WIN32_971021)
 *
 * (c) Copyright 1989-1997 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 * To use, derive a class from RWBench including a doLoop(unsigned long) 
 * function, and a what() function (if you plan to use report()).  
 * The doLoop(unsigned long N) function should perform N operations of
 * the type you are trying to benchmark.  RWBench will call this function
 * over and over again until "duration" time has elapsed.  Then it will sum
 * the total number of operations performed.
 *
 * To run, construct an object then call go().  Then call report() to get
 * a summary.  You can call ops() outerLoops(), etc. for more detail.
 *
 * If you wish to correct for overhead, then provide an idleLoop() function
 * which does non-benchmark related calculations.
 *
 ***************************************************************************
 *
 * $Log: bench.h,v $
 * Revision 7.8  1996/07/18 04:15:21  hart
 * ObjectStore changes
 *
 * Revision 7.7  1996/05/08 18:51:02  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.6  1996/02/17 23:13:37  griswolf
 * Replace all tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.5  1995/09/05 22:01:44  jims
 * Change tlyrs.h to tlyrs.str
 *
 * Revision 7.4  1995/07/07  23:13:01  pearson
 * Added destructor to clean up memory allocated for machine_
 *
 * Revision 7.2  1995/07/03  10:32:02  pearson
 * (#1792) Overloaded 'parse' adding parse(char*) to support Windows
 * programmers.
 *
 * Revision 7.1  94/10/16  00:31:00  00:31:00  josh
 * Merged 6.1 and 7.0 development trees
 * 
 * Revision 6.2  1994/07/12  18:45:56  vriezen
 * Update Copyright notice
 *
 * Revision 6.1  1994/04/15  19:09:22  vriezen
 * Move all files to 6.1
 *
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.2   17 Mar 1992 11:31:22   KEFFER
 * 
 *    Rev 1.0   10 Feb 1992 18:54:06   KEFFER
 * Initial revision.
 */

#include "rw/timer.h"

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWBench RWBehavior {

public:

  // Default: 5 second test, one inner loop op, no machine name.
  RWBench(double duration = 5, RW4Byte ILO=1000, const char* machine = 0);

  // Deletes memory allocated for machine_
  ~RWBench();            
   
  void           parse(const char *args);   // A 'parse' for Windows users - calls parse(int, char*[])
  virtual void   parse(int argc, char* argv[]);
  virtual void   go();                      // Run the benchmark
  virtual void   doLoop(RW4Byte n)=0;       // User supplied: should execute the inner loop n times
  virtual void   idleLoop(RW4Byte n);       // To calculate looping overhead
  virtual void   what(ostream&) const;      // Print out what we're doing --- used by report()
  void           where(ostream&) const;     // Print out the machine type and the compiler
  virtual void   report(ostream&) const;    // Print out #reps and speed
  double         setDuration(double);       // Set test duration to something else
  RW4Byte        setInnerLoops(RW4Byte);    // Set # inner loops to something else
  double         duration() const           {return timeToTest_;}
  const char *   machine() const            {return machine_;   }
  RW4Byte        innerLoops() const         {return innerLoops_;}
  double         time() const;              // time to execute in seconds
  RW4Byte        outerLoops() const;        // Number of times the inner loop was executed
  double         ops() const;               // Number of operations performed
  double         opsRate() const;           // Number of operations per second
  double         kiloOpsRate() const;       // Number of thousands of ops per second
  double         megaOpsRate() const;       // Number of millions of ops per second

private:

  char*          machine_;       // What machine we're running on
  double         timeToTest_;    // How long should the test take
  RW4Byte        innerLoops_;    // Number of inner loop operations to be done
  RW4Byte        outerLoops_;    // Number of outer loops actually executed
  double         delta_;         // Actual time (corrected for overhead)
  
  RW_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWBENCH_H__ */
