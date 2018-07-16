/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <apple2.h>
#include <peekpoke.h>
#include <tgi.h>
#include <stdint.h>
#include "../config.h"

extern ConfigInfo config; 

#define outb(addr,val)        (*(addr)) = (val)
#define outw(addr,val)        (*(addr)) = (val)

extern void install_nmi_trampoline(void); /* nmi_trampoline.s */

static uint8_t bp=0;
static uint8_t bd=0;

/**
 * screen_load_driver()
 * Load the TGI driver
 */
void screen_load_driver(void)
{
  tgi_install(tgi_static_stddrv);
}

/**
 * screen_init_hook()
 * Called after tgi_init to set any special features, e.g. nmi trampolines.
 */
void screen_init_hook(void)
{
}

/**
 * Set the terminal colors
 */
void screen_update_colors(void)
{
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  // TODO: 60hz wait
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  for (bd=0;bd<75;++bd)
    {
      POKE(0xC030,0);
      for (bp=0;bp<75;++bp)
	{
	}
    }
}
