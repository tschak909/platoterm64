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
#include "config.h"

static padBool TouchActive;

static struct mouse_info mouse_data;
static uint16_t screen_w;
static uint16_t screen_h;

extern ConfigInfo config;
extern uint16_t scaletx[];
extern uint16_t scalety[];

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{
  mouse_load_driver(&mouse_def_callbacks,config.driver_mou);
  mouse_show();
}

/**
 * touch_allow - Set whether touchpanel is active or not.
 */
void touch_allow(padBool allow)
{
  /* // If mouse is off screen (due to previously being moved off screen, move onscreen to make visible. */
  /* if (allow) */
  /*   { */
  /*     mouse_move(previous_mouse_x,previous_mouse_y); */
  /*   } */
  /* else */
  /*   { */
  /*     if (mouse_data.pos.x != screen_w && mouse_data.pos.y != screen_h) */
  /* 	{ */
  /* 	  previous_mouse_x = mouse_data.pos.x; */
  /* 	  previous_mouse_y = mouse_data.pos.y; */
  /* 	  mouse_move(screen_w,screen_h); */
  /* 	} */
  /*   } */
  TouchActive=allow;
}

/**
 * touch_main - Process mouse events and turn into scaled touch events
 */
void touch_main(void)
{
  uint8_t lastbuttons;
  padPt coord;
  
  mouse_info(&mouse_data);
  
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

/**
 * touch_done() - Stop the mouse driver
 */
void touch_done(void)
{
  mouse_uninstall();
}
