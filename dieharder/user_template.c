/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include <dieharder.h>

/*
 *========================================================================
 * This is a very simple test template for a test that generates a
 * single trivial statistic.  It demonstrates the basic idea of most
 * random number tests -- think up a number -- ANY number -- that depends
 * in ANY way on the fact that random numbers are truly random (aye,
 * there's the mathematical rub:-).  Ideally a number that in the end
 * is (thanks to the central limit theorem) normally distributed with a
 * theoretically computable mean and variance.  Use a GSL-wrapped
 * generator to generate the rands required to sample that number,
 * obtaining an iid (independent, identically distributed) set of samples.
 * find their mean.  Determine the probability of obtaining that
 * particular value by a random trial (from the erf of the associated
 * normal distribution) -- this is the "p-value" of the trial.  Store it
 * in a vector for many (psamples) independent trials, each containing
 * many (tsamples) iid samples.
 *
 * The pvalues of the independent trials are themseles a random variable --
 * one that should be distributed UNIFORMLY on the interval [0,1].  The
 * (often ambiguous) signal of the FAILURE of a rng is a non-uniform
 * distribution of p-values.  In some cases the failure isn't subtle --
 * all the p-values are zero, or close to it, for example.  However, in
 * other cases there are lots of perfectly reasonable p's generated if you
 * look at them one at a time -- does "p = 0.348" mean success or failure?
 * If ALL the p-values are clustered around 0.35, it means failure, but
 * this isn't obvious from a single test and MAY not be obvious from MANY
 * tests.  We want a mathematical criterion for deciding how likely it is
 * that we get any given DISTRIBUTION of p from many trials, and besides
 * would like to look at that distribution as the human eye is a fair
 * judge of emergent structure.  We therefore PLOT THE HISTOGRAM of the
 * final distribution of trial p-values , and apply a "Kolmogorov-Smirnov"
 * test to the vector to determine how uniform (or not) it is.  This leads
 * to "the" p-value of the entire test run -- the probability that the
 * entire sequence of results could have been obtained if in fact the
 * generator was a good one.  [Of course THIS p-value could be put in a
 * vector and subjected to yet another KS test and so on ad infinitum, but
 * we arbitrarily terminate here and instead resolve failures where
 * this might work by cranking up psamples and/or tsamples, if possible.]
 *
 * Even here, judging success or failure is at best ambiguous and may
 * require several runs and some play to inform a decision.  We arbitrarily
 * set "failure" to mean a final p-value less than 0.01%.  Only once in
 * ten thousand runs are you likely to get such a low p by chance.  A
 * generator is flagged as "poor" if it returns a p-value of 0.01 (1% of
 * the time this will happen even for a good generator, mind you!).  A
 * "possibly weak" warning ensues from a p-value < 0.05, noting that this
 * will happpen with good generators 5% of the time and ONLY by rerunning
 * the test several times (or increasing the number of psamples and seeing
 * the p-value go down to an less ambiguous measure of failure) can you
 * increase our confidence that the generator is good -- or not.
 *
 * All other p-values are judged "success".
 *
 * As you can see, testing random number generators is a messy business.
 * Answers are rarely clear and clean.  Besides, one is armed a priori
 * with the certain knowledge that ANY random number generator can be
 * made to fail, as "random number generator" is an mathematically
 * oxymoronic term (reserving judgement only for the POSSIBILITY that
 * quantum generators MAY prove to be truly random, but even this is
 * an assertion, not a known fact, and dieharder applied to random
 * sequences created by random processes are indeed an EXPERIMENT that
 * might test the assertion!).
 *
 *========================================================================
 */

int user_template(Test **test,int irun)
{

 unsigned int t,i,lag;
 Xtest ptest;

 /*
  * Get the lag from ntuple.  Note that a lag of zero means
  * "don't throw any away".
  */
 lag = test[0]->ntuple;

 /*
  * ptest.x = actual sum of tsamples lagged samples from rng
  * ptest.y = tsamples*0.5 is the expected mean value of the sum
  * ptest.sigma = sqrt(tsamples/12.0) is the standard deviation
  */
 ptest.x = 0.0;  /* Initial value */
 ptest.y = (double) test[0]->tsamples*0.5;
 ptest.sigma = sqrt(test[0]->tsamples/12.0);

 if(verbose == D_USER_TEMPLATE || verbose == D_ALL){
   printf("# user_template(): Doing a test with lag %u\n",lag);
 }

 for(t=0;t<test[0]->tsamples;t++){

   /*
    * A VERY SIMPLE test, but sufficient to demonstrate the
    * weaknesses in e.g. mt19937.
    */

   /* Throw away lag per sample */
   for(i=0;i<lag;i++) gsl_rng_uniform(rng);

   /* sample only every lag numbers, reset counter */
   ptest.x += gsl_rng_uniform(rng);

 }

 Xtest_eval(&ptest);
 test[0]->pvalues[irun] = ptest.pvalue;

 if(verbose == D_USER_TEMPLATE || verbose == D_ALL){
   printf("# user_template(): ks_pvalue[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 return(0);

}

void help_user_template()
{

  printf("%s",user_template_dtest.description);

}
