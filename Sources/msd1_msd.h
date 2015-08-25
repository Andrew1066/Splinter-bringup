/* ###################################################################
**     Filename    : msd1_msd.h
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-18, 16:51, # CodeGen: 71
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         msd1_application_callback    - void msd1_application_callback(uint8_t event_type, void * val, void * arg);
**         msd1_class_specific_callback - uint8_t msd1_class_specific_callback(uint8_t event_type, uint16_t value,...
**         msd1_get_desc_entity         - uint8_t msd1_get_desc_entity(uint32_t handle, entity_type type, uint32_t *...
**
** ###################################################################*/
/*!
** @file msd1_msd.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup msd1_msd_module msd1_msd module documentation
**  @{
*/         

#ifndef __msd1_msd_H
#define __msd1_msd_H
/* MODULE msd1_msd */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "osa1.h"
#include "gpio2.h"
#include "i2cCom1.h"
#include "flexTimer1.h"
#include "dspiCom1.h"
#include "usbDsc1.h"
#include "usbFmw1.h"
#include "msd1.h"
#include "DbgCs1.h"

#ifdef __cplusplus
extern "C" {
#endif 

void msd1_application_callback(uint8_t event_type, void * val, void * arg);

uint8_t msd1_class_specific_callback(uint8_t event_type, uint16_t value, uint8_t ** data, uint32_t * size, void * arg);


/* END msd1_msd */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __msd1_msd_H*/
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
