/*****************************************************************************
* Product:  QF/C
* Last Updated for Version: 3.1.03
* Date of the Last Update:  Nov 07, 2005
*
* Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly allow the
* licensees to retain the proprietary status of their code. The licensees
* who use this software under one of Quantum Leaps commercial licenses do
* not use this software under the GPL and therefore are not subject to any
* of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"

/*..........................................................................*/
/** \ingroup qf
* \file qf_psini.c
* \brief QF_subscrList_, QF_maxSignal_ definitions,
* and QF_psInit() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QSubscrList *QF_subscrList_;
QSignal QF_maxSignal_;

/*..........................................................................*/
void QF_psInit(QSubscrList *subscrSto, QSignal maxSignal) {
    QF_subscrList_ = subscrSto;
    QF_maxSignal_ = maxSignal;
}
