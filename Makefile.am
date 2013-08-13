## Process this file with automake to produce Makefile.in
# These are made, in order presented, from the toplevel make
SUBDIRS = libdieharder dieharder include

# These are automagically included in the make dist rule.  I
# do believe that I'm going to have to eliminate my own version
# of this below.  Sigh.  Time to actually learn to use the GBT
# properly, I suppose.
# EXTRA_DIST = autogen.sh dieharder-config.in configure.ac \
#  THANKS BUGS SUPPORT dieharder.spec.in dieharder.m4 test_dieharder.sh
EXTRA_DIST = autogen.sh set_ld_library_path Copyright

#========================================================================
# This is the toplevel Makefile for the dieharder project.  It has
# some specialized targets:
#
# make          alone should build the entire application as usual
# make install  should install it.  Use configure --prefix= to specify
#               a toplevel path other than the default /usr.
#
# make tgz      makes $(TGZ) of entire tree for standalone or rpm build
# make rpm      makes $(RPM) packages built by dieharder.spec
#
#========================================================================
# These are targets just for me to facilitate project management.
#========================================================================
# make svn      does a svn commit and creates the timestamp $(SVNTIME)
# make sync     does a svn commit and syncs to list of svn-tree hosts
# make installweb  installs to project website for download
# make installrepo installs to yum repo
#
#========================================================================
PROJECT = dieharder
LIBRARY = libdieharder
LIBTIME = libwulf.time
INCTIME = include.time
PROGRAM = dieharder
PROGTIME = dieharder.time
MANUAL = manual
MANTIME = manual.time
RDIEHARDER = RDieHarder

#========================================================================
# Constructs used in project maintenance and installation.
#
# The destination on an ssh-accessible remote webserver $(WLOGIN), 
# used as:
#    $(HOME)/public_html/$(WDIR)
#========================================================================
WLOGIN = ganesh.phy.duke.edu
WDIR = General
ABS = $(PROJECT).abs
PHP = $(PROJECT).php


#========================================================================
# This is revision information, automagically set from autoconf.  The
# actual version information is set ONLY in configure.ac
#========================================================================
VERSION=@VERSION@
RELEASE=@RELEASE@
LITTLE_ENDIAN=@LITTLE_ENDIAN@

#========================================================================
# RPM/tarball target objects.  We need rules for all of these.
#========================================================================
PROJECTDIR = $(PROJECT)-$(VERSION)
TAR = $(PROJECTDIR).tar
TGZ = $(PROJECTDIR).tgz
SPEC = $(PROJECT).spec

#========================================================================
# List of variants one can make.  all is the default.  We always
# presume the simplest of dependencies and remake if includes change
# for example.
#========================================================================
all: $(LIBTIME) $(PROGTIME) $(INCTIME)

# This is not, actually, a particularly useful toplevel target.  To
# work correctly it also would require a full parsing of all
# lower level dependencies.  I'm leaving it in for the moment just
# to have a default target at the toplevel that CAN be used to test.
$(LIBTIME):
	(cd $(LIBRARY); \
	make)

$(INCTIME):
	(cd include; \
	make)

$(PROGTIME):
	(cd $(PROGRAM); \
	make)

$(ABS): Makefile
	cat $(ABS) | \
	sed -e 's/^\(<center><H2>Version \)\(.*\)/\1$(VERSION)<\/H2><\/center>/' > /tmp/$(ABS).$$
	mv /tmp/$(ABS).$$ $(ABS)

