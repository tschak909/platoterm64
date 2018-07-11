/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.c - Input/output functions (serial/ethernet) (apple2 specific)
 */

#include <apple2.h>
#include <peekpoke.h>
#include <stdint.h>

extern uint8_t xoff_enabled;

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size(void)
{
  return 0;
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off(void)
{
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on(void)
{
}
