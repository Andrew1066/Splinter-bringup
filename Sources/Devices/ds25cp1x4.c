/*
 * ds25cp1x4.c
 *
 *  Created on: May 19, 2015
 *      Author: alexf
 */
#include <stdio.h>
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "ds25cp1x4.h"

#define DEVICE_ADDR 0x5f


static int ds25cp1x4_reg_write(byte bus, byte reg, byte val)
{
	byte block[2] =
	{
		reg,
		val
	};
	return i2c_master_send_blocking(bus, DEVICE_ADDR, block, sizeof(block));
}

static int ds25cp1x4_reg_read(byte bus, byte reg, byte *val)
{
	byte block[] =
	{
		reg,
	};
	i2c_master_send_blocking(bus, DEVICE_ADDR, block, 1);
	return i2c_master_receive_blocking(bus, DEVICE_ADDR, val, 1);
}


void ds25cp1x4_init(byte bus){
    byte pe = 0x55; // Preemphasis - low for all
    byte eq = 0x55; // Equalizer - low for all

    union {
        struct {
        byte out0:2;
        byte out1:2;
        byte out2:2;
        byte out3:2;
        };
        byte config;
    } u1;
    u1.out0 = 0;
    u1.out2 = 2;

    union {
		struct {
			byte enable:4;
			byte eq_ctrl:1;
			byte pe_ctrl:1;
			byte los_ctrl:1;
			byte soft_pwdwn:1;
			};
			byte ctrl;
    } u2;

    u2.eq_ctrl = 1;
    u2.pe_ctrl = 1;
    u2.los_ctrl = 0;
    u2.soft_pwdwn = 1;
    u2.enable = 0x05; // Enable 0 and 2

    ds25cp1x4_reg_write(bus, 1, pe);
    ds25cp1x4_reg_write(bus, 2, eq);
    ds25cp1x4_reg_write(bus, 0, u1.config);
    ds25cp1x4_reg_write(bus, 3, u2.ctrl);
}
