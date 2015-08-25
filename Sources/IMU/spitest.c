/*
 * spitest.c
 *
 *  Created on: Jan 19, 2015
 *      Author: alexf
 */
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device_stack_interface.h"
//#include "usb_class_hid.h"
//#include "main.h"   /* Keyboard Application Header File */
//#include "usb_descriptor.h"
#include "dispatcher.h"

#include "spitest.h"

int SHV_IMU_SendData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t*data);
int SHV_IMU_ReadData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data);
uint8_t v;

#define BIT_PWR_MGMT_1_DEVICE_RESET 0x80
#define BIT_PWR_MGMT_1_CLK_ZGYRO 0x10

#define BIT_USER_CTRL_I2C_IF_DIS 0x10


uint8_t _spi_read(uint8_t instance,  uint8_t reg)
{
	SHV_IMU_ReadData(instance, reg, 1, &v);
	return v;
}

void _spi_write(uint8_t instance,  uint8_t reg, uint8_t val)
{
	SHV_IMU_SendData(instance, reg, 1, &val);
}

#define delay OSA_TimeDelay

uint8_t instance;

#define spi_read(a) _spi_read(instance, a)
#define spi_write(a, b) _spi_write(instance, a, b)
#if 0
void run_test(uint8_t inst)
{

	uint32_t sample_rate = 200;
	instance = inst;
	spi_write(MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_DEVICE_RESET);
	delay(100);

	// Wake up device and select GyroZ clock. Note that the
	// MPU6000 starts up in sleep mode, and it can take some time
	// for it to come out of sleep
	spi_write(MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_CLK_ZGYRO);
	delay(5);

	// check it has woken up
	//spi_read(MPUREG_PWR_MGMT_1) == BIT_PWR_MGMT_1_CLK_ZGYRO)

	spi_write(MPUREG_USER_CTRL, BIT_USER_CTRL_I2C_IF_DIS);

	// only used for wake-up in accelerometer only low power mode
	spi_write(MPUREG_PWR_MGMT_2, 0x00);

	// Disable I2C bus (recommended on datasheet)
	spi_write(MPUREG_USER_CTRL, BIT_USER_CTRL_I2C_IF_DIS);

	/*
	// sample rate and filtering
	// to minimise the effects of aliasing we choose a filter
	// that is less than half of the sample rate
	switch (sample_rate) {
	case 50:
	_default_filter_hz = 15;
	_sample_time_usec = 20000;
	break;
	case 100:
	_default_filter_hz = 30;
	_sample_time_usec = 10000;
	break;
	case 200:
	_default_filter_hz = 30;
	_sample_time_usec = 5000;
	break;
	case 400:
	default:
	_default_filter_hz = 30;
	_sample_time_usec = 2500;
	break;
	}
*/
	// used a fixed filter of 42Hz on the sensor, then filter using
	// the 2-pole software filter
	spi_write(MPUREG_CONFIG, BITS_DLPF_CFG_42HZ);

	// set sample rate to 1kHz, and use the 2 pole filter to give the
	// desired rate
	spi_write(MPUREG_SMPLRT_DIV, MPUREG_SMPLRT_1000HZ);
	spi_write(MPUREG_GYRO_CONFIG, BITS_GYRO_FS_2000DPS); // Gyro scale 2000º/s

	// RM-MPU-9250A-00.pdf, pg. 15, select accel full scale 8g
	spi_write(MPUREG_ACCEL_CONFIG,2<<3);

	// configure interrupt to fire when new data arrives
	spi_write(MPUREG_INT_ENABLE, BIT_RAW_RDY_EN);

	// clear interrupt on any read, and hold the data ready pin high
	// until we clear the interrupt
	spi_write(MPUREG_INT_PIN_CFG, BIT_INT_RD_CLEAR | BIT_LATCH_INT_EN);

}
#endif

uint8_t AK8963_whoami(){

    uint8_t response;

    instance = 0;

    spi_write(MPUREG_USER_CTRL, 0x20);
    spi_write(MPUREG_I2C_MST_CTRL, 0x0D);

    spi_write(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR); //Set the I2C slave addres of AK8963 and set for read.
    spi_write(MPUREG_I2C_SLV0_REG, AK8963_CNTL2); //I2C slave 0 register address from where to begin data transfer
    spi_write(MPUREG_I2C_SLV0_DO, 0x01);
    spi_write(MPUREG_I2C_SLV0_CTRL, 0x81); //Read 1 byte from the magnetometer
    spi_write(MPUREG_I2C_SLV0_CTRL, 0x00); //Read 1 byte from the magnetometer

    spi_write(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
    spi_write(MPUREG_I2C_SLV0_REG, AK8963_WIA); //I2C slave 0 register address from where to begin data transfer
    spi_write(MPUREG_I2C_SLV0_CTRL, 0x81); //Read 1 byte from the magnetometer

    //WriteReg(MPUREG_I2C_SLV0_CTRL, 0x81);    //Enable I2C and set bytes
    delay(1);
    spi_write(MPUREG_I2C_SLV0_CTRL, 0x00); //Read 1 byte from the magnetometer
    response=spi_read(MPUREG_EXT_SENS_DATA_00|READ_FLAG);    //Read I2C
    response = spi_read(MPUREG_I2C_MST_STATUS);
    //ReadRegs(MPUREG_EXT_SENS_DATA_00,response,1);
    //response=WriteReg(MPUREG_I2C_SLV0_DO, 0x00);    //Read I2C

    return response;
}
