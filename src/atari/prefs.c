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
extern uint8_t touch_prefs_updated;
extern uint8_t io_prefs_updated;
extern uint8_t io_load_successful;

/**
 * Show PLATOTERM READY - Press '<key>' for setup.
 */
void prefs_show_greeting(void)
{
  /* if (io_load_successful==false) */
  /*   prefs_display("serial driver didn't load. Check handler."); */
  /* else */
  /*   prefs_display("platoterm ready - option for setup"); */
}

/**
 * prefs_touch(void)
 * Preferences for selecting touch driver
 */
void prefs_touch(void)
{
  prefs_display("a)miga j)oy k)oala s)t t)rkball n)one b)ack: ");

  ch=prefs_get_key_matching("ajkstnbAJKSTNB");

  switch(ch)
    {
    case 'a':
      prefs_select("amiga");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRAMI);
      touch_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'j':
      prefs_select("joy");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRJOY);
      touch_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'k':
      prefs_select("koala");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRTT);
      touch_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 's':
      prefs_select("st");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRST);
      touch_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 't':
      prefs_select("trkball");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_ATRTRK);
      touch_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'n':
      prefs_select("none");
      strcpy(config.driver_mou,"NONE");
      touch_prefs_updated=true;
      prefs_need_updating=true;
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
  
  ch=prefs_get_key_matching("rbRB");

  switch(ch)
    {
    case 'r':
      prefs_select("rdevice");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_ATRRDEV);
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

