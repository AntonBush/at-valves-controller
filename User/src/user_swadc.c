/*
 * user_swadc.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swadc.h"

uint8_t User_AdcPollData[USER__ADC_POLL_DATA_COUNT] = {0};
/*
static uint8_t User_AdcPollCommands[USER__ADC_POLL_COMMAND_COUNT] = {0};
static uint8_t User_AdcPollCommandIndex = 0;
static uint8_t User_SpiTransferMutexLocked = 0;
*/

/*
  if (!User_SpiTransferMutexLocked)
  {
    User_SpiTransferMutexLocked = !User_SpiTransferMutexLocked;

    HAL_SPI_Transmit_DMA(&hspi2, User_AdcPollCommands + User_AdcPollCommandIndex, 1);
    ++User_AdcPollCommandIndex;
  }
 */
void User_StartPollingAdc(SPI_HandleTypeDef *hspi)
{
  // @todo
}

/*
  if (User_AdcPollCommandIndex != USER__ADC_POLL_COMMAND_COUNT) return;

  User_AdcPollCommandIndex = 0;
 */
User_AdcPollingStatus_t User_ContinuePollingAdc(SPI_HandleTypeDef *hspi)
{
  // @todo
}

/*
  HAL_SPI_TransmitReceive_DMA(
      &hspi2
      , User_AdcPollData + User_AdcPollCommandIndex - 1
      , User_AdcPollCommands + User_AdcPollCommandIndex
      , 3
  );
  User_AdcPollCommandIndex += USER__ADC_POLL_DATA_LENGTH;
 */
void User_ReadAdcChannel(SPI_HandleTypeDef *hspi)
{
  // @todo
}
