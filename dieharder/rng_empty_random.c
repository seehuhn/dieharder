/*
 * This is a wrapping of the /dev/random hardware rng
 */

#include "dieharder.h"


static unsigned long int dev_random_get (void *vstate);
static double dev_random_get_double (void *vstate);
static void dev_random_set (void *vstate, unsigned long int s);

typedef struct
  {
  }
dev_random_state_t;

static unsigned long int
empty_random_get (void *vstate)
{

}

static double
empty_random_get_double (void *vstate)
{
  return 0.0;
}

static void
empty_random_set (void *vstate, unsigned long int s)
{

 return;

}

static const gsl_rng_type empty_random_type =
{"empty",			/* name */
 0,				/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (dev_random_state_t),
 &empty_random_set,
 &empty_random_get,
 &empty_random_get_double};

const gsl_rng_type *gsl_rng_empty_random = &empty_random_type;
