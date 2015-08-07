/* ###################################################################
**     Filename    : hid1_hid.c
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
** @file hid1_hid.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup hid1_hid_module hid1_hid module documentation
**  @{
*/         
/* MODULE hid1_hid */

#include "Cpu.h"
#include "Events.h"
#include "hid1_hid.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */


#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */ 
static volatile bool HidInitialized = FALSE;
#endif

/******************************************************************************
 * @name:         KeyBoard_Events_Process
 *
 * @brief:        This function gets the input data from HID and sends to the
 *                host device.
 *
 * @param:        None
 *
 * @return:       None
 *
 *****************************************************************************/
void hid1_hid_events_process(void)
{
#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */ 
  static int32_t Count = 0;  
  static enum { STATE_0, STATE_1} State = STATE_0;

  switch (State) {
    case STATE_0:
      hid1_SetInputReportBufferState0(hid1_InputReportBuffer);
      Count++;
      if (Count > hid1_StateMaxCount) {
        State = STATE_1;
      }
      break;
    case STATE_1:
      hid1_SetInputReportBufferState1(hid1_InputReportBuffer);
      Count--;
      if (Count < 0) {
        State = STATE_0;
      }
      break;
  }
  (void)USB_Class_HID_Send_Data(hid1_HidHandle, hid1_InputPipeNumber, hid1_InputReportBuffer, hid1_InputReportBufferSize);
#else
  /* Write your code here ... */
#endif     
}

/******************************************************************************
**
**    @name        hid1_application_callback
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
void hid1_application_callback(uint8_t event_type, void * val, void * arg)
{
  switch(event_type) {
    case USB_DEV_EVENT_BUS_RESET:     
#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */    
      HidInitialized = FALSE;
#else
      /* Write your code here ... */
#endif      
      break;
      
    case USB_DEV_EVENT_ENUM_COMPLETE:  
#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */    
      HidInitialized = TRUE;
      hid1_hid_events_process();      
#else
      /* Write your code here ... */
#endif      
      break;
      
    case USB_DEV_EVENT_ERROR: 
      /* Write your code here ... */    
      break;
      
    default: 
      break;
  }
}

/******************************************************************************
**
**    @name        hid1_class_specific_callback
**
**    @brief       This function handles USB-HID Class callback
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
uint8_t hid1_class_specific_callback(uint8_t request, uint16_t value, uint8_t ** data, uint32_t * size, void * arg)
{
  uint8_t error = USB_OK;
  
  if ((request == USB_DEV_EVENT_SEND_COMPLETE) && (value == USB_REQ_VAL_INVALID)) {
#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */    
    if (HidInitialized) {
      hid1_hid_events_process();
    }
    return error;
#else  
    /* Write your code here ... */  
#endif    
  }
  *size = 0;
  /* Handle the class request */
  switch (request) {
    case USB_HID_GET_REPORT_REQUEST:
#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */     
      *data = &hid1_InputReportBuffer[0]; /* Pointer to the report to send */
      *size = hid1_InputReportBufferSize; /* Report size */
#else
      /* Write your code here ... */    
#endif      
      break;
              
    case USB_HID_SET_REPORT_REQUEST:
#ifdef hid1_HID_DEMO                   /* USB HID PEx demo project (This code is possible to enable by 'USB HID demo' property). */     
      for(int index = 0; index < hid1_InputReportBufferSize; index++) { /* Copy the report sent by the host */
        hid1_InputReportBuffer[index] = (*data)[index];
      }
#else
      /* Write your code here ... */    
#endif      
      break;

    case USB_HID_GET_IDLE_REQUEST:
      /* Write your code here ... */    
      break;
                
    case USB_HID_SET_IDLE_REQUEST:
      /* Write your code here ... */    
      break;

    case USB_HID_GET_PROTOCOL_REQUEST:
      /* Write your code here ... */    
      break;

    case USB_HID_SET_PROTOCOL_REQUEST:
      /* Write your code here ... */    
      break;
  }        
  return error; 
}

/* END hid1_hid */

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
