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
#include "../io.h"
#include "../touch.h"
#include "../prefs.h"
#include "../plato_key.h"
#include "../keyboard.h"
#include "../screen.h"
#include "key.h"

static uint8_t lastkey,key;
static uint8_t keyboard_start_pressed;
static uint8_t keyboard_select_pressed;
static uint8_t keyboard_option_pressed;
extern uint8_t xoff_enabled;
extern uint8_t prefs_running;
extern void click();
extern padBool TTY;

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  // Read console keys, no need to read combos of these at present.
  key=PEEK(764);
  if (PEEK(0xD01F)==6)
    keyboard_start_pressed=true;
  else if (PEEK(0xD01F)==5)
    keyboard_select_pressed=true;
  else if (PEEK(0xD01F)==3)
    keyboard_option_pressed=true;
  else if (PEEK(0xD01F)==7)
    keyboard_start_pressed=keyboard_select_pressed=keyboard_option_pressed=false;

  // Process non-terminal keys
  if (keyboard_option_pressed==true && prefs_running==false)
    {
      prefs_run();
      POKE(764,255);
    }
  else if (keyboard_select_pressed==true)
    {
      
      if (key==14)
	{
	  screen_cycle_background();
	  screen_cycle_border();
	  POKE(764,255);
	  screen_update_colors();
	}
      else if (key==15)
	{
	  screen_cycle_background_back();
	  screen_cycle_border_back();
	  POKE(764,255);
	  screen_update_colors();
	}
      else if (key==6)
	{
	  screen_cycle_foreground_back();
	  POKE(764,255);
	  screen_update_colors();
	}
      else if (key==7)
	{
	  screen_cycle_foreground();
	  POKE(764,255);
	  screen_update_colors();
	}
      else if (key==22)
	{
	  prefs_clear(); // because greeting might be there.
	  prefs_display("exit platoterm (y/n)? ");
	  key=prefs_get_key_matching("ynYN");
	  switch(key)
	    {
	    case 'y':
	      POKE(764,255); // clear keyboard buffer
	      POKE(82,2);    // reset left margin to OS default.
	      io_done();
	      touch_done();
	      screen_done();
	      exit(0);
	      break;
	    default:
	      prefs_clear();
	    }
	}

    }
  else if (key!=lastkey)
    {
      click();
      // This is a mess.
      if (TTY)
	{
	  switch(key)
	    {
	    case 0x0c: // RETURN
	      keyboard_out_tty(0x0D);
	      keyboard_out_tty(0x0A);
	      break;
	    case 0x5b: // Ampersand
	      keyboard_out_tty('&');
	      break;
	    case 0x58: // Dollar sign
	      keyboard_out_tty('$');
	      break;
	    case 0x5A: // Pound sign
	      keyboard_out_tty('#');
	      break;
	    default:
	      keyboard_out(key_to_pkey[key]);
	      break;
	    }
	}
      else
	{
	  keyboard_out(key_to_pkey[key]);
	}
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
