/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions - C64 specific
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
  prefs_display("1)351 i)nkwell j)oy k)oala b)ack: ");

  ch=prefs_get_key_matching("1ijkb");

  switch(ch)
    {
    case '1':
      prefs_select("1351");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_1351);
      prefs_need_updating=true;
      break;
    case 'i':
      prefs_select("inkwell");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_INKWELL);
      prefs_need_updating=true;
      break;
    case 'j':
      prefs_select("joystick");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_JOY);
      prefs_need_updating=true;
      break;
    case 'k':
      prefs_select("koala");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_POT);
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
  prefs_display("driver - s)wiftlink232 u)serport2400 b)ack: ");
  
  ch=prefs_get_key_matching("sub");

  switch(ch)
    {
    case 's':
      prefs_select("swiftlink232");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_SWIFTLINK);
      prefs_need_updating=true;
      break;
    case 'u':
      prefs_select("userport2400");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_UP2400);
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

