/*
 * user_it.c
 *
 *  Created on: Aug 24, 2023
 *      Author: bushev
 */

#include "user_swcurrent.h"
#include "user_swadc.h"
#include "user_can.h"

extern TIM_HandleTypeDef htim6;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim != &htim6) return;

  // @todo -> User_AddCanMessage
  /*
    User_CanTxHeader.ExtId = USER__PWM_CAN_ID__T3;
    User_SetRegularParam(User_CanRxData, 16, 0, User_SwCurrentBuffer[0]);
    User_SetRegularParam(User_CanRxData, 16, 1, User_SwCurrentBuffer[1]);
    HAL_CAN_AddTxMessage(hcan, &User_CanTxHeader, User_CanTxData, &User_TxMailbox);

    User_CanTxHeader.ExtId = USER__PWM_CAN_ID__T4;
    User_SetRegularParam(User_CanRxData, 16, 0, User_SwCurrentBuffer[2]);
    User_SetRegularParam(User_CanRxData, 16, 1, User_SwCurrentBuffer[3]);
    User_SetRegularParam(User_CanRxData, 16, 2, User_SwCurrentBuffer[4]);
    User_SetRegularParam(User_CanRxData, 16, 3, User_SwCurrentBuffer[5]);
    HAL_CAN_AddTxMessage(hcan, &User_CanTxHeader, User_CanTxData, &User_TxMailbox);

    User_CanTxHeader.ExtId = USER__PWM_CAN_ID__T5;
    User_SetRegularParam(User_CanRxData, 16, 0, User_SwCurrentBuffer[6]);
    User_SetRegularParam(User_CanRxData, 16, 1, User_SwCurrentBuffer[7]);
    User_SetRegularParam(User_CanRxData, 16, 2, User_SwCurrentBuffer[8]);
    User_SetRegularParam(User_CanRxData, 16, 3, User_SwCurrentBuffer[9]);
    HAL_CAN_AddTxMessage(hcan, &User_CanTxHeader, User_CanTxData, &User_TxMailbox);
  */
  // @todo make init before uncomment
  //User_StartPollingAdc();
}
