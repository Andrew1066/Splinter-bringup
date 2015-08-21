/*
 * tc358860.c
 *
 *  Created on: May 20, 2015
 *      Author: alexf
 */
#include <stdio.h>
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "tc358860.h"
#include "tc358860-defines.h"
#include "i2c.h"
#include "WAIT1.h"
//#include "GPIO_B.h"

// RST - PU3 - PTE3

#define DEVICE_ADDR 0x68
#define REG_WRITE32(a, b) tc358860_reg_write(bus, DEVICE_ADDR, a, b)
#define REG_WRITE8(a, b) tc358860_reg_write_short(bus, DEVICE_ADDR, a, b)

#define REG_READ(a) tc358860_reg_read_short(bus, DEVICE_ADDR, a)
//#define CHECK(a, b) printf("Received %02x expected %02x\n", a, b)
#define CHECK(a, b) a

void usleep_range(uint32_t min, uint32_t max) {
	WAIT1_WaitOSms(max / 1000);
}

static int tc358860_reg_write(byte bus, byte dev, uint16_t reg, uint32_t val)
{
	uint8_t block[] =
	{
		reg >> 8,
		reg & 0xff,
		val & 0xff,
		(val >> 8) & 0xff,
		(val >> 16) & 0xff,
		(val >> 24) & 0xff,
	};
	return i2c_master_send_blocking(bus, dev, (uint8_t*)block, sizeof(block));
}

static int tc358860_reg_write_short(byte bus, byte dev, uint16_t reg, uint8_t val)
{
	uint8_t block[] =
	{
		reg >> 8,
		reg & 0xff,
		val,
	};
	return i2c_master_send_blocking(bus, dev, (uint8_t*)block, sizeof(block));
}

static int tc358860_reg_read(byte bus, byte dev, uint16_t reg, uint32_t *val)
{
	uint8_t block[4] =
	{
		reg >> 8,
		reg & 0xff,
	};
	i2c_master_send_blocking(bus, dev, (uint8_t*)block, 2);
	int ret = i2c_master_receive_blocking(bus, dev, block, 4);
	if ( ! ret && val) {
		*val = block[0] << 24 | block[1] << 16 | block[2] << 8 | block[3];
	}
	return ret;
}

static uint8_t tc358860_reg_read_short(byte bus, byte dev, uint16_t reg)
{
	uint8_t block[4] =
	{
		reg >> 8,
		reg & 0xff,
	};
	i2c_master_send_blocking(bus, dev, (uint8_t*)block, 2);
	int ret = i2c_master_receive_blocking(bus, dev, block, 1);
	if ( ret )
		PRINTF("i2c_master_receive: %02x\n", ret);
	return block[0];
}

