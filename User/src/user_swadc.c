/*
 * user_swadc.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swadc.h"
#include "user_swcurrent.h"
#include "main.h"

#include "ad7718.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_spi.h"

#include "stdbool.h"

// extern variables

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;

// public variables

uint8_t User_AdcPollData[USER__ADC_POLL_DATA_COUNT] = {0};

// private variables

static uint8_t User_AdcPollCommands[USER__ADC_POLL_COMMAND_COUNT] = {
  AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_0 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_1 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_2 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_3 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_4 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_5 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_6 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_7 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_8 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA

, AD7718__CR_ADDR__ADC_CONTROL
, AD7718__CH10__PSEUDO_DIFFERENTIAL_9 | AD7718__RN__2_560_V
, AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA
};
static uint8_t User_DumbAdcPollCommands[USER__ADC_POLL_DATA_LENGTH] = {0};
static uint8_t User_AdcFetchIndex = 0;

// private function prototypes

static void User_FetchAdcChannel(SPI_HandleTypeDef *hspi);

// extern functions

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != ADC_NRDY_Pin) return;

  User_ReadAdcChannel(&hspi2);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi != &hspi2) return;

  if (User_ContinuePollingAdc(hspi) != USER__ADC_POLLING_IS_NOT_PERFORMED) return;

  uint8_t *adc_poll_data = User_AdcPollData;
  for (int i = 0; i < USER__SW_COUNT; ++i)
  {
    uint32_t adc_data = 0;
    for (int j = 0; j < USER__ADC_POLL_DATA_LENGTH; ++j)
    {
      adc_data = (adc_data << 8) | (*adc_poll_data++);
    }
    User_SwCurrentBuffer[i] = User_CalculateSwCurrentFactor125EMin5(adc_data);
  }
}

// public functions

void User_StartPollingAdc(SPI_HandleTypeDef *hspi)
{
  if (User_AdcFetchIndex != 0) return;
  User_FetchAdcChannel(hspi);
}

User_AdcPollingStatus_t User_ContinuePollingAdc(SPI_HandleTypeDef *hspi)
{
  if (User_AdcFetchIndex == USER__SW_COUNT)
  {
    User_AdcFetchIndex = 0;
    return USER__ADC_POLLING_IS_NOT_PERFORMED;
  }

  User_FetchAdcChannel(hspi);
  return USER__ADC_POLLING_IN_PROCESS;
}

void User_ReadAdcChannel(SPI_HandleTypeDef *hspi)
{
  HAL_SPI_TransmitReceive_DMA(
    hspi
  , User_DumbAdcPollCommands
  , User_AdcPollData + User_AdcFetchIndex * USER__ADC_POLL_DATA_LENGTH
  , USER__ADC_POLL_DATA_LENGTH
  );
  ++User_AdcFetchIndex;
}

// private functions

static void User_FetchAdcChannel(SPI_HandleTypeDef *hspi)
{
  HAL_SPI_Transmit_DMA(
    hspi
  , User_AdcPollCommands + User_AdcFetchIndex * USER__ADC_POLL_FETCH_COMMAND_COUNT
  , USER__ADC_POLL_FETCH_COMMAND_COUNT
  );
}
