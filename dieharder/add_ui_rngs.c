/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"

/*
 * ========================================================================
 * This adds JUST the one example generator included with the dieharder
 * sources.  It is provided only to give people a template to follow to
 * add their own rng inside a GSL/dieharder-compatible testing harness.
 * ========================================================================
 */

void add_ui_rngs()
{

 int i;

 /*
  * If you are hacking dieharder to add your own rng, add it below using
  * the "empty_random" example as a template (right AFTER the example) and
  * it should "just appear" in the dieharder list of available generators
  * and be immediately useful.  Note that the range 600-699 is currently
  * reserved "only" for UI rngs.
  */
 i = 600;
 dh_num_user_rngs = 0;
 /* Template from here.  Uncomment and fill in your own generator:
 dh_rng_types[i] = gsl_rng_empty_random;
 i++;
  */
 dh_num_user_rngs++;
 dh_num_rngs++;
 /* to here.*/
 MYDEBUG(D_STARTUP){
   printf("# startup:  Found %u user interface generators.\n",dh_num_user_rngs);
   printf("# startup:  A total of %u generators available.\n",dh_num_rngs);
 }

 /*
  * If you are writing an entirely new UI, e.g. to add dieharder
  * functionality to things such as octave or a new GUI, yes, there is
  * a reserved range for you as well -- 700-999 is for custom UI rngs that
  * don't fit into any of the categories above.  Just block copy the
  * enter user example and set i = 700 to start and increment the
  * global dh_num_reserved_rngs counter.
  */
  
}

