/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * system.c - System specific functions
 */

#include <cbm.h>
#include <c64.h>

/**
 * Wait one frame of time
 */
void system_wait_frame(void)
{
  waitvsync();
}

/**
 * System specific screen initialization hook.
 * called after tgi_init.
 */ 
void system_screen_init_hook(void)
{
  install_nmi_trampoline();
}

/**
 * System specific beep
 */
void system_screen_beep(void)
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
