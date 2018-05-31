/**
 * protocol.c - PLATO Protocol logic
 *
 * author: Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Started: 2018-03-27
 *
 */

#define PROTOCOL_DEBUG 1

#ifdef PROTOCOL_DEBUG
#include <stdio.h>
#endif

#include <string.h>
#include <peekpoke.h>
#include "protocol.h"

#define FALSE 0
#define TRUE 1

#define ASCII_STATE_NONE 0
#define ASCII_STATE_PNI_RS 1
#define ASCII_STATE_PLATO_META_DATA 2
#define ASCII_STATE_LOAD_COORDINATES 3
#define ASCII_STATE_SSF 4
#define ASCII_STATE_LOAD_EXTERNAL 5
#define ASCII_STATE_LOAD_ADDRESS 6
#define ASCII_STATE_LOAD_ECHO 7
#define ASCII_STATE_FG 8
#define ASCII_STATE_BG 9
#define ASCII_STATE_PAINT 10
#define ASCII_STATE_GSFG 11

#define ASCII_NUL 0x00
#define ASCII_ESCAPE 0x1B
#define ASCII_STX 0x02
#define ASCII_ETX 0x03
#define ASCII_CR 0x0D
#define ASCII_LF 0x0A
#define ASCII_FF 0x0C
#define ASCII_SYN 0x16
#define ASCII_DC1  0x11
#define ASCII_DC2  0x12
#define ASCII_DC3  0x13
#define ASCII_DC4  0x14
#define ASCII_2  0x32
#define ASCII_AT  0x40
#define ASCII_A  0x41
#define ASCII_B  0x42
#define ASCII_C  0x43
#define ASCII_D  0x44
#define ASCII_E  0x45
#define ASCII_F  0x46
#define ASCII_G  0x47
#define ASCII_H  0x48
#define ASCII_I  0x49
#define ASCII_J  0x4A
#define ASCII_K  0x4B
#define ASCII_L  0x4C
#define ASCII_M  0x4D
#define ASCII_N  0x4E
#define ASCII_O  0x4F
#define ASCII_P  0x50
#define ASCII_Q  0x51
#define ASCII_R  0x52
#define ASCII_S  0x53
#define ASCII_T  0x54
#define ASCII_U  0x55
#define ASCII_V  0x56
#define ASCII_W  0x57
#define ASCII_X  0x58
#define ASCII_Y  0x59
#define ASCII_Z  0x5A
#define ASCII_a  0x61
#define ASCII_b  0x62
#define ASCII_c  0x63
#define ASCII_g  0x67
#define ASCII_BACKSPACE  0x08
#define ASCII_TAB  0x09
#define ASCII_VT  0x0B
#define ASCII_EM  0x19
#define ASCII_FS  0x1C
#define ASCII_GS  0x1D
#define ASCII_RS  0x1E
#define ASCII_US  0x1F
#define ASCII_SPACE  0x20
#define ASCTYPE 12
#define ASCFEATURES 0x08
#define ASCII_XOFF ASCII_DC1
#define ASCII_XON ASCII_DC3

extern unsigned short mode;
extern unsigned char escape;
extern unsigned char dumb_terminal_active;
extern unsigned int margin;
extern unsigned short x;
extern unsigned short y;
extern unsigned short last_x;
extern unsigned short last_y;
extern unsigned char delta_x;
extern unsigned char delta_y;
extern unsigned char ascii_state;
extern unsigned char ascii_bytes;
extern unsigned char pmd[256];
extern unsigned char font_pmd;
extern unsigned char font_info;
extern unsigned char connection_active;
extern unsigned char xor_mode;
extern unsigned char character_set;
extern unsigned char vertical_writing_mode;
extern unsigned char reverse_writing_mode;
extern unsigned char bold_writing_mode;
extern unsigned char mode_words;
extern unsigned long assembler; 
extern unsigned short mar;
extern unsigned char flow_control;
extern unsigned long platofgcolor;
extern unsigned long platobgcolor;
extern unsigned int mode4start;
extern unsigned short plato_m23[128*8];
extern unsigned char special_mode;
extern unsigned char special_mode_param;

