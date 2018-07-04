/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * system.h - System specific functions
 */

#ifndef SYSTEM_H
#define SYSTEM_H

/**
 * Wait one frame of time
 */
void system_wait_frame(void);

/**
 * System specific screen initialization hook.
 * called after tgi_init.
 */ 
void system_screen_init_hook(void);

/**
 * System specific beep
 */
void system_screen_beep(void);

#endif /* SYSTEM_H */
