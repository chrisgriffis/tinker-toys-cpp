/*****************************************************************************
* Product:  VGA screen output
* Version:  3.0
* Updated:  Jan 23, 2005
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
* terms of Quantum Leaps commercial licenses, which are designed for users
* who want to retain proprietary status of their code. This "dual-licensing"
* model is possible because Quantum Leaps owns the copyright to this source
* code and as such can license its intelectual property any number of times.
* The users who license this software under one of Quantum Leaps commercial
* licenses do not use this software under the GPL and therefore are not
* subject to any of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
* Quantum Leaps products:  http://www.quantum-leaps.com/products
* e-mail:                  sales@quantum-leaps.com
*****************************************************************************/
#include <stdint.h>
#include <conio.h>
#include <dos.h>

#include "video.h"

/*..........................................................................*/
void Video_clearScreen(uint8_t bgColor) {
    clrscr();
    Video_clearRect(0,  0, 80, 25, bgColor);
}
/*..........................................................................*/
void Video_clearRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                     uint8_t bgColor)
{
    for ( ; y1 < y2; ++y1) {
        uint8_t x;
        uint8_t far *pscr = (uint8_t far *)MK_FP(0xB800,
                                           (uint16_t)(((y1 * 80) + x1) * 2));
        for (x = x1; x < x2; ++x) {
            pscr[0] = ' ';                        /* Put space in video RAM */
            pscr[1] = bgColor;          /* Put video attribute in video RAM */
            pscr += 2;
        }
    }
}
/*..........................................................................*/
void Video_printStrAt(uint8_t x, uint8_t y, uint8_t color,
                      char const *str)
{
                               /* calculate position on the video RAM (VGA) */
    uint8_t far *pscr = (uint8_t far *)MK_FP(0xB800,
                                             (uint16_t)(((y * 80) + x) * 2));
    while (*str != (uint8_t)0) {
        pscr[0] = *str++;                     /* Put character in video RAM */
        pscr[1] |= color;               /* Put video attribute in video RAM */
        pscr += 2;
    }
}
/*..........................................................................*/
void Video_printNumAt(uint8_t x, uint8_t y, uint8_t color, uint32_t num) {
    char buf[4];
    buf[3] = (char)0;
    buf[2] = (char)('0' + num % 10);
    num /= 10;
    buf[1] = (char)('0' + num % 10);
    num /= 10;
    buf[0] = (char)('0' + num % 10);
    if (buf[0] == '0') {
        buf[0] = ' ';
    }
    Video_printStrAt(x, y, color, buf);
}
