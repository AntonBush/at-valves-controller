/*
 * user_sw.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_SW_H_
#define INC_USER_SW_H_

#include "stdint.h"

#define USER__SW_COUNT 10U

#define USER__T3_PWM_BUFFER_SIZE 2U
#define USER__T4_PWM_BUFFER_SIZE 4U
#define USER__T5_PWM_BUFFER_SIZE 4U

#define USER__PWM_MAX_VALUE 0xFFFFU

#define USER__PWM_VALUE_FROM_PERCENTAGES(percentages) \
((percentages) * USER__PWM_MAX_VALUE / 100)

extern uint16_t User_T3PwmBuffer[USER__T3_PWM_BUFFER_SIZE];
extern uint16_t User_T4PwmBuffer[USER__T4_PWM_BUFFER_SIZE];
extern uint16_t User_T5PwmBuffer[USER__T5_PWM_BUFFER_SIZE];

#endif /* INC_USER_SW_H_ */
