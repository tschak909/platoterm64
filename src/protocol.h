/**
 * protocol.h - header definitions for PLATO protocol decoder
 *
 * Author: Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Started: 2018-03-27
 */

/**
 * decode(unsigned char b) The primary entry point for protocol decoding.
 * Input is a byte received from PLATO
 * The protocol then calls other parts of the program to do
 * What is asked by the protocol.
 */
void decode(unsigned char b);

/**
 * decode_dumb_terminal(unsigned char b) - When terminal is in dumb terminal mode, go here.
 */
void decode_dumb_terminal(char b);

/**
 * decode_plato(unsigned char b) - Decode next PLATO Character
 * This is the meat of protocol decoding.
 */
void decode_plato(unsigned char b);

/**
 * process_pni_rs(void) - Process PNI RS sequences, for now
 * ignore them.
 */
void process_pni_rs(void);

/**
 * process_plato_metadata(unsigned char b)
 * Process a byte of PLATO metadata.
 */
void process_plato_metadata(unsigned char b);

/**
 * process_escape_sequence(unsigned char b)
 * Process the next escape sequence
 */
void process_escape_sequence(unsigned char b);

/**
 * Process an ASCII control character b, not part of escape sequence.
 */
void process_control_characters(unsigned char b);

/**
 * process_other_states(unsigned char b) Process the rest of the possible protocol states.
 */
void process_other_states(unsigned char b);

/**
 * process_ext(unsigned int n) - Process EXT packets
 */
unsigned int process_ext(unsigned int n);

/**
 * assemble_ascii_plato_metadata(unsigned char b) Assemble next byte of PLATO metadata.
 */
unsigned int assemble_ascii_plato_metadata(unsigned char b);

/**
 * assemble_coordinate(unsigned char b) - Get next part of coordinate data
 * and assemble towards a completed coordinate.
 */
unsigned int assemble_coordinate(unsigned char b);

/**
 * assemble_paint(unsigned char b) - Get next part of paint data
 * and assemble towards a completed paint request.
 */
unsigned int assemble_paint(unsigned char b);

/**
 * assemble_data(unsigned char b) - Assemble the body of a data packet.
 */
unsigned int assemble_data(unsigned char b);

/**
 * assemble_color(unsigned char b) - Assemble the next color byte
 */
unsigned int assemble_color(unsigned char b);

/**
 * assemble_grayscale(unsigned char b)
 */
unsigned int assemble_grayscale(unsigned char b);

/**
 * send_ext(unsigned int key) - Send EXT sequence
 */
void send_ext(unsigned int key);

/**
 * send_echo_request(unsigned int n) - Send an echo request back to PLATO
 */
void send_echo_request(unsigned int n);

/**
 * send_processed_key(unsigned int n) - Send a key to PLATO
 */
void send_processed_key(unsigned int n);

/**
 * process_color(unsigned long n) - Process a color
 */
void process_color(unsigned long n);

/**
 * process_grayscale(unsigned long n) - Process Grayscale
 */
void process_grayscale(unsigned long n);

/**
 * process_modes(unsigned int b) - Process the current mode
 */
void process_modes(unsigned int b);

/**
 * mode0(unsigned int n) - Mode 0, dot mode.
 */
void mode0(unsigned int n);

/**
 * mode1(unsigned int n) - Mode 1, line mode
 */
void mode1(unsigned int n);

/**
 * mode2(unsigned int n) - Mode 2 - Memory load
 */
void mode2(unsigned int n);

/**
 * mode3(unsigned int n) - Mode 3 - alpha data word.
 */
void mode3(unsigned char b);

/**
 * Process mode 4 (block erase) data word.
 */
void mode4(int n);

/**
 * Mode 5,6,7 stubbed and ignored.
 */
void mode5(int n);

void mode6(int n);

void mode7(int n);
