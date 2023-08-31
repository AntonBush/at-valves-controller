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

#include "stdbool.h"

#define USER__ADC_POLL_DATA_LENGTH 3U

typedef struct User_AdcData
{
  uint8_t values[USER__SW_COUNT][USER__ADC_POLL_DATA_LENGTH];
} User_AdcData_t;

HAL_StatusTypeDef User_InitAdc(void);
void User_StartPollingAdc(void);
bool User_isAdcDataUpdated(void);
void User_ReadAdcData(User_AdcData_t *data);

#endif /* INC_USER_SWADC_H_ */
