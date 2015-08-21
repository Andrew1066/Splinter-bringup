/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : dmaController1.h
**     Project     : Splinter bringup
**     Processor   : MK65FX1M0VMI18
**     Component   : fsl_edma
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.2.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-18, 14:55, # CodeGen: 60
**     Contents    :
**         EDMA_DRV_Init                           - edma_status_t EDMA_DRV_Init(edma_state_t * edmaState,const edma_user_config_t...
**         EDMA_DRV_Deinit                         - edma_status_t EDMA_DRV_Deinit(void);
**         EDMA_DRV_RequestChannel                 - uint8_t EDMA_DRV_RequestChannel(uint8_t channel,dma_request_source_t...
**         EDMA_DRV_ReleaseChannel                 - edma_status_t EDMA_DRV_ReleaseChannel(edma_chn_state_t * chn);
**         EDMA_DRV_PrepareDescriptorTransfer      - static inline edma_status_t EDMA_DRV_Pr...
**         EDMA_DRV_PrepareDescriptorScatterGather - static inline edma_status_t EDMA_DRV_Pr...
**         EDMA_DRV_PrepareDescriptorChannelLink   - static inline edma_status_t EDMA_DRV_Pr...
**         EDMA_DRV_PushDescriptorToReg            - edma_status_t EDMA_DRV_PushDescriptorToReg(edma_chn_state_t *...
**         EDMA_DRV_ConfigLoopTransfer             - edma_status_t EDMA_DRV_ConfigLoopTransfer(edma_chn_state_t *...
**         EDMA_DRV_ConfigScatterGatherTransfer    - edma_status_t EDMA_DRV_ConfigScatterGatherTransfer(edma_chn_state_t *...
**         EDMA_DRV_StartChannel                   - edma_status_t EDMA_DRV_StartChannel(edma_chn_state_t * chn);
**         EDMA_DRV_StopChannel                    - edma_status_t EDMA_DRV_StopChannel(edma_chn_state_t * chn);
**         EDMA_DRV_InstallCallback                - edma_status_t EDMA_DRV_InstallCallback(edma_chn_state_t * chn,edma_callback_t...
**         EDMA_DRV_IRQHandler                     - void EDMA_DRV_IRQHandler(uint8_t channel);
**         EDMA_DRV_ErrorIRQHandler                - void EDMA_DRV_ErrorIRQHandler(uint8_t instance);
**         EDMA_DRV_GetChannelStatus               - static inline edma_chn_status_t EDMA_DRV_GetChannelStatus(edma_chn_state_t *...
**         EDMA_DRV_GetUnfinishedBytes             - static inline uint32_t EDMA_DRV_GetUnfinishedBytes(edma_chn_state_t * chn);
**         EDMA_DRV_GetFinishedBytes               - static inline uint32_t EDMA_DRV_GetFinishedBytes(edma_chn_state_t * chn);
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
** @file dmaController1.h
** @version 01.00
*/         
/*!
**  @addtogroup dmaController1_module dmaController1 module documentation
**  @{
*/         
#ifndef __dmaController1_H
#define __dmaController1_H
/* MODULE dmaController1. */

/* Include inherited beans */
#include "clockMan1.h"
#include "osa1.h"
#include "Cpu.h"

/*! @brief Device instance number */
#define FSL_DMACONTROLLER1 0

/*! @brief Driver state structure which holds driver runtime data */
extern edma_state_t dmaController1_State;

/*! @brief Configuration declaration */
extern const edma_user_config_t dmaController1_InitConfig0;
    
/*! @brief Dma channel 0 ISR */
void DMA0_DMA16_IRQHandler(void);
/*! @brief Dma channel 1 ISR */
void DMA1_DMA17_IRQHandler(void);
/*! @brief Dma channel 2 ISR */
void DMA2_DMA18_IRQHandler(void);
/*! @brief Dma channel 3 ISR */
void DMA3_DMA19_IRQHandler(void);
/*! @brief Dma channel 4 ISR */
void DMA4_DMA20_IRQHandler(void);
/*! @brief Dma channel 5 ISR */
void DMA5_DMA21_IRQHandler(void);
/*! @brief Dma channel 6 ISR */
void DMA6_DMA22_IRQHandler(void);
/*! @brief Dma channel 7 ISR */
void DMA7_DMA23_IRQHandler(void);
/*! @brief Dma channel 8 ISR */
void DMA8_DMA24_IRQHandler(void);
/*! @brief Dma channel 9 ISR */
void DMA9_DMA25_IRQHandler(void);
/*! @brief Dma channel 10 ISR */
void DMA10_DMA26_IRQHandler(void);
/*! @brief Dma channel 11 ISR */
void DMA11_DMA27_IRQHandler(void);
/*! @brief Dma channel 12 ISR */
void DMA12_DMA28_IRQHandler(void);
/*! @brief Dma channel 13 ISR */
void DMA13_DMA29_IRQHandler(void);
/*! @brief Dma channel 14 ISR */
void DMA14_DMA30_IRQHandler(void);
/*! @brief Dma channel 15 ISR */
void DMA15_DMA31_IRQHandler(void);
  
#endif
/* ifndef __dmaController1_H */
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