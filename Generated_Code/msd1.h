/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : msd1.h
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : fsl_usb_device_msd_class
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.2.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-18, 16:51, # CodeGen: 71
**     Abstract    :
**
**     Settings    :
**          Component name                                 : msd1
**          fsl_usb_descriptor component link              : usbDsc1
**          Update fsl_usb_descriptor component            : 
**          MSD Class                                      : 0x08
**            Subclass code                                : 0x06 SCSI transparent command set
**            Protocol code                                : 0x50 BBB (bulk only transport)
**          Initialization                                 : Enabled
**            MSD device handler name                      : msd1_MsdHandle
**            MSD configuration structure name             : msd1_MsdConfigStructure
**            USB RAM disk demo                            : Disabled
**            Callback user parameters                     : 
**              Application callback                       : 
**                application_callback arg                 : NULL
**                External declaration of parameter        : 
**              Vendor request callback                    : 
**                vendor_req_callback_arg                  : NULL
**                External declaration of parameter        : 
**              Class specific callback                    : 
**                class_specific_callback arg              : NULL
**                External declaration of parameter        : 
**            Auto initialization                          : Enabled
**     Contents    :
**         USB_Class_MSC_Init   - usb_status USB_Class_MSC_Init(uint8_t controller_id, msc_config_struct_t *...
**         USB_Class_MSC_Deinit - usb_status USB_Class_MSC_Deinit(msd_handle_t msd_handle);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file msd1.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup msd1_module msd1 module documentation
**  @{
*/         

#ifndef __msd1_H
#define __msd1_H

/* MODULE msd1. */

/* Include inherited components */
#include "usbDsc1.h"
#include "usb.h"
#include "usb_device_stack_interface.h"
#include "usb_class_msc.h"

/*! @brief USB MSC device class handle */  
extern msd_handle_t msd1_MsdHandle; 

/*! @brief USB MSD class configuration structure */
extern msc_config_struct_t msd1_MsdConfigStructure;

  
/* END msd1. */

#endif
/* ifndef __msd1 */
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