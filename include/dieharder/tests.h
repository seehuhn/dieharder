/*
 *========================================================================
 * $Id: libdieharder.h 221 2006-08-16 22:43:03Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */
#include <dieharder/rgb_timing.h>
#include <dieharder/rgb_persist.h>
#include <dieharder/rgb_bitdist.h>
#include <dieharder/rgb_kstest_test.h>
#include <dieharder/rgb_lagged_sums.h>
#include <dieharder/rgb_minimum_distance.h>
#include <dieharder/rgb_operm.h>
#include <dieharder/rgb_permutations.h>
#include <dieharder/dab_bytedistrib.h>
#include <dieharder/dab_dct.h>
#include <dieharder/dab_filltree.h>
#include <dieharder/dab_filltree2.h>
#include <dieharder/dab_monobit2.h>
#include <dieharder/diehard_birthdays.h>
#include <dieharder/diehard_operm5.h>
#include <dieharder/diehard_rank_32x32.h>
#include <dieharder/diehard_rank_6x8.h>
#include <dieharder/diehard_bitstream.h>
#include <dieharder/diehard_opso.h>
#include <dieharder/diehard_oqso.h>
#include <dieharder/diehard_dna.h>
#include <dieharder/diehard_count_1s_stream.h>
#include <dieharder/diehard_count_1s_byte.h>
#include <dieharder/diehard_parking_lot.h>
#include <dieharder/diehard_2dsphere.h>
#include <dieharder/diehard_3dsphere.h>
#include <dieharder/diehard_squeeze.h>
#include <dieharder/diehard_sums.h>
#include <dieharder/diehard_runs.h>
#include <dieharder/diehard_craps.h>
#include <dieharder/marsaglia_tsang_gcd.h>
#include <dieharder/sts_monobit.h>
#include <dieharder/sts_runs.h>
#include <dieharder/sts_serial.h>

/*
#include <dieharder/marsaglia_tsang_gorilla.h>
#include <dieharder/rgb_lmn.h>
*/

 /* Diehard Tests (by number) */
 typedef enum {
   DIEHARD_NONE,
   DIEHARD_BDAY,
   DIEHARD_OPERM5,
   DIEHARD_RANK_32x32,
   DIEHARD_RANK_6x8,
   DIEHARD_BITSTREAM,
   DIEHARD_OPSO,
   DIEHARD_OQSO,
   DIEHARD_DNA,
   DIEHARD_COUNT_1S_STREAM,
   DIEHARD_COUNT_1S_BYTE,
   DIEHARD_PARKING_LOT,
   DIEHARD_2DSPHERE,
   DIEHARD_3DSPHERE,
   DIEHARD_SQUEEZE,
   DIEHARD_SUMS,
   DIEHARD_RUNS,
   DIEHARD_CRAPS,
   MARSAGLIA_TSANG_GCD,
   MARSAGLIA_TSANG_GORILLA,
   N_DIEHARD_TESTS
 } Diehard_Tests;

 /* RGB Tests (by number) */
 typedef enum {
   RGB_NONE,
   RGB_TIMING,
   RGB_PERSIST,
   RGB_BITDIST,
   RGB_MINIMUM_DISTANCE,
   RGB_PERMUTATIONS,
   RGB_LAGGED_SUMS,
   RGB_LMN,
   RGB_OPERM,
   DAB_BYTEDISTRIB,
   DAB_DCT,
   DAB_FILLTREE,
   DAB_FILLTREE2,
   DAB_MONOBIT2,
   N_RGB_TESTS
 } Rgb_Tests;

 typedef enum {
   STS_NONE,
   STS_MONOBIT,
   STS_RUNS,
   STS_SERIAL,
   N_STS_TESTS
 } Sts_Tests;

 /*
  * Add your own test types here!  Use/rename/copy template here and in
  * the subroutine prototypes below.  Note also the D_USER_TEMPLATE in
  * the Debug enum, in case you want to add controllable I/O to help
  * you debug with the -v flag.
  */
 typedef enum {
   USER_NONE,
   USER_TEMPLATE,
   N_USER_TESTS
 } User_Tests;


