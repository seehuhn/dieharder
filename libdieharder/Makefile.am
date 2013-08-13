#========================================================================
# This is a Makefile.am for the libdieharder library, part of the
# overall dieharder package.  It pretty much sets up standard library
# 
# These two targets execute in the toplevel project directory only.
# make svn      does a svn commit and creates the timestamp $(SVNTIME)
# make sync     does a svn commit and rsyncs the project repo to a 
#               list of hosts and also creates $(SVNTIME)
#========================================================================

# This is automagically set in the toplevel build.  Do not edit by
# hand.
VERSION=@VERSION@
LITTLE_ENDIAN=@LITTLE_ENDIAN@
# VERSION_MAJOR = $(shell expr $(VERSION) : '\(.*\)\..*\..*' \| $(VERSION))
# RELEASE=0

# Man page
man3_MANS = libdieharder.3

# Library definitions
lib_LTLIBRARIES = libdieharder.la
libdieharder_la_LIBADD = -lgsl -lgslcblas -lm
libdieharder_la_LDFLAGS = -version-number @DIEHARDER_LT_VERSION@
libdieharder_la_CFLAGS = -std=c99 -Wall -pedantic

# Sources
# The shell script is the easy way to do this, by far.  But it may not
# be sufficiently portable.
# libdieharder_la_SOURCES = $(shell ls *.c  2>&1 | sed -e "/\/bin\/ls:/d")
libdieharder_la_SOURCES = \
	bits.c \
	chisq.c \
	countx.c \
	dab_bytedistrib.c \
	dab_dct.c \
	dab_filltree.c \
	dab_filltree2.c \
	dab_monobit2.c \
	diehard_2dsphere.c \
	diehard_3dsphere.c \
	diehard_birthdays.c \
	diehard_bitstream.c \
	diehard_count_1s_byte.c \
	diehard_count_1s_stream.c \
	diehard_craps.c \
	diehard_dna.c \
	diehard_operm5.c \
	diehard_opso.c \
	diehard_oqso.c \
	diehard_parking_lot.c \
	diehard_rank_32x32.c \
	diehard_rank_6x8.c \
	diehard_runs.c \
	diehard_squeeze.c \
	diehard_sums.c \
	dieharder_rng_types.c \
	dieharder_test_types.c \
	histogram.c \
	kstest.c \
	marsaglia_tsang_gcd.c \
	marsaglia_tsang_gorilla.c \
	parse.c \
	prob.c \
	random_seed.c \
	rank.c \
	rgb_bitdist.c \
	rgb_kstest_test.c \
	rgb_lagged_sums.c \
	rgb_minimum_distance.c \
	rgb_permutations.c \
	rgb_persist.c \
	rgb_timing.c \
	rngav.c \
	rng_aes.c \
	rijndael-alg-fst.c \
	rng_ca.c \
	rng_dev_random.c \
	rng_dev_arandom.c \
	rng_dev_urandom.c \
	rng_file_input.c \
	rng_file_input_raw.c \
	rngs_gnu_r.c \
	rng_kiss.c \
	rng_jenkins.c \
	rng_superkiss.c \
	rng_stdin_input_raw.c \
	rng_threefish.c \
	rng_uvag.c \
	rng_XOR.c \
	sample.c \
	skein_block64.c \
	skein_block_ref.c \
	std_test.c \
	sts_monobit.c \
	sts_runs.c \
	sts_serial.c \
	timing.c \
	Vtest.c \
	version.c \
	Xtest.c

#	rgb_operm.c
#	rgb_lmn.c

# The program/library may need to know what version it is, define
# the VERSION macro.
DEFINES = -DVERSION=$(VERSION) -DLITTLE_ENDIAN=$(LITTLE_ENDIAN)

#========================================================================
# Define parameters and directives needed in compile/link steps.
#========================================================================
# C Compiler.  Change only if you must.
# CC = gcc

# Compile flags (use fairly standard -O3 as default)
AM_CPPFLAGS = $(DEFINES) -I ${top_srcdir}/include -I ${includedir}
AM_CFLAGS = -O3 -Wall -pedantic -Wno-unused-variable 

#========================================================================
# The only safe place to do commits is in the toplevel directory
#========================================================================
SVNTREE = $(HOME)/Src/svn-tree
SVNPATH = $(SVNTREE)/dieharder
SVNTIME = dieharder.svn.time

svn:
	(cd ..;make svn)

sync:
	(cd ..;make sync)

