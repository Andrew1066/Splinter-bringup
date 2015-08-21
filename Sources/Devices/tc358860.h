/*
 * tc358860.h
 *
 *  Created on: May 20, 2015
 *      Author: alexf
 */

#ifndef TC358860_H_
#define TC358860_H_

uint8_t tc358860_get_dp_link_status(byte bus);
int tc358860_enable(uint8_t bus);
void tc358860_after_training(byte bus);

#endif /* TC358860_H_ */
