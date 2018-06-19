/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.c - Keyboard functions
 */

#include <stdint.h>
#include <peekpoke.h>
#include "keyboard.h"
#include "key.h"
#include "screen.h"
#include "protocol.h"

static uint8_t lastkey;

/**
 * handle_keyboard - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void handle_key(uint8_t platoKey)
{
  if (platoKey==0xff)
    return;
  
  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;
}

/**
 * handle_keyboard - Handle the keyboard presses
 */
void handle_keyboard(void)
{
  uint8_t key=PEEK(0xCB);
  uint8_t modifier=PEEK(0x28D);

  // Handle Function keys
  if (key==0x04 && lastkey!=0x04)
    {
      // Change colors
      if (modifier==0x00)
	{
	  screen_cycle_background();
	}
      else if (modifier==0x01)
	{
	  screen_cycle_foreground();
	}
      else if (modifier==0x02)
	{
	  screen_cycle_border();
	}
      set_terminal_colors();
    }
  
  if (key!=lastkey)
    {  
      if (modifier==MODIFIER_NONE)
	handle_key(KEYBOARD_TO_PLATO[key]);
      else if (modifier==MODIFIER_SHIFT)
	handle_key(KEYBOARD_TO_PLATO_SHIFT[key]);
      else if (modifier==MODIFIER_COMMO)
	handle_key(KEYBOARD_TO_PLATO_COMMO[key]);
      else if (modifier==MODIFIER_COMMO_SHIFT)
	handle_key(KEYBOARD_TO_PLATO_CS[key]);
      else if (modifier==MODIFIER_CTRL)
	handle_key(KEYBOARD_TO_PLATO_CTRL[key]);
      else if (modifier==MODIFIER_CTRL_SHIFT)
	handle_key(KEYBOARD_TO_PLATO_CTRL_SHIFT[key]);
    }
      lastkey=key;
}

