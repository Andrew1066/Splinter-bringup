/* Diagnostics functionality
 *
 * This file contains all the diags functionality. It has interesting snippets from
 * donor projects (such as queue implementations and hooks) as these might, indeed
 * should be useful in the future. Note that USART tx activities should be queued
 * and streamed to make debug mode (verbatim) less intrusive on performance.

 * Devices to interface with:
 *  	MPU 9250
 *  	MAX 9860
 *
 *
 */

/*
#define RX_ERROR	0
#define	ISR_ERROR	4

#define	RX_Q_SIZE	0x3f	
#define	TX_Q_SIZE	0xff ; 	
#define RX_Q_EMPTY	6
#define RX_Q_FULL	7
#define TX_Q_EMPTY	6
#define TX_Q_FULL	7


;**********************************************************************

//; in order to allow applications to consume standard in streams, it will be necessary to allow pshell
//; execution to be suspended - at least the part which reads the input queue.
//; pshell generates a linear version of the input character stream up to and including the LF. It handles BS
//; and converts spaces to zeros which become the token delimiters for the parse function. Escaping a sequence
//; using " will maintain spaces up to a closing ".
//;
 *	TODO Check back space across double quoted arguments
 *	TODO Add alias feature if required
 *	TODO Change sdtout to interrupt driven via queue
 *	TODO Change delay function to be multi-tasking not nasty blocking
 *	TODO

*/
// dependencies and headers

#include "stdint.h"
#include "Globals.h"
#include "simple_print.h"
#include "versions.h"
#include "diags.h"
#include "fsl_debug_console.h"
#include "MK65F18_extension.h"
#include "fsl_gpio_driver.h"
#include "gpio2.h"


// array of pointers to argument strings

// static int onflag;
// static int offflag;


static int MacroMode;				// mode of the macro handler
static char * MacroWPtr;			// write macro pointer
static char * MacroRPtr;			// macro read pointer (starts at selected macro name)
static char Macros[256];			// place to store macro commands
static char * MacroLPtr;			// single level loop pointer for iteration
//static int MacroLCount;			// macro loop counter
static int argc;

enum {M_OFF, M_ENTER, M_RUN};

/*
 * below shows the structure of a diags test schema. Initially all values are set to 0. If a request is made
 * to run an empty context, the string "not configured" will be sent to stdout
 * On start-up in one of the ATE modes, or on request from the command line each entry will be checked and the
 * associated command run.
 */

struct s_test
{
	unsigned int porta[32];
	unsigned int portb[32];
	unsigned int portc[32];
	unsigned int portd[32];
	unsigned int porte[32];
	unsigned long freq;
		unsigned char ant;			//
		unsigned char load;		// set these as blind outputs
		unsigned char band;		//
		char bias;
		unsigned char pwr_lvl;
		unsigned int	pwr_am;
		unsigned char mod;
		unsigned int p_dur;
		unsigned int p_rep;
		unsigned int dur;
		unsigned int lowsine;
		unsigned int hisine;
} test_block;

#define FLASH_PAGE_SIZE	256;
#define testss	(uint32_t) 0x08080000;			// Start address of flash E2 test save area
#define test_size (sizeof (test_block));

#define True	0xff
#define False	0x00

// command specific persistents

static char * cptr;

// constants

char * Aliases[];
char * Unknown[];
char * BuiltIns[];
char * Prompt[];
char * CmdRun[];
char * Bands[];
char * Ant[];
char * Mods[];

#define MAX_PROCESSES 8
#define device_id 0xe0042000			// 10386416 is the STM board
#define PSH_SLOT	0
#define TEST_SLOT	1
#define STPD_SLOT	2
#define MACRO_SLOT	3
#define SPA_SLOT	4
#define CMD_SLOT	5					// Foreground task
#define STDO_SLOT	6
#define STDI_SLOT	7					// Background process <test>

//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

enum {STOPPED, SUSPENDED, RUNNING};


// function references which spawn to foreground tasks

void help (unsigned char);
void port (unsigned char);
void proc (unsigned char);
void echo (unsigned char);
void mem (unsigned char);
void run (unsigned char);
void stop (unsigned char);
void status (unsigned char);
void adc (unsigned char);
void reset (unsigned char);
void ver (unsigned char);
void alias (unsigned char);
void loop (unsigned char);
void sleep (unsigned char);
void endl (unsigned char);
void macro (unsigned char);
void endm (unsigned char);
void save (unsigned char);
void runtest(void);

void band(unsigned char);
void tx(unsigned char);

void mru(unsigned char);
void colourbar(unsigned char);
void dac(unsigned char);
void ant(unsigned char);
void pwr(unsigned char);

// background tasks with their own process slots

void test (unsigned char);
void pshell (unsigned char);
void sigstpd (unsigned char);
void macrod (unsigned char);
void backgnd (unsigned char);
void stodo(void);				// "something to do" dummy test to prove architecture

extern void PutChar(char);

// CLI functions

int parse(char *);					// convert command line into argv list and argc count
int spawn(int,int);				// spawn is told what slot to use (unlike an rtos)
int getstrid(char *, char *);		// get the index of a command string in the built-ins
char * Strfnd(char *, char *, char);
char * getcmdstr (char *, int);
char ToLower(char);
char GetChar(void);

void diags(void);
void procterm(int);
long DectoBin (char *);
long HextoBin (char *);

typedef struct processcb
{
	int pmode;		// process mode ie running, suspended, stopped.
	int pindex;		// process index, number of built-in command in list
	int pstate;		// process state
	int pspare;		//
}pcb_s;

static unsigned char taskid;
static struct processcb pcb[8];


#define NVR_LOG_START 0x08080100;
#define NVR_CONFIG_START 0x08081000; // Is this correct -  check sizes

typedef struct{
	int lifetime;
	unsigned int nofGPStests;
	int etc_etc;

} nvr_stuff;

nvr_stuff * logs = (nvr_stuff*)NVR_LOG_START;

typedef void (* func_t)(unsigned char);
typedef void (* const voidfunc)(unsigned char);

//func_t FuncPtrs[30];
func_t FuncPtrs[]	= {pshell, test, sigstpd, macrod, backgnd, port, help, proc, echo, mem, run, stop, status, \
					adc, reset, colourbar, pwr, ver, alias, \
							loop, sleep, endl, macro, endm, band, ant, tx, dac, save, mru};

int portlist[] = {GPIOA_IDX, GPIOB_IDX, GPIOC_IDX, GPIOD_IDX, GPIOE_IDX};

/*
 * Initialise function - sets up diagnostics, memory, and other stuff
 *
 */
void diagsinit(void)
{
//	pcbinit();
// send 1s RLS flash ???
// GNSS configuration setup
// set pll for ramp enabled, slowest 20uS ramp rate, pll in rdy mode
//	EmColdStartGPS
}

/* this function looks through the running processes and selects each in
 * turn. This allows multiple instances of any process (not implemented)
 * and multiple processes to share the processor.
 *
 * called by main
 */
void diags (void)
{
	if (pcb[taskid].pmode == RUNNING)
	   {
	   (*FuncPtrs[pcb[taskid].pindex])(taskid);		// task to run
	   }
	// go to next process and use id to find function start address eg
	if(++taskid > MAX_PROCESSES)
		taskid = 0;
	return;
}

