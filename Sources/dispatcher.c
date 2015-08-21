/*
 * dispatcher.c
 *
 *  Created on: Jan 11, 2015
 *      Author: alexf
 */

#include "usb.h"
#include "usb_device_stack_interface.h"
//#include "usb_class_hid.h"
//#include "usb_descriptor.h"
#include "dispatcher.h"
#include <assert.h>
#include <string.h>
#include "fsl_i2c_master_driver.h"
#include "fsl_dspi_master_driver.h"
#include "fsl_dspi_shared_function.h"
//#include "pincontrol.h"
//#include "main.h"
//#include "Hardware/lcos.h"
//#include "Hardware/pwm.h"

extern app_global_variable_struct_t           		g_app;
extern i2c_master_state_t							g_i2c_state[2];
extern uint8_t g_i2c_buffer[2][64];
//extern int test;

uint8_t* mpl_state = NULL;
uint32_t mpl_state_size = 0;
int current_chunk = -1;

i2c_device_t device;
struct shaving_request_header_t g_status;

i2c_request_state_t g_pending_i2c_requests[2];
struct shaving_request_header_t g_read_request;

struct shaving_request_get_version g_version = { VERSION };

int process_i2c_write_request(struct shaving_request_i2c_write* req)
{
	uint8_t bus = req->bus;
	if (bus > 1 || req->len > MAX_I2C_REQUEST_DATA_LENGTH)
		return -1;
	if ( (g_pending_i2c_requests[bus].status == I2cStatusError)) //busy
		g_pending_i2c_requests[bus].status = I2cStatusIdle;
	if ( (g_pending_i2c_requests[bus].status != I2cStatusIdle)) //busy
		return -1;
	g_pending_i2c_requests[bus].write_request = *req;
	g_pending_i2c_requests[bus].status = I2cStatusSendRequested;
	return 0;
}

int process_i2c_read_request(struct shaving_request_i2c_read* req)
{
	uint8_t bus = req->bus;
	if (bus > 1 || req->len > MAX_I2C_REQUEST_DATA_LENGTH)
		return -1;
	if ( (g_pending_i2c_requests[bus].status == I2cStatusError)) //busy
		g_pending_i2c_requests[bus].status = I2cStatusIdle;
	if ( (g_pending_i2c_requests[bus].status != I2cStatusIdle)) //busy
		return -1;
	g_pending_i2c_requests[bus].read_request = *req;
	g_pending_i2c_requests[bus].status = I2cStatusReceiveRequested;

	return 0;
}

int process_i2c_status_request(struct shaving_request_header_t** req)
{
	g_status.get_status.status[0] = g_pending_i2c_requests[0].status;
	g_status.get_status.status[1] = g_pending_i2c_requests[1].status;
	*req = &g_status;
	return 0;
}

void process_bootloader_request()
{
	// The reason this works is because the bootloader has
	// been modified to check the reboot flag on start. If
	// the flag indicate software reset, bootloader will
	// not launch the user app, but instead update firmware
	NVIC_SystemReset();
}

void process_pwm_request(struct shaving_request_set_pwm* req)
{
	_MLPrintLog(0, "", "ftm: %d, %d\n", req->freq, req->pct_on);
	//ftm_start(req->bus, req->freq, req->pct_on);
}

void process_spi_transfer(struct shaving_request_spi_transfer_sync* req)
{
	char buffer[56];
	if ( req->reg & 0x80 ) {
		SHV_IMU_ReadData(req->bus, req->reg, req->len, req->data);
		memcpy(&g_spi_req, req, sizeof(struct shaving_request_spi_transfer_sync));
	}
	else
		SHV_IMU_SendData(req->bus, req->reg, req->len, req->data);
}

