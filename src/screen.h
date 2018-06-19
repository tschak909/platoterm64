/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.h - Display output functions
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "protocol.h"

/**
 * screen_init() - Set up the screen
 */
void screen_init(void);

/**
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void);

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void);

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 */
void screen_cycle_border(void);

/**
 * Set the terminal colors
 */
void set_terminal_colors(void);

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void Wait(void);

/**
 * Beep(void) - Beep the terminal
 */
void Beep(void);

/**
 * ClearScreen - Clear the screen
 */
void ClearScreen(void);

/**
 * BlockDraw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void BlockDraw(padPt* Coord1, padPt* Coord2);

/**
 * dotDraw(Coord) - Plot a mode 0 pixel
 */
void DotDraw(padPt* Coord);

/**
 * lineDraw(Coord1, Coord2) - Draw a mode 1 line
 */
void LineDraw(padPt* Coord1, padPt* Coord2);

/**
 * CharDraw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void CharDraw(padPt* Coord, unsigned char* ch, unsigned char count);

/**
 * TTYChar - Called to plot chars when in tty mode
 */
void TTYChar(padByte theChar);



#endif /* SCREEN_H */
