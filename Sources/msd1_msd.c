/* ###################################################################
**     Filename    : msd1_msd.c
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
** @file msd1_msd.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup msd1_msd_module msd1_msd module documentation
**  @{
*/         
/* MODULE msd1_msd */

#include "Cpu.h"
#include "Events.h"
#include "msd1_msd.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/******************************************************************************
**
**    @name        msd1_application_callback
**
**    @brief       This function handles the callback  
**
**    @param       handle : handle to Identify the controller
**    @param       event_type       : value of the event
**    @param       val              : gives the configuration value
**    @param       arg      :  user parameter
**
**    @return      None
**
******************************************************************************/
void msd1_application_callback(uint8_t event_type, void * val, void * arg)
{
  switch (event_type) {

    case USB_DEV_EVENT_BUS_RESET:       /* BUS reset received */
      /* Write your code here ... */
      break;

    case USB_DEV_EVENT_ENUM_COMPLETE:   /* Device enumerated process complete */
      /* Write your code here ... */
      break;

    case USB_DEV_EVENT_ERROR:           /* Device error detected */
      /* Write your code here ... */
      break;

    case USB_MSC_DEVICE_GET_SEND_BUFF_INFO:                   
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */
      if(val != NULL) {                /* Get MSD send buffer size */
        *((uint32_t *)val) = (uint32_t) msd1_RAM_DISK_SIZE; 
      }  
#else
      /* Write your code here ... */
#endif
      break;

    case USB_MSC_DEVICE_GET_RECV_BUFF_INFO:          
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */
      if(val != NULL) {                /* Get MSD receive buffer size */
        *((uint32_t *)val) = (uint32_t) msd1_RAM_DISK_SIZE;      
      }    
#else
      /* Write your code here ... */
#endif
      break;
  } /* End Switch */ 
}

/******************************************************************************
**
**    @name        msd1_class_specific_callback
**
**    @brief       This function handles USB-MSD Class callback
**
**    @param       request  :  request type
**    @param       value    :  give report type and id
**    @param       data     :  pointer to the data
**    @param       size     :  size of the transfer
**    @param       arg      :  user parameter
**
**    @return      status
**                  USB_OK  :  if successful
**                  else return error
**
******************************************************************************/
uint8_t msd1_class_specific_callback(uint8_t event_type, uint16_t value, uint8_t ** data, uint32_t * size, void * arg)
{
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */  
  lba_app_struct_t * lba_data_ptr;
  device_lba_info_struct_t * device_lba_info_ptr;
#endif
  uint8_t error = USB_OK;
  
  switch (event_type) {

    /* Write the storage device buffer data the storage device */
    case USB_DEV_EVENT_DATA_RECEIVED:
      /* Write your code here... */ 
      break;  

    /* Read data from mass storage device to driver buffer */
    case USB_DEV_EVENT_SEND_COMPLETE: 
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */
      lba_data_ptr = (lba_app_struct_t *)size; 
      if (lba_data_ptr->size != 0) {
        if(data != NULL) {
          *data = (uint8_t *)&msd1_StorageDisk[lba_data_ptr->offset];
        }   
      }   
#else 
      /* Write your code here ... */
#endif
      break;

    /* Code to be added by user for starting, stopping or           
    ** ejecting the disk drive. e.g. starting/stopping the motor in 
    ** case of CD/DVD                                               
    */
    case USB_MSC_START_STOP_EJECT_MEDIA:
      /* Write your code here... */
      break;

    /* Get data from the storage device before sending it to the USB bus.
    ** It reads data from the mass storage device to the driver buffer.   
    */
    case USB_MSC_DEVICE_READ_REQUEST:  
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */
      lba_data_ptr = (lba_app_struct_t *)size;  
      if(data != NULL) {
        *data = (uint8_t *)&msd1_StorageDisk[lba_data_ptr->offset];
      }   
#else
      /* Write your code here ... */
#endif
      break;

    /* Prepare the storage device buffer for USB transfer */
    case USB_MSC_DEVICE_WRITE_REQUEST:
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */
      lba_data_ptr = (lba_app_struct_t *)size;  
      if(data != NULL) {
        *data = (uint8_t *)&msd1_StorageDisk[lba_data_ptr->offset];
      }   
#else
      /* Write your code here ... */
#endif
      break;

    case USB_MSC_DEVICE_FORMAT_COMPLETE:
      /* Write your code here ... */
      break;

    case USB_MSC_DEVICE_REMOVAL_REQUEST:
      /* Write your code here ... */
      break;

    case USB_MSC_DEVICE_GET_INFO:
#ifdef msd1_RAM_DISK_DEMO              /* USB RAM disk PEx demo project (This code is possible to enable by 'USB RAM disk demo' property). */
      device_lba_info_ptr = (device_lba_info_struct_t *)size;
      device_lba_info_ptr->num_lun_supported = msd1_RAM_DISK_LUN_NUMBER; /* Logical unit supported */
      device_lba_info_ptr->total_lba_device_supports = msd1_RAM_DISK_SECTOR_COUNT; /* Total logical address blocks */
      device_lba_info_ptr->length_of_each_lab_of_device = msd1_RAM_DISK_SECTOR_SIZE; /* Length of each lab */
#else
      /* Write your code here ... */                                                      
#endif
      break; 
           
    default : 
      break;                                    
  } /* End Switch */ 
  return error;
}

static device_lba_info_struct_t usb_msc_lba_info_struct;

uint8_t msd1_get_desc_entity(uint32_t handle, entity_type type, uint32_t * object)
{
    switch(type) {

      /* Get mass storage LAB related information */
      case USB_MSC_LBA_INFO:          
        msd1_class_specific_callback(USB_MSC_DEVICE_GET_INFO, USB_REQ_VAL_INVALID, NULL, (uint32_t *)&usb_msc_lba_info_struct, NULL);
        *object = (unsigned long)&usb_msc_lba_info_struct;  
        break;    

      default:
           
        break; 
    } /* End Switch */    
    return USB_OK;  
}

/* END msd1_msd */

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
