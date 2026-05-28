##############################################################################
# Product: QHsmTst/C++ example, Linux/P-threads, g++
# Last Updated for Version: 3.1.01
# Date of the Last Update:  Oct 26, 2005
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

# The g++ compiler and linker
CC   = g++
LINK = g++

QP_INCDIR = ../../../../../include
QP_PRTDIR = ../../../../../ports/80x86/linux/g++
QP_LIBDIR = $(QP_PRTDIR)/$(BINDIR)
BLDDIR    = .
LINKFLAGS =

CCINC    := -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(BLDDIR) -Iom/CppUnitLite
QP_DEP   := $(QP_INCDIR)/qassert.h \
	$(QP_INCDIR)/qep.h \
	$(QP_PRTDIR)/qep_port.h

APP_DEP  := $(BLDDIR)/qhsmtst.h

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

#.............................................................................

$(BINDIR)/qhsmtst : \
	$(BINDIR)/main.o \
	$(BINDIR)/qhsmtst.o \
	$(QP_LIBDIR)/libqep.a
	$(LINK) $(LINKFLAGS) \
	-o $(BINDIR)/qhsmtst \
	$(BINDIR)/main.o \
	$(BINDIR)/qhsmtst.o \
	$(QP_LIBDIR)/libqep.a \
	om/CppUnitLite/cppunitlite.a \
	-lpthread

$(BINDIR)/main.o: $(BLDDIR)/main.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qhsmtst.o: $(BLDDIR)/qhsmtst.cpp $(APP_DEP) $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<


dbg: $(BINDIR)/qhsmtst

rel: $(BINDIR)/qhsmtst


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

dbg_clean: clean

clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(BINDIR)/*.d
	-$(RM) $(BINDIR)/qhsmtst
