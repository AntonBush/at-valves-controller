/*
 * user_swadc.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swadc.h"

#include "main.h"

#include "ad7718.h"
#include "stm32f1xx_hal_gpio.h"

#include "stdbool.h"

// private defines

#define USER__ADC_MODE(mode) \
( \
  AD7718__MR__DISABLE_CHOPPING \
| AD7718__MR__NEGATIVE_INPUT_BUFFERED \
| AD7718__MR__10_CHANNELS \
| (mode) \
)
#define USER__ADC_INIT_TIMEOUT 5000U

// private typedefs

typedef struct User_AdcChannelCommands
{
  const uint8_t choose_adc_channel_command;
  uint8_t adc_channel;
} User_AdcChannelCommands_t;

typedef struct User_AdcPullCommand {
  uint8_t read_command;
  uint8_t dumb[USER__ADC_POLL_DATA_LENGTH];
} User_AdcPullCommand_t;

typedef struct User_AdcPollValue
{
  uint8_t dumb;
  uint8_t data[USER__ADC_POLL_DATA_LENGTH];
} User_AdcPollValue_t;

typedef enum User_AdcPollingStatus
{
  USER__ADC_POLL_FINISHED
, USER__ADC_POLL_IN_PROCESS
} User_AdcPollStatus_t;

typedef struct User_AdcDataBuffer
{
  User_AdcData_t data;
  bool isUpdated;
} User_AdcDataBuffer_t;

typedef enum User_AdcDataBufferIndex
{
  USER__ADC_DATA_BUFFER__A
, USER__ADC_DATA_BUFFER__B
, USER__ADC_DATA_BUFFER__COUNT
, USER__ADC_DATA_NO_BUFFER
} User_AdcDataBufferIndex_t;

// extern variables

extern SPI_HandleTypeDef hspi2;

// private variables

static const Ad7718_Ad7718_AdcControlChannel10_t User_AdcChannelCommands[10] = {
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

uint8_t User_AdcPreInitCommands[] = {
  AD7718__CR_ADDR__FILTER
, 0x03 // 1365.33 Hz
};
uint8_t User_AdcPostInitCommands[] = {
  AD7718__CR_ADDR__MODE
, USER__ADC_MODE(AD7718__MR_MOD__CONTINUOUS_CONVERSION)
};
User_AdcChannelCommands_t User_AdcPreCalibrationCommands = {
  .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
, .adc_channel = User_AdcChannelCommands[0]
};
uint8_t User_AdcZeroScaleCalibrationCommands[] = {
  AD7718__CR_ADDR__MODE
, USER__ADC_MODE(AD7718__MR_MOD__INTERNAL_ZERO_SCALE_CALIBRATION)
};
uint8_t User_AdcFullScaleCalibrationCommands[] = {
  AD7718__CR_ADDR__MODE
, USER__ADC_MODE(AD7718__MR_MOD__INTERNAL_FULL_SCALE_CALIBRATION)
};

static User_AdcChannelCommands_t User_AdcFetchCommand = {
  .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
, .adc_channel = User_AdcChannelCommands[0]
};
static const User_AdcPullCommand_t User_AdcPullCommand = {
  .read_command = AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA
, .dumb = {0}
};
static User_AdcPollValue_t User_AdcPollValues[USER__SW_COUNT];
static uint8_t User_AdcPollIndex = 0;

static User_AdcDataBuffer_t User_AdcDataBuffers[USER__ADC_DATA_BUFFER__COUNT] = {
  {
    .isUpdated = false
  }
, {
    .isUpdated = false
  }
};
static User_AdcDataBufferIndex_t User_ActiveAdcDataBuffer = USER__ADC_DATA_BUFFER__A;
static User_AdcDataBufferIndex_t User_LockedAdcDataBuffer = USER__ADC_DATA_NO_BUFFER;

// private function prototypes

static HAL_StatusTypeDef User_CalibrateAdc(uint8_t *commands, uint16_t command_count);
static User_AdcPollStatus_t User_ContinuePollAdc(SPI_HandleTypeDef *hspi);
static void User_FetchAdcChannel(void);
static void User_ReadAdcChannel(void);

// extern functions

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != ADC_NRDY_Pin) return;

  User_ReadAdcChannel();
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi != &hspi2) return;
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi != &hspi2) return;

  if (User_ContinuePollAdc(hspi) != USER__ADC_POLL_FINISHED) return;

  if (User_ActiveAdcDataBuffer == User_LockedAdcDataBuffer)
  {
    User_ActiveAdcDataBuffer
      = User_ActiveAdcDataBuffer == USER__ADC_DATA_BUFFER__A
      ? USER__ADC_DATA_BUFFER__B
      : USER__ADC_DATA_BUFFER__A;
  }

  for (uint8_t i = 0; i < USER__SW_COUNT; ++i)
  {
    for (uint8_t j = 0; j < USER__ADC_POLL_DATA_LENGTH; ++j)
    {
      User_AdcDataBuffers[User_ActiveAdcDataBuffer].data.values[i][j] = (
        User_AdcPollValues[i].data[j]
      );
    }
  }

  User_AdcDataBuffers[User_ActiveAdcDataBuffer].isUpdated = true;
}

// public functions

HAL_StatusTypeDef User_InitAdc(void)
{
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);

  HAL_StatusTypeDef status = HAL_SPI_Transmit(
    &hspi2
  , User_AdcPreInitCommands
  , sizeof(User_AdcPreInitCommands)
  , USER__ADC_INIT_TIMEOUT
  );
  if (status != HAL_OK)
  {
    return status;
  }

  for (int i = 0; i < USER__SW_COUNT; ++i)
  {
    User_AdcPreCalibrationCommands.adc_channel = User_AdcChannelCommands[i];
    status = User_CalibrateAdc(
      (uint8_t *) &User_AdcPreCalibrationCommands
    , sizeof(User_AdcPreCalibrationCommands)
    );
    if (status != HAL_OK)
    {
      return status;
    }

    status = User_CalibrateAdc(
      User_AdcZeroScaleCalibrationCommands
    , sizeof(User_AdcZeroScaleCalibrationCommands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
    while (!HAL_GPIO_ReadPin(ADC_NRDY_GPIO_Port, ADC_NRDY_Pin))
    {}

    status = User_CalibrateAdc(
      User_AdcFullScaleCalibrationCommands
    , sizeof(User_AdcFullScaleCalibrationCommands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
    while (!HAL_GPIO_ReadPin(ADC_NRDY_GPIO_Port, ADC_NRDY_Pin))
    {}
  }

  status = HAL_SPI_Transmit(
    &hspi2
  , User_AdcPostInitCommands
  , sizeof(User_AdcPostInitCommands)
  , USER__ADC_INIT_TIMEOUT
  );

  return status;
}

void User_StartPollingAdc(void)
{
  if (User_AdcPollIndex != 0) return;
  User_FetchAdcChannel();
}

bool User_isAdcDataUpdated(void)
{
  return User_AdcDataBuffers[User_ActiveAdcDataBuffer].isUpdated;
}

void User_ReadAdcData(User_AdcData_t *data)
{
  User_LockedAdcDataBuffer = User_ActiveAdcDataBuffer;
  (*data) = User_AdcDataBuffers[User_LockedAdcDataBuffer].data;
  User_AdcDataBuffers[User_LockedAdcDataBuffer].isUpdated = false;
  User_LockedAdcDataBuffer = USER__ADC_DATA_NO_BUFFER;
}

// private functions

static HAL_StatusTypeDef User_CalibrateAdc(uint8_t *commands, uint16_t command_count)
{
  return HAL_SPI_Transmit(
    &hspi2
  , commands
  , command_count
  , USER__ADC_INIT_TIMEOUT
  );
}

static User_AdcPollStatus_t User_ContinuePollAdc(SPI_HandleTypeDef *hspi)
{
  if (User_AdcPollIndex == USER__SW_COUNT)
  {
    User_AdcPollIndex = 0;
    return USER__ADC_POLL_FINISHED;
  }

  User_FetchAdcChannel();
  return USER__ADC_POLL_IN_PROCESS;
}

static void User_FetchAdcChannel(void)
{
  User_AdcFetchCommand.adc_channel = (
      User_AdcChannelCommands[User_AdcPollIndex] | AD7718__RN__2_560_V
  );

  HAL_SPI_Transmit_DMA(
    &hspi2
  , (uint8_t *)(&User_AdcFetchCommand)
  , sizeof(User_AdcFetchCommand)
  );
}

static void User_ReadAdcChannel(void)
{
  HAL_SPI_TransmitReceive_DMA(
    &hspi2
  , (uint8_t *)(&User_AdcPullCommand)
  , (uint8_t *)(User_AdcPollValues + User_AdcPollIndex)
  , sizeof(User_AdcPullCommand)
  );
  ++User_AdcPollIndex;
}