void process_cal_request(struct shaving_request_set_cal_data* req, bool set)
{
	if ( mpl_state == NULL ) // need to allocate
	{
		inv_get_mpl_state_size(&mpl_state_size);
		mpl_state = OSA_MemAlloc(mpl_state_size);
	}
	if ( !mpl_state ) {
		_MLPrintLog(0, "", "Unable to allocate memory for the mpl state\n");
		return;
	}
	if ( set ) {
		// Sanity check
		if ( mpl_state_size / MPL_STATE_CHUNK_SIZE < current_chunk ) {
			_MLPrintLog(0, "", "Invalid chunk number %d\n", current_chunk);
			return;
		}
		int last_chunk = mpl_state_size / MPL_STATE_CHUNK_SIZE == current_chunk;
		int copy_size = last_chunk? (mpl_state_size - MPL_STATE_CHUNK_SIZE * current_chunk): MPL_STATE_CHUNK_SIZE;
		memcpy(mpl_state + (MPL_STATE_CHUNK_SIZE * current_chunk), req->data, copy_size);
		if ( last_chunk ) {
			_MLPrintLog(0, "", "Uploaded last chunk. Applying\n");
			int res = inv_load_mpl_states(mpl_state, mpl_state_size);
			_MLPrintLog(0, "", "Load state returned %d\n", res);
			inv_accel_was_turned_off();
			inv_gyro_was_turned_off();
			inv_compass_was_turned_off();
		}
	} else {
		// Sanity check
		if ( mpl_state_size / MPL_STATE_CHUNK_SIZE < current_chunk ) {
			_MLPrintLog(0, "", "Invalid chunk number %d\n", current_chunk);
			return;
		}
		if ( current_chunk == 0 ) {
			_MLPrintLog(0, "", "Retrieving state\n");
			inv_save_mpl_states(mpl_state, mpl_state_size);
		}
		int last_chunk = mpl_state_size / MPL_STATE_CHUNK_SIZE == current_chunk;
		int copy_size = last_chunk? (mpl_state_size - MPL_STATE_CHUNK_SIZE * current_chunk): MPL_STATE_CHUNK_SIZE;
		_MLPrintLog(0, "", "Providing chunk %d size %d\n", current_chunk, copy_size);
		memcpy(req->data, mpl_state + (MPL_STATE_CHUNK_SIZE * current_chunk), copy_size);
	}
}

volatile unsigned char rx_new;

static void test(struct shaving_request_header_t* hdr)
{
	rx_new = ((uint8_t*)hdr)[1];
}

usb_status SHV_SetReport(struct shaving_request_header_t* hdr, uint32_t size)
{
	int err;
	int sz = sizeof(enum shaving_request_type_t);
	switch(hdr->reqtype)
	{
		case ReqGetStatus:
			g_status.get_status.status[0] = g_pending_i2c_requests[0].status;
			g_status.get_status.status[1] = g_pending_i2c_requests[1].status;
			//USB_Class_HID_Send_Data(g_app.app_handle, 1, (uint8_t*)&g_status, sizeof(g_status));
			break;
		case ReqI2cWrite:
			if ( (err = process_i2c_write_request(&hdr->i2c_write)))
				return USBERR_BAD_STATUS;
			break;
		case ReqI2cRead:
			if ( (err = process_i2c_read_request(&hdr->i2c_read)))
				return USBERR_BAD_STATUS;
			break;
		case ReqGpioSet:
			break;
		case ReqGpioGet:
			break;
		case ReqBootloader:
			process_bootloader_request();
			break;
		case ReqSetPwm:
			process_pwm_request(&hdr->set_pwm);
			break;
		case ReqSpiTransferSync:
			process_spi_transfer(&hdr->spi_transfer);
			break;
		case ReqCalDataSetChunk:
			current_chunk = hdr->set_cal_data_chunk.chunk;
			break;
		case ReqCalData:
			process_cal_request(&hdr->set_cal_data, 1);
			break;
		case ReqTest:
			test(hdr);
			break;
		case ReqInitGyro:
			force_imu_self_test();
			break;
		default:
			break;
	}
	return USB_OK;
}

void SpiTest();
int optics_initialized = 1;