void pcbinit (void)
{
	int index, port, mode;
	unsigned int * ports[5];

	for (index = 0; index < MAX_PROCESSES; index++ )
		pcb[index].pmode = STOPPED;
	taskid = 0;

	MacroMode = M_OFF;
	MacroWPtr = &Macros[0];			// point to the start of the macro block

	SigStp = 0x00;
	spawn (2, STPD_SLOT);			// Start the stop daemon
	// initialise current test spec

	ports[0] = &test_block.porta[0];		// set the array of array pointers
	ports[1] = &test_block.portb[0];
	ports[2] = &test_block.portc[0];

	for (port = 0; port < 3; port++)
		for (index = 0; index < 16; index++)
		{
 		mode = GPIO_DRV_GetPinDir((port << 8) | index);
		switch (mode)
			{
			case (0):
				ports[port][index] = 0xff;
				break;
			case (1):
				ports[port][index] = 0x03;		// show as unchanged (X)
				break;
			case (2):
				ports[port][index] = 0xff;
				break;
			case (3):
				ports[port][index] = 0xff;
				break;
			}
		}
}
/*
 *  this function takes characters as typed by the user and stores them sequentially in a command buffer with the following
 *  exception.
 *  1) if a space is encountered, it is replaced by a 0x00 unless within double quotes
 *  2) if backspace is encountered, the last character entered is deleted, and if it was a double quote, handled accordingly

 *  All commands are foreground. There can only be one running at a time. However, some kick off b/g processes which will not
 *  terminate and can only be stopped using the "stop" command which stops everything that is currently running.
 */
void pshell(unsigned char pindex)
{
	char newchar;
	unsigned int index;
//	char * aliascmdstr[32];				// place to put any alias substitution (cannot overwrite cmdstring entry as
										// new substitution string might be longer than entered)
	switch (pcb[pindex].pstate)
	{
	case 0 :
		// initialise
		cmdstrptr = &cmdstr[0];	// Reset args pointer to start for next time......
		dquote = False;			// and reset the escape double quotes count
		print_str(Prompt[0]);
		pcb[pindex].pstate++;
		break;
	case 0xff:
		pcb[pindex].pmode = STOPPED;
		break;
	default:

	if ((newchar = GetChar()) == 0)		// if something to do, do it
		return;
	else
	{
//		newchar = RxChar;			// as ASCII only, the character could be the flag (ie NULL = nothing)
//		RxChar = 0x00;				// clear flag for next character - this is only good for ASCII stdin reception of course

		switch (newchar)
		{
		case '\t':					// ignore tab and lf (and any other chars)
		case 0x0a:
			break;

		case ' ' :					// space is preserved as space if within double quotes
			*cmdstrptr++ = dquote ? 0x20 : 0x00;
			PutChar(newchar);		// echo character to stdout (if echo switched on)
			break;

		case 0x08 :					// backspace
			{
			if (cmdstrptr > &cmdstr[0])
			{
				cmdstrptr--;
				print_str ("\b \b");
			}
			if (*cmdstrptr == 0x20)
				dquote ^= True;					// toggle double quote if we find a space -  TODO well no as there might be > 1

			// TODO we will have to walk back through to see if next 00/20 byte was 20. if so, leave as is, otherwise toggle

			break;
			}

		case '"' :							// double quote
			dquote ^= True;					// toggle
			PutChar(newchar);		// echo character to stdout (if echo switched on)
			break;

		case 0x0d:		// CR means end of line
			*cmdstrptr++ = 0x00;	// terminate command string
			*cmdstrptr = 0x03;
			print_str("\r\n");

			// state 2
			if (!parse(cmdstr))			// parse the complete terminated string set argc argv
				{
				pcb[pindex].pstate = 0; // restart the pshell
				break;
				}							// next we have a command string to interrogate

			// if macro ENTER mode, copy this clean string to macro block

			if (MacroMode == M_ENTER)
				{
				cmdstrptr = &cmdstr[0];		// copy the whole string (includng terminator) to the macro from current position
				while ((*MacroWPtr++ = *cmdstrptr++) != 0x03);
				}

			index = getstrid((char *) BuiltIns[0], argv[0]);
			PutChar('\n');
			if (index != -1)
				{
				// use index (+ #backgrounds) to spawn the command (ie pcflags(index) = true or something

				if (spawn (index + 5, CMD_SLOT) == -1)		// only use slot 2 for transient commands
					{
					print_str((char *) CmdRun[0]);
					break;
					}
				else
					pcb[PSH_SLOT].pmode = SUSPENDED;	// suspend pshell until command completes for fg commands

				pcb[pindex].pstate = 0;
				}
			else
// This part is to do with command aliasing which is not required
//			{
//				index = getstrid((char *) Aliases, argv[0]);		// Check aliases, index should be even
//				if (index != -1)
//					// substitute with alias
//					{
//					Strcpy (getcmdstr((char * ) Aliases, index + 5), *aliascmdstr);
//					argv[0] = (char *) &aliascmdstr; 			// update argv[0] pointer
//					goto SearchCmd;	// try again to find command
//					}
//				else
				{
				print_str (argv[0]);
				print_str (Unknown[0]);
				pcb[pindex].pstate = 0;
				break;
				}
//			}
		default:
			*cmdstrptr++ = ToLower(newchar);	//case insensitive
			PutChar(newchar);		// echo input character to stdout (if echo switched on)
		}
	}
	}
	return;
}

char ToLower(char inchar)
{
	if ((inchar <= 0x5a) && (inchar >= 0x41))
		return (inchar |= 0x20);
//	return (inchar);		// ARB Change 19/8/15
}


/*
 * This function takes the command string as entered and tokenised by pshell and sets argc count
 * and the argv pointers to identify how many arguments were entered and where they are.
 */
int parse(char * command)
{
	int ws = True;				// whitespace flag - set to allow initial ws if it exists
	char * cmdptr = command;	// Pointer to proceed through command string
	char **argvptr = &argv[0];	// pointer to array of string pointers for the arguments
	argc = 0;

	while (*cmdptr != 0x03)		// while not at the end of the command string entered
	{
		if (*cmdptr == 0x00)	// set ws if we find one (as null is arg separator)
			ws = True;
		else
			{
			if (ws)
				{
				ws = False;		// Clear whitespace flag for next arg
				argc++;			// increment the argument counter
				*argvptr++ =  cmdptr;
				}
			}
	cmdptr++;
	}
	return (argc);				// return the number of arguments entered
}

// find a string amongst a list of strings and return pointer to the char after it

char * Strfind (char * str, char * target, char delim)
{
	char * tableptr = str;
	char * targptr = target;

	while (*tableptr != delim)
	{
		if (*tableptr++ != *targptr)
		{
			targptr = target;
		}
		else
			if (*targptr++ == 0x00)
				return (targptr);
	}
	return (0);
}

// look for a string within a given number of characters

char * Strnfind (char * str, char * target, int todo)
{
	char * tableptr = str;
	char * targptr = target;

	while (todo-- > 0)
	{
		if (*tableptr++ != *targptr)
			targptr = target;
		else
			if (*targptr++ == 0x00)
				return (tableptr);
	}
	return (0);
}


int getstrid (char * str, char * target)
{
	char * tableptr = str;
	char * targptr = target;
	int index = 0;

	while (*tableptr != 0x03)
	{
		if (*tableptr++ != *targptr)
		{
			index++;
			targptr = target;
			while (*tableptr++ != 0x00); 	// wind through to start of next one
		}
		else
			if (*targptr++ == 0x00)
				return (index);
	}
	return (-1);
}

// need this for aliasing

char * getcmdstr (char * stringsptr, int index)
{
	// move through list of strings until we get to the one of the number
	// associated with index
	int cindex = 0;
	char * startptr = stringsptr;

	while (cindex != index)
	{
		while (*startptr++ != 0x00);  // move to end of current string
		cindex++;
	}
	return (startptr);
}

/*
 * this function makes a command selected from hte command line into a process which runs
 * concurrently to all others non-transients.
 */

