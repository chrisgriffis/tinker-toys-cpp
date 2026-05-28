##############################################################################
# Product:  QDPP/C++ example, uC/OS-II, Turbo C++ 1.01, Large model
# Last Updated for Version: 3.1.01
# Date of the Last Update:  Oct 20, 2005
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
LINK = $(TCPP101)\tlink

# Memory model -- large
MM  = l

QP_INCDIR = ..\..\..\..\..\..\include
QP_PRTDIR = ..\..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)
QP_LIBDIR = $(QP_PRTDIR)\$(BINDIR)
BLDDIR    = .
OS_SRCDIR = ..\..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)\ucos2.60
OS_PRTDIR = ..\..\..\..\..\..\ports\80x86\ucos2\tcpp101\$(MM)\ucos2.60

#CCINC     := -I$(QP_INCDIR) -I$(QP_INCDIR_PRT) -I$(BLDDIR)
CCINC     := @include.rsp
QP_DEP    := $(QP_INCDIR)\qassert.h \
	$(QP_INCDIR)\qep.h \
	$(QP_PRTDIR)\qep_port.h \
	$(QP_INCDIR)\qf.h \
	$(QP_PRTDIR)\qf_port.h \
	$(OS_SRCDIR)\UCOS_II.H \
	$(OS_PRTDIR)\INCLUDES.H \
	$(OS_PRTDIR)\OS_CFG.H \
	$(OS_PRTDIR)\OS_CPU.H

APP_DEP   := $(BLDDIR)\qdpp.h \
	$(BLDDIR)\video.h

# dbg (default target) .......................................................

BINDIR   = dbg

# tcc options
# -c    : compile only
# -v    : source level debugging
# -m    : memory model
# -o... : output file
CCFLAGS  = -c  -v -m$(MM) -o$@

# tlink options
# /m  : map file with publics
# /v  : include full symbolic information
LINKFLAGS =	/m /v @link_dbg.rsp


# release ....................................................................
ifeq ($(MAKECMDGOALS), rel)

BINDIR   = rel

# tcc options
# -c    : compile only
# -m    : memory model
# -o... : output file
CCFLAGS  = -c -m$(MM) -o$@

# tlink options
# /m  : map file with publics
LINKFLAGS =	/m	@link_rel.rsp

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

# tlink options
# /m  : map file with publics
# /v  : include full symbolic information
LINKFLAGS =	/m /v @link_spy.rsp

QP_DEP  := $(QP_DEP) \
	$(QP_INCDIR)\qs.h \
	$(QP_PRTDIR)\qs_port.h

QS_LIB  := $(QP_LIBDIR)\qs.lib

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif
#.............................................................................

$(BINDIR)\qdpp.exe: \
	$(BINDIR)\bsp.obj \
	$(BINDIR)\main.obj \
	$(BINDIR)\philo.obj \
	$(BINDIR)\table.obj \
	$(BINDIR)\kbdmgr.obj \
	$(BINDIR)\video.obj \
	$(QP_LIBDIR)\qep.lib \
	$(QP_LIBDIR)\qf.lib
	$(LINK) $(LINKFLAGS)

$(BINDIR)\bsp.obj: $(BLDDIR)\bsp.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\main.obj: $(BLDDIR)\main.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\philo.obj: $(BLDDIR)\philo.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\table.obj: $(BLDDIR)\table.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\kbdmgr.obj: $(BLDDIR)\kbdmgr.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\video.obj: $(BLDDIR)\video.cpp $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<


dbg: $(BINDIR)\qdpp.exe

rel: $(BINDIR)\qdpp.exe

spy: $(BINDIR)\qdpp.exe


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

spy_clean: clean

dbg_clean: clean

clean:
	$(RM) $(BINDIR)\*.obj
	$(RM) $(BINDIR)\*.map
	$(RM) $(BINDIR)\*.exe

