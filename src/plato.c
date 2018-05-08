#include <6502.h>
#include <stdio.h>
#include <tgi.h>
#include <conio.h>
#include <serial.h>
#include <peekpoke.h>
#include "font.h"
#include "scale.h"
#include "protocol.h"

unsigned char retval;
unsigned char c;

unsigned char chstr[2];
unsigned char a;
unsigned char b;
unsigned char i;
unsigned char j;
unsigned char mode;
unsigned char escape;
unsigned char decoded;
unsigned char dumb_terminal_active;
unsigned int margin=0;
unsigned int x=0;
unsigned int y=0;
unsigned int nx=0;
unsigned int ny=0;
unsigned int deltax=0;
unsigned int deltay=0;
unsigned int last_x=0;
unsigned int last_y=0;
unsigned char delta_x=0;
unsigned char delta_y=0;
unsigned char ascii_state=0;
unsigned char ascii_bytes=0;
unsigned char pmd[64];
unsigned char font_pmd=0;
unsigned char font_info=0;
unsigned char connection_active=0;
unsigned char xor_mode=0;
unsigned char character_set=0;
unsigned char vertical_writing_mode=0;
unsigned char reverse_writing_mode=0;
unsigned char bold_writing_mode=0;
unsigned char mode_words=0;
unsigned char assembler=0; 
unsigned char mar=0;
unsigned char flow_control=0;
unsigned long platofgcolor=0;
unsigned long platobgcolor=0;
unsigned int mode4start=0;

// PLATOTerm for Commodore 64
const unsigned char welcomemsg_1[]={80,76,65,84,79,84,101,114,109,32,102,111,114,32,67,111,109,109,111,100,111,114,101,32,54,52};
#define WELCOMEMSG_1_LEN 26

// Copyright 2018 IRATA.ONLINE
const unsigned char welcomemsg_2[]={67,111,112,121,114,105,103,104,116,32,40,99,41,32,0x32,0x30,0x31,0x38,0x20,73,82,65,84,65,46,79,78,76,73,78,69};
#define WELCOMEMSG_2_LEN 31

// This software is licensed under GPL 3.0.
const unsigned char welcomemsg_3[]={84,104,105,115,32,115,111,102,116,119,97,114,101,32,105,115,32,108,105,99,101,110,115,101,100,32,117,110,100,101,114,32,71,80,76,32,51,46,48,32,46};
#define WELCOMEMSG_3_LEN 40

// See COPYING for details.
const unsigned char welcomemsg_4[]={83,101,101,32,99,111,112,121,105,110,103,32,102,111,114,32,100,101,116,97,105,108,115};
#define WELCOMEMSG_4_LEN 23

// PLATOTerm READY
const unsigned char welcomemsg_5[]={80,76,65,84,79,84,101,114,109,32,82,69,65,68,89};
#define WELCOMEMSG_5_LEN 15

// The static symbol for the c64 tgi graphics driver.
extern char c64_hi;

// The static symbol for the  up2400 driver
extern char c64_up2400;

extern void install_nmi_tgi();

void send_byte(unsigned char b)
{
}

void scroll_up(void)
{
}

void draw_char(unsigned char charset_to_use, unsigned char char_to_plot)
{
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

  x=x+deltax;
}

void screen_erase(void)
{
}
void screen_erase_block(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
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

void draw_string(const char* text,unsigned char len)
{
  unsigned char* currChar=NULL;
  unsigned char counter=0;
  for (counter=0;counter<len;counter++)
    {
      decode_dumb_terminal(text[counter]);
    }
}

void draw_point(unsigned int x,unsigned int y)
{
  tgi_setpixel(scalex[x],scaley[y]);
}
void draw_line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2)
{
  tgi_line(scalex[x1],scaley[y1],scalex[x2],scaley[y2]);
}
void paint(void)
{
}
void enable_touch(int n)
{
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

void main(void)
{
  int i=0;

  struct ser_params params = {
    SER_BAUD_2400,
    SER_BITS_8,
    SER_STOP_1,
    SER_PAR_NONE,
    SER_HS_NONE
  };

  c=ser_install(&c64_up2400);

  if (c!=SER_ERR_OK)
    {
      printf("ser_install returned: %d\n",c);
      return;
    }

  y=511-16;
  deltax=8;
  deltay=16;
  dumb_terminal_active=1;
  tgi_install(&c64_hi);
  tgi_init();
  tgi_clear();

  install_nmi_tgi();
  
  c=ser_open(&params);
  ser_ioctl(1, NULL);  
    
  greeting();
  
  for(;;)
    {
      if (ser_get(&c)==SER_ERR_OK)
	{
	  decode(c);
	}
      if (kbhit())
	{
	  ser_put(cgetc());
	}
    }
  
  tgi_done();
  ser_close();
  ser_uninstall();
  tgi_uninstall();
}

