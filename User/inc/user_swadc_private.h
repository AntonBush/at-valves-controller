/*
 * user_swadc_private.h
 *
 *  Created on: 5 сент. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_SWADC_PRIVATE_H_
#define INC_USER_SWADC_PRIVATE_H_

#include "user_sw.h"

#include "ad7718.h"

typedef struct User_AdcChannelCommands
{
  const uint8_t choose_adc_channel_command;
  uint8_t adc_channel;
} User_AdcChannelCommands_t;

extern Ad7718_Ad7718_AdcControlChannel10_t User_AdcChannelCommands[USER__SW_COUNT];

#endif /* INC_USER_SWADC_PRIVATE_H_ */