/* Normally this sort of function would search through the slot list to find an
 * empty one if one exists. This means that processes would have to be given
 * a base address for their runtime memory block. Instead, we allocate
 * the memory blocks statically. Command blocks can be union structs as only one runs at a time
 * Command processes are effectively foreground in that pshell is suspended until they finish - yuk!.
 *
 */
int spawn (int index, int slot)
{
	// check the slot is empty (process control block)

	if (pcb[slot].pmode == STOPPED)
		{							// empty process - lets use it
		pcb[slot].pindex = index;	// this command index
		pcb[slot].pstate = 0;  		// constructor state
		pcb[slot].pmode = RUNNING;	// now set to running
		SigErr = 0;					// clear error return code
		return (slot);				// return back the required slot if ok
		}
	else							// already running - no re-entrancy
		return (-1);
}

/*
 * process identified by pid will be terminated
 * and pshell will be re-started
 */
void procterm (int pid)
{
	pcb[pid].pmode = STOPPED;

	if (MacroMode == M_RUN)
		pcb[MACRO_SLOT].pmode = RUNNING;
	else
		pcb[PSH_SLOT].pmode = RUNNING;
}

/* process identified by pid will be shut down
 * meaning that the process will run its
 * destructor before termination
 * example stubs.......
 */
//void procsd (pid)
//{
//	pcb[pid].pstate = -1;
//}

// Mind-map section to explore memory methods for use in future....

// some constructs for unions as these may be required to minimise SRAM usage

struct a
{
	int fred;
	long tom;
};

struct b
{
	long martha;
	int amie;
};

struct c
{
	char chararray [6];
};

union try
{
	struct a aa;
	struct b bb;
	struct c cc;

}myunion;

// above, all functions would therefore use myunion.<X> for the variables to write/read.
// no concurrency - no multiple instance of command execution
// context saved by commands to context structure which is saved

/*
 * Every process will need to have its own local variables mapped onto the process slot block. This block will remain persistent
 * until the process gets killed or finishes. The block will be one of 'n' possible blocks and which one at any instantiation
 * will be determined by "spawn" which will allocate the next slot (if available)
 */

void memory(void * base_addr)
{
	void * varptr;

typedef struct _local
	{
		int counter;
		long address;
		char * strptr;
		char instr[32];
	}_mylocals;

//	typedef struct _local _mylocals;		// should be able to combine with struct def whcih we can

// two methods of accessing process block locations

	base_addr += offsetof(_mylocals, counter);
	*(int *)base_addr = 1;

	varptr = base_addr + offsetof(_mylocals, instr);
	*(int *)varptr = 1;
}

// mechanism for generating named offsets for use in the processable functions.

#define character sizeof(char)
#define byte sizeof(char)
#define integer sizeof(int)
#define longint sizeof(long)
#define charstr sizeof(char[])

// allocate right bound offset and introduce a gap to force compiler to use next available offset
#define MEM_OFFSET(name, size) name ## _offset, ___tmp__ ## name = name ## _offset + size - 1,

typedef enum {
	 MEM_OFFSET(param1, byte)
	 MEM_OFFSET(param2, integer)
	 MEM_OFFSET(param3, longint)
	 MEM_OFFSET(param4, character)
	} param_offset;


struct offsets {
    int param1;
    long param2;
} eeprom;

    uint8_t buf;
      /* eeprom_read(offset, size, buf) */
 //   offsetof(eeprom, param1) = 1;

void exproc(void)
{
//	void * base = 0x808000;
//	base + OFFSET(param1) = 1;
}
//; receive bytes from the RS232 port. Each one is placed in the Rx queue and since this process is
//; slower than the task of parsing the characters from the queue, it should not fill up.

void stdid(void)
{
//	btfss	NewRxByte, 7	; if newbyte
//	return		;
//	clrf	NewRxByte		;   newByte = false
//	movf	RxByte, w		;
//	call	Rx_q_Write		;

//; if the last write results in buffer full, then send XOFF
	return;
}

// Transmit any characters that are in the tx queue.
// and turn the Tx off when q empty
// TODO this will need to be implemented properly - ie not blocking tx as it is now
 
void stdod (void)
{
//	btfsc	Tx_Ctrl, TX_Q_EMPTY
//	return
//	btfss	TXSTA, TRMT		; if still tx'ing or q empty - then exit
//	goto	TxDaemon10
//	call	Tx_q_Read		;
//	movwf	TXREG			; transmit the byte
	return;
}

// Back to reality.......

// atomised process to reduce impact on performance

void help(unsigned char pindex)
{
	switch (pcb[pindex].pstate)
	{
	case 0 :
		// initialise
		cptr = BuiltIns[0];
		pcb[pindex].pstate++;
		break;
	case 0xff:
		procterm(pindex);
		break;
	default:		// do the actual function (atomised across each command)
		while (*cptr)
			PutChar(*cptr++);
		cptr++;
		print_str("\r\n");
		if (*cptr == 0x03)
			pcb[pindex].pstate = 0xff;
		break;
	}
}

// just print to stdout what is in arg 1.
// not atomised - JFDI.
// Interesting note: If the argv tokens were null terminated strings with inner CRLF
// then there would be no need to add the crlf print statement.

void echo(unsigned char pindex)
{
	print_str(argv[1]);
	print_str("\r\n");
	procterm(pindex);			// no destructor, just kill process as done
}

#define Flash_Lo	(long *)0x00000000
#define Flash_Hi	(long *)0x0801ffff
#define Sram_Lo	(long *)0x20000000
#define Sram_Hi	(long *)0x20003fff
#define Periph_Hi	(long *)0x40000000
#define Periph_Lo	(long *)0x40026400

/*
 * mem shows a list of bytes (arg 2) from base hex address (arg 1)
 * This is atiomisable as there might be a lot of memory to be displayed
 */
void mem(unsigned char pindex)
{

	long *address = 0;
	long value;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		// read hex string and convert to address
		if (argc > 1)
			{
			address = (long *)HextoBin(argv[1]);

			// check range as we dont want to throw an exception

			if (((address >= Flash_Lo) && (address < Flash_Hi)) || \
					((address >= Sram_Lo) && (address < Sram_Hi)) || \
					((address >= Periph_Lo) && (address < Periph_Hi)))
					{
					value = (long) *address;
					print_hex((long) address, 8);
					print_str(": ");
					print_hex(value, 8);
					print_str("\r\n");
					}
			else
				print_str("Illegal address entered\r\n");
			}
		else
			print_str("no address specified\r\n");
		pcb[pindex].pstate++;
		break;
	case 0xff:			// destructor
		procterm(pindex);
		break;
	default:		// do the actual function (atomised)
		pcb[pindex].pstate = 0xff;
		break;
	}
}

// runs current test from ram, saved test from flash or macro
// this spawns a background task. It finishes, but sets off

void run(unsigned char pindex)
{
	unsigned char * saveptr;
	unsigned char * testptr;
	unsigned char toffset;

	int i;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm (pindex);
		break;
	default:		// do the actual function (atomised)

		if (argc == 1)
		{
			runtest();
			pcb[pindex].pstate = 0xff;
			break;
		}

		if ((argv[1][0] >= '1') && (argv[1][0] <= '8'))
			{
			print_str("running test ");
			print_str(argv[1]);
			print_str("\r\n");

			// copy test from flash - block arg[1]

//			CopyTest(argv[1][0] & 0x0f - 1);			// Test required 0-7
// Note tests 3 and 7 are not available from TEST pins

			testptr = (unsigned char * ) &test_block;
			toffset = (unsigned char) (argv[1][0] & 0x0f) - 1;
			saveptr = (unsigned char *) testss;
			saveptr += (sizeof(test_block) * toffset);

			for (i = 0; i < sizeof (test_block); i++)
				*testptr++ = *saveptr++;
			runtest();
			}
		else
			{
			// look for macro name (argv[1]) in Macros[]
			if ((MacroRPtr = Strnfind((char *) Macros, argv[1], sizeof Macros)) != 0)
			{
				print_str("running macro ");
				print_str(argv[1]);
				print_str("\r\n");
				MacroMode = M_RUN;
				// run commands from script through macro daemon as pshell does
				// commands have already been validated and tokenised
				spawn (3, MACRO_SLOT);
				procterm (pindex);	// terminate run command
			}
			else
				print_str ("macro not found\r\n");
			}
		pcb[pindex].pstate = 0xff;
		break;
	}
}

