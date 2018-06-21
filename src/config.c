/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <stdio.h>
#include <string.h>
#include <serial.h>
#include <stdbool.h>
#include <c64.h>
#include "config.h"
#include "screen.h"

#define CONFIG_FILE "config"

ConfigInfo config;

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void)
{
  memset(&config,0,sizeof(config));
  config_load();
  screen_update_colors(); /* because the screen is already initialized. */
}

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void)
{
  FILE *fp;
  fp = fopen(CONFIG_FILE,"r");
  
  if (!fp)
    {
      config_set_defaults();
    }
  else
    {
      fread(&config,1,sizeof(config),fp);
    }
  fclose(fp);
}

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void)
{
  FILE *fp;
  fp = fopen(CONFIG_FILE,"w");

  if (!fp)
    return;

  fwrite(&config,1,sizeof(config),fp);
  fclose(fp);
}

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void)
{
  config.io_mode = IO_MODE_SERIAL;
  config.baud = SER_BAUD_19200;
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

