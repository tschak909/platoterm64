/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions (c64)
 */

#include <stdint.h>
#include "../protocol.h"

/**
 * touch_translate - Translate coordinates from native system to PLATO
 */
void touch_translate(padPt* Coord)
{
  Coord->x = Coord->x-64;
  Coord->y = Coord->y^0x1FF;
}
