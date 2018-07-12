/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.c - Input/output functions (serial/ethernet) (c64 specific)
 */

#include <cbm.h>
#include <c64.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdbool.h>

extern uint8_t xoff_enabled;

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size(void)
{
  return PEEK(0x29B)-PEEK(0x29C)&0xff;
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off(void)
{
  // for now, assume user port.
  xoff_enabled=true;
  POKE(0xDD01,PEEK(0xDD01)&0x02);
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on(void)
{
  // For now, assume user port.
  xoff_enabled=false;
  POKE(0xDD01,PEEK(0xDD01)|0x02);
}
