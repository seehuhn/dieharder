/*
 *
 * See copyright in copyright.h and the accompanying file COPYING
 * See also accompanying file STS.COPYING
 *
 */

/*
 *========================================================================
 * Output a list of random numbers of the selected type into a file.
 * Write a nice informative header, as well, to indicate several things
 * about the list (type, generator, count etc.)
 *========================================================================
 */

#include "dieharder.h"

void output_rnds()
{

 unsigned int i,j;
 double d;
 FILE *fp;

 if(verbose) {
   fprintf(stderr,"# output_rnds: Dumping %lu rands\n",tsamples);
 }

 /*
  * If Seed is set, use it.  Otherwise reseed from /dev/random
  */
 if(Seed){
   seed = Seed;
   if(verbose) {
     fprintf(stderr,"# output_rnds: seeding rng %s with %lu\n",gsl_rng_name(rng),seed);
   }
   gsl_rng_set(rng,seed);
 } else {
   seed = random_seed();
   if(verbose) {
     fprintf(stderr,"# output_rnds: seeding rng %s with %lu\n",gsl_rng_name(rng),seed);
   }
   gsl_rng_set(rng,seed);
 }

 /*
  * Open the output file.  If no filename is specified, or if
  * filename is "-", use stdout.
  */
 if(verbose) {
   fprintf(stderr,"# output_rnds: Opening file %s\n",filename);
 }
 if( (filename[0] == 0) || (strncmp("-",filename,1)==0) ){
   fp = stdout;
 } else {
   if ((fp = fopen(filename,"w")) == NULL) {
     fprintf(stderr,"Error: Cannot open %s, exiting.\n",filename);
     exit(0);
   }
 }

 if(verbose) {
   fprintf(stderr,"# output_rnds: Opened %s\n",filename);
 }
 /*
  * We completely change the way we control output.
  *
  *   -O output_format
  *      output_format = 0 (binary), 1 (uint), 2 (decimal)
  *
  * We just do a case switch, since each of them has its own
  * peculiarities.
  */
 switch(output_format){
   case 0:
     if(verbose) {
       fprintf(stderr,"Ascii values of binary data being written into file %s:\n",filename);
     }
     /*
      * make the samples and output them.  If we run binary with tsamples
      * = 0, we just loop forever or until the program is interrupted by
      * hand.
      */
     if(tsamples > 0){
       for(i=0;i<tsamples;i++){
         j = gsl_rng_get(rng);
         fwrite(&j,sizeof(unsigned int),1,fp);
         /*
          * Printing to stderr lets me read it and pass the binaries on through
          * to stdout and a pipe.
          */
         if(verbose) {
           fprintf(stderr,"%10u\n",j);
         }
       }
     } else {
       /*
        * If tsamples = 0, just pump them into stdout.  One HOPES that this
        * blocks when out goes into a pipe -- but that's one of the
        * questions I need to resolve.  This will make an infinite number of
        * binary rands (until the pipe is broken and this instance of
        * dieharder dies).
        */
       while(1){
         j = gsl_rng_get(rng);
         fwrite(&j,sizeof(unsigned int),1,fp);
         /*
          * Printing to stderr lets me read it and pass the binaries on through
          * to stdout and a pipe.
          */
         if(verbose) {
           fprintf(stderr,"%10u\n",j);
         }
       }
     }
     break;
   case 1:
     fprintf(fp,"#==================================================================\n");
     fprintf(fp,"# generator %s  seed = %lu\n",gsl_rng_name(rng),seed);
     fprintf(fp,"#==================================================================\n");
     fprintf(fp,"type: d\ncount: %lu\nnumbit: 32\n",tsamples);
     for(i=0;i<tsamples;i++){
       j = gsl_rng_get(rng);
       fprintf(fp,"%10u\n",j);
     }
     break;
   case 2:
     fprintf(fp,"#==================================================================\n");
     fprintf(fp,"# generator %s  seed = %lu\n",gsl_rng_name(rng),seed);
     fprintf(fp,"#==================================================================\n");
     fprintf(fp,"type: f\ncount: %lu\nnumbit: 32\n",tsamples);
     for(i=0;i<tsamples;i++){
       d = gsl_rng_uniform(rng);
       fprintf(fp,"%0.10f\n",d);
     }
     break;

 }

 fclose(fp);

}

