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
#include <stdio.h>
#include <atari.h>
#include "io.h"
#include "protocol.h"

#define NULL 0

uint8_t xoff_enabled;
uint8_t io_load_successful=false;

static uint8_t ch=0;
static uint8_t io_res;
uint8_t recv_buffer[256];
static uint16_t recv_buffer_size=0;

static struct ser_params params = {
  SER_BAUD_1200,
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
  io_res=ser_install(atrrdev_ser);

  if (io_res==SER_ERR_OK)
    io_load_successful=true;
  
  xoff_enabled=false;

  if (io_load_successful)
    {
      io_open();
    }
  else
    {
    }
  
}

/**
 * io_open() - Open the device
 */
void io_open(void)
{
  
  io_res=ser_open(&params);
  
  if (io_res!=SER_ERR_OK)
    {
      io_load_successful=false;
    }
}

/**
 * io_main() - The IO main loop
 */
void io_main(void)
{
  if (io_load_successful==false)
    return;
  
  // Drain primary serial FIFO as fast as possible.
  while (ser_get(&ch)!=SER_ERR_NO_DATA)
    {
      recv_buffer[recv_buffer_size++]=ch;
    }
  
  if (xoff_enabled==false)
    {
      if (recv_buffer_size>127)
  	{
  	  io_recv_serial_flow_off();
  	}
    }
  else /* xoff_enabled==true */
    {
      if (xoff_enabled==true && recv_buffer_size<46)
  	{
  	  io_recv_serial_flow_on();
  	}
    }

  ShowPLATO(recv_buffer,recv_buffer_size);
  recv_buffer_size=0;
}

/**
 * io_change_baud() - Change baud rate.
 */
void io_change_baud(unsigned char baud)
{
  ser_close();
  params.baudrate=baud;
  ser_open(&params);
}

/**
 * io_done() - Called to close I/O
 */
void io_done(void)
{
  if (io_load_successful==false)
    return;
  
  ser_close();
  ser_uninstall();
}
