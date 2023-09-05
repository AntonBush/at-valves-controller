/*
 * user_swadc.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swadc.h"

#include "user_swadc_private.h"

#include "main.h"

#include "stdbool.h"

// private typedefs

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

static User_AdcChannelCommands_t User_AdcFetchCommand = {
  .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
};
static const User_AdcPullCommand_t User_AdcPullCommand = {
  .read_command = AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA
, .dumb = {0}
};
static User_AdcPollValue_t User_AdcPollValues[USER__SW_COUNT];
static uint8_t User_AdcPollIndex = 0;
static User_AdcPollStatus_t User_AdcPollStatus = USER__ADC_POLL_FINISHED;

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

static User_AdcPollStatus_t User_ContinuePollAdc(SPI_HandleTypeDef *hspi);
static void User_FetchAdcChannel(void);
static void User_ReadAdcChannel(void);

// extern functions

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != ADC_NRDY_Pin) return;
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);

  User_ReadAdcChannel();
/*
  typedef struct
  {
    uint8_t command;
    uint8_t dummy;
  } tx_t;

  typedef struct
  {
    uint8_t dummy;
    uint8_t rx;
  } rx_t;

  tx_t write_commands[] = {
    {
      .command = AD7718__CR__READ | AD7718__CR_ADDR__STATUS_ON_READ
    , .dummy = 0xFF
    }
  , {
      .command = AD7718__CR__READ | AD7718__CR_ADDR__MODE
    , .dummy = 0xFE
    }
  , {
      .command = AD7718__CR__READ | AD7718__CR_ADDR__ADC_CONTROL
    , .dummy = 0xFD
    }
  , {
      .command = AD7718__CR__READ | AD7718__CR_ADDR__FILTER
    , .dummy = 0xFC
    }
  };
  typedef volatile uint8_t (* volatile u8p)[sizeof(write_commands)];
  rx_t read_status[sizeof(write_commands) / sizeof(tx_t)];
  u8p tx = (u8p)(write_commands);
  u8p rx = (u8p)(read_status);

  for (int i = 0; i < sizeof(write_commands) / sizeof(tx_t); ++i)
  {
    read_status[i].dummy = read_status[i].rx = 0;
  }


  SET_BIT(SPI2->CR1, SPI_CR1_SPE);

  SPI2->DR = (*tx)[0];
  size_t i = 1;
  for (; i < sizeof(write_commands); ++i)
  {
    while (READ_BIT(SPI2->SR, SPI_SR_TXE) == 0)
    {}
    SPI2->DR = (*tx)[i];
    while (READ_BIT(SPI2->SR, SPI_SR_RXNE) == 0)
    {}
    (*rx)[i - 1] = SPI2->DR;
  }
  while (READ_BIT(SPI2->SR, SPI_SR_RXNE) == 0)
  {}
  (*rx)[i - 1] = SPI2->DR;

  while (READ_BIT(SPI2->SR, SPI_SR_TXE) == 0)
  {}
  while (READ_BIT(SPI2->SR, SPI_SR_BSY))
  {}
  CLEAR_BIT(SPI2->CR1, SPI_CR1_SPE);

  for (int i = 0; i < sizeof(write_commands); ++i)
  {
    *((uint8_t *)User_AdcPollValues) = (*rx)[i];
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////
  HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
    &hspi2
  , (uint8_t *)(write_commands)
  , (uint8_t *)(read_status)
  , sizeof(write_commands)
  , 5000
  );
  if (status != HAL_OK)
  {
    while(1);
  }

  HAL_SPI_TransmitReceive(
    &hspi2
  , (uint8_t *)(&User_AdcPullCommand)
  , (uint8_t *)(User_AdcPollValues + User_AdcPollIndex)
  , sizeof(User_AdcPullCommand)
  , 5000
  );
  ++User_AdcPollIndex;

  if (User_AdcPollIndex == USER__SW_COUNT)
  {
    User_AdcPollIndex = 0;
  }

  User_AdcFetchCommand.adc_channel = (
      User_AdcChannelCommands[User_AdcPollIndex]
  );
  HAL_SPI_Transmit(
    &hspi2
  , (uint8_t *)(&User_AdcFetchCommand)
  , sizeof(User_AdcFetchCommand)
  , 5000
  );
*/
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
  User_AdcPollStatus = USER__ADC_POLL_FINISHED;
}

// public functions

void User_StartPollingAdc(void)
{
  if (User_AdcPollStatus == USER__ADC_POLL_IN_PROCESS) return;
  User_AdcPollStatus = USER__ADC_POLL_IN_PROCESS;
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
      User_AdcChannelCommands[User_AdcPollIndex]
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
