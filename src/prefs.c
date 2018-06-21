/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>
#include <tgi.h>
#include <string.h>
#include <serial.h>
#include <peekpoke.h>
#include "screen.h"
#include "prefs.h"
#include "protocol.h"
#include "terminal.h"
#include "config.h"
#include "ip65.h"

extern ConfigInfo config;

static uint8_t prefs_running;
static uint8_t prefs_need_updating;
static uint8_t ch;
static char temp_ip_address[16];
static padBool TTYSave;
static padPt TTYLocSave;
extern padBool TTY;
extern padPt TTYLoc;

/**
 * Run the preferences menu
 */
void prefs_run(void)
{
  TTYSave=TTY;
  TTYLocSave.x = TTYLoc.x;
  TTYLocSave.y = TTYLoc.y;
  TTY=true;
  prefs_running=true;
  prefs_need_updating=false;
  while (prefs_running)
    {
      switch (config.io_mode)
	{
	case IO_MODE_SERIAL:
	  prefs_serial();
	  break;
	case IO_MODE_ETHERNET:
	  prefs_ethernet();
	  break;
	}
    }
  TTY=TTYSave;
  TTYLoc.x=TTYLocSave.x;
  TTYLoc.y=TTYLocSave.y;
  prefs_done();
}

/**
 * prefs_serial()
 * Preferences menu to show for serial devices.
 */
void prefs_serial(void)
{
  prefs_display("i)nterface d)river b)aud t)ouch s)ave e)xit: ");

  ch=prefs_get_key_matching("idbtse");

  switch(ch)
    {
    case 'i':
      prefs_select("interface");
      prefs_interface();
      break;
    case 'd':
      prefs_select("driver");
      prefs_driver();
      break;
    case 'b':
      prefs_select("baud");
      prefs_baud();
      break;
    case 't':
      prefs_select("touch");
      prefs_touch();
      break;
    case 's':
      prefs_select("save");
      prefs_save();
      break;
    case 'e':
      prefs_running=false;
      break;
    }
  
}

/**
 * prefs_save(void)
 * Save preferences
 */
void prefs_save(void)
{
  prefs_display("saving preferences...");
  config_save();
  prefs_need_updating=true;
  prefs_clear();
}

/**
 * prefs_touch(void)
 * Preferences for selecting touch driver
 */
void prefs_touch(void)
{
  prefs_display("1)351 i)nkwell j)oy k)oala b)ack: ");

  ch=prefs_get_key_matching("1ijkb");

  switch(ch)
    {
    case '1':
      prefs_select("1351");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_1351);
      prefs_need_updating=true;
      break;
    case 'i':
      prefs_select("inkwell");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_INKWELL);
      prefs_need_updating=true;
      break;
    case 'j':
      prefs_select("joystick");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_JOY);
      prefs_need_updating=true;
      break;
    case 'k':
      prefs_select("koala");
      strcpy(config.driver_mou,CONFIG_MOUSE_DRIVER_POT);
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_baud(void)
 * Preferences for selecting serial baud rate
 */
