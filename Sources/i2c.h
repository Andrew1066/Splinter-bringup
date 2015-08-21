/*
 * i2c.h
 *
 *  Created on: May 19, 2015
 *      Author: alexf
 */

#ifndef I2C_H_
#define I2C_H_

#if USE_PRINTF
#define PRINTF printf
#else
#define PRINTF(...)
#endif

typedef struct _i2c_adapter_info {
	LDD_TDeviceData* data;
	volatile bool send_pending;
	volatile bool rcv_pending;
	LDD_TDeviceData* (*I2C_Init)(LDD_TUserData *UserDataPtr);
	LDD_TError (*I2C_MasterSendBlock)(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, LDD_I2C_TSize Size, LDD_I2C_TSendStop SendStop);
	LDD_TError (*I2C_MasterReceiveBlock)(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, LDD_I2C_TSize Size, LDD_I2C_TSendStop SendStop);
	bool (*I2C_MasterGetBlockSentStatus)(LDD_TDeviceData *DeviceDataPtr);
	bool (*I2C_MasterGetBlockReceivedStatus)(LDD_TDeviceData *DeviceDataPtr);
	LDD_I2C_TSize (*I2C_MasterGetSentDataNum)(LDD_TDeviceData *DeviceDataPtr);
	LDD_I2C_TSize (*I2C_MasterGetReceivedDataNum)(LDD_TDeviceData *DeviceDataPtr);
	LDD_TError (*I2C_SelectSlaveDevice)(LDD_TDeviceData *DeviceDataPtr, LDD_I2C_TAddrType AddrType, LDD_I2C_TAddr Addr);
} i2c_adapter_info_t;

#define DEFINE_I2C_ADAPTER(index) \
	.I2C_Init = CI2C##index##_Init, \
	.I2C_MasterSendBlock = CI2C##index##_MasterSendBlock, \
	.I2C_MasterReceiveBlock = CI2C##index##_MasterReceiveBlock, \
	.I2C_MasterGetBlockSentStatus = CI2C##index##_MasterGetBlockSentStatus, \
	.I2C_MasterGetBlockReceivedStatus = CI2C##index##_MasterGetBlockReceivedStatus, \
	.I2C_MasterGetSentDataNum = CI2C##index##_MasterGetSentDataNum, \
	.I2C_MasterGetReceivedDataNum = CI2C##index##_MasterGetReceivedDataNum, \
	.I2C_SelectSlaveDevice = CI2C##index##_SelectSlaveDevice,

#define SEND_IS_PENDING(bus) (adapters[bus].send_pending)
#define RCV_IS_PENDING(bus) (adapters[bus].rcv_pending)
#define SEND_SET_PENDING(bus) (adapters[bus].send_pending = 1)
#define SEND_CLR_PENDING(bus) (adapters[bus].send_pending = 0)
#define RCV_SET_PENDING(bus) (adapters[bus].rcv_pending = 1)
#define RCV_CLR_PENDING(bus) (adapters[bus].rcv_pending = 0)

void i2c_init_all();

LDD_TError i2c_master_receive_blocking(byte bus, byte addr, byte* data, byte len);
LDD_TError i2c_master_send_blocking(byte bus, byte addr, byte* data, byte len);
LDD_TError i2c_master_send_blocking_no_stop(byte bus, byte addr, byte* data, byte len);
LDD_TDeviceDataPtr GetI2cDevice(uint8_t bus);
extern bool slave_data_available;
extern bool slave_data_requested;
extern uint16_t slave_data_address;
extern uint8_t edid_data[];
extern uint8_t edid_data_from_eeprom[];
extern uint8_t edid_new[];
extern uint8_t edid_tosh[];
#endif /* I2C_H_ */
