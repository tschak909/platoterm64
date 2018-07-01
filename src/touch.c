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

/* Scaling tables for touch to convert 320x192 to 512x512 */
unsigned short scaletx[]={
0,2,4,5,7,8,10,12,
13,15,16,18,20,21,23,24,
26,28,29,31,32,34,36,37,
39,40,42,44,45,47,48,50,
52,53,55,56,58,60,61,63,
64,66,68,69,71,72,74,76,
77,79,80,82,84,85,87,88,
90,92,93,95,96,98,100,101,
103,104,106,108,109,111,112,114,
116,117,119,120,122,124,125,127,
128,130,132,133,135,136,138,140,
141,143,144,146,148,149,151,152,
154,156,157,159,160,162,164,165,
167,168,170,172,173,175,176,178,
180,181,183,184,186,188,189,191,
192,194,196,197,199,200,202,204,
205,207,208,210,212,213,215,216,
218,220,221,223,224,226,228,229,
231,232,234,236,237,239,240,242,
244,245,247,248,250,252,253,255,
256,258,260,261,263,264,266,268,
269,271,272,274,276,277,279,280,
282,284,285,287,288,290,292,293,
295,296,298,300,301,303,304,306,
308,309,311,312,314,316,317,319,
320,322,324,325,327,328,330,332,
333,335,336,338,340,341,343,344,
346,348,349,351,352,354,356,357,
359,360,362,364,365,367,368,370,
372,373,375,376,378,380,381,383,
384,386,388,389,391,392,394,396,
397,399,400,402,404,405,407,408,
410,412,413,415,416,418,420,421,
423,424,426,428,429,431,432,434,
436,437,439,440,442,444,445,447,
448,450,452,453,455,456,458,460,
461,463,464,466,468,469,471,472,
474,476,477,479,480,482,484,485,
487,488,490,492,493,495,496,498,
500,501,503,504,506,508,509,};

unsigned short scalety[]={
510,507,504,502,499,496,494,491,
488,486,483,480,478,475,472,470,
467,464,462,459,456,454,451,448,
446,443,440,438,435,432,430,427,
424,422,419,416,414,411,408,406,
403,400,398,395,392,390,387,384,
382,379,376,374,371,368,366,363,
360,358,355,352,350,347,344,342,
339,336,334,331,328,326,323,320,
318,315,312,310,307,304,302,299,
296,294,291,288,286,283,280,278,
275,272,270,267,264,262,259,256,
254,251,248,246,243,240,238,235,
232,230,227,224,222,219,216,214,
211,208,206,203,200,198,195,192,
190,187,184,182,179,176,174,171,
168,166,163,160,158,155,152,150,
147,144,142,139,136,134,131,128,
126,123,120,118,115,112,110,107,
104,102,99,96,94,91,88,86,
83,80,78,75,72,70,67,64,
62,59,56,54,51,48,46,43,
40,38,35,32,30,27,24,22,
19,16,14,11,8,6,3,0,
};

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
