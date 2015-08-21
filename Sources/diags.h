/*
 * diags.h
 *
 *  Created on: 12 Sep 2014
 *      Author: bucand
 */

#ifndef DIAGS_H_
#define DIAGS_H_

#define SI446X_SLEEP	0x01
#define SI446X_READY	0x03
#define	 SI446X_TX		0x07

#define FLASH_PAGE_SIZE	256;
#define testss	(uint32_t) 0x08080000;			// Start address of flash E2 test save area
#define test_size (sizeof (test_block));

#define True	0xff
#define False	0x00


#define MAX_PROCESSES 8
#define device_id 0xe0042000			// 10386416 is the STM board, 10786416 is Titanium

#define SI_GPIO_NC		0x00		// no change
#define SI_GPIO_O_0		0x02		// set GPIO to 0
#define SI_GPIO_O_1		0x03		// set GPIO to 1

#define PSH_SLOT	0
#define TEST_SLOT	1
#define STPD_SLOT	2
#define MACRO_SLOT	3
#define GPS_SLOT	4
#define CMD_SLOT	5					// Foreground task
#define STDO_SLOT	6
#define STDI_SLOT	7					// Background process <test>

//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define Flash_Lo	(long *)0x00000000
#define Flash_Hi	(long *)0x0801ffff
#define Sram_Lo	(long *)0x20000000
#define Sram_Hi	(long *)0x20003fff
#define Periph_Hi	(long *)0x40000000
#define Periph_Lo	(long *)0x40026400

// non-persistant / shared

extern char volatile RxNew;					// value from the Rx ISR;

// persistent globals

char cmdstr[32];			// command string as entered by user (with backspace handled)
unsigned char dquote;		// allows entry of quoted strings to preserve white space
char * cmdstrptr;			// persistent pointer to command string as entered
// static int argc;					// argument count reset in case command not found
char * argv[5];

#endif /* DIAGS_H_ */
