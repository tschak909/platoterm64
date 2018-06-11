/**
 * key.h
 * Keyboard Translation Tables
 */

#ifndef KEY_H
#define KEY_H

/* 64 entries for each possible primary scancode */
/* each of these map to indexes in PTAT          */
/* Thereby mapping each C= key to a PLATO key.   */

static const uint8_t KEYBOARD_TO_PLATO[] = {
  0x13, /* INS/DEL */
  0x16, /* RETURN */
  0x0b, /* CRSR <-> */
  0xff, /* F7/F8 */
  0xff, /* F1/F2 */
  0xff, /* F3/F4 */
  0xff, /* F5/F6 */
  0x0a, /* CRSR ^V */
  0x03, /* 3 */
  0x57, /* W */
  0x41, /* A */
  0x04, /* 4 */
  0x5a, /* Z */
  0x53, /* S */
  0x45, /* E */
  0xff, /* UNUSED */
  0x05, /* 5 */
  0x52, /* R */
  0x44, /* D */
  0x06, /* 6 */
  0x43, /* C */
  0x46, /* F */
  0x54, /* T */
  0x58, /* X */
  0x07, /* 7 */
  0x59, /* Y */
  0x47, /* G */
  0x08, /* 8 */
  0x42, /* B */
  0x48, /* H */
  0x55, /* U */
  0x56, /* V */
  0x09, /* 9 */
  0x49, /* I */
  0x4a, /* J */
  0x00, /* 0 */
  0x4d, /* M */
  0x4b, /* K */
  0x4f, /* O */
  0x4e, /* N */
  0x0e, /* + */
  0x50, /* P */
  0x4c, /* L */
  0x0f, /* - */
  0x5e, /* . */
  0x7C, /* : */
  0xff, /* @ */
  0x5f, /* , */
  0xff, /* STERLING */
  0xff, /* ASTERISK */
  0x5c, /* ] */
  0xff, /* CLR/HOME */
  0xff, /* UNUSED */
  0x5b, /* = */
  0x0C, /* UP ARROW */
  0x5d, /* / */
  0x01, /* 1 */
  0x0d, /* LEFT ARROW */
  0xff, /* UNUSED */
  0x02, /* 2 */
  0x40, /* SPACE */
  0xff, /* UNUSED */
  0x51, /* Q */
  0x1a, /* RUN/STOP */
  0xff, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_SHIFT[] = {
  0x33, /* INS/DEL */
  0x36, /* RETURN */
  0x2b, /* CRSR <-> */
  0xff, /* F7/F8 */
  0xff, /* F1/F2 */
  0xff, /* F3/F4 */
  0xff, /* F5/F6 */
  0x2a, /* CRSR ^V */
  0x23, /* 3 */
  0x77, /* W */
  0x61, /* A */
  0x24, /* 4 */
  0x7a, /* Z */
  0x73, /* S */
  0x65, /* E */
  0xff, /* UNUSED */
  0x25, /* 5 */
  0x52, /* R */
  0x64, /* D */
  0x26, /* 6 */
  0x63, /* C */
  0x66, /* F */
  0x74, /* T */
  0x78, /* X */
  0x27, /* 7 */
  0x79, /* Y */
  0x67, /* G */
  0x29, /* 8 */
  0x62, /* B */
  0x68, /* H */
  0x75, /* U */
  0x76, /* V */
  0x7B, /* 9 */
  0x69, /* I */
  0x6a, /* J */
  0xFF, /* 0 */
  0x6d, /* M */
  0x6b, /* K */
  0x6f, /* O */
  0x6e, /* N */
  0x0A, /* + */ /* possible multiply */
  0x70, /* P */
  0x6c, /* L */
  0x0b, /* - */ /* possible divide */
  0x20, /* . */
  0x02, /* : */
  0xff, /* @ */
  0x21, /* , */
  0xff, /* STERLING */
  0x28, /* ASTERISK */
  0x23, /* ] */
  0xff, /* CLR/HOME */
  0xff, /* UNUSED */
  0xFF, /* = */
  0xFF, /* UP ARROW */
  0x7d, /* / */
  0x7E, /* 1 */
  0xff, /* LEFT ARROW */
  0xff, /* UNUSED */
  0x7F, /* 2 */
  0x60, /* SPACE */
  0xff, /* UNUSED */
  0x71, /* Q */
  0x3a, /* RUN/STOP */
  0xff, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_COMMO[] = {
  0xff, /* INS/DEL */
  0xff, /* RETURN */
  0xff, /* CRSR <-> */
  0xff, /* F7/F8 */
  0xff, /* F1/F2 */
  0xff, /* F3/F4 */
  0xff, /* F5/F6 */
  0xff, /* CRSR ^V */
  0xff, /* 3 */
  0xff, /* W */
  0x12, /* A */
  0xff, /* 4 */
  0xff, /* Z */
  0x1a, /* S */
  0x17, /* E */
  0xff, /* UNUSED */
  0xff, /* 5 */
  0x13, /* R */
  0x19, /* D */
  0xff, /* 6 */
  0x1b, /* C */
  0x34, /* F */
  0x32, /* T */
  0x0a, /* X */
  0xff, /* 7 */
  0x11, /* Y */
  0x0b, /* G */
  0xff, /* 8 */
  0x18, /* B */
  0x15, /* H */
  0xff, /* U */
  0xff, /* V */
  0xff, /* 9 */
  0xff, /* I */
  0xff, /* J */
  0xff, /* 0 */
  0x14, /* M */
  0xff, /* K */
  0xff, /* O */
  0x16, /* N */
  0x2e, /* + */ /* possible multiply */
  0x10, /* P */
  0x1d, /* L */
  0x2f, /* - */ /* possible divide */
  0xff, /* . */
  0xff, /* : */
  0xff, /* @ */
  0xff, /* , */
  0xff, /* STERLING */
  0xff, /* ASTERISK */
  0xff, /* ] */
  0xff, /* CLR/HOME */
  0xff, /* UNUSED */
  0xff, /* = */
  0xff, /* UP ARROW */
  0xff, /* / */
  0xff, /* 1 */
  0xff, /* LEFT ARROW */
  0xff, /* UNUSED */
  0xff, /* 2 */
  0xff, /* SPACE */
  0xff, /* UNUSED */
  0x1C, /* Q */
  0x1a, /* RUN/STOP */
  0xff, /* EMPTY KEY */
};

static const uint8_t KEYBOARD_TO_PLATO_CS[] = {
  0xff, /* INS/DEL */
  0xff, /* RETURN */
  0xff, /* CRSR <-> */
  0xff, /* F7/F8 */
  0xff, /* F1/F2 */
  0xff, /* F3/F4 */
  0xff, /* F5/F6 */
  0xff, /* CRSR ^V */
  0xff, /* 3 */
  0xff, /* W */
  0x12, /* A */
  0xff, /* 4 */
  0xff, /* Z */
  0x3a, /* S */
  0x17, /* E */
  0xff, /* UNUSED */
  0xff, /* 5 */
  0x13, /* R */
  0x19, /* D */
  0xff, /* 6 */
  0x1b, /* C */
  0x34, /* F */
  0x32, /* T */
  0x0a, /* X */
  0xff, /* 7 */
  0x11, /* Y */
  0x0b, /* G */
  0xff, /* 8 */
  0x18, /* B */
  0x15, /* H */
  0xff, /* U */
  0xff, /* V */
  0xff, /* 9 */
  0xff, /* I */
  0xff, /* J */
  0xff, /* 0 */
  0x14, /* M */
  0xff, /* K */
  0xff, /* O */
  0x16, /* N */
  0x2e, /* + */ /* possible multiply */
  0x10, /* P */
  0x1d, /* L */
  0x2f, /* - */ /* possible divide */
  0xff, /* . */
  0xff, /* : */
  0xff, /* @ */
  0xff, /* , */
  0xff, /* STERLING */
  0xff, /* ASTERISK */
  0xff, /* ] */
  0xff, /* CLR/HOME */
  0xff, /* UNUSED */
  0xff, /* = */
  0xff, /* UP ARROW */
  0xff, /* / */
  0xff, /* 1 */
  0xff, /* LEFT ARROW */
  0xff, /* UNUSED */
  0xff, /* 2 */
  0xff, /* SPACE */
  0xff, /* UNUSED */
  0x1C, /* Q */
  0x3a, /* RUN/STOP */
  0xff, /* EMPTY KEY */
};

#endif /* KEY_H */
