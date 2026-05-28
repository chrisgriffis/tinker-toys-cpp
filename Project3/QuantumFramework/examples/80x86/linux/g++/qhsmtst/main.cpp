//////////////////////////////////////////////////////////////////////////////
// Product: QHsmTst Example for DOS with Borland Turbo C++ 1.01
// Version: Compatible with QEP/C++ 3.x.yy
// Updated: Oct 24, 2005
//
// Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
//
// This example is part of the Quantum Leaps QP/C++ software, and may be
// distributed and modified under the terms of the GNU General Public License
// version 2 (GPL) as published by the Free Software Foundation and appearing
// in the file GPL.TXT included in the packaging of this file. Please note
// that GPL Section 2[b] requires that all works based on this software must
// also be made publicly available under the terms of the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified in conjunction
// with a valid QP/C++ Quantum Leaps commercial license. Quantum Leaps
// commercial licenses are designed for users who want to retain proprietary
// status of their code. The users who license this software under one of
// Quantum Leaps commercial licenses do not use this software under the GPL
// and therefore are not subject to any of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qep_port.h"
#include "qhsmtst.h"
#include "qassert.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cpp unit lite stuff
#include "Testharness.h"

Q_DEFINE_THIS_FILE

// local objects -------------------------------------------------------------
static QHsmTst l_test;
static char transitionList[100][32];
static int transitionCount = 0;

