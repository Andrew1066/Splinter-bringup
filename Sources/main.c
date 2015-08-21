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
#include "msd1_msd.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "osa1.h"
#include "gpio2.h"
#include "i2cCom1.h"
#include "flexTimer1.h"
#include "usbDsc1.h"
#include "usbFmw1.h"
#include "dspiCom1.h"
#include "dspiCom2.h"
#include "dmaController1.h"
#include "msd1.h"
#include "DbgCs1.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "diags.h"
#include "Globals.h"
#include "splinter.h"
#include "fsl_gpio_driver.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_debug_console.h"
#include "colourbar.h"
#include "fsl_dspi_master_driver.h"
//#include "lcos_data.h"
#include "lcos.h"

/*****************************************************************************
 * Constants and Macros
 *****************************************************************************/
char buffStart[]    = "Splinter Board Diagnostic Tool\n\r\0";
char bufferData1[]  = "\n\rType characters from keyboard, the board will receive and then echo them to terminal\0";

extern void diags(void);
extern void pcbinit(void);
extern int spawn (int,int);

extern unsigned char GetChar(void);
extern void PutChar (char);
extern void print_str(char *);

extern uint8_t lm3549_right_init_regs[];

extern i2c_status_t init_lcos_ctrlr(uint8_t instance);

#define STANDARD_IO	1
//#define BASIC_IO
#define BOARD_I2C_COMM_INSTANCE 0

#define DATA_LENGTH             64

// Buffer store data to send to slave
uint8_t txBuff[DATA_LENGTH] = {0};
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};

dspi_device_t g_dspiDevice;
dspi_master_user_config_t dspiConfig;

dspi_master_state_t							g_spi_state[2];
/*!
 * @brief compare the data received with data sent.
 * @param source The pointer to send buffer
 * @param sink The pointer to received buffer
 * @param count The number of bytes to be compared
 * @return true or false
 */
bool i2c_compare(uint8_t *txBuff, uint8_t *rxBuff, uint32_t count)
{
    uint32_t i;
    /* Compare source and sink data*/
    for (i = 0; i < count ; i++)
    {
        if (txBuff[i] != rxBuff[i])
        {
            return false;
        }
    }
    return true;
}

#define DSPI_BUFFER_SIZE 16
static uint8_t dspi_buffer_out[DSPI_BUFFER_SIZE];
static uint8_t dspi_buffer_in[DSPI_BUFFER_SIZE];
#define min(a,b) ((a) < (b)? a: b)

int SHV_IMU_SendData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t*data)
{
	dspi_status_t status;
	int i;
	if ( addr > 1 )
		return -1;
#if DUMP_REG
	sprintf(buffer, "%12d imu(%d)> %02x [", OSA_TimeGetMsec(), addr, reg, len);
	swo_puts(buffer);
	for(i = 0; i < len; i++){
		sprintf(buffer, "%02x ", data[i]);
		swo_puts(buffer);
	}
#endif
	while(len)
	{
		uint8_t chunk = min(DSPI_BUFFER_SIZE - 1, len);
		dspi_buffer_out[0] = reg;
		memcpy(dspi_buffer_out + 1, data, chunk);
		GPIO_DRV_ClearPinOutput(MPU_SPI_CS);
		status = DSPI_DRV_MasterTransferBlocking(addr, &g_dspiDevice, dspi_buffer_out, dspi_buffer_in, chunk + 1, 10);
		GPIO_DRV_SetPinOutput(MPU_SPI_CS);
		if ( status )
		{
			print_str("SPI transfer failed\n");
			break;
		}
		//memcpy(data, dspi_buffer_in + 1, chunk);
		data += chunk;
		len -= chunk;
		reg += chunk;
	}
#if DUMP_REG
	sprintf(buffer, "] %d\n", status);
	swo_puts(buffer);
#endif
	return status;
}

