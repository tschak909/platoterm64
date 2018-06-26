/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <stdbool.h>
#include <c64.h>
#include <tgi.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "screen.h"
#include "protocol.h"
#include "font.h"
#include "config.h"
#include "io.h"

uint8_t CharWide=8;
uint8_t CharHigh=16;
padPt TTYLoc;
uint8_t pal[2];

extern uint8_t xoff_enabled; /* io.c */
extern padBool FastText; /* protocol.c */
extern void install_nmi_trampoline(void); /* nmi_trampoline.s */
extern ConfigInfo config; /* config.c */

/* X and Y tables used to scale 512x512 PLATO display to 320x192 */
unsigned short scalex[]={
0,1,2,2,3,4,4,5,
5,6,7,7,8,9,9,10,
10,11,12,12,13,14,14,15,
15,16,17,17,18,19,19,20,
20,21,22,22,23,24,24,25,
25,26,27,27,28,29,29,30,
30,31,32,32,33,34,34,35,
35,36,37,37,38,39,39,40,
40,41,42,42,43,44,44,45,
45,46,47,47,48,49,49,50,
50,51,52,52,53,54,54,55,
55,56,57,57,58,59,59,60,
60,61,62,62,63,64,64,65,
65,66,67,67,68,69,69,70,
70,71,72,72,73,74,74,75,
75,76,77,77,78,79,79,80,
80,81,82,82,83,84,84,85,
85,86,87,87,88,89,89,90,
90,91,92,92,93,94,94,95,
95,96,97,97,98,99,99,100,
100,101,102,102,103,104,104,105,
105,106,107,107,108,109,109,110,
110,111,112,112,113,114,114,115,
115,116,117,117,118,119,119,120,
120,121,122,122,123,124,124,125,
125,126,127,127,128,129,129,130,
130,131,132,132,133,134,134,135,
135,136,137,137,138,139,139,140,
140,141,142,142,143,144,144,145,
145,146,147,147,148,149,149,150,
150,151,152,152,153,154,154,155,
155,156,157,157,158,159,159,160,
160,161,162,162,163,164,164,165,
165,166,167,167,168,169,169,170,
170,171,172,172,173,174,174,175,
175,176,177,177,178,179,179,180,
180,181,182,182,183,184,184,185,
185,186,187,187,188,189,189,190,
190,191,192,192,193,194,194,195,
195,196,197,197,198,199,199,200,
200,201,202,202,203,204,204,205,
205,206,207,207,208,209,209,210,
210,211,212,212,213,214,214,215,
215,216,217,217,218,219,219,220,
220,221,222,222,223,224,224,225,
225,226,227,227,228,229,229,230,
230,231,232,232,233,234,234,235,
235,236,237,237,238,239,239,240,
240,241,242,242,243,244,244,245,
245,246,247,247,248,249,249,250,
250,251,252,252,253,254,254,255,
255,256,257,257,258,259,259,260,
260,261,262,262,263,264,264,265,
265,266,267,267,268,269,269,270,
270,271,272,272,273,274,274,275,
275,276,277,277,278,279,279,280,
280,281,282,282,283,284,284,285,
285,286,287,287,288,289,289,290,
290,291,292,292,293,294,294,295,
295,296,297,297,298,299,299,300,
300,301,302,302,303,304,304,305,
305,306,307,307,308,309,309,310,
310,311,312,312,313,314,314,315,
315,316,317,317,318,319,319,320,
};

