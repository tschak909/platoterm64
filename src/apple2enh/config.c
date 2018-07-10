/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <apple2.h>
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
  config.io_mode = IO_MODE_ETHERNET;
  config.baud = SER_BAUD_2400;
  config.use_dhcp = true;
  config.ip_address=0;
  config.netmask=0;
  config.gateway=0;
  config.dns=0;
  strcpy(config.driver_ser,CONFIG_DEFAULT_SERIAL_DRIVER);
  strcpy(config.driver_mou,CONFIG_DEFAULT_MOUSE_DRIVER);
  config_save();
}
