/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.c - Input/output functions (serial/ethernet) (atari specific)
 */

#include <atari.h>
#include <stdbool.h>
#include <peekpoke.h>
#include <stdint.h>
#include <string.h>
#include <serial.h>

extern uint8_t io_load_successful;
extern uint8_t xoff_enabled;

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(uint8_t b)
{
  if (io_load_successful==false)
    return;

  ser_put(b);
}

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size(void)
{
  return 0; // Not implemented.
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off(void)
{
  if (io_load_successful==false)
    return;
  xoff_enabled=true;
  ser_put(0x13);
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on(void)
{
  if (io_load_successful==false)
    return;
  xoff_enabled=false;
  ser_put(0x11);
}

