/*
 *  rng_xoshiro32bit.c
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

static inline uint32_t rotl(const uint32_t x, int k) {
  return (x << k) | (x >> (32 - k));
}

/*
 * These are the wrappers for the xoshiro128 32bit prngs
 */
#define RNG_WRAP(n) \
  static unsigned long int n##_get (void *vstate);       \
  static double n##_get_double (void *vstate)

// 32bit
RNG_WRAP (xoshiro128_pp);
RNG_WRAP (xoshiro128_ss);
RNG_WRAP (xoshiro128_p);
RNG_WRAP (xoroshiro64_ss);
RNG_WRAP (xoroshiro64_s);
static void xoshiro32_set (void *vstate, unsigned long int s);

typedef struct
  {
    uint32_t s[4];
  }
xoshiro32_state_t;

/* This is xoshiro128++ 1.0, one of our 32-bit all-purpose, rock-solid
   generators. It has excellent speed, a state size (128 bits) that is
   large enough for mild parallelism, and it passes all tests we are aware
   of.

   For generating just single-precision (i.e., 32-bit) floating-point
   numbers, xoshiro128+ is even faster.

   The state must be seeded so that it is not everywhere zero. */
static inline unsigned long int
xoshiro128_pp_get (void *vstate)
{
  #define s state->s
  xoshiro32_state_t *state = (xoshiro32_state_t*)vstate;
  const uint32_t result = rotl(s[0] + s[3], 7) + s[0];
  const uint32_t t = s[1] << 9;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 11);
  #undef s
  return (unsigned long int)result;
}

/* This is xoshiro128** 1.1, one of our 32-bit all-purpose, rock-solid
   generators. It has excellent speed, a state size (128 bits) that is
   large enough for mild parallelism, and it passes all tests we are aware
   of.

   Note that version 1.0 had mistakenly s[0] instead of s[1] as state
   word passed to the scrambler.

   For generating just single-precision (i.e., 32-bit) floating-point
   numbers, xoshiro128+ is even faster.

   The state must be seeded so that it is not everywhere zero. */
static inline unsigned long int
xoshiro128_ss_get (void *vstate)
{
  #define s state->s
  xoshiro32_state_t *state = (xoshiro32_state_t*)vstate;
  const uint32_t result = rotl(s[1] * 5, 7) * 9;
  const uint32_t t = s[1] << 9;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 11);
  #undef s
  return (unsigned long int)result;
}

/* This is xoshiro128+ 1.0, our best and fastest 32-bit generator for 32-bit
   floating-point numbers. We suggest to use its upper bits for
   floating-point generation, as it is slightly faster than xoshiro128**.
   It passes all tests we are aware of except for
   linearity tests, as the lowest four bits have low linear complexity, so
   if low linear complexity is not considered an issue (as it is usually
   the case) it can be used to generate 32-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. */
static inline unsigned long int
xoshiro128_p_get (void *vstate)
{
  #define s state->s
  xoshiro32_state_t *state = (xoshiro32_state_t*)vstate;
  const uint32_t result = s[0] + s[3];
  const uint32_t t = s[1] << 9;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 11);
  #undef s
  return (unsigned long int)result;
}

/* This is xoroshiro64* 1.0, our best and fastest 32-bit small-state
   generator for 32-bit floating-point numbers. We suggest to use its
   upper bits for floating-point generation, as it is slightly faster than
   xoroshiro64**. It passes all tests we are aware of except for linearity
   tests, as the lowest six bits have low linear complexity, so if low
   linear complexity is not considered an issue (as it is usually the
   case) it can be used to generate 32-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. */
static inline unsigned long int
xoroshiro64_s_get (void *vstate)
{
  #define s state->s
  xoshiro32_state_t *state = (xoshiro32_state_t*)vstate;
  const uint32_t s0 = s[0];
  uint32_t s1 = s[1];
  const uint32_t result = s0 * 0x9E3779BB;

  s1 ^= s0;
  s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
  s[1] = rotl(s1, 13); // c
  #undef s
  return result;
}

