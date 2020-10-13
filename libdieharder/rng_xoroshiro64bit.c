/*
 *  rng_xoroshiro64bit.c
 *  From http://prng.di.unimi.it/
 *
 *  Copyright(c) 2020 Reini Urban <rurban@cpan.org>
 */

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <dieharder/libdieharder.h>

static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (32 - k));
}

/*
 * These are the wrappers for the xoroshiro128 64bit prngs
 */
#define RNG_WRAP(n) \
  static unsigned long int n##_get (void *vstate);       \
  static double n##_get_double (void *vstate)

// 64bit
RNG_WRAP (xoroshiro128_pp);
RNG_WRAP (xoroshiro128_ss);
RNG_WRAP (xoroshiro128_p);
RNG_WRAP (xoshiro256_pp);
RNG_WRAP (xoshiro256_ss);
RNG_WRAP (xoshiro256_p);
static void xoroshiro64_set (void *vstate, unsigned long int s);

typedef struct
  {
    uint64_t s[4];
  }
xoshiro64_state_t;

/* This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-point numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
static inline unsigned long int
xoroshiro128_pp_get (void *vstate)
{
  #define s state->s
  xoshiro64_state_t *state = (xoshiro64_state_t*)vstate;
  const uint64_t s0 = s[0];
  uint64_t s1 = s[1];
  const uint64_t result = rotl(s0 + s1, 17) + s0;

  s1 ^= s0;
  s[0] = rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
  s[1] = rotl(s1, 28); // c
  #undef s
  return (unsigned long int)result;
}

/* This is xoroshiro128** 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-point numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
static inline unsigned long int
xoroshiro128_ss_get (void *vstate)
{
  #define s state->s
  xoshiro64_state_t *state = (xoshiro64_state_t*)vstate;
  const uint64_t s0 = s[0];
  uint64_t s1 = s[1];
  const uint64_t result = rotl(s0 * 5, 7) * 9;

  s1 ^= s0;
  s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
  s[1] = rotl(s1, 37); // c
  #undef s
  return (unsigned long int)result;
}

/* This is xoroshiro128+ 1.0, our best and fastest small-state generator
   for floating-point numbers. We suggest to use its upper bits for
   floating-point generation, as it is slightly faster than
   xoroshiro128++/xoroshiro128**. It passes all tests we are aware of
   except for the four lower bits, which might fail linearity tests (and
   just those), so if low linear complexity is not considered an issue (as
   it is usually the case) it can be used to generate 64-bit outputs, too;
   moreover, this generator has a very mild Hamming-weight dependency
   making our test (http://prng.di.unimi.it/hwd.php) fail after 5 TB of
   output; we believe this slight bias cannot affect any application. If
   you are concerned, use xoroshiro128++, xoroshiro128** or xoshiro256+.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. 

   NOTE: the parameters (a=24, b=16, b=37) of this version give slightly
   better results in our test than the 2016 version (a=55, b=14, c=36).
*/
static inline unsigned long int
xoroshiro128_p_get (void *vstate)
{
  #define s state->s
  xoshiro64_state_t *state = (xoshiro64_state_t*)vstate;
  const uint64_t s0 = s[0];
  uint64_t s1 = s[1];
  const uint64_t result = s0 + s1;

  s1 ^= s0;
  s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
  s[1] = rotl(s1, 37); // c

  #undef s
  return (unsigned long int)result;
}

/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
   It has excellent (sub-ns) speed, a state (256 bits) that is large
   enough for any parallel application, and it passes all tests we are
   aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
