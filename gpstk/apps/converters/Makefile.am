# $Id: Makefile.am 3140 2012-06-18 15:03:02Z susancummins $
INCLUDES = -I$(srcdir)/../../src -I$(srcdir)/../../lib/rxio
LDADD = ../../src/libgpstk.la ../../lib/rxio/librxio.la

bin_PROGRAMS = RinexConvert novaRinex ash2mdp ats2mdp ash2xyz

RinexConvert_SOURCES = RinexConvert.cpp

novaRinex_SOURCES = novaRinex.cpp 

ash2mdp_SOURCES = ash2mdp.cpp

ats2mdp_SOURCES = ats2mdp.cpp

ash2xyz_SOURCES = ash2xyz.cpp
