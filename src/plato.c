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

static uint8_t color_background=TGI_COLOR_BLUE;
static uint8_t color_foreground=TGI_COLOR_LIGHTBLUE;
static uint8_t color_border=TGI_COLOR_LIGHTBLUE;
static uint8_t pal[2];
static uint8_t modemc=0;
static uint8_t lastmodemc=0;

static uint8_t lastkey;

static padBool TouchActive;

static struct mouse_info mouse_data;
static uint16_t previous_mouse_x;
static uint16_t previous_mouse_y;

static uint16_t screen_w;
static uint16_t screen_h;

extern padPt PLATOSize;
extern padPt TTYLoc;

extern unsigned short scalex[];
extern unsigned short scaley[];
extern unsigned short scaletx[];
extern unsigned short scalety[];

// The static symbol for the c64 swlink driver
extern char c64_swlink;

extern void install_nmi_trampoline(void);
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
 * TouchAllow - Set whether touchpanel is active or not.
 */
void TouchAllow(padBool allow)
{
  // If mouse is off screen (due to previously being moved off screen, move onscreen to make visible.
  if (allow)
    {
      mouse_move(previous_mouse_x,previous_mouse_y);
    }
  else
    {
      previous_mouse_x = mouse_data.pos.x;
      previous_mouse_y = mouse_data.pos.y;
    }
  TouchActive=allow;
}


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
 * Set the terminal colors
 */
void set_terminal_colors(void)
{
  pal[0]=color_background;
  pal[1]=color_foreground;
  tgi_setpalette(pal);
  POKE(0xD020,color_border);
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
	  ++color_background;
	  color_background&=0x0f;
	}
      else if (modifier==0x01)
	{
	  ++color_foreground;
	  color_background&=0x0f;
	}
      else if (modifier==0x02)
	{
	  ++color_border;
	  color_border&=0x0f;
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

/**
 * handle_mouse - Process mouse events and turn into scaled touch events
 */
void handle_mouse(void)
{
  uint8_t lastbuttons;
  padPt coord;
  
  mouse_info(&mouse_data);

  /* If touch screen isn't active, don't let the mouse be used. */
  if (TouchActive==0)
    {
      previous_mouse_x = mouse_data.pos.x;
      previous_mouse_y = mouse_data.pos.y;
      mouse_move(screen_w,screen_h);
      return;
    }
  
  if (mouse_data.buttons == lastbuttons)
    return; /* debounce */
  else if ((mouse_data.buttons & MOUSE_BTN_LEFT))
    {
      coord.x = scaletx[mouse_data.pos.x];
      coord.y = scalety[mouse_data.pos.y];
      Touch(&coord);
    }
  lastbuttons = mouse_data.buttons;
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

  POKE(0xD020,0);
  mouse_install(&mouse_def_callbacks,&mouse_static_stddrv);
  tgi_install(tgi_static_stddrv);
  tgi_init();
  install_nmi_trampoline();
  set_terminal_colors();
  tgi_setpalette(pal);
  modemc=ser_open(&params);
  ser_ioctl(1, NULL);  
  screen_w = tgi_getmaxx();
  screen_h = tgi_getmaxy();
  mouse_move(screen_w,screen_h);
  mouse_show();
  SetTTY();
  TTYLoc.y=368; // Right under the greeting.
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
	  if (TouchActive)
	    {
	      handle_mouse();
	    }
	  else
	    {
	      mouse_move(screen_w,screen_h);
	    }
	}
    }
  tgi_done();
  ser_close();
  ser_uninstall();
  tgi_uninstall();
  mouse_uninstall();
}
