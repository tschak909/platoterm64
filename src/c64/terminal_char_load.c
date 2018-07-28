/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * terminal_char_load.c - Character set loading routine for 5x6 font.
 */

#include <cbm.h>
#include <c64.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "../terminal.h"
#include "../protocol.h"

// Temporary PLATO character data, 8x16 matrix
static unsigned char char_data[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char BTAB[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; // flip one bit on (OR)
static unsigned char BTAB_5[]={0x08,0x10,0x10,0x20,0x20,0x40,0x80,0x80}; // flip one bit on for the 5x6 matrix (OR)

static unsigned char TAB_0_5[]={0x05,0x05,0x05,0x04,0x04,0x04,0x03,0x03,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x00};

static unsigned char TAB_0_4[]={0x00,0x00,0x01,0x02,0x02,0x03,0x03,0x04}; // return 0..4 given index 0 to 7

static unsigned char PIX_THRESH[]={0x03,0x02,0x03,0x03,0x02, // Pixel threshold table.
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

static unsigned char TAB_0_25[]={0,5,10,15,20,25}; // Given index 0 of 5, return multiple of 5.

static unsigned char pix_cnt;     // total # of pixels
static unsigned char curr_word;   // current word
static unsigned char u,v,w;       // loop counters

extern unsigned char fontm23[768];
extern unsigned short fontptr[160];

/* /\** */
/*  * log - send data to printer */
/*  *\/ */
/* void log(const char* format, ...) */
/* { */
/*   char outputbuff[256]; */
/*   va_list args; */
/*   cbm_open(4,4,CBM_WRITE,""); */
/*   va_start(args, format); */
/*   vsprintf(outputbuff,format,args); */
/*   cbm_write(4,outputbuff,strlen(outputbuff)); */
/*   va_end(args); */
/*   cbm_close(4); */
/* } */

/**
 * terminal_char_load - Store a character into the user definable
 * character set.
 */
void terminal_char_load(padWord charnum, charData theChar)
{
  // Clear char data. 
  memset(char_data,0,sizeof(char_data));
  memset(PIX_WEIGHTS,0,sizeof(PIX_WEIGHTS));
  memset(&fontm23[fontptr[charnum]],0,6);
  
  // Transpose character data.  
  for (curr_word=0;curr_word<8;curr_word++)
    {
      for (u=16; u-->0; )
	{
	  if (theChar[curr_word] & 1<<u)
	    {
	      pix_cnt++;
	      PIX_WEIGHTS[TAB_0_25[TAB_0_5[u]]+TAB_0_4[curr_word]]++;
	      char_data[u^0x0F&0x0F]|=BTAB[curr_word];
	    }
	}
    }

  // Determine algorithm to use for number of pixels.
  // Algorithm A is used when roughly half of the # of pixels are set.
  // Algorithm B is used either when the image is densely or sparsely populated (based on pix_cnt).
  if ((54 <= pix_cnt) && (pix_cnt < 85))
    {
      // Algorithm A - approx Half of pixels are set
      for (v=6; v-->0; )
  	{
  	  for (w=5; w-->0; )
  	    {
  	      if (PIX_WEIGHTS[TAB_0_25[v]+w] >= PIX_THRESH[TAB_0_25[v]+w])
  		fontm23[fontptr[charnum]+v]|=BTAB[w];
  	    }
  	}
    }
  else if ((pix_cnt < 54) || (pix_cnt >= 85))
    {
      // Algorithm B - Sparsely or heavily populated bitmaps
      for (v=16; v-->0; )
	{
	  for (w=8; w-->0; )
	    {
	      if (char_data[v] & (1<<w))
		{
		  fontm23[fontptr[charnum]+TAB_0_5[v]]|=BTAB_5[w];
		}
	    }
	}
    }
  
}

