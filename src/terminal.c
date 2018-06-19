/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * terminal.c - Terminal state functions
 */

/* Some functions are intentionally stubbed. */
#pragma warn(unused-param, off)

#include <stdbool.h>
#include "terminal.h"

/**
 * ASCII Features to return in Features
 */
#define ASC_ZFGT        0x01
#define ASC_ZPCKEYS     0x02
#define ASC_ZKERMIT     0x04
#define ASC_ZWINDOW     0x08

/**
 * protocol.c externals
 */
extern CharMem CurMem;
extern padBool TTY;
extern padBool ModeBold;
extern padBool Rotate;
extern padBool Reverse;
extern DispMode CurMode;
extern padBool FlowControl;

/**
 * screen.c externals
 */
extern uint8_t CharWide;
extern uint8_t CharHigh;
extern padPt TTYLoc;

/**
 * terminal_init()
 * Initialize terminal state
 */
void terminal_init(void)
{
  SetTTY();
  TTYLoc.y=368; // Right under the greeting.
}

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
  CurMode=ModeWrite;    /* For speed reasons. */
  /* tgi_setcolor(TGI_COLOR_WHITE); */ // move this to screen.c
  /* tgi_clear(); */ // also move to screen.c
  CharWide=8;
  CharHigh=16;
  TTYLoc.x = 0;        // leftmost coordinate on screen
  TTYLoc.y = 495;      // Top of screen - one character height
}

/**
 * SetPLATO(void) - Switch to PLATO mode
 */
void SetPLATO(void)
{
  TTY=false;
  /* tgi_clear(); */  // move to screen.c
}

/**
 * Features(void) - Inquire about terminal ASCII features
 */
uint8_t Features(void)
{
  return ASC_ZFGT; /* This terminal can do Fine Grained Touch (FGT) */
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
