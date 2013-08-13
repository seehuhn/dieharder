#! /bin/sh

# Run this to generate all the auto-generated files needed by the GNU
# configure program.  This must be done FIRST; note well that there is
# no Makefile to run until it is done.

# echo "Running autoheader..."
# autoheader
# echo "Running aclocal..."
# aclocal
# echo "Running libtoolize..."
# libtoolize
# rm -f Makefile Makefile.in dieharder/Makefile dieharder/Makefile.in \
#    libdieharder/Makefile libdieharder/Makefile.in
# echo "Running automake..."
# automake --add-missing --copy --gnu

echo "Running autoreconf..."
autoreconf

#
# Might as well run this -- then we are ready to run make.
#
echo "Running default ./configure --prefix=/usr"
./configure --prefix=/usr
echo "=================================================================="
echo ""
echo " Makefile should be built, and ./configure --prefix=/usr should be"
echo " completed.  You may rerun configure with a different e.g."
echo "--prefix if you want to put dieharder someplace else.  Otherwise"
echo "you should be able to make, make install, or make rpm."
echo ""
echo "=================================================================="
