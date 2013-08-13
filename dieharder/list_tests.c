/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 * ========================================================================
 * This routine simply lists all the tests currently known to dieharder.
 * VERY SOON NOW we will automate it and make it look EXACTLY like
 * the list_rngs() output.
 * ========================================================================
 */

#include "dieharder.h"

void list_tests()
{

 int i;
 
 dieharder_test_types();

 dh_header();
 fprintf(stdout,"Installed dieharder tests:\n");
 fprintf(stdout,"%12s\t%30s\t%30s\n","Test Number","Test Name","Test Reliability");
 fprintf(stdout,"===============================================================================\n");
 for(i=0;i<900;i++){
   if(dh_test_types[i]){
     fprintf(stdout,"  -d %d  \t%40s",i,dh_test_types[i]->name);
     if( i == 5 || i == 6 || i == 7 ){
       fprintf(stdout,"\t%10s\n","Suspect");
     } else if(i == 14){
       fprintf(stdout,"\t%10s\n","Do Not Use");
     } else {
       fprintf(stdout,"\t%10s\n","Good");
     }
     
   }

 }

}

void list_tests_old()
{

 dh_header();

 printf("\n\
Bug/Development Key (use at your own risk):\n\
   rft = ready for testing (the test may - or may not - work correctly)\n\
   sus = suspect test (consistently fails \"good\" generators)\n\
   dev = development only (may crash, isn't finished)\n\
\n\
Only fully reliable tests will be run when diehard -a is invoked,\n\
with \"reasonable\" default parameters.  \n\
\n\
PLEASE READ THE MAN PAGE to learn about p-values and the null hypothesis\n\
in testing BEFORE using this testing tool!\n\
\n\
            Diehard Tests\n\
   -d 1  Diehard Birthdays test\n\
   [sus: -d 2  Diehard Overlapping Permutations test]\n\
   -d 3  Diehard 32x32 Binary Rank test\n\
   -d 4  Diehard 6x8 Binary Rank test\n\
   -d 5  Diehard Bitstream test\n\
   -d 6  Diehard OPSO test\n\
   -d 7  Diehard OQSO test\n\
   -d 8  Diehard DNA test\n\
   -d 9  Diehard Count the 1s (stream) test\n\
   -d 10 Diehard Count the 1s (byte) test\n\
   -d 11 Diehard Parking Lot test\n\
   -d 12 Diehard Minimum Distance (2D Spheres) test\n\
   -d 13 Diehard 3D Spheres (minimum distance) test\n\
   -d 14 Diehard Squeeze test\n\
   [sus: -d 15 Diehard Sums test]\n\
   -d 16 Diehard Runs test\n\
   -d 17 Diehard Craps test\n\
   -d 18 Marsaglia and Tsang GCD test\n\
   [dev: -d 19 Marsaglia and Tsang Gorilla test]\n\
\n\
             RGB Tests\n\
   -r 1 RGB Timing test (times the rng)\n\
   -r 2 RGB Bit Persistence test\n\
   -r 3 RGB Ntuple Bit Distribution test suite (-n ntuple)\n\
   -r 4 RGB Generalized Minimum Distance test\n\
   -r 5 RGB Permutations test (new, partial replacement for operm tests)]\n\
   [rft: -r 6 RGB Lagged Sums test\n\
         (do not use the following as tests yet)\n\
   [dev: -r 7 RGB L-M-Ntuple Distribution test suite (quite long)]\n\
   [dev: -r 8 RGB Overlapping Permutations test]\n\
   [rft: -r 9 RGB KStest test]\n\
\n\
      Statistical Test Suite (STS)\n\
   -s 1 STS Monobit test\n\
   -s 2 STS Runs test\n\
   -s 3 STS Serial test\n\
\n\
            User Tests\n\
   -u 1 User Template (Lagged Sum Test)\n\
\n");

}
