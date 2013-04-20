# $Id: Makefile.am 2535 2011-03-25 15:58:06Z ccutlip $
INCLUDES = -I$(srcdir)/../../src -I$(srcdir)/../../lib/geomatics -I$(srcdir)/../../lib/vdraw -I$(srcdir)/../../lib/vplot

LDADD = ../../src/libgpstk.la ../../lib/vdraw/libvdraw.la ../../lib/vplot/libvplot.la ../../lib/geomatics/libgeomatics.la

bin_PROGRAMS = mpsolve

mpsolve_SOURCES = mpsolve.cpp ObsArray.cpp