/* This is xoroshiro64** 1.0, our 32-bit all-purpose, rock-solid,
   small-state generator. It is extremely fast and it passes all tests we
   are aware of, but its state space is not large enough for any parallel
   application.

   For generating just single-precision (i.e., 32-bit) floating-point
   numbers, xoroshiro64* is even faster.

   The state must be seeded so that it is not everywhere zero. */
static inline unsigned long int
xoroshiro64_ss_get (void *vstate)
{
  #define s state->s
  xoshiro32_state_t *state = (xoshiro32_state_t*)vstate;
  const uint32_t s0 = s[0];
  uint32_t s1 = s[1];
  const uint32_t result = rotl(s0 * 0x9E3779BB, 5) * 5;

  s1 ^= s0;
  s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
  s[1] = rotl(s1, 13); // c
  #undef s
  return result;
}

static double
xoshiro128_pp_get_double (void *vstate)
{
  return xoshiro128_pp_get(vstate) / (double) UINT32_MAX;
}
static double
xoshiro128_ss_get_double (void *vstate)
{
  return xoshiro128_ss_get(vstate) / (double) UINT32_MAX;
}
static double
xoshiro128_p_get_double (void *vstate)
{
  return xoshiro128_p_get(vstate) / (double) UINT32_MAX;
}
static double
xoroshiro64_ss_get_double (void *vstate)
{
  return xoroshiro64_ss_get(vstate) / (double) UINT32_MAX;
}
static double
xoroshiro64_s_get_double (void *vstate)
{
  return xoroshiro64_s_get(vstate) / (double) UINT32_MAX;
}

static void xoshiro32_set (void *vstate, unsigned long int s) {

 /* Initialize automaton using specified seed. */
 xoshiro32_state_t *state __attribute__((unused)) = (xoshiro32_state_t *) vstate;
 state->s[0] = random_seed() ^ s;
 state->s[1] = random_seed() ^ rotl(s, 4);
 state->s[2] = random_seed() ^ rotl(s, 8);
 state->s[3] = random_seed() ^ rotl(s, 12);
}

static const gsl_rng_type xoshiro128_pp_type =
{"xoshiro128++",                      /* name */
 UINT32_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro32_state_t),
 &xoshiro32_set,
 &xoshiro128_pp_get,
 &xoshiro128_pp_get_double};

static const gsl_rng_type xoshiro128_ss_type =
{"xoshiro128**",                      /* name */
 UINT32_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro32_state_t),
 &xoshiro32_set,
 &xoshiro128_ss_get,
 &xoshiro128_ss_get_double};

static const gsl_rng_type xoshiro128_p_type =
{"xoshiro128+",                      /* name */
 UINT32_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro32_state_t),
 &xoshiro32_set,
 &xoshiro128_p_get,
 &xoshiro128_p_get_double};

static const gsl_rng_type xoroshiro64_ss_type =
{"xoroshiro64**",                      /* name */
 UINT32_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro32_state_t),
 &xoshiro32_set,
 &xoroshiro64_ss_get,
 &xoroshiro64_ss_get_double};

static const gsl_rng_type xoroshiro64_s_type =
{"xoroshiro64*",                      /* name */
 UINT32_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (xoshiro32_state_t),
 &xoshiro32_set,
 &xoroshiro64_s_get,
 &xoroshiro64_s_get_double};

const gsl_rng_type *gsl_rng_xoshiro128_pp = &xoshiro128_pp_type;
const gsl_rng_type *gsl_rng_xoshiro128_ss = &xoshiro128_ss_type;
const gsl_rng_type *gsl_rng_xoshiro128_p  = &xoshiro128_p_type;
const gsl_rng_type *gsl_rng_xoroshiro64_ss = &xoroshiro64_ss_type;
const gsl_rng_type *gsl_rng_xoroshiro64_s  = &xoroshiro64_s_type;
