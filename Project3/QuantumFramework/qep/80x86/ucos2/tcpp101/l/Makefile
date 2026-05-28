##############################################################################
# Product:  QEP/C++ port to 80x86, uC/OS-II, Turbo C++ 1.01, Large model
# Last Updated for Version: 3.1.00
# Date of the Last Update:  Sep 22, 2005
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
OS_INCDIR = ..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)\ucos2.60
SRCDIR    = ..\..\..\..\source
LIBDIR    = $(QP_PRTDIR)\$(BINDIR)
BLDDIR    = .
LIBFLAGS  =

#CCINC        := -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(SRCDIR) -I$(BLDDIR)
CCINC        := @include.rsp
QP_DEP       := $(QP_INCDIR)\qassert.h \
	$(QP_INCDIR)\qep.h \
	$(QP_PRTDIR)\qep_port.h \
	$(SRCDIR)\qep_pkg.h

OS_DEP       := $(OS_INCDIR)\includes.h \
	$(OS_INCDIR)\os_cfg.h \
	$(OS_INCDIR)\os_cpu.h \
	$(OS_INCDIR)\ucos_ii.h


# dbg (default target) .......................................................

BINDIR   = dbg

# tcc options
# -c    : compile only
# -v    : source level debugging
# -m    : memory model
# -o... : output file
CCFLAGS  = -c -v -m$(MM) -o$@

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
CCFLAGS  = -c -v -m$(MM) -DQ_SPY -o$@

QP_DEP  := $(QP_DEP) \
	$(QP_INCDIR)\qs.h \
	$(QP_PRTDIR)\qs_port.h

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif

#.............................................................................

$(LIBDIR)\qep.lib : \
	$(BINDIR)\qep.obj \
	$(BINDIR)\qfsm_ini.obj \
	$(BINDIR)\qfsm_dis.obj \
	$(BINDIR)\qhsm_ini.obj \
	$(BINDIR)\qhsm_dis.obj \
	$(BINDIR)\qhsm_top.obj \
	$(BINDIR)\qhsm_tra.obj \
	$(BINDIR)\qhsm_in.obj
	$(RM) $(LIBDIR)\qep.lib
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qep
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qfsm_ini
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qfsm_dis
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qhsm_ini
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qhsm_dis
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qhsm_top
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qhsm_tra
	$(LIB) $(LIBDIR)\qep +$(BINDIR)\qhsm_in
	$(RM) $(LIBDIR)\qep.bak

$(BINDIR)\qep.obj: $(SRCDIR)\qep.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qfsm_ini.obj: $(SRCDIR)\qfsm_ini.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qfsm_dis.obj: $(SRCDIR)\qfsm_dis.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qhsm_ini.obj: $(SRCDIR)\qhsm_ini.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qhsm_dis.obj: $(SRCDIR)\qhsm_dis.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qhsm_top.obj: $(SRCDIR)\qhsm_top.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qhsm_tra.obj: $(SRCDIR)\qhsm_tra.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\qhsm_in.obj: $(SRCDIR)\qhsm_in.cpp $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<


dbg: $(LIBDIR)\qep.lib

rel: $(LIBDIR)\qep.lib

spy: $(LIBDIR)\qep.lib


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

spy_clean: clean

dbg_clean: clean

clean:
	$(RM) $(BINDIR)\*.obj
	$(RM) $(LIBDIR)\qep.bak
	$(RM) $(LIBDIR)\qep.lib

