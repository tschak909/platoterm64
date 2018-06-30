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
#include <mouse.h>
#include "io.h"
#include "screen.h"
#include "keyboard.h"
#include "prefs.h"
#include "protocol.h"
#include "terminal.h"
#include "config.h"
#include "ip65.h"

extern ConfigInfo config;

static uint8_t prefs_running;
static uint8_t prefs_need_updating;
static uint8_t ch;
static padBool TTYSave;
static padPt TTYLocSave;
extern padBool TTY;
extern padPt TTYLoc;

static char temp_ip_address[17];

/**
 * Run the preferences menu
 */
void prefs_run(void)
{
  keyboard_clear();
  TTYSave=TTY;
  TTYLocSave.x = TTYLoc.x;
  TTYLocSave.y = TTYLoc.y;
  TTY=true;
  prefs_running=true;
  prefs_need_updating=false;
  prefs_clear();
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

  if (prefs_need_updating)
    {
      prefs_update();
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
  prefs_select("ok");
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
void prefs_get_address(void)
{
  unsigned char strp=0;
  
  ch=0;

  while (ch != 0x0d)
    {
      ch=prefs_get_key_matching1("0123456789.");
      if (ch==0x08) /* was translated from 0x14 to 0x08 */
  	{
	  if (strp>0)
	    {
	      --strp;
	      temp_ip_address[strp]=0;
	      ShowPLATO(&ch,1);
	    }
  	}
      else if (ch==0x0d)
	{
	  // Don't append or show the CR
	}
      else
  	{
  	  temp_ip_address[strp]=ch;
  	  ShowPLATO(&ch,1);
	  ++strp;	  
  	}
    }
}

/**
 * prefs_ip(void)
 * Preferences menu for IP address
 */
void prefs_ip(void)
{
  prefs_display("ip address (x.x.x.x) or return for none: ");
  prefs_get_address();
  config.ip_address = parse_dotted_quad(temp_ip_address);
  prefs_select(" ok");
  prefs_need_updating=true;
}

/**
 * prefs_dns(void)
 * Preferences menu for dns
 */
void prefs_dns(void)
{
  prefs_display("dns (x.x.x.x) or return for none: ");
  prefs_get_address();
  config.dns = parse_dotted_quad(temp_ip_address);
  prefs_select(" ok");
  prefs_need_updating=true;
}

/**
 * prefs_netmask(void)
 * Preferences menu for netmask
 */
void prefs_netmask(void)
{
  prefs_display("netmask (x.x.x.x) or return for none: ");
  prefs_get_address();
  config.netmask = parse_dotted_quad(temp_ip_address);
  prefs_select(" ok");
  prefs_need_updating=true;
}

/**
 * prefs_ip(void)
 * Preferences menu for IP address
 */
void prefs_gateway(void)
{
  prefs_display("gateway (x.x.x.x) or return for none: ");
  prefs_get_address();
  config.gateway = parse_dotted_quad(temp_ip_address);
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
      prefs_select("netmask");
      prefs_netmask();
      break;
    case 'g':
      prefs_select("gateway");
      prefs_gateway();
      break;
    case 'w':
      prefs_select("dns");
      prefs_dns();
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
 * prefs_display(text)
 * Display a line of the preferences menu
 */
void prefs_display(const char* text)
{
  uint8_t c;

  TTYLoc.x=0;
  TTYLoc.y=0;
  
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
  unsigned char ch=0;
  unsigned char i;
  
  for (;;)
    {
      ch=cgetc();

      if (ch==0x0d)
	return 0x0d;
      else if (ch==0x14)
	return 0x08; /* convert PETSCII DEL to ASCII BS */
      
      for (i=0;i<strlen(matches);++i)
	{
	  if (ch==matches[i])
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
 * Update program state with selected preferences
 */
void prefs_update(void)
{
  unsigned char retv;
  
  // Close any serial drivers.
  prefs_clear();
  prefs_display("closing serial driver...");
  ser_close();
  prefs_clear();
  prefs_display("unloading serial driver...");
  ser_unload();
  prefs_clear();

  // Close any touch drivers
  prefs_display("unloading touch driver...");
  mouse_unload();
  prefs_clear();
  
  if (config.io_mode == IO_MODE_SERIAL)
    {
      prefs_display("loading serial driver...");
      io_open();
    }
  else if (config.io_mode == IO_MODE_ETHERNET)
    {
      // Come back here and implement ethernet specific stuff
      prefs_display("ethernet not implemented, yet.");
      prefs_select("");
    }

  io_open();
  
  prefs_display("loading touch driver...");
  retv = mouse_load_driver(&mouse_def_callbacks,config.driver_mou);
  if (retv==MOUSE_ERR_OK)
    {
      prefs_select("ok");
      mouse_show();
    }
  else
    {
      prefs_select("error");
    }
}

/**
 * close prefs. 
 */
void prefs_done(void)
{
  TTY=TTYSave;
  prefs_clear();
  TTYLoc.x=TTYLocSave.x;
  TTYLoc.y=TTYLocSave.y;
}
