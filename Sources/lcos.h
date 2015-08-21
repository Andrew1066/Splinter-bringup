#pragma once

#include "fsl_i2c_master_driver.h"


i2c_status_t init_lcos_ctrlr(uint8_t instance);
i2c_status_t init_lcos(uint8_t instance);
i2c_status_t init_lm3549(uint8_t instance);