extern void send_byte(unsigned char b);
extern void scroll_up(void);
extern void draw_char(unsigned char charset_to_use, unsigned char char_to_plot);
extern void screen_erase(void);
extern void screen_erase_block(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
extern void screen_sleep(void);
extern void screen_backspace(void);
extern void screen_forwardspace(void);
extern void beep(void);
extern void draw_point(unsigned int x,unsigned int y);
extern void draw_line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2);
extern void paint(void);
extern void enable_touch(int n);

short lastMode=0xFF;

static const unsigned char asciiM0[] = 
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x2d, 0x38, 0x37, 0x80, 0x2b, 0x33, 0xab, 0x36,
  0x29, 0x2a, 0x27, 0x25, 0x2e, 0x26, 0x2f, 0x28,
  0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22,
  0x23, 0x24, 0x00, 0x39, 0x3a, 0x2c, 0x3b, 0x3d,
  0xbd, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x31, 0xbe, 0x32, 0x9d, 0x3c,
  0x9f, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0xa9, 0xae, 0xaa, 0xa4, 0xff
};
static const unsigned char asciiM1[] = 
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x2d, 0x28, 0xb0, 0x9b, 0x35, 0xac, 0xa0, 0xa1,
  0xa2, 0xa3, 0x34, 0xa5, 0xa6, 0xa7, 0xa8, 0x30,
  0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
  0xb9, 0xba, 0xbb, 0xbc, 0xc0, 0xaf, 0xc1, 0x3e,
  0xc2, 0x9c, 0xc3, 0xc8, 0xc4, 0xc5, 0x9e, 0xc9,
  0xc6, 0xc7, 0xae, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const unsigned char asciiKeycodes[] =
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x26, 0x60, 0x0a, 0x5e, 0x2b, 0x2d,
  0x13, 0x04, 0x07, 0x08, 0x7b, 0x0b, 0x0d, 0x1a,
  0x02, 0x12, 0x01, 0x03, 0x7d, 0x0c, 0xff, 0xff,
  0x3c, 0x3e, 0x5b, 0x5d, 0x24, 0x25, 0x5f, 0x7c,
  0x2a, 0x28, 0x40, 0x27, 0x1c, 0x5c, 0x23, 0x7e,
  0x17, 0x05, 0x14, 0x19, 0x7f, 0x09, 0x1e, 0x18,
  0x0e, 0x1d, 0x11, 0x16, 0x00, 0x0f, 0xff, 0xff,
  0x20, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x3d, 0x3b, 0x2f, 0x2e, 0x2c,
  0x1f, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x29, 0x3a, 0x3f, 0x21, 0x22
};

// Conversion from ascii mode codes to classic codes
static const unsigned char ascii_mode[] = { 0, 3, 2, 1 };

/**
 * decode(unsigned char b) The primary entry point for protocol decoding.
 * Input is a byte received from PLATO
 * The protocol then calls other parts of the program to do
 * What is asked by the protocol.
 */
void decode(unsigned char b)
{
  if (dumb_terminal_active==TRUE)
    {
      decode_dumb_terminal(b);
    }
  else
    {
      decode_plato(b);
    }
}

/**
 * decode_dumb_terminal(unsigned char b) - When terminal is in dumb terminal mode, go here.
 */
void decode_dumb_terminal(char b)
{
  if (b==ASCII_ESCAPE)
    {
      escape=TRUE;
      
    }
  else if (b==ASCII_STX)
    {
      if (escape==FALSE)
	{
	  // STX found without corresponding escape, ignore.
	  
	}
      else
	{
	  // Proper STX sequence, set PLATO mode.
	  dumb_terminal_active=FALSE;
	  mode=(3<<2)+1;
	  escape=FALSE;
	  
	}
    }
  else if (b==ASCII_ETX)
    {
      if (escape==FALSE)
	{
	  // Improper ETX without corresponding esc, ignore.
	}
      else
	{
	  // ETX detected in dumb terminal mode, ignore.
	  
	}
    }
  else if (b==ASCII_CR)
    {
      x=margin;
      
    }
  else if (b==ASCII_LF)
    {
      if (y != 0)
	{
	  y = y - 16;
	}
      else
	{
	  scroll_up();
	}
      
    }
  else if (b>=32 && b<127) // Printable char
    {
      unsigned char char_to_plot = asciiM0[b];
      if (char_to_plot != 0xFF)
	{
	  unsigned char charset_to_use = (char_to_plot & 0x80) >> 7;
	  char_to_plot &= 0x7F; // Lower 7 bits.
	  
	  draw_char(charset_to_use,char_to_plot);
	}
    }
}

