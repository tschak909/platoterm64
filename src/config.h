/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.h - Configuration routines
 */

#ifndef CONFIG_H
#define CONFIG_H

#define IO_MODE_SERIAL        0
#define IO_MODE_ETHERNET      1

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void);

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void);

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void);

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void);

/**
 * config_get_serial_driver()
 * Get the defined serial driver from configuration
 */
const char* config_get_serial_driver(void);

/**
 * config_get_baud_rate()
 * Get the defined serial baud rate from configuration
 */
unsigned char config_get_baud_rate(void);

/** 
 * config_get_mouse_driver()
 * Get the defined mouse driver from configuration
 */
const char* config_get_mouse_driver(void);

#endif /* CONFIG_H */
