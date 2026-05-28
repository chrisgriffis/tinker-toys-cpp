//////////////////////////////////////////////////////////////////////////////
// Product: Direct Video (VGA) screen output
// Version: 1.0
// Updated: Jan 23, 2005
//
// Copyright (C) 2002-2005 Quantum Leaps, LLC. All rights reserved.
//
// This software may be distributed and modified under the terms of the GNU
// General Public License version 2 (GPL) as published by the Free Software
// Foundation and appearing in the file GPL.TXT included in the packaging of
// this file. Please note that GPL Section 2[b] requires that all works based
// on this software must also be made publicly available under the terms of
// the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which are designed for users
// who want to retain proprietary status of their code. This "dual-licensing"
// model is possible because Quantum Leaps owns the copyright to this source
// code and as such can license its intelectual property any number of times.
// The users who license this software under one of Quantum Leaps commercial
// licenses do not use this software under the GPL and therefore are not
// subject to any of its terms.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// Quantum Leaps licensing: http://www.quantum-leaps.com/licensing
// Quantum Leaps products:  http://www.quantum-leaps.com/products
// e-mail:                  sales@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#ifndef video_h
#define video_h

class Video {
public:
    enum VideoColor {
        /* foreground */
        FGND_BLACK        = 0x00,
        FGND_BLUE         = 0x01,
        FGND_GREEN        = 0x02,
        FGND_CYAN         = 0x03,
        FGND_RED          = 0x04,
        FGND_PURPLE       = 0x05,
        FGND_BROWN        = 0x06,
        FGND_LIGHT_GRAY   = 0x07,
        FGND_DARK_GRAY    = 0x08,
        FGND_LIGHT_BLUE   = 0x09,
        FGND_LIGHT_GREEN  = 0x0A,
        FGND_LIGHT_CYAN   = 0x0B,
        FGND_LIGHT_RED    = 0x0C,
        FGND_LIGHT_PURPLE = 0x0D,
        FGND_YELLOW       = 0x0E,
        FGND_WHITE        = 0x0F,
        /* background */
        BGND_BLACK        = 0x00,
        BGND_BLUE         = 0x10,
        BGND_GREEN        = 0x20,
        BGND_CYAN         = 0x30,
        BGND_RED          = 0x40,
        BGND_PURPLE       = 0x50,
        BGND_BROWN        = 0x60,
        BGND_LIGHT_GRAY   = 0x70,

        BGND_BLINK        = 0x80
    };
    static void clearScreen(uint8_t bgColor);
    static void clearRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                          uint8_t bgColor);
    static void printStrAt(uint8_t x, uint8_t y, uint8_t color,
                           char const *str);
    static void printNumAt(uint8_t x, uint8_t y, uint8_t color, uint32_t num);
};

#endif                                                              // video_h

