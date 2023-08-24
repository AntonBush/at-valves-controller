/*
 * user_buffer.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_buffer.h"

uint16_t User_T3PwmBuffer[USER__T3_PWM_BUFFER_SIZE] = {0};
uint16_t User_T4PwmBuffer[USER__T4_PWM_BUFFER_SIZE] = {0};
uint16_t User_T5PwmBuffer[USER__T5_PWM_BUFFER_SIZE] = {0};

uint16_t User_CurrentBuffers[USER__CURRENT_BUFFER_SIZE] = {0};

uint8_t User_AdcPollData[USER__ADC_POLL_DATA_LENGTH] = {0};
uint8_t User_AdcPollCommands[USER__ADC_POLL_COMMAND_COUNT] = {0};
uint8_t User_AdcPollCommandIndex = 0;
