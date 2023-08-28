/*
 * user_it.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_swcurrent.h"
#include "user_swadc.h"
#include "user_can.h"

extern CAN_HandleTypeDef hcan1;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim6;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim != &htim6) return;

  // @todo -> User_AddCanMessage
  User_TransmitCanMessage(&hcan1);
  User_StartPollingAdc(&hspi2);
}
