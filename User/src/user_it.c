/*
 * user_it.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_buffer.h"

#include "main.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_spi.h"

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;

extern TIM_HandleTypeDef htim6;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != ADC_NRDY_Pin) return;

  HAL_SPI_TransmitReceive_DMA(
      &hspi2
      , User_AdcPollData + User_AdcPollCommandIndex - 1
      , User_AdcPollCommands + User_AdcPollCommandIndex
      , 1
  );
  ++User_AdcPollCommandIndex;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim != &htim6) return;

  // Begin transfer to CAN
  ; // @todo

  // Begin transfer to SPI
  HAL_SPI_Transmit_DMA(&hspi2, User_AdcPollCommands + User_AdcPollCommandIndex, 1);
  ++User_AdcPollCommandIndex;
}

void HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef * hspi)
{
  if (hspi != &hspi2) return;

  if (User_AdcPollCommandIndex != USER__ADC_POLL_COMMAND_COUNT) return;

  // Calculate currents and send to User_CurrentBuffer
  ; // @todo

  User_AdcPollCommandIndex = 0;
}
