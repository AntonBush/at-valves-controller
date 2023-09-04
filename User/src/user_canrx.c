/*
 * user_canrx.c
 *
 *  Created on: 30 авг. 2023 г.
 *      Author: bushev
 */

#include "user_canrx.h"

#include "user_sw.h"

#include "user_bytevec.h"

typedef enum User_PwmCanId
{
  USER__PWM_CAN_ID__SW_1_2  = 0x113FFFFF
, USER__PWM_CAN_ID__SW_3_10 = 0x114FFFFF
} User_PwmCanId_t;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

uint16_t User_CanRxLifeCounter = 0;

void User_CanRx(User_CanRxMessage_t *message)
{
  if (message->header.IDE != CAN_ID_EXT) return;

  switch (message->header.ExtId)
  {
  case USER__PWM_CAN_ID__SW_1_2:
    // T5: 1, 2
    htim5.Instance->CCR1 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 0)
    );
    htim5.Instance->CCR2 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 1)
    );
    ++User_CanRxLifeCounter;
    break;
  case USER__PWM_CAN_ID__SW_3_10:
    // T5: 3, 4
    htim5.Instance->CCR3 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 0)
    );
    htim5.Instance->CCR4 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 1)
    );
    // T3: 1, 2
    htim3.Instance->CCR1 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 2)
    );
    htim3.Instance->CCR2 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 3)
    );
    // T4: 1-4
    htim4.Instance->CCR1 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 4)
    );
    htim4.Instance->CCR2 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 5)
    );
    htim4.Instance->CCR3 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 6)
    );
    htim4.Instance->CCR4 = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 7)
    );
    break;
  }
}
