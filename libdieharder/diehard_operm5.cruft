/*
 * $Id: diehard_operm5.c 230 2006-08-22 05:31:54Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard OPERM5 test, rewritten from the description
 * in tests.txt on George Marsaglia's diehard site.
 *
 *          THE OVERLAPPING 5-PERMUTATION TEST                 ::
 * This is the OPERM5 test.  It looks at a sequence of one mill- ::
 * ion 32-bit random integers.  Each set of five consecutive     ::
 * integers can be in one of 120 states, for the 5! possible or- ::
 * derings of five numbers.  Thus the 5th, 6th, 7th,...numbers   ::
 * each provide a state. As many thousands of state transitions  ::
 * are observed,  cumulative counts are made of the number of    ::
 * occurences of each state.  Then the quadratic form in the     ::
 * weak inverse of the 120x120 covariance matrix yields a test   ::
 * equivalent to the likelihood ratio test that the 120 cell     ::
 * counts came from the specified (asymptotically) normal dis-   ::
 * tribution with the specified 120x120 covariance matrix (with  ::
 * rank 99).  This version uses 1,000,000 integers, twice.       ::
 *
 * Note -- this test almost certainly has errors.  It has been
 * suggested that the actual rank is 5!-4!=96, not 99.  However,
 * "good" generators still fail this test with the lower rank.
 * I really think that the covariance matrix is going to have to
 * recomputed...
 *
 *                       Comment
 * Good test.  Just about everything fails it.  It is validated
 * to the extent humanly possible against the last c port of
 * diehard that I could find (die.c) on the remaining diehard
 * mirror but it is complicated enough (and poorly documented
 * enough) that I worry.  One lousy error in the r[][], s[][]
 * or map[] vector data and we'd get slightly wrong answers
 * and never know it, but we get the SAME answers right now
 * either way from the same data.
 *========================================================================
 */


#include <dieharder/libdieharder.h>

static int tflag=0;
static double tcount[120];

/*
 * kperm computes the permutation number of a vector of five integers
 * passed to it.
 */
uint kperm(uint v[],uint voffset)
{

 uint i,j,k,max;
 uint w[5];
 uint pindex,uret,tmp;

 /*
  * work on a copy of v, not v itself in case we are using overlapping
  * 5-patterns.
  */
 for(i=0;i<5;i++){
   j = (i+voffset)%5;
   w[i] = v[j];
 }

 if(verbose == -1){
   printf("==================================================================\n");
   printf("%10u %10u %10u %10u %10u\n",w[0],w[1],w[2],w[3],w[4]);
   printf(" Permutations = \n");
 }
 pindex = 0;
 for(i=4;i>0;i--){
   max = w[0];
   k = 0;
   for(j=1;j<=i;j++){
     if(max <= w[j]){
       max = w[j];
       k = j;
     }
   }
   pindex = (i+1)*pindex + k;
   tmp = w[i];
   w[i] = w[k];
   w[k] = tmp;
   if(verbose == -1){
     printf("%10u %10u %10u %10u %10u\n",w[0],w[1],w[2],w[3],w[4]);
   }
 }
 if(pindex < 60 ){
   uret = map[pindex];
 } else {
   uret = pindex;
 }

 if(verbose == -1){
   printf(" => %u\n",pindex);
   printf("map[%u] = %u\n",pindex,uret);
 }

 return uret;
   
}

int diehard_operm5(Test **test, int irun)
{

 uint i,j,k,kp,t,vind,v[5];
 double count[120];
 double av,norm,x[60],y[60],chisq,ndof,pvalue;

 if(verbose == D_DIEHARD_OPERM5 || verbose == D_ALL){
   printf("int r[][] = {\n");
   for(i=0;i<60;i++){
     printf("{");
     for(j=0;j<59;j++){
       printf("%10d, ",r[i][j]);
     }
     printf("%10d},\n",r[i][59]);
   }
   printf("};\n");
   printf("int s[][] = {\n");
   for(i=0;i<60;i++){
     printf("{");
     for(j=0;j<59;j++){
       printf("%10d, ",s[i][j]);
     }
     printf("%10d},\n",s[i][59]);
   }
   printf("};\n");
 }

 /*
  * for display only.  5 flags the size of the permutation set.
  */
 test[0]->ntuple = 5;

 /*
  * Zero count vector, was t(120) in diehard.f90.
  */
 for(i=0;i<120;i++) {
   count[i] = 0.0;
   if(tflag == 0){
     tcount[i] = 0.0;
     tflag = 1;
   }
 }

 for(i=0;i<5;i++){
   v[i] = gsl_rng_get(rng);
 }
 vind = 0;

 for(t=0;t<test[0]->tsamples;t++){

   /*
    * OK, now we are ready to generate a list of permutation indices.
    * Basically, we take a vector of 5 integers and transform it into a
    * number with the kperm function.  We will use the overlap flag to
    * determine whether or not to refill the entire v vector or just
    * rotate bytes.
    */
   kp = kperm(v,vind);
   /* printf("kp = %u\n",kp); */
   count[kp]++;
   v[vind] = gsl_rng_get(rng);
   vind = (vind+1)%5;
 }

 MYDEBUG(D_DIEHARD_OPERM5){
   for(i=0;i<120;i++){
     printf("%u: %f\n",i,count[i]);
   }
 }
 for(i=0;i<120;i++){
   tcount[i] += count[i];
   /* printf("%u: %f\n",i,tcount[i]); */
 }

 /*
  * Now (at last) we compute the statistic and get a p-value.  The
  * computation is straightforward, but involves the r and s matrices
  * so it doesn't fit the existing X or B models.
  */
 chisq = 0.0;
 /*
  * Not at all sure about this, yet.
  */
 av = 2.0*test[0]->tsamples/120.0;
 norm = 2.e5*test[0]->tsamples;
 for(i=0;i<60;i++){
   x[i] = count[i] + count[i+60] - av;
   y[i] = count[i] - count[i+60];
   /* printf("count[%u] = %f  count[%u] = %f  x[%u] = %f  y[%u] = %f\n",i,count[i],i+60,count[i+60],i,x[i],i,y[i]);*/
 }
 for(i=0;i<60;i++){
   for(j=0;j<60;j++){
     chisq = chisq + x[i]*r[i][j]*x[j] + y[i]*s[i][j]*y[j];
   }
 }
 /*
  * The absolute value "shouldn't" be necessary but it is -- every
  * few hundred runs we get a negative chisq, which seems very
  * plausible (actually) given the numbers and wierd chisq in the
  * first place.  The other possibility (alas a very reasonable one)
  * is that some of the numbers in r[][], s[][] or map[] are incorrect.
  * Noting well that they AGREE with an INDEPENDENT PORT of diehard
  * to C to 12 significant figures when run on identical binary files
  * and flagged to use the same algorithm...
  *
  * It would be nice, so nice, to have SOME clue how to actually generate
  * the matrices and other numbers since even a simple sign error on
  * a single number could make the test useless and (incidentally) cause
  * it to sometimes return a negative chisq.
  *
  * In the meantime, negative chisq causes the incomplete gamma function
  * routine to crash, so we protect it with the fabs() call.
  */
 chisq = fabs(chisq / norm);
 ndof = 96;
 MYDEBUG(D_DIEHARD_OPERM5){
   printf("# diehard_operm5(): chisq[%u] = %10.5f\n",kspi,chisq);
 }
 test[0]->pvalues[irun] = gsl_sf_gamma_inc_Q((double)(ndof)/2.0,chisq/2.0);
 MYDEBUG(D_DIEHARD_OPERM5){
   printf("# diehard_operm5(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 kspi++;

 return(0);

}

