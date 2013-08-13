/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * time_rng() times the CURRENT random number generator specified by
 * the global rng.  Truthfully, most of the stuff below is unnecessary,
 * as rgb_timing() isn't a pvalue-generating test.  I'll rewrite it
 * eventually, but it works fine as is for now.
 *========================================================================
 */

#include "dieharder.h"

void time_rng()
{

 /*
  * Declare the results struct.
  */
 Rgb_Timing timing;
 Test **rgb_timing_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 rgb_timing_test = create_test(&rgb_timing_dtest,tsamples,psamples);

 /*
  * Call the actual test that fills in the results struct.
  */
 rgb_timing(rgb_timing_test,&timing);

 /*
  * Save this for display in 
  */
 rng_avg_time_nsec = timing.avg_time_nsec;
 rng_rands_per_second = timing.rands_per_sec;

 destroy_test(&rgb_timing_dtest,rgb_timing_test);

}
