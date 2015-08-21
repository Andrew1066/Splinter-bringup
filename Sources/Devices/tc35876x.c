/*
 * tc35876x.c
 *
 *  Created on: May 19, 2015
 *      Author: alexf
 */

#include <stdio.h>
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "i2c.h"
#include "tc35876x.h"
#include "tc35876x-defines.h"



static int tc35876x_reg_write(byte bus, byte dev, uint16_t reg, uint32_t val)
{
	uint8_t block[6] =
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

static int tc35876x_reg_read(byte bus, byte dev, uint16_t reg, uint32_t *val)
{
	LDD_TError err;
	uint8_t block[4] =
	{
		reg >> 8,
		reg & 0xff,
	};
	err = i2c_master_send_blocking(bus, dev, (uint8_t*)block, 2);
	if ( err ) {
		PRINTF("Failed to send address: %d\n", err);
		return err;
	}
	err = i2c_master_receive_blocking(bus, dev, block, 4);
	if ( ! err && val) {
		*val = block[0] << 24 | block[1] << 16 | block[2] << 8 | block[3];
	}
	return err;
}

#define TRY_WRITE(a) \
if ( (err = a) ) { \
	printf("%s failed (%d)\n", #a, err); \
	goto fail; \
}

static int tc35876x_configure_lvds_bridge(byte bus, byte dev, struct tegra_dc_mode *mode)
{
	uint32_t ppi_lptxtimecnt;
	uint32_t txtagocnt;
	uint32_t txtasurecnt;
	uint32_t id;
	int err = 0;


	if (!tc35876x_reg_read(bus, dev, IDREG, &id)) {
		//printf("tc35876x ID 0x%08x\n", id);
	}
	else
		PRINTF("Cannot read ID: bus=%d, dev = %d\n", bus, dev);

	ppi_lptxtimecnt = 4;
	txtagocnt = (5 * ppi_lptxtimecnt - 3) / 4;
	txtasurecnt = 3 * ppi_lptxtimecnt / 2;
	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_TX_RX_TA, FLD_VAL(txtagocnt, 26, 16) | FLD_VAL(txtasurecnt, 10, 0))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_LPTXTIMECNT, FLD_VAL(ppi_lptxtimecnt, 10, 0))))

	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_D0S_CLRSIPOCOUNT, FLD_VAL(1, 5, 0))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_D1S_CLRSIPOCOUNT, FLD_VAL(1, 5, 0))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_D2S_CLRSIPOCOUNT, FLD_VAL(1, 5, 0))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_D3S_CLRSIPOCOUNT, FLD_VAL(1, 5, 0))))

	/* Enabling MIPI & PPI lanes, Enable 4 lanes */
	//TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_LANEENABLE, BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))))
	//TRY_WRITE((tc35876x_reg_write(bus, dev, DSI_LANEENABLE, BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))))

	//printf("Setting lane mask for %d lanes to %x\n", DSI2LVDS_NUM_LANES, (1 << (DSI2LVDS_NUM_LANES + 1)) - 1);

	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_LANEENABLE, (1 << (DSI2LVDS_NUM_LANES + 1)) - 1)))
	TRY_WRITE((tc35876x_reg_write(bus, dev, DSI_LANEENABLE, (1 << (DSI2LVDS_NUM_LANES + 1)) - 1)))
	TRY_WRITE((tc35876x_reg_write(bus, dev, PPI_STARTPPI, BIT(0))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, DSI_STARTDSI, BIT(0))))

	/* Setting LVDS output frequency */
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVPHY0, FLD_VAL(1, 20, 16) | FLD_VAL(2, 15, 14) | FLD_VAL(6, 4, 0)))) /* 0x00048006 */

	/* Setting video panel control register,0x00000120 VTGen=ON ?!?!? */
	TRY_WRITE((tc35876x_reg_write(bus, dev, VPCTRL, BIT(8) | BIT(5))))

	/* Horizontal back porch and horizontal pulse width. 0x00280028 */
	TRY_WRITE((tc35876x_reg_write(bus, dev, HTIM1, FLD_VAL(mode->h_back_porch, 24, 16) | FLD_VAL(mode->h_sync_width, 8, 0))))

	/* Horizontal front porch and horizontal active video size. 0x00500500*/
	TRY_WRITE((tc35876x_reg_write(bus, dev, HTIM2, FLD_VAL(mode->h_front_porch, 24, 16) | FLD_VAL(mode->h_active, 10, 0))))

	/* Vertical back porch and vertical sync pulse width. 0x000e000a */
	TRY_WRITE((tc35876x_reg_write(bus, dev, VTIM1, FLD_VAL(mode->v_back_porch, 23, 16) | FLD_VAL(mode->v_sync_width, 7, 0))))

	/* Vertical front porch and vertical display size. 0x000e0320 */
	TRY_WRITE((tc35876x_reg_write(bus, dev, VTIM2, FLD_VAL(mode->v_front_porch, 23, 16) | FLD_VAL(mode->v_active, 10, 0))))

	/* Set above HTIM1, HTIM2, VTIM1, and VTIM2 at next VSYNC. */
	TRY_WRITE((tc35876x_reg_write(bus, dev, VFUEN, BIT(0))))

	/* Soft reset LCD controller. */
	TRY_WRITE((tc35876x_reg_write(bus, dev, SYSRST, BIT(2))))

	/* LVDS-TX input muxing */
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX0003, INPUT_MUX(INPUT_R5, INPUT_R4, INPUT_R3, INPUT_R2))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX0407, INPUT_MUX(INPUT_G2, INPUT_R7, INPUT_R1, INPUT_R6))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX0811, INPUT_MUX(INPUT_G1, INPUT_G0, INPUT_G4, INPUT_G3))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX1215, INPUT_MUX(INPUT_B2, INPUT_G7, INPUT_G6, INPUT_G5))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX1619, INPUT_MUX(INPUT_B4, INPUT_B3, INPUT_B1, INPUT_B0))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX2023, INPUT_MUX(LOGIC_0,  INPUT_B7, INPUT_B6, INPUT_B5))))
	TRY_WRITE((tc35876x_reg_write(bus, dev, LVMX2427, INPUT_MUX(INPUT_R0, INPUT_DE, INPUT_VSYNC, INPUT_HSYNC))))

	/* Enable LVDS transmitter. */
//	TRY_WRITE(tc35876x_reg_write(bus, dev, LVCFG, BIT(0)|(4 << 4)))
	TRY_WRITE(tc35876x_reg_write(bus, dev, LVCFG, BIT(0)|BIT(9)))

	/* Clear notifications. Don't write reserved bits. Was write 0xffffffff
	 * to 0x0288, must be in error?! */
	TRY_WRITE((tc35876x_reg_write(bus, dev, DSI_INTCLR, FLD_MASK(31, 30) | FLD_MASK(22, 0))))

fail:
	return err;
}


void tc35876x_init(byte bus, byte dev){
	tc35876x_configure_lvds_bridge(bus, dev, &mode_single);
}


