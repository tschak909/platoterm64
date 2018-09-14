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
#include "config.h"
#include "screen.h"

#ifdef __ATARI__
#define CONFIG_FILE "D:CONFIG"
#else
#define CONFIG_FILE "config"
#endif

ConfigInfo config;
char configFile[13];

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void)
{
  strcpy(configFile,CONFIG_FILE);
  config_init_hook();
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
  fp = fopen(configFile,"r");
  
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
  fp = fopen(configFile,"w");

  if (!fp)
    return;

  fwrite(&config,1,sizeof(config),fp);
  fclose(fp);
}

