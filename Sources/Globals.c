
// ================= GLOBAL VARIABLES ===============
// Allocates storage for static globals   
// Initializes non-zero globals
// Flag bits initialized in INIT 
//
// NOTE: Compiler is expected to reset all variables on power up.
//       Ensure \Build options\Linker\Initialise data is ticked
//
// HISTORY
// 05.01.11   	Copy code from AIS SART issue 1
// 02.02.11	Add new bit flags GAP, SYNCD
//
// ==================================================
// include declarations to ensure definitions are compatible
#include "Globals.h"

// MASTER GLOBALS
         long		randx;				// 32 bit random number for slotoffset

unsigned int		TXMSG[28]; 			// Array for building in situ TX message
         int		Txlen;				// length of current TXMSG (19,21,28)

unsigned int		slot;				// 0000..2249 master TDMA slot count
unsigned int		slotoffset;			// 2025..2475 offset for next 8 bursts
unsigned int		TXprep;	   			// 0000..2249 Slot before TXslot
unsigned int		TXslot;				// 0000..2249 Next TX slot
unsigned int		tick;				// 0,1 set by Timer1 every 27ms

unsigned int		timeout;			// 7..0 minute count sets sub-message	
unsigned int		pulse;				// 1..8 within burst, 0 between bursts	
unsigned int		TXgood;				// 0,1..8 counts pulses above DET thd
unsigned int		bursts;				// 0..27 counts TX bursts for GPS on/off
unsigned int		searching;			// 0..3 GPS searching for N bursts
unsigned int		GPSage;				// 0,1..27 fix age in bursts (minutes)
         int		OFFcount;			// 0..75 accumulates 2s if TEST/OFF pressed
unsigned int		delay;  			// General delay N*27ms

unsigned int		DACval;				// value to be written to DAC
unsigned int		CAL;				// current CAL scale-down for DAC
unsigned int		Acal;				// CAL value for 161 TCXO (typ 125 for Golledge)
unsigned int		Bcal;				// CAL value for 162 TCXO

unsigned int		row;				// 0..7 row number in notional NVM array
unsigned int		ptr;				// FFFF shifted 0 ptr to valid NVM posn on this row
unsigned int		col;				// 0..15 column number in notional NVM array  
unsigned int		val;				// FFFF value stored at the 'posn' in NVM array

// CONTROL FLAGS
unsigned char 	FIXED;				// set forever once GPS has achieved a fix
unsigned char		SYNCD;              // set forever once PPS has done FULL SYNC
unsigned char		INLOCK;				// TX is adequately sync'd to UTC
unsigned char		LIVE;				// LIVE mode is active
unsigned char		PRESSED;			// collects TEST Key presses
unsigned char		GAP;     	  		// collects Gaps in Key presses
unsigned char		BATTERY;			// collects NEW Key presses
unsigned char		FAULT;				// fault condition, PIC unexpectedly still on

unsigned char		ppsflg;				//
unsigned long		lftick;				// tick counter for peripheral / UI delays
unsigned int		_6sfromnow;			//
unsigned int		_10msfromnow;		//
unsigned int		_5msfromnow;
unsigned long		batadc, fwdadc, revadc;


// GPS VALID DATA	                                // individual variables much more code efficient
unsigned int 	UTChrs,UTCmin,UTCsec;                   // current timestamp for sub-message (23,59,59)
unsigned int 	LATns,LATdeg,LATmin,LATdec;             // last valid LATITUDE  (NS,89,59,9999)
unsigned int 	LONew,LONdeg,LONmin,LONdec;             // last valid LONGITIDE (EW,179,59,9999)
unsigned int 	SOG,COG;                                // last valid SOG,COG x10 (1023,3599)
unsigned int 	NSAT, HDOP, HDOPDEC;					  // last valid NSAT, HDOP

// NMEA WORKING DATA
unsigned int 	HH,MM,SS,DD;                            // DD is decimal seconds - used for coarse sync
unsigned int	Yns,Ydeg,Ymin,Ydec;                     // exact mimic of LATITUDE
unsigned int	Xew,Xdeg,Xmin,Xdec;                     // exact mimic of LONGITUDE
unsigned int	sog,cog;                                // exact mimics of SOG,COG
unsigned char	CS, CSok, csum, Qual;                   // for checksum validation
int             field, digit;                           // flow control	
unsigned int nsat, hdop, hdopdec;					  // last valid NSAT, HDOP
unsigned int alt, sep;						  // last valid altitude, separation
unsigned char ualt, usep;					  // altitudea and separation units
unsigned int diffage, diffstat;			 // differential corrections age, id of station if DGPS

unsigned char sentence;					// sentence type received


char RxChar;						// new character (0=none)
char GPSChar;						// new character (0=none)
unsigned char SigStp;				// signal a stop on current process
unsigned char SigErr;				// pass back builtin error or 0 for no error
