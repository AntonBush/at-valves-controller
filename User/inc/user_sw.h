/*
 * user_sw.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_SW_H_
#define INC_USER_SW_H_

#include "main.h"

#include "stdint.h"

#ifdef USER__BOARD_1
#define USER__SW_COUNT 4U
#else
#define USER__SW_COUNT 5U
#endif

/*
#define USER__T3_PWM_BUFFER_SIZE 2U
#define USER__T4_PWM_BUFFER_SIZE 4U
#define USER__T5_PWM_BUFFER_SIZE 4U

extern uint16_t User_T3PwmBuffer[USER__T3_PWM_BUFFER_SIZE];
extern uint16_t User_T4PwmBuffer[USER__T4_PWM_BUFFER_SIZE];
extern uint16_t User_T5PwmBuffer[USER__T5_PWM_BUFFER_SIZE];
*/

#endif /* INC_USER_SW_H_ */
