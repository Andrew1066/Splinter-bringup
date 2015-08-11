/** ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : Cpu.c
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : MK65FN2M0MI18
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.2.0
**     Datasheet   : K65P169M180SF5RMV2, Rev. 1, Mar 2015
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-11, 19:53, # CodeGen: 28
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         SystemInit            - void SystemInit(void);
**         SystemCoreClockUpdate - void SystemCoreClockUpdate(void);
**
**     (c) Freescale Semiconductor, Inc.
**     2004 All Rights Reserved
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
** @file Cpu.c
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup Cpu_module Cpu module documentation
**  @{
*/         

/* MODULE Cpu. */

/* {Default RTOS Adapter} No RTOS includes */
#include "Cpu.h"
#include "Events.h"
#include "hid1_hid.h"


#ifdef __cplusplus
extern "C" {
#endif

/*
** ===================================================================
**     Method      :  Common_Init (component MK65FN2M0MI18)
**     Description :
**         Initialization of registers for that there is no 
**         initialization component.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#if CPU_COMMON_INIT
void Common_Init(void)
{
}

#endif /* CPU_COMMON_INIT */
/*
** ===================================================================
**     Method      :  Components_Init (component MK65FN2M0MI18)
**     Description :
**         Initialization of components (with exception for Peripheral
**         Initialization Components which are initialized in 
**         Peripherals_Init() method).
**         For example, if automatic initialization feature 
**         is enabled in LDD component then its Init method call 
**         is executed in Components_Init() method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#if CPU_COMPONENTS_INIT
void Components_Init(void)
{

  /*! gpio2 Auto initialization start */
  GPIO_DRV_Init(gpio2_InpConfig0,gpio2_OutConfig0);
  /*! gpio2 Auto initialization end */
  
  /*! i2cCom1 Auto initialization start */
  I2C_DRV_MasterInit(FSL_I2CCOM1, &i2cCom1_MasterState);
  I2C_DRV_MasterSetBaudRate(FSL_I2CCOM1, &i2cCom1_MasterConfig0);
  /*! i2cCom1 Auto initialization end */
  
  /*! flexTimer1 Auto initialization start */
  FTM_DRV_Init(FSL_FLEXTIMER1,&flexTimer1_InitConfig0);
  FTM_DRV_SetClock(FSL_FLEXTIMER1, kClock_source_FTM_SystemClk, kFtmDividedBy1);
  FTM_DRV_SetTimeOverflowIntCmd(FSL_FLEXTIMER1,false);
  FTM_DRV_SetFaultIntCmd(FSL_FLEXTIMER1,false);
  /*! flexTimer1 Auto initialization end */
  
  /*! DbgCs1 Auto initialization start */
  /* Enable clock source for LPUART - bitfield LPUART0SRC within SIM_SOPT2 */
  CLOCK_SYS_SetLpuartSrc(BOARD_DEBUG_UART_INSTANCE,kClockLpuartSrcOsc0erClk);
  /* Debug console initialization */
  DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, DEBUG_UART_BAUD, DEBUG_UART_TYPE);
  /*! DbgCs1 Auto initialization end */
  /*! hid1 Auto initialization start */
  (void)USB_Class_HID_Init(USBFMW1_USB_CONTROLLER_ID, &hid1_HidConfigStructure, &hid1_HidHandle);
  /*! hid1 Auto initialization end */
  /*! dspiCom1 Auto initialization start */
  DSPI_DRV_MasterInit(FSL_DSPICOM1, &dspiCom1_MasterState, &dspiCom1_MasterConfig0);
  DSPI_DRV_MasterConfigureBus(FSL_DSPICOM1, &dspiCom1_BusConfig0, &dspiCom1_calculatedBaudRate);
  /*! dspiCom1 Auto initialization end */
  
  /*! dspiCom2 Auto initialization start */
  DSPI_DRV_MasterInit(FSL_DSPICOM2, &dspiCom2_MasterState, &dspiCom2_MasterConfig0);
  DSPI_DRV_MasterConfigureBus(FSL_DSPICOM2, &dspiCom2_BusConfig0, &dspiCom2_calculatedBaudRate);
  /*! dspiCom2 Auto initialization end */
  
  /*! uartCom1 Auto initialization start */
  UART_DRV_Init(FSL_UARTCOM1,&uartCom1_State,&uartCom1_InitConfig0);
  UART_DRV_InstallRxCallback(FSL_UARTCOM1, uartCom1_RxCallback, NULL, NULL, false);
  UART_DRV_InstallTxCallback(FSL_UARTCOM1, uartCom1_TxCallback, NULL, NULL);
  /*! uartCom1 Auto initialization end */
}
#endif /* CPU_COMPONENTS_INIT */


#ifdef __cplusplus
}
#endif

/* END Cpu. */

