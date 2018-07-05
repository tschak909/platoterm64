/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.c - Input/output functions (serial/ethernet) (c64 specific)
 */

#include <atari.h>
#include <peekpoke.h>
#include <stdint.h>

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size(void)
{
  return 0; // Not implemented.
}
