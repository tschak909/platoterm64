/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard_base.c - Keyboard functions (base)
 */

#include <stdint.h>
#include <peekpoke.h>
#include <stdbool.h>
#include "keyboard.h"
#include "screen.h"
#include "protocol.h"
#include "io.h"
#include "plato_key.h"

extern padBool TTY;

/* ACCESS Key combinations. */
const uint8_t ACCESS_KEYS[] = {
  PKEY_a, /* 0x80 a ɑ alpha */ 
  PKEY_B, /* 0x81 b ß beta */
  PKEY_c, /* 0x82 c cedilla */
  PKEY_d, /* 0x83 d δ delta */
  PKEY_e, /* 0x84 e ' acute accent */
  PKEY_g, /* 0x85 g æ ae */
  PKEY_h, /* 0x86 h oe oe */
  PKEY_j, /* 0x87 j å a with ring */
  PKEY_k, /* 0x88 k ä a with diaeresis */
  PKEY_l, /* 0x89 l ƛ lambda */
  PKEY_m, /* 0x8A m μ mu */
  PKEY_n, /* 0x8B n ~ tilde */
  PKEY_o, /* 0x8C o ° degree */
  PKEY_p, /* 0x8D p π pi */
  0x51, /* 0x8E q ` grave */
  PKEY_r, /* 0x8F r ρ rho */
  PKEY_s, /* 0x90 s σ sigma */
  PKEY_t, /* 0x91 t θ theta */
  PKEY_u, /* 0x92 u ¨ diaeresis */
  PKEY_v, /* 0x93 v hacek (upside down circumflex) */
  PKEY_w, /* 0x94 w ϖ capital pi */
  PKEY_x, /* 0x95 x ^ circumflex */
  PKEY_0, /* 0x96 0 l-embed */
  PKEY_1, /* 0x97 1 r-embed */
  PKEY_5, /* 0x98 5 @ */
  PKEY_6, /* 0x99 6 arrow */
  PKEY_PLUS, /* 0x9a + & */
  0x26, /* 0x9b & interpunct */
  PKEY_COLON, /* 0x9c : ~ lower tilde */
  0x5f, /* 0x9d , delimiter */
  PKEY_SLASH, /* 0x9e / \ */
  PKEY_EQUALS, /* 0x9f = not equal */
  PKEY_y, /* 0xA0 y ö */
  0x61, /* 0xA1 A left arrow */
  0x78, /* 0xA2 X down arrow */
  0x64, /* 0xA3 D right arrow */
  0x77, /* 0xA4 W up arrow */
  0x63, /* 0xA5 C © */
  0x66, /* 0xA6 F ♦ */
  0x67, /* 0xA7 G Æ */
  0x68, /* 0xA8 H OE */
  0x69, /* 0xA9 I | */
  0x6A, /* 0xAA J Å */
  0x6B, /* 0xAB K Ä */
  0x6F, /* 0xAC O SQUARE */
  0x79, /* 0xAD Y Ö */
  0x20, /* 0xAE < ≤ */
  0x21, /* 0xAF > ≥ */
  0x5B, /* 0xB0 [ { */
  PKEY_SLASH, /* 0xB1 ] } */
  0x24, /* 0xB2 $ # */
  0x9a, /* 0xB3 & big cross */
  0x7B  /* 0xB4 EQUIVALENT */
};

/**
 * keyboard_out - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void keyboard_out(uint8_t platoKey)
{
  if (platoKey==0xff)
    return;
  
  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;
}

/**
 * keyboard_out_tty - keyboard output to serial I/O in TTY mode
 */
void keyboard_out_tty(char ch)
{
  io_send_byte(ch);
}