void runtest()
{
	if (pcb[TEST_SLOT].pmode != STOPPED)
		print_str("Test already running\r\n");
	else
	{
		spawn (1, TEST_SLOT);					// spawn the process
	}
}


// something to do - will be real test  from test spec and state machine
//
void stodo(void)
 {

/*
 * function to perform the test as specified.

 * Set up the timer request 6s
 * Perform the transmitter on
 * Set up the timer request as per period setting
 * Perform the transmitter off
 */

#define SI446X_SLEEP	0x01
#define SI446X_READY	0x03
#define	 SI446X_TX		0x07

}

// This is a bg task kicked off by run
// It is the main test specification running task. It is what diags is really all about.
// delays will need to be added to allow for setup times etc

/*
* Set up frequency of the Si4464
* Set up the load,
* Set up the bias
* Set up the Antennae compensation
* Set up the modulation mode
* Set up the power am
* Set up the power lvl
* Perform the state machine...'scheduler'
*/

void test(unsigned char pindex)
{
//	int tuneresp;
	int load;
	int i;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		// check freq, h/w and stuff in accordance with current test specification
		// or may need to do some here and some in state machine to allow for delays
		//
		if ((test_block.freq == 0) || \
			(test_block.bias == 0) || \
			(test_block.pwr_am == 0) || \
			(test_block.pwr_lvl == 0) || \
			(test_block.band == 0))
			{
			print_str("value(s) not set, type status to check\r\n");
// TODO put back			procterm(pindex);
			}
			break;
	case 0x01:								// set up the ports
		for (i = 1; i < 16; i++)
			{
			if (test_block.porta[i] == 0)
//				GPIO_WriteBit(portlist[0], 0x0001 << i, 0)
				;
			if (test_block.porta[i] == 1)
//				GPIO_WriteBit(portlist[0], 0x0001 << i, 1)
				;
			}
		for (i = 1; i < 16; i++)
			{
			if (test_block.portb[i] == 0)
//				GPIO_WriteBit(portlist[1], 0x0001 << i, 0)
				;
			if (test_block.portb[i] == 1)
//				GPIO_WriteBit(portlist[1], 0x0001 << i, 1)
				;
			}
		for (i = 1; i < 16; i++)
			{
			if (test_block.portc[i] == 0)
//				GPIO_WriteBit(portlist[1], 0x0001 << i, 0)
				;
			if (test_block.portc[i] == 1)
//				GPIO_WriteBit(portlist[1], 0x0001 << i, 1)
				;
			}
		pcb[pindex].pstate++;
		break;
	case 0x02:
//		vRadio_Init(MODULATION_121_5_CW,1);		// set up the modulation type
//		while (radio_comm_PollCTS() != 0xff);	// wait for CTS

	    // do a cheeky monitor on the TIM9, TIM3 chain to see if they are working...

//	    print_hex(TIM_GetCounter(TIM3), h8);
//	    print_hex(TIM_GetCounter(TIM9), h8);
//	    print_str("\r\n");

		pcb[pindex].pstate++;
	    break;

	case 0x03:								// Set up the load
		load = test_block.band - 1;
/*
 		if (load & 0x01)
			GPIO_SetBits(LOAD0_GPIO, LOAD0_PIN);
		else
			GPIO_ResetBits(LOAD0_GPIO, LOAD0_PIN);

		if (load & 0x02)
			GPIO_SetBits(LOAD1_GPIO, LOAD1_PIN);
		else
			GPIO_ResetBits(LOAD1_GPIO, LOAD1_PIN);
*/
		pcb[pindex].pstate++;
		break;

#define SI_GPIO_NC		0x00		// no change
#define SI_GPIO_O_0		0x02		// set GPIO to 0
#define SI_GPIO_O_1		0x03		// set GPIO to 1

	case 0x04:								// set up the bias
//		if (test_block.bias == 'l')
//			si446x_gpio_pin_cfg(SI_GPIO_NC, SI_GPIO_NC, SI_GPIO_O_0, SI_GPIO_NC, 0x00, 0x00, 0x00);
//		if (test_block.bias == 'h')
//			si446x_gpio_pin_cfg(SI_GPIO_NC, SI_GPIO_NC, SI_GPIO_O_1, SI_GPIO_NC, 0x00, 0x00, 0x00);
//		pcb[pindex].pstate++;
		break;

	case 0x05:
 //		if (test_block.ant & 0x01)
//			GPIO_SetBits(DRY_GPIO, DRY_PIN);
//		else
//			GPIO_ResetBits(DRY_GPIO, DRY_PIN);

		pcb[pindex].pstate++;
		break;

	case 0x06:			//Set up frequency of the Si4464
//	    vRadio_Tune((unsigned long) test_block.freq);
//	    while (radio_comm_PollCTS() != 0xff);	// wait for CTS

/*		switch (test_block.mod)
		{
		case (1):							//
			vRadio_Init(MODULATION_406,1); // 406
			break;

		case (2):							// Homer
			vRadio_Init(MODULATION_121_5_CW,1);
		//		CalculateSweep(33);
			break;

		case (3):							//
			vRadio_Init(MODULATION_AIS_4GFSK_TX,1); //AIS
			break;
		}
			*/

	pcb[pindex].pstate++;
	break;

	case 7:							// set power AM

		// This is about setting the PWM for the port PB8 to the value in pwr_am at 120KHz
		// must user Timer10 for this. Set up the counter as pwr_am value
		/* Set the Capture Compare Register value */

//		TIM10->CCR1 = test_block.pwr_am / 3;
		pcb[pindex].pstate++;
		break;

	case 8:							// set power lvl and txramp output
//		si446x_set_property(
//		         		SI446X_PROP_GRP_ID_PA,
//		         		6,
//		         		SI446X_PROP_GRP_INDEX_PA_PWR_LVL ,
//		         		(unsigned char)(test_block.pwr_lvl),
//		         		SI446X_PROP_GRP_INDEX_PA_MODE,
//		         		(unsigned char)(0x88),
//		         		SI446X_PROP_GRP_INDEX_PA_BIAS_CLKDUTY,
//		         		(unsigned char)(0x00),
//		         		SI446X_PROP_GRP_INDEX_PA_RAMP_EX,
//		         		(unsigned char)(0xff),
//		         		SI446X_PROP_GRP_INDEX_PA_RAMP_DOWN_DELAY,
//		         		(unsigned char)(0x0f),
//		           		SI446X_PROP_GRP_INDEX_PA_TC ,
//		             	(unsigned char)(0x7f)			// slowest Ramp time 20uS
//		            	);
//		  while (radio_comm_PollCTS() != 0xff);	// wait for CTS


// if AIS mode
//	    vRadio_SetTransmitFilter(GMSK_BT0_4_Coeffs);

	    // encode AIS packet : each bit in AIS HDLC coded data
		  // becomes a symbol (Kipper)
		  // nominal frequency => 11 for runin, run out = +Fdev
		  // a '1'             => 10                    = +3Fdev
		  // a '0'             => 01                    = -Fdev

	    pcb[pindex].pstate = 100;		// scheduler starts at state 100 to allow setup expansion
	    break;

// run scheduler....similar to main live architecture but this is one tx at a time
// here just the modulation type changes.

	case 100:							// initial delay prior to first tx
//		_delay_ms(100);					// must be refactored for multi-tasking
//		si446x_start_tx(0x00, 0, 0);
//		radio_comm_PollCTS();
		pcb[pindex].pstate++;
		break;

	case 101:							// register tx on in 20mS with diag_txon_cb
		if (test_block.mod == 8)
		{
//			si446x_change_state(0x07); // tx state
			pcb[pindex].pstate = 105;	// stay in state 105 until terminated
		}
		else
			pcb[pindex].pstate++;
		break;

	case 102:							// set up data if any in accordance with tx mode
//		if (AIS)
//		BuildMessage();					// from GPS

		if (ppsflg == 0xff)
		{
			ppsflg = 0;
//			print_hex (TIM_GetCounter(TIM3) << 16 + TIM_GetCounter(TIM9), h8);
//		   	print_hex (TIM_GetCounter(TIM3), h4);
//		   	print_hex (TIM_GetCounter(TIM9), h4);
		}
		stodo();

		/*int timerValue = TIM_GetCounter(TIM6);

	      if (timerValue <= test_block.p_dur)
	      	  {
	    	  offflag=0;
	    	  if (onflag == 0)
	    	  	  {
	    		  GPIO_SetBits(R_LED_GPIO, R_LED_PIN);		// Blips the red led on startup @@@ debug

	    		  si446x_change_state(SI446X_TX); // tx state
	    		  onflag++;
	    	  	  }
	      	  }
	   */
		pcb[pindex].pstate++;
		break;

	case 103:							// register tx off in test_block.period mS with diag_txoff_cb
/*
		int timerValue = TIM_GetCounter(TIM6);

	      if (timerValue >  test_block.p_dur)
      	  {
    	  onflag = 0;
    	  if (offflag == 0)
    	  	  {
    		  GPIO_ResetBits(R_LED_GPIO, R_LED_PIN);		// Blips the red led on startup @@@ debug
	          si446x_change_state(SI446X_READY); 			// ready state
	          offflag++;
    	  	  }
     // read the adc channels too
      }

*/
		pcb[pindex].pstate++;
		break;

//txon scheduled and run when timer expires

//        vRadio_StartTx121_5CW(
//                         pRadioConfiguration_AIS_4GFSK_TX->Radio_ChannelNumber, NULL);

	case 104:							// register tx on again in 6 seconds
		pcb[pindex].pstate = 102;
		break;

	case 105:							// stay here forever - well until stop typed
		break;

	case 0xff:								// destructor
		// turn the si4464 to ready state
//        si446x_change_state(0x03); // ready state
        // turn anything else off?
//		si446x_set_property(
//		         		SI446X_PROP_GRP_ID_PA,
//		         		3,
//		         		SI446X_PROP_GRP_INDEX_PA_PWR_LVL ,
//		         		(unsigned char)(0x00),
//		         		SI446X_PROP_GRP_INDEX_PA_MODE,
//		         		(unsigned char)(0x08),
//		           		SI446X_PROP_GRP_INDEX_PA_TC ,
//		             	(unsigned char)(0x1f)			// slowest Ramp time 20uS
//		            	);
		procterm(pindex);
	    break;
	default:		// do the actual function (atomised) - does not terminate itself
		// perform the actual state machine for the test context in test_block
		// does not terminate - run until cntl Z or stop entered
		print_str("ooops");
		procterm(pindex);
		break;
	}
}