usb_status SHV_GetReport(uint8_t request, struct shaving_request_header_t** hdr, uint32_t* size)
{
	uint8_t bus = request >> 7;
	*size = REPORT_BUFF_SIZE;
	//process_i2c_status_request(hdr);
	switch(request & 0x7f)
	{
		case ReqI2cRead:
			if(g_status.get_status.status[bus] != I2cStatusBusyReceive) {
				*hdr = &g_read_request;
				g_read_request.i2c_read	= g_pending_i2c_requests[bus].read_request;
				g_read_request.reqtype = ReqI2cRead;
				*size = 64;
			}
			break;
		case ReqSpiTransferSync:
			*hdr = &g_read_request;
			(*hdr)->spi_transfer = g_spi_req;
			*size = 64;
			break;
		case ReqGetStatus:
			process_i2c_status_request(hdr);
			break;
		case ReqGetVersion:
			*hdr = &g_read_request;
			(*hdr)->reqtype = ReqGetVersion;
			(*hdr)->get_version = g_version;
			break;
		case ReqUniqueId:
			*hdr = &g_read_request;
			(*hdr)->reqtype = ReqUniqueId;
			(*hdr)->get_unique_id.uid[0] = HW_SIM_UIDH(SIM_BASE).U;
			(*hdr)->get_unique_id.uid[1] = HW_SIM_UIDMH(SIM_BASE).U;
			(*hdr)->get_unique_id.uid[2] = HW_SIM_UIDML(SIM_BASE).U;
			(*hdr)->get_unique_id.uid[3] = HW_SIM_UIDL(SIM_BASE).U;
			break;
		case ReqCalData:
			*hdr = &g_read_request;
			(*hdr)->reqtype = ReqCalData;
			process_cal_request(&(*hdr)->set_cal_data, 0);
			*size = 64;
			break;
		case ReqCalDataGetSize:
			*hdr = &g_read_request;
			inv_get_mpl_state_size(&mpl_state_size);
			(*hdr)->set_cal_data_chunk.chunk = mpl_state_size;
			*size = 64;
			break;
		case ReqTest:
			//SpiTest();
			break;
		default:
			break;
	}

	return USB_OK;
}


int SHV_PowerUpDevices()
{
	InitPins();
	EnableDeserializer(0);
	EnableDeserializer(1);
	EnableMPU(0);
	EnableMPU(1);
	EnableLEPS(0);
	EnableLEPS(1);
	optics_initialized = 0;

	return 0;
}

void WDOG_IRQ_HANDLER()
{

}

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];

    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}
void myHardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
}

static phase = 7;
void SHV_InitOptics()
{
	if (!optics_initialized)
	{
		phase--;
		i2c_status_t status = 0;

		if ( phase == 6 ) {
			status = init_lm3549(0);
			status = init_lm3549(0);
			return;
		}
		if ( phase == 5 ) {
			status = init_lm3549(1);
			status = init_lm3549(1);
			return;
		}
		if ( phase == 4 ) {
			status = init_lcos_ctrlr(0);
			return;
		}

		if ( phase == 3 ) {
			status = init_lcos_ctrlr(1);
			return;
		}
		if ( phase == 2 ) {
			status = init_lcos(0);
			return;
		}
		if ( phase == 1 ) {
			status = init_lcos(1);
			return;
		}

		optics_initialized = 1;
	}
}

#define DSPI_BUFFER_SIZE 16
static uint8_t dspi_buffer_out[DSPI_BUFFER_SIZE];
static uint8_t dspi_buffer_in[DSPI_BUFFER_SIZE];
#define min(a,b) ((a) < (b)? a: b)

void swo_puts(char* s);
char buffer[40];

void log_imu_io(char read, uint8_t reg, uint8_t len, uint8_t*data, dspi_status_t status)
{
}

#define DUMP_REG 0

