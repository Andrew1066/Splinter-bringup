#include "usb.h"
#include "fsl_ftm_driver.h"


void ftm_preinit()
{
    ftm_user_config_t ftmInfo;

    pin_mux_FTM(0);
	pin_mux_FTM(1);

    // Enable FTM module and generate PWM
    memset(&ftmInfo, 0, sizeof(ftmInfo));
    ftmInfo.syncMethod = kFtmUseSoftwareTrig;

    FTM_DRV_Init(0, &ftmInfo);

}

int32_t ftm_start(uint8_t instance, uint32_t freq, uint8_t pct_on)
{
    ftm_pwm_param_t ftmParam;

    ftmParam.mode = kFtmEdgeAlignedPWM;
    ftmParam.edgeMode = kFtmHighTrue;
    ftmParam.uFrequencyHZ = freq;
    ftmParam.uDutyCyclePercent = pct_on;
    ftmParam.uFirstEdgeDelayPercent = 0;

    FTM_DRV_PwmStart(0, &ftmParam, instance);

    // Issue a software trigger to update registers
    FTM_HAL_SetSoftwareTriggerCmd(FTM0_BASE, true);

    // Enable the generation of the FTM peripheral timer channel trigger
    FTM_HAL_SetChnTriggerCmd(FTM0_BASE, instance, true);

    return 0;
}