// callbacks for diag scheduler

void diag_txon_cb(void)
{

}

void diag_txoff_cb(void)
{

}


// background task to halt current command (which could be a macro) and re-start pshell
// on entry of cntrl z. Also stops the GNSS receiver
// only need to run this when bg tasks are running.

void sigstpd (unsigned char pindex)
{
	if (SigStp)
		{
		if (MacroMode == M_RUN)
			pcb[MACRO_SLOT].pmode = STOPPED;	// stop the macro daemon

		MacroMode = M_OFF;

		pcb[TEST_SLOT].pstate = 0xff;		// let background test process gracefully shutdown
		pcb[PSH_SLOT].pmode = RUNNING;		// restart pshell
		pcb[PSH_SLOT].pstate = 0;		// restart pshell
//		USART_Cmd(GPS_USART, DISABLE);	// TODO this could be a background task
		SigStp = 0;
		}
}

void macrod (unsigned char pindex)
{
	int index;

	parse(MacroRPtr);
	while (*MacroRPtr++ != 0x03);		// advance MacroRPtr to next command in macro
	index = getstrid((char *) BuiltIns[0], argv[0]);
	spawn (index + 5, CMD_SLOT);		// only use slot 2 for foreground commands
	pcb[MACRO_SLOT].pmode = SUSPENDED;	// suspend Macro daemon until command completes in fg
}

// performs background gnss acquisition for tx messages

void backgnd (unsigned char pindex)
{
// if state = ff, then print the parser results out

	if (GPSChar != 0)
	{
		PutChar(GPSChar);
//		NMEA(GPSChar);       // process whole FIFO
		GPSChar = 0;
	}
}


// stops currently running test or macro

void stop(unsigned char pindex)
{
	procterm(pindex);					// destructor
//	USART_Cmd(GPS_USART, DISABLE);		// TODO this must be a bg task
	pcb[pindex].pstate = 0xff;
	pcb[TEST_SLOT].pstate = 0xff;		// shutdown background tasks nicely
	pcb[MACRO_SLOT].pmode = STOPPED;	// stop the macro daemon
}

void status(unsigned char pindex)
{
	int	index;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:					// destructor
		procterm(pindex);
		break;
	default:		// do the actual function (atomised)
		print_str("PortA: ");
		for (index = 15; index >= 0; index-- )
		{
			switch(test_block.porta[index])
			{
			case 0:
				print_str("0");				// only get set if explicit write using port
				break;
			case 1:
				print_str("1");				// only get set if explicit write using port
				break;
			case 255:
				print_str("-");
				break;
			default:
				print_str("X");				// valid output port but not changed in test spec
				break;						// from default value in init.c.
			}
			if (index == 12 || index == 8 || index == 4)
				print_str(" ");
		}
		print_str("\r\n");
		print_str("PortB: ");
		for (index = 15; index >= 0; index-- )
		{
			switch(test_block.portb[index])
			{
			case 0:
				print_str("0");
				break;
			case 1:
				print_str("1");
				break;
			case 255:
				print_str("-");
				break;
			default:
				print_str("X");
				break;
			}
			if (index == 12 || index == 8 || index == 4)	// format as per SRS
				print_str(" ");
		}
		print_str("\r\n");
		print_str("PortC: ");
		for (index = 15; index >= 0; index-- )
		{
			switch(test_block.portc[index])
			{
			case 0:
				print_str("0");
				break;
			case 1:
				print_str("1");
				break;
			case 255:
				print_str("-");
				break;
			default:
				print_str("X");
				break;
			}
			if (index == 12 || index == 8 || index == 4)	// format as per SRS
				print_str(" ");
		}
		print_str("\r\n");
		print_str("Antenna: ");
		print_str(getcmdstr((char *) Ant[0], test_block.ant));
		print_str("\r\n");
		print_str("frequency: ");
		print_dec(test_block.freq);
		print_str("\r\n");
		print_str("Band: ");
		print_str(getcmdstr((char *) Bands[0], test_block.band));
		print_str("\r\n");
		print_str("PwrAM: ");
		print_dec(test_block.pwr_am);
		print_str("\r\n");
		print_str("Bias: ");
		PutChar(test_block.bias);
		print_str("\r\n");
		print_str("Drv: ");
		print_dec(test_block.pwr_lvl);
		print_str("\r\n");
		print_str("TXtype: ");
		print_str(getcmdstr((char *) Mods[0], test_block.mod));
		print_str("\r\n");
		if (test_block.mod == 11)
		{
			print_str("Period: ");
			print_dec(test_block.p_dur);
			print_str("\r\n");
		}
		if (test_block.mod == 10)
		{
			print_str("Low Sine: ");
			print_dec(test_block.lowsine);
			print_str("\r\n");
			print_str("High Sine: ");
			print_dec(test_block.hisine);
			print_str("\r\n");

		}
		print_str("Current GPS status\r\n");
		print_str("Qual: ");
		PutChar(Qual);
		print_str("\r\n");
		print_str("NSAT: ");
		print_dec(nsat);
		print_str("\r\n");
		print_str("HDOP: ");
		print_dec(hdop);
		PutChar('.');
		print_dec(hdopdec);
		print_str("\r\n");
		print_str("Long:");
		print_dec(Xdeg);
		PutChar('.');
		print_dec(Xmin);
		print_str("\r\n");
		print_str("Lat:");
		print_dec(Ydeg);
		PutChar('.');
		print_dec(Ymin);
		print_str("\r\n");
		print_str("UTC: ");
		print_dec(HH);
		PutChar(':');
	    print_dec(MM);
		PutChar(':');
	    print_dec(SS);
		print_str("\r\n");

		pcb[pindex].pstate = 0xff;
		break;
	}
}


