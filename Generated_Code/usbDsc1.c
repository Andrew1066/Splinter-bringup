/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : usbDsc1.c
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : fsl_usb_descriptors
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.2.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-10, 10:31, # CodeGen: 11
**     Abstract    :
**          This component encapsulates the Universal Serial Bus (USB)
**          module in the device mode according to specification 2.0. 
**          It allows design variable configurations, interfaces and endpoints.
**     Settings    :
**          Component name                                 : usbDsc1
**          Lower level component link                     : usbFmw1
**          Generated code common settings                 : 
**            Mark all descriptors as const                : yes
**          Composite device                               : no
**          Class setting                                  : 
**            AUDIO class support                          : no
**            CDC class support                            : no
**            HID class support                            : yes
**            MSD class support                            : no
**          Supported languages                            : 1
**            Language 0                                   : 0x0409 English (United States)
**          External power source                          : yes
**          Common device settings                         : 
**            USB revision                                 : USB 2.0
**            Vendor ID                                    : 0x0000
**            Product ID                                   : 0x0000
**            Device release number                        : 01.00
**            Manufacturer description                     : Enabled
**              Language list                              : 1
**                Language 0                               : Freescale Processor Expert
**            Product description                          : Enabled
**              Language list                              : 1
**                Language 0                               : Processor Expert USB Device component
**            Device's serial number                       : Enabled
**              Descriptor index                           : 3
**              Language list                              : 1
**                Language 0                               : 123456789ABCDEF
**            Device speed                                 : Full speed
**              Device description                         : 
**                Class code                               : 0x00 Class information at interface level
**                Subclass code                            : 0
**                Protocol code                            : 0
**                EP0 settings                             : 
**                  Max packet size                        : 64
**                Configuration list                       : 1
**                  Configuration 1                        : 
**                    Configuration name                   : usbDsc1_Full_Speed_Configuration_1
**                    Total length                         : 34
**                    Configuration description            : Enabled
**                      Language list                      : 1
**                        Language 0                       : Configuration 1
**                    Power characteristics                : self powered
**                    Maximum power consumption            : 0 mA
**                    Remote wake-up                       : yes
**                    Class list                           : 1
**                      Class 0                            : 
**                        Class component name             : hid1
**                        Class user name                  : 
**                        Implementation specific settings : SDK
**                          SDK specific settings          : 
**                            SDK callbacks                : 
**                              USB_Desc_Get_Descriptor callback name: 
**                              USB_Desc_Get_Entity callback name: 
**                            Class config structure       : 
**                              Application callback       : 
**                                Callback name            : hid1_application_callback
**                                Callback parametr        : NULL
**                                External declaration of parameter: 
**                              Class specific callback    : 
**                                Callback name            : hid1_class_specific_callback
**                                Callback parametr        : NULL
**                                External declaration of parameter: 
**                              Vendor request callback    : 
**                                Callback name            : NULL
**                                Callback parametr        : NULL
**                                External declaration of parameter: 
**                        Interface list                   : 1
**                          Interface 0                    : 
**                            Alternate setting list       : 1
**                              Alternate setting 0        : 
**                                Interface user name      : 
**                                Default request handler name: 
**                                Class code               : 0x03 HID
**                                Subclass code            : 0x00 No Subclass
**                                Protocol code            : 0x02 Mouse
**                                Alternate setting description: Disabled
**                                Class descriptors        : 0x03 HID
**                                  HID descriptor         : 
**                                    HID Class specification release number: 1.11
**                                    Country code         : 0x00 Not Supported
**                                    Class descriptor list: 1
**                                      HID class descriptor 0: 
**                                        Descriptor type  : HID_REPORT
**                                        Descriptor size  : 50
**                                        Descriptor name  : hid1_MouseReportDescriptor
**                                Pipe list                : 1
**                                  Pipe 0                 : Interrupt IN
**                                    Pipe user name       : hid1_PipeIn
**                                    Default request handler name: 
**                                    Endpoint number      : 1
**                                    Maximum packet size  : 8
**                                    Polling interval     : 1 ms
**                                    ZLT                  : yes
**          SDK settings                                   : 
**            Class drivers configuration                  : 
**              CDC driver configuration                   : Disabled
**              HID class driver configuration             : Enabled
**                Max. human interface device number       : 1
**                Max. class endpoint number               : 1
**                Data transfer queuing                    : Disabled
**              MSD class driver configuration             : Disabled
**              Composite driver configuration             : Disabled
**            Initialization composite device              : Disabled
**     Contents    :
**         No public methods
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
** @file usbDsc1.c
** @version 01.00
** @brief
**          This component encapsulates the Universal Serial Bus (USB)
**          module in the device mode according to specification 2.0. 
**          It allows design variable configurations, interfaces and endpoints.
*/         
/*!
**  @addtogroup usbDsc1_module usbDsc1 module documentation
**  @{
*/         

