#include <c64.h>
#include <6502.h>
#include <stdio.h>
#include <tgi.h>
#include <stdbool.h>
#include <conio.h>
#include <serial.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdlib.h>
#include "protocol.h"
#include "terminal.h"
#include "welcome.h"
#include "screen.h"
#include "touch.h"
#include "keyboard.h"

static uint8_t modemc=0;
static uint8_t lastmodemc=0;

extern uint16_t screen_w;
extern uint16_t screen_h;

extern padPt PLATOSize;
extern padPt TTYLoc;

// The static symbol for the c64 swlink driver
extern char c64_swlink;

extern void ShowPLATO(padByte *buff, uint16_t count);

/**
 * log(const char* format, ...)
 * put some data out to the printer for logging
 */
/* void log(const char* format, ...) */
/* { */
/* #ifdef PROTOCOL_DEBUG */
/*   char lbuf[128]; */
/*   va_list args; */
/*   cbm_open(1,4,CBM_WRITE,""); */
/*   va_start(args,format); */
/*   vsprintf(lbuf,format,args); */
/*   va_end(args); */
/*   cbm_write(1,lbuf,strlen(lbuf)); */
/*   cbm_close(1); */
/* #endif */
/* } */

/**
 * send_byte(b) - Send specified byte out
 */
void send_byte(uint8_t b)
{
  ser_put(b);
}

/**
 * greeting(void) - Show terminal greeting
 */
void greeting(void)
{
  padPt coord;

  coord.x=168; coord.y=480; screen_char_draw(&coord,welcomemsg_1,WELCOMEMSG_1_LEN);
  coord.x=144; coord.y=464; screen_char_draw(&coord,welcomemsg_2,WELCOMEMSG_2_LEN);
  coord.x=104; coord.y=432; screen_char_draw(&coord,welcomemsg_3,WELCOMEMSG_3_LEN);
  coord.x=160; coord.y=416; screen_char_draw(&coord,welcomemsg_4,WELCOMEMSG_4_LEN);
  coord.x=16;  coord.y=384; screen_char_draw(&coord,welcomemsg_5,WELCOMEMSG_5_LEN);
}

void main(void)
{
  struct ser_params params = {
    SER_BAUD_19200,
    SER_BITS_8,
    SER_STOP_1,
    SER_PAR_NONE,
    SER_HS_HW
  };
  
  modemc=ser_install(&c64_swlink);

  if (modemc!=SER_ERR_OK)
    {
      printf("ser_install returned: %d\n",modemc);
      return;
    }

  modemc=ser_open(&params);
  ser_ioctl(1, NULL);
  screen_init();
  touch_init();
  greeting();
  
  // And do the terminal
  for (;;)
    {
      if (ser_get(&modemc)==SER_ERR_OK)
	{
	  // Detect and strip IAC escapes (two consecutive bytes of 0xFF)
	  if (modemc==0xFF && lastmodemc == 0xFF)
	    {
	      lastmodemc=0x00;
	    }
	  else
	    {
	      lastmodemc=modemc;
	      ShowPLATO(&modemc,1);
	    }
	}
      if (TTY)
	{
	  touch_hide();
	  if (kbhit())
	    {
	      send_byte(cgetc());
	    }
	}
      else
	{
	  keyboard_main();
	  touch_main();
	}
    }
  screen_done();
  touch_done();
  ser_close();
  ser_uninstall();
}
