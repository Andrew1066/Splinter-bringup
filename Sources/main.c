/* ###################################################################
**     Filename    : main.c
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-06, 20:52, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "hid1_hid.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "osa1.h"
#include "gpio2.h"
#include "i2cCom1.h"
#include "flexTimer1.h"
#include "hid1.h"
#include "usbDsc1.h"
#include "usbFmw1.h"
#include "DbgCs1.h"
#include "dspiCom1.h"
#include "dspiCom2.h"
#include "uartCom1.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif
/* User includes (#include below this line is not maintained by Processor Expert) */

/*****************************************************************************
 * Constants and Macros
 *****************************************************************************/
const uint8_t buffStart[]    = "\n\r++++++++++++++++ UART Send/Receive Polling Example +++++++++++++++++\n\r";
const uint8_t bufferData1[]  = "\n\rType characters from keyboard, the board will receive and then echo them to ter";

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
	   uint8_t  rxChar          = 0;
	    uint32_t byteCountBuff   = 0;
	    uint32_t uartSourceClock = 0;
	    UART_Type * baseAddr     = UART2_BASE; // was BOARD_DEBUG_UART_BASEADDR;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  // Initialize the uart module with base address and config structure
#if 0
  CLOCK_SYS_EnableUartClock(BOARD_DEBUG_UART_INSTANCE);

   // Get working uart clock
   uartSourceClock = CLOCK_SYS_GetUartFreq(BOARD_DEBUG_UART_INSTANCE);

   // Initialize UART baud rate, bit count, parity and stop bit
   UART_HAL_SetBaudRate(baseAddr, uartSourceClock, 9600); // was BOARD_DEBUG_UART_BAUD
   UART_HAL_SetBitCountPerChar(baseAddr, kUart8BitsPerChar);
   UART_HAL_SetParityMode(baseAddr, kUartParityDisabled);
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
   UART_HAL_SetStopBitCount(baseAddr, kUartOneStopBit);
#endif

   // Enable the UART transmitter and receiver
   UART_HAL_EnableTransmitter(baseAddr);
   UART_HAL_EnableReceiver(baseAddr);

   // Inform to start polling example
   byteCountBuff = sizeof(buffStart);
   UART_HAL_SendDataPolling(baseAddr, buffStart, byteCountBuff);

   // Inform user of what to do
   byteCountBuff = sizeof(bufferData1);
   UART_HAL_SendDataPolling(baseAddr, bufferData1, byteCountBuff);

   while(true)
   {
       // Wait to receive input data
       if (kStatus_UART_Success == UART_HAL_ReceiveDataPolling(baseAddr, &rxChar, 1u))
       {
           // Send any character that received
           UART_HAL_SendDataPolling(baseAddr, &rxChar, 1u);
       }
   }
#endif
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
