/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * terminal_char_load.c - Character set loading routine for 8x16 font.
 */

#include <string.h>
#include "../terminal.h"
#include "../protocol.h"

static unsigned char BTAB[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; // flip one bit on (OR)
static unsigned char u;
static unsigned char curr_word;

extern unsigned char fontm23[768];
extern unsigned short fontptr[160];

/**
 * terminal_char_load - Store a character into the user definable
 * character set.
 */
void terminal_char_load(padWord charnum, charData theChar)
{
  // clear char data
  memset(&fontm23[fontptr[charnum]],0,16);

  // Transpose character data
  for (curr_word=0;curr_word<8;curr_word++)
    {
      for (u=16; u-->0; )
	{
	  if (theChar[curr_word] & 1<<u)
	    {
	      fontm23[fontptr[charnum]+u^0x0f&0x0f]|=BTAB[curr_word];
	    }
	}
    }

  // and...that's it, really. :)
  
}

