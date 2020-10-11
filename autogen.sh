#! /bin/sh

case `uname` in
    Darwin*)
	LIBTOOLIZE=glibtoolize
	;;
    *)
	LIBTOOLIZE=libtoolize
	;;
esac
$LIBTOOLIZE --copy

#autoheader
#aclocal
autoreconf -fi
automake --add-missing --copy --gnu
autoconf
