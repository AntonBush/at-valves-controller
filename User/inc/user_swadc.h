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

#define USER__ADC_POLL_DATA_LENGTH 3U
#define USER__ADC_POLL_DATA_COUNT \
(USER__ADC_POLL_DATA_LENGTH * USER__SW_COUNT)

#define USER__ADC_POLL_FETCH_COMMAND_COUNT 2U
#define USER__ADC_POLL_COMMAND_COUNT \
(USER__ADC_POLL_FETCH_COMMAND_COUNT * USER__SW_COUNT)

typedef enum User_AdcPollingStatus
{
  USER__ADC_POLLING_IS_NOT_PERFORMED
, USER__ADC_POLLING_IN_PROCESS
} User_AdcPollingStatus_t;

extern uint8_t User_AdcPollData[USER__ADC_POLL_DATA_COUNT];

void User_StartPollingAdc(SPI_HandleTypeDef *hspi);
User_AdcPollingStatus_t User_ContinuePollingAdc(SPI_HandleTypeDef *hspi);
void User_ReadAdcChannel(SPI_HandleTypeDef *hspi);

#endif /* INC_USER_SWADC_H_ */