int SHV_IMU_ReadData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data)
{
	dspi_status_t status;
	int i = len;
	uint8_t *p = data;
	uint32_t speed = 1000000;
	if ( addr > 1 )
		return -1;
#if DUMP_REG
	sprintf(buffer, "%12d imu(%d)< %02x (%d) [", OSA_TimeGetMsec(), addr, reg, len);
	swo_puts(buffer);
#endif
#if 0
	if ( reg >= 114 && reg <= 116 ) {
		speed = g_dspiDevice[i].bitsPerSec;
		g_dspiDevice[i].bitsPerSec = 1000000;
	}
#endif
	while(len)
	{
		uint8_t chunk = min(DSPI_BUFFER_SIZE - 1, len);
		dspi_buffer_out[0] = reg | 0x80;
		GPIO_DRV_ClearPinOutput(MPU_SPI_CS);
		status = DSPI_DRV_MasterTransferBlocking(addr, &g_dspiDevice, dspi_buffer_out, dspi_buffer_in, chunk + 1, 10);
		GPIO_DRV_SetPinOutput(MPU_SPI_CS);
		if ( status )
		{
			print_str("SPI transfer failed\n");
			break;
		}
		memcpy(data, dspi_buffer_in + 1, chunk);
		data += chunk;
		len -= chunk;
		reg += chunk;
	}
	len = i;
#if DUMP_REG
	for(i = 0; i < len; i++){
		sprintf(buffer, "%02x ", p[i]);
		swo_puts(buffer);
	}
	sprintf(buffer, "] %d\n", status);
	swo_puts(buffer);
#endif
#if 0
	if ( reg >= 114 && reg <= 116 ) {
		g_dspiDevice.bitsPerSec = speed;
	}
#endif
	return status;
}


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
//#define DISABLE_WDOG

  /* Write your local variable definition here */
	   uint8_t  rxChar          = 0;
	   UART_Type * baseAddr;
	   ;

	    // Number byte data will be transfer
	    uint32_t count = 0;
	    uint32_t i = 0;
	    // i2c master state
	    i2c_master_state_t master;
	    i2c_status_t status = kStatus_I2C_Success;
	    // i2c device configuration
	    i2c_device_t device =
	    {
	      .address = 0x36U,
	      .baudRate_kbps = 100   // 400 Kbps
	    };
     	uint8_t buffer_in[2] = { 0xf5, 0x00 };
     	uint8_t buffer_out[2] = { 0x00, 0x00 };
     	uint8_t val = 0x50;

        uint32_t calculatedBaudRate;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */

   CLOCK_SYS_DisableEwmClock(0);

//    i2c_init_all();

//   init_devices();


//  #if 0
   baseAddr     = (UART_Type *) UART1_BASE; // was BOARD_DEBUG_UART_BASEADDR;
   g_dspiDevice.dataBusConfig.bitsPerFrame = 8;
   g_dspiDevice.dataBusConfig.clkPhase = kDspiClockPhase_SecondEdge;
   g_dspiDevice.dataBusConfig.clkPolarity = kDspiClockPolarity_ActiveLow;
   g_dspiDevice.dataBusConfig.direction = kDspiMsbFirst;
   g_dspiDevice.bitsPerSec = 1000000; /* 100KHz */

   dspiConfig.isChipSelectContinuous = false;
   dspiConfig.isSckContinuous = false;
   dspiConfig.pcsPolarity = kDspiPcs_ActiveLow;
   dspiConfig.whichCtar = kDspiCtar0;
   dspiConfig.whichPcs = kDspiPcs0;
   calculatedBaudRate = TPI->ACPR;

   // Initialize the uart module with base address and config structure
  CLOCK_SYS_EnableUartClock(STANDARD_IO);

   // Get working uart clock
//   uartSourceClock = CLOCK_SYS_GetUartFreq(STANDARD_IO);

   // Initialise UART baud rate, bit count, parity and stop bit
//   UART_HAL_SetBaudRate(baseAddr, 24000000, 9600); // was BOARD_DEBUG_UART_BAUD
   UART_HAL_SetBitCountPerChar(baseAddr, 8);
   UART_HAL_SetParityMode(baseAddr, kUartParityDisabled);
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
   UART_HAL_SetStopBitCount(baseAddr, kUartOneStopBit);
#endif

   // Enable the UART transmitter and receiver
   UART_HAL_EnableTransmitter(baseAddr);
   UART_HAL_EnableReceiver(baseAddr);
