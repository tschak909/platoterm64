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
#include "font.h"
#include "scale.h"
#include "protocol.h"
#include "key.h"

static uint8_t color_background=TGI_COLOR_BLUE;
static uint8_t color_foreground=TGI_COLOR_LIGHTBLUE;
static uint8_t color_border=TGI_COLOR_LIGHTBLUE;
static uint8_t pal[2];
static uint8_t modemc=0;
static uint8_t lastmodemc=0;

static uint8_t lastkey;

static padPt TTYLoc;
static uint8_t CharWide=8;
static uint8_t CharHigh=16; 
static padBool TouchActive;

static struct mouse_info mouse_data;
static uint16_t previous_mouse_x;
static uint16_t previous_mouse_y;

static uint16_t screen_w;
static uint16_t screen_h;

extern padPt PLATOSize;
extern padBool FastText;

// PLATOTerm for Commodore 64
padByte welcomemsg_1[]={80,76,65,84,79,84,101,114,109,32,102,111,114,32,67,111,109,109,111,100,111,114,101,32,54,52};
#define WELCOMEMSG_1_LEN 26

// Copyright 2018 IRATA.ONLINE
padByte welcomemsg_2[]={67,111,112,121,114,105,103,104,116,32,40,99,41,32,0x32,0x30,0x31,0x38,0x20,73,82,65,84,65,46,79,78,76,73,78,69};
#define WELCOMEMSG_2_LEN 31

// This software is licensed under GPL 3.0.
padByte welcomemsg_3[]={84,104,105,115,32,115,111,102,116,119,97,114,101,32,105,115,32,108,105,99,101,110,115,101,100,32,117,110,100,101,114,32,71,80,76,32,51,46,48,32,46};
#define WELCOMEMSG_3_LEN 40

// See COPYING for details.
padByte welcomemsg_4[]={83,101,101,32,99,111,112,121,105,110,103,32,102,111,114,32,100,101,116,97,105,108,115};
#define WELCOMEMSG_4_LEN 23

// PLATOTerm READY
padByte welcomemsg_5[]={80,76,65,84,79,84,101,114,109,32,82,69,65,68,89};
#define WELCOMEMSG_5_LEN 15

// The static symbol for the c64 swlink driver
extern char c64_swlink;

extern void install_nmi_trampoline(void);
extern void ShowPLATO(padByte *buff, uint16_t count);

/**
 * log(const char* format, ...)
 * put some data out to the printer for logging
 */
void log(const char* format, ...)
{
#ifdef PROTOCOL_DEBUG
  char lbuf[128];
  va_list args;
  cbm_open(1,4,CBM_WRITE,"");
  va_start(args,format);
  vsprintf(lbuf,format,args);
  va_end(args);
  cbm_write(1,lbuf,strlen(lbuf));
  cbm_close(1);
#endif
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void Wait(void)
{
  waitvsync();
}


/**
 * Beep(void) - Beep the terminal
 */
void Beep(void)
{
  /* TODO: Implement beep(); */
}

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
 * ClearScreen - Clear the screen
 */
void ClearScreen(void)
{
  tgi_clear();
}

/**
 * send_byte(b) - Send specified byte out
 */
void send_byte(uint8_t b)
{
  ser_put(b);
}

/**
 * BlockDraw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void BlockDraw(padPt* Coord1, padPt* Coord2)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);
  
  tgi_bar(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * dotDraw(Coord) - Plot a mode 0 pixel
 */
void DotDraw(padPt* Coord)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);
  
  tgi_setpixel(scalex[Coord->x],scaley[Coord->y]);
}

/**
 * lineDraw(Coord1, Coord2) - Draw a mode 1 line
 */
