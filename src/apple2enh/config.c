/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <apple2enh.h>
#include <serial.h>
#include <stdbool.h>
#include <string.h>
#include "../config.h"

extern ConfigInfo config;

#define CONFIG_DEFAULT_SERIAL_DRIVER CONFIG_SERIAL_DRIVER_SSC
#define CONFIG_DEFAULT_MOUSE_DRIVER CONFIG_MOUSE_DRIVER_STDMOU

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void)
{
  config.baud = SER_BAUD_2400;;
  config.xon_threshold=XON_THRESHOLD;
  config.xoff_threshold=XOFF_THRESHOLD;
  config.fill=false;
  config.driver_ser=CONFIG_DEFAULT_SERIAL_DRIVER;
  config.driver_mou=CONFIG_DEFAULT_MOUSE_DRIVER;
  config_save();
}
