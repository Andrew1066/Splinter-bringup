/*
 * pwm.h
 *
 *  Created on: Jan 19, 2015
 *      Author: alexf
 */

#ifndef PWM_H_
#define PWM_H_

void ftm_preinit();

int32_t ftm_start(uint8_t instance, uint32_t freq, uint8_t pct_on);





#endif /* PWM_H_ */