//#endif

     // Inform to start polling example
   print_str(buffStart);
   PRINTF("\r\n======================== I2C MASTER ========================\r\n");

   pcbinit();			// Initialisation of diags CLI
   spawn (0, PSH_SLOT);	// kickoff pshell

   GPIO_DRV_WritePinOutput (0x200, 1);
   GPIO_DRV_WritePinOutput (0x202, 1);
   GPIO_DRV_WritePinOutput (0x303, 1);
   GPIO_DRV_WritePinOutput (USB_HUB_RST, on);
   GPIO_DRV_WritePinOutput (TEMPLE_LEFT_LE_ENA, on);
   GPIO_DRV_WritePinOutput (TEMPLE_RIGHT_LE_ENA, on);
   GPIO_DRV_WritePinOutput (0x110, 1);
   GPIO_DRV_WritePinOutput (0x111, 1);
   GPIO_DRV_WritePinOutput (0x112, 1);
   GPIO_DRV_WritePinOutput (0x008, 0);

   // Initialize i2c master
     I2C_DRV_MasterInit(BOARD_I2C_COMM_INSTANCE, &master);

     PRINTF("Put Colour bar out\r\n    ");

     status = init_lcos(BOARD_I2C_COMM_INSTANCE);

     status = init_lcos_ctrlr(BOARD_I2C_COMM_INSTANCE);

     status = init_lm3549(BOARD_I2C_COMM_INSTANCE);

     PRINTF("\r\n========================  MASTER ========================\r\n");

     PRINTF("\r\n    ");

     DSPI_DRV_MasterInit(0, &g_spi_state[0], &dspiConfig);
     DSPI_DRV_MasterConfigureBus(0, &g_dspiDevice, &calculatedBaudRate);

     	SHV_IMU_SendData(0, 0x6a, 1, &val);

     	val = 0x80;
     	SHV_IMU_SendData(0, 0x6b, 1, &val);

     	val = 0x50;
     	SHV_IMU_SendData(1, 0x6a, 1, &val);

     	val = 0x80;
     	SHV_IMU_SendData(1, 0x6b, 1, &val);

     	SHV_IMU_ReadData(0, 0x75, 1, &val);
     	SHV_IMU_ReadData(1, 0x75, 1, &val);



while(true)
   {
#ifndef BASIC_IO
		  diags();
#if 0
	        // Delay to wait slave received data
	        OSA_TimeDelay(25);

	        // Clear rxBuff
	        for(i = 0; i < count; i++)
	        {
	            rxBuff[i] = 0;
	        }

	        // Master receives count byte data from slave
	        I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_COMM_INSTANCE, &device,
	                                                  NULL, 0, rxBuff, count, 1000);

	        /* Compare to check result */
	        if(i2c_compare((uint8_t*)txBuff, rxBuff, count) != true)
	        {
	            PRINTF("\r\nFailure when transfer with size of buffer is %d.\n\r", count);
	            break;
	        }

	        PRINTF("\r\nMaster Sends/ Receives %2d bytes Successfully\r\n\n", count);
	        if(++count > DATA_LENGTH)
	        {
	            count = 1;
	        }
#endif
	    }
#if 0
	    PRINTF("\r\n==================== I2C MASTER FINISH =================== \r\n");

	    // Deinit i2c
	    I2C_DRV_MasterDeinit(BOARD_I2C_COMM_INSTANCE);
#endif

#if 0
		  if (slave_data_available)
		  {
		  LDD_TError err;
		  uint8_t buffer[64];
		  int size = 42;
		  slave_data_available = 0;
		  //err = CI2C2_SlaveReceiveBlock(GetI2cDevice(1), &slave_data_address, 1);
	 		if ( err )
	 		{
				PRINTF("Error receiving block: %d\n", err);
				continue;
			}
			//while ( CI2C2_SlaveGetReceivedDataNum(GetI2cDevice(1)) < 1);
			//size = CI2C2_SlaveGetReceivedDataNum(GetI2cDevice(1));
			//printf("I2c: requested offset 0x%02x\n", slave_data_address);
		  }
		  if ( slave_data_requested ) {
			  LDD_TError err;
			  slave_data_requested = 0;
	/*
			  while ( !CI2C2_SlaveGetBlockSentStatus(GetI2cDevice(1))) {
				int size = CI2C2_SlaveGetSentDataNum(GetI2cDevice(1));
				size = 0;
			}
	*/
			//printf("Send edid block\n");
		  }
		  if ( init_requested )
		  {
			  init_requested = FALSE;
			  PRINTF("Init requested\n");
	#define EDPDSI_BUS 1
	#define DSILVDS_BUS0 2
	#define DSILVDS_BUS1 0
			  usleep_range(2000000, 2000000);
			  tc358860_after_training(EDPDSI_BUS);
			usleep_range(4000, 5000);

			tc35876x_init(DSILVDS_BUS0, 0x0f);
			tc35876x_init(DSILVDS_BUS1, 0x0f);
		  }
#endif
		  #else
       // Wait to receive input data
       if (kStatus_UART_Success == UART_HAL_ReceiveDataPolling(baseAddr, &rxChar, 1u))
       {
           // Send any character that received
           UART_HAL_SendDataPolling(baseAddr, &rxChar, 1u);
       }
#endif

//#endif
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
