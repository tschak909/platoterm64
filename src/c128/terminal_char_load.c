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

static unsigned char char_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char BTAB[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; // flip one bit on (OR)
static unsigned char u;
static unsigned char curr_word;

#define VDC_LORES 0
#define VDC_HIRES 1
extern uint8_t vdcmode;

extern unsigned char fontm23[2048];

#define FONTPTR_200(a) (((a << 1) + a) << 1)
#define FONTPTR_400(a) (a << 4)

#define FONTPTR(a) ((vdcmode == VDC_LORES) ? FONTPTR_200(a) : FONTPTR_400(a))

/**
 * terminal_char_load - Store a character into the user definable
 * character set.
 */
void terminal_char_load(padWord charNum, charData theChar)
{
  switch (vdcmode)
  {
  case VDC_HIRES:
    // clear char data
    memset(&fontm23[FONTPTR(charNum)], 0, 16);

    // Transpose character data
    for (curr_word = 0; curr_word < 8; curr_word++)
    {
      for (u = 16; u-- > 0;)
      {
        if (theChar[curr_word] & 1 << u)
        {
          fontm23[FONTPTR(charNum) + u ^ 0x0f & 0x0f] |= BTAB[curr_word];
        }
      }
    }

    // and...that's it, really. :)

    break;

  case VDC_LORES:
    // Clear char data.
    memset(char_data, 0, sizeof(char_data));

    // load and transpose character data into 8x16 array
    for (curr_word = 0; curr_word < 8; curr_word++)
    {
      for (u = 16; u-- > 0;)
      {
        if (theChar[curr_word] & 1 << u)
        {
          char_data[u ^ 0x0F & 0x0F] |= BTAB[curr_word];
        }
      }
    }

    // OR pixel rows together from 16 to 6 rows
    fontm23[(charNum * 6) + 0] = char_data[0] | char_data[1] | char_data[2];
    fontm23[(charNum * 6) + 1] = char_data[3] | char_data[4];
    fontm23[(charNum * 6) + 2] = char_data[5] | char_data[6] | char_data[7];
    fontm23[(charNum * 6) + 3] = char_data[8] | char_data[9];
    fontm23[(charNum * 6) + 4] = char_data[10] | char_data[11] | char_data[12];
    fontm23[(charNum * 6) + 5] = char_data[13] | char_data[14] | char_data[15];
    break;
  }
}
