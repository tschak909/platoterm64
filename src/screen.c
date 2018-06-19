/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <c64.h>
#include <tgi.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "screen.h"
#include "protocol.h"
#include "font.h"
#include "scale.h"

uint8_t CharWide=8;
uint8_t CharHigh=16;
padPt TTYLoc;
uint8_t pal[2];

static uint8_t color_background=TGI_COLOR_BLUE;
static uint8_t color_foreground=TGI_COLOR_LIGHTBLUE;
static uint8_t color_border=TGI_COLOR_LIGHTBLUE;

extern padBool FastText;
extern void install_nmi_trampoline(void);

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  tgi_install(tgi_static_stddrv);
  tgi_init();
  install_nmi_trampoline();
  set_terminal_colors();
  tgi_setpalette(pal);
}

/**
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void)
{
  ++color_foreground;
  color_foreground&=0x0f;
}

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void)
{
  ++color_background;
  color_background&=0x0f;
}

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 */
void screen_cycle_border(void)
{
  ++color_border;
  color_border&=0x0f;
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
 * ClearScreen - Clear the screen
 */
void ClearScreen(void)
{
  tgi_clear();
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

  /* the diet chardraw routine - fast text output. */
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
