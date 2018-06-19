/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * welcome.h - ASCII data for Welcome greeting
 */

#ifndef WELCOME_H
#define WELCOME_H

#include <stdint.h>

// PLATOTerm for Commodore 64
uint8_t welcomemsg_1[]={80,76,65,84,79,84,101,114,109,32,102,111,114,32,67,111,109,109,111,100,111,114,101,32,54,52};
#define WELCOMEMSG_1_LEN 26

// Copyright 2018 IRATA.ONLINE
uint8_t welcomemsg_2[]={67,111,112,121,114,105,103,104,116,32,40,99,41,32,0x32,0x30,0x31,0x38,0x20,73,82,65,84,65,46,79,78,76,73,78,69};
#define WELCOMEMSG_2_LEN 31

// This software is licensed under GPL 3.0.
uint8_t welcomemsg_3[]={84,104,105,115,32,115,111,102,116,119,97,114,101,32,105,115,32,108,105,99,101,110,115,101,100,32,117,110,100,101,114,32,71,80,76,32,51,46,48,32,46};
#define WELCOMEMSG_3_LEN 40

// See COPYING for details.
uint8_t welcomemsg_4[]={83,101,101,32,99,111,112,121,105,110,103,32,102,111,114,32,100,101,116,97,105,108,115};
#define WELCOMEMSG_4_LEN 23

// PLATOTerm READY
uint8_t welcomemsg_5[]={80,76,65,84,79,84,101,114,109,32,82,69,65,68,89};
#define WELCOMEMSG_5_LEN 15

#endif