/**
 * decode_plato(unsigned char b) - Decode next PLATO Character
 * This is the meat of protocol decoding.
 */
void decode_plato(unsigned char b)
{
  if (b==ASCII_ESCAPE)
    {
      escape=TRUE;
      
    }
  else if (ascii_state == ASCII_STATE_PNI_RS)
    {
      process_pni_rs(); // This is void because we basically ignore it.
    }
  else if (ascii_state == ASCII_STATE_PLATO_META_DATA)
    {
      process_plato_metadata(b);
    }
  else if (escape==TRUE)
    {
      escape=FALSE;
      process_escape_sequence(b);
    }
  else
    {
      process_control_characters(b);
      process_other_states(b);
    }
  
  lastMode=mode;
}

/**
 * process_pni_rs(void) - Process PNI RS sequences, for now
 * ignore them.
 */
void process_pni_rs(void)
{
  // Ignore the next three bytes.
  if (++ascii_bytes==3)
    {
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
    }  
}

/**
 * process_plato_metadata(unsigned char b)
 * Process a byte of PLATO metadata.
 */
void process_plato_metadata(unsigned char b)
{
  unsigned long n = assemble_ascii_plato_metadata(b);
  if (n==0)
    {
      // Completed metadata, but just ignore it.
    }
  
}

/**
 * process_escape_sequence(unsigned char b)
 * Process the next escape sequence
 */
void process_escape_sequence(unsigned char b)
{
  switch(b)
    {
    case ASCII_STX:
      // STX called while still in PLATO mode. Ignore.
      dumb_terminal_active=FALSE;
      break;
    case ASCII_ETX:
      // ETX ends PLATO mode and returns to dumb terminal mode.
      dumb_terminal_active=TRUE;
      x=0;
      y=496;
      break;
    case ASCII_FF:
      // Erase screen
      screen_erase();
      break;
    case ASCII_SYN:
      // Set XOR writing mode
      mode=(mode & 3) + 2;
      xor_mode=TRUE;
    case ASCII_DC1:
    case ASCII_DC2:
    case ASCII_DC3:
    case ASCII_DC4:
      // Inverse, write, erase, rewrite
      xor_mode=FALSE;

      mode=(mode & ~3)+ascii_mode[b-ASCII_DC1];
      break;
    case ASCII_2:
      // Load coordinate
      ascii_state=ASCII_STATE_LOAD_COORDINATES;
      ascii_bytes=0;
      
      break;
    case ASCII_AT:
      // Superscript
      // TODO: IMPLEMENT SUPERSCRIPT
      
      break;
    case ASCII_A:
      // Subscript
      // TODO: IMPLEMENT SUBSCRIPT
      
      break;
    case ASCII_B:
    case ASCII_C:
    case ASCII_D:
    case ASCII_E:
    case ASCII_F:
    case ASCII_G:
    case ASCII_H:
    case ASCII_I:
      // Select current character set memory
      character_set=b-ASCII_B;
      
      break;
    case ASCII_J:
      // Enable Horizontal Writing Mode
      vertical_writing_mode=FALSE;
      
      break;
    case ASCII_K:
      // Enable Vertical Writing Mode
      vertical_writing_mode=TRUE;
      
      break;
    case ASCII_L:
      // Forward writing mode (LTR)
      reverse_writing_mode=FALSE;
      
      break;
    case ASCII_M:
      // Reverse writing mode (RTL)
      reverse_writing_mode=TRUE;
      
      break;
    case ASCII_N:
      // Normal Size Writing Mode
      bold_writing_mode=FALSE;
      
      break;
    case ASCII_O:
      // Bold size writing mode
      bold_writing_mode=TRUE;
      
      break;
    case ASCII_P:
      // Load Character memory
      xor_mode=FALSE;
      mode=(mode & 3)+(2 << 2);
      break;
    case ASCII_Q:
      // SSF
      ascii_state=ASCII_STATE_SSF;
      ascii_bytes=0;
      
      break;
    case ASCII_R:
      // Load EXTERNAL
      ascii_state=ASCII_STATE_LOAD_EXTERNAL;
      ascii_bytes=0;
      
      break;
    case ASCII_S:
      // LOAD Memory (Mode 2 data)
      xor_mode=FALSE;
      mode=(mode & 3)+(2 << 2);
      
      break;
    case ASCII_T:
      // MODE 5 (not used, but must be decoded)
      xor_mode=FALSE;
      mode=(mode & 3)+(5 << 2);
      
      break;
    case ASCII_U:
      // MODE 6 (not used, but must be decoded)
      xor_mode=FALSE;
      mode=(mode & 3)+(6 << 2);
      
      break;
    case ASCII_V:
      // Mode 7 (not used, but must be decoded)
      xor_mode=FALSE;
      mode=(mode & 3)+(7 << 2);
      
      break;
    case ASCII_W:
      // Load memory Address
      ascii_state=ASCII_STATE_LOAD_ADDRESS;
      ascii_bytes=0;
      
      break;
    case ASCII_X:
      // Start PLATO Metadata
      ascii_state=ASCII_STATE_PLATO_META_DATA;
      ascii_bytes=0;
      
    case ASCII_Y:
      // Start LOAD ECHO
      ascii_state=ASCII_STATE_LOAD_ECHO;
      ascii_bytes=0;
      
    case ASCII_Z:
      // Set X margin
      margin=x;
      break;
    case ASCII_a:
      // Start Foreground color
      ascii_state=ASCII_STATE_FG;
      ascii_bytes=0;
      
      break;
    case ASCII_b:
      // Start Background color
      ascii_state=ASCII_STATE_BG;
      ascii_bytes=0;
      
      break;
    case ASCII_c:
      // Start Paint
      ascii_state=ASCII_STATE_PAINT;
      ascii_bytes=0;
      
      break;
    case ASCII_g:
      // Start greyscale foreground color
      ascii_state=ASCII_STATE_GSFG;
      ascii_bytes=0;
      
      break;
    }
}

