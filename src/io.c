/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.c - Input/output functions (serial/ethernet)
 */

#include <serial.h>
#include <stdint.h>
#include "io.h"
#include "protocol.h"
#include "config.h"

#define NULL 0

static uint8_t ch=0;
static uint8_t lastch=0;

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
  uint8_t res;
  res=ser_load_driver(config.driver_ser);

  if (res!=SER_ERR_OK)
    {
      /* printf("ser_install returned: %d\n",res); */
      return;
    }

  params.baudrate = config.baud;
  
  res=ser_open(&params);

  if (res!=SER_ERR_OK)
    {
      /* printf("ser_open returned: %d\n",res); */
      return;
    }

  // Needed to enable up2400. Ignored with swlink.
  ser_ioctl(1, NULL);
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
