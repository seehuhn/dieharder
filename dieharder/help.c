/*
 *========================================================================
 * $Id: help.c 529 2011-04-01 17:49:31Z rgbatduke $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"

void help()
{

 int i;

 dieharder_test_types();

 /*
  * help for EACH test must be defined in the test.c package, even if
  * it is null or says "sorry, no help available".  help should be
  * a brief test description all pretty with each line beginning with
  * "#" so it is easy to filter out of standard test results output.
  */
 if(all==YES){
   for(i=0;i<900;i++){
     if(dh_test_types[i]){
       fprintf(stdout,"%s",dh_test_types[i]->description);
     }
   }
   Exit(0);
 }

 /* printf("OK, what is dtest_num = %u\n",dtest_num);*/

 if(dh_test_types[dtest_num]){
   fprintf(stdout,"%s",dh_test_types[dtest_num]->description);
   Exit(0);
 } else {
   Usage();
   Exit(0);
 }

}

void Usage()
{

 dh_header();

 fprintf(stdout, "\n\
Usage:\n\
\n\
dieharder [-a] [-d dieharder test number] [-f filename] [-B]\n\
          [-D output flag [-D output flag] ... ] [-F] [-c separator]\n\
          [-g generator number or -1] [-h] [-k ks_flag] [-l] \n\
          [-L overlap] [-m multiply_p] [-n ntuple] \n\
          [-p number of p samples] [-P Xoff]\n\
          [-o filename] [-s seed strategy] [-S random number seed]\n\
          [-n ntuple] [-p number of p samples] [-o filename]\n\
          [-s seed strategy] [-S random number seed]\n\
          [-t number of test samples] [-v verbose flag]\n\
          [-W weak] [-X fail] [-Y Xtrategy]\n\
          [-x xvalue] [-y yvalue] [-z zvalue]\n");
fprintf(stdout, "\n\
  -a - runs all the tests with standard/default options to create a report\n\
  -d test number -  selects specific diehard test.\n\
  -f filename - generators 201 or 202 permit either raw binary or \n\
     formatted ASCII numbers to be read in from a file for testing.\n\
     generator 200 reads in raw binary numbers from stdin.\n\
     Note well: many tests with default parameters require a lot of rands!\n\
     To see a sample of the (required) header for ASCII formatted input, run\n\
\n\
         dieharder -o -f example.input -t 10\n\
\n\
     and then examine the contents of example.input.\n\
     Raw binary input reads 32 bit increments of the specified data stream.\n\
     stdin_input_raw accepts a pipe from a raw binary stream.\n\
  -B binary output (used with -o)\n\
  -D output flag - permits fields to be selected for inclusion in dieharder\n\
     output.  Each flag can be entered as a binary number that turns\n\
     on a specific output field or header or by flag name; flags are\n\
     aggregated.  To see all currently known flags use the -F command.\n\
  -F - lists all known flags by name and number.\n\
  -c table separator - where separator is e.g. ',' (CSV) or ' ' (whitespace).\n\
  -g generator number - selects a specific generator for testing.  Using\n\
     -1 causes all known generators to be printed out to the display.\n\
  -h prints context-sensitive help -- usually Usage (this message) or a\n\
     test synopsis if entered as e.g. dieharder -D 3 -h.\n\
  -k ks_flag - ks_flag\n\
\n\
     0 is fast but slightly sloppy for psamples > 4999 (default).\n\
\n\
     1 is MUCH slower but more accurate for larger numbers of psamples.\n\
\n\
     2 is very slow and accurate to machine precision.\n\
\n\
     3 is kuiper ks, fast, quite inaccurate for small samples, deprecated.\n\
\n\
  -l list all known tests.\n\
  -L overlap \n\
\n\
     1 (use overlap, default) \n\
\n\
     0 (don't use overlap) \n\
\n\
     in operm5 or other tests that support overlapping and non-overlapping \n\
     sample modes. \n\
  -m multiply_p - multiply default # of psamples in -a(ll) runs to crank\n\
     up the resolution of failure.\n\
  -n ntuple - set ntuple length for tests on short bit strings that permit\n\
     the length to be varied (e.g. rgb bitdist).\n\
  -o filename - output -t count random numbers from current generator to file.\n\
  -p count - sets the number of p-value samples per test (default 100).\n\
  -P Xoff - sets the number of psamples that will cumulate before deciding \n\
     that a generator is 'good' and really, truly passes even a -Y 2 T2D run. \n\
     Currently the default is 100000; eventually it will be set from \n\
     AES-derived T2D test failure thresholds for fully automated reliable \n\
     operation, but for now it is more a 'boredom' threshold set by how long \n\
     one might reasonably want to wait on any given test run. \n\
  -S seed - where seed is a uint.  Overrides the default random seed\n\
     selection.  Ignored for file or stdin input.\n\
  -s strategy - if strategy is the (default) 0, dieharder reseeds (or\n\
     rewinds) once at the beginning when the random number generator is\n\
     selected and then never again.  If strategy is nonzero, the generator\n\
     is reseeded or rewound at the beginning of EACH TEST.  If -S seed was\n\
     specified, or a file is used, this means every test is applied to the\n\
     same sequence (which is useful for validation and testing of dieharder,\n\
     but not a good way to test rngs).  Otherwise a new random seed is\n\
     selected for each test.\n\
  -t count - sets the number of random entities used in each test, where\n\
     possible.  Be warned -- some tests will take a long time with the\n\
     default value of 10000.  Read the test synopses for suggested settings\n\
     for -t or use -a first.  Many tests will ignore -t as they require\n\
     a very specific number of samples to be used in generating their\n\
     statistic.\n");
fprintf(stdout, "  -W weak - sets the 'weak' threshold to make the test(s) more or less \n\
     forgiving during e.g. a test-to-destruction run.  Default is currently \n\
     0.005. \n\
  -X fail - sets the 'fail' threshold to make the test(s) more or less \n\
     forgiving during e.g. a test-to-destruction run.  Default is currently \n\
     0.000001, which is basically 'certain failure of the null hypothesis', \n\
     the desired mode of reproducible generator failure. \n\
  -Y Xtrategy - the Xtrategy flag controls the new 'test to failure' (T2F)\n\
     modes.  These flags and their modes act as follows:\n\
\n\
     0 - just run dieharder with the specified number of tsamples and\n\
     psamples, do not dynamically modify a run based on results.  This is\n\
     the way it has always run, and is still the default.\n\
\n\
     1 - 'resolve ambiguity' (RA) mode.  If a test returns 'weak', this is \n\
     an undesired result.  What does that mean, after all?  If you run a long\n\
     test series, you will see occasional weak returns for a perfect\n\
     generators because p is uniformly distributed and will appear in any\n\
     finite interval from time to time.  Even if a test run returns more than\n\
     one weak result, you cannot be certain that the generator is failing.\n\
     RA mode adds psamples (usually in blocks of 100) until the\n\
     test result ends up solidly not weak or proceeds to unambiguous failure.\n\
     This is morally equivalent to running the test several times to see if a\n\
     weak result is reproducible, but eliminates the bias of personal\n\
     judgement in the process since the default failure threshold is very\n\
     small and very unlikely to be reached by random chance even in many\n\
     runs.\n\
\n\
     This option should only be used with -k 2.\n\
\n\
     2 - 'test to destruction' (T2D) mode.  Sometimes you just want to know\n\
     where or if a generator will .I ever fail a test (or test series).  -Y 2\n\
     causes psamples to be added 100 at a time until a test returns an\n\
     overall pvalue lower than the failure threshold or a specified maximum\n\
     number of psamples (see -P) is reached.  \n");
fprintf(stdout, "\n\
     Note well!  In this mode one may well fail due to the alternate\n\
     null hypothesis -- the test itself is a bad test and fails!  Many\n\
     dieharder tests, despite our best efforts, are numerically unstable or\n\
     have only approximately known target statistics or are straight up\n\
     asymptotic results, and will eventually return a failing result even for\n\
     a gold-standard generator (such as AES), or for the hypercautious the\n\
     XOR generator with AES, threefish, kiss, all loaded at once and xor'd\n\
     together.  It is therefore safest to use this mode comparatively,\n\
     executing a T2D run on AES to get an idea of the test failure\n\
     threshold(s) (something I will eventually do and publish on the web so\n\
     everybody doesn't have to do it independently) and then running it on\n\
     your target generator.  Failure with numbers of psamples within an order\n\
     of magnitude of the AES thresholds should probably be considered\n\
     possible test failures, not generator failures.  Failures at levels\n\
     significantly less than the known gold standard generator failure\n\
     thresholds are, of course, probably failures of the generator.\n\
\n\
     This option should only be used with -k 2.\n\
\n\
  -v verbose flag -- controls the verbosity of the output for debugging\n\
     only.  Probably of little use to non-developers, and developers can\n\
     read the enum(s) in dieharder.h and the test sources to see which\n\
     flag values turn on output on which routines.  1 is 'all' and will\n\
     result in a highly detailed trace of program activity.\n\
\n\
     -x,-y,-z number - Some tests have parameters that can safely be varied\n\
     from their default value.  For example, in the diehard birthdays test,\n\
     one can vary the number of 'dates' drawn from the 'year' of some\n\
     length, which can also be varied.  -x 2048 -y 30 alters these two values\n\
     but should still run fine.  These parameters should be documented\n\
     internally (where they exist) in the e.g. -d 0 -h visible notes.\n\
\n\
  NOTE WELL:  The assessment(s) for the rngs may, in fact, be completely\n\
  incorrect or misleading.  In particular, 'Weak' pvalues should occur\n\
  one test in a hundred, and 'Failed' pvalues should occur one test in\n\
  a thousand -- that's what p MEANS.  Use them at your Own Risk!  Be Warned!\n\
\n");

 exit(0);

}

/* Cruft
 * This is a standard way of putting out a test description
void help_test(Dtest *dtest)
{

 printf("%s",dtest->description);
 
}
 */