unsigned short scaley[]={
192,192,191,191,191,190,190,189,
189,189,188,188,188,187,187,186,
186,186,185,185,185,184,184,183,
183,183,182,182,182,181,181,180,
180,180,179,179,179,178,178,177,
177,177,176,176,176,175,175,174,
174,174,173,173,173,172,172,171,
171,171,170,170,170,169,169,168,
168,168,167,167,167,166,166,165,
165,165,164,164,164,163,163,162,
162,162,161,161,161,160,160,159,
159,159,158,158,158,157,157,156,
156,156,155,155,155,154,154,153,
153,153,152,152,152,151,151,150,
150,150,149,149,149,148,148,147,
147,147,146,146,146,145,145,144,
144,144,143,143,143,142,142,141,
141,141,140,140,140,139,139,138,
138,138,137,137,137,136,136,135,
135,135,134,134,134,133,133,132,
132,132,131,131,131,130,130,129,
129,129,128,128,128,127,127,126,
126,126,125,125,125,124,124,123,
123,123,122,122,122,121,121,120,
120,120,119,119,119,118,118,117,
117,117,116,116,116,115,115,114,
114,114,113,113,113,112,112,111,
111,111,110,110,110,109,109,108,
108,108,107,107,107,106,106,105,
105,105,104,104,104,103,103,102,
102,102,101,101,101,100,100,99,
99,99,98,98,98,97,97,96,
96,96,95,95,95,94,94,93,
93,93,92,92,92,91,91,90,
90,90,89,89,89,88,88,87,
87,87,86,86,86,85,85,84,
84,84,83,83,83,82,82,81,
81,81,80,80,80,79,79,78,
78,78,77,77,77,76,76,75,
75,75,74,74,74,73,73,72,
72,72,71,71,71,70,70,69,
69,69,68,68,68,67,67,66,
66,66,65,65,65,64,64,63,
63,63,62,62,62,61,61,60,
60,60,59,59,59,58,58,57,
57,57,56,56,56,55,55,54,
54,54,53,53,53,52,52,51,
51,51,50,50,50,49,49,48,
48,48,47,47,47,46,46,45,
45,45,44,44,44,43,43,42,
42,42,41,41,41,40,40,39,
39,39,38,38,38,37,37,36,
36,36,35,35,35,34,34,33,
33,33,32,32,32,31,31,30,
30,30,29,29,29,28,28,27,
27,27,26,26,26,25,25,24,
24,24,23,23,23,22,22,21,
21,21,20,20,20,19,19,18,
18,18,17,17,17,16,16,15,
15,15,14,14,14,13,13,12,
12,12,11,11,11,10,10,9,
9,9,8,8,8,7,7,6,
6,6,5,5,5,4,4,3,
3,3,2,2,2,1,1,0,
};

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  tgi_install(tgi_static_stddrv);
  tgi_init();
  install_nmi_trampoline();
  config.color_foreground=TGI_COLOR_LIGHTBLUE;
  config.color_background=TGI_COLOR_BLUE;
  config.color_border=TGI_COLOR_LIGHTBLUE;
  screen_update_colors();
  tgi_setpalette(pal);
  tgi_clear();
}

/**
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void)
{
  ++config.color_foreground;
  config.color_foreground&=0x0f;
}

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void)
{
  ++config.color_background;
  config.color_background&=0x0f;
}

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 */
void screen_cycle_border(void)
{
  ++config.color_border;
  config.color_border&=0x0f;
}

/**
 * Set the terminal colors
 */
void screen_update_colors(void)
{
  pal[0]=config.color_background;
  pal[1]=config.color_foreground;
  tgi_setpalette(pal);
  POKE(0xD020,config.color_border);
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  waitvsync();
}


/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  /* TODO: Implement beep(); */
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
  tgi_clear();
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  // Block erase takes forever, manually assert flow control.

  io_send_byte(0x13);
  xoff_enabled=true;
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);
  
  tgi_bar(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);

  io_send_byte(0x11);
  xoff_enabled=false;
  
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);
  
  tgi_setpixel(scalex[Coord->x],scaley[Coord->y]);
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  uint16_t x1=scalex[Coord1->x];
  uint16_t x2=scalex[Coord2->x];
  uint16_t y1=scaley[Coord1->y];
  uint16_t y2=scaley[Coord2->y];
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);

  /* if ((x1==x2) && (y1==y2)) */
  /*   tgi_setpixel(x1,y1);        // Avoid bug in tgi_line() */
  /* else */
    tgi_line(x1,y1,x2,y2);
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
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
  uint8_t width=5;
  uint8_t height=6;
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

  tgi_setcolor(mainColor);

  x=scalex[(Coord->x&0x1FF)];
  y=scaley[(Coord->y)+14&0x1FF];
  
  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

  y=scaley[(Coord->y)+14&0x1FF];

  /* the diet chardraw routine - fast text output. */
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&font[fontptr[a]];

      // Line 1
      b=*p;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      ++y;
      x-=width;
      ++p;

      // Line 2
      b=*p;
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      ++y;
      x-=width;
      ++p;

      // Line 3
      b=*p;
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      ++y;
      x-=width;
      ++p;

      // Line 4
      b=*p;
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      ++y;
      x-=width;
      ++p;

      // Line 5
      b=*p;
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      ++y;
      x-=width;
      ++p;

      // Line 6
      b=*p;
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;
      
      if (b<0)
	tgi_setpixel(x,y);
      ++x;
      b<<=1;

      ++y;
      x-=width;
      ++p;
      
      x+=width;
      y-=height;
    }

  return;
  
 chardraw_with_fries:
  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
      height<<=1;
    }
  
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
    }

  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&font[fontptr[a]];
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;

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

	  y+=deltaY;
	  x-=width;
	  ++p;
  	}

      Coord->x+=width;
      x+=width;
      y-=height;
    }

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
      tgi_setcolor(TGI_COLOR_BLACK);
      tgi_bar(scalex[TTYLoc.x],scaley[TTYLoc.y],scalex[TTYLoc.x+CharWide],scaley[TTYLoc.y+CharHigh]);
      tgi_setcolor(TGI_COLOR_WHITE);
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
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
  tgi_done();
  tgi_uninstall();
}
