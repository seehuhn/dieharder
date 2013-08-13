/*
 *========================================================================
 * $Id: startup.c 242 2006-10-04 05:54:56Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * This routine does all the required initialization and startup,
 * including memory allocation and prefilling of vectors.  It is
 * COMPLETELY outside the timing loops.
 *========================================================================
 */

#include "dieharder.h"

void testbits()
{

 unsigned int input[4],output[4];
 unsigned int tuple,off;
 int pattern[10];

 off = get_uint_rand(rng);
 printf("\n");
 printf(" off = ");
 dumpuintbits(&off,1);
 printf("\n");
 mybitadd((char*) &tuple,0, (char*) &off,4,16);
 printf(" Result = ");
 dumpuintbits(&tuple,1);
 printf("\n");
 exit(0);
 /*
  * Let's test some patterns
  */
 pattern[0] = 4;  /* grab 4 bits */
 pattern[1] = -8; /* skip 8 bits */
 pattern[2] = 4;  /* grab 4 more */
 pattern[3] = -12;/* skip 12 */
 pattern[4] = 3;  /* grab 3 more */
 pattern[5] = 0;
 




 /*
  * test get_uint_rand(rng)
  */
 printf("Testing get_uint_rand(rng)\n");
 printf("==============================================================================\n");
 input[0] = get_uint_rand(rng);
 printf("get_uint_rand(rng) = %10u = ",input[0]);
 dumpuintbits(&input[0],1);printf("\n");
 input[1] = get_uint_rand(rng);
 printf("get_uint_rand(rng) = %10u = ",input[1]);
 dumpuintbits(&input[1],1);printf("\n");
 input[2] = get_uint_rand(rng);
 printf("get_uint_rand(rng) = %10u = ",input[2]);
 dumpuintbits(&input[2],1);printf("\n");
 input[3] = get_uint_rand(rng);
 printf("get_uint_rand(rng) = %10u = ",input[3]);
 dumpuintbits(&input[3],1);printf("\n");
 printf("==============================================================================\n\n");


 
 /*
  * The following are different tests we need to run to validate
  * get_ntuple_cyclic (which is a bitch to code).
  */
 printf("\nTesting get_ntuple_cyclic with random data.\n");
 /* Get exactly one line.  Terminates rule 2a */ 
 tuple = 32;
 off = 0;
 printf("Getting a %d-tuple from offset = %d\n|",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 1a */ 
 tuple = 24;
 off = 0;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /*
  * The following are different tests we need to run to validate
  * get_ntuple_cyclic (which is a bitch to code).
  */
 /* Terminates rule 2a */ 
 tuple = 24;
 off = 8;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /*
  * The following are different tests we need to run to validate
  * get_ntuple_cyclic (which is a bitch to code).
  */
 /* Terminates rule 1a (interior) */ 
 tuple = 16;
 off = 8;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 2c -- one fill from rule R, then 2c */
 tuple = 24;
 off = 24;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 1 -- alternate R-L R-L R-L 1 */
 tuple = 120;
 off = 0;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 2b -- R R R 2b */
 tuple = 120;
 off = 8;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 2c, cycli -- one fill from rule R, then 2c */
 tuple = 24;
 off = 120;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 1, but wraps cyclic -- R L R L R L 1 */
 tuple = 120;
 off = 32;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /* Terminates rule 2b, cyclic -- R R R 2b */
 tuple = 120;
 off = 40;
 printf("Getting a %d-tuple from offset = %d\n",tuple,off);
 printf("==================================================================\n");
 dumpuintbits(&input[0],2);printf("\n");
 dumpuintbits(&input[2],2);printf("\n");
 get_ntuple_cyclic(input,4,output,4,tuple,off);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 /*
  * test get_rand_bits()
  */
 printf("Testing get_rand_bits()\n");
 tuple = 32;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 printf("Testing get_rand_bits()\n");
 tuple = 32;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 printf("Testing get_rand_bits()\n");
 tuple = 64;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 tuple = 16;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 tuple = 16;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 tuple = 16;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 tuple = 32;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

 tuple = 24;
 printf("Getting a %d-tuple from circulating buffer\n",tuple);
 printf("==============================================================================\n");
 get_rand_bits((void *)output,4*sizeof(unsigned int),tuple,rng);
 dumpuintbits(&output[0],2);printf("\n");
 dumpuintbits(&output[2],2);printf("\n");
 printf("==================================================================\n\n");

}