void prefs_baud(void)
{
  prefs_display("3)00 1)200 2)400 9)600 q)19200 w)38400 b)ack: ");
  
  ch=prefs_get_key_matching("3129qwb");

  switch(ch)
    {
    case '3':
      prefs_select("300");
      config.baud=SER_BAUD_300;
      prefs_need_updating=true;
      break;
    case '1':
      prefs_select("1200");
      config.baud=SER_BAUD_1200;
      prefs_need_updating=true;
      break;
    case '2':
      prefs_select("2400");
      config.baud=SER_BAUD_2400;
      prefs_need_updating=true;
      break;
    case '9':
      prefs_select("9600");
      config.baud=SER_BAUD_9600;
      prefs_need_updating=true;
      break;
    case 'q':
      prefs_select("19200");
      config.baud=SER_BAUD_19200;
      prefs_need_updating=true;
      break;
    case 'w':
      prefs_select("38400");
      config.baud=SER_BAUD_38400;
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_driver(void)
 * Preferences menu to select serial driver
 */
void prefs_driver(void)
{
  prefs_display("driver - s)wiftlink232 u)serport2400 b)ack: ");
  
  ch=prefs_get_key_matching("sub");

  switch(ch)
    {
    case 's':
      prefs_select("swiftlink232");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_SWIFTLINK);
      prefs_need_updating=true;
      break;
    case 'u':
      prefs_select("userport2400");
      strcpy(config.driver_ser,CONFIG_SERIAL_DRIVER_UP2400);
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_interface(void)
 * Preferences menu to select interface.
 */
void prefs_interface(void)
{
  prefs_display("interface - e)thernet s)erial b)ack: ");

  ch=prefs_get_key_matching("esb");

  switch(ch)
    {
    case 'e':
      prefs_select("ethernet");
      config.io_mode=IO_MODE_ETHERNET;
      prefs_need_updating=true;
      break;
    case 's':
      prefs_select("serial");
      config.io_mode=IO_MODE_SERIAL;
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_dhcp(void)
 * Preferences menu to enable/disable dhcp
 */
void prefs_dhcp(void)
{
  prefs_display("dhcp - y)es n)o b)ack: ");

  ch=prefs_get_key_matching("ynb");

  switch(ch)
    {
    case 'y':
      prefs_select("yes");
      config.use_dhcp=true;
      prefs_need_updating=true;
      break;
    case 'n':
      prefs_select("no");
      config.use_dhcp=false;
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_get_address()
 * get string with ip address numbers, terminated by return.
 */
void prefs_get_address(char* address)
{
  unsigned char i=0;
  
  memset(&address,0,sizeof(address));
  ch=0;

  while (ch != '\r')
    {
      ch=prefs_get_key_matching1("0123456789.");
      ShowPLATO(&ch,1);
    }
  
  /* while (ch != '\n') */
  /*   { */
  /*     POKE(0xD020,i&0x0f); */
  /*     ch=prefs_get_key_matching("0123456789.\n\b"); */
  /*     if (ch=='\b' && i>0) */
  /* 	{ */
  /* 	  --i; */
  /* 	  address[i]=0; */
  /* 	} */
  /*     else */
  /* 	{ */
  /* 	  ++i; */
  /* 	  address[i]=ch; */
  /* 	  ShowPLATO(&ch,1); */
  /* 	} */
  /*   } */
}

/**
 * prefs_ip(void)
 * Preferences menu for IP address
 */
void prefs_ip(void)
{
  prefs_display("ip address (x.x.x.x) or return for none: ");
  prefs_get_address(temp_ip_address);
  config.ip_address = parse_dotted_quad(temp_ip_address);
  prefs_select(" ok");
  prefs_need_updating=true;
}

/**
 * prefs_ethernet(void)
 * Preferences menu to show for ethernet devices.
 */
void prefs_ethernet(void)
{
  prefs_display("i)nterface d)hcp p)ip n)etmask g)ateway w)dns s)save e)xit: ");

  ch=prefs_get_key_matching("idpngwse");

  switch(ch)
    {
    case 'i':
      prefs_select("interface");
      prefs_interface();
      break;
    case 'd':
      prefs_select("dhcp");
      prefs_dhcp();
      break;
    case 'p':
      prefs_select("ip");
      prefs_ip();
      break;
    case 'n':
      break;
    case 'g':
      break;
    case 'w':
      break;
    case 's':
      break;
    case 'e':
      prefs_running=false;
      break;
    }
}

/**
 * prefs_display(text)
 * Display a line of the preferences menu
 */
void prefs_display(const char* text)
{
  uint8_t c;

  c=tgi_getcolor();

  tgi_setcolor(TGI_COLOR_WHITE);

  ShowPLATO((unsigned char*)text, strlen(text));
  
  tgi_setcolor(c);
}

/**
 * Wait for a key matching input, return it.
 */
unsigned char prefs_get_key_matching(const char* matches)
{
  unsigned char ch;
  unsigned char i;
  
  while (true)
    {
      ch=cgetc();
      for (i=0;i<strlen(matches);++i)
	{
	  if (ch==matches[i])
	    return ch;
	}
    }
}

/**
 * TEMPORARY: Wait for a key matching input, return it.
 */
unsigned char prefs_get_key_matching1(const char* matches)
{
  unsigned char ch;
  unsigned char i;
  
  for (;;)
    {
      ch=cgetc();

      if ((ch==0x14) || (ch==0x0d))
	return ch;
      
      for (i=0;i<strlen(matches);++i)
	{
	  return ch;
	}
    }
}

/**
 * erase prefs bar
 */
void prefs_clear(void)
{
  uint8_t c;
  c=tgi_getcolor();
  tgi_setcolor(TGI_COLOR_BLACK);
  tgi_bar(0,185,319,191);
  tgi_setcolor(c);
  ShowPLATO("\n\v",2);
}

/**
 * indicate selection, display it, and wait a bit for visual confirmation.
 */
void prefs_select(const char* text)
{
  unsigned char i=0;
  ShowPLATO((unsigned char *)text,strlen(text));
  
  for (i=0;i<100;i++)
    {
      waitvsync();
    }

  prefs_clear();

}

/**
 * close prefs. 
 */
void prefs_done(void)
{
  prefs_clear();
}
