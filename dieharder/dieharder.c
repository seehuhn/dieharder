/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"


/*
 * For Command Line Interface (CLI) dieharder, main() is just a shell for
 * routines that parse the command line, do all the requested work, and
 * then exit cleanly.  Interactive User Interfaces (UIs) have an internal
 * interactive loop.  The initialization commands are therefore split up
 * so that things that must be done once per invocation of the binary
 * itself are cleanly distinct from things that must be done once per
 * selection of rng to be tested, that are in turn distinct from things
 * that must be done once per test.  See comments around each call below.
 */
int main(int argc, char *argv[]) 
{

 /*
  * set_globals picks the default values that control the CLI/UI (not the
  * tests, which have internal defaults).  If you are writing your own UI,
  * replace or patch set_globals with your own choices if desired.  Note
  * that some of the variables set control e.g. dieharder stdout output
  * options that will likely be irrelevant to a GUI or tool interface.
  */
 set_globals();

 /*
  * parsecl executes CLI-specific code that overrides the defaults set by
  * set_globals with choices selected via command line options.  A UI
  * should almost certainly just reset the global variables directly and
  * not use parsecl().  parsecl() (and the options available to the CLI)
  * is subject to change without notice as the CLI evolves, but the global
  * variables themselves should change much more slowly.
  */
 parsecl(argc,argv);

 /*
  * This fills the global *dh_rng_types defined in libdieharder with all
  * the rngs known directly to libdieharder including known hardware
  * generators and file input "generators".  This routine also sets the
  * counts of each "kind" of generator into global/shared variables.  This
  * command must be run (by all UIs, not just the dieharder CLI) BEFORE
  * adding any UI generators, and BEFORE selecting a generator or input
  * stream to test.
  */
 dieharder_rng_types();

 /*
  * Similarly we load *dh_test_types[].
  */
 dieharder_test_types();

 /*
  * The following contains commands that are likely to be different for
  * different UIs.  At the very least, they can be done more than once in
  * an interactive UI, even though some of them will usually be done just
  * one time in the dieharder CLI.  I'm indenting them to show their
  * RELATIVE precedence in a core event loop in a UI.
  */

   /*
    * Pick a rng, establish a seed based on how things were initialized
    * in parsecl() or elsewhere.  Note that choose_rng() times the selected
    * rng as a matter of course now.
    */
   choose_rng();

   /*
    * At this point, a valid rng should be selected, allocated, and
    * provisionally seeded.  It -a(ll) is set (CLI only) run all the
    * available tests on the selected rng, reseeding at the beginning of
    * each test if Seed is nonzero.  Otherwise, run the single selected
    * test (which may still return a vector of pvalues) on the single
    * selected rng.  The CLI then goes on to exit; an interactive UI would
    * presumably loop back to permit the user to run another test on the
    * selected rng or select a new rng (and run more tests on it) until the
    * user elects to exit.
    *
    * It is the UI's responsibility to ensure that run_test() is not called
    * without choosing a valid rng first!
    */
   if(all){
     run_all_tests();
   } else {
     run_test();
   }

 /*
  * This ends the core loop for a non-CLI interactive UI.  GUIs will
  * typically exit directly from the event loop.  Tool UIs may well fall
  * through, and the CLI simply proceeds sequentially to exit.  It isn't
  * strictly necessary to execute an exit() command at the end, but it
  * does make the code a bit clearer (and let's one choose an exit code,
  * if that might ever matter.  Exit code 0 clearly means "completed
  * normally".
  */
 exit(0);

}
