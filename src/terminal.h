/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * terminal.c - Terminal state functions
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include "protocol.h"

/**
 * SetTTY(void) - Switch to TTY mode
 */
void SetTTY(void);

/**
 * SetPLATO(void) - Switch to PLATO mode
 */
void SetPLATO(void);

/**
 * Features(void) - Inquire about terminal ASCII features
 */
uint8_t Features(void);

/**
 * TermType(void) - Return the appropriate terminal type
 */
uint8_t TermType(void);

/**
 * SubType(void) - Return the appropriate terminal subtype
 */
uint8_t SubType(void);

/**
 * LoadFile(void) - Return the appropriate terminal loadfile (should just be 0)
 */
uint8_t LoadFile(void);

/**
 * Configuration(void) - Return the terminal configuration
 */
uint8_t Configuration(void);

/**
 * CharAddress(void) - Return the base address of the character set.
 */
uint16_t CharAddress(void);

/**
 * MemRead - Read a byte of program memory.
 * not needed for our terminal, but must
 * be decoded.
 */
padByte MemRead(padWord addr);

/**
 * MemLoad - Write a byte to non-character memory.
 * not needed for our terminal, but must be decoded.
 */
void MemLoad(padWord addr, padWord value);

/**
 * CharLoad - Store a character into the user definable
 * character set.
 */
void CharLoad(padWord charnum, charData theChar);

/**
 * Mode5, 6, and 7 are basically stubbed.
 */
void Mode5(padWord value);
void Mode6(padWord value);
void Mode7(padWord value);

/**
 * ExtAllow - External Input allowed. Not implemented.
 */
void ExtAllow(padBool allow);

/**
 * SetExtIn - Set which device to get input from.
 * Not implemented
 */
void SetExtIn(padWord device);

/**
 * SetExtOut - Set which device to send external data to.
 * Not implemented
 */
void SetExtOut(padWord device);

/**
 * ExtIn - get an external input from selected device.
 * Not implemented.
 */
padByte ExtIn(void);

/**
 * ExtOut - Send an external output to selected device
 * Not implemented.
 */
void ExtOut(padByte value);

#endif
