/*
 $License:
    Copyright (C) 2010 InvenSense Corporation, All Rights Reserved.
 $
 */
/*******************************************************************************
 * $Id: $
 *******************************************************************************/

/**
 *  @defgroup STM32L STM32L System Layer
 *  @brief  STM32L System Layer APIs.
 *          To interface with any platform, eMPL needs access to various
 *          system layer functions.
 *
 *  @{
 *      @file   packet.h
 *      @brief  Defines needed for sending data/debug packets via USB.
 */

#ifndef __PACKET_H__
#define __PACKET_H__

#include "mltypes.h"

typedef enum {
    PACKET_DATA_ACCEL = 0,
    PACKET_DATA_GYRO = 1,
    PACKET_DATA_COMPASS = 2,
    PACKET_DATA_QUAT = 3,
    PACKET_DATA_GRAVITY = 4,
    PACKET_DATA_ROT = 5,
    PACKET_DATA_HEADING = 6,
    PACKET_DATA_LINEAR_ACCEL = 7,
    PACKET_DATA_EULER = 8,
    NUM_DATA_PACKETS
} eMPL_packet_e;

/**
 *  @brief      Send a quaternion packet via UART.
 *  The host is expected to use the data in this packet to graphically
 *  represent the device orientation. To send quaternion in the same manner
 *  as any other data packet, use eMPL_send_data.
 *  @param[in]  quat    Quaternion data.
 */
void eMPL_send_quat(long *quat);

/**
 *  @brief      Send a data packet via UART
 *  @param[in]  type    Contents of packet (PACKET_DATA_ACCEL, etc).
 *  @param[in]  data    Data (length dependent on contents).
 */
void eMPL_send_data(unsigned char type, long *data);

#endif /* __PACKET_H__ */

/**
 * @}
 */