/* MODULE usbDsc1. */

#include "usbDsc1.h"

            
extern uint8_t hid1_MouseReportDescriptor[50];

/* hid1: FS_Cfg_1_Class_0_Int_0_AltSet_0 endpoint info array */                   
static usb_ep_struct_t FS_Cfg_1_Class_0_Int_0_AltSet_0_EndpointInfoArray [] = {
  { /* hid1_PipeIn */   
    1,                                                                /* Endpoint number */
    USB_INTERRUPT_PIPE,                                               /* Endpoint type */
    USB_SEND,                                                         /* Endpoint direction */
    8                                                                 /* Maximum packet size */
  }     			     
};
          
/* Full speed Configuration 0 Interface  settings */                   
static usb_if_struct_t FS_Cfg_1_Class_0_InterfaceInfoArray [] = {
  { 
    0,                                                               /* Interface number */
    { /* Interface alternate setting 0 endpoints */
      1U,                                                            /* Endpoint count */
      FS_Cfg_1_Class_0_Int_0_AltSet_0_EndpointInfoArray              /* Endpoint info array address */        
    }
  } 
};
        
static usb_class_struct_t FS_Cfg_1_ClassInfoArray [] = {
  { /* Class hid1 */
    USB_CLASS_HID,                                                   /* Class type */ 
    { 
      1U,                                                            /* Interface count */
      FS_Cfg_1_Class_0_InterfaceInfoArray,                           /* Interface info array address */
    }   
  }
};         			        

/*****************************************************************************/
/* 0x0409 English (United States) string descriptors                         */
/*****************************************************************************/

/* Manufacturer string descriptor, value: "Freescale Processor Expert" */
const uint8_t usbDsc1_StringDescriptor_1_0x0409[]= {
  0x36,                                /* Descriptor size: 54 bytes */
  USB_STRING_DESCRIPTOR,               /* Descriptor type: String descriptor */
  'F',0x00,'r',0x00,'e',0x00,'e',0x00,
  's',0x00,'c',0x00,'a',0x00,'l',0x00,
  'e',0x00,' ',0x00,'P',0x00,'r',0x00,
  'o',0x00,'c',0x00,'e',0x00,'s',0x00,
  's',0x00,'o',0x00,'r',0x00,' ',0x00,
  'E',0x00,'x',0x00,'p',0x00,'e',0x00,
  'r',0x00,'t',0x00
};

/* Product string descriptor, value: "Processor Expert USB Device component" */
const uint8_t usbDsc1_StringDescriptor_2_0x0409[]= {
  0x4C,                                /* Descriptor size: 76 bytes */
  USB_STRING_DESCRIPTOR,               /* Descriptor type: String descriptor */
  'P',0x00,'r',0x00,'o',0x00,'c',0x00,
  'e',0x00,'s',0x00,'s',0x00,'o',0x00,
  'r',0x00,' ',0x00,'E',0x00,'x',0x00,
  'p',0x00,'e',0x00,'r',0x00,'t',0x00,
  ' ',0x00,'U',0x00,'S',0x00,'B',0x00,
  ' ',0x00,'D',0x00,'e',0x00,'v',0x00,
  'i',0x00,'c',0x00,'e',0x00,' ',0x00,
  'c',0x00,'o',0x00,'m',0x00,'p',0x00,
  'o',0x00,'n',0x00,'e',0x00,'n',0x00,
  't',0x00
};

/* Serial number string descriptor, value: "123456789ABCDEF" */
const uint8_t usbDsc1_StringDescriptor_3_0x0409[]= {
  0x20,                                /* Descriptor size: 32 bytes */
  USB_STRING_DESCRIPTOR,               /* Descriptor type: String descriptor */
  '1',0x00,'2',0x00,'3',0x00,'4',0x00,
  '5',0x00,'6',0x00,'7',0x00,'8',0x00,
  '9',0x00,'A',0x00,'B',0x00,'C',0x00,
  'D',0x00,'E',0x00,'F',0x00
};

/* Configuration string descriptor, value: "Configuration 1" */
const uint8_t usbDsc1_StringDescriptor_4_0x0409[]= {
  0x20,                                /* Descriptor size: 32 bytes */
  USB_STRING_DESCRIPTOR,               /* Descriptor type: String descriptor */
  'C',0x00,'o',0x00,'n',0x00,'f',0x00,
  'i',0x00,'g',0x00,'u',0x00,'r',0x00,
  'a',0x00,'t',0x00,'i',0x00,'o',0x00,
  'n',0x00,' ',0x00,'1',0x00
};

/*****************************************************************************/
/* String descriptor Zero                                                    */
/*****************************************************************************/