/**
 * Process an ASCII control character b, not part of escape sequence.
 */
void process_control_characters(unsigned char b)
{
  switch(b)
    {
    case ASCII_NUL:
      // Sleep for 8 milliseconds
      screen_sleep();
      
      break;
    case ASCII_BACKSPACE:
      // Go back one character
      x=(x-delta_x)&0x1FF;
      
      break;
    case ASCII_TAB:
      x=(x+delta_x)&0x1FF;
      
      break;
    case ASCII_LF:
      y=(y-delta_y)&0x1FF;
      
      break;
    case ASCII_VT:
      y=(y+delta_y)&0x1FF;
      
      break;
    case ASCII_FF:
      x=0;
      y=511;
      
      break;
    case ASCII_CR:
      x=margin;
      y=(y-delta_y)&0x1FF;
      
      break;
    case ASCII_EM:
      // Block erase
      mode=(mode&3)+(4<<2);
      mode_words=0;
      
      break;
    case ASCII_FS:
      // Point plot
      mode=(mode&3)+(0 << 2);
      break;
    case ASCII_GS:
      // Draw Line
      mode=(mode&3)+(1<<2);
      ascii_state=ASCII_STATE_LOAD_COORDINATES;
      ascii_bytes=0;
      
      break;
    case ASCII_RS:
      // PNI_RS START DOWNLOAD. IGNORE NEXT THREE COMMANDS.
      ascii_state=ASCII_STATE_PNI_RS;
      
      break;
    case ASCII_US:
      // alpha mode (text)

      mode=(mode&3)+(3<<2);
      break;
    }
}

/**
 * process_other_states(unsigned char b) Process the rest of the possible protocol states.
 */
void process_other_states(unsigned char b)
{
  unsigned long n;
  if (b >= ASCII_SPACE)
    {
      switch(ascii_state)
	{
	case ASCII_STATE_LOAD_COORDINATES:
	  if (assemble_coordinate(b))
	    {
	      x=last_x;
	      y=last_y;
	    }
	  
	  break;
	case ASCII_STATE_PAINT:
	  n=assemble_paint(b);
	  if (n != -1)
	    {
	      paint();
	    }
	  
	  break;
	case ASCII_STATE_LOAD_ECHO:
	  n=assemble_data(b);
	  if (n != -1)
	    {
	      n &= 0x7F;
	      send_echo_request(n);
	    }
	  
	  break;
	case ASCII_STATE_LOAD_ADDRESS:
	  n=assemble_data(b);
	  if (n!=1)
	    {
	      mar=n&0x7FFF;
	    }
	  break;
	  
	case ASCII_STATE_LOAD_EXTERNAL:
	  n=assemble_data(b);
	  process_ext(n);
	  break;
	case ASCII_STATE_SSF:
	  n=assemble_data(b);
	  if (n != -1)
	    {
	      enable_touch((n & 0x20) != 0);
	    }
	  
	  break;
	case ASCII_STATE_FG:
	case ASCII_STATE_BG:
	  n = assemble_color(b);
	  process_color((long)n);
	  
	  break;
	case ASCII_STATE_GSFG:
	  n= assemble_grayscale(b);
	  process_grayscale(n);
	  
	  break;
	case ASCII_STATE_PLATO_META_DATA:
	   // Handled above.
	  break;
	case ASCII_STATE_PNI_RS:
	   // Ignored.
	  break;
	case ASCII_STATE_NONE:
	  process_modes(b);
	  break;
	}
    }
}

