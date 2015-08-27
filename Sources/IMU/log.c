/*
 $License:
    Copyright (C) 2010 InvenSense Corporation, All Rights Reserved.
 $
 */
/*******************************************************************************
 * $Id: $
 *******************************************************************************/

/**
 *  @defgroup MSP430_System_Layer MSP430 System Layer
 *  @brief  MSP430 System Layer APIs.
 *          To interface with any platform, eMPL needs access to various
 *          system layer functions.
 *
 *  @{
 *      @file   log_msp430.c
 *      @brief  Logging facility for the TI MSP430.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device_stack_interface.h"
//#include "usb_class_hid.h"
//#include "main.h"   /* Keyboard Application Header File */
//#include "usb_descriptor.h"
#include "packet.h"
#include "log.h"
#include "DbgCs1.h"

//extern app_global_variable_struct_t           		g_app;
volatile int packet_ready = 0;

#define BUF_SIZE        (256)
#define PACKET_LENGTH   (63)

#define PACKET_DEBUG    (1)
#define PACKET_QUAT     (2)
#define PACKET_DATA     (3)
static char output_buffers[2][PACKET_LENGTH];
const char* format_float(float val, int buf, int n, int m);
#define FMT74(a, b) format_float((a), b, 7, 4)


/**
 *  @brief      Prints a variable argument log message.
 *  USB output will be formatted as follows:\n
 *  packet[0]       = $\n
 *  packet[1]       = packet type (1: debug, 2: quat, 3: data)\n
 *  packet[2]       = \n for debug packets: log priority\n
 *                    for quaternion packets: unused\n
 *                    for data packets: packet content (accel, gyro, etc)\n
 *  packet[3-20]    = data\n
 *  packet[21]      = \\r\n
 *  packet[22]      = \\n
 *  @param[in]  priority    Log priority (based on Android).
 *  @param[in]  tag         File specific string.
 *  @param[in]  fmt         String of text with optional format tags.
 *
 *  @return     0 if successful.
 */
int _MLPrintLog (int priority, const char* tag, const char* fmt, ...)
{
    va_list args;
#if 0
    int length, ii, i;
    char buf[BUF_SIZE], out[PACKET_LENGTH], this_length;
#endif
    /* This can be modified to exit for unsupported priorities. */
    switch (priority) {
    case MPL_LOG_UNKNOWN:
    case MPL_LOG_DEFAULT:
    case MPL_LOG_VERBOSE:
    case MPL_LOG_DEBUG:
    case MPL_LOG_INFO:
    case MPL_LOG_WARN:
    case MPL_LOG_ERROR:
    case MPL_LOG_SILENT:
        break;
    default:
        return 0;
    }

    va_start(args, fmt);
//    swo_vprintf(fmt, args); // ARB @@@
#if 0
    length = vsprintf(buf, fmt, args);
    if (length <= 0) {
        va_end(args);
        return length;
    }

    memset(out, 0, PACKET_LENGTH);
    out[0] = '$';
    out[1] = PACKET_DEBUG;
    out[2] = priority;
    out[21] = '\r';
    out[22] = '\n';
    for (ii = 0; ii < length; ii += (PACKET_LENGTH-5)) {
#define min(a,b) ((a < b) ? a : b)
        this_length = min(length-ii, PACKET_LENGTH-5);
        memset(out+3, 0, 18);
        memcpy(out+3, buf+ii, this_length);
        packet_ready = 1;
        for (i=0; i<PACKET_LENGTH; i++) {
          //USART_SendData(USART1, out[i]);
          //printf("%c", out[i]);
          //fputchar(out[i]);
        }
    }
    
#endif
    va_end(args);

    return 0;
}

void eMPL_send_quat(long *quat)
{
#if 0
    int i;
    char *out = output_buffers[0];

    if (!quat)
        return;
    memset(out, 0, PACKET_LENGTH);
    out[0] = '$';
    out[1] = PACKET_QUAT;
    out[3] = (char)(quat[0] >> 24);
    out[4] = (char)(quat[0] >> 16);
    out[5] = (char)(quat[0] >> 8);
    out[6] = (char)quat[0];
    out[7] = (char)(quat[1] >> 24);
    out[8] = (char)(quat[1] >> 16);
    out[9] = (char)(quat[1] >> 8);
    out[10] = (char)quat[1];
    out[11] = (char)(quat[2] >> 24);
    out[12] = (char)(quat[2] >> 16);
    out[13] = (char)(quat[2] >> 8);
    out[14] = (char)quat[2];
    out[15] = (char)(quat[3] >> 24);
    out[16] = (char)(quat[3] >> 16);
    out[17] = (char)(quat[3] >> 8);
    out[18] = (char)quat[3];
    out[21] = '\r';
    out[22] = '\n';
    
    _MLPrintLog(0, "", "%8d QUAT: %10s %10s %10s %10s\n", OSA_TimeGetMsec(), FMT74(quat[0] * 1.f / (1<<30), 0), FMT74(quat[1] * 1.f / (1<<30), 1), FMT74(quat[2] * 1.f / (1<<30), 2), FMT74(quat[3] * 1.f / (1<<30), 3));
    OSA_MutexLock(&g_app.output_mutex, -1);
//    while(!packet_ready);
//	packet_ready = 0;
//    USB_Class_HID_Send_Data(g_app.app_handle, 1, out, PACKET_LENGTH);
    OSA_MutexUnlock(&g_app.output_mutex);

    for (i=0; i<PACKET_LENGTH; i++) {
       //USART_SendData(USART1, out[i]);
      //printf("%c", out[i]);
      //fputchar(out[i]);
    }
#endif
}