// read a channel of the adc.

void adc(unsigned char pindex)
{
	long value = 0x30;
	int chan;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		if (argc < 2)
			{
			print_str("Channel not specified\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm(pindex);					// nothing to do here except terminate
		break;
	default:		// do the actual function (atomised)
		chan = argv[1][0] & 0x0f;
		if (argv[1][1] != 0x00)
			chan *= 10;
			chan += argv[1][1] & 0x0f;
		if (chan > 25)
		{
			print_str ("invalid channel");
			print_dec(chan);
			print_str("\r\n ");
			pcb[pindex].pstate = 0xff;
			break;
		}

//		ADC_RegularChannelConfig(ADC1, chan, 1, ADC_SampleTime_24Cycles);
		// Start the conversion
//		ADC_SoftwareStartConv(ADC1);
		// Wait until conversion completion
//		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		// Get the conversion value
//		value = ADC_GetConversionValue(ADC1) * 3300/4096;
		print_dec(value);
		print_str ("\r\n");
		pcb[pindex].pstate = 0xff;
		break;
	}
}

void dac(unsigned char pindex)
{
	long value;
	int chan;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		if (argc < 3)
			{
			print_str("Insufficient args\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm(pindex);					// nothing to do here except terminate
		break;
	default:		// do the actual function (atomised)
		chan = argv[1][0] & 0x0f;
		if (chan > 2)
		{
			print_str ("invalid channel");
			print_dec(chan);
			print_str("\r\n ");
			pcb[pindex].pstate = 0xff;
			break;
		}

		value = DectoBin(argv[2]);

		if (value > 4096)
			print_str("value out of range");

//		if (chan == 1)
//			DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned int) value);
//		else
//			DAC_SetChannel2Data(DAC_Align_12b_R, (unsigned int) value);

//		ADC_RegularChannelConfig(ADC1, chan, 1, ADC_SampleTime_24Cycles);
		// Start the conversion
//		ADC_SoftwareStartConv(ADC1);
		// Wait until conversion completion
//		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		// Get the conversion value
//		value = ADC_GetConversionValue(ADC1) * 3300/4096;
//		print_dec(value);
//		print_str ("\r\n");
		pcb[pindex].pstate = 0xff;
		break;
	}
}
// this is now called tx in the spec

void tx(unsigned char pindex)
{
	int modn;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm(pindex);
		break;
	default:		// do the actual function (atomised)
		modn = getstrid((char *) Mods[0], argv[1]);
		if (modn == -1)
			print_str ("invalid tx mode\r\n ");
		else
			test_block.mod = modn;

		if (modn == 11)		// pulse mode
		{
			if (argc < 3)
				print_str("missing parameter\r\n");
			else
			{
				test_block.p_dur = DectoBin(argv[2]);
				if (test_block.p_dur < 10 || test_block.p_dur > 1000)
				{
					print_str ("Duration out of range\r\n");
					test_block.p_dur = 0;
				}
			}
		}
		if (modn == 10)		// sine mode
		{
			if (argc < 4)
				print_str("missing parameter(s)\r\n");
			else
			{
				test_block.lowsine = DectoBin(argv[2]);
				if (test_block.lowsine < 10 || test_block.lowsine > 100)
				{
					print_str ("Sine Lo out of range\r\n");
					test_block.lowsine = 0;
				}
				test_block.hisine = DectoBin(argv[3]);
				if (test_block.hisine < 100 || test_block.hisine > 300)
				{
					print_str ("Sine Hi out of range\r\n");
					test_block.hisine = 0;
				}
			}
		}
		pcb[pindex].pstate = 0xff;
		break;
	}
}


// puts pll to sleep
// clears everything - what is that exactly?

void reset(unsigned char pindex)
{
//    si446x_change_state(0x03); // ready state
	procterm(pindex);
}

// reads NMEA data from Max

void gnss(unsigned char pindex)
{
	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm(pindex);
		break;
	default:		// do the actual function (atomised)
//		USART_Cmd(GPS_USART, ENABLE);
		spawn(4, 4);	// set gnss going as bg task
		pcb[pindex].pstate = 0xff;
		break;
	}
}
// put value as argv[1] in freq location as binary in context
// leaves pll in rdy mode
// 9 digit input (range 159000000 - 500000000)

void colourbar(unsigned char pindex)
{
	unsigned long frequency;

		frequency = DectoBin (argv[1]);
		if ((frequency < 119000000) || (frequency > 1050000000))
			print_str("freqency out of range\r\n");
		else
			test_block.freq = frequency;
//        si446x_change_state(0x03); 		// leave PLL in RDY mode
		procterm(pindex);
}


// inputs pwr_am, bias, pwr_lvl
// Bias sets GPIO2 of 4464 to 0 (LOW PASS) or 1 (HIGH PASS)

void pwr(unsigned char pindex)
{
	unsigned int pwr_am;
	unsigned char pwr_lvl;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		if (argc < 3)
			{
			print_str("too few arguments\r\n");
			pcb[pindex].pstate = 0xff;
			}
		else
			pcb[pindex].pstate++;
		break;
	case 0x01:
		pwr_am = DectoBin (argv[1]);
		if (pwr_am > 330)
			{
			print_str("pwr am out of range\r\n");
			pcb[pindex].pstate = 0xff;
			}
		else
			{
			test_block.pwr_am = pwr_am;
			pcb[pindex].pstate++;
			}
		break;
	case 0xff:								// destructor
		procterm(pindex);
		break;
	default:		// do the actual function (atomised)
		if (argv[2][0] != 'l' && argv[2][0] != 'h')
			print_str("invalid Bias\r\n");
		else
			test_block.bias = argv[2][0];

		pwr_lvl = DectoBin (argv[3]);
		if (pwr_lvl > 127)
			print_str("pwr lvl out of range\r\n");
		else
			test_block.pwr_lvl = pwr_lvl;

		pcb[pindex].pstate = 0xff;
		break;
	}
}

