/*
 * user_swadc.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_SWADC_H_
#define INC_USER_SWADC_H_

#include "user_sw.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_gpio.h"

#include "stdbool.h"

#define USER__ADC_POLL_DATA_LENGTH 3U

typedef enum User_Adc
{
  USER__ADC_1
, USER__ADC_2
, USER__ADC_COUNT
} User_Adc_t;

typedef struct User_AdcData
{
  uint8_t values[USER__SW_COUNT][USER__ADC_POLL_DATA_LENGTH];
} User_AdcData_t;

typedef struct User_AdcInfoPin
{
  GPIO_TypeDef * port;
  uint16_t pin;
} User_AdcInfoPin_t;

typedef struct User_AdcInfo
{
  User_AdcInfoPin_t nss;
  IRQn_Type interrupt;
  User_AdcInfoPin_t nrdy;
} User_AdcInfo_t;

extern User_AdcInfo_t User_AdcInfo[USER__ADC_COUNT];

void User_ChooseAdc(User_Adc_t adc);

void User_StartPollingAdc(void);
bool User_isAdcDataUpdated(void);
void User_ReadAdcData(User_AdcData_t *data);
void User_CheckAdcPolling(void);

#endif /* INC_USER_SWADC_H_ */