static inline unsigned long int
xoshiro256_pp_get (void *vstate)
{
  #define s state->s
  xoshiro64_state_t *state = (xoshiro64_state_t*)vstate;
  const uint64_t result = rotl(s[0] + s[3], 23) + s[0];

  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);
  #undef s
  return (unsigned long int)result;
}
/* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
   generators. It has excellent (sub-ns) speed, a state (256 bits) that is
   large enough for any parallel application, and it passes all tests we
   are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
static inline unsigned long int
xoshiro256_ss_get (void *vstate)
{
  #define s state->s
  xoshiro64_state_t *state = (xoshiro64_state_t*)vstate;
  const uint64_t result = rotl(s[1] * 5, 7) * 9;

  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);
  #undef s
  return (unsigned long int)result;
}

/* This is xoshiro256+ 1.0, our best and fastest generator for floating-point
   numbers. We suggest to use its upper bits for floating-point
   generation, as it is slightly faster than xoshiro256++/xoshiro256**. It
   passes all tests we are aware of except for the lowest three bits,
   which might fail linearity tests (and just those), so if low linear
   complexity is not considered an issue (as it is usually the case) it
   can be used to generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
static inline unsigned long int
xoshiro256_p_get (void *vstate)
{
  #define s state->s
  xoshiro64_state_t *state = (xoshiro64_state_t*)vstate;
  const uint64_t result = s[0] + s[3];

  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);  
  #undef s
  return (unsigned long int)result;
}

#define TO_DOUBLE(x)  ((x) >> 11) * 0x1.0p-53

static double
xoroshiro128_pp_get_double (void *vstate)
{
  return TO_DOUBLE(xoroshiro128_pp_get(vstate));
}
static double
xoroshiro128_ss_get_double (void *vstate)
{
  return TO_DOUBLE(xoroshiro128_ss_get(vstate));
}
static double
xoroshiro128_p_get_double (void *vstate)
{
  return TO_DOUBLE(xoroshiro128_p_get(vstate));
}
static double
xoshiro256_pp_get_double (void *vstate)
{
  return TO_DOUBLE(xoshiro256_pp_get(vstate));
}
static double
xoshiro256_ss_get_double (void *vstate)
{
  return TO_DOUBLE(xoshiro256_ss_get(vstate));
}
static double
xoshiro256_p_get_double (void *vstate)
{
  return TO_DOUBLE(xoshiro256_p_get(vstate));
}

static void xoroshiro64_set (void *vstate, unsigned long int s) {

 /* Initialize automaton using specified seed. */
 xoshiro64_state_t *state __attribute__((unused)) = (xoshiro64_state_t *) vstate;
 state->s[0] = random_seed() ^ s;
 state->s[1] = random_seed() ^ rotl(s, 8);
 state->s[2] = random_seed() ^ rotl(s, 16);
 state->s[3] = random_seed() ^ rotl(s, 24);
}

static const gsl_rng_type xoroshiro128_pp_type =
{"xoroshiro128++",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro64_state_t),
 &xoroshiro64_set,
 &xoroshiro128_pp_get,
 &xoroshiro128_pp_get_double};

static const gsl_rng_type xoroshiro128_ss_type =
{"xoroshiro128**",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro64_state_t),
 &xoroshiro64_set,
 &xoroshiro128_ss_get,
 &xoroshiro128_ss_get_double};

static const gsl_rng_type xoroshiro128_p_type =
{"xoroshiro128+",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro64_state_t),
 &xoroshiro64_set,
 &xoroshiro128_p_get,
 &xoroshiro128_p_get_double};

static const gsl_rng_type xoshiro256_pp_type =
{"xoshiro256++",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro64_state_t),
 &xoroshiro64_set,
 &xoshiro256_pp_get,
 &xoshiro256_pp_get_double};

static const gsl_rng_type xoshiro256_ss_type =
{"xoshiro256**",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro64_state_t),
 &xoroshiro64_set,
 &xoshiro256_ss_get,
 &xoshiro256_ss_get_double};

static const gsl_rng_type xoshiro256_p_type =
{"xoshiro256+",                      /* name */
 UINT64_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro64_state_t),
 &xoroshiro64_set,
 &xoshiro256_p_get,
 &xoshiro256_p_get_double};

const gsl_rng_type *gsl_rng_xoroshiro128_pp = &xoroshiro128_pp_type;
const gsl_rng_type *gsl_rng_xoroshiro128_ss = &xoroshiro128_ss_type;
const gsl_rng_type *gsl_rng_xoroshiro128_p  = &xoroshiro128_p_type;
const gsl_rng_type *gsl_rng_xoshiro256_pp = &xoshiro256_pp_type;
const gsl_rng_type *gsl_rng_xoshiro256_ss = &xoshiro256_ss_type;
const gsl_rng_type *gsl_rng_xoshiro256_p  = &xoshiro256_p_type;
