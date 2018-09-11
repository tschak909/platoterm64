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
#include "../config.h"
#include "../protocol.h"

extern uint8_t pal[2];
extern ConfigInfo config; 
extern short FONT_SIZE_X;
extern short FONT_SIZE_Y;

static short offset;

// referenced by fast_text routines.
unsigned short cx;
unsigned char cy;
unsigned char CharCode;
unsigned char Flags;
unsigned char* GlyphData;

extern unsigned char font[];
extern unsigned char fontm23[];
extern uint16_t mul0625(uint16_t val);
extern uint16_t mul0375(uint16_t val);

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
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  // My feeble attempt at a beep.
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
  
  if (ModeBold)
    {
      dx<<=1;
      Flags|=0x40;
    }
  cx=mul0625((Coord->x&0x1FF));
  cy=mul0375((Coord->y+14^0x1FF)&0x1FF);

  for (i=0;i<count;++i)
    {
      CharCode=ch[i]+offset;
      RenderGlyph();
      cx+=dx;
    }
  
}
