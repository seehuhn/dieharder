/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"

/*
 * ========================================================================
 * This adds JUST the one example test included with the dieharder
 * sources.  It is provided only to give people a template to follow to
 * add their own test(s) to dieharder.  Note well, however, that this is
 * not a trivial process.
 * ========================================================================
 */

void add_ui_tests()
{

 int i;

 /*
  * If you are hacking dieharder to add your own test, add it below using
  * the "empty_random" example as a template (right AFTER the example) and
  * it should "just appear" in the dieharder list of available generators
  * and be immediately useful.  Note that the range 600-699 is currently
  * reserved "only" for UI rngs.
  */
 i = 600;
 dh_num_user_tests = 0;
 ADD_TEST(&user_template_dtest);
 dh_num_user_tests++;

 /*
  * Please do not exceed the i=600-899 range.  900 and higher are reserved
  * for test development at this time and are not looped over in
  * run_all_tests().
  */

 /*
  * This is the total number of DOCUMENTED tests reported back to the UIs.
  * Note that dh_num_[diehard,sts,other]_tests are counted up by
  * dieharder_test_types(), which also sets this variable (so they can be
  * called in either order).
  */
 dh_num_tests = dh_num_diehard_tests + dh_num_sts_tests + dh_num_other_tests
                + dh_num_user_tests;

 MYDEBUG(D_STARTUP){
   printf("# startup:  Found %u user interface generators.\n",dh_num_user_tests);
   printf("# startup:  A total of %u generators available.\n",dh_num_tests);
 }

}

