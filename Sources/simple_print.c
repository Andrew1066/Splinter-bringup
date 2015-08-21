/*
 * simple_print.c
 *
 *  Created on: 19 Aug 2013
 *      Author: jammic
 *
 *      This implements a small set of useful text output
 *      routines for use in small beacon code.
 *      redirects output to the serial port ...
 *
 */
#include "stdint.h"
#include "simple_print.h"
#include "fsl_uart_hal.h"
#include "Globals.h"
#include "fsl_debug_console.h"

void PutChar (char);

//#include <string.h>

//#if defined SIMPLE_PRINT
///////////////////////////////////////////////////////
// checksum handling
static uint16_t   sum = 0;
static char      use_sum =0;
uint8_t * use_ptTab=0;

void print_init_sum(void)
{
  sum = 0;
  use_sum = 1;
}


void print_stop_sum(void)
{
	use_sum=0;
}

void print_sum(void)
{
	print_dec(sum);
}

void print_str(char * s)
{
//	uint8_t i=0;

	while (*s)
	{
			PutChar(*s++);
	}
}

void print_conststr(char * s)
{
//	uint8_t i=0;
	char * cptr = s;

	while (*cptr)
	{
			PutChar(*cptr++);
	}
}


void print_crlf(void)
{
	print_str("\r\n");
}

void print_comma(void)
{
	print_str(",");
}

void print_dec(unsigned long n)
{
//  int i;
    char buff[11]; // 10 chars
    buff[10]=0; // terminate
    char *p=buff+9;
    do  // at least one loop to ensure a 0 is printed.
    {
    	char  d;
     	d=n%10;
    	n/=10;
    	*p-- = d+'0' ;
    } while(n);
    p++;
    print_str(p);
}

void print_signed(long n)
{
  if(n<0)
  {
    print_str("-");
    n=0-n;
  }
  print_dec(n);
}

void format_hex(uint8_t * tabOut, unsigned long n, eHexFmt fmt)
{	use_ptTab=tabOut;
	print_hex( n,  fmt);
	use_ptTab=0;

}

void print_hex(unsigned long n, eHexFmt fmt)
{
  char buff[9]; // 8 spaces with a terminating \0
  int i;
  char c;
  char *p=buff+7;         // point at least significant char

  if(fmt & 0x10 ) // space prefixed hex
  {
    c=' ';
    fmt &=0x0F; // remove flag leaving number of chars
  }
  else
  {
    c='0';
  }
  for(i=0; i<8; i++)
  {
    buff[i]=c;
  }
  buff[8]=0;

  // lay out the hex digits right justified in buffer, stop when done.
  // the remainder of the buffer to the left is either spaces or zeros.
  do // at least one loop to ensure a '0' is produced.
  {
    char  d;
    d=n & 0x0f;
    n>>=4;
    *p-- = d>9 ?  d-10+'A' :  d+'0' ;
  } while(n);

  p=buff + 8 - fmt; // index in the right number of chars from the start to output

    print_str(p);

}

void PutChar (char nextchar)
{
	debug_putchar((int) nextchar);
}

unsigned char GetChar (void)
{
	return ((unsigned char) debug_getchar());
}

#if 0
void PutChar (char nextchar)
{
	UART_Type * base = UART1_BASE;
	while (!UART_BRD_S1_TDRE(base))
        ;
        UART_HAL_Putchar(base, nextchar);
    }

unsigned char GetChar (void)
{
    unsigned char retVal = 0;
    uint8_t dummy = 0;
    UART_Type * base = UART1_BASE;

    if (UART_BRD_S1_RDRF(base))
        {
    	retVal = UART_RD_D(base);

        /* Clear the Overrun flag since it will block receiving */
        if (UART_BRD_S1_OR(base))//
        {
            dummy = UART_RD_S1(base);
            dummy = UART_RD_D(base);
            dummy++; /* For unused variable warning */
 //           retVal = 1;
        }
    }
    return retVal;
}

// end of SIMPLE_PRINT block
#endif
