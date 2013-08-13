#! /bin/sh

# This script runs all the tests the hard way, with more psamples.  Ones
# that fail are probably BAD TESTS given that it is difficult to conclude
# that this is likely to be a bad generator.  It passes all tests with their
# default number of psamples (usually 100).

# Diehard tests
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 0 -D default -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 1 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 2 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 3 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 4 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 5 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 6 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 7 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 8 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 9 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 10 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 11 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 12 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 13 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 14 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 15 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 16 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 17 -D default -D -5 -D histogram
# STS tests.  Several of these require runs over ntuples
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 100 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 101 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 102 -D default -D -5 -D histogram
# rgb tests.  Several of these require runs over ntuples or other additional
# arguments.
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 200 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 201 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 202 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -p 1000 -d 203 -D default -D -5 -D histogram
cat /dev/zero | aespipe -P aeskey | ./dieharder -g 200 -t 1000 -p 1000 -d 204 -D default -D -5 -D histogram
