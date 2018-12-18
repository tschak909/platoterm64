/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.c - Keyboard functions (atari)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <peekpoke.h>
#include <conio.h>
#include <serial.h>
#include "../io.h"
#include "../touch.h"
#include "../plato_key.h"
#include "../keyboard.h"
#include "../screen.h"
#include "key.h"

static uint16_t delay;
static uint8_t lastkey,key;
static uint8_t keyboard_start_pressed;
static uint8_t keyboard_select_pressed;
static uint8_t keyboard_option_pressed;
extern uint8_t xoff_enabled;
extern void click();
extern padBool TTY;
extern padPt TTYLoc;
static uint8_t tty_ch;

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  // Read console keys, no need to read combos of these at present.
  key=PEEK(764);
  
  if (PEEK(0xD01F)==5 && key != lastkey)
    {
      if (key==45)
	{
	  ShowPLATO("TTY Mode",8);
	  TTY=true;
	}
      else if (key==10)
	{
	  ShowPLATO("PLATO Mode",10);
	  TTY=false;
	}
      else if (key==12)
	{
	  keyboard_out_tty(0x0D);
	  keyboard_out_tty(0x0A);
	}
      
      TTYLoc.y-=16;
      POKE(764,255);
    }
  else if (TTY)
    {
      if (kbhit())
	{
	  tty_ch=cgetc();
	  if (tty_ch==0x9B) // ATASCII EOL, send CR/LF
	    {
	      keyboard_out_tty(0x0D);
	    }
	  else if (tty_ch==0x7E) // ATASCII BS, send ASCII BS
	    {
	      keyboard_out_tty(0x08);
	    }
	  else
	    {
	      keyboard_out_tty(tty_ch);
	    }
	}
    }
  else if (key!=lastkey)
    {
      click();
      keyboard_out(key_to_pkey[key]);
      POKE(764,255);
    }
  lastkey=key;
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{

}
