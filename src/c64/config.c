/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <c64.h>
#include <cbm.h>
#include <serial.h>
#include <stdbool.h>
#include <string.h>
#include "../config.h"

extern ConfigInfo config;

#define CONFIG_DEFAULT_SERIAL_DRIVER CONFIG_SERIAL_DRIVER_UP2400
#define CONFIG_DEFAULT_MOUSE_DRIVER CONFIG_MOUSE_DRIVER_1351

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void)
{
  config.io_mode = IO_MODE_SERIAL;
  config.baud = SER_BAUD_2400;
  config.use_dhcp = false;
  config.ip_address=0;
  config.netmask=0;
  config.gateway=0;
  config.dns=0;
  strcpy(config.driver_ser,CONFIG_DEFAULT_SERIAL_DRIVER);
  strcpy(config.driver_mou,CONFIG_DEFAULT_MOUSE_DRIVER);
  config.color_foreground=TGI_COLOR_LIGHTBLUE;
  config.color_background=TGI_COLOR_BLUE;
  config.color_border=TGI_COLOR_LIGHTBLUE;
  config_save();
}