// non-atomisable function spawned as process

void ver(unsigned char pindex)
{
	print_str(BUILD_VERSION);
	print_str("\r\n");
	procterm (pindex);
}

void alias(unsigned char pindex)
{
	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm(pindex);
		break;
	default:		// do the actual function (atomised)
		pcb[pindex].pstate = 0xff;
		break;
	}
}

// this commands acts as a label in a macro
// sets the single level loop pointer in the macro command list
// for endl command to take up.
// non-atomisable

void loop(unsigned char pindex)
{
	if (MacroMode == M_RUN)
	{
		MacroLPtr = MacroRPtr;
//	if (argv[1] != 0)
//		MacroLCount = (argv[1][0] & 0x0f) - 1;
//	else
//		MacroLCount = 0xff;
	}
	procterm (pindex);
}

// this is currently a yukky blocking delay but must remain as a fg process.

void sleep(unsigned char pindex)
{
	int delayms = 0;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		// take snapshot of timer, and set target value
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		procterm (pindex);
		break;
	default:		// do the actual function (atomised)
		// wait for the interrupt to occur
		delayms = DectoBin (argv[1]);
//		_delay_ms(delayms);					// foreground wait
		pcb[pindex].pstate = 0xff;
		break;
	}
}

// macro command to mark end of loop
// this command resets the MacroRPtr to the command after the previous loop.
//
void endl(unsigned char pindex)
{
	if (MacroMode == M_RUN /* && MacroLCount != 0*/)
		MacroRPtr = MacroLPtr;
//	if (MacroLCount != 0xff)
//		MacroLCount--;
	procterm (pindex);
}

// macro definition
// initially allow one macro only but put in capability for more in future

void macro(unsigned char pindex)
{
	char * nameptr = argv[1];

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		if (MacroMode == M_ENTER)
			{
			print_str ("cannot nest macros\r\n");
			break;
			}
		if (argc < 2)
			print_str ("missing macro name\r\n");
		else
			{
			// Todo if the name = show, then list out all macros
			while ((*MacroWPtr++ = *nameptr++) != 0x00);
			MacroMode = M_ENTER;
			}
		break;
	case 0xff:			// destructor
		procterm(pindex);
		break;
	default:
		// do the actual function (atomised)
		pcb[pindex].pstate = 0xff;
		break;
	}
}


//  ignores any arguments as not required.
//  stops and returns control to stdin if macro running otherwise
// terminates copy of commands into macro space and puts pshell back
// into foreground mode.

void endm(unsigned char pindex)
{
	if (MacroMode == M_RUN)
		pcb[MACRO_SLOT].pmode = STOPPED;	// stop the macro daemon

	MacroMode = M_OFF;

	procterm(pindex);
}

void band(unsigned char pindex)
{
	int band;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		pcb[pindex].pmode = STOPPED;
		pcb[PSH_SLOT].pmode = RUNNING;		// restart pshell
		break;
	default:		// do the actual function (atomised)
		band = getstrid((char *) Bands[0], argv[1]);
		if (band == -1)
			print_str ("invalid band\r\n ");
		else
			test_block.band = band;

		pcb[pindex].pstate = 0xff;
		break;
	}
}

// Some commands like this one specifially set/reset port pins. This could be
// done using the port command of course but setting / resseting this way is
// semantically tricky (consider sense and default level). How do we identify
// a single port pin for specific output from up to 3 x 16 bit ports worth?

void ant(unsigned char pindex)
{
	unsigned char ant;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		pcb[pindex].pmode = STOPPED;
		pcb[PSH_SLOT].pmode = RUNNING;		// restart pshell
		break;
	default:		// do the actual function (atomised)
		ant = getstrid((char *) Ant[0], argv[1]);
		if (ant == -1)
			print_str ("invalid ant\r\n ");
		else
			test_block.ant = ant;

		pcb[pindex].pstate = 0xff;
		break;
	}
}

void mru(unsigned char pindex)
{

}


// dac routes an anlog input from port as provided in arg 1 to the dac
// output (dedicated to the speaker port on the Rx board currently)
// this kicks off a b/g task

// could be done via DMA of course