void eMPL_send_data(unsigned char type, long *data)
{
#if 1
    int i;
    char *out = output_buffers[0];
    if (!data)
        return;
    switch (type) {
#if 1
    /* Two bytes per-element. */
    case PACKET_DATA_ROT:
#if 0
    	out[3] = (char)(data[0] >> 24);
        out[4] = (char)(data[0] >> 16);
        out[5] = (char)(data[1] >> 24);
        out[6] = (char)(data[1] >> 16);
        out[7] = (char)(data[2] >> 24);
        out[8] = (char)(data[2] >> 16);
        out[9] = (char)(data[3] >> 24);
        out[10] = (char)(data[3] >> 16);
        out[11] = (char)(data[4] >> 24);
        out[12] = (char)(data[4] >> 16);
        out[13] = (char)(data[5] >> 24);
        out[14] = (char)(data[5] >> 16);
        out[15] = (char)(data[6] >> 24);
        out[16] = (char)(data[6] >> 16);
        out[17] = (char)(data[7] >> 24);
        out[18] = (char)(data[7] >> 16);
        out[19] = (char)(data[8] >> 24);
        out[20] = (char)(data[8] >> 16);
#endif
        // _MLPrintLog(0, "", ...
        PRINTF("%8d ROT: %5d %5d %5d %5d %5d %5d %5d %5d %5d\n", OSA_TimeGetMsec(),
    			data[0] >> 16, data[1] >> 16,
    			data[2] >> 16, data[3] >> 16,
    			data[4] >> 16, data[5] >> 16,
    			data[6] >> 16, data[7] >> 16,
    			data[8] >> 16
    			);
    	break;
#endif
    /* Four bytes per-element. */
    /* Four elements. */
#if 1
    case PACKET_DATA_QUAT:
        out[3] = (char)(data[0] >> 24);
        out[4] = (char)(data[0] >> 16);
        out[5] = (char)(data[0] >> 8);
        out[6] = (char)data[0];
        out[7] = (char)(data[1] >> 24);
        out[8] = (char)(data[1] >> 16);
        out[9] = (char)(data[1] >> 8);
        out[10] = (char)data[1];
        out[11] = (char)(data[2] >> 24);
        out[12] = (char)(data[2] >> 16);
        out[13] = (char)(data[2] >> 8);
        out[14] = (char)data[2];
        out[15] = (char)(data[3] >> 24);
        out[16] = (char)(data[3] >> 16);
        out[17] = (char)(data[3] >> 8);
        out[18] = (char)data[3];
        out[2] |= (1 << type);
    	//_MLPrintLog(0, "", "%8d QUAT: %8d %8d %8d %8d \n", OSA_TimeGetMsec(), data[0], data[1], data[2], data[3]);
        break;
#endif
    /* Three elements. */
    case PACKET_DATA_ACCEL:
        //memset(out, 0, PACKET_LENGTH);
    	data += 3;
        out[19] = (char)(data[0] >> 24);
        out[20] = (char)(data[0] >> 16);
        out[21] = (char)(data[0] >> 8);
        out[22] = (char)data[0];
        out[23] = (char)(data[1] >> 24);
        out[24] = (char)(data[1] >> 16);
        out[25] = (char)(data[1] >> 8);
        out[26] = (char)data[1];
        out[27] = (char)(data[2] >> 24);
        out[28] = (char)(data[2] >> 16);
        out[29] = (char)(data[2] >> 8);
        out[30] = (char)data[2];
        out[2] |= (1 << type);
    	//_MLPrintLog(0, "", "%8d AGCU(%d): %8d %8d %8d\n", OSA_TimeGetMsec(), type, data[0], data[1], data[2]);
        break;
    case PACKET_DATA_GYRO:
        out[31] = (char)(data[0] >> 24);
        out[32] = (char)(data[0] >> 16);
        out[33] = (char)(data[0] >> 8);
        out[34] = (char)data[0];
        out[35] = (char)(data[1] >> 24);
        out[36] = (char)(data[1] >> 16);
        out[37] = (char)(data[1] >> 8);
        out[38] = (char)data[1];
        out[39] = (char)(data[2] >> 24);
        out[40] = (char)(data[2] >> 16);
        out[41] = (char)(data[2] >> 8);
        out[42] = (char)data[2];
        out[2] |= (1 << type);
    	//_MLPrintLog(0, "", "%8d AGCU(%d): %8d %8d %8d\n", OSA_TimeGetMsec(), type, data[0], data[1], data[2]);
        break;
    case PACKET_DATA_COMPASS:
    	if ( !(out[2] & ( 1 << PACKET_DATA_GRAVITY ))) {
			out[43] = (char)(data[0] >> 24);
			out[44] = (char)(data[0] >> 16);
			out[45] = (char)(data[0] >> 8);
			out[46] = (char)data[0];
			out[47] = (char)(data[1] >> 24);
			out[48] = (char)(data[1] >> 16);
			out[49] = (char)(data[1] >> 8);
			out[50] = (char)data[1];
			out[51] = (char)(data[2] >> 24);
			out[52] = (char)(data[2] >> 16);
			out[53] = (char)(data[2] >> 8);
			out[54] = (char)data[2];
		    out[2] |= (1 << type);
			//_MLPrintLog(0, "", "%8d AGCU(%d): %8d %8d %8d\n", OSA_TimeGetMsec(), type, data[0], data[1], data[2]);
    	}
        break;
	case PACKET_DATA_GRAVITY:
		if ( !(out[2] & ( 1 << PACKET_DATA_COMPASS ))) {
			out[43] = (char)(data[0] >> 24);
			out[44] = (char)(data[0] >> 16);
			out[45] = (char)(data[0] >> 8);
			out[46] = (char)data[0];
			out[47] = (char)(data[1] >> 24);
			out[48] = (char)(data[1] >> 16);
			out[49] = (char)(data[1] >> 8);
			out[50] = (char)data[1];
			out[51] = (char)(data[2] >> 24);
			out[52] = (char)(data[2] >> 16);
			out[53] = (char)(data[2] >> 8);
			out[54] = (char)data[2];
		    out[2] |= (1 << type);
			PRINTF("%8d AGCU(%d): %8d %8d %8d\n", OSA_TimeGetMsec(), type, data[0], data[1], data[2]);
		}
		break;
#if 0
    case PACKET_DATA_EULER:
        //memset(out, 0, PACKET_LENGTH);
/*
    	out[55] = (char)(data[0] >> 24);
        out[56] = (char)(data[0] >> 16);
        out[57] = (char)(data[0] >> 8);
        out[58] = (char)data[0];
        out[59] = (char)(data[1] >> 24);
        out[60] = (char)(data[1] >> 16);
        out[6] = (char)(data[1] >> 8);
        out[26] = (char)data[1];
        out[27] = (char)(data[2] >> 24);
        out[28] = (char)(data[2] >> 16);
        out[29] = (char)(data[2] >> 8);
        out[30] = (char)data[2];
        */
    	PRINTF(0, "", "%8d Euler: %8d %8d %8d\n", OSA_TimeGetMsec(), type, data[0], data[1], data[2]);
        break;
#endif
#if 0
    case PACKET_DATA_HEADING:
        memset(out, 0, PACKET_LENGTH);
        out[3] = (char)(data[0] >> 24);
        out[4] = (char)(data[0] >> 16);
        out[5] = (char)(data[0] >> 8);
        out[6] = (char)data[0];
    	PRINTF(0, "", "%8d Heading: %8d\n", OSA_TimeGetMsec(), data[0]);
        return;
        break;
#endif
    default:
        return;
    }
    out[0] = '$';
    out[1] = PACKET_DATA;
    out[61] = '\r';
    out[62] = '\n';
    PRINTF("Stuff stubbed out for USB here");
//    OSA_MutexLock(&g_app.output_mutex, OSA_WAIT_FOREVER);
    if(packet_ready)
    {
		packet_ready = 0;
		memcpy(output_buffers[1], output_buffers[0], PACKET_LENGTH);
		memset(out, 0, PACKET_LENGTH);
		//_MLPrintLog(0, "", "Sending packet %d\n", output_buffers[1][2]);
//		USB_Class_HID_Send_Data(g_app.app_handle, 1, output_buffers[1], PACKET_LENGTH);
    }
//    OSA_MutexUnlock(&g_app.output_mutex);
#endif
}

/**
 * @}
**/


