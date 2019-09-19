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
#include <peekPOKE.h>
#include <stdint.h>
#include <string.h>
#include <serial.h>
#include "../config.h"

extern uint8_t io_load_successful;
extern uint8_t xoff_enabled;
extern ConfigInfo config;
extern uint8_t (*io_serial_buffer_size)(void);
extern void (*io_recv_serial_flow_off)(void);
extern void (*io_recv_serial_flow_on)(void);

void io_recv_serial_flow_off_atari(void);
void io_recv_serial_flow_on_atari(void);
uint8_t io_serial_buffer_size_atari(void);

/**
 * io_init_funcptrs() - Set up I/O function pointers
 */
void io_init_funcptrs(void)
{
  if (io_load_successful==false)
    return;

  if (config.driver_ser==CONFIG_SERIAL_DRIVER_ATRRDEV)
    {
      io_serial_buffer_size=io_serial_buffer_size_atari;
      io_recv_serial_flow_off=io_recv_serial_flow_off_atari;
      io_recv_serial_flow_on=io_recv_serial_flow_on_atari;
    }
}

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
uint8_t io_serial_buffer_size_atari(void)
{
  return 0; // Not implemented.
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off_atari(void)
{
  if (io_load_successful==false)
    return;
  xoff_enabled=true;
  POKE(712,35);
  ser_put(0x13);
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on_atari(void)
{
  if (io_load_successful==false)
    return;
  xoff_enabled=false;
  POKE(712,0);
  ser_put(0x11);
}

/**
 * io_ser_driver_name() - return serial driver name given constant
 */
const char* io_ser_driver_name(unsigned char driver)
{
  switch(driver)
    {
    case CONFIG_SERIAL_DRIVER_ATRRDEV:
      return "atrrdev.ser";
    }
}
