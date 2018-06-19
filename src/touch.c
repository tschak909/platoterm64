/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions
 */

#include <mouse.h>
#include "touch.h"
#include "scale.h"

static padBool TouchActive;

static struct mouse_info mouse_data;
static uint16_t previous_mouse_x;
static uint16_t previous_mouse_y;
static uint16_t screen_w;
static uint16_t screen_h;

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{
  mouse_install(&mouse_def_callbacks,&mouse_static_stddrv);
  mouse_show();
  touch_hide();
}

/**
 * TouchAllow - Set whether touchpanel is active or not.
 */
void TouchAllow(padBool allow)
{
  // If mouse is off screen (due to previously being moved off screen, move onscreen to make visible.
  if (allow)
    {
      mouse_move(previous_mouse_x,previous_mouse_y);
    }
  else
    {
      previous_mouse_x = mouse_data.pos.x;
      previous_mouse_y = mouse_data.pos.y;
    }
  TouchActive=allow;
}

/**
 * handle_mouse - Process mouse events and turn into scaled touch events
 */
void handle_mouse(void)
{
  uint8_t lastbuttons;
  padPt coord;
  
  mouse_info(&mouse_data);

  /* If touch screen isn't active, don't let the mouse be used. */
  if (TouchActive==0)
    {
      previous_mouse_x = mouse_data.pos.x;
      previous_mouse_y = mouse_data.pos.y;
      mouse_move(screen_w,screen_h);
      return;
    }
  
  if (mouse_data.buttons == lastbuttons)
    return; /* debounce */
  else if ((mouse_data.buttons & MOUSE_BTN_LEFT))
    {
      coord.x = scaletx[mouse_data.pos.x];
      coord.y = scalety[mouse_data.pos.y];
      Touch(&coord);
    }
  lastbuttons = mouse_data.buttons;
}

/**
 * touch_hide() - hide the mouse cursor
 */
void touch_hide(void)
{
  mouse_move(screen_w,screen_h);
}
