/*
 * $Id: rgb_bitdist.c 248 2006-10-09 17:59:54Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is a test that checks to see if the rng generates bit patterns
 * (n-tuples) that are distributed correctly (binomially).  For example,
 * for 2-tuples (bit pairs) there are four possibilities: 00, 01, 10, 11.
 * Each should occur with a probability of 1/4, hence the count of (say) K
 * 00 bitpairs out of N trials should be distributed (over M samples)
 * according to the binomial probability p = binomial(N,K,0.25) -- that
 * is, the expected count for k 00's is M*p(N,K,0.25).
 *
 * This test should be more sensitive than just doing a large number of
 * trials and aggregating all the 00's over all the samples and comparing
 * the result to the expected mean, as there can be distributions that
 * have the expected mean of 0.25 that are >>not<< binomially distributed.
 *
 * By making a single program capable of doing any n-tuple, we gain
 * immediate benefit.  Both STS and Diehard, for example, contain
 * tests that validate bit distribution frequencies; however, they typically
 * do so for specific n-tuples, e.g. 5 in several diehard tests.  There
 * is nothing terribly special about 5, and no point in looking too
 * hard at e.g. intervals between patterns at 5, since testing pure
 * frequency at 6 bits simultaneously ensures that all 6 bit patterns have
 * the correct frequency (since they are sub-patterns of 6 bit patterns
 * that have the correct frequency, and if they had the INcorrect frequency
 * the six-bit patterns would as well by inheritance) and (less obviously,
 * but I'm pretty sure I can prove it) that intervals between the five
 * bit patterns must also be correct IF the test is satisfied robustly
 * for various arbitrary test string lengths and sampling counts.
 *
 * Anyway, I'm giving this one a shot as it may end up being a one-size
 * fits all tool for validating bitlevel randomness for any generator,
 * up to some n-tuple.  I expect that 2004/2005 computers will easily
 * be able to validate through n=8 without really warming up or taking
 * terribly long (validating the frequency and distribution for e.g.
 * the entire ascii alphabet at the byte level) and MAY be able to do
 * actual frequency validation on 16-bit integers.  Note that it only takes
 * a few seconds to generate millions of 32 bit integers with most of
 * the GSL routines.  To get statistically valid results, one needs to
 * accumulate order of 100 trials per outcome, or test order of millions
 * of integers and accumulate the results in an outcome vector 64K long.
 * both of these are well within reach using at most hours of CPU time,
 * and partitioned on a beowulf might take only minutes or even seconds.
 *
 * The latest version of the actual test (below) tests only ONE ntuple,
 * the value set in the global variable rgb_bitdist_ntuple which must
 * be a positive integer.  The calling program is responsible for e.g.
 * testing a range of ntuples.
 *========================================================================
 */

#include <dieharder/libdieharder.h>