/**
 * process_ext(unsigned int n) - Process EXT packets
 */
unsigned int process_ext(unsigned int n)
{
  // TODO
  return n;
}

/**
 * assemble_ascii_plato_metadata(unsigned char b) Assemble next byte of PLATO metadata.
 */
unsigned int assemble_ascii_plato_metadata(unsigned char b)
{
  unsigned char ob = b;

  b &= 0x3F;
  if (ascii_bytes == 0)
    {
      memset(&pmd,0,sizeof(pmd));
    }
  ascii_bytes++;
  if (ob == 'F' && ascii_bytes == 1)
    {
      // Font PMD
      font_pmd=TRUE;
    }
  else if (ob == 'f' && ascii_bytes == 1)
    {
      // Font info PMD
      font_info=TRUE;
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
      send_ext(0);  // send 0 width and height.
      send_ext(0);
    }
  else if (ob == 'o' && ascii_bytes == 1)
    {
      // Send 3 external keys, OS, major version, minor version
      send_ext(0);
      send_ext(0);
      send_ext(0);
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
    }
  else if (font_pmd && ascii_bytes == 2)
    {
      // Set font face and family, ignored.
      if (b==0)
	{
	  ascii_bytes=0;
	  ascii_state=ASCII_STATE_NONE;
	  return 0;
	}
    }
  else if (font_pmd && ascii_bytes == 3)
    {
      // Set font size, ignored.
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
      return 0;
    }
  else
    {
      // PLATO Metadata body decode
      // Since we don't use it, doesn't matter.
    }
  return -1;  
}

/**
 * assemble_coordinate(unsigned char b) - Get next part of coordinate data
 * and assemble towards a completed coordinate.
 */
unsigned int assemble_coordinate(unsigned char b)
{
  int coordinate = b & 037;  // Mask off top 3 bits.
  switch(b >> 5) // get control bits 6 and 7 to determine what part of coordinate to parse
    {
    case 1:   // High X or High Y
      if (ascii_bytes==0)
	{
	  // High Y coordinate
	  last_y = (last_y & 0x1F) | (coordinate << 5);
	  ascii_bytes=2;
	}
      else
	{
	  // High X coordinate
	  last_x = (last_x & 0x1F) | (coordinate << 5);
	}
      break;
    case 2:  // Low X
      last_x = (last_x & 0740) | coordinate;
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
      return TRUE;
    case 3:  // Low Y
      last_y = (last_y & 0x1E0) | coordinate;
      ascii_bytes = 2;
      break;
    }
  return FALSE;
}

/**
 * assemble_paint(unsigned char b) - Get next part of paint data
 * and assemble towards a completed paint request.
 */
unsigned int assemble_paint(unsigned char b)
{
  if (ascii_bytes == 0)
    assembler=0;

  assembler |= ((b & 0x3F) << (ascii_bytes * 6));
  if (++ascii_bytes==2)
    {
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
      return assembler;
    }
  else
    {
      // Still assembling the paint request.
    }
  return -1;
}

/**
 * assemble_data(unsigned char b) - Assemble the body of a data packet.
 */
unsigned long assemble_data(unsigned char b)
{
  if (ascii_bytes==0)
    {
      assembler=0;
    }

  assembler |= ((b & 077) << (ascii_bytes * 6));
  if (++ascii_bytes==3)
    {
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
      return assembler;
    }
  else
    {
      // Still assembling
    }
  return -1;
}

/**
 * assemble_color(unsigned char b) - Assemble the next color byte
 */
