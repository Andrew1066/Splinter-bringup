/*
 * misc_stuff.h
 *
 *  Created on: 13 Sep 2013
 *      Author: Mike
 */

#ifndef SIMPLE_PRINT_H_
#define SIMPLE_PRINT_H_

#define SIMPLE_PRINT

#if defined SIMPLE_PRINT
extern void print_str(char * s);

// all of these functions use print_str so it can compute the checksum of printed data.
extern void print_crlf(void);
extern void print_comma(void);
extern void print_dec(unsigned long n); ///< print an integer as an unsigned value
extern void print_signed(long n);       ///< print an integer as a signed value

// checksumming control
extern void print_stop_sum(void);
extern void print_init_sum(void);
extern void print_sum(void);


typedef enum { h02=2,h04=4,h08=8, h2=0x12, h4=0x14,h8=0x18 } eHexFmt;
extern void format_hex(unsigned char  * tabOut, unsigned long n, eHexFmt fmt);
extern void print_hex(unsigned long n,eHexFmt fmt);
#endif

#endif /* SIMPLE_PRINT_H_ */
