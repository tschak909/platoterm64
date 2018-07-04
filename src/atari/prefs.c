/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions - Atari specific
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../prefs.h"
#include "../config.h"

extern ConfigInfo config;
extern uint8_t ch;
extern uint8_t prefs_need_updating;

/**
 * prefs_touch(void)
 * Preferences for selecting touch driver
 */
void prefs_touch(void)
{
  prefs_display("a)miga j)oy k)oala s)t t)rkball b)ack: ");

  ch=prefs_get_key_matching("ajkstb");

  switch(ch)
    {
    case 'a':
      prefs_select("amiga");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRAMI);
      prefs_need_updating=true;
      break;
    case 'j':
      prefs_select("joy");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRJOY);
      prefs_need_updating=true;
      break;
    case 'k':
      prefs_select("koala");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRTT);
      prefs_need_updating=true;
      break;
    case 's':
      prefs_select("st");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRST);
      prefs_need_updating=true;
      break;
    case 't':
      prefs_select("trkball");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRTRK);
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_driver(void)
 * Preferences menu to select serial driver
 */
void prefs_driver(void)
{
  prefs_display("driver - r)device b)ack: ");
  
  ch=prefs_get_key_matching("rb");

  switch(ch)
    {
    case 's':
      prefs_select("swiftlink232");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_ATRRDEV);
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

