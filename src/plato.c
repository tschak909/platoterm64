#define _optspeed_
#define PROTOCOL_DEBUG 1

#include <6502.h>
#include <stdio.h>
#include <tgi.h>
#include <conio.h>
#include <serial.h>
#include <peekpoke.h>
#include <stdint.h>
#include "font.h"
#include "scale.h"
#include "protocol.h"

#ifdef PROTOCOL_DEBUG
#include <cbm.h>
#include <string.h>
#endif

uint8_t retval;
uint8_t c=0;
uint8_t lastc=0;

uint16_t mode=017;
uint8_t escape;
uint8_t dumb_terminal_active=1;
uint16_t margin=0;
uint16_t x=0;
uint16_t y=0;
uint16_t last_x=0;
uint16_t last_y=0;
uint8_t delta_x=0;
uint8_t delta_y=0;
uint8_t ascii_state=0;
uint8_t ascii_bytes=0;
uint8_t pmd[64];
uint8_t font_pmd=0;
uint8_t font_info=0;
uint8_t connection_active=1;
uint8_t xor_mode=0;
uint8_t character_set=0;
uint8_t vertical_writing_mode=0;
uint8_t reverse_writing_mode=0;
uint8_t bold_writing_mode=0;
uint8_t mode_words=0;
uint32_t assembler=0; 
uint16_t mar=0;
uint8_t flow_control=0;
uint32_t platofgcolor=0;
uint32_t platobgcolor=0;
uint16_t mode4start=0;
uint8_t touch=0;
uint16_t plato_m23[128*8];
uint8_t special_mode=0;
uint8_t special_mode_param=0;

// PLATOTerm for Commodore 64
const uint8_t welcomemsg_1[]={80,76,65,84,79,84,101,114,109,32,102,111,114,32,67,111,109,109,111,100,111,114,101,32,54,52};
#define WELCOMEMSG_1_LEN 26

// Copyright 2018 IRATA.ONLINE
const uint8_t welcomemsg_2[]={67,111,112,121,114,105,103,104,116,32,40,99,41,32,0x32,0x30,0x31,0x38,0x20,73,82,65,84,65,46,79,78,76,73,78,69};
#define WELCOMEMSG_2_LEN 31

// This software is licensed under GPL 3.0.
const uint8_t welcomemsg_3[]={84,104,105,115,32,115,111,102,116,119,97,114,101,32,105,115,32,108,105,99,101,110,115,101,100,32,117,110,100,101,114,32,71,80,76,32,51,46,48,32,46};
#define WELCOMEMSG_3_LEN 40

// See COPYING for details.
const uint8_t welcomemsg_4[]={83,101,101,32,99,111,112,121,105,110,103,32,102,111,114,32,100,101,116,97,105,108,115};
#define WELCOMEMSG_4_LEN 23

// PLATOTerm READY
const uint8_t welcomemsg_5[]={80,76,65,84,79,84,101,114,109,32,82,69,65,68,89};
#define WELCOMEMSG_5_LEN 15

// The static symbol for the c64 swlink driver
extern char c64_swlink;
extern void install_nmi_trampoline(void);

void send_byte(uint8_t b)
{
  ser_put(b);
}

void scroll_up(void)
{
}

void draw_char(uint8_t charset_to_use, uint8_t char_to_plot)
{
  uint16_t a;
  uint16_t nx;
  uint16_t ny;
  uint16_t b;
  a=0;
  nx=scalex[x+margin];
  ny=scaley[y+16];
  
  if (charset_to_use==1)
    a=64;
   
  a=a+char_to_plot;

  nx=scalex[x+margin];

  /////////////////////////////////////////////////////////////////////
  // Line 1
  b=font[fontptr[a]];

  // Dot 1
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 2
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 3
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 4
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 5
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx=scalex[x+margin];
  ny++;

  /////////////////////////////////////////////////////////////////////
  // Line 2
  b=font[fontptr[a]+1];

  // Dot 1
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 2
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 3
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 4
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 5
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx=scalex[x+margin];
  ny++;

  /////////////////////////////////////////////////////////////////////
  // Line 3
  b=font[fontptr[a]+2];

  // Dot 1
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 2
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 3
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 4
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 5
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx=scalex[x+margin];
  ny++;

  /////////////////////////////////////////////////////////////////////
  // Line 4
  b=font[fontptr[a]+3];

  // Dot 1
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 2
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 3
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 4
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 5
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx=scalex[x+margin];
  ny++;

  /////////////////////////////////////////////////////////////////////
  // Line 5
  b=font[fontptr[a]+4];

  // Dot 1
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 2
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 3
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 4
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 5
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx=scalex[x+margin];
  ny++;

  /////////////////////////////////////////////////////////////////////
  // Line 6
  b=font[fontptr[a]+5];

  // Dot 1
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 2
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 3
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 4
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx++;
  
  // Dot 5
  c=b&0x80;
  if (c==0x80)
    tgi_setpixel(nx,ny);
  b <<= 1;
  nx=scalex[x+margin];
  ny++;
  
  /* for (i=0;i<6;i++) */
  /*   { */
  /*     nx=scalex[x+margin]; */
  /*     b=font[fontptr[a]+i]; */
  /*     for (j=0;j<5;j++) */
  /* 	{ */
  /* 	  c=b&0x80; */
  /* 	  if (c==0x80) */
  /* 	    tgi_setpixel(nx,ny); */
  /* 	  nx++; */
  /* 	  b <<= 1; */
  /* 	} */
  /*     ny++; */
  /*   } */

  x=x+delta_x;
}

