# $Id: Makefile.am 2535 2011-03-25 15:58:06Z ccutlip $
INCLUDES = -I$(srcdir)/../../src -I$(srcdir)/../../lib/rxio
LDADD = ../../src/libgpstk.la ../../lib/rxio/librxio.la

bin_PROGRAMS = svvis wheresat findMoreThan12 compStaVis compSatVis DOPcalc CalcDOPs

svvis_SOURCES = svvis.cpp
wheresat_SOURCES = WhereSat.cpp
findMoreThan12_SOURCES = findMoreThan12.cpp
compStaVis_SOURCES = compStaVis.cpp VisSupport.cpp StaStats.cpp DiscreteVisibleCounts.cpp
compSatVis_SOURCES = compSatVis.cpp VisSupport.cpp StaStats.cpp DiscreteVisibleCounts.cpp
DOPcalc_SOURCES = DOPcalc.cpp
CalcDOPs_SOURCES = CalcDOPs.cpp

