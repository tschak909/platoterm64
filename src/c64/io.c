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
#include <string.h>
#include "../io.h"
#include "../config.h"

extern uint8_t xoff_enabled;
extern ConfigInfo config;
extern uint8_t (*io_serial_buffer_size)(void);
extern void (*io_recv_serial_flow_off)(void);
extern void (*io_recv_serial_flow_on)(void);

void io_recv_serial_flow_off_user_port(void);
void io_recv_serial_flow_on_user_port(void);
uint8_t io_serial_buffer_size_user_port(void);
void io_recv_serial_flow_off_swiftlink(void);
void io_recv_serial_flow_on_swiftlink(void);
uint8_t io_serial_buffer_size_swiftlink(void);

/**
 * io_init_funcptrs() - Set up I/O function pointers
 */
void io_init_funcptrs(void)
{
  POKE(0xD020,0);
  if (strcmp(config.driver_ser,CONFIG_SERIAL_DRIVER_UP2400)==0)
    {
      POKE(0xD020,2);
      io_serial_buffer_size=io_serial_buffer_size_user_port;
      io_recv_serial_flow_off=io_recv_serial_flow_off_user_port;
      io_recv_serial_flow_on=io_recv_serial_flow_on_user_port;
    }
  else if (strcmp(config.driver_ser,CONFIG_SERIAL_DRIVER_SWIFTLINK)==0)
    {
      POKE(0xD020,3);
      io_serial_buffer_size=io_serial_buffer_size_swiftlink;
      io_recv_serial_flow_off=io_recv_serial_flow_off_swiftlink;
      io_recv_serial_flow_on=io_recv_serial_flow_on_swiftlink;
    }
}

/********* USER PORT *****************************/

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size_user_port(void)
{
  return PEEK(0x29B)-PEEK(0x29C)&0xff;
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off_user_port(void)
{
  // for now, assume user port.
  POKE(0xD020,0);
  xoff_enabled=true;
  POKE(0xDD01,PEEK(0xDD01)&~0x02);
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on_user_port(void)
{
  // For now, assume user port.
  POKE(0xD020,14);
  xoff_enabled=false;
  POKE(0xDD01,PEEK(0xDD01)|0x02);
}

/************** SWIFTLINK ***********************/

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size_swiftlink(void)
{
  return PEEK(0xF9)-PEEK(0xF8);
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off_swiftlink(void)
{
  io_send_byte(0x13);
  xoff_enabled=true;
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on_swiftlink(void)
{
  io_send_byte(0x11);
  xoff_enabled=false;
}