const uint8_t usbDsc1_StrDescrZero[]={
  0x04,                                /* Descriptor size 4 bytes */
  USB_STRING_DESCRIPTOR,               /* Descriptor type: String descriptor */
  0x09,0x04                            /* 0x0409 English (United States) */
};

/*****************************************************************************/
/* 0x0409 English (United States) string descriptor table                    */
/*****************************************************************************/

const uint8_t* const usbDsc1_StringDescriptorTable_0x0409[]= {
  usbDsc1_StrDescrZero,
  usbDsc1_StringDescriptor_1_0x0409,   /* Freescale Processor Expert */
  usbDsc1_StringDescriptor_2_0x0409,   /* Processor Expert USB Device component */
  usbDsc1_StringDescriptor_3_0x0409,   /* 123456789ABCDEF */
  usbDsc1_StringDescriptor_4_0x0409    /* Configuration 1 */
};

/***************************************************************************/
/* Device Descriptor                                                         */
/***************************************************************************/
const uint8_t usbDsc1_FS_DeviceDescriptor[]={
  0x12,                                /* Descriptor size: 18 bytes */
  USB_DEVICE_DESCRIPTOR,               /* Descriptor type: Device descriptor */
  0x00,0x02,                           /* USB specification release number: USB 2.0 */
  0x00,                                /* Device class code: 0x00 Class information at interface level */
  0x00,                                /* Device subclass code: 0 */
  0x00,                                /* Device protocol code: 0 */
  0x40,                                /* EP0 maximum packet size: 64 bytes */
  0x00,0x00,                           /* Vendor ID: 0 */
  0x00,0x00,                           /* Product ID: 0 */
  0x00,0x01,                           /* Device release number in: 01.00 */
  0x01,                                /* Manufacturer string descriptor index */
  0x02,                                /* Product string descriptor index */
  0x03,                                /* Serial number string descriptor index */
  0x01                                 /* Number of possible configurations */
};

const uint8_t usbDsc1_Full_Speed_Configuration_1[]={
  /***************************************************************************/
  /* Configuration 1 Descriptor                                              */
  /***************************************************************************/
  0x09,                                /* Descriptor size: 9 bytes */
  USB_CONFIGURATION_DESCRIPTOR,        /* Descriptor type: Configuration descriptor */
  0x22,0x00,                           /* Total length of data for this configuration: 34 bytes */
  0x01,                                /* No of interfaces supported by this configuration */
  0x01,                                /* Designator value for this configuration */
  0x04,                                /* Configuration string descriptor index */
  0xE0,                                /* Power source: self powered, remote wake-up: yes */
  0x00,                                /* Max. power consumption: 0 mA */
  /***************************************************************************/
  /* hid1: Interface 0 Alternate setting 0 Descriptor                        */
  /***************************************************************************/
  0x09,                                /* Descriptor size: 9 bytes */
  USB_INTERFACE_DESCRIPTOR,            /* Descriptor type: INTERFACE descriptor */
  0x00,                                /* Interface number: 0 */
  0x00,                                /* Alternative setting number: 0 */
  0x01,                                /* Number of EPs(excluding EP0): 1 */
  0x03,                                /* Class code: 0x03 HID */
  0x00,                                /* Subclass code: 0x00 No Subclass */
  0x02,                                /* Protocol code: 0x02 Mouse */
  0x00,                                /* String descriptor index */
  /***************************************************************************/
  /* HID Descriptor                                                          */
  /***************************************************************************/
  0x09,                                /* Descriptor size: 9 bytes */
  USB_HID_DESCRIPTOR,                  /* Descriptor type: HID descriptor */
  0x11,0x01,                           /* HID specification release number: 1.11 */
  0x00,                                /* Country code: 0x00 Not Supported bytes */
  0x01,                                /* Number of class descriptors : 1 */
  USB_HID_REPORT_DESCRIPTOR,           /* Descriptor type: HID_REPORT descriptor */
  0x32,0x00,                           /* Descriptor size: 0x32 */
  /***************************************************************************/
  /* hid1: Endpoint FS Interrupt EP1 IN, 8 KB/s Descriptor                   */
  /***************************************************************************/
  0x07,                                /* Descriptor size: 7 bytes */
  USB_ENDPOINT_DESCRIPTOR,             /* Descriptor type: ENDPOINT descriptor */
  0x81,                                /* Address: 1 IN */
  0x03,                                /* Transfer type: Interrupt */
  0x08,0x00,                           /* Max. packet size: 8 byte(s) */
  0x01                                 /* Polling interval: 1 ms */
};


