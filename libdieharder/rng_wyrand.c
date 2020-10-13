/*
 *  rng_wyrand.c
 *  From git://github.com/wangyi-fudan/wyhash.git
 *
 *  Copyright(c) 2020 Reini Urban <rurban@cpan.org>
 */

#include <dieharder/libdieharder.h>
#include "wyhash.h"

/*
 * This is a wrapping of the wyhash.h rng
 */

static unsigned long int wyrand_get (void *vstate);
static double wyrand_get_double (void *vstate);
static void wyrand_set (void *vstate, unsigned long int s);

typedef struct
  {
    uint64_t i;
  }
wyrand_state_t;

/*
 * WYHASH specific defines.
 *
 * WYHASH provides an effectively infinite sequence of 64bit values.
 */

/*
 * Global variables and data for WYHASH
 */

static inline unsigned long int
wyrand_get (void *vstate)
{

  /*
   * Returns a 64-bit unsigned integer produced by wyrand
   */
  return (unsigned long int)wyrand(&((wyrand_state_t*)vstate)->i);
}

static double
wyrand_get_double (void *vstate)
{
  return wy2u01(wyrand_get(vstate));
}

static void wyrand_set (void *vstate, unsigned long int s) {

 /* Initialize automaton using specified seed. */
 wyrand_state_t *state __attribute__((unused)) = (wyrand_state_t *) vstate;
 state->i = (uint64_t)s;
}

static const gsl_rng_type wyrand_type =
{"wyrand",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (wyrand_state_t),
 &wyrand_set,
 &wyrand_get,
 &wyrand_get_double};

const gsl_rng_type *gsl_rng_wyrand = &wyrand_type;
