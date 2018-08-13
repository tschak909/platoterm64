/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.c - Keyboard functions (atari)
 */

#include <stdint.h>
#include <stdbool.h>
#include <peekpoke.h>
#include "../prefs.h"

static uint8_t lastkey;
static uint8_t keyboard_start_pressed;
static uint8_t keyboard_select_pressed;
static uint8_t keyboard_option_pressed;
extern uint8_t xoff_enabled;

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  /* // Read console keys, no need to read combos of these at present. */
  /* if (PEEK(0xD01F)==6) */
  /*   keyboard_start_pressed=true; */
  /* else if (PEEK(0xD01F)==5) */
  /*   keyboard_select_pressed=true; */
  /* else if (PEEK(0xD01F)==3) */
  /*   keyboard_option_pressed=true; */

  /* // Process non-terminal keys */
  /* if (keyboard_option_pressed==true) */
  /*   prefs_run(); */
  
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{

}
