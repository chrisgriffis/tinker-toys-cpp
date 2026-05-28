##############################################################################
# Product:  QF/C++ port to x86, uC/OS-II, Turbo C++ 1.01, Large memory model
# Last Updated for Version: 3.1.04
# Date of the Last Update:  Dec 11, 2005
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
RM = erase

TCPP101 = c:\tools\tcpp101\bin

# Turbo C++ 1.01 compiler and librarian
CC  = $(TCPP101)\tcc
LIB = $(TCPP101)\tlib

# Memory model -- large
MM  = l

QP_INCDIR = ..\..\..\..\..\include
QP_PRTDIR = ..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)
SRCDIR    = ..\..\..\..\source
LIBDIR    = $(QP_PRTDIR)\$(BINDIR)
BLDDIR    = .
LIBFLAGS  =
OS_SRCDIR = ..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)\ucos2.60
OS_PRTDIR = ..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)\ucos2.60

#CCINC        := -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(SRCDIR) -I$(BLDDIR) \
	-I$(OS_SRCDIR) -I$(OS_PRTDIR)
CCINC        := @include.rsp
QP_DEP       := $(QP_INCDIR)\qassert.h \
	$(QP_INCDIR)\qep.h \
	$(QP_PRTDIR)\qep_port.h \
	$(QP_INCDIR)\qf.h \
	$(QP_PRTDIR)\qf_port.h \
	$(SRCDIR)\qf_pkg.h

OS_DEP        := $(OS_PRTDIR)\INCLUDES.H \
	$(OS_SRCDIR)\UCOS_II.H \
	$(OS_PRTDIR)\OS_CFG.H \
	$(OS_PRTDIR)\OS_CPU.H

# dbg (default target) .......................................................

BINDIR   = dbg

# tcc options
# -c    : compile only
# -v    : source level debugging
# -m    : memory model
# -o... : output file
CCFLAGS  = -c  -v -m$(MM) -o$@

# release ....................................................................
ifeq ($(MAKECMDGOALS), rel)

BINDIR   = rel

# tcc options
# -c    : compile only
# -m    : memory model
# -o... : output file
CCFLAGS  = -c -m$(MM) -o$@

endif
ifeq ($(MAKECMDGOALS), rel_clean)

BINDIR   = rel

endif

# spy ........................................................................
ifeq ($(MAKECMDGOALS), spy)

BINDIR   = spy

# tcc options
# -c    : compile only
# -v    : source level debugging
# -m    : memory model
# -D... : define macro
# -o... : output file
CCFLAGS  = -c  -v -m$(MM)  -DQ_SPY -o$@

QP_DEP  := $(QP_DEP) \
	$(QP_INCDIR)\qs.h \
	$(QP_PRTDIR)\qs_port.h

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif

#.............................................................................

$(LIBDIR)\qf.lib: \
	$(BINDIR)\qa_fifo.obj \
	$(BINDIR)\qa_lifo.obj \
	$(BINDIR)\qa_run.obj \
	$(BINDIR)\qa_sub.obj \
	$(BINDIR)\qa_usub.obj \
	$(BINDIR)\qa_usuba.obj \
	$(BINDIR)\qf_act.obj \
	$(BINDIR)\qf_gc.obj \
	$(BINDIR)\qf_log2.obj \
	$(BINDIR)\qf_new.obj \
	$(BINDIR)\qf_pool.obj \
	$(BINDIR)\qf_psini.obj \
	$(BINDIR)\qf_pspub.obj \
	$(BINDIR)\qf_pwr2.obj \
	$(BINDIR)\qf_tick.obj \
	$(BINDIR)\qf_time.obj \
	$(BINDIR)\qte_ctor.obj \
	$(BINDIR)\qte_arm.obj \
	$(BINDIR)\qte_darm.obj \
	$(BINDIR)\qte_rarm.obj \
	$(BINDIR)\qf_port.obj
	$(RM) $(LIBDIR)\qf.lib
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qa_fifo
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qa_lifo
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qa_run
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qa_sub
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qa_usub
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qa_usuba
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_act
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_gc
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_log2
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_new
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_pool
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_psini
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_pspub
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_pwr2
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_tick
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_time
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qte_ctor
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qte_arm
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qte_darm
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qte_rarm
	$(LIB) $(LIBDIR)\qf +$(BINDIR)\qf_port
	$(RM) $(LIBDIR)\qf.bak

$(BINDIR)\qa_fifo.obj: $(SRCDIR)\qa_fifo.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qa_lifo.obj: $(SRCDIR)\qa_lifo.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qa_run.obj: $(SRCDIR)\qa_run.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qa_sub.obj: $(SRCDIR)\qa_sub.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qa_usub.obj: $(SRCDIR)\qa_usub.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qa_usuba.obj: $(SRCDIR)\qa_usuba.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_act.obj: $(SRCDIR)\qf_act.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_gc.obj: $(SRCDIR)\qf_gc.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_log2.obj: $(SRCDIR)\qf_log2.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_new.obj: $(SRCDIR)\qf_new.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_pool.obj: $(SRCDIR)\qf_pool.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_psini.obj: $(SRCDIR)\qf_psini.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_pspub.obj: $(SRCDIR)\qf_pspub.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_pwr2.obj: $(SRCDIR)\qf_pwr2.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_tick.obj: $(SRCDIR)\qf_tick.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_time.obj: $(SRCDIR)\qf_time.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qte_ctor.obj: $(SRCDIR)\qte_ctor.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qte_arm.obj: $(SRCDIR)\qte_arm.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qte_darm.obj: $(SRCDIR)\qte_darm.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qte_rarm.obj: $(SRCDIR)\qte_rarm.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qf_port.obj: $(BLDDIR)\qf_port.cpp $(QP_DEP) $(OS_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<


rel: $(LIBDIR)\qf.lib

spy: $(LIBDIR)\qf.lib

dbg: $(LIBDIR)\qf.lib


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

spy_clean: clean

dbg_clean: clean

clean:
	$(RM) $(BINDIR)\*.obj
	$(RM) $(LIBDIR)\qf.bak
	$(RM) $(LIBDIR)\qf.lib

