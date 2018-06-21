/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * plato.c - main program
 */

#include <serial.h>
#include "protocol.h"
#include "terminal.h"
#include "welcome.h"
#include "screen.h"
#include "touch.h"
#include "keyboard.h"
#include "io.h"
#include "config.h"

/**
 * greeting(void) - Show terminal greeting
 */
void greeting(void)
{
  padPt coord;

  coord.x=168; coord.y=480; screen_char_draw(&coord,welcomemsg_1,WELCOMEMSG_1_LEN);
  coord.x=144; coord.y=464; screen_char_draw(&coord,welcomemsg_2,WELCOMEMSG_2_LEN);
  coord.x=104; coord.y=432; screen_char_draw(&coord,welcomemsg_3,WELCOMEMSG_3_LEN);
  coord.x=160; coord.y=416; screen_char_draw(&coord,welcomemsg_4,WELCOMEMSG_4_LEN);
  coord.x=16;  coord.y=384; screen_char_draw(&coord,welcomemsg_5,WELCOMEMSG_5_LEN);
}

void main(void)
{
  screen_init();
  config_init();
  io_init();
  // touch_init();
  greeting();
  
  // And do the terminal
  for (;;)
    {
      io_main();
      keyboard_main();
      // touch_main();
    }
  
  screen_done();
  touch_done();
}
