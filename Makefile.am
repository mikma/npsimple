AUTOMAKE_OPTIONS = foreign 1.10
ACLOCAL_AMFLAGS = -I m4

MAINTAINERCLEANFILES = \
	config.log config.status \
	$(srcdir)/Makefile.in \
	$(srcdir)/config.h.in $(srcdir)/config.h.in~ $(srcdir)/configure \
	$(srcdir)/install-sh $(srcdir)/ltmain.sh $(srcdir)/missing \
	$(srcdir)/m4/libtool.m4 $(srcdir)/m4/lt~obsolete.m4 \
	$(srcdir)/m4/ltoptions.m4 $(srcdir)/m4/ltsugar.m4 \
	$(srcdir)/m4/ltversion.m4 \
	$(srcdir)/depcomp $(srcdir)/aclocal.m4 \
	$(srcdir)/config.guess $(srcdir)/config.sub

CLEANFILES = English.rsrc English.lproj

EXTRA_DIST = .gitignore

AM_CFLAGS = $(NPAPI_SDK_CFLAGS)

if DARWIN
plugindir=$(pluginsdir)/$(PACKAGE_NAME).plugin
contentsdir=$(plugindir)/Contents
resourcesdir=$(contentsdir)/Resources
pluginexedir=$(contentsdir)/MacOS/$(PACKAGE_NAME)
else
pluginexedir=$(pluginsdir)
endif

pluginexe_LTLIBRARIES = npsimple@so_suffix@.la

npsimple@so_suffix@_la_SOURCES = \
	common.h \
	npsimple.c \
	npsimple.exports
npsimple@so_suffix@_la_LDFLAGS = $(AM_LDFLAGS) \
	-export-symbols "$(srcdir)/npsimple.exports" \
	-module -shared -avoid-version -no-undefined

if DARWIN
English.lproj:	$(builddir)/English.rsrc
	cp $(builddir)/English.rsrc English.lproj

dist_noinst_DATA = English.r
resources_DATA = English.lproj
dist_contents_DATA = Info.plist
endif

if WIN32
npsimple@so_suffix@_la_SOURCES += \
	npsimple-resources.rc
npsimple@so_suffix@_la_LDFLAGS += \
	-Wl,--enable-stdcall-fixup
endif

#############

RCCOMPILE = $(RC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS)
LTRCCOMPILE = $(LIBTOOL) --mode=compile --tag=RC $(RCCOMPILE)

.rc.lo:
	$(LTRCCOMPILE) -i "$<" -o "$@"

.rc.o:
	$(RCCOMPILE) -i "$<" -o "$@"

SUFFIXES = .r .rsrc

RESCOMPILE = $(RES) $(AM_CPPFLAGS) $(CPPFLAGS)

.r.rsrc:
	$(RESCOMPILE) -o "$@" -useDF "$<"
