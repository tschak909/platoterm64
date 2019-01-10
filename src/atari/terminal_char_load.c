/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * terminal_char_load.c - Character set loading routine for 5x6 font.
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <peekpoke.h>
#include "../terminal.h"
#include "../protocol.h"

#define FONTPTR(a) (((a << 1) + a) << 1)

static unsigned char char_data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

const unsigned char BTAB[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; // flip one bit on (OR)
const unsigned char BTAB_5[]={0x08,0x10,0x10,0x20,0x20,0x40,0x80,0x80}; // flip one bit on for the 5x6 matrix (OR)
const unsigned char TAB_0_5[]={0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04,0x04,0x05,0x05,0x05};
const unsigned char TAB_0_4[]={0x00,0x00,0x01,0x02,0x02,0x03,0x03,0x04}; // return 0..4 given index 0 to 7
const unsigned char TAB_0_25[]={0,5,10,15,20,25}; // Given index 0 of 5, return multiple of 5.
const unsigned char TAB_0_25i[]={25,20,15,10,5,0};

const unsigned char PIX_THRESH[]={0x03,0x02,0x03,0x03,0x02, // Pixel threshold table.
                   0x03,0x02,0x03,0x03,0x02,
                   0x02,0x01,0x02,0x02,0x01,
                   0x02,0x01,0x02,0x02,0x01,
                   0x03,0x02,0x03,0x03,0x02,
                   0x03,0x02,0x03,0x03,0x02};

static unsigned char PIX_WEIGHTS[]={0x00,0x00,0x00,0x00,0x00, // Pixel weights
                    0x00,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00,0x00};

static unsigned char pix_cnt;
static unsigned char curr_word;
static unsigned char u,v,w;
static unsigned char temp_byte;

extern unsigned char fontm23[768];

/**
 * terminal_char_load - Store a character into the user definable
 * character set.
 */
void terminal_char_load(padWord charnum, charData theChar)
{
  // Clear char data. 
  memset(char_data,0,sizeof(char_data));
  memset(PIX_WEIGHTS,0,sizeof(PIX_WEIGHTS));
  memset(&fontm23[FONTPTR(charnum)],0,6);

  // Transpose character data.  
  for (curr_word=0;curr_word<8;curr_word++)
    {
      for (u=16; u-->0; )
	{
	  if (theChar[curr_word] & 1<<u)
	    {
	      pix_cnt++;
	      PIX_WEIGHTS[TAB_0_25[TAB_0_5[u]]+TAB_0_4[curr_word]]++;
	      char_data[u^0x0f]|=BTAB[curr_word];
	    }
	}
    }
  
  if ((54 <= pix_cnt) && (pix_cnt < 85))
    {
      // Algorithm A - approx Half of pixels are set
      for (v=6; v-->0; )
        {
          for (w=5; w-->0; )
            {
              if (PIX_WEIGHTS[TAB_0_25i[v]+w] >= PIX_THRESH[TAB_0_25i[v]+w])
                fontm23[FONTPTR(charnum)+v]|=BTAB[w];
            }
        }
    }
  else
    {
      // Algorithm B - Sparsely or heavily populated bitmaps
      for (v=16; v-->0; )
        {
	  if (pix_cnt >= 85)
	    char_data[v]^=0xff;
	  
          temp_byte = 0x00;    

          for (w=8; w-->0; )
            {
              if (char_data[v] & (1<<w))
                {
                  temp_byte|=BTAB_5[w];
                }
	      fontm23[FONTPTR(charnum)+TAB_0_5[v]]|=temp_byte;
            }
        }

      // Flip the bits back
      if (pix_cnt >= 85)
	{
	  for (v=6; v-->0; )
	    {
	      fontm23[FONTPTR(charnum)+v]^=0xFF;
	      fontm23[FONTPTR(charnum)+v]&=0xF8;
	    }
	}
    }
}
