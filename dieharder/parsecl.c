/*
 * $Id: parsecl.c 529 2011-04-01 17:49:31Z rgbatduke $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

#include "dieharder.h"
#include <getopt.h>
/*
 * parsecl parses the command line for the dieharder CLI to:
 *
 *  a) override the defaults set by set_globals().
 *  b) handle ill-formed command lines (usage errors)
 *  c) manage all autodocumentation features of dieharder (commands
 *     that cause any of the general output documentation panels to print).
 *
 * With the exception of the usage/documentation pathways (that all exit
 * on completion without executing any test) parsecl does no actual work.
 * Actual dieharder setup and task execution is handled by downstream
 * routines.
 */

char table_entry[TLENGTH];
int show_flags = 0;

void parsecl(int argc, char **argv)
{

 /*
  * Thus far, dieharder has used getopt() to parse the CL.  However, this
  * isn't as flexible as one might like.  To manage CLI-driven control
  * flags and a few other things, we may need to parse out options the
  * hard way in a future release.  The following variables are pretty much
  * all to support getopt and would change if we change parsing tools or
  * strategies.
  */

 int i,c,errflg=0;
 int itmp;
 int tflag_tmp = 0,dtest_tmp,gen_tmp=17;
 extern char *optarg;
 extern int optind, opterr, optopt;
 char *endptr;

 /*
  * If dieharder is executed by itself on a command line, print out help
  * and then exit.  dieharder cannot be run without options, in other
  * words.
  */
 if(argc == 1){
    Usage();
    exit(1); /* count this as an error */
 }

 while ((c = getopt(argc,argv,"aBc:D:d:Ff:g:hi:k:lL:m:n:oO:p:P:S:s:t:Vv:W:X:x:Y:y:Z:z:")) != EOF){
   switch (c){
     case 'a':
       all = YES;
       break;
     case 'B':
       binary = 1;
       break;
     case 'c':
       /* printf("Setting separator to %c\n",optarg[0]); */
       table_separator = (char) optarg[0];
       break;
     case 'D':
       /*
        * We only override the default table display flags if there is
        * actually an argument to -D.  I hope.
        */
       tflag_tmp =  strtol(optarg,&endptr,10);
       if(*endptr != 0){
         /*
	  * This trick will be useful for parsing both random
	  * number generator names and test names.  Try to convert
	  * the string to a number, but if the conversion fails
	  * then try to lookup a string instead and convert to
	  * the index/number needed.
	  */
	 for(i=0;i<TCNT;i++){
	   if(strncmp(optarg,table_fields[i],TLENGTH) == 0){
             /* printf("Setting table option %s.\n",optarg); */
	     /*
	      * If default is set explicitly, just add tflag_default
	      * to presumed zero tflag.
	      */
	     if(i == 0){
	       tflag = tflag_default;
	     } else {
               tflag_tmp = (int) pow(2,i-1);
	     }
	     break;
	   }
	 }
	 if(i == TCNT) {
	   fprintf(stderr,"Invalid -T %s option.\n",optarg);
	   exit(1);
	 }
       }
       if(tflag_tmp == 0){
         /*
	  * If default is set explicitly, just set tflag to the value of
	  * tflag_default.  This means one can ADD a feature (e.g. prefix)
	  * easily but don't add any twice!  Note that an undocumented
	  * feature is that one can subtract flag values from the default
	  * as well, but numerically only.
	  */
         tflag = tflag_default;
       } else {
         /*
	  * We accumulate tflag from zero.
	  */
         tflag += tflag_tmp;
       }
       break;
     case 'd':
       /*
        * We need to set dtest_num, either from a number or by searching
        * the dh_test_types[] set of tests for a string/name match.  We
	* also have to be sure to leave dtest_num negative if we set
	* dtest_name, since that is the signal to search for the number
	* later.
        */
       dtest_tmp =  strtol(optarg,&endptr,10);
       /* printf("optarg = %s, dtest_tmp = %d endptr = %s\n",optarg,dtest_tmp,endptr); */
       if(strncmp(optarg,endptr,1) == 0){
         strncpy(dtest_name,optarg,128);
       } else {
         dtest_num = dtest_tmp;
       }
       break;
     case 'F':
       show_flags = 1;
       break;
     case 'f':
       strncpy(filename,optarg,128);
       fromfile = 1;
       break;
     /*
      * The dieharder CLI supports a "super" XOR generator.  Multiple
      * instances (up to GVECMAX = 100) of generators and associated seeds
      * on the command line cause this generator to automagically be used.
      * It simply XOR's together the sequential output of all the
      * generators, (uint) word by word, and returns that as a standard
      * uint/udev gsl generator.  The parsing code below thus simply moves
      * the CLI entries into the appropriate global vectors, to be handled
      * by the choose_rng function later.
      */
     case 'g':
       gen_tmp =  strtol(optarg,&endptr,10);
       /* printf("optarg = %s, dtest_tmp = %d endptr = %s\n",optarg,dtest_tmp,endptr); */
       if(strncmp(optarg,endptr,1) == 0){
         strncpy(gnames[gvcount],optarg,128);
       } else {
         gnumbs[gvcount] = gen_tmp;
       }
       gvcount++;
       break;
     case 'h':
       help_flag = YES;
       break;
     case 'i':
       iterations = strtol(optarg,(char **) NULL,10);
       break;
     case 'k':
       ks_test = strtol(optarg,(char **) NULL,10);
       break;
     case 'l':
       list = YES;
       break;
     case 'L':
       overlap = strtol(optarg,(char **) NULL,10);
       break;
     case 'm':
       multiply_p = strtod(optarg,(char **) NULL);
       /*
        * Safety checking on multiply_p is done in std_test, where
        * more information (the number of p-samples) is available.
        */
       break;
     case 'n':
       ntuple = strtol(optarg,(char **) NULL,10);
       break;
     case 'o':
       output_file = 1;
       break;
     case 'O':
       output_format = strtol(optarg,(char **) NULL,10);
       if(output_format > 2){
         fprintf(stderr,"-O %u format not recognized.\n",output_format);
	 fprintf(stderr," Choices: 0 (binary), 1 (uint), 2 (decimal)\n");
       }
       break;
     case 'P':
       Xoff = strtol(optarg,(char **) NULL,10);
       break;
     case 'p':
       psamples = strtol(optarg,(char **) NULL,10);
       break;
     case 'S':
       Seed = strtol(optarg,(char **) NULL,10);
       gseeds[gscount] = gen_tmp;
       gscount++;
       break;
     case 's':
       strategy = strtol(optarg,(char **) NULL,10);
       break;
     case 't':
       itmp = strtol(optarg,(char **) NULL,10);
       /*
        * For a variety of reasons, we can't yet manage a full uint here,
        * nor can we deal with a negative number.  HOWEVER, we DO have
	* a really big ull tsamples, and we really SHOULD do our checking
	* for excessive size elsewhere.
        */
       if(itmp>=0){
         tsamples = itmp;
       } else {
         fprintf(stderr,"Warning!  tsamples cannot be negative = %d\n",itmp);
	 fprintf(stderr,"          using default tsamples value(s).\n");
	 fflush(stderr);
       }
       break;
     case 'V':
       dh_version();
       exit(0);
       break;
     case 'v':
       verbose = strtol(optarg,(char **) NULL,10);
       printf("# Verbose is now %d\n",verbose);
       break;
     case 'W':
       Xweak = strtod(optarg,(char **) NULL);
       break;
     case 'X':
       Xfail = strtod(optarg,(char **) NULL);
       break;
     case 'x':
       x_user = strtod(optarg,(char **) NULL);
       break;
     case 'Y':
       Xtrategy = strtol(optarg,(char **) NULL,10);
       break;
     case 'y':
       y_user = strtod(optarg,(char **) NULL);
       break;
     case 'z':
       z_user = strtod(optarg,(char **) NULL);
       break;
     case '?':
       errflg++;
   }
 }

 /*
  * By the time we get here, all the command line options should have
  * been processed, and we are ready to act on them.  There are a small
  * set of options that we act on right away below, mostly associated with
  * the autodocumenting features of the dieharder CLI (only).
  * Interfaces such as Rdieharder should just ifdef them out, or better
  * yet, not use this routine at all since it should invariably be faster
  * and easier to just set the options directly instead of building a
  * fake command line to hand off to parsecl().
  */

 /*
  * Some option was set incorrectly.
  */
 if(errflg){
   Usage();
   exit(0);
 }

 /*
  * If there are no more arguments it is a legal command.  We execute
  * some of the simpler, autodocumenting ones right here and then
  * immediately exit.
  */
 if(argc-optind == 0){

   /*
    * If a list of tests was requested, print it and exit.
    */
   if(list == YES) {
     list_tests();
     Exit(0);
   }

   /*
    * If a list of all the generators was requested, we give it, but
    * list_rngs() won't work until after the types[] list is built.
    * I could probably do this several ways, but this one is quite
    * simple and works fine.
    */
   if(generator == -1){
     list_rngs();
     Exit(0);
   }

   if(show_flags == 1){
     dh_header();
     fprintf(stdout," Dieharder output control flags.  Set with -D flagname or -D flag\n");
     fprintf(stdout," Multiple flags may be given:\t\tdieharder -a -D test_name -D pvalues.\n");
     fprintf(stdout," Flags names and numbers may be mixed:\tdieharder -a -D 8 -D pvalues.\n");
     fprintf(stdout," Flag\t    Flag Name\n");
     fprintf(stdout,"=======================\n");
     fprintf(stdout,"%5d\t%15s\n",0,table_fields[0]);
     for(i=0;i<TCNT;i++){
       fprintf(stdout,"%5d\t%15s\n",(int)pow(2,i),table_fields[i+1]);
     }
     Exit(0);
   }

   /*
    * If help was requested, call the help routine.  This routine does
    * different things depending on the other flags and variables, so
    * we defer calling it until they are all set.  help() will exit.
    */
   if(help_flag) help();

   /*
    * This one is tricky, but:  If tflag_tmp == 0, it means that either we
    * did not get any -D flags OR that we ONLY got -D 0.  Either way, we
    * have to set tflag to the default if we get here.  The one "odd"
    * behavior this will produce is that -D 8 -D 0 will effectively be -D
    * 0, where -D 0 -D -8 will actually turn OFF feature 8 from the
    * default list.  I don't plan to document this further anywhere but
    * here, but I may use it...;-)
    */
   if(tflag_tmp == 0){
     tflag = tflag_default;
   }

   /*
    * If we get here, it is time to move on and execute dieharder
    */
   return;

 }

 /*
  * Leftover commands on the command line are errors and should trigger
  * the standard Usage message and exit.
  */

 Usage();
 

}

