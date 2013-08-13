/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */


/*
 * RDieHarder interface to DieHarder
 * Copyright (C) 2006 - 2008 Dirk Eddelbuettel
 * GPL'ed
 *
 * Based on dieharder.c from DieHarder, and interfacing DieHarder
 * DieHarder is Copyright 2002 - 2008 (C) Robert G. Brown and GPL'ed
 *
 */

#ifdef RDIEHARDER
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

#include "dieharder.h"		/* from the front-end sources */

SEXP dieharder(SEXP genS, SEXP testS, SEXP seedS, SEXP psamplesS, SEXP verbS, SEXP infileS, SEXP ntupleS) {

    int verb, testarg;
    unsigned int i;
    SEXP result = NULL, vec, pv, name, desc, nkps;
    char *inputfile;

    /* Setup argv to allow call of parsecl() to let dieharder set globals */
    char *argv[] = { "dieharder" };
    optind = 0;
    parsecl(1, argv); 			

    /* Parse 'our' parameters from R */
    generator  = INTEGER_VALUE(genS);
    testarg = INTEGER_VALUE(testS);
    diehard = rgb = sts = user = 0;
    if (testarg < 100) {
	diehard = testarg;
    } else if (testarg < 200) {
	rgb = testarg - 100;
    } else if (testarg < 300) {
	sts = testarg - 200;
    } else {
	user = testarg - 300;
    }
    Seed = (unsigned long int) INTEGER_VALUE(seedS); /* (user-select) Seed, not (save switch) seed */
    psamples = INTEGER_VALUE(psamplesS);
    verb = INTEGER_VALUE(verbS);
    inputfile = (char*) CHARACTER_VALUE(infileS);
    ntuple = INTEGER_VALUE(ntupleS);

    rdh_testptr = NULL;
    rdh_dtestptr = NULL; 	/* to be safe, explicitly flag as NULL; cf test in output.c */

    if (strcmp(inputfile, "") != 0) {
	strncpy(filename, inputfile, 128);
	fromfile = 1;			/* flag this as file input */
    }
 
   if (Seed == 0) {
    	seed = random_seed();
    } else {
    	seed = (unsigned long int) Seed;
    }

    if (verb) {
	Rprintf("Dieharder called with gen=%d test=%d seed=%lu\n", generator, diehard, seed);
	quiet = 0;
	hist_flag = 1;
    } else {
	quiet = 1; 			/* override dieharder command-line default */
	hist_flag = 0;
    }

    /* Now do the work that dieharder.c does */
    startup();
    work();
    gsl_rng_free(rng);
    reset_bit_buffers();

    /* And then bring our results back to R */

    /* create vector of size four: [0] is vector (!!) ks_pv, [1] is pvalues vec, [2] name, [3] nkps */
    PROTECT(result = allocVector(VECSXP, 4)); 

    /* alloc vector and scalars, and set it */
    PROTECT(pv = allocVector(REALSXP, rdh_dtestptr->nkps));
    PROTECT(name = allocVector(STRSXP, 1));
    PROTECT(nkps = allocVector(INTSXP, 1));

    if (rdh_testptr != NULL && rdh_dtestptr != NULL) {
	for (i=0; i<rdh_dtestptr->nkps; i++) { 		/* there can be nkps p-values per test */
	    REAL(pv)[i] = rdh_testptr[i]->ks_pvalue;
	}
	PROTECT(vec = allocVector(REALSXP, rdh_testptr[0]->psamples)); /* alloc vector and set it */
	for (i = 0; i < rdh_testptr[0]->psamples; i++) {
	    REAL(vec)[i] = rdh_testptr[0]->pvalues[i];
	}
	SET_STRING_ELT(name, 0, mkChar(rdh_dtestptr->name));
	INTEGER(nkps)[0] = rdh_dtestptr->nkps; 		/* nb of Kuiper KS p-values in pv vector */
    } else {
	PROTECT(vec = allocVector(REALSXP, 1)); 
	REAL(pv)[0] = R_NaN;
	REAL(vec)[0] = R_NaN;
	SET_STRING_ELT(name, 0, mkChar(""));
	INTEGER(nkps)[0] = R_NaN;
    }

    /* insert vectors and scalars into result vector */
    SET_VECTOR_ELT(result, 0, pv);
    SET_VECTOR_ELT(result, 1, vec);
    SET_VECTOR_ELT(result, 2, name);
    SET_VECTOR_ELT(result, 3, nkps);
  
    UNPROTECT(5);

    return result;
}


SEXP dieharderGenerators(void) {
    SEXP result, gens, genid;
    unsigned int i,j;

    /* from startup.c */
    /*
     * We new have to work a bit harder to determine how many
     * generators we have of the different dh_types because there are
     * different ranges for different sources of generator.
     *
     * We start with the basic GSL generators, which start at offset 0.
     */
    dh_types = dieharder_rng_dh_types_setup ();
    i = 0;
    while(dh_types[i] != NULL){
	i++;
	j++;
    }
    num_gsl_rngs = i;

    /*
     * Next come the dieharder generators, which start at offset 200.
     */
    i = 200;
    j = 0;
    while(dh_types[i] != NULL){
	i++;
	j++;
    }
    num_dieharder_rngs = j;

    /*
     * Next come the R generators, which start at offset 400.
     */
    i = 400;
    j = 0;
    while(dh_types[i] != NULL){
	i++;
	j++;
    }
    num_R_rngs = j;

    /*
     * Next come the hardware generators, which start at offset 500.
     */
    i = 500;
    j = 0;
    while(dh_types[i] != NULL){
	i++;
	j++;
    }
    num_hardware_rngs = j;

    /*
     * Finally, any generators added by the user at the interface level.
     * That is, if you are hacking dieharder to add your own rng, add it
     * below and it should "just appear" in the dieharder list of available
     * generators and be immediately useful.
     */
    i = 600;
    j = 0;
    dh_types[i] = gsl_rng_empty_random;  
    i++;
    j++;
    num_ui_rngs = j;

    /* /\* */
    /*  * Now add my own dh_types and count THEM. */
    /*  *\/ */
    /* add_ui_rngs(); */
    /* while(dh_types[i] != NULL){ */
    /* 	i++; */
    /* } */

    num_rngs = num_gsl_rngs + num_dieharder_rngs + num_R_rngs +
	num_hardware_rngs + num_ui_rngs;

    /* vector of size onetwo: [0] is scalar ks_pv, [1] is vector of pvalues */
    PROTECT(result = allocVector(VECSXP, 2)); 
    PROTECT(gens = allocVector(STRSXP, num_rngs)); 
    PROTECT(genid = allocVector(INTSXP, num_rngs)); 

    j = 0;
    for (i = 0; i < num_gsl_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 200; i < 200 + num_dieharder_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 400; i < 400 + num_R_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 500; i < 500 + num_hardware_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 600; i < 600 + num_ui_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    SET_VECTOR_ELT(result, 0, gens);
    SET_VECTOR_ELT(result, 1, genid);
  
    UNPROTECT(3);
    return result;
}
#endif   /* RDIEHARDER */
