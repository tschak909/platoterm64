/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions (atari)
 */

#include "../convert_coordinates.h"
#include "../protocol.h"

/**
 * touch_translate - Translate coordinates from native system to PLATO
 */
void touch_translate(padPt* Coord)
{
  Coord->x = touch_scale_320(Coord->x);
  Coord->y = touch_scale_192(Coord->y);
}


