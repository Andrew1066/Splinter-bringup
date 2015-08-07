/* ###################################################################
**     Filename    : Events.c
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-06, 20:52, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "hid1_hid.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

void FTM0_IRQHandler(void)
{
  FTM_DRV_IRQHandler(FSL_FLEXTIMER1);
  /* Write your code here ... */
}

/*! i2cCom1 IRQ handler */
void I2C0_IRQHandler(void)
{
  I2C_DRV_IRQHandler(FSL_I2CCOM1);
}

void uartCom1_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

void uartCom1_TxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

/*! dspiCom2 IRQ handler */
void SPI1_IRQHandler(void)
{
#if DSPICOM2_DMA_MODE
  DSPI_DRV_EdmaIRQHandler(FSL_DSPICOM2);
#else
  DSPI_DRV_IRQHandler(FSL_DSPICOM2);
#endif
  /* Write your code here ... */
}

/*! dspiCom1 IRQ handler */
void SPI0_IRQHandler(void)
{
#if DSPICOM1_DMA_MODE
  DSPI_DRV_EdmaIRQHandler(FSL_DSPICOM1);
#else
  DSPI_DRV_IRQHandler(FSL_DSPICOM1);
#endif
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