int tc358860_enable(uint8_t bus) {

	uint32_t val;
	// 1.    Power On Sequence

	// 2.    IO Voltage Setting

	REG_WRITE8( 0x0800,  0x01 ); // Set IOB_CTRL1
	// 3.    Boot Sequence

	REG_WRITE32( 0xb010, 0x00000101 );

#if 1
	REG_WRITE32( 0x1000, 0x00006978 ); // Set Wait0Cnt
	REG_WRITE32( 0x1004, 0x00040906 ); // Set BootSet0	27 MHz
//	REG_WRITE32( 0x1004, 0x00050501 ); // Set BootSet0	25 MHz
	REG_WRITE32( 0x1008, 0x05b80003 ); // Set BootSet1
	REG_WRITE32( 0x100c, 0x030008AB ); // Set BootSet2
	REG_WRITE32( 0x1010, 0x00c80020 ); // Set BootSet3
	REG_WRITE32( 0x1014, 0x00000001 ); // Set BootCtrl
	usleep_range(1100, 1300);
	val = REG_READ(0x1018);
	CHECK(val, 0x06); //Read Boot Status (can skip)
	REG_WRITE8( 0x8003,  0x01 ); // Set MAX_DOWNSPREAD
	REG_WRITE8( 0xb400,  0x0C ); // Set DPRX_AL_MODE_CTRL

//	REG_WRITE8( 0xb632, 0x12); // 25 MHz

	REG_WRITE8( 0xb005,  0x0a );
	REG_WRITE8( 0xb006,  0x08 );
	REG_WRITE8( 0xb007,  0x2e );
	REG_WRITE8( 0xb008,  0x00 );
	REG_WRITE8( 0xb009,  0x22 );
	REG_WRITE8( 0xb00a,  0x03 );


	REG_WRITE8( 0xb88e,  0xff ); // Set CR_OPT_WCNT 0	0xb88e
	REG_WRITE8( 0xb88f,  0xff ); // Set CR_OPT_WCNT 1	0xb88f
	REG_WRITE8( 0xb89a,  0xff ); // Set CR_OPT_WCNT 2	0xb89a
	REG_WRITE8( 0xb89b,  0xff ); // Set CR_OPT_WCNT 3	0xb89b
	REG_WRITE8( 0xb800,  0x0E ); // Set CDR_PHASE_LP_EN	0xb800
	REG_WRITE8( 0xBB26,  0x02 ); // RX_VREG_VALUE
	REG_WRITE8( 0xBB01,  0x20 ); // RX_VREG_ENABLE
	REG_WRITE8( 0xB8C0,  0xf1 ); // RX_CDR_LUT1
	REG_WRITE8( 0xB8C1,  0xf1 ); // RX_CDR_LUT2
	REG_WRITE8( 0xB8C2,  0xf0 ); // RX_CDR_LUT3
	REG_WRITE8( 0xB8C3,  0xf0 ); // RX_CDR_LUT4
	REG_WRITE8( 0xB8C4,  0xf0 ); // RX_CDR_LUT5
	REG_WRITE8( 0xB8C5,  0xf0 ); // RX_CDR_LUT6
	REG_WRITE8( 0xB8C6,  0xf0 ); // RX_CDR_LUT7
	REG_WRITE8( 0xB8C7,  0xf0 ); // RX_CDR_LUT8
	REG_WRITE8( 0xB80B,  0x00 ); // PLL_CP1P1
	REG_WRITE8( 0xB833,  0x00 ); // PLL_CP1P2
	REG_WRITE8( 0xB85B,  0x00 ); // PLL_CP1P3
	REG_WRITE8( 0xB810,  0x00 ); // PLL_CP2P1
	REG_WRITE8( 0xB838,  0x00 ); // PLL_CP2P2
	REG_WRITE8( 0xB860,  0x00 ); // PLL_CP2P3
	REG_WRITE8( 0xB815,  0x00 ); // PLL_CP2P4
	REG_WRITE8( 0xB83D,  0x00 ); // PLL_CP2P5
	REG_WRITE8( 0xB865,  0x00 ); // PLL_CP2P6
	REG_WRITE8( 0xB81A,  0x00 ); // PLL_CP2P7
	REG_WRITE8( 0xB842,  0x00 ); // PLL_CP2P8
	REG_WRITE8( 0xB86A,  0x00 ); // PLL_CP2P9
	REG_WRITE8( 0xB81F,  0x00 ); // PLL_CP3P1
	REG_WRITE8( 0xB847,  0x00 ); // PLL_CP3P2
	REG_WRITE8( 0xB86F,  0x00 ); // PLL_CP3P3
	REG_WRITE8( 0xB824,  0x00 ); // PLL_CP4P1
	REG_WRITE8( 0xB84C,  0x00 ); // PLL_CP4P2
	REG_WRITE8( 0xB874,  0x00 ); // PLL_CP4P3
	REG_WRITE8( 0xB829,  0x00 ); // PLL_CP4P4
	REG_WRITE8( 0xB851,  0x00 ); // PLL_CP4P5
	REG_WRITE8( 0xB879,  0x00 ); // PLL_CP4P6
	REG_WRITE8( 0xB82E,  0x00 ); // PLL_CP5P7
	REG_WRITE8( 0xB856,  0x00 ); // PLL_CP5P2
	REG_WRITE8( 0xB87E,  0x00 ); // PLL_CP5P3
	REG_WRITE8( 0xBB90,  0x10 ); // ctle_em_data_rate_control_0[7:0]
	REG_WRITE8( 0xBB91,  0x0F ); // ctle_em_data_rate_control_1[7:0]
	REG_WRITE8( 0xBB92,  0xF6 ); // ctle_em_data_rate_control_2[7:0]
	REG_WRITE8( 0xBB93,  0x10 ); // ctle_em_data_rate_control_3[7:0]
	REG_WRITE8( 0xBB94,  0x0F ); // ctle_em_data_rate_control_4[7:0]
	REG_WRITE8( 0xBB95,  0xF6 ); // ctle_em_data_rate_control_5[7:0]
	REG_WRITE8( 0xBB96,  0x10 ); // ctle_em_data_rate_control_6[7:0]
	REG_WRITE8( 0xBB97,  0x0F ); // ctle_em_data_rate_control_7[7:0]
	REG_WRITE8( 0xBB98,  0xF6 ); // ctle_em_data_rate_control_8[7:0]
	REG_WRITE8( 0xBB99,  0x10 ); // ctle_em_data_rate_control_9[7:0]
	REG_WRITE8( 0xBB9a,  0x0F ); // ctle_em_data_rate_control_a[7:0]
	REG_WRITE8( 0xBB9b,  0xF6 ); // ctle_em_data_rate_control_b[7:0]
	REG_WRITE8( 0xB88A,  0x03 ); // CR_OPT_CTRL
	REG_WRITE8( 0xB896,  0x03 ); // EQ_OPT_CTRL
	REG_WRITE8( 0xBBD1,  0x07 ); // ctle_em_contro_1
	REG_WRITE8( 0xBBB0,  0x07 ); // eye_configuration_0
	REG_WRITE8( 0xB88B,  0x04 ); // CR_OPT_MIN_EYE_VALID
	REG_WRITE8( 0xB88C,  0x45 ); // CR_OPT_WCNT0_EYE
	REG_WRITE8( 0xB88D,  0x05 ); // CT_OPT_WCNT1_EYE
	REG_WRITE8( 0xB897,  0x04 ); // EQ_OPT_MIN_EYE_VALID
	REG_WRITE8( 0xB898,  0x17 ); // EQ_OPT_WCNT0_FEQ
	REG_WRITE8( 0xB899,  0x15 ); // EQ_OPT_WCNT1_FEQ
	REG_WRITE32( 0x1014, 0x00000007 ); // Set BootCtrl (Resume 1)

	REG_WRITE32( 0x0110, 0x00000500 );
	REG_WRITE32( 0x0124, 0x00000500 );
	REG_WRITE32( 0x0148, 0x00000a00 );
	REG_WRITE32( 0x2920, 0x3e0b0f00 );
	REG_WRITE32( 0x3920, 0x3e0b0f00 );

#else
	REG_WRITE8( 0x0800,  0x01 ); // Set IOB_CTRL1
	// 3.    Boot Sequence

	REG_WRITE32( 0xb010, 0x00000101 );

	REG_WRITE32( 0x1000, 0x00006996 ); // Set Wait0Cnt
	REG_WRITE32( 0x1004, 0x00050901 ); // Set BootSet0	27 MHz
//	REG_WRITE32( 0x1004, 0x00050501 ); // Set BootSet0	25 MHz
	REG_WRITE32( 0x1008, 0x03380003 ); // Set BootSet1
	REG_WRITE32( 0x100c, 0x030008AB ); // Set BootSet2
	REG_WRITE32( 0x1010, 0x01280020 ); // Set BootSet3
	REG_WRITE32( 0x1014, 0x00000003 ); // Set BootCtrl
	usleep_range(1100, 1300);
	val = REG_READ(0x1018);
	CHECK(val, 0x06); //Read Boot Status (can skip)
	REG_WRITE8( 0x8003,  0x01 ); // Set MAX_DOWNSPREAD
	REG_WRITE8( 0xb400,  0x0C ); // Set DPRX_AL_MODE_CTRL

//	REG_WRITE8( 0xb632, 0x12); // 25 MHz

	REG_WRITE8( 0xb88e,  0xff ); // Set CR_OPT_WCNT 0	0xb88e
	REG_WRITE8( 0xb88f,  0xff ); // Set CR_OPT_WCNT 1	0xb88f
	REG_WRITE8( 0xb89a,  0xff ); // Set CR_OPT_WCNT 2	0xb89a
	REG_WRITE8( 0xb89b,  0xff ); // Set CR_OPT_WCNT 3	0xb89b
	REG_WRITE8( 0xb800,  0x0E ); // Set CDR_PHASE_LP_EN	0xb800
	REG_WRITE8( 0xBB26,  0x02 ); // RX_VREG_VALUE
	REG_WRITE8( 0xBB01,  0x20 ); // RX_VREG_ENABLE
	REG_WRITE8( 0xB8C0,  0xf1 ); // RX_CDR_LUT1
	REG_WRITE8( 0xB8C1,  0xf1 ); // RX_CDR_LUT2
	REG_WRITE8( 0xB8C2,  0xf0 ); // RX_CDR_LUT3
	REG_WRITE8( 0xB8C3,  0xf0 ); // RX_CDR_LUT4
	REG_WRITE8( 0xB8C4,  0xf0 ); // RX_CDR_LUT5
	REG_WRITE8( 0xB8C5,  0xf0 ); // RX_CDR_LUT6
	REG_WRITE8( 0xB8C6,  0xf0 ); // RX_CDR_LUT7
	REG_WRITE8( 0xB8C7,  0xf0 ); // RX_CDR_LUT8
	REG_WRITE8( 0xB80B,  0x00 ); // PLL_CP1P1
	REG_WRITE8( 0xB833,  0x00 ); // PLL_CP1P2
	REG_WRITE8( 0xB85B,  0x00 ); // PLL_CP1P3
	REG_WRITE8( 0xB810,  0x00 ); // PLL_CP2P1
	REG_WRITE8( 0xB838,  0x00 ); // PLL_CP2P2
	REG_WRITE8( 0xB860,  0x00 ); // PLL_CP2P3
	REG_WRITE8( 0xB815,  0x00 ); // PLL_CP2P4
	REG_WRITE8( 0xB83D,  0x00 ); // PLL_CP2P5
	REG_WRITE8( 0xB865,  0x00 ); // PLL_CP2P6
	REG_WRITE8( 0xB81A,  0x00 ); // PLL_CP2P7
	REG_WRITE8( 0xB842,  0x00 ); // PLL_CP2P8
	REG_WRITE8( 0xB86A,  0x00 ); // PLL_CP2P9
	REG_WRITE8( 0xB81F,  0x00 ); // PLL_CP3P1
	REG_WRITE8( 0xB847,  0x00 ); // PLL_CP3P2
	REG_WRITE8( 0xB86F,  0x00 ); // PLL_CP3P3
	REG_WRITE8( 0xB824,  0x00 ); // PLL_CP4P1
	REG_WRITE8( 0xB84C,  0x00 ); // PLL_CP4P2
	REG_WRITE8( 0xB874,  0x00 ); // PLL_CP4P3
	REG_WRITE8( 0xB829,  0x00 ); // PLL_CP4P4
	REG_WRITE8( 0xB851,  0x00 ); // PLL_CP4P5
	REG_WRITE8( 0xB879,  0x00 ); // PLL_CP4P6
	REG_WRITE8( 0xB82E,  0x00 ); // PLL_CP5P7
	REG_WRITE8( 0xB856,  0x00 ); // PLL_CP5P2
	REG_WRITE8( 0xB87E,  0x00 ); // PLL_CP5P3
	REG_WRITE8( 0xBB90,  0x10 ); // ctle_em_data_rate_control_0[7:0]
	REG_WRITE8( 0xBB91,  0x0F ); // ctle_em_data_rate_control_1[7:0]
	REG_WRITE8( 0xBB92,  0xF6 ); // ctle_em_data_rate_control_2[7:0]
	REG_WRITE8( 0xBB93,  0x10 ); // ctle_em_data_rate_control_3[7:0]
	REG_WRITE8( 0xBB94,  0x0F ); // ctle_em_data_rate_control_4[7:0]
	REG_WRITE8( 0xBB95,  0xF6 ); // ctle_em_data_rate_control_5[7:0]
	REG_WRITE8( 0xBB96,  0x10 ); // ctle_em_data_rate_control_6[7:0]
	REG_WRITE8( 0xBB97,  0x0F ); // ctle_em_data_rate_control_7[7:0]
	REG_WRITE8( 0xBB98,  0xF6 ); // ctle_em_data_rate_control_8[7:0]
	REG_WRITE8( 0xBB99,  0x10 ); // ctle_em_data_rate_control_9[7:0]
	REG_WRITE8( 0xBB9a,  0x0F ); // ctle_em_data_rate_control_a[7:0]
	REG_WRITE8( 0xBB9b,  0xF6 ); // ctle_em_data_rate_control_b[7:0]
	REG_WRITE8( 0xB88A,  0x03 ); // CR_OPT_CTRL
	REG_WRITE8( 0xB896,  0x03 ); // EQ_OPT_CTRL
	REG_WRITE8( 0xBBD1,  0x07 ); // ctle_em_contro_1
	REG_WRITE8( 0xBBB0,  0x07 ); // eye_configuration_0
	REG_WRITE8( 0xB88B,  0x04 ); // CR_OPT_MIN_EYE_VALID
	REG_WRITE8( 0xB88C,  0x45 ); // CR_OPT_WCNT0_EYE
	REG_WRITE8( 0xB88D,  0x05 ); // CT_OPT_WCNT1_EYE
	REG_WRITE8( 0xB897,  0x04 ); // EQ_OPT_MIN_EYE_VALID
	REG_WRITE8( 0xB898,  0x17 ); // EQ_OPT_WCNT0_FEQ
	REG_WRITE8( 0xB899,  0x15 ); // EQ_OPT_WCNT1_FEQ
	REG_WRITE32( 0x1014, 0x00000007 ); // Set BootCtrl (Resume 1)
#endif
	usleep_range(1100, 1300);
	CHECK(REG_READ(0x1018), 0x07); //Read Boot Status (can skip)
	REG_WRITE8( 0xb800,  0x1E ); // Set CDR_PHASE_LP_EN
	// 4.    Additional mode setting

	REG_WRITE8( 0x8001,  0x0a ); // Set MAX LINK RATE
	REG_WRITE8( 0x8002,  0x04 ); // Set MAX LANE COUNT
	REG_WRITE8( 0xb608,  0x0B ); // Set AUXTXHSEN
	REG_WRITE8( 0xb800,  0x1e );
	REG_WRITE8( 0x0700,  0x00 );

	REG_WRITE8( 0x8200,  0x01 ); // Set MAX LANE COUNT

	//REG_WRITE8( 0x810a,  0x01 ); // Set EDP ASC RESET ENABLE
	REG_WRITE32( 0x0110, 0x00000500 ); // Set HTIM2_LEFT
	REG_WRITE32( 0x0124, 0x00000500 ); // Set HTIM2_RIGHT
	REG_WRITE32( 0x0148, 0x00000A00 ); // Set DPRX_HTIM2
	REG_WRITE32( 0x2920, 0x3E0B0F00 ); // Set DSI0_PIC_SYN_PKT
	REG_WRITE32( 0x3920, 0x3E0B0F00 ); // Set DSI1_PIC_SYN_PKT

//	REG_WRITE32( 0x41b0, 0x0000024a ); // Set DSI1_PIC_SYN_PKT
//	REG_WRITE32( 0x41bc, 0x00000050 ); // Set DSI1_PIC_SYN_PKT
//	REG_WRITE32( 0x41c0, 0x00000030 ); // Set DSI1_PIC_SYN_PKT

	// 5.    Set MAX_LANE_COUNT DSI Start

	REG_WRITE32( 0x201c, 0x00000001 ); // Set DSITX_START
	REG_WRITE32( 0x301c, 0x00000001 ); // Set DSITX_START
	usleep_range(1100, 1300);
	// 6.    Panel Setting

	REG_WRITE32( 0x22fc, 0x80773A15 ); // CQ set_pixel_format
	usleep_range(50, 100);
	// Wait (I2C:50us,  AUX:300us)

	CHECK(REG_READ(0x2200), 0x01); //Read CQ status (can skip)
	REG_WRITE32( 0x22fc, 0x80001105 ); // CQ exit_sleep_mode
	usleep_range(50, 100);
	// Wait (I2C:50us,  AUX:300us)

	CHECK(REG_READ(0x2200), 0x01); //Read CQ status (can skip)

	//CHECK(REG_READ(0xb201), 0x);
	//REG_WRITE8(0xb200, 0);
	REG_WRITE8(0xb201, 0x30);
	REG_WRITE8(0xb202, 0x00);
	REG_WRITE8(0xb203, 0x0);
	REG_WRITE8(0xb204, 4);
	REG_WRITE8(0xb205, 0);

	REG_WRITE32(0x0080, 0x00000001);
    usleep_range(1000, 1000);
	REG_WRITE32(0x008c, 0x00000044);
    usleep_range(10000, 10000);
	REG_WRITE32(0x5010, 0x0169016a);
	REG_WRITE32(0x5014, 0x01750174);
    usleep_range(10000, 10000);

    REG_WRITE32(0x00b0, 1);

    CI2C2_Disable(GetI2cDevice(bus));

/*
	GPIO_B_Init(NULL);
	GPIO_B_SetPortOutputDirection(NULL, 1, 0);
	GPIO_B_SetFieldValue(NULL, HPD, 0);
	usleep_range(600, 900);
	GPIO_B_SetFieldValue(NULL, HPD, 1);
*/
}
#undef CHECK
#define CHECK(a, b) PRINTF("Received %02x expected %02x\n", a, b)
void tc358860_after_training(byte bus)
{
	uint32_t val;
	// 7.    DP Training

	//REG_WRITE32(0x00b0, 0);

    CI2C2_Enable(GetI2cDevice(bus));
    REG_WRITE32(0x00b0, 0);
    usleep_range(100000, 100000);
	CHECK(REG_READ(0xb631), 0x01); //Read DPRX_MainLink Status
	CHECK(REG_READ(0x8202), 0x77); //Read Lane status (can skip)
	CHECK(REG_READ(0x8204), 0x01); //Read Align status (can skip)
	// 10.  Start Video Transmission

	REG_WRITE32( 0x2a10, 0x80040010 ); // Set DSI0_CQMODE
	REG_WRITE32( 0x3a10, 0x80040010 ); // Set DSI1_CQMODE
	REG_WRITE32( 0x2a04, 0x00000001 ); // Set DSI0_VideoSTART
	REG_WRITE32( 0x3a04, 0x00000001 ); // Set DSI1_VideoSTART
	REG_WRITE32( 0x0154, 0x00000001 ); // Set DPVideoEn
	// 8.    TC358764 configuration

	// 9.    LCOS and controller configuration

	// Wait ( 1 frame)
	usleep_range(17000, 20000);

	REG_WRITE32( 0x42fc, 0x8000b023 ); // CQ MCAP
	usleep_range(17000, 20000);

	//CHECK(REG_READ(0x4200), 0x01); //Read CQ status (can skip)
	REG_WRITE32( 0x42fc, 0x8014b323 ); // CQ Interface setting
	usleep_range(17000, 20000);

	CHECK(REG_READ(0x4200), 0x01); //Read CQ status (can skip)
	REG_WRITE32( 0x42fc, 0x8003b023 ); // CQ MCAP
    REG_WRITE32(0x00b0, 1);
    CI2C2_Disable(GetI2cDevice(bus));
}

bool tc358660_training_done(uint8_t bus) {
	return REG_READ(0xb631) == 1 && REG_READ(0x8202) == 0x77;
}

uint8_t tc358860_get_dp_link_status(byte bus) {
	return REG_READ(0xb631);
}
