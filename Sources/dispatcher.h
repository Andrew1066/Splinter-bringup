/*
 * dispatcher.h
 *
 *  Created on: Jan 11, 2015
 *      Author: alexf
 */


#ifndef DISPATCHER_H_
#define DISPATCHER_H_

//#include "main.h"

#define MAX_I2C_REQUEST_DATA_LENGTH 56
#define MPL_STATE_CHUNK_SIZE 60

typedef enum _i2c_bus_status_t
{
	I2cStatusIdle = 0,
	I2cStatusBusySend = 1,
	I2cStatusBusyReceive = 2,
	I2cStatusReceiveRequested = 10,
	I2cStatusSendRequested = 11,
	I2cStatusError = 0xff,
} i2c_bus_status_t;

#define I2cIsBusy(stat) ((stat == I2cStatusBusySend) || (I2cStatusBusyReceive))

enum shaving_request_type_t
{
	ReqI2cWrite = 1,
	ReqI2cRead = 2,
	ReqGpioSet = 3,
	ReqGpioGet = 4,
	ReqGetStatus = 5,
	ReqGetVersion = 6,
	ReqSetPwm = 7,
	ReqSpiTransferSync = 8,
	ReqInitGyro = 9,
	ReqCalData = 10,
	ReqCalDataGetSize = 11,
	ReqCalDataSetChunk = 12,
	ReqBootloader = 0x42,
	ReqUniqueId = 0x43,
	ReqTest = 0x7f,
} __attribute__ ((__packed__));

struct shaving_request_get_status {
	i2c_bus_status_t status[2];
}__attribute__ ((__packed__));

struct shaving_request_i2c_write
{
	uint8_t	bus;
	uint8_t device; //7 bit
	uint8_t reg;
	uint8_t len;
	uint8_t data[56];
}__attribute__ ((__packed__));

struct shaving_request_i2c_read
{
	uint8_t	bus;
	uint8_t device; //7 bit
	uint8_t reg;
	uint8_t len;
	uint8_t data[56];
}__attribute__ ((__packed__));

struct shaving_request_get_version
{
	uint8_t major;
	uint8_t minor;
	uint16_t revision;
}__attribute__ ((__packed__));

struct shaving_request_set_pwm
{
	uint8_t bus;
	uint8_t pct_on;
	uint16_t freq;
}__attribute__ ((__packed__));

struct shaving_request_set_cal_data
{
	uint8_t data[64];
}__attribute__ ((__packed__));

struct shaving_request_set_cal_data_chunk
{
	int	chunk;
}__attribute__ ((__packed__));

struct shaving_request_get_unique_id
{
	uint32_t uid[4];
}__attribute__ ((__packed__));

struct shaving_request_spi_transfer_sync
{
	uint8_t	bus;
	uint8_t reg;
	uint8_t len;
	uint8_t data[56];
}__attribute__ ((__packed__));

struct shaving_request_header_t {
	enum shaving_request_type_t reqtype;
	union {
		struct shaving_request_get_status 				get_status;
		struct shaving_request_i2c_write 				i2c_write;
		struct shaving_request_i2c_read 				i2c_read;
		struct shaving_request_get_version 				get_version;
		struct shaving_request_get_unique_id 			get_unique_id;
		struct shaving_request_set_pwm 					set_pwm;
		struct shaving_request_spi_transfer_sync 		spi_transfer;
		struct shaving_request_set_cal_data 			set_cal_data;
		struct shaving_request_set_cal_data_chunk 		set_cal_data_chunk;
	}__attribute__ ((__packed__));
}__attribute__ ((__packed__));

typedef struct i2c_request_state
{
	i2c_bus_status_t status;
	struct shaving_request_i2c_read  read_request;
	struct shaving_request_i2c_write write_request;
} i2c_request_state_t;

extern i2c_request_state_t g_pending_i2c_requests[2];

struct shaving_request_spi_transfer_sync g_spi_req;

usb_status SHV_SetReport(struct shaving_request_header_t* buf, uint32_t size);
usb_status SHV_GetReport(uint8_t request, struct shaving_request_header_t** hdr, uint32_t* size);
int SHV_IMU_SendData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t*data);
int SHV_IMU_ReadData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data);
int SHV_PowerUpDevices();
void SHV_InitOptics();



#endif /* DISPATCHER_H_ */
