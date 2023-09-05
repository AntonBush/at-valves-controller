/*
 * user_swadc_private.c
 *
 *  Created on: 5 сент. 2023 г.
 *      Author: bushev
 */

#include "user_swadc_private.h"

#include "ad7718.h"

Ad7718_Ad7718_AdcControlChannel10_t User_AdcChannelCommands[USER__SW_COUNT] = {
  AD7718__CH10__PSEUDO_DIFFERENTIAL_0 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_1 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_2 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_3 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_4 | AD7718__RN__2_560_V

, AD7718__CH10__PSEUDO_DIFFERENTIAL_5 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_6 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_7 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_8 | AD7718__RN__2_560_V
, AD7718__CH10__PSEUDO_DIFFERENTIAL_9 | AD7718__RN__2_560_V
};
