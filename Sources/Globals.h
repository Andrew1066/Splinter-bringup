
// ================= GLOBAL VARIABLES ===============
// Declares static globals and configuration  
// 
//
// NOTE: Compiler is expected to reset all variables on power up.
//       Ensure \Build options\Linker\Initialise data is ticked
//
// HISTORY
// 05.01.11   	Copy code from AIS SART issue 1
// 02.02.11	Add new bit flags GAP, SYNCD
// 07.02.12 Added macro TRAINING_BUILD to allow a training build
//
// ==================================================

// configuration bits
// set this to 1 for a training build 
// set this to 0 for a live build
// a training build does not transmit in either normal or self-test mode 
// Normal mode : after 30 seconds switches to 'GPS fix' mode. Never writes runtime to NVRAM.
// Selftest mode : will not transmit. Passes test without transmit
// Long GPS Selftest mode : will not transmit. Waits for GPS position fix . Passes test without transmit.
// ATE mode : all tests should function as normal including transmit test
//
#define TRAINING_BUILD 		0



// MASTER GLOBALS
extern unsigned long		MMSI; 				// Stores MMSI in binary 
extern          long		randx;				// 32 bit random number for slotoffset

extern unsigned int		TXMSG[28]; 			// Array for building in situ TX message
extern          int		Txlen;				// length of current TXMSG (19,21,28)

extern unsigned int		slot;				// 0000..2249 master TDMA slot count
extern unsigned int		slotoffset;			// 2025..2475 offset for next 8 bursts
extern unsigned int		TXprep;	   			// 0000..2249 Slot before TXslot
extern unsigned int		TXslot;				// 0000..2249 Next TX slot
extern unsigned int		GPSonSlot;			// 0000..2249 Turns GPS on before burst
extern unsigned int		tick;				// 0,1 set by Timer1 every 27ms

extern unsigned int		timeout;			// 7..0 minute count sets sub-message	
extern unsigned int		pulse;				// 1..8 within burst, 0 between bursts	
extern unsigned int		TXgood;				// 0,1..8 counts pulses above DET thd
extern unsigned int		bursts;				// 0..27 counts TX bursts for GPS on/off
extern unsigned int		searching;			// 0..3 GPS searching for N bursts
extern unsigned int		GPSage;				// 0,1..27 fix age in bursts (minutes)
extern          int		OFFcount;			// 0..75 accumulates 2s if TEST/OFF pressed
extern unsigned int		delay;  			// General delay N*27ms

extern unsigned int		DACval;				// value to be written to the DAC

extern unsigned int		CAL;				// current CAL scale-down for DAC
extern unsigned int		Acal;				// CAL value for 161 TCXO (typ 125 for Golledge)
extern unsigned int		Bcal;				// CAL value for 162 TCXO

extern unsigned int		row;				// 0..7 row number in notional NVM array
extern unsigned int		ptr;				// FFFF shifted 0 ptr to valid NVM posn on this row
extern unsigned int		col;				// 0..15 column number in notional NVM array  
extern unsigned int		val;				// FFFF value stored at the 'posn' in NVM array

// CONTROL FLAGS
extern  unsigned char	FIXED;				// set forever once GPS has achieved a fix
extern  unsigned char	SYNCD;              // set forever once PPS has done FULL SYNC
extern  unsigned char	INLOCK;				// TX is adequately sync'd to UTC
extern  unsigned char	LIVE;				// LIVE mode is active
extern  unsigned char	PRESSED;			// collects TEST Key presses
extern  unsigned char	GAP;     	  		// collects Gaps in Key presses
extern  unsigned char	BATTERY;			// collects NEW Key presses
extern  unsigned char	FAULT;				// fault condition, PIC unexpectedly still on

extern unsigned char ppsflg;
extern unsigned long lftick;
extern unsigned int _6sfromnow;			//
extern unsigned int		_10msfromnow;		//
extern unsigned int		_5msfromnow;
extern unsigned long		batadc, fwdadc, revadc;


// GPS VALID DATA	                                // individual variables much more code efficient
extern unsigned int 	UTChrs,UTCmin,UTCsec;                   // current timestamp for sub-message (23,59,59)
extern unsigned int 	LATns,LATdeg,LATmin,LATdec;             // last valid LATITUDE  (NS,89,59,9999)
extern unsigned int 	LONew,LONdeg,LONmin,LONdec;             // last valid LONGITIDE (EW,179,59,9999)
extern unsigned int 	SOG,COG;                                // last valid SOG,COG x10 (1023,3599)

// NMEA WORKING DATA
extern unsigned int 	HH,MM,SS,DD;                            // DD is decimal seconds - used for coarse sync
extern unsigned int	Yns,Ydeg,Ymin,Ydec;                     // exact mimic of LATITUDE
extern unsigned int	Xew,Xdeg,Xmin,Xdec;                     // exact mimic of LONGITUDE
extern unsigned int	sog,cog;                                // exact mimics of SOG,COG
extern unsigned char	CS, CSok, csum, Qual;                   // for checksum validation
extern int             field, digit;                           // flow control	
extern unsigned int nsat, hdop, hdopdec;					  // working NSAT, HDOP
extern unsigned int NSAT, HDOP, HDOPDEC;					  // last valid NSAT, HDOP

extern char RxChar;								// simple single byte ISR interface character (for now)
extern char GPSChar;								// new character (0=none)
extern char MacroChar;								// characters stream from a macro
extern unsigned char SigStp;						// Signal to stop cli process (eg infinite macro)
extern unsigned char SigErr;						// pass back builtin error or 0 for no error

