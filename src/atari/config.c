/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <atari.h>
#include <serial.h>
#include <stdbool.h>
#include <string.h>
#include <peekpoke.h>
#include "../config.h"

#define CONFIG_DEFAULT_SERIAL_DRIVER CONFIG_SERIAL_DRIVER_ATRRDEV
#define CONFIG_DEFAULT_MOUSE_DRIVER CONFIG_MOUSE_DRIVER_ATRJOY

extern ConfigInfo config;

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void)
{
  config.baud = SER_BAUD_1200;
  config.xon_threshold=XON_THRESHOLD;
  config.xoff_threshold=XOFF_THRESHOLD;
  config.fill=false;
  config.driver_ser=CONFIG_DEFAULT_SERIAL_DRIVER;
  config.driver_mou=CONFIG_DEFAULT_MOUSE_DRIVER;
  config.color_foreground=TGI_COLOR_WHITE;
  config.color_background=TGI_COLOR_BLACK;
  config.color_border=TGI_COLOR_BLACK;
  config_save();
}
