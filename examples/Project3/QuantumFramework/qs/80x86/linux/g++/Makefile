##############################################################################
# Product:  QS/C++ port to Linux
# Last Updated for Version: 3.1.06
# Date of the Last Update:  May 06, 2006
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (c) 2002-2006 Quantum Leaps, LLC. All rights reserved.
#
# Internet: www.quantum-leaps.com     Licensing: sales@quantum-leaps.com
#
# This Software is protected by the United States copyright laws and
# international treaties. Distribution of products containing this Software
# or based upon this Software (Derivative Works) requires a valid Quantum
# Leaps Distribution License. Any other distribution, in source or binary
# format is illegal.
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
	$(QP_INCDIR)/qs.h \
	$(QP_PRTDIR)/qs_port.h \
	$(SRCDIR)/qs_pkg.h

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

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif

#.............................................................................

$(LIBDIR)/libqs.a: \
	$(BINDIR)/qs.o \
	$(BINDIR)/qs_.o \
	$(BINDIR)/qs_blk.o \
	$(BINDIR)/qs_byte.o \
	$(BINDIR)/qs_f32.o \
	$(BINDIR)/qs_f64.o \
	$(BINDIR)/qs_mem.o \
	$(BINDIR)/qs_str.o \
	$(BINDIR)/qs_port.o
	-rm -f $(LIBDIR)/libqs.a
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqs.a \
	$(BINDIR)/qs.o \
	$(BINDIR)/qs_.o \
	$(BINDIR)/qs_blk.o \
	$(BINDIR)/qs_byte.o \
	$(BINDIR)/qs_f32.o \
	$(BINDIR)/qs_f64.o \
	$(BINDIR)/qs_mem.o \
	$(BINDIR)/qs_str.o \
	$(BINDIR)/qs_port.o

$(BINDIR)/qs.o: $(SRCDIR)/qs.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_.o: $(SRCDIR)/qs_.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_blk.o: $(SRCDIR)/qs_blk.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_byte.o: $(SRCDIR)/qs_byte.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_f32.o: $(SRCDIR)/qs_f32.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_f64.o: $(SRCDIR)/qs_f64.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_mem.o: $(SRCDIR)/qs_mem.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_str.o: $(SRCDIR)/qs_str.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qs_port.o: $(BLDDIR)/qs_port.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

dbg: $(LIBDIR)/libqs.a

rel: $(LIBDIR)/libqs.a

spy: $(LIBDIR)/libqs.a


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

spy_clean: clean

dbg_clean: clean

clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(BINDIR)/*.d
	-$(RM) $(LIBDIR)/libqs.a
