/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.c - Keyboard functions (apple2)
 */

#include <stdint.h>
#include <peekpoke.h>
#include "../screen.h"
#include "../prefs.h"
#include "../keyboard.h"
#include "key.h"

static uint8_t lastkey;
extern uint8_t xoff_enabled;

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{
}