#========================================================================
# This is a required target for both its own sake and to support the
# rpm build.  It has to run unconditionally when called.  Note that we
# make PRECISELY what we need in terms of the source directories,
# excluding all restricted material and irrelevant data.
#========================================================================
tgz: Makefile COPYING Copyright NOTES README $(SPEC) $(ABS) $(PHP)
	( rm -rf $(TAR) $(TGZ) $(PROJECTDIR); \
	mkdir -p $(PROJECTDIR); \
	cd $(LIBRARY); \
	make clean; \
	cd ..; \
	cp -r $(LIBRARY) $(PROJECTDIR); \
	cd $(PROGRAM); \
	make clean; \
	cd ..; \
	cp -r $(PROGRAM) $(PROJECTDIR); \
	cd $(MANUAL); \
	make clean; \
	cd ..; \
	cp -r $(MANUAL) $(PROJECTDIR); \
	cp -r include $(PROJECTDIR); \
	cp -r m4 $(PROJECTDIR); \
	cp $(ABS) $(PROJECTDIR); \
	cp $(PHP) $(PROJECTDIR); \
	cp Makefile $(PROJECTDIR); \
	cp Makefile.am $(PROJECTDIR); \
	cp Makefile.in $(PROJECTDIR); \
	cp configure $(PROJECTDIR); \
	cp config.h.in $(PROJECTDIR); \
	cp $(SPEC).in $(PROJECTDIR); \
	cp dieharder_version.h.in $(PROJECTDIR); \
	cp dieharder.html.in $(PROJECTDIR); \
	cp dieharder-config.in $(PROJECTDIR); \
	cp autogen.sh $(PROJECTDIR); \
	cp missing $(PROJECTDIR); \
	cp aclocal.m4 $(PROJECTDIR); \
	cp mkinstalldirs $(PROJECTDIR); \
	cp ltmain.sh $(PROJECTDIR); \
	cp depcomp $(PROJECTDIR); \
	cp configure $(PROJECTDIR); \
	cp configure.ac $(PROJECTDIR); \
	cp config.guess $(PROJECTDIR); \
	cp config.sub $(PROJECTDIR); \
	cp config.status $(PROJECTDIR); \
	cp install-sh $(PROJECTDIR); \
	cp Copyright $(PROJECTDIR); \
	cp COPYING $(PROJECTDIR); \
	cp INSTALL $(PROJECTDIR); \
	cp AUTHORS $(PROJECTDIR); \
	cp NEWS $(PROJECTDIR); \
	cp ChangeLog $(PROJECTDIR); \
	cp README $(PROJECTDIR); \
	cp NOTES $(PROJECTDIR); \
	tar -cvpf $(TAR) \
            --exclude=.svn \
	    --exclude=Cruft \
	    --exclude=Exclude \
            --exclude=*.tar \
            --exclude=*.tgz \
            --exclude=*.rpm \
            ./$(PROJECTDIR); \
	gzip $(TAR); \
	mv $(TAR).gz $(TGZ); \
	rm -rf $(PROJECTDIR))

LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

#========================================================================
# rpm target special stuff
#
# To work in userspace, add the following:
# %_topdir	/home/rgb/Src/redhat
# to your personal $(HOME)/.rpmmacros after building
# yourself a private copy of the /usr/src/redhat directory structure.
#
# RPM_TOPDIR=/usr/src/redhat
RPM_TOPDIR=$(HOME)/Src/rpm_tree

# This is needed to get the right library and binary rpm.
ARCH=`uname -i`
# ARCH=i386
# These are the three rpms automagically built by the spec
SSRPM = $(PROJECT)-src-$(VERSION)-$(RELEASE).src.rpm
SRPM = $(PROJECT)-$(VERSION)-$(RELEASE).src.rpm
LRPM = libdieharder-$(VERSION)-$(RELEASE).$(ARCH).rpm
PRPM = dieharder-$(VERSION)-$(RELEASE).$(ARCH).rpm

$(TGZ): tgz
$(SRPM): rpm
$(LRPM): rpm
$(PRPM): rpm

#========================================================================
# One stop shop.  Basically we build this every time, we hope.
rpm:	Makefile $(TGZ)
	rm -rf /var/tmp/dieharder*
	cp $(TGZ) $(RPM_TOPDIR)/SOURCES
	cp $(SPEC) $(RPM_TOPDIR)/SPECS
	rpmbuild -ba --target=$(ARCH) $(RPM_TOPDIR)/SPECS/$(SPEC)
	cp $(RPM_TOPDIR)/SRPMS/$(SSRPM) $(SRPM)
	cp $(RPM_TOPDIR)/RPMS/$(ARCH)/$(LRPM) .
	cp $(RPM_TOPDIR)/RPMS/$(ARCH)/$(PRPM) .

# I can leave this target in here, but I have to run it BY HAND right
# before a release build.  Otherwise a src rpm rebuild can fail for
# people without the svn repo in odd ways.
ChangeLog: $(SVNTIME)
	svn2cl dieharder.svn.time

#========================================================================
# Make targets for checking in to svn, syncing svn repo from a local
# (e.g. laptop) to one or more network/primary repos.
#========================================================================
SVNTREE = $(HOME)/Src/svn-tree
SVNPATH = $(SVNTREE)/$(PROJECT)
SVNTIME = $(PROJECT:=.svn.time)

svn:
	echo "New Checkin `date`" >> $(SVNTIME)	# Will force a commit and increment revision
	svn commit .
	cat $(SVNTIME) | \
	sed -e '/^New Checkin/d' >> $(SVNTIME).tmp
	mv $(SVNTIME).tmp $(SVNTIME)

