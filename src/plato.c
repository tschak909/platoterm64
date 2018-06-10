#define PROTOCOL_DEBUG 1

#include <6502.h>
#include <stdio.h>
#include <tgi.h>
#include <stdbool.h>
#include <conio.h>
#include <serial.h>
#include <peekpoke.h>
#include <stdint.h>
#include <stdlib.h>
#include "font.h"
#include "scale.h"
#include "protocol.h"

#ifdef PROTOCOL_DEBUG
#include <cbm.h>
#include <string.h>
#endif

static uint8_t modemc=0;
static uint8_t lastmodemc=0;

static uint8_t CharWide=8;
static uint8_t CharHigh=16; 
static padBool TouchActive;

static padPt TTYLoc;

extern padPt PLATOSize;
extern CharMem CurMem;
extern padBool TTY;
extern padBool FlowControl;
extern padBool ModeBold;
extern padBool Rotate;
extern padBool Reverse;
extern DispMode CurMode;

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
 * SetTTY(void) - Switch to TTY mode
 */
void SetTTY(void)
{
  TTY=true;
  ModeBold=padF;
  Rotate=padF;
  Reverse=padF;
  CurMem=M0;
  /* CurMode=ModeRewrite; */
  CurMode=ModeWrite; /* For speed reasons. */
  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_clear();
  CharWide=8;
  CharHigh=16;
  TTYLoc.x = 0;
  TTYLoc.y = 368;
}

/**
 * SetPLATO(void) - Switch to PLATO mode
 */
void SetPLATO(void)
{
  TTY=false;
  tgi_clear();
}

/**
 * TermType(void) - Return the appropriate terminal type
 */
uint8_t TermType(void)
{
  return 12; /* ASCII terminal type */
}

/**
 * SubType(void) - Return the appropriate terminal subtype
 */
uint8_t SubType(void)
{
  return 1; /* ASCII terminal subtype IST-III */
}

/**
 * LoadFile(void) - Return the appropriate terminal loadfile (should just be 0)
 */
uint8_t LoadFile(void)
{
  return 0; /* This terminal does not load its resident from the PLATO system. */
}

/**
 * Configuration(void) - Return the terminal configuration
 */
uint8_t Configuration(void)
{
  return 0x40; /* Touch panel is present. */
}

/**
 * CharAddress(void) - Return the base address of the character set.
 */
uint16_t CharAddress(void)
{
  return 0x3000; /* What the? Shouldn't this be 0x3800? */
}

/**
 * Beep(void) - Beep the terminal
 */
void Beep(void)
{
  /* TODO: Implement beep(); */
}

/**
 * MemRead - Read a byte of program memory.
 * not needed for our terminal, but must
 * be decoded.
 */
padByte MemRead(padWord addr)
{
  return (0xFF);
}

/**
 * MemLoad - Write a byte to non-character memory.
 * not needed for our terminal, but must be decoded.
 */
void MemLoad(padWord addr, padWord value)
{
  /* Not Implemented */
}

/**
 * CharLoad - Store a character into the user definable
 * character set.
 */
void CharLoad(padWord charnum, charData theChar)
{
  /* TODO: Implement CharLoad */
}

/**
 * Mode5, 6, and 7 are basically stubbed.
 */
void Mode5(padWord value)
{
  
}

void Mode6(padWord value)
{
  
}

void Mode7(padWord value)
{
  
}

/**
 * TouchAllow - Set whether touchpanel is active or not.
 */
void TouchAllow(padBool allow)
{
  TouchActive=allow;
}

/**
 * ExtAllow - External Input allowed. Not implemented.
 */
void ExtAllow(padBool allow)
{
  /* Not Implemented */
}

/**
 * SetExtIn - Set which device to get input from.
 * Not implemented
 */
void SetExtIn(padWord device)
{
}

/**
 * SetExtOut - Set which device to send external data to.
 * Not implemented
 */
void SetExtOut(padWord device)
{
}

/**
 * ExtIn - get an external input from selected device.
 * Not implemented.
 */
padByte ExtIn(void)
{
  return 0;
}

/**
 * ExtOut - Send an external output to selected device
 * Not implemented.
 */
void ExtOut(padByte value)
{
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
  uint8_t mainColor=TGI_COLOR_WHITE;
  uint8_t altColor;
    
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
  
  // Mode Write or Erase
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=TGI_COLOR_BLACK;
  else
    mainColor=TGI_COLOR_WHITE;
  
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

	      if (Rotate)
		{
		  if (ModeBold)
		    x-=2;
		  else
		    x--;
		}
	      else
		{
		  if (ModeBold)
		    x+=2;
		  else
		    ++x;
		}
	      
  	      b<<=1;
  	    }

	  if (ModeBold)
	    y+=2;
	  else
	    ++y;
	    
  	}

      /* If vertical, X axes behavior needs to be reversed for next character */
      if (Rotate)
	{
	  if (ModeBold)
	    {
	      if (Reverse)
		Coord->x-=width+width;
	      else
		Coord->x+=width+width;
	    }
	  else
	    {
	      if (Reverse)
		Coord->x+=width;
	      else
		Coord->x-=width; 
	    }
	}
      else
	{
	  if (ModeBold)
	    {
	      if (Reverse)
		Coord->x-=width+width;
	      else
		Coord->x+=width+width;
	    }
	  else
	    {
	      if (Reverse)
		Coord->x-=width;
	      else
		Coord->x+=width; 
	    }
	}
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

void main(void)
{
  static const uint8_t pal[2]={TGI_COLOR_BLUE,TGI_COLOR_LIGHTBLUE};
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

  tgi_install(tgi_static_stddrv);
  tgi_init();
  install_nmi_trampoline();
  tgi_setpalette(pal);
  modemc=ser_open(&params);
  ser_ioctl(1, NULL);  

  SetTTY();
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
      if (kbhit())
	{
	  send_byte(cgetc());
	}
    }
  tgi_done();
  ser_close();
  ser_uninstall();
  tgi_uninstall();  
}
