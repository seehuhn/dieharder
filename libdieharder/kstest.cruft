/*
 * $Id: kstest.c 223 2006-08-17 06:19:38Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 * This is the Kolmogorov-Smirnov test for uniformity on the interval
 * [0,1).  p-values from a (large) number of independent trials should
 * be uniformly distributed on the interval [0,1) if the underlying
 * result is consistent with the hypothesis that the rng in use is
 * not correlated.  Deviation from uniformity is a sign of the failure
 * of the hypothesis that the generator is "good".  Here we simply
 * input a vector of p-values and a count, and output an overall
 * p-value for the set of tests.
 *========================================================================
 */


#include <dieharder/libdieharder.h>


double kstest(double *pvalue,int count)
{

 int i,j,k;
 double y,d,dmax,csqrt;
 double p,x;

 /*
  * We start by sorting the list of pvalues.
  */
 gsl_sort(pvalue,1,count);

 /*
  * Here's the test.  For each (sorted) pvalue, its index is the
  * number of values cumulated to the left of it.  d is the distance
  * between that number and the straight line representing a uniform
  * accumulation.  We save the maximum d across all cumulated samples
  * and transform it into a p-value at the end.
  */
 dmax = 0.0;
 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("    p       y       d       dmax\n");
 }
 for(i=0;i<count;i++){
   y = (double) i/count;
   d = fabs(pvalue[i] - y);
   if(verbose == D_KSTEST || verbose == D_ALL){
     printf("%8.3f   %8.3f    %8.3f   %8.3f\n",pvalue[i],y,d,dmax);
   }
   if(d > dmax) dmax = d;
 }

 csqrt = sqrt(count);
 x = (csqrt + 0.12 + 0.11/csqrt)*dmax;
 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("Kolmogorov-Smirnov D = %8.3f, evaluating q_ks(%6.2f)\n",dmax,x);
 }
 p = q_ks(x);

 return(p);

}


double q_ks(double x)
{

 int i,sign;
 double qsum;
 double kappa;

 kappa = -2.0*x*x;
 sign = -1;
 qsum = 0.0;
 for(i=1;i<100;i++){
   sign *= -1;
   qsum += (double)sign*exp(kappa*(double)i*(double)i);
   if(verbose == D_KSTEST || verbose == D_ALL){
     printf("Q_ks %d: %f\n",i,2.0*qsum);
   }
 }

 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("Q_ks returning %f\n",2.0*qsum);
 }
 return(2.0*qsum);

}

/*
 * This is the Kuiper variant of KS, that is a bit more symmetric.
 */
double kstest_kuiper(double *pvalue,int count)
{

 int i,j,k;
 double y,v,vmax,vmin,csqrt;
 double p,x;

 /*
  * We start by sorting the list of pvalues.
  */
 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("# kstest_kuiper(): Computing Kuiper KS pvalue for:\n");
   for(i=0;i<count;i++){
     printf("# kstest_kuiper(): %3d    %10.5f\n",i,pvalue[i]);
   }
 }

 /*
  * This test is useless if there is only one pvalue.  In fact, it appears
  * to return a wrong answer in this case, as it cannot set BOTH vmin
  * AND vmax correctly, or so it appears.  So one solution is to just
  * return the one pvalue and skip the rest of the test.
  */
 if(count == 1) return pvalue[0];
 gsl_sort(pvalue,1,count);

 /*
  * Here's the test.  For each (sorted) pvalue, its index is the number of
  * values cumulated to the left of it.  v is the distance between that
  * number and the straight line representing a uniform accumulation.  We
  * save the maximum AND minimum v across all cumulated samples and
  * transform it into a p-value at the end.
  */
 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("    obs       exp           v        vmin         vmax\n");
 }
 vmin = pvalue[0];
 vmax = pvalue[0];
 for(i=0;i<count;i++){
   y = (double) i/count;
   v = pvalue[i] - y;
   /* can only do one OR the other here, not AND the other. */
   if(v > vmax) {
     vmax = v;
   } else if(v < vmin) {
     vmin = v;
   }
   if(verbose == D_KSTEST || verbose == D_ALL){
     printf("%8.3f   %8.3f    %16.6e   %16.6e    %16.6e\n",pvalue[i],y,v,vmin,vmax);
   }
 }
 v = fabs(vmax) + fabs(vmin);
 csqrt = sqrt(count);
 x = (csqrt + 0.155 + 0.24/csqrt)*v;
 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("Kuiper's V = %8.3f, evaluating q_ks_kuiper(%6.2f)\n",v,x);
 }
 p = q_ks_kuiper(x);

 if(verbose == D_KSTEST || verbose == D_ALL){
   if(p < 0.0001){
     printf("# kstest_kuiper(): Test Fails!  Visually inspect p-values:\n");
     for(i=0;i<count;i++){
       printf("# kstest_kuiper(): %3d    %10.5f\n",i,pvalue[i]);
     }
   }
 }

 return(p);

}

double q_ks_kuiper(double x)
{

 int i,sign;
 double qsum = 0;

 i = 0;
 if(x<0.4){
   qsum = 1.0;
   if(verbose == D_KSTEST || verbose == D_ALL){
     printf("(cutoff): Q_ks %d: %f\n",i,qsum);
   }
 } else {
   for(i=1;i<100;i++){
     qsum += (4.0*i*i*x*x - 1.0)*exp(-2.0*i*i*x*x);
     if(verbose == D_KSTEST || verbose == D_ALL){
       printf("Q_ks %d: %f\n",i,2.0*qsum);
     }
   }
 }

 if(verbose == D_KSTEST || verbose == D_ALL){
   printf("Q_ks returning %f\n",2.0*qsum);
 }
 return(2.0*qsum);

}

