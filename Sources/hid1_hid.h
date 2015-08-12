/* ###################################################################
**     Filename    : hid1_hid.h
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-07, 14:52, # CodeGen: 10
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         hid1_hid_events_process      - void hid1_hid_events_process(void);
**         hid1_application_callback    - void hid1_application_callback(uint8_t event_type, void * val, void * arg);
**         hid1_class_specific_callback - uint8_t hid1_class_specific_callback(uint8_t request, uint16_t value, uint8_t...
**
** ###################################################################*/
/*!
** @file hid1_hid.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup hid1_hid_module hid1_hid module documentation
**  @{
*/         

#ifndef __hid1_hid_H
#define __hid1_hid_H
/* MODULE hid1_hid */

#include "fsl_device_registers.h"
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

#ifdef __cplusplus
extern "C" {
#endif 

void hid1_application_callback(uint8_t event_type, void * val, void * arg);

uint8_t hid1_class_specific_callback(uint8_t request, uint16_t value, uint8_t ** data, uint32_t * size, void * arg);


/* END hid1_hid */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __hid1_hid_H*/
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
