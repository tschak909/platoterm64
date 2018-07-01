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
#include <peekpoke.h>
#include "io.h"
#include "protocol.h"
#include "config.h"

#define NULL 0

#define XOFF_THRESHOLD 250
#define XON_THRESHOLD  100

uint8_t xoff_enabled;

static uint8_t ch=0;
static uint8_t lastch=0;
static uint8_t io_res;
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
      POKE(0xD020,2);
      return;
    }

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
	  POKE(0xD020,2);
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
  if ((xoff_enabled==false) || (b==0x11))
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
  recv_buffer_size=PEEK(0x29B)-PEEK(0x29C)&0xff;
  if (recv_buffer_size>XOFF_THRESHOLD && xoff_enabled==false)
    {
      POKE(0xD020,0);
      ser_put(0x13);
      xoff_enabled=true;
    }
  else if (recv_buffer_size<XON_THRESHOLD && xoff_enabled==true)
    {
      POKE(0xD020,14);
      ser_put(0x11);
      xoff_enabled=false;
    }

  if (ser_get(&ch)==SER_ERR_OK)
    {
      // Detect and strip IAC escapes (two consecutive bytes of 0xFF)
      if (ch==0xFF && lastch == 0xFF)
	{
	  lastch=0x00;
	}
      else
	{
	  lastch=ch;
	  ShowPLATO(&ch,1);
	}
    }
}

/**
 * io_done() - Called to close I/O
 */
void io_done(void)
{
  ser_close();
  ser_uninstall();
}
