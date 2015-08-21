/*
 * fxos8700cq.c
 *
 *  Created on: May 19, 2015
 *      Author: alexf
 */

#include <stdio.h>
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "fxos8700cq.h"

#define DEVICE_ADDR 0x1d
#define FXOS8700CQ_WHOAMI 0xd

int fxos8700sq_reg_write(byte reg, byte val)
{
	byte block[2] =
	{
		reg,
		val
	};
	return i2c_master_send_blocking(0, DEVICE_ADDR, block, sizeof(block));
}

int fxos8700sq_reg_read(byte reg, byte *val)
{
	byte block[] =
	{
		reg,
	};
	i2c_master_send_blocking_no_stop(0, DEVICE_ADDR, block, 1);
	return i2c_master_receive_blocking(0, DEVICE_ADDR, val, 1);
}

void fxos8700sq_init()
{
	byte who;
	int ret = fxos8700sq_reg_read(FXOS8700CQ_WHOAMI, &who);
	PRINTF("%d, %02x\n", ret, who);
}