void screen_erase(void)
{
  tgi_clear();
}

void screen_erase_block(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  tgi_setcolor(TGI_COLOR_BLACK);
  tgi_bar(scalex[x1],scaley[y1],scalex[x2],scaley[y2]);
  tgi_setcolor(TGI_COLOR_ORANGE);
}
void screen_sleep(void)
{
}

void screen_backspace(void)
{
}

void screen_forwardspace(void)
{
}

void beep(void)
{
}

void draw_string(const char* text,uint8_t len)
{
  uint8_t* currChar=NULL;
  uint8_t counter=0;
  for (counter=0;counter<len;counter++)
    {
      decode_dumb_terminal(text[counter]);
    }
}

void draw_point(uint16_t x,uint16_t y)
{
  tgi_setpixel(scalex[x],scaley[y]);
}

void draw_line(uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
  tgi_line(scalex[x1],scaley[y1],scalex[x2],scaley[y2]);
}

void paint(void)
{
}

void enable_touch(uint8_t n)
{
  touch=n;
}

void greeting(void)
{
  x=168; y=480; draw_string(welcomemsg_1,WELCOMEMSG_1_LEN);
  x=144; y=464; draw_string(welcomemsg_2,WELCOMEMSG_2_LEN);
  x=104; y=432; draw_string(welcomemsg_3,WELCOMEMSG_3_LEN);
  x=160; y=416; draw_string(welcomemsg_4,WELCOMEMSG_4_LEN);
  x=16;  y=384; draw_string(welcomemsg_5,WELCOMEMSG_5_LEN);

  x=0;   y=352;
}

void log_open(void)
{
#ifdef PROTOCOL_DEBUG
  cbm_open(1,4,CBM_WRITE,"");
#endif
}

void log(const char* format, ...)
{
#ifdef PROTOCOL_DEBUG
  char lbuf[48];
  va_list args;
  va_start(args,format);
  vsprintf(lbuf,format,args);
  va_end(args);
  cbm_write(1,lbuf,strlen(lbuf));
  //  cbm_write(1,"\n",2);
#endif
}

void log_close(void)
{
#ifdef PROTOCOL_DEBUG
  cbm_close(1);
#endif
}

void main(void)
{
  static const uint8_t pal[2]={TGI_COLOR_BLACK,TGI_COLOR_ORANGE};  
  struct ser_params params = {
    SER_BAUD_19200,
    SER_BITS_8,
    SER_STOP_1,
    SER_PAR_NONE,
    SER_HS_HW
  };

  log_open();
  
  c=ser_install(&c64_swlink);

  if (c!=SER_ERR_OK)
    {
      printf("ser_install returned: %d\n",c);
      return;
    }

  y=496;
  delta_x=8;
  delta_y=16;
  dumb_terminal_active=1;
  tgi_install(tgi_static_stddrv);
  tgi_init();
  install_nmi_trampoline();
  tgi_clear();
  POKE(0xD020,0);
  tgi_setpalette(pal);
  c=ser_open(&params);
  ser_ioctl(1, NULL);  
    
  greeting();

  // And do the terminal
  for (;;)
    {
      if (ser_get(&c)==SER_ERR_OK)
	{
	  // Detect and strip IAC escapes (two consecutive bytes of 0xFF)
	  if (c==0xFF && lastc == 0xFF)
	    {
	      lastc=0x00;
	    }
	  else
	    {
	      log("%02x ",c&0x7F);
	      lastc=c;
	      decode(c&0x7F);
	    }
	}
      if (kbhit())
	{
	  send_byte(cgetc());
	}
    }
  tgi_done();
  ser_close();
  ser_uninstall();
  tgi_uninstall();
#ifdef PROTOCOL_DEBUG
  log_close();
#endif
  
}