//............................................................................
void Q_assert_handler(char const *file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
//............................................................................
void dumpTransitions()
{
	for (int i = 0; i < transitionCount; ++i)
	{
		printf("%s, ", transitionList[i]);
	}
	printf("\n");
}

void logTransition(const char *msg)
{
	strcpy(transitionList[transitionCount], msg);
	transitionCount += 1;
}

void QHsmTst::display(char const *msg) {
	logTransition(msg);
}
//............................................................................
void QHsmTst::exit(void) {
    printf("Bye, Bye!");
    ::exit(0);
}
//............................................................................
static void dispatch(QSignal sig) {
    QEvent e;
    Q_REQUIRE((A_SIG <= sig) && (sig <= I_SIG));
    e.sig = sig;
    char buffer[128];
    sprintf(buffer, "%c:", 'A' + sig - A_SIG);
    transitionCount = 0;
    logTransition(buffer);
    l_test.dispatch(&e);                                 // dispatch the event
}
//............................................................................
int main()
{
	TestResult tr;
	TestRegistry::runAllTests(tr);
	return 0;
}

#define CHECK_RESULTS(results) \
{\
	LONGS_EQUAL(sizeof results / sizeof results[0], transitionCount);\
	for (unsigned int i = 0; i < sizeof results / sizeof results[0]; ++i)\
	{\
		CHECK(strcmp(results[i], transitionList[i]) == 0);\
	}\
}

TEST(init, first)
{
	const char * const results00[] =
	{
	"top-INIT;", "d-ENTRY;", "d2-ENTRY;", "d2-INIT;", "d21-ENTRY;", "d211-ENTRY;"
	};
	l_test.init((QEvent const *)0);          // trigger initial transition
	CHECK_RESULTS(results00)

	const char * const results01[] =
	{
	"A:", "d21-A;", "d211-EXIT;", "d21-EXIT;", "d21-ENTRY;", "d21-INIT;", "d211-ENTRY;"
	};
	dispatch(A_SIG);
	CHECK_RESULTS(results01)

	const char * const results02[] =
	{
	"B:", "d21-B;", "d211-EXIT;", "d211-ENTRY;"
	};
	dispatch(B_SIG);
	CHECK_RESULTS(results02)

	const char * const results03[] =
	{
	"D:", "d211-D;", "d211-EXIT;", "d21-INIT;", "d211-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results03)

	const char * const results04[] =
	{
	"E:", "d-E;", "d211-EXIT;", "d21-EXIT;", "d2-EXIT;", "d1-ENTRY;", "d11-ENTRY;"
	};
	dispatch(E_SIG);
	CHECK_RESULTS(results04)

	const char * const results05[] =
	{
	"I:", "d1-I;"
	};
	dispatch(I_SIG);
	CHECK_RESULTS(results05)

	const char * const results06[] =
	{
	"F:", "d1-F;", "d11-EXIT;", "d1-EXIT;", "d2-ENTRY;", "d21-ENTRY;", "d211-ENTRY;"
	};
	dispatch(F_SIG);
	CHECK_RESULTS(results06)

	const char * const results07[] =
	{
	"I:", "d2-I;"
	};
	dispatch(I_SIG);
	CHECK_RESULTS(results07)

	const char * const results08[] =
	{
	"I:", "d-I;"
	};
	dispatch(I_SIG);
	CHECK_RESULTS(results08)

	const char * const results09[] =
	{
	"F:", "d2-F;", "d211-EXIT;", "d21-EXIT;", "d2-EXIT;", "d1-ENTRY;", "d11-ENTRY;"
	};
	dispatch(F_SIG);
	CHECK_RESULTS(results09)

	const char * const results10[] =
	{
	"A:", "d1-A;", "d11-EXIT;", "d1-EXIT;", "d1-ENTRY;", "d1-INIT;", "d11-ENTRY;"
	};
	dispatch(A_SIG);
	CHECK_RESULTS(results10)

	const char * const results11[] =
	{
	"B:", "d1-B;", "d11-EXIT;", "d11-ENTRY;"
	};
	dispatch(B_SIG);
	CHECK_RESULTS(results11)

	const char * const results12[] =
	{
	"D:", "d1-D;", "d11-EXIT;", "d1-EXIT;", "d-INIT;", "d1-ENTRY;", "d11-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results12)

	const char * const results13[] =
	{
	"D:", "d11-D;", "d11-EXIT;", "d1-INIT;", "d11-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results13)

	const char * const results14[] =
	{
	"E:", "d-E;", "d11-EXIT;", "d1-EXIT;", "d1-ENTRY;", "d11-ENTRY;"
	};
	dispatch(E_SIG);
	CHECK_RESULTS(results14)

	const char * const results15[] =
	{
	"G:", "d11-G;", "d11-EXIT;", "d1-EXIT;", "d2-ENTRY;", "d21-ENTRY;", "d211-ENTRY;"
	};
	dispatch(G_SIG);
	CHECK_RESULTS(results15)

	const char * const results16[] =
	{
	"H:", "d211-H;", "d211-EXIT;", "d21-EXIT;", "d2-EXIT;", "d-INIT;", "d1-ENTRY;", "d11-ENTRY;"
	};
	dispatch(H_SIG);
	CHECK_RESULTS(results16)

	const char * const results17[] =
	{
	"H:", "d11-H;", "d11-EXIT;", "d1-EXIT;", "d-INIT;", "d1-ENTRY;", "d11-ENTRY;"
	};
	dispatch(H_SIG);
	CHECK_RESULTS(results17)

	const char * const results18[] =
	{
	"C:", "d1-C;", "d11-EXIT;", "d1-EXIT;", "d2-ENTRY;", "d2-INIT;", "d21-ENTRY;", "d211-ENTRY;"
	};
	dispatch(C_SIG);
	CHECK_RESULTS(results18)

	const char * const results19[] =
	{
	"G:", "d21-G;", "d211-EXIT;", "d21-EXIT;", "d2-EXIT;", "d1-ENTRY;", "d1-INIT;", "d11-ENTRY;"
	};
	dispatch(G_SIG);
	CHECK_RESULTS(results19)

	const char * const results20[] =
	{
	"C:", "d1-C;", "d11-EXIT;", "d1-EXIT;", "d2-ENTRY;", "d2-INIT;", "d21-ENTRY;", "d211-ENTRY;"
	};
	dispatch(C_SIG);
	CHECK_RESULTS(results20)

	const char * const results21[] =
	{
	"C:", "d-C;", "d211-EXIT;", "d21-EXIT;", "d2-EXIT;", "d-EXIT;", "s-ENTRY;", "s-INIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(C_SIG);
	CHECK_RESULTS(results21)

	const char * const results22[] =
	{
	"C:", "s1-C;", "s11-EXIT;", "s1-EXIT;", "s2-ENTRY;", "s2-INIT;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(C_SIG);
	CHECK_RESULTS(results22)

	const char * const results23[] =
	{
	"A:", "s21-A;", "s211-EXIT;", "s21-EXIT;", "s21-ENTRY;", "s21-INIT;", "s211-ENTRY;"
	};
	dispatch(A_SIG);
	CHECK_RESULTS(results23)

	const char * const results24[] =
	{
	"A:", "s21-A;", "s211-EXIT;", "s21-EXIT;", "s21-ENTRY;", "s21-INIT;", "s211-ENTRY;"
	};
	dispatch(A_SIG);
	CHECK_RESULTS(results24)

	const char * const results25[] =
	{
	"B:", "s21-B;", "s211-EXIT;", "s211-ENTRY;"
	};
	dispatch(B_SIG);
	CHECK_RESULTS(results25)

	const char * const results26[] =
	{
	"B:", "s21-B;", "s211-EXIT;", "s211-ENTRY;"
	};
	dispatch(B_SIG);
	CHECK_RESULTS(results26)

	const char * const results27[] =
	{
	"D:", "s211-D;", "s211-EXIT;", "s21-INIT;", "s211-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results27)

	const char * const results28[] =
	{
	"D:", "s211-D;", "s211-EXIT;", "s21-INIT;", "s211-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results28)

	const char * const results29[] =
	{
	"E:", "s-E;", "s211-EXIT;", "s21-EXIT;", "s2-EXIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(E_SIG);
	CHECK_RESULTS(results29)

	const char * const results30[] =
	{
	"I:", "d1-I;"
	};
	dispatch(I_SIG);
	CHECK_RESULTS(results30)

	const char * const results31[] =
	{
	"F:", "s1-F;", "s11-EXIT;", "s1-EXIT;", "s2-ENTRY;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(F_SIG);
	CHECK_RESULTS(results31)

	const char * const results32[] =
	{
	"I:", "s2-I;"
	};
	dispatch(I_SIG);
	CHECK_RESULTS(results32)

	const char * const results33[] =
	{
	"I:", "s-I;"
	};
	dispatch(I_SIG);
	CHECK_RESULTS(results33)

	const char * const results34[] =
	{
	"F:", "s2-F;", "s211-EXIT;", "s21-EXIT;", "s2-EXIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(F_SIG);
	CHECK_RESULTS(results34)

	const char * const results35[] =
	{
	"A:", "s1-A;", "s11-EXIT;", "s1-EXIT;", "s1-ENTRY;", "s1-INIT;", "s11-ENTRY;"
	};
	dispatch(A_SIG);
	CHECK_RESULTS(results35)

	const char * const results36[] =
	{
	"A:", "s1-A;", "s11-EXIT;", "s1-EXIT;", "s1-ENTRY;", "s1-INIT;", "s11-ENTRY;"
	};
	dispatch(A_SIG);
	CHECK_RESULTS(results36)

	const char * const results37[] =
	{
	"B:", "s1-B;", "s11-EXIT;", "s11-ENTRY;"
	};
	dispatch(B_SIG);
	CHECK_RESULTS(results37)

	const char * const results38[] =
	{
	"B:", "s1-B;", "s11-EXIT;", "s11-ENTRY;"
	};
	dispatch(B_SIG);
	CHECK_RESULTS(results38)

	const char * const results39[] =
	{
	"D:", "s1-D;", "s11-EXIT;", "s1-EXIT;", "s-INIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results39)

	const char * const results40[] =
	{
	"D:", "s11-D;", "s11-EXIT;", "s1-INIT;", "s11-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results40)

	const char * const results41[] =
	{
	"D:", "s1-D;", "s11-EXIT;", "s1-EXIT;", "s-INIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results41)

	const char * const results42[] =
	{
	"D:", "s11-D;", "s11-EXIT;", "s1-INIT;", "s11-ENTRY;"
	};
	dispatch(D_SIG);
	CHECK_RESULTS(results42)

	const char * const results43[] =
	{
	"E:", "s-E;", "s11-EXIT;", "s1-EXIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(E_SIG);
	CHECK_RESULTS(results43)

	const char * const results44[] =
	{
	"G:", "s11-G;", "s11-EXIT;", "s1-EXIT;", "s2-ENTRY;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(G_SIG);
	CHECK_RESULTS(results44)

	const char * const results45[] =
	{
	"H:", "s211-H;", "s211-EXIT;", "s21-EXIT;", "s2-INIT;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(H_SIG);
	CHECK_RESULTS(results45)

	const char * const results46[] =
	{
	"G:", "s21-G;", "s211-EXIT;", "s21-EXIT;", "s2-EXIT;", "s1-ENTRY;", "s1-INIT;", "s11-ENTRY;"
	};
	dispatch(G_SIG);
	CHECK_RESULTS(results46)

	const char * const results47[] =
	{
	"H:", "s11-H;", "s11-EXIT;", "s1-EXIT;", "s-INIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(H_SIG);
	CHECK_RESULTS(results47)

	const char * const results48[] =
	{
	"F:", "s1-F;", "s11-EXIT;", "s1-EXIT;", "s2-ENTRY;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(F_SIG);
	CHECK_RESULTS(results48)

	const char * const results49[] =
	{
	"H:", "s211-H;", "s211-EXIT;", "s21-EXIT;", "s2-INIT;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(H_SIG);
	CHECK_RESULTS(results49)

	const char * const results50[] =
	{
	"F:", "s2-F;", "s211-EXIT;", "s21-EXIT;", "s2-EXIT;", "s1-ENTRY;", "s11-ENTRY;"
	};
	dispatch(F_SIG);
	CHECK_RESULTS(results50)

	const char * const results51[] =
	{
	"C:", "s1-C;", "s11-EXIT;", "s1-EXIT;", "s2-ENTRY;", "s2-INIT;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(C_SIG);
	CHECK_RESULTS(results51)

	const char * const results52[] =
	{
	"G:", "s21-G;", "s211-EXIT;", "s21-EXIT;", "s2-EXIT;", "s1-ENTRY;", "s1-INIT;", "s11-ENTRY;"
	};
	dispatch(G_SIG);
	CHECK_RESULTS(results52)

	const char * const results53[] =
	{
	"G:", "s11-G;", "s11-EXIT;", "s1-EXIT;", "s2-ENTRY;", "s21-ENTRY;", "s211-ENTRY;"
	};
	dispatch(G_SIG);
	CHECK_RESULTS(results53)
}

