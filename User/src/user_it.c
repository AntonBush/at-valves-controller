/*
 * user_it.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_logic.h"
#include "user_swadc.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"

extern TIM_HandleTypeDef htim6;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim != &htim6) return;

  User_updateData();
  //User_CheckAdcPolling();
}
