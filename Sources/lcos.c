#include "usb.h"
#include "fsl_i2c_master_driver.h"
#include "lcos.h"
#include "lcos_data.h"
#include "colourbar.h"

i2c_device_t device_desc = {
		//.address = addr,
		.baudRate_kbps = 100,
};

static i2c_status_t write_table(uint8_t instance, uint8_t addr, uint8_t* table, size_t size)
{
	uint8_t* p = table;
	uint32_t i;
	i2c_status_t status = kStatus_I2C_Success;
	device_desc.address = addr;
	for(i = 0; i < size; i++)
	{
		//status = I2C_DRV_MasterSendDataBlocking(instance, &lcos, NULL, 0, p, 2, 100);
		status = I2C_DRV_MasterSendData(instance, &device_desc, NULL, 0, p, 2);
		if ( status != kStatus_I2C_Success )
			break;
		do {
			status = I2C_DRV_MasterGetSendStatus(instance, NULL);
		} while( status == kStatus_I2C_Busy);
//		OSA_TimeDelay(1);
		p += 2;
		if ( status != kStatus_I2C_Success )
			break;
	}

	return status;
}

#define WRITE_TABLE(instance, addr, table) write_table(instance, addr, (uint8_t* )table, sizeof(table)/sizeof(table[0]) / 2)

i2c_status_t init_lcos_ctrlr(uint8_t instance)
{
	if ( instance == 0)
		return WRITE_TABLE(instance, 0x3c,  hx7816_colourbar_regs);
	else
		return WRITE_TABLE(instance, 0x3c,  hx7816_colourbar_regs);
}

i2c_status_t init_lcos(uint8_t instance)
{
	if ( instance == 0)
		return WRITE_TABLE(instance, 0x49, hx7318_left_init_regs);
	else
		return WRITE_TABLE(instance, 0x49, hx7318_right_init_regs);
}

i2c_status_t init_lm3549(uint8_t instance)
{
	uint8_t status = 0x17;
	device_desc.address = 0x36;

	I2C_DRV_MasterReceiveData(instance, &device_desc, &status, 1, &status, 1);
	if ( status & 0x18 ) // LED open
	{
//		_MLPrintLog(0, "", "Disabling LEPS %d because of status %02x\n", instance, status);
//		DisableLEPS(instance);
//		return 0;
	}
	if ( instance == 0)
		return WRITE_TABLE(instance, 0x36, lm3549_left_init_regs);
	else
		return WRITE_TABLE(instance, 0x36, lm3549_right_init_regs);
}

