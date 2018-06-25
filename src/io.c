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

uint8_t xoff_enabled=false;

static uint8_t ch=0;
static uint8_t lastch=0;
static uint8_t io_res;
static uint8_t recv_buffer_size=0;
static uint8_t xoff_counter=0;
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

  if (io_res!=SER_ERR_OK)
    {
      POKE(0xD020,2);
      /* printf("ser_install returned: %d\n",io_res); */
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
	  /* printf("ser_open returned: %d\n",io_res); */
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
  recv_buffer_size=PEEK(0x29B)-PEEK(0x29C)&0xff;
  if (recv_buffer_size>200 && xoff_enabled==false)
    {
      io_send_byte(XOFF);
      xoff_counter=200;
      xoff_enabled=true;
    }

  if (xoff_enabled==true && xoff_counter==0)
    {
      io_send_byte(XON);
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
	  if (xoff_counter>0)
	    --xoff_counter;
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
