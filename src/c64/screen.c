/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <c64.h>
#include <cbm.h>
#include <peekpoke.h>
#include <tgi.h>
#include <stdint.h>
#include "../config.h"

extern uint8_t pal[2];
extern ConfigInfo config; 

#define outb(addr,val)        (*(addr)) = (val)
#define outw(addr,val)        (*(addr)) = (val)

extern void install_nmi_trampoline(void); /* nmi_trampoline.s */

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
  install_nmi_trampoline();
}

/**
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void)
{
  ++config.color_foreground;
}

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void)
{
  ++config.color_background;
}

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 */
void screen_cycle_border(void)
{
  ++config.color_border;
}

/**
 * Set the terminal colors
 */
void screen_update_colors(void)
{
  pal[0]=config.color_background;
  pal[1]=config.color_foreground;
  tgi_setpalette(pal);
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  waitvsync();
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  // My feeble attempt at a beep.
  outw(&SID.v1.freq,0x22cd);
  outw(&SID.v1.pw,0x0800);
  outb(&SID.v1.ad,0x33);
  outb(&SID.v1.sr,0xF0);
  outb(&SID.amp,0x5F);
  outw(&SID.flt_freq,0xF0F0);
  outb(&SID.flt_ctrl,0xF2);
  outb(&SID.v1.ctrl,0x11);
  waitvsync();
  waitvsync();
  waitvsync();
  waitvsync();
  outb(&SID.v1.ctrl,0);
}
