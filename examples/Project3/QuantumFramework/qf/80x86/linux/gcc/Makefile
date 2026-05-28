##############################################################################
# Product:  QF/C port to Linux/P-threads, g++
# Last Updated for Version: 3.1.02
# Date of the Last Update:  Oct 24, 2005
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

# The gcc compiler and ar librarian
CC  = gcc
LIB = ar

QP_INCDIR = ../../../../include
QP_PRTDIR = ../../../../ports/80x86/linux/gcc
SRCDIR    = ../../../source
LIBDIR    = $(QP_PRTDIR)/$(BINDIR)
BLDDIR    = .
LIBFLAGS = rs

CCINC    := -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(SRCDIR) -I$(BLDDIR)
QP_DEP   := $(QP_INCDIR)/qassert.h \
	$(QP_INCDIR)/qep.h \
	$(QP_PRTDIR)/qep_port.h \
	$(QP_INCDIR)/qf.h \
	$(QP_INCDIR)/qequeue.h \
	$(QP_INCDIR)/qmpool.h \
	$(QP_INCDIR)/qpset.h \
	$(QP_PRTDIR)/qf_port.h \
	$(SRCDIR)/qf_pkg.h

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

$(LIBDIR)/libqf.a: \
	$(BINDIR)/qa_fifo.o \
	$(BINDIR)/qa_lifo.o \
	$(BINDIR)/qa_fifo_.o \
	$(BINDIR)/qa_lifo_.o \
	$(BINDIR)/qa_get_.o \
	$(BINDIR)/qa_ctor.o \
	$(BINDIR)/qa_sub.o \
	$(BINDIR)/qa_usub.o \
	$(BINDIR)/qa_usuba.o \
	$(BINDIR)/qeq_init.o \
	$(BINDIR)/qeq_fifo.o \
	$(BINDIR)/qeq_lifo.o \
	$(BINDIR)/qeq_get.o \
	$(BINDIR)/qf_act.o \
	$(BINDIR)/qf_gc.o \
	$(BINDIR)/qf_log2.o \
	$(BINDIR)/qf_new.o \
	$(BINDIR)/qf_pool.o \
	$(BINDIR)/qf_psini.o \
	$(BINDIR)/qf_pspub.o \
	$(BINDIR)/qf_pwr2.o \
	$(BINDIR)/qf_tick.o \
	$(BINDIR)/qf_time.o \
	$(BINDIR)/qmp_get.o \
	$(BINDIR)/qmp_init.o \
	$(BINDIR)/qmp_put.o \
	$(BINDIR)/qte_ctor.o \
	$(BINDIR)/qte_arm.o \
	$(BINDIR)/qte_darm.o \
	$(BINDIR)/qte_rarm.o \
	$(BINDIR)/qf_port.o
	-$(RM) -f $(LIBDIR)/libqf.a
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqf.a \
	$(BINDIR)/qa_fifo.o \
	$(BINDIR)/qa_lifo.o \
	$(BINDIR)/qa_fifo_.o \
	$(BINDIR)/qa_lifo_.o \
	$(BINDIR)/qa_get_.o \
	$(BINDIR)/qa_ctor.o \
	$(BINDIR)/qa_sub.o \
	$(BINDIR)/qa_usub.o \
	$(BINDIR)/qa_usuba.o \
	$(BINDIR)/qeq_init.o \
	$(BINDIR)/qeq_fifo.o \
	$(BINDIR)/qeq_lifo.o \
	$(BINDIR)/qeq_get.o \
	$(BINDIR)/qf_act.o \
	$(BINDIR)/qf_gc.o \
	$(BINDIR)/qf_log2.o \
	$(BINDIR)/qf_new.o \
	$(BINDIR)/qf_pool.o \
	$(BINDIR)/qf_psini.o \
	$(BINDIR)/qf_pspub.o \
	$(BINDIR)/qf_pwr2.o \
	$(BINDIR)/qf_tick.o \
	$(BINDIR)/qf_time.o \
	$(BINDIR)/qmp_get.o \
	$(BINDIR)/qmp_init.o \
	$(BINDIR)/qmp_put.o \
	$(BINDIR)/qte_ctor.o \
	$(BINDIR)/qte_arm.o \
	$(BINDIR)/qte_darm.o \
	$(BINDIR)/qte_rarm.o \
	$(BINDIR)/qf_port.o

$(BINDIR)/qa_fifo.o: $(SRCDIR)/qa_fifo.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_lifo.o: $(SRCDIR)/qa_lifo.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_fifo_.o: $(SRCDIR)/qa_fifo_.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_lifo_.o: $(SRCDIR)/qa_lifo_.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_get_.o: $(SRCDIR)/qa_get_.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_ctor.o: $(SRCDIR)/qa_ctor.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_sub.o: $(SRCDIR)/qa_sub.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_usub.o: $(SRCDIR)/qa_usub.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qa_usuba.o: $(SRCDIR)/qa_usuba.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qeq_init.o: $(SRCDIR)/qeq_init.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qeq_fifo.o: $(SRCDIR)/qeq_fifo.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qeq_lifo.o: $(SRCDIR)/qeq_lifo.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qeq_get.o: $(SRCDIR)/qeq_get.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_act.o: $(SRCDIR)/qf_act.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_gc.o: $(SRCDIR)/qf_gc.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_log2.o: $(SRCDIR)/qf_log2.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_new.o: $(SRCDIR)/qf_new.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_pool.o: $(SRCDIR)/qf_pool.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_psini.o: $(SRCDIR)/qf_psini.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_pspub.o: $(SRCDIR)/qf_pspub.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_pwr2.o: $(SRCDIR)/qf_pwr2.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_tick.o: $(SRCDIR)/qf_tick.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_time.o: $(SRCDIR)/qf_time.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qmp_get.o: $(SRCDIR)/qmp_get.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qmp_init.o: $(SRCDIR)/qmp_init.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qmp_put.o: $(SRCDIR)/qmp_put.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qte_ctor.o: $(SRCDIR)/qte_ctor.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qte_arm.o: $(SRCDIR)/qte_arm.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qte_darm.o: $(SRCDIR)/qte_darm.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qte_rarm.o: $(SRCDIR)/qte_rarm.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qf_port.o: $(BLDDIR)/qf_port.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<


dbg: $(LIBDIR)/libqf.a

rel: $(LIBDIR)/libqf.a

spy: $(LIBDIR)/libqf.a


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

spy_clean: clean

dbg_clean: clean

clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(BINDIR)/*.d
	-$(RM) $(LIBDIR)/libqf.a
