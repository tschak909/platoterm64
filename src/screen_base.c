/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen_base.c - Display output functions (base)
 */

#include <string.h>
#include <stdbool.h>
#include <tgi.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "screen.h"
#include "protocol.h"
#include "config.h"
#include "io.h"

uint8_t CharWide=8;
uint8_t CharHigh=16;
padPt TTYLoc;
uint8_t pal[2];
unsigned char current_foreground=COLOR_WHITE;
unsigned char current_background=COLOR_BLACK;

extern uint8_t xoff_enabled; /* io.c */
extern padBool FastText; /* protocol.c */
extern ConfigInfo config; /* config.c */

#ifndef __ATARI__
extern unsigned short scalex[];
extern unsigned short scaley[];
#endif

extern uint8_t font[];
extern uint8_t fontm23[];
extern uint8_t FONT_SIZE_X;
extern uint8_t FONT_SIZE_Y;

extern void (*io_recv_serial_flow_on)(void);
extern void (*io_recv_serial_flow_off)(void);

// Atari uses fast frac multiplies to save memory
#ifdef __ATARI__
extern uint16_t mul0625(uint16_t val);
extern uint16_t mul0375(uint16_t val);
#endif

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  screen_load_driver();
  tgi_init();
  screen_init_hook();
  tgi_clear();
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
  tgi_clear();
  screen_update_colors();
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  // Block erase takes forever, manually assert flow control.
  io_recv_serial_flow_off(); 
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
#ifdef __ATARI__
    tgi_setcolor(0);
#else
    tgi_setcolor(TGI_COLOR_BLACK);
#endif
    else
#ifdef __ATARI__
      tgi_setcolor(1);
#else
    tgi_setcolor(TGI_COLOR_WHITE);
#endif
    
#ifdef __ATARI__
  tgi_bar(mul0625(Coord1->x),mul0375(Coord1->y^0x1FF),mul0625(Coord2->x),mul0375(Coord2->y^0x1FF));
