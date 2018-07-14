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
#include <string.h>
#include "../config.h"

extern uint8_t xoff_enabled;
extern ConfigInfo config;
extern uint8_t (*io_serial_buffer_size)(void);
extern void (*io_recv_serial_flow_off)(void);
extern void (*io_recv_serial_flow_on)(void);

void io_recv_serial_flow_off_ssc(void);
void io_recv_serial_flow_on_ssc(void);
uint8_t io_serial_buffer_size_ssc(void);

/**
 * io_init_funcptrs() - Set up I/O function pointers
 */
void io_init_funcptrs(void)
{
  if (strcmp(config.driver_ser,CONFIG_SERIAL_DRIVER_SSC)==0)
    {
      io_serial_buffer_size=io_serial_buffer_size_ssc;
      io_recv_serial_flow_off=io_recv_serial_flow_off_ssc;
      io_recv_serial_flow_on=io_recv_serial_flow_on_ssc;
    }
}

/**
 * Return the serial buffer size
 */
uint8_t io_serial_buffer_size_ssc(void)
{
  return 0;
}

/**
 * io_recv_serial_flow_off() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_off_ssc(void)
{
}

/**
 * io_recv_serial_flow_on() - Tell modem to stop receiving.
 */
void io_recv_serial_flow_on_ssc(void)
{
}
