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
#include <unistd.h>
#include <fcntl.h>

#include "../config.h"
#include "../screen.h"

#define CONFIG_DEFAULT_SERIAL_DRIVER CONFIG_SERIAL_DRIVER_ATRRDEV
#define CONFIG_DEFAULT_MOUSE_DRIVER CONFIG_MOUSE_DRIVER_ATRJOY

extern unsigned char recv_buffer; // recycling from io_base.c :)
extern ConfigInfo config;

#define CONFIG_FILE "D:CONFIG"

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
  int fd=open(CONFIG_FILE,O_RDONLY);

  if (fd < 0)
    config_set_defaults();
  else
    {
      read(fd, &config, sizeof(config));
      close(fd);
    }
  close(fd);
}

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void)
{
  int fd=open(CONFIG_FILE,O_WRONLY|O_CREAT);
  if (fd<0)
    {
    }
  else
    {
      write(fd,&config,sizeof(config));
    }
  close(fd);
}

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
