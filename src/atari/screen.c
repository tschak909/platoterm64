/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <peekpoke.h>
#include <tgi.h>
#include <stdint.h>
#include <conio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../config.h"
#include "../protocol.h"
#include "../screen.h"

extern uint8_t pal[2];
extern ConfigInfo config; 
extern short FONT_SIZE_X;
extern short FONT_SIZE_Y;
extern uint8_t CharWide;
extern uint8_t CharHigh;

/* screen_beep.s */
extern void screen_beep(void);

static short offset;

// referenced by fast_text routines.
unsigned short cx;
unsigned char cy;
unsigned char CharCode;
unsigned char Flags;
unsigned char* GlyphData;

extern unsigned char recv_buffer[384];
extern unsigned char font[];
extern unsigned char fontm23[];
extern uint16_t mul0625(uint16_t val);
extern uint16_t mul0375(uint16_t val);
extern void (*io_recv_serial_flow_on)(void);
extern void (*io_recv_serial_flow_off)(void);
extern padPt TTYLoc;

// size of scaled PLATO screen
padPt actualSize = {320, 192};

extern void RenderGlyph(void);

/**
 * screen_init_hook()
 * Called after tgi_init to set any special features, e.g. nmi trampolines.
 */
void screen_init_hook(void)
{
  POKE(0x22F,62); // Turn on P/M DMA for mouse cursor.
  bordercolor(TGI_COLOR_BLUE);
}

/**
 * screen_load_driver()
 * Load the TGI driver
 */
void screen_load_driver(void)
{
  tgi_install(tgi_static_stddrv);
}

/**
 * screen_splash - Show splash screen
 */
void screen_splash(void)
{
  int fd=open("D:SPLASH.BIN",O_RDONLY);
  short len=0;

  while (len=read(fd,recv_buffer,sizeof(recv_buffer)))
    ShowPLATO((padByte *)recv_buffer,len);

  close(fd);
}

/**
 * screen_set_pen_mode()
 * Set the pen mode based on CurMode.
 */
void screen_set_pen_mode(void)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(0);
  else
    tgi_setcolor(1);
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  // Block erase takes forever, manually assert flow control.
  io_recv_serial_flow_off(); 
  
  screen_set_pen_mode();
  tgi_bar(mul0625(Coord1->x),mul0375(Coord1->y^0x1FF),mul0625(Coord2->x),mul0375(Coord2->y^0x1FF));

  io_recv_serial_flow_on();  
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  screen_set_pen_mode();
  tgi_setpixel(mul0625(Coord->x),mul0375(Coord->y^0x1FF));
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  screen_set_pen_mode();
  tgi_line(mul0625(Coord1->x),mul0375(Coord1->y^0x1FF),mul0625(Coord2->x),mul0375(Coord2->y^0x1FF));
}

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    screen_char_draw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
  }
  else if ((theChar == 0x0b)) /* Vertical Tab */
    {
      TTYLoc.y += CharHigh;
    }
  else if ((theChar == 0x08) && (TTYLoc.x > 7))	/* backspace */
    {
      TTYLoc.x -= CharWide;

      tgi_setcolor(0);
      tgi_bar(mul0625(TTYLoc.x),mul0375(TTYLoc.y^0x1FF),mul0625(TTYLoc.x+CharWide),mul0375((TTYLoc.y+CharHigh)^0x1FF));
      tgi_setcolor(1);
    }
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
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void)
{
  ++config.color_foreground;
}

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void)
{
  ++config.color_background;
}

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 */
void screen_cycle_border(void)
{
  ++config.color_border;
}

/**
 * screen_cycle_foreground_back()
 * Go to the previous foreground color in palette
 */
void screen_cycle_foreground_back(void)
{
  --config.color_foreground;
}

/**
 * screen_cycle_background_back()
 * Go to the previous background color in palette
 */
void screen_cycle_background_back(void)
{
  --config.color_background;
}

/**
 * screen_cycle_border_back()
 * Go to the previous border color in palette
 */
void screen_cycle_border_back(void)
{
  --config.color_border;
}

/**
 * Set the terminal colors
 */
void screen_update_colors(void)
{
  pal[0]=config.color_background;
  pal[1]=config.color_foreground;
  tgi_setpalette(pal);
  POKE(712,config.color_border);
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  // TODO: function to do one vblank wait
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  unsigned char i;
  unsigned char dx=5;
  Flags=0;
  
  switch(CurMem)
    {
    case M0:
      GlyphData=font;
      offset=-32;
      break;
    case M1:
      GlyphData=font;
      offset=64;
      break;
    case M2:
      GlyphData=fontm23;
      offset=-32;
      break;
    case M3:
      GlyphData=fontm23;
      offset=32;      
      break;
    }

  if (CurMode==ModeInverse)
    Flags|=0x80;
  else if (CurMode==ModeWrite)
    Flags|=0x20;
  else if (CurMode==ModeErase)
    Flags|=0x10;
  
  if (ModeBold)
    {
      dx<<=1;
      Flags|=0x40;
    }
  cx=mul0625((Coord->x&0x1FF));

  if (ModeBold)
  cy=mul0375((Coord->y+30^0x1FF)&0x1FF);
  else
  cy=mul0375((Coord->y+15^0x1FF)&0x1FF);

  for (i=0;i<count;++i)
    {
      CharCode=ch[i]+offset;
      RenderGlyph();
      cx+=dx;
    }
  
}
