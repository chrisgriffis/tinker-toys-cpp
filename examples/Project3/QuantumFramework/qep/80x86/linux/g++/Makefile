##############################################################################
# Product:  QEP/C++ port to Linux
# Last Updated for Version: 3.1.01
# Date of the Last Update:  Oct 21, 2005
#
# Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
#
# This software may be distributed and modified under the terms of the GNU
# General Public License version 2 (GPL) as published by the Free Software
# Foundation and appearing in the file GPL.TXT included in the packaging of
# this file. Please note that GPL Section 2[b] requires that all works based
# on this software must also be made publicly available under the terms of
# the GPL ("Copyleft").
#
# Alternatively, this software may be distributed and modified under the
# terms of Quantum Leaps commercial licenses, which expressly allow the
# licensees to retain the proprietary status of their code. The licensees
# who use this software under one of Quantum Leaps commercial licenses do
# not use this software under the GPL and therefore are not subject to any
# of its terms.
#
# Contact information:
# Quantum Leaps Web site:  http://www.quantum-leaps.com
# Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
# Quantum Leaps products:  http://www.quantum-leaps.com/products
# e-mail:                  sales@quantum-leaps.com
##############################################################################
# general utilities
RM = rm

# The g++ compiler and ar librarian
CC  = g++
LIB = ar

QP_INCDIR = ../../../../include
QP_PRTDIR = ../../../../ports/80x86/linux/g++
SRCDIR    = ../../../source
LIBDIR    = $(QP_PRTDIR)/$(BINDIR)
BLDDIR    = .
LIBFLAGS = rs

CCINC    := -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(SRCDIR) -I$(BLDDIR)
QP_DEP   := $(QP_INCDIR)/qassert.h \
	$(QP_INCDIR)/qep.h \
	$(QP_PRTDIR)/qep_port.h \
	$(SRCDIR)/qep_pkg.h

# dbg (default target) .......................................................

BINDIR   = dbg

# g++ options
# -c    : compile only
# -g    : source level debugging
CCFLAGS  = -g -c -Wall -o$@

# release ....................................................................
ifeq ($(MAKECMDGOALS), rel)

BINDIR   = rel

# g++ options
# -c    : compile only
# -O    : optimize
CCFLAGS  = -O -c -Wall -o$@

endif
ifeq ($(MAKECMDGOALS), rel_clean)

BINDIR   = rel

endif

# spy ........................................................................
ifeq ($(MAKECMDGOALS), spy)

BINDIR   = spy

# g++ options
# -c    : compile only
# -g    : source level debugging
# -D    : define preprocessor macro
CCFLAGS  = -g -c -Wall -DQ_SPY -o$@

QP_DEP  := $(QP_DEP) \
	$(QP_INCDIR)/qs.h \
	$(QP_PRTDIR)/qs_port.h

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif

#.............................................................................

$(LIBDIR)/libqep.a: \
	$(BINDIR)/qep.o \
	$(BINDIR)/qfsm_ini.o \
	$(BINDIR)/qfsm_dis.o \
	$(BINDIR)/qhsm_ini.o \
	$(BINDIR)/qhsm_dis.o \
	$(BINDIR)/qhsm_top.o \
	$(BINDIR)/qhsm_tra.o \
	$(BINDIR)/qhsm_in.o
	-rm -f $(LIBDIR)/libqep.a
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqep.a \
	$(BINDIR)/qep.o \
	$(BINDIR)/qfsm_ini.o \
	$(BINDIR)/qfsm_dis.o \
	$(BINDIR)/qhsm_ini.o \
	$(BINDIR)/qhsm_dis.o \
	$(BINDIR)/qhsm_top.o \
	$(BINDIR)/qhsm_tra.o \
	$(BINDIR)/qhsm_in.o

$(BINDIR)/qep.o: $(SRCDIR)/qep.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qfsm_ini.o: $(SRCDIR)/qfsm_ini.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qfsm_dis.o: $(SRCDIR)/qfsm_dis.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qhsm_ini.o: $(SRCDIR)/qhsm_ini.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qhsm_dis.o: $(SRCDIR)/qhsm_dis.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qhsm_top.o: $(SRCDIR)/qhsm_top.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qhsm_tra.o: $(SRCDIR)/qhsm_tra.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qhsm_in.o: $(SRCDIR)/qhsm_in.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

dbg: $(LIBDIR)/libqep.a

rel: $(LIBDIR)/libqep.a

spy: $(LIBDIR)/libqep.a


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

spy_clean: clean

dbg_clean: clean

clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(BINDIR)/*.d
	-$(RM) $(LIBDIR)/libqep.a