unsigned long assemble_color(unsigned char b)
{
  if (ascii_bytes == 0) {
    assembler = 0;
  }
  assembler |= ((b & 077) << (ascii_bytes * 6));
  if (++ascii_bytes == 4) {
    return assembler;
  } 
  return -1;
}

/**
 * assemble_grayscale(unsigned char b)
 */
unsigned long assemble_grayscale(unsigned char b)
{
  if (ascii_bytes == 0) {
    assembler = 0;
  }
  assembler = (b & 0x3F) << 2;
  if (++ascii_bytes == 1) {
    return assembler;
  } 
  return -1;
}

/**
 * send_ext(unsigned int key) - Send EXT sequence
 */
void send_ext(unsigned int key)
{
  unsigned char data[3];
  int i;
  
  if (connection_active==FALSE)
    {
      // Connection isn't active, ignore and return.
      return;
    }

  // Send external key
  data[0] = 0x1B; // ESC
  data[1] = 0x40 | (key & 0x3F);
  data[2] = 0x68 | ((key >> 6) & 0x03);
  for (i=0; i<3; i++)
    {
      send_byte(data[i]);
    }
}

/**
 * send_echo_request(unsigned int n) - Send an echo request back to PLATO
 */
void send_echo_request(unsigned int n)
{
  switch(n)
    {
    case 0x70:
      n = 0x70 + ASCTYPE;
      break;
    case 0x71:
      n = 1;
      break;
    case 0x72:
      n=0;
      break;
    case 0x73:
      n=0x40;
      break;
    case 0x7b:
      beep();
      break;
    case 0x7d:
      n=mar;
      break;
    case 0x52:
      flow_control=FALSE;
      /* flow_control=TRUE; */
      /* n=0x53; */
      n=0x52;
      break;
    case 0x60:
      n += ASCFEATURES;
      break;
    }

  if (n==0x7B)
    {
      // Beep does not send a reply
      return;
    }

  n += 0x80;
  send_processed_key(n);
  
}

/**
 * send_processed_key(unsigned int n) - Send a key to PLATO
 */
void send_processed_key(unsigned int n)
{
  unsigned char data[5];
  unsigned char length;
  unsigned char i;
  
  /* if (connection_active==FALSE) */
  /*   { */
  /*     // Connection isn't there anymore, don't send. */
  /*     return; */
  /*   } */
  
  if (n<0x80)
    {
      // Regular key code
      n = asciiKeycodes[n];
      if (n==0xff)
	{
	  return;
	}
      if (flow_control==TRUE)
	{
	  switch(n)
	    {
	    case 0x00: // ACCESS
	      n=0x1d;
	      length=2;
	      break;
	    case 0x05: // SHIFT-SUB
	      n=0x04;
	      length=2;
	      break;
	    case 0x0a: // TAB
	      n=0x09;
	      break;
	    case 0x11: // SHIFT_STOP
	      n=0x05;
	      break;
	    case 0x17: // SHIFT-SUPER
	      length=2;
	      // Fall through
	    case 0x7C:
	      n=0x27;
	      break;
	    case 0x27:
	      n=0x7C;
	      break;
	    }
	  data[0]=0x1B; // Store ESC for two byte codes.
	}
      data[length-1]=n;
      for (i=0;i<length;i++)
	{
	  send_byte(data[i]);
	}
    }
  else if (dumb_terminal_active==FALSE)
    {
      length = 3;
      data[0] = 0x1B;
      data[1] = (0x40 + (n & 0x3F));
      data[2] = (0x60 + (n >> 6));
      for (i = 0; i < length; i++) {
	send_byte(data[i]);
      }
    }
  else
    {
      data[0] = n >> 7;
      data[1] = 0x80 | n;
      length = 2;
      for (i = 0; i < length; i++)
	{
	  send_byte(data[i]);
	}
    }
}

/**
 * process_color(unsigned long n) - Process a color
 */
void process_color(unsigned long n)
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  if (n != 1)
    {
      r = (long)(n >> 16) & 0xff;
      g = (long)(n >> 8) & 0xff;
      b = (long)n & 0xff;
      if (ascii_state == ASCII_STATE_FG)
	{
#ifdef PROTOCOL_DEBUG
	  printf("fg: %u %u %u\n",r,g,b);
#endif
	}
      else if (ascii_state == ASCII_STATE_BG)
	{
#ifdef PROTOCOL_DEBUG
	  printf("bg: %u %u %u\n",r,g,b);
#endif
	}
    }
  
  if (ascii_bytes == 4)
    {
      ascii_bytes=0;
      ascii_state=ASCII_STATE_NONE;
    }
}