int SHV_IMU_SendData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t*data)
{
	dspi_status_t status;
	int i;
	if ( addr > 1 )
		return -1;
#if DUMP_REG
	sprintf(buffer, "%12d imu(%d)> %02x [", OSA_TimeGetMsec(), addr, reg, len);
	swo_puts(buffer);
	for(i = 0; i < len; i++){
		sprintf(buffer, "%02x ", data[i]);
		swo_puts(buffer);
	}
#endif
	while(len)
	{
		uint8_t chunk = min(DSPI_BUFFER_SIZE - 1, len);
		dspi_buffer_out[0] = reg;
		memcpy(dspi_buffer_out + 1, data, chunk);
		GPIO_DRV_ClearPinOutput(addr == 0? kGpioSpi0Cs0: kGpioSpi1Cs0);
		status = DSPI_DRV_MasterTransferBlocking(addr, &g_dspiDevice[addr], dspi_buffer_out, dspi_buffer_in, chunk + 1, 10);
		GPIO_DRV_SetPinOutput(addr == 0? kGpioSpi0Cs0: kGpioSpi1Cs0);
		if ( status ) {
			swo_puts("SPI transfer failed\n");
			break;
		}
		//memcpy(data, dspi_buffer_in + 1, chunk);
		data += chunk;
		len -= chunk;
		reg += chunk;
	}
#if DUMP_REG
	sprintf(buffer, "] %d\n", status);
	swo_puts(buffer);
#endif
	return status;
}

int SHV_IMU_ReadData(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data)
{
	dspi_status_t status;
	int i = len;
	uint8_t *p = data;
	uint32_t speed = 1000000;
	if ( addr > 1 )
		return -1;
#if DUMP_REG
	sprintf(buffer, "%12d imu(%d)< %02x (%d) [", OSA_TimeGetMsec(), addr, reg, len);
	swo_puts(buffer);
#endif
#if 0
	if ( reg >= 114 && reg <= 116 ) {
		speed = g_dspiDevice[i].bitsPerSec;
		g_dspiDevice[i].bitsPerSec = 1000000;
	}
#endif
	while(len)
	{
		uint8_t chunk = min(DSPI_BUFFER_SIZE - 1, len);
		dspi_buffer_out[0] = reg | 0x80;
		GPIO_DRV_ClearPinOutput(addr == 0? kGpioSpi0Cs0: kGpioSpi1Cs0);
		status = DSPI_DRV_MasterTransferBlocking(addr, &g_dspiDevice[addr], dspi_buffer_out, dspi_buffer_in, chunk + 1, 10);
		GPIO_DRV_SetPinOutput(addr == 0? kGpioSpi0Cs0: kGpioSpi1Cs0);
		if ( status ) {
			swo_puts("SPI transfer failed\n");
			break;
		}
		memcpy(data, dspi_buffer_in + 1, chunk);
		data += chunk;
		len -= chunk;
		reg += chunk;
	}
	len = i;
#if DUMP_REG
	for(i = 0; i < len; i++){
		sprintf(buffer, "%02x ", p[i]);
		swo_puts(buffer);
	}
	sprintf(buffer, "] %d\n", status);
	swo_puts(buffer);
#endif
#if 0
	if ( reg >= 114 && reg <= 116 ) {
		g_dspiDevice[i].bitsPerSec = speed;
	}
#endif
	return status;
}
#undef Watchdog_IRQHandler

void Watchdog_IRQHandler()
{
}

void GetActive()
{
	volatile uint32_t* IABR = NVIC->IABR;
#if defined (DEBUG)
//    __ASM("BKPT #0");
#endif // DEBUG
}

void UART_Err_IRQHandler()
{
#if defined (DEBUG)
    __ASM("BKPT #0");
#endif // DEBUG
}

void SysTick_Handler()
{
#if defined (DEBUG)
    __ASM("BKPT #0");
#endif // DEBUG
}

void SWI_IRQHandler()
{
#if defined (DEBUG)
    __ASM("BKPT #0");
#endif // DEBUG
}

void SVC_Handler()
{
#if defined (DEBUG)
    __ASM("BKPT #0");
#endif // DEBUG
}

//! @brief Usage Fault Handler
void UsageFault_Handler(void)
{
#if defined (DEBUG)
    __ASM("BKPT #0");
#endif // DEBUG

    // Reset MCU
    NVIC_SystemReset();
}

void __attribute__((__interrupt__)) USB0_IRQHandler(void)
{
	_usb_khci_isr();

#if defined (DEBUG)
    //__ASM("BKPT #0");
#endif // DEBUG
}
