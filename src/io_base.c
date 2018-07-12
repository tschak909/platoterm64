/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.c - Input/output functions (serial/ethernet)
 */

#include <stdbool.h>
#include <serial.h>
#include <stdint.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "io.h"
#include "protocol.h"
#include "config.h"

#define NULL 0

uint8_t xoff_enabled;

static uint8_t ch=0;
static uint8_t io_res;
static uint8_t* recv_buffer;
static uint8_t recv_buffer_size=0;
extern ConfigInfo config;

static struct ser_params params = {
  SER_BAUD_38400,
  SER_BITS_8,
  SER_STOP_1,
  SER_PAR_NONE,
  SER_HS_HW
};

/**
 * io_init() - Set-up the I/O
 */
void io_init(void)
{
  io_res=ser_load_driver(config.driver_ser);
  xoff_enabled=false;
  
  if (io_res!=SER_ERR_OK)
    {
      exit(0);
      return;
    }

  recv_buffer=malloc(2048);
  
  io_open();

}

/**
 * io_open() - Open the device
 */
void io_open(void)
{
  if (config.io_mode == IO_MODE_SERIAL)
    {
      params.baudrate = config.baud;
      
      io_res=ser_open(&params);
      
      if (io_res!=SER_ERR_OK)
	{
	  exit(0);
	  return;
	}
      
      // Needed to enable up2400. Ignored with swlink.
      ser_ioctl(1, NULL);
    }
  else if (config.io_mode == IO_MODE_ETHERNET)
    {
      // Not implemented, yet.
    }
}

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(uint8_t b)
{
  ser_put(b);
}

/**
 * io_main() - The IO main loop
 */
void io_main(void)
{
  io_recv_serial();
}

/**
 * io_recv_serial() - Receive and interpret serial data.
 */
void io_recv_serial(void)
{

  // Drain primary serial FIFO as fast as possible.
  while (ser_get(&ch)!=SER_ERR_NO_DATA)
    {
      recv_buffer[recv_buffer_size++]=ch;
    }
  
  if (xoff_enabled==false)
    {
      if (recv_buffer_size>2000)
  	{
  	  io_recv_serial_flow_off();
  	}
    }
  else /* xoff_enabled==true */
    {
      if (recv_buffer_size<64)
  	{
  	  io_recv_serial_flow_on();
  	}
    }

  ShowPLATO(recv_buffer,recv_buffer_size);
  recv_buffer_size=0;
  
}

/**
 * io_done() - Called to close I/O
 */
void io_done(void)
{
  ser_close();
  ser_uninstall();
  free(recv_buffer);
}