/**
 * process_grayscale(unsigned long n) - Process Grayscale
 */
void process_grayscale(unsigned long n)
{
  if (n != 1)
    {
      unsigned short a = 0xFF;
      unsigned short r = (n & 0xFF);
      unsigned short g = (n & 0xFF);
      unsigned short b = (n & 0xFF);
      unsigned long c = 1; // for now.
      if (ascii_state == ASCII_STATE_GSFG)
	{
	  platofgcolor=c;
	}
    }
}

/**
 * process_modes(unsigned int b) - Process the current mode
 */
void process_modes(unsigned int b)
{
  switch(mode >> 2)
    {
    case 0: // Dot mode
      if (assemble_coordinate(b))
	{
	  mode0((last_x << 9)+last_y);
	  
	}
      break;
    case 1: // Line mode
      if (assemble_coordinate(b))
	{
	  mode1((last_x << 9)+last_y);
	  
	}
      break;
    case 2: // Load Memory (Character sets)
      
    case 3: // Text mode
      mode3(b);
      
      break;
    case 4: // Block Erase mode
      if (assemble_coordinate(b))
	{
	  mode_words++;
	  mode4((last_x<<9)+last_y);
	}
      
      break;
    case 5: // Micro Tutor mode 5
    case 6: // Micro Tutor mode 6
    case 7: // Micro Tutor mode 7
      break;
    }
}

/**
 * mode0(unsigned int n) - Mode 0, dot mode.
 */
void mode0(unsigned long n)
{
  int nx, ny;
  nx = (n >> 9) & 0x1FF;
  ny = n & 0x1FF;
  draw_point(nx,ny);
  x=nx;
  y=ny;
}

/**
 * mode1(unsigned long n) - Mode 1, line mode
 */
void mode1(unsigned long n)
{
  int nx, ny;
  nx = (n >> 9) & 0x1FF;
  ny = n & 0x1FF;
  draw_line(x,y,nx,ny);
  x=nx;
  y=ny;  
}

/**
 * mode2(unsigned int n) - Mode 2 - Memory load
 */
void mode2(unsigned long n)
{
  int chaddr;

  chaddr = mar - 0x3800;

  if (chaddr < 0 || chaddr > 127*16)
    {
      // Out of bounds. Ignore.
    }
  else
    {
      chaddr /= 2;
      if (((n >> 16) & 3) == 0)
	{
	  // Load character data
	  plato_m23[chaddr] = n & 0xffff;
	  ++chaddr;
	}
    }
  mar += 2;
}

/**
 * mode3(unsigned int n) - Mode 3 - alpha data word.
 */
void mode3(unsigned char n)
{
  ascii_state=ASCII_STATE_NONE;
  ascii_bytes=0;
  if (character_set == 0)
    {
      n = asciiM0[n];
      character_set = (n & 0x80) >> 7;
    }
  else if (character_set == 1)
    {
      n = asciiM1[n];
      character_set = (n & 0x80) >> 7;
    }
  else
    {
      n = ((n - 0x20) & 0x3F);
    }
  if (n != 0xff)
    {
      n &= 0x7F;
      draw_char(character_set, n);
    }
}

/**
 * Process mode 4 (block erase) data word.
 */
void mode4(unsigned long n)
{

  unsigned int x1;
  unsigned int x2;
  unsigned int y1;
  unsigned int y2;
  
  if ((mode_words & 1) > 0)
    {
      mode4start=n;
      return;
    }

  x1 = (mode4start >> 9) & 0x1FF;
  y1 = mode4start & 0x1FF;
  x2 = (n >> 9) & 0x1FF;
  y2 = n & 0x1FF;
  
  screen_erase_block(x1, y1, x2, y2);
  x=x1;
  y=y1 - 15;
}

/**
 * Mode 5,6,7 stubbed and ignored.
 */
void mode5(unsigned long n)
{
  special_mode=5;
  special_mode_param=n;
}

void mode6(unsigned long n)
{
  special_mode=6;
  special_mode_param=n;
}

void mode7(unsigned long n)
{
  special_mode=7;
  special_mode_param=n;
}
