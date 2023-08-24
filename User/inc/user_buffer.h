/*
 * user_buffer.h
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#ifndef USER_INC_USER_BUFFER_H_
#define USER_INC_USER_BUFFER_H_


#include "stdint.h"


#define USER__T3_PWM_BUFFER_SIZE 2
#define USER__T4_PWM_BUFFER_SIZE 4
#define USER__T5_PWM_BUFFER_SIZE 4

#define USER__CURRENT_BUFFER_SIZE 10

#define USER__ADC_POLL_DATA_LENGTH (10 * 3)
#define USER__ADC_POLL_COMMAND_COUNT (USER__ADC_POLL_DATA_LENGTH + 1) // @attention @todo уточнить

extern uint16_t User_T3PwmBuffer[USER__T3_PWM_BUFFER_SIZE];
extern uint16_t User_T4PwmBuffer[USER__T4_PWM_BUFFER_SIZE];
extern uint16_t User_T5PwmBuffer[USER__T5_PWM_BUFFER_SIZE];

extern uint16_t User_CurrentBuffer[USER__CURRENT_BUFFER_SIZE];

extern uint8_t User_AdcPollData[USER__ADC_POLL_DATA_LENGTH];
extern uint8_t User_AdcPollCommands[USER__ADC_POLL_COMMAND_COUNT];
extern uint8_t User_AdcPollCommandIndex;

#endif /* USER_INC_USER_BUFFER_H_ */