sync:
	echo "New Checkin `date`" >> $(SVNTIME)	# Will force a commit and increment revision
	svn commit .		# Do the commit
	rsync -avz --delete $(SVNPATH) ganesh.phy.duke.edu:/home/einstein/prof/rgb/Src/svn-tree
	rsync -avz --delete $(SVNPATH) 209.42.212.5:$(SVNTREE)
	cat $(SVNTIME) | \
	sed -e '/^New Checkin/d' >> $(SVNTIME).tmp
	mv $(SVNTIME).tmp $(SVNTIME)

installweb : $(TGZ) $(RPM) $(SRPM) $(LRPM) $(PRPM) $(MRPM) $(ABS) $(PHP)
	(ssh $(WLOGIN) mkdir -p public_html/$(WDIR)/$(PROJECT);\
	rsync -avz $(TGZ) $(WLOGIN):public_html/$(WDIR)/$(PROJECT)/; \
	rsync -avz $(SRPM) $(WLOGIN):public_html/$(WDIR)/$(PROJECT)/; \
	rsync -avz $(LRPM) $(WLOGIN):public_html/$(WDIR)/$(PROJECT)/; \
	rsync -avz $(PRPM) $(WLOGIN):public_html/$(WDIR)/$(PROJECT)/; \
	rsync -avz $(MRPM) $(WLOGIN):public_html/$(WDIR)/$(PROJECT)/; \
	rsync -avz $(ABS) $(WLOGIN):public_html/$(WDIR)/$(PROJECT)/; \
	ssh $(WLOGIN) "cd public_html/$(WDIR)/$(PROJECT);ln -sf $(TGZ) $(PROJECT).tgz"; \
	ssh $(WLOGIN) "cd public_html/$(WDIR)/$(PROJECT);ln -sf $(SRPM) $(PROJECT).src.rpm"; \
	ssh $(WLOGIN) "cd public_html/$(WDIR)/$(PROJECT);rm Current*;touch Current_Is_$(VERSION)"; \
	rsync -avz $(PHP) $(WLOGIN):public_html/$(WDIR)/)

REPOSERVER = uriel
REPOPATH = /var/www/html/fc/6/local/
installrepo : $(TGZ) $(RPM) $(SRPM) $(LRPM) $(PRPM) $(MRPM) $(ABS) $(PHP)
	(ssh $(REPOSERVER) mkdir -p $(REPOPATH);\
	rsync -avz $(TGZ) root@$(REPOSERVER):$(REPOPATH);\
	rsync -avz $(SRPM) root@$(REPOSERVER):$(REPOPATH)/SRPM;\
	ssh $(REPOSERVER) mkdir -p $(REPOPATH)/$(ARCH);\
	rsync -avz $(LRPM) root@$(REPOSERVER):$(REPOPATH)/$(ARCH);\
	rsync -avz $(PRPM) root@$(REPOSERVER):$(REPOPATH)/$(ARCH);\
	rsync -avz $(MRPM) root@$(REPOSERVER):$(REPOPATH)/$(ARCH);\
	ssh root@$(REPOSERVER) "cd $(REPOPATH)/$(ARCH);createrepo .")

#========================================================================
# This target takes the dieharder sources and "magically" transforms
# them into RDieHarder sources.
RDHTAR=RDieHarder_@VERSION@.tar
RDHPKG=RDieHarder_@VERSION@.tar.gz

$(RDHPKG): rdhpkg

rdhpkg:
	rm -rf RDieHarder/src/*.h RDieHarder/src/*.c \
	RDieHarder/src/*.o RDieHarder/src/Makefile RDieHarder/src/*.so
	cp dieharder/*.c RDieHarder/src/
	cp dieharder/*.h RDieHarder/src/
	mv RDieHarder/src/rdieharder.c RDieHarder/src/dieharder.c
	cp dieharder/Makefile.RDH RDieHarder/src/Makefile
	tar -cvpf $(RDHTAR) RDieHarder
	gzip -f $(RDHTAR)

# This can be run by anybody
rdhclean:
	rm -rf RDieHarder/src/*.h RDieHarder/src/*.c $(RDHTAR) $(RDHPKG) \
	RDieHarder/src/*.o RDieHarder/src/Makefile RDieHarder/src/*.so

# This can be run as anybody
rdhprep:
	rm -rf RDieHarder/src/*.h RDieHarder/src/*.c \
	RDieHarder/src/*.o RDieHarder/src/Makefile RDieHarder/src/*.so
	cp dieharder/*.c RDieHarder/src/
	cp dieharder/*.h RDieHarder/src/
	mv RDieHarder/src/rdieharder.c RDieHarder/src/dieharder.c
	cp dieharder/Makefile.RDH RDieHarder/src/Makefile

# This has to be run only as root, sorry.
rdh:	$(RDHPKG)
	R CMD INSTALL $(RDHPKG)

