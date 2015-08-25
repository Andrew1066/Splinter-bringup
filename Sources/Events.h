/* ###################################################################
**     Filename    : Events.h
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
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "osa1.h"
#include "gpio2.h"
#include "i2cCom1.h"
#include "flexTimer1.h"
#include "usbDsc1.h"
#include "usbFmw1.h"
#include "dspiCom1.h"
#include "msd1.h"
#include "DbgCs1.h"

#ifdef __cplusplus
extern "C" {
#endif 


void FTM0_IRQHandler(void);

/*! i2cCom1 IRQ handler */
void I2C0_IRQHandler(void);

/*! dspiCom2 IRQ handler */
void SPI1_IRQHandler(void);

/*! dspiCom1 IRQ handler */
void SPI0_IRQHandler(void);

/*! extWdog1 IRQ handler */
/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
