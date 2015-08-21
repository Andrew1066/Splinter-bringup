/*
 * display.c
 *
 *  Created on: May 19, 2015
 *      Author: alexf
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "tc35876x/tc35876x.h"
#include "tc358860/tc358860.h"
#include "ds90uh925q/ds90uh925q.h"
#include "Reset.h"
#include "i2c.h"
#include "MK22F12.h"

#define EDPDSI_BUS 1
#define DSILVDS_BUS0 2
#define DSILVDS_BUS1 0
#define DS25CP1X4_BUS 1

struct tegra_dc_mode mode_dual = {
        .pclk = 131780000,
        .h_ref_to_sync = 11,
        .v_ref_to_sync = 1,
        .h_sync_width = 80,
        .v_sync_width = 5,
        .h_back_porch = 216,
        .v_back_porch = 22,
        .h_active = 2560,
        .v_active = 720,
        .h_front_porch = 72,
        .v_front_porch = 3,
};

struct tegra_dc_mode mode_single = {
        .pclk = 74500000,
        .h_ref_to_sync = 11,
        .v_ref_to_sync = 1,
        .h_sync_width = 40,
        .v_sync_width = 3,
        .h_back_porch = 108,
        .v_back_porch = 11,
        .h_active = 1280,
        .v_active = 720,
        .h_front_porch = 36,
        .v_front_porch = 1,
};

static bool link_status = 0;
volatile bool init_requested = FALSE;

void hpd_interrupt_handler()
{
	PRINTF("hpd interrupt\n");
	bool new_link_status = tc358860_get_dp_link_status(EDPDSI_BUS);
	if (link_status == 0 && new_link_status == 1){
		tc358860_after_training(EDPDSI_BUS);
		tc35876x_init(DSILVDS_BUS0, 0x0f);
		tc35876x_init(DSILVDS_BUS1, 0x0f);
	}
	link_status = new_link_status;
}

void on_edp_irq() {
	PRINTF("Irq triggered\n");
}

void link_status_poll() {

	while(1) {
		bool new_link_status = tc358860_get_dp_link_status(EDPDSI_BUS);
		if (link_status != new_link_status ){
			PRINTF("Link status change: %d\n", new_link_status);
			link_status = new_link_status;
		}
	}
}

void dp_wait_for_training_completed(byte bus) {
	while(!tc358660_training_done(bus))
		usleep_range(10000, 10000);
}
void check_eeprom();
void init_devices()
{
	Reset_ClrBit(NULL, 0);
	Reset_ClrBit(NULL, 1);
	Reset_ClrBit(NULL, 2);
	usleep_range(100000, 100000);
	Reset_SetBit(NULL, 0);
	Reset_SetBit(NULL, 1);
	Reset_SetBit(NULL, 2);

	check_eeprom();
//CI2C2_Disable(GetI2cDevice(EDPDSI_BUS));

#if 0
	i2c_master_send_blocking(EDPDSI_BUS, 0x50, data, 1);
	i2c_master_receive_blocking(EDPDSI_BUS, 0x50, data, 128);
	i2c_master_receive_blocking(EDPDSI_BUS, 0x50, data + 128, 128);
	for(int i = 0; i < 256; i+=16) {
		for(int j = 0; j  <16; j++)
			PRINTF("0x%02x, ", data[i + j]);
		PRINTF("\n");
	}
#endif
#if 0
	memset(data, 0, 256);
	//memset(edid_data_from_eeprom + 128, 0, 128);
	for(int i = 0; i < 256; i += 8) {
		memcpy(data + 1, edid_new+ i, 8);
		data[0] = i;
		i2c_master_send_blocking(EDPDSI_BUS, 0x50, data, 9);
		usleep_range(100000, 100000);
	};
#endif
	//i2c_master_send_blocking(EDPDSI_BUS, 0x50, data, 3);

	tc358860_enable(EDPDSI_BUS);

//	Bit2_ClrVal(NULL);
//	usleep_range(200, 200);
//	Bit2_SetVal(NULL);

	//I2C_MasterReceiveBlock(EDPDSI_BUS, data, 1, LDD_I2C_SEND_STOP);

#if 1
//	tc358860_after_training(EDPDSI_BUS);

	usleep_range(4000, 5000);

	tc35876x_init(DSILVDS_BUS1, 0x0f);
	tc35876x_init(DSILVDS_BUS0, 0x0f);
#endif
}

bool eeprom_valid() {
	static byte buffer[128];
	buffer[0] = 0;
	bool ret;
	i2c_master_send_blocking(EDPDSI_BUS, 0x50, buffer, 1);
	i2c_master_receive_blocking(EDPDSI_BUS, 0x50, buffer, 128);
	ret = !memcmp(buffer, edid_tosh, 128);

	buffer[0] = 0x80;
	i2c_master_send_blocking(EDPDSI_BUS, 0x50, buffer, 1);
	i2c_master_receive_blocking(EDPDSI_BUS, 0x50, buffer, 128);
	ret &= !memcmp(buffer, edid_tosh + 128, 128);
	return ret;
}
void check_eeprom() {
	byte buffer[9];
	int i;
	if ( eeprom_valid() )
		return;

	for( i = 0; i < 256; i += 8 ) {
		buffer[0] = i;
		memcpy(buffer + 1, edid_tosh + i, 8);
		i2c_master_send_blocking(EDPDSI_BUS, 0x50, buffer, 9);
		usleep_range(100000, 100000);
	}
}

#if 0
void NVIC_ClearPendingIRQ(int IRQn)
{
  NVIC_ICPR(IRQn >> 5) = (1 << ((uint32_t)(IRQn) & 0x1F)); /* Clear pending interrupt */
}



PE_ISR(vsync_isr) {
	NVIC_ClearPendingIRQ(0x4E);
	//NVIC_ClearPendingIRQ()
	//NVIC_ICPR_CLRPEND(0x4e);
}
#endif

void on_vsync_change(bool active) {
	if ( active ) {
		//printf("vsync up\n");
		init_requested = TRUE;
	}
	else {
	//	printf("vsync down\n");
	}
}