void rgb_bitdist(Test **test,int irun)
{

 uint bsize;       /* number of bits in the sample buffer */
 uint nb;          /* number of bits in a tested ntuple */
 uint value_max;   /* 2^{nb}, basically (max size of nb bit word + 1) */
 uint bsamples;    /* The number of non-overlapping samples in buffer */
 uint boffset;     /* The offset of the current/next sample */
 uint value;       /* value of sampled ntuple (as a uint) */
 uint randbuf[4];  /* A 128 bit space for rng samples */

 /* Look for cruft below */

 uint b,t,i;   /* loop indices? */
 uint ri;
 uint *count,ctotal; /* count of any ntuple per bitstring */

 uint size;
 double pvalue,ntuple_prob,pbin;  /* probabilities */
 Vtest *vtest;               /* A reusable vector of binomial test bins */

 /*
  * This is the size of the allocated sample buffer, in bits.
  */
 bsize = sizeof(randbuf)*CHAR_BIT;

 /*
  * Sample a bitstring of rgb_bitstring_ntuple in length (exactly).
  */
 if(rgb_bitdist_ntuple>0){
   nb = rgb_bitdist_ntuple;
   if(verbose == D_RGB_BITDIST || verbose == D_ALL){
     printf("# rgb_bitdist: Buffer size is %d\n",bsize);
     printf("# rgb_bitdist: Testing ntuple = %u\n",nb);
   }
 } else {
   printf("Error:  rgb_bitdist_ntuple must be a positive integer.  Exiting.\n");
   exit(0);
 }


 /*
  * The number of nb-bit numbers we can pull from a single sample
  * of bit length bsize, without overlap.
  */
 bsamples = bsize/nb;

 /*
  * The largest integer for this ntuple is 2^nb-1 (they range from 0 to
  * 2^nb - 1).  However, this is used to size count and limit loops, so
  * we use 2^nb and start indices from 0 as usual.
  */
 value_max = (uint) pow(2,nb);
 if(verbose == D_RGB_BITDIST || verbose == D_ALL){
   printf("# rgb_bitdist(): value_max = %u\n",value_max);
 }

 /*
  * Allocate memory for value_max vector of Vtest structs and counts,
  * PER TEST.  Note that we must free both of these when we are done
  * or leak.
  */
 vtest = (Vtest *)malloc(value_max*sizeof(Vtest));
 count = (uint *)malloc(value_max*sizeof(uint));

 /*
  * This is the probability of getting any given ntuple.  For example,
  * for bit triples, value_max = 2^3 = 8 and each value should occur
  * with probability 1/8.
  */
 ntuple_prob = 1.0/(double)value_max;
 if(verbose == D_RGB_BITDIST || verbose == D_ALL){
   printf("# rgb_bitdist(): ntuple_prob = %f\n",ntuple_prob);
   printf("# rgb_bitdist(): Testing %u samples of %u bit strings\n",test[0]->tsamples,bits);
   printf("# rgb_bitdist():=====================================================\n");
   printf("# rgb_bitdist():            vtest table\n");
   printf("# rgb_bitdist(): Outcome   bit          x           y       sigma\n");
 }
 for(i=0;i<value_max;i++){
   Vtest_create(&vtest[i],bsamples+1,"rgb_bitdist",gsl_rng_name(rng));
   for(b=0;b<=bsamples;b++){
     if(i==0){
       pbin = gsl_ran_binomial_pdf(b,ntuple_prob,bsamples);
       vtest[i].x[b] = 0.0;
       vtest[i].y[b] = test[0]->tsamples*pbin;
     } else {
       vtest[i].x[b] = 0.0;
       vtest[i].y[b] = vtest[0].y[b];
     }
     if(verbose == D_RGB_BITDIST || verbose == D_ALL){
       printf("# rgb_bitdist():  %3u     %3u   %10.5f  %10.5f\n",
         i,b,vtest[i].x[b],vtest[i].y[b]);
     }
   }
   if(verbose == D_RGB_BITDIST || verbose == D_ALL){
     printf("# rgb_bitdist():=====================================================\n");
   }
 }

 /*
  * Now (per ntuple) we check tsamples bitstrings of bits in length,
  * counting the 1's.  At the end we increment the result histogram
  * with the bitcount as an index as a trial that generated that
  * bitcount.
  */
 for(t=0;t<test[0]->tsamples;t++){

   /*
    * This safely fills the buffer even if the generator returns
    * less than 32 bits.  Note that fill_uint_buffer requires the
    * LENGTH of the uint vector in uints, not in bytes...
   fill_uint_buffer(randbuf,sizeof(randbuf)/sizeof(uint));
    *
    * CRUFT.
    */
   if(verbose == D_RGB_BITDIST || verbose == D_ALL){
      printf("# rgb_bitdist(): randbuf = %10u = ",randbuf[0]);
      dumpbits(&randbuf[0],CHAR_BIT*sizeof(uint));
   }

   /*
    * Clear the count vector for this sample.
    */
   memset(count,0,value_max*sizeof(uint));

   /*
    * Rotate starting offset with t, e.g. 0,1,2,0,1,2 for nb=3
    */
   boffset = t%nb;
   for(b=0;b<bsamples;b++){
     /*
      * This gets the integer value of the ntuple of length nb, at index
      * position boffset in the current uint randbuf[] of length 4, from a
      * window with cyclic wraparound.
      *
     value = get_bit_ntuple((void *)&randbuf[0],sizeof(uint),nb,boffset);
      *
      * BUT it is cruft, to be used NO MORE.  THIS gets precisely THE bit
      * ntuple of length nb that is the next available in the bitstream
      * provided by the generator, without skipping bits.  This is clearly
      * the right thing to use here, as we prepare to create a supertest
      * of this test with a user-adjustable gap parameter and a fourier
      * version of this test ditto.  Note that we don't need the rng call
      * above any more.
      */
     get_rand_bits(&value,sizeof(uint),nb,rng);

     /*
      * We increment the count of this ntuple value's occurence in this
      * sample string of bits.
      */
     count[value]++;
     if(verbose == D_RGB_BITDIST || verbose == D_ALL){
       printf("# rgb_bitdist():b=%u boffset = %u count[%u] = %u\n",b,boffset,value,count[value]);
     }

     /*
      * We increment the offset and do the next bsample
      */
     boffset += nb;
   }

   /*
    * We've finished the pattern.  count contains the number of
    * times each possible pattern occurred in samples drawn from
    * randbuf[].  We therefore have to increment the CUMULATIVE
    * counter -- vtest -- so we can compare the result to the
    * expected value when we're done.
    */
   ctotal = 0;
   for(i=0;i<value_max;i++){
     vtest[i].x[count[i]]++;
     ctotal += count[i];
     if(verbose == D_RGB_BITDIST || verbose == D_ALL){
       printf("# rgb_bitdist(): vtest[%u].x[%u] = %u\n",i,count[i],(uint)vtest[i].x[count[i]]);
     }
   }
   if(verbose == D_RGB_BITDIST || verbose == D_ALL){
     printf("# rgb_bitdist(): Sample %u: total count = %u (should be %u, count of bits)\n",t,ctotal,bits);
   }
 }

 /*
  * Now, finally, we need to score the vtest for this value of nb
  * (all tsamples of it) and turn it into a p-value.  This is one
  * of the two places this test may be screwing up big time.
  */

 if(verbose == D_RGB_BITDIST || verbose == D_ALL){
   printf("# rgb_bitdist(): ntuple_prob = %f\n",ntuple_prob);
   printf("# rgb_bitdist(): Testing %u samples of %u bit strings\n",test[0]->tsamples,bits);
   printf("# rgb_bitdist():=====================================================\n");
   printf("# rgb_bitdist():            vtest table\n");
   printf("# rgb_bitdist(): Outcome   bit          x           y       sigma\n");
 }
 ri = gsl_rng_uniform_int(rng,value_max);
 for(i=0;i<value_max;i++){
   for(b=0;b<=bsamples;b++){
     if(verbose == D_RGB_BITDIST || verbose == D_ALL){
       printf("# rgb_bitdist():  %3u     %3u   %10.5f  %10.5f\n",
         i,b,vtest[i].x[b],vtest[i].y[b]);
     }
   }
   if(verbose == D_RGB_BITDIST || verbose == D_ALL){
     printf("# rgb_bitdist():=====================================================\n");
   }
   Vtest_eval(&vtest[i]);
   /*
    * This is a bit nasty.  We can only save ONE pvalue per call.  The
    * only way to do so without bias is to randomly select which one to
    * save from large set of possibilities.
    */
   if(i == ri ) {
     test[0]->pvalues[irun] = vtest[i].pvalue;
     MYDEBUG(D_RGB_BITDIST) {
       printf("# rgb_bitdist(): test[%u]->pvalues[%u] = %10.5f\n",
          0,irun,test[0]->pvalues[irun]);
     }
   }
   Vtest_destroy(&vtest[i]);
 }

}