#else
  tgi_bar(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
#endif
  io_recv_serial_flow_on();
  
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
#ifdef __ATARI__
    tgi_setcolor(0);
#else
  tgi_setcolor(TGI_COLOR_BLACK);
#endif
    else
#ifdef __ATARI__
      tgi_setcolor(1);
#else
  tgi_setcolor(TGI_COLOR_WHITE);
#endif
  
#ifdef __ATARI__
  tgi_setpixel(mul0625(Coord->x),mul0375(Coord->y^0x1FF));
#else
  tgi_setpixel(scalex[Coord->x],scaley[Coord->y]);
#endif
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
#ifdef __ATARI__
  uint16_t x1=mul0625(Coord1->x);
  uint16_t x2=mul0625(Coord2->x);
  uint16_t y1=mul0375(Coord1->y^0x1FF);
  uint16_t y2=mul0375(Coord2->y^0x1FF);  
#else
  uint16_t x1=scalex[Coord1->x];
  uint16_t x2=scalex[Coord2->x];
  uint16_t y1=scaley[Coord1->y];
  uint16_t y2=scaley[Coord2->y];
#endif

  if (CurMode==ModeErase || CurMode==ModeInverse)
#ifdef __ATARI__
    tgi_setcolor(0);
#else
    tgi_setcolor(TGI_COLOR_BLACK);
#endif
    else
#ifdef __ATARI__
      tgi_setcolor(1);
#else
  tgi_setcolor(TGI_COLOR_WHITE);
#endif

  tgi_line(x1,y1,x2,y2);
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

#ifdef __ATARI__
      tgi_setcolor(0);
#else
      tgi_setcolor(TGI_COLOR_BLACK);
#endif
      
#ifdef __ATARI__
      tgi_bar(mul0625(TTYLoc.x),mul0375(TTYLoc.y^0x1FF),mul0625(TTYLoc.x+CharWide),mul0375((TTYLoc.y+CharHigh)^0x1FF));
#else
      tgi_bar(scalex[TTYLoc.x],scaley[TTYLoc.y],scalex[TTYLoc.x+CharWide],scaley[TTYLoc.y+CharHigh]);
#endif

#ifdef __ATARI__
      tgi_setcolor(1);
#else
      tgi_setcolor(TGI_COLOR_WHITE);
#endif
      
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
 * match_color - quantize to nearest system palette color.
 * will eventually do a full hsv match to nearest possible color,
 * but for now, it's a straight match of system colors only.
 */
unsigned char screen_match_color(padRGB* theColor)
{
#ifdef __C64__
  if (theColor->red==0 && theColor->green==0 && theColor->blue==0)
    {
      return COLOR_BLACK;
    }
  else if (theColor->red==0 && theColor->green==0 && theColor->blue==255)
    {
      return COLOR_BLUE;
    }
  else if (theColor->red==0 && theColor->green==255 && theColor->blue==0)
    {
      return COLOR_GREEN;
    }
  else if (theColor->red==255 && theColor->green==0 && theColor->blue==0)
    {
      return COLOR_RED;
    }
  else if (theColor->red==0 && theColor->green==255 && theColor->blue==255)
    {
      return COLOR_CYAN;
    }
  else if (theColor->red==255 && theColor->green==0 && theColor->blue==255)
    {
      return COLOR_LIGHTRED;
    }
  else if (theColor->red==255 && theColor->green==255 && theColor->blue==0)
    {
      return COLOR_YELLOW;
    }
#endif
  // For any other color, return white.
  return COLOR_WHITE;
}

/**
 * screen_foreground - set foreground color
 */
void screen_foreground(padRGB* theColor)
{
  current_foreground=screen_match_color(theColor);
}

/**
 * screen_background - set background_color
 */
void screen_background(padRGB* theColor)
{
  current_background=screen_match_color(theColor);
}

/**
 * screen_paint - Paint screen scanline_fill
 */
void _screen_paint(unsigned short x, unsigned short y)
{
/* #ifndef __APPLE2__ */
/*   // Currently we run out of memory on apple2, so this is temporarily disabled. */
/*   static unsigned short xStack[64]; */
/*   static unsigned char yStack[64]; */
/*   unsigned char stackentry = 1; */
/*   unsigned char spanAbove, spanBelow; */
  
/*   unsigned char oldColor = tgi_getpixel(x,y); */
  
/*   if (oldColor == 1) */
/*     return; */
  
/*   do */
/*     { */
/*       while (x > 0 && tgi_getpixel(x-1,y) == oldColor) */
/* 	--x; */
      
/*       spanAbove = spanBelow = false; */
/*       while(tgi_getpixel(x,y) == oldColor) */
/* 	{ */
/* 	  tgi_setpixel(x,y); */
	  
/* 	  if (y < (191)) */
/* 	    { */
/* 	      unsigned char belowColor = tgi_getpixel(x, y+1); */
/* 	      if (!spanBelow  && belowColor == oldColor) */
/* 		{ */
/* 		  xStack[stackentry]  = x; */
/* 		  yStack[stackentry]  = y+1;					 */
/* 		  ++stackentry; */
/* 		  spanBelow = true; */
/* 		} */
/* 	      else if (spanBelow && belowColor != oldColor) */
/* 		spanBelow = false; */
/* 	    } */
	  
/* 	  if (y > 0) */
/* 	    { */
/* 	      unsigned char aboveColor = tgi_getpixel(x, y-1); */
/* 	      if (!spanAbove  && aboveColor == oldColor) */
/* 		{ */
/* 		  xStack[stackentry]  = x; */
/* 		  yStack[stackentry]  = y-1; */
/* 		  ++stackentry; */
/* 		  spanAbove = true; */
/* 		} */
/* 	      else if (spanAbove && aboveColor != oldColor) */
/* 		spanAbove = false; */
/* 	    } */
	  
/* 	  ++x; */
/* 	} */
/*       --stackentry; */
/*       x = xStack[stackentry]; */
/*       y = yStack[stackentry]; */
/*     } */
/*   while (stackentry); */
/* #endif */
}

/**
 * screen_paint - Paint the screen
 * Calling convention is different due to this needing to be recursive.
 */
void screen_paint(padPt* Coord)
{
/*   if (config.fill==true) */
/*     { */
/* #ifdef __ATARI__ */
/*       _screen_paint(mul0625(Coord->x),mul0375(Coord->y^0x1FF)); */
/* #else */
/*       _screen_paint(scalex[Coord->x],scaley[Coord->y]); */
/* #endif */
/*     } */
}

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
  tgi_done();
  tgi_uninstall();
}
