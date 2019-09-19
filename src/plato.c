/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * plato.c - main program
 */

#include <stdbool.h>
#include "terminal.h"
#include "screen.h"
#include "touch.h"
#include "keyboard.h"
#include "io.h"
#include "config.h"
#include "prefs.h"

uint8_t already_started=false;

void main(void)
{
  screen_init();
  config_init();
  touch_init();
  terminal_init();
  screen_splash();
  io_init();
  prefs_show_greeting();
  terminal_initial_position();
  screen_beep();
  
  already_started=true;
  
  // And do the terminal
  for (;;)
    {
      io_main();
      keyboard_main();
      touch_main();
    }
  
  screen_done();
  touch_done();
}
