/*
 * ad7718_def.h
 *
 *  Created on: Aug 23, 2023
 *      Author: bushev
 */

#ifndef AD7718_INC_AD7718_DEF_H_
#define AD7718_INC_AD7718_DEF_H_

#include "user_bit.h"

#define AD7718__ADC_DEFAULT_OFFSET 0x800000
#define AD7718__ADC_DEFAULT_GAIN   0x500005

typedef enum Ad7718_CommunicationControl
{
  AD7718__CR__IGNORE_WRITE = USER__BIT8__7
, AD7718__CR__READ         = USER__BIT8__6
} Ad7718_CommunicationControl_t;

typedef enum Ad7718_CommunicationAddress
{
  AD7718__CR_ADDR__THIS_ON_WRITE  = 0
, AD7718__CR_ADDR__STATUS_ON_READ = 0
, AD7718__CR_ADDR__MODE        = 1
, AD7718__CR_ADDR__ADC_CONTROL = 2
, AD7718__CR_ADDR__FILTER      = 3

, AD7718__CR_ADDR__ADC_DATA   = 4
, AD7718__CR_ADDR__ADC_OFFSET = 5
, AD7718__CR_ADDR__ADC_GAIN   = 6
, AD7718__CR_ADDR__IO_CONTROL = 7

, AD7718__CR_ADDR__TEST_1 = 12
, AD7718__CR_ADDR__TEST_2 = 13

, AD7718__CR_ADDR__ID = 15
} Ad7718_CommunicationAddress_t;

typedef enum Ad7718_Status
{
  AD7718__SR__READY                 = USER__BIT8__7
, AD7718__SR__CALIBRATION_COMPLETED = USER__BIT8__5
, AD7718__SR__ADC_ERROR             = USER__BIT8__3
, AD7718__SR__PLL_LOCKED            = USER__BIT8__0
} Ad7718_Status_t;

typedef enum Ad7718_ModeControl
{
  AD7718__MR__DISABLE_CHOPPING        = USER__BIT8__7
, AD7718__MR__NEGATIVE_INPUT_BUFFERED = USER__BIT8__6
, AD7718__MR__REFIN2                  = USER__BIT8__5
, AD7718__MR__10_CHANNELS             = USER__BIT8__4

, AD7718__MR__STOP_OSCILLATOR         = USER__BIT8__3
} Ad7718_ModeControl_t;

typedef enum Ad7718_Mode
{
  AD7718__MR_MOD__POWER_DOWN = 0
, AD7718__MR_MOD__IDLE_MODE  = 1
, AD7718__MR_MOD__SINGLE_CONVERSION_MODE = 2
, AD7718__MR_MOD__CONTINUOUS_CONVERSION  = 3

, AD7718__MR_MOD__INTERNAL_ZERO_SCALE_CALIBRATION = 4
, AD7718__MR_MOD__INTERNAL_FULL_SCALE_CALIBRATION = 5
, AD7718__MR_MOD__SYSTEM_ZERO_SCALE_CALIBRATION   = 6
, AD7718__MR_MOD__SYSTEM_FULL_SCALE_CALIBRATION   = 7
} Ad7718_Mode_t;

typedef enum Ad7718_AdcControl
{
  AD7718__ADCCON__UNIPOLAR = USER__BIT8__3
} Ad7718_AdcControl_t;

typedef enum Ad7718_AdcControlChannel8
{
  AD7718__CH8__PSEUDO_DIFFERENTIAL_0 = (0 << 4)
, AD7718__CH8__PSEUDO_DIFFERENTIAL_1 = (1 << 4)
, AD7718__CH8__PSEUDO_DIFFERENTIAL_2 = (2 << 4)
, AD7718__CH8__PSEUDO_DIFFERENTIAL_3 = (3 << 4)

, AD7718__CH8__PSEUDO_DIFFERENTIAL_4 = (4 << 4)
, AD7718__CH8__PSEUDO_DIFFERENTIAL_5 = (5 << 4)
, AD7718__CH8__PSEUDO_DIFFERENTIAL_6 = (6 << 4)
, AD7718__CH8__PSEUDO_DIFFERENTIAL_7 = (7 << 4)

, AD7718__CH8__FULLY_DIFFERENTIAL_0 = ( 8 << 4)
, AD7718__CH8__FULLY_DIFFERENTIAL_1 = ( 9 << 4)
, AD7718__CH8__FULLY_DIFFERENTIAL_2 = (10 << 4)
, AD7718__CH8__FULLY_DIFFERENTIAL_3 = (11 << 4)

, AD7718__CH8__AIN2_AIN2     = (12 << 4)
, AD7718__CH8__AINCOM_AINCOM = (13 << 4)
, AD7718__CH8__REFINP_REFINM = (14 << 4)
, AD7718__CH8__OPEN_OPEN     = (15 << 4)
} Ad7718_Ad7718_AdcControlChannel8_t;

typedef enum Ad7718_AdcControlChannel10
{
  AD7718__CH10__PSEUDO_DIFFERENTIAL_0 = (0 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_1 = (1 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_2 = (2 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_3 = (3 << 4)

, AD7718__CH10__PSEUDO_DIFFERENTIAL_4 = (4 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_5 = (5 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_6 = (6 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_7 = (7 << 4)

, AD7718__CH10__FULLY_DIFFERENTIAL_0 = ( 8 << 4)
, AD7718__CH10__FULLY_DIFFERENTIAL_1 = ( 9 << 4)
, AD7718__CH10__FULLY_DIFFERENTIAL_2 = (10 << 4)
, AD7718__CH10__FULLY_DIFFERENTIAL_3 = (11 << 4)

, AD7718__CH10__FULLY_DIFFERENTIAL_4 = (12 << 4)

, AD7718__CH10__AINCOM_AINCOM = (13 << 4)

, AD7718__CH10__PSEUDO_DIFFERENTIAL_8 = (14 << 4)
, AD7718__CH10__PSEUDO_DIFFERENTIAL_9 = (15 << 4)
} Ad7718_Ad7718_AdcControlChannel10_t;

typedef enum Ad7718_AdcControlRange
{
  AD7718__RN__0_020_V = 0
, AD7718__RN__0_040_V = 1
, AD7718__RN__0_080_V = 2
, AD7718__RN__0_160_V = 3

, AD7718__RN__0_320_V = 4
, AD7718__RN__0_640_V = 5
, AD7718__RN__1_280_V = 6
, AD7718__RN__2_560_V = 7
} Ad7718_AdcControlRange_t;

#endif /* AD7718_INC_AD7718_DEF_H_ */
