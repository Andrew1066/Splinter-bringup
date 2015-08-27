
/* Various defines and enums specific to the Splinter board
 *
 *
 */

#define on	1
#define off	0

#define MPU9250

// i2c allocations

#define TEMPLE_RIGHT_I2C	1
#define USB_HUB_I2C TEMPLE_RIGHT_I2C
#define TEMPLE_LEFT_I2C		0


// Port allocations
// Port A Outputs

#define EDP2DSI_I2C_ACCESS  (GPIOA_IDX << GPIO_PORT_SHIFT) | 0x08	//
#define USB_SWITCH_CTL		(GPIOA_IDX << GPIO_PORT_SHIFT) | 0x09   //


// Port B Outputs

#define USB_HUB_RST			(GPIOB_IDX << GPIO_PORT_SHIFT) | 0x03	//
#define TEMPLE_LEFT_LE_ENA	(GPIOB_IDX << GPIO_PORT_SHIFT) | 0x08	//
#define TEMPLE_RIGHT_LE_ENA	(GPIOB_IDX << GPIO_PORT_SHIFT) | 0x09	//
#define UTC_FLASH_PGM		(GPIOB_IDX << GPIO_PORT_SHIFT) | 0x10	//
#define CAM_PDWN			(GPIOB_IDX << GPIO_PORT_SHIFT) | 0x11	//
#define CAM_RS				(GPIOB_IDX << GPIO_PORT_SHIFT) | 0x12	//

// Port C Outputs

#define UTC_RST_M			(GPIOC_IDX << GPIO_PORT_SHIFT) | 0x00	//
#define EN_MAIN_PWR			(GPIOC_IDX << GPIO_PORT_SHIFT) | 0x02	//
#define MPU_SPI_CS			(GPIOC_IDX << GPIO_PORT_SHIFT) | 0x04	//

// Port D Outputs

#define UTC_RST_S			(GPIOD_IDX << GPIO_PORT_SHIFT) | 0x03	//
