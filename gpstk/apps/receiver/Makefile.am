# $Id: Makefile.am 919 2007-11-21 05:12:37Z rickmach $
INCLUDES = -I$(srcdir)/../../src -I$(srcdir)/../../lib/rxio -I../rfw
LDADD = ../../src/libgpstk.la ../../lib/rxio/librxio.la ../rfw/FDStreamBuff.o ../rfw/TCPStreamBuff.o

bin_PROGRAMS = rtAshtech

rtAshtech_SOURCES = AshtechMessage.cpp rtAshtech.cpp ScreenControl.cpp

