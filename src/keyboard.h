/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.h - Keyboard functions
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * handle_keyboard - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void handle_key(uint8_t platoKey);

/**
 * handle_keyboard - Handle the keyboard presses
 */
void handle_keyboard(void);

#endif
