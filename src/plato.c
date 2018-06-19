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
#include <mouse.h>
#include "protocol.h"
#include "key.h"
#include "terminal.h"
#include "welcome.h"
#include "screen.h"
#include "touch.h"

static uint8_t modemc=0;
static uint8_t lastmodemc=0;

static uint8_t lastkey;

extern uint16_t screen_w;
extern uint16_t screen_h;

extern padPt PLATOSize;
extern padPt TTYLoc;

extern unsigned short scalex[];
extern unsigned short scaley[];
extern unsigned short scaletx[];
extern unsigned short scalety[];

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

  coord.x=168; coord.y=480; CharDraw(&coord,welcomemsg_1,WELCOMEMSG_1_LEN);
  coord.x=144; coord.y=464; CharDraw(&coord,welcomemsg_2,WELCOMEMSG_2_LEN);
  coord.x=104; coord.y=432; CharDraw(&coord,welcomemsg_3,WELCOMEMSG_3_LEN);
  coord.x=160; coord.y=416; CharDraw(&coord,welcomemsg_4,WELCOMEMSG_4_LEN);
  coord.x=16;  coord.y=384; CharDraw(&coord,welcomemsg_5,WELCOMEMSG_5_LEN);
}

/**
 * handle_keyboard - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void handle_key(uint8_t platoKey)
{
  if (platoKey==0xff)
    return;
  
  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;
}

/**
 * handle_keyboard - Handle the keyboard presses
 */
void handle_keyboard(void)
{
  uint8_t key=PEEK(0xCB);
  uint8_t modifier=PEEK(0x28D);

  // Handle Function keys
  if (key==0x04 && lastkey!=0x04)
    {
      // Change colors
      if (modifier==0x00)
	{
	  screen_cycle_background();
	}
      else if (modifier==0x01)
	{
	  screen_cycle_foreground();
	}
      else if (modifier==0x02)
	{
	  screen_cycle_border();
	}
      set_terminal_colors();
    }
  
  if (key!=lastkey)
    {  
      if (modifier==MODIFIER_NONE)
	handle_key(KEYBOARD_TO_PLATO[key]);
      else if (modifier==MODIFIER_SHIFT)
	handle_key(KEYBOARD_TO_PLATO_SHIFT[key]);
      else if (modifier==MODIFIER_COMMO)
	handle_key(KEYBOARD_TO_PLATO_COMMO[key]);
      else if (modifier==MODIFIER_COMMO_SHIFT)
	handle_key(KEYBOARD_TO_PLATO_CS[key]);
      else if (modifier==MODIFIER_CTRL)
	handle_key(KEYBOARD_TO_PLATO_CTRL[key]);
      else if (modifier==MODIFIER_CTRL_SHIFT)
	handle_key(KEYBOARD_TO_PLATO_CTRL_SHIFT[key]);
    }
      lastkey=key;
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
	  mouse_move(screen_w,screen_h);
	  if (kbhit())
	    {
	      send_byte(cgetc());
	    }
	}
      else
	{
	  handle_keyboard();
	  touch_main();
	}
    }
  tgi_done();
  ser_close();
  ser_uninstall();
  tgi_uninstall();
  mouse_uninstall();
}
