/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.c - Keyboard functions (apple2)
 */

#include <stdint.h>
#include <stdbool.h>
#include <peekpoke.h>
#include <conio.h>
#include "../screen.h"
#include "../prefs.h"
#include "../keyboard.h"
#include "key.h"

static uint8_t ch;
static uint8_t is_escape=false;
extern uint8_t xoff_enabled;

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  if (kbhit())
    {
      ch=cgetc();
      if (ch=0x1B) // ESC
	is_escape=true;
      else if (is_escape)
	{
	  keyboard_out(esc_key_to_pkey[ch]);
	  is_escape=false;
	}
      else
	keyboard_out(key_to_pkey[ch]);
    }
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{
}
