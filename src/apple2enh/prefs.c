/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions - Apple2 specific
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
  prefs_display("m)ouse b)ack: ");

  ch=prefs_get_key_matching("mb");

  switch(ch)
    {
    case 'm':
      prefs_select("mouse");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_STDMOU);
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
  prefs_display("driver - s)sc b)ack: ");
  
  ch=prefs_get_key_matching("sb");

  switch(ch)
    {
    case 's':
      prefs_select("ssc");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_SSC);
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