/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Descriptor
 *
 * @brief The function returns the corresponding descriptor
 *
 * @param handle        : handle     
 * @param type          : type of descriptor requested
 * @param sub_type      : string index for string descriptor
 * @param index         : string descriptor language Id
 * @param descriptor    : output descriptor pointer
 * @param size          : size of descriptor returned
 *
 * @return USB_OK                      When Successful
 *         USBERR_INVALID_REQ_TYPE     when Error
 *****************************************************************************/
static uint8_t USB_Desc_Get_Descriptor (
  uint32_t     handle,           /* handle, not used */
  uint8_t      type,             /* type of descriptor requested */
  uint8_t      str_num,
  uint16_t     index,
  uint8_t    **descriptor,
  uint32_t    *size
)
{
  
  UNUSED_ARGUMENT(handle);
  
  *descriptor = NULL;  
  switch (type) {

    case USB_REPORT_DESCRIPTOR:
      if (index == 0U) {
        *descriptor = (uint8_t *)&hid1_MouseReportDescriptor;
        *size       = 50U;
      }
      break;

    case USB_HID_DESCRIPTOR:
      if (index == 0U) {
        *descriptor = (uint8_t *)&usbDsc1_Full_Speed_Configuration_1[18];
        *size       = 9U;
      }
      break;
      
    case USB_DEVICE_DESCRIPTOR:
      *descriptor = (uint8_t *)usbDsc1_FS_DeviceDescriptor;
      *size = sizeof(usbDsc1_FS_DeviceDescriptor);
      break;
      
    case USB_CONFIGURATION_DESCRIPTOR: /* All configuration */
      if (index == 0x00U) {
        *descriptor = (uint8_t *)usbDsc1_Full_Speed_Configuration_1;
        *size = sizeof(usbDsc1_Full_Speed_Configuration_1);
      }
      break;
    
    case USB_STRING_DESCRIPTOR:
      if (str_num <= sizeof(usbDsc1_StringDescriptorTable_0x0409)/sizeof(uint8_t *)) { /* Check if descriptor with this this index exists */
        switch(index) {
          case 0x0000:                 /* String descriptor 0 */
          case 0x0409:                 /* String descriptor for language 0x0409 English (United States) is requested */
            *descriptor = (uint8_t *)usbDsc1_StringDescriptorTable_0x0409[str_num];
            *size = *(uint8_t *)*descriptor;
            break;
          default:
            break;
        } /* End switch(index) */
      }
      break;
      
  } /* End switch(type) */ 
  
  if (*descriptor == NULL)  {
    return USBERR_INVALID_REQ_TYPE;
  } else { 
    return USB_OK;
  }
}    

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Entity
 *
 * @brief The function retrieves the entity specified by type.
 *
 * @param handle      handle
 *
 * @return USB_OK  - if success
 *****************************************************************************/
static uint8_t USB_Desc_Get_Entity(uint32_t handle,entity_type type, uint32_t * object)
{
  UNUSED_ARGUMENT(handle);
  switch (type)  {

    case USB_CLASS_INFO:    
      *object = (uint32_t)FS_Cfg_1_ClassInfoArray;
      break;
      
    default :
      break;
  }/* End Switch */
  return USB_OK;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Interface
 *
 * @brief The function returns the alternate interface
 *
 * @param handle:         handle     
 * @param interface:      interface number     
 * @param alt_interface:  output alternate interface     
 *
 * @return USB_OK                     When Successfull
 *         USBERR_INVALID_REQ_TYPE    when Error
 *****************************************************************************/
static uint8_t USB_Desc_Get_Interface (
  uint32_t      handle,
  uint8_t       interface,
  uint8_t      *alt_interface
)
{
  UNUSED_ARGUMENT(handle);
  return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Interface
 *
 * @brief The function sets the alternate interface
 *
 * @param handle:         handle     
 * @param interface:      interface number     
 * @param alt_interface:  input alternate interface     
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
static uint8_t USB_Desc_Set_Interface (
  uint32_t  handle,
  uint8_t       interface,
  uint8_t       alt_interface
)
{
  UNUSED_ARGUMENT(handle);
  return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Set_Configation
 *
 * @brief The function checks whether the configuration parameter
 *        input is valid or not
 *
 * @param handle          handle
 * @param config_val      configuration value
 *
 * @return TRUE           When Valid
 *         FALSE          When Error
 *****************************************************************************/
static uint8_t USB_Set_Configation (
  uint32_t      handle,
  uint8_t       config
)
{
  UNUSED_ARGUMENT(handle);
  return USB_OK;
}

usb_desc_request_notify_struct_t  usbDsc1_desc_callback =
{
  USB_Desc_Get_Descriptor,
  USB_Desc_Get_Interface,
  USB_Desc_Set_Interface,
  USB_Set_Configation,
  USB_Desc_Get_Entity
};


/* END usbDsc1. */

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