void dactoadc(unsigned char pindex)
{
	int portno, pinno;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		if (argc < 2)
			{
			print_str("port not specified\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}
		if ((argv[1][0] >= 'A' && argv[1][0] <= 'F') ||
		    (argv[1][0] >= 'a' && argv[1][0] <= 'f'))
			pcb[pindex].pstate++;
		else
			{
			print_str("port id invalid\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}
		break;
	case 0xff:								// destructor
		pcb[pindex].pmode = STOPPED;
		pcb[PSH_SLOT].pmode = RUNNING;		// restart pshell
		break;
	default:		// do the actual function (atomised)

		portno = (argv[1][0] & 0x0f) - 1; // Todo range check
		pinno = 0;
		pinno += argv[1][1] & 0x0f;

		if (argv[1][2] != 0x00)
			{
			pinno *= 10;
			pinno += argv[1][2] & 0x0f;
			}

		if (argv[1][1] == 0x00 || pinno > 15)
			{
			print_str("invalid pin\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}

		pcb[pindex].pstate = 0xff;
		break;
	}
}
// saves a test specification into one of 8 flash stores
// SRS requirement number DTX-BLK-25

void save(unsigned char pindex)
{
	unsigned char *testptr;
	unsigned char *saveptr;
	unsigned char toffset;
	int status;

	int i;

	switch (pcb[pindex].pstate)
	{
	case 0 :								// constructor
		pcb[pindex].pstate++;
		break;
	case 0xff:								// destructor
		pcb[pindex].pmode = STOPPED;
		pcb[PSH_SLOT].pmode = RUNNING;		// restart pshell
		break;
	default:		// do the actual function (atomised)
		// only save test specs, not macros
		if ((argv[1][0] >= '1') && (argv[1][0] <= '8'))
		{
			print_str("saving test ");
			print_str(argv[1]);
			print_str("\r\n");

			// copy test to eeprom - block arg[1]

//			FLASH_ClearFlag(FLASH_FLAG_PGAERR | FLASH_FLAG_WRPERR | FLASH_FLAG_SIZERR | \
//			        FLASH_FLAG_OPTVERR | FLASH_FLAG_EOP); // clear error status
//			DATA_EEPROM_Unlock();

			testptr = (unsigned char * ) &test_block;
			toffset = (unsigned char) (argv[1][0] & 0x0f) - 1;
			saveptr = (unsigned char *) testss;
			saveptr += (sizeof(test_block) * toffset);

/*			for (i = 0; i < sizeof(test_block); i++)
			{
//				status = DATA_EEPROM_FastProgramByte((uint32_t) saveptr++, (uint8_t) *testptr++);
				if (status != FLASH_COMPLETE)
				{
					print_str("Error ");
					print_dec(status);
					print_str(" SR ");
//  				print_hex(FLASH->SR, 8);
					print_str("\r\n");
					break;
				}
			}
//			 DATA_EEPROM_Lock();
*/
		}
		else
			print_str("invalid test number\r\n");

		pcb[pindex].pstate = 0xff;
		break;
	}
}

/* port <A-x>[pin] [value]
 * port <A-x> reads type of all pins on port against uP id
 * port <A-x><pin> reads port status if i/o or Alt/Ana
 * port <A-x><pin> <value> sets pin to value if o/p
 */
void port(unsigned char pindex)
{
//	GPIO_TypeDef* GPIOx;
	uint16_t mode = 0;
	int pin;
	static int i, portno, pinno;
	unsigned int * ports[5];

	switch (pcb[pindex].pstate)
	{
	case 0 :		// constructor
		portno = 0;
		pinno = 0;
		if (argc < 2)
			{
			print_str("port not specified\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}
		if ((argv[1][0] >= 'A' && argv[1][0] <= 'E') ||
		    (argv[1][0] >= 'a' && argv[1][0] <= 'e'))
			pcb[pindex].pstate++;
		else
			{
			print_str("port id invalid\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}
		break;
	case 0xff:		// destructor
		procterm (pindex);
		break;
	default:		// actual process
		portno = (argv[1][0] & 0x0f) - 1; // Todo range check
		pinno = argv[1][1] & 0x0f;
		ports[0] = &test_block.porta[0];		// set the array of array pointers
		ports[1] = &test_block.portb[0];
		ports[2] = &test_block.portc[0];
		ports[3] = &test_block.portd[0];
		ports[4] = &test_block.porte[0];

		if (argv[1][2] != 0x00)
			{
			pinno *= 10;
			pinno += argv[1][2] & 0x0f;
			}

		if (pinno > 31)
			{
			print_str("invalid pin\r\n");
			pcb[pindex].pstate = 0xff;
			break;
			}

		if (argc == 2)
		{

		if (argv[1][1] == 0x00)
		{//scan through all the pins for this port

		for (i = 0; i < 32; i++)
			{
			print_str ("pin ");
			print_dec (i);
			pin = (portno << 8) | i;
			mode = (uint16_t) GPIO_DRV_GetPinDir(pin) ;
			switch (mode)
				{
				case (0):
					print_str (" in");
					break;
				case (1):
					print_str (" out");
					break;
				case (2):
					print_str (" alt");
					break;
				case (3):
					print_str (" ana");
					break;
				}
			print_str("\r\n");
			}
		}
		else
		{
			pin = (portno << 8) | pinno;
			mode = (uint16_t) GPIO_DRV_GetPinDir( pin) ;

			switch (mode)
			{
			case 0:
				print_str("IN ");
				pin = (portno << 8) | pinno;
				if (GPIO_DRV_ReadPinInput(pin))
					print_str ("1 ");
				else
					print_str ("0 ");
/*
				//				mode = (portlist[portno]->PUPDR >> pinno * 2) & 0x00000003;
				switch (mode)
					{
					case (0):
						print_str ("Hi Z");
						break;
					case (1):
						print_str ("Pull Up");
						break;
					case (2):
						print_str ("Pull Down");
						break;
					case (3):
						print_str ("Reserved");
						break;
					}
				print_str("\r\n");
*/
				break;
			case 1:
				print_str("OUT ");
				pin = (int) (portno << 8) | pinno;
				if (GPIO_DRV_ReadPinInput(pin))
					print_str ("1 ");
				else
					print_str ("0 ");
//				if ((portlist[portno]->OTYPER >> pinno) & 1)
//					print_str("Open Drain\r\n");
//				else
//					print_str("Push Pull\r\n");
				break;
			case 2:
				print_str("ALT\r\n");
				break;
			case 3:
				print_str("ANA\r\n");
				break;
			}
		}
		}
		else
			{
			pin = (portno << 8) | pinno;
			mode = (uint16_t) GPIO_DRV_GetPinDir(pin) ;
			if (mode == 1)

			{
				GPIO_DRV_WritePinOutput(pin, argv[2][0] & 0x01);
				ports[portno][pinno] = argv[2][0] & 0x01;	// set test specification port write
			}
			else
				print_str ("not an output port\r\n");
			}
		pcb[pindex].pstate = 0xff;		// done- so terminate
		break;
	}
}

// does not require atomising

void proc(unsigned char pindex)
{
	long * upver;
	long id;

	upver = (long *) device_id;
	id = *upver;
	print_str("micro controller version: ");
	print_hex(id, 8);
	print_str("\r\n");
	procterm (pindex);
}

// TODO some functions for utils.c......

int Strlen (char * strptr)
{
	int i;

	for (i = 0; *strptr++ != 00; i++);
	return (i);
}

// copy a string value until terminating 0x00 reached

void Strcpy (char* src, char * dest)
{
	while ((*dest++ = *src++) != 0x00);
//	*dest = 0x00;	// terminate string
}

//; copy a string of length Counter characters

void Strncpy (char* src, char * dest, int n)
{
	while (n--)
		*dest++ = *src++;
}

void Strcmp (char * strlist, char * target)
{
	char * lookingfor = target;

	while (*strlist != 0x03)
	{
		while (*strlist++ == *lookingfor++)
			if (*strlist == 0)
				break;
		lookingfor = target;
		strlist++;
	}
}

/*
 * Simple decimal to binary conversion.Does not check
 * ASCII input are digits at the moment (AJJ = 100!)
 */
long DectoBin(char * strptr)
{
	long result = 0;

	while (*strptr != 0x00)
	{
		result *= 10;
		result += (*strptr++ & 0x0f);
	}

	return (result);
}

long HextoBin (char * strptr)
{
	long result = 0;

	while (*strptr != 0x00)
	{
		result <<= 4;
		result += (*strptr > 0x39 ? ((*strptr & 0x0f) + 0x09) : *strptr & 0x0f);
		strptr++;
	}
	return (result);
}

char * BuiltIns[] = {
		"port\000"					// set a single digital bit output to 0 or 1 (off or on)
		"ls\000"					// list of commands
		"proc\000"					// processor version
		"echo\000"					// send a string to stdout
		"mem\000"
		"run\000"					// run [x] runs current context or saved context "x"
		"stop\000"					// stop current context
		"status\000"
		"adc\000"					// single adc channel read
		"reset\000"					// various levels of reset like init
		"colourbar\000"
		"pwr\000"
		"ver\000"					// f/w version
		"alias\000"					// alias facility
		"loop\000"					// some macro capability cos it's gonna be useful
		"delay\000"					// wait a while
		"endl\000"					// end loop
		"macro\000"					// start macro
		"endm\000"					// end macro
		"band\000"
		"ant\000"
		"tx\000"
		"dac\000"
		"save\000"					// save current test
		"mpu\000"
//		"log\000"					// reads the live log
//		"config\000"				// reads and sets config params
		"\003"
		};

char Invalid[] = {"invalid arguments\r\n\000"};
char * Unknown[] = {" not found\r\n"};
char * CmdRun[]	= {"Previous Command still running\r\n"};
char * Prompt[] = {">"};

/*
char CRLF = {0x0d, 0x0a, 0x00};
char Welcome[] = {"1066labs" 0x0d, 0x0a, 0x00};
char manstub = {"command [arg] [on|off]", 0x00};
char All = {"all", 0x00};
char AliasStr = {"aliases:", 0x0d, 0x0a, 0x00};
char Off = {"off", 0x00};
char On = {	"on", 0x00};
char Password[] =  {"R1ngdale", 0\000};
*/

/*
Aliases are set up as string pairs. A found alias on search will be followed by its target string. All aliases
may contain space characters. Aliases can also be aliased. The end of the alias list is denoted by ETX. Aliases
can be set up for any string substitution including commands (and command macros?) TBD
*/

char * Aliases[] =  {"pilot switch","1/01",
						"bonding","1/02",
						"pwr", "1/03",
						"power", "2/06","\003"};

char * Bands[] = {			// Sets PB6, PB7 to index value
		"off\000"
		"406\000"
		"ais\000"
		"121\000"
		"\003"
		};

char * Mods[] = {
		"off\000"
		"406\000"
		"hom\000"
		"ais\000"
		"ais1\000"
		"ais2\000"
		"ais3\000"
		"data\000"
		"cw\000"
		"am\000"
		"sine\000"
		"pulse\000"
		"\003"
		};

char * Ant[] = {
		"dry\000"
		"wet\000"
		"\003"
		};
