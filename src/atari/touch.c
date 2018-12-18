/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions (atari)
 */

#include "../protocol.h"

unsigned short touch_scale_320(short x)
{
   uint16_t n, q;
   n  = x << 3;
   q  = x + (x >> 1);
   q += q >> 4;
   q += q >> 8;
   n -= q << 2;
   n -= q;
   n += ((n << 1) + n) << 2;
   return q + (n >> 6);
}

unsigned short touch_scale_192(short y)
{
  uint16_t n, q;
  n  = y << 3;
  q  = ((y << 1) + n) >> 2;
  q += q >> 4;
  q += q >> 8;
  n -= q << 1;
  n -= q;
  n += ((n << 2) + n) << 1;
  return (q + (n >> 5) ^ 0x1FF);
}

/**
 * touch_translate - Translate coordinates from native system to PLATO
 */
void touch_translate(padPt* Coord)
{
  Coord->x = touch_scale_320(Coord->x);
  Coord->y = touch_scale_192(Coord->y);
}
