#! /bin/sh
#
# This is the standard validation run for the current (3.x) working
# snapshot of dieharder.  I'm going to start keeping this and its output
# in this directory under svn

# Generic Tmpfile plus automagic cleanup.
Tmpfile=$0.$$
trap 'rm $Tmpfile > /dev/null 2>&1; exit 0' 0 1 2 3 15

./dieharder -g 206 -S 1 -s 1 -m 10 -a | tee $Tmpfile

echo "Validating against standard validate.dat"
diff validate.dat $Tmpfile
echo "Done!"
mv validate.dat validate.dat.old
cp $Tmpfile validate.dat
