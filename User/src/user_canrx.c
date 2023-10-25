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
#ifdef USER__BOARD_1
  USER__PWM_CAN_ID__SW_1_4 = 0x114
#else
  USER__PWM_CAN_ID__SW_5_8 = 0x158
, USER__PWM_CAN_ID__SW_9   = 0x109
#endif
} User_PwmCanId_t;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

uint16_t User_CanRxLifeCounter = 0;

void User_CanRx(User_CanRxMessage_t *message)
{
  if (message->header.IDE != CAN_ID_EXT) return;

  ++User_CanRxLifeCounter;

  TIM_HandleTypeDef *sw_1_4_htim = &htim5;
  TIM_HandleTypeDef *sw_5_htim = &htim3;
  switch (message->header.ExtId)
  {
#ifdef USER__BOARD_1
  case USER__PWM_CAN_ID__SW_1_4:
    // T5: 1, 3, 4
	sw_1_4_htim->Instance->CCR1 = User_GetRegularParam(message->content, 16, 0);
    sw_1_4_htim->Instance->CCR2 = 0;
    sw_1_4_htim->Instance->CCR3 = User_GetRegularParam(message->content, 16, 1);
    sw_1_4_htim->Instance->CCR4 = User_GetRegularParam(message->content, 16, 2);
    // T3: 1
    sw_5_htim->Instance->CCR1 = User_GetRegularParam(message->content, 16, 3);
#else
  case USER__PWM_CAN_ID__SW_5_8:
    // T5: 1-4
	sw_1_4_htim->Instance->CCR1 = User_GetRegularParam(message->content, 16, 0);
	sw_1_4_htim->Instance->CCR2 = User_GetRegularParam(message->content, 16, 1);
	sw_1_4_htim->Instance->CCR3 = User_GetRegularParam(message->content, 16, 2);
    sw_1_4_htim->Instance->CCR4 = User_GetRegularParam(message->content, 16, 3);
  case USER__PWM_CAN_ID__SW_9:
    // T3: 1
	sw_5_htim->Instance->CCR1 = User_GetRegularParam(message->content, 16, 0);
    break;
#endif
  }
}
