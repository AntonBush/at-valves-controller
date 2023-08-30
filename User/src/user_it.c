/*
 * user_it.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_swcurrent.h"
#include "user_swadc.h"
#include "user_cantx.h"

extern TIM_HandleTypeDef htim6;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim != &htim6) return;

  // @todo make init before uncomment
  //User_StartPollingAdc();

  User_CanTx();
}
