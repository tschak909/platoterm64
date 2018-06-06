#define _optspeed_
#define PROTOCOL_DEBUG 1

#include <6502.h>
#include <stdio.h>
#include <tgi.h>
#include <stdbool.h>
#include <conio.h>
#include <serial.h>
#include <peekpoke.h>
#include <stdint.h>
#include "font.h"
#include "scale.h"
#include "protocol.h"

#ifdef PROTOCOL_DEBUG
#include <cbm.h>
#include <string.h>
#endif

static uint8_t c=0;
static uint8_t lastc=0;

static uint8_t CharWide=5;
static uint8_t CharHigh=6; 
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
const uint8_t welcomemsg_1[]={80,76,65,84,79,84,101,114,109,32,102,111,114,32,67,111,109,109,111,100,111,114,101,32,54,52};
#define WELCOMEMSG_1_LEN 26

// Copyright 2018 IRATA.ONLINE
const uint8_t welcomemsg_2[]={67,111,112,121,114,105,103,104,116,32,40,99,41,32,0x32,0x30,0x31,0x38,0x20,73,82,65,84,65,46,79,78,76,73,78,69};
#define WELCOMEMSG_2_LEN 31

// This software is licensed under GPL 3.0.
const uint8_t welcomemsg_3[]={84,104,105,115,32,115,111,102,116,119,97,114,101,32,105,115,32,108,105,99,101,110,115,101,100,32,117,110,100,101,114,32,71,80,76,32,51,46,48,32,46};
#define WELCOMEMSG_3_LEN 40

// See COPYING for details.
const uint8_t welcomemsg_4[]={83,101,101,32,99,111,112,121,105,110,103,32,102,111,114,32,100,101,116,97,105,108,115};
#define WELCOMEMSG_4_LEN 23

// PLATOTerm READY
const uint8_t welcomemsg_5[]={80,76,65,84,79,84,101,114,109,32,82,69,65,68,89};
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
  CurMode=ModeRewrite;
  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_clear();
  CharWide=5;
  CharHigh=6;
  TTYLoc.x = 0;
  TTYLoc.y = scaley[511]-CharHigh;
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
  return 1; /* ASCII terminal subtype */
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
  tgi_setcolor(TGI_COLOR_BLACK);
  tgi_bar(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
  tgi_setcolor(TGI_COLOR_WHITE);
}

/**
 * dotDraw(Coord) - Plot a mode 0 pixel
 */
void DotDraw(padPt* Coord)
{
  tgi_setpixel(scalex[Coord->x],scaley[Coord->y]);
}

/**
 * lineDraw(Coord1, Coord2) - Draw a mode 1 line
 */
void LineDraw(padPt* Coord1, padPt* Coord2)
{
  tgi_line(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * CharDraw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void CharDraw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  int16_t offset=0; /* due to negative offsets */
  uint8_t deltaX=CharWide;
  uint8_t deltaY=CharHigh;
  uint8_t* chPt=&font[0];
  int16_t x=0;
  int16_t y=0;
  int8_t i=0; /* current character counter */
  int8_t j=0; /* vertical loop counter */
  int8_t k=0; /* horizontal loop counter */
  int8_t a=0; /* current character byte */
  int8_t b=0; /* current character row bit */
  
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

  if (ModeBold)
    {
      deltaX <<= 1;  // 16 pixels
      deltaY <<= 1; // 32 pixels
    }

  x=scalex[(Coord->x)&0x1FF];
  y=scaley[(Coord->y+deltaY)&0x1FF];

  for (i=0;i<count;++i)
    {
      chPt=&font[fontptr[*ch]];
      for (j=0;j<deltaY;++j)
	{
	  x=scalex[(x&0x1ff)];
	  a=*chPt;
	  for (k=0;k<deltaX;++k)
	    {
	      b=a&0x80;
	      if (c==0x80)
		tgi_setpixel(x,y);
	      x++;
	      b<<=1;
	    }
	  y++;
	}
      ch++;
    }
}

/**
 * TTYChar - Called to plot chars when in tty mode
 */
void TTYChar(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    CharDraw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
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

void main(void)
{
  static const uint8_t pal[2]={TGI_COLOR_BLACK,TGI_COLOR_ORANGE};  
  struct ser_params params = {
    SER_BAUD_19200,
    SER_BITS_8,
    SER_STOP_1,
    SER_PAR_NONE,
    SER_HS_HW
  };
  
  c=ser_install(&c64_swlink);

  if (c!=SER_ERR_OK)
    {
      printf("ser_install returned: %d\n",c);
      return;
    }

  tgi_install(tgi_static_stddrv);
  tgi_init();
  install_nmi_trampoline();
  tgi_clear();
  POKE(0xD020,0);
  tgi_setpalette(pal);
  c=ser_open(&params);
  ser_ioctl(1, NULL);  

  // And do the terminal
  for (;;)
    {
      if (ser_get(&c)==SER_ERR_OK)
	{
	  // Detect and strip IAC escapes (two consecutive bytes of 0xFF)
	  if (c==0xFF && lastc == 0xFF)
	    {
	      lastc=0x00;
	    }
	  else
	    {
	      lastc=c;
	      ShowPLATO(&c,1);
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