void LineDraw(padPt* Coord1, padPt* Coord2)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);
  
  tgi_line(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * CharDraw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void CharDraw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  int16_t offset; /* due to negative offsets */
  uint16_t x;      /* Current X and Y coordinates */
  uint16_t y;
  uint16_t* px;   /* Pointers to X and Y coordinates used for actual plotting */
  uint16_t* py;
  uint8_t i; /* current character counter */
  uint8_t j; /* vertical loop counter */
  uint8_t k; /* horizontal loop counter */
  uint8_t a; /* current character byte */
  int8_t b; /* current character row bit signed */
  uint8_t width=CharWide;
  uint8_t height=CharHigh;
  uint16_t deltaX=1;
  uint16_t deltaY=1;
  uint8_t mainColor=TGI_COLOR_WHITE;
  uint8_t altColor;
  uint8_t *p;
  
  switch(CurMem)
    {
    case M0:
      offset=-32;
      break;
    case M1:
      offset=64;
      break;
    case M2:
      /* TODO: custom charsets */
      break;
    case M3:
      /* TODO: custom charsets */
      break;
    }

  if (CurMode==ModeRewrite)
    {
      altColor=TGI_COLOR_BLACK;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=TGI_COLOR_WHITE;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=TGI_COLOR_BLACK;
  else
    mainColor=TGI_COLOR_WHITE;
  
  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

 diet_chardraw:
  for (i=0;i<count;++i)
    {
      y=scaley[(Coord->y)+14&0x1FF];
      a=*ch;
      ++ch;
      a+=offset;
      p=&font[fontptr[a]];
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;
  	  x=scalex[(Coord->x&0x1FF)];

  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		{
		  tgi_setcolor(mainColor);
		  tgi_setpixel(x,y);
		}

	      ++x;
  	      b<<=1;
  	    }

	  ++y;
	  ++p;
  	}

      Coord->x+=width;
    }

  return;
  
 chardraw_with_fries:
  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
    }
  
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
    }

  for (i=0;i<count;++i)
    {
      y=scaley[(Coord->y)+14&0x1FF];
      a=*ch;
      ++ch;
      a=a+offset;
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=font[fontptr[a]+j];
  	  x=scalex[(Coord->x&0x1FF)];

	  if (Rotate)
	    {
	      px=&y;
	      py=&x;
	    }
	  else
	    {
	      px=&x;
	      py=&y;
	    }

  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		{
		  tgi_setcolor(mainColor);
		  if (ModeBold)
		    {
		      tgi_setpixel(*px+1,*py);
		      tgi_setpixel(*px,*py+1);
		      tgi_setpixel(*px+1,*py+1);
		    }
		  tgi_setpixel(*px,*py);
		}
	      else
		{
		  if (CurMode==ModeInverse || CurMode==ModeRewrite)
		    {
		      tgi_setcolor(altColor);
		      if (ModeBold)
			{
			  tgi_setpixel(*px+1,*py);
			  tgi_setpixel(*px,*py+1);
			  tgi_setpixel(*px+1,*py+1);
			}
		      tgi_setpixel(*px,*py); 
		    }
		}

	      x += deltaX;
  	      b<<=1;
  	    }

	  y += deltaY;
  	}

      Coord->x+=width;
    }

}

/**
 * TTYChar - Called to plot chars when in tty mode
 */
void TTYChar(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    CharDraw(&TTYLoc, &theChar, 1);
    /* TTYLoc.x += CharWide; */
  }
  else if ((theChar == 0x08) && (TTYLoc.x > 7))	/* backspace */
    TTYLoc.x -= CharWide;
  else if (theChar == 0x0A)			/* line feed */
    TTYLoc.y -= CharHigh;
  else if (theChar == 0x0D)			/* carriage return */
    TTYLoc.x = 0;
  
  if (TTYLoc.x + CharWide > 511) {	/* wrap at right side */
    TTYLoc.x = 0;
    TTYLoc.y -= CharHigh;
  }
  
  if (TTYLoc.y < 0) {
    tgi_clear();
    TTYLoc.y=495;
  }

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
