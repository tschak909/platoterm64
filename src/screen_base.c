/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen_base.c - Display output functions (base)
 */

#include <string.h>
#include <stdbool.h>
#include <tgi.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <stdio.h>
#include "screen.h"
#include "protocol.h"
#include "io.h"

uint8_t CharWide=8;
uint8_t CharHigh=16;
padPt TTYLoc;
uint8_t pal[2];
unsigned char current_foreground=COLOR_WHITE;
unsigned char current_background=COLOR_BLACK;

#define RECV_BUFFER_SIZE 384
#define FONTM23_SIZE 768

extern uint8_t xoff_enabled; /* io.c */
extern padBool FastText; /* protocol.c */
extern uint8_t* recv_buffer;
extern uint8_t font[];
extern uint8_t FONT_SIZE_X;
extern uint8_t FONT_SIZE_Y;
extern uint8_t* fontm23;

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  screen_load_driver();
  tgi_init();
  screen_init_hook();
  tgi_clear();
  recv_buffer=malloc(RECV_BUFFER_SIZE);
  fontm23=malloc(FONTM23_SIZE);
}

/**
 * screen_splash - Show splash screen
 */
void screen_splash(void)
{
  TTYLoc.x = 0;
  TTYLoc.y = 496;
  ShowPLATO("PLATOTerm Cartridge Version",27);

  TTYLoc.x = 0;
  TTYLoc.y-=16;
  ShowPLATO("(\xC0) 2018 IRATA.ONLINE",22);
  
  TTYLoc.x = 0;
  TTYLoc.y-=16;
  ShowPLATO("1200 Baud. Terminal Ready.",26);
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
  tgi_clear();
}

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
  tgi_done();
  tgi_uninstall();
  free(recv_buffer);
  free(fontm23);
}
