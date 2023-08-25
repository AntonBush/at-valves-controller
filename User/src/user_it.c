/*
 * user_it.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_swcurrent.h"
#include "user_swadc.h"
#include "user_can.h"

#include "main.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_spi.h"

extern CAN_HandleTypeDef hcan1;

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;

extern TIM_HandleTypeDef htim6;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != ADC_NRDY_Pin) return;

  User_ReadAdcChannel(&hspi2);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi != &hspi2) return;

  if (User_ContinuePollingAdc(hspi) != USER__ADC_POLLING_IS_NOT_PERFORMED) return;

  // Calculate currents and send to User_CurrentBuffer
  ; // @todo
  for (
    int i = 0
    ; i < USER__ADC_POLL_DATA_COUNT
    ; i += USER__ADC_POLL_DATA_LENGTH
  )
  {
    //uint32_t ;
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan != &hcan1) return;
  User_ReceiveCanMessage(hcan);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim != &htim6) return;

  User_TransmitCanMessage(&hcan1);
  User_StartPollingAdc(&hspi2);
}
