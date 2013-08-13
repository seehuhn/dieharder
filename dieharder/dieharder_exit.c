/*
 *========================================================================
 * $Id: work.c 320 2007-05-21 13:34:44Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"

/*
 *========================================================================
 * We need a wrapper for exit() because we cannot call it in R.
 * We also very likely will need to do some cleanup on exit that
 * didn't bother with in dieharder on a single pass CL program --
 * somebody might well run dieharder in a loop and leak otherwise.
 *========================================================================
 */

void Exit(int exitcode) {

#if !defined(RDIEHARDER)
  exit(exitcode);
#endif  /* !defined(RDIEHARDER) */

 /* Add any e.g. free() statements below */

}

