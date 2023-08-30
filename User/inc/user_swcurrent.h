/*
 * user_swcurrent.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_SWCURRENT_H_
#define INC_USER_SWCURRENT_H_

#include "user_sw.h"
#include "user_int.h"

extern uint16_t User_SwCurrentBuffer[USER__SW_COUNT];

uint16_t User_CalculateSwCurrentFactor125EMin5(User_UInt24_t adc_data);

#endif /* INC_USER_SWCURRENT_H_ */
