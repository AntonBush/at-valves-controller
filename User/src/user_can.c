/*
 * user_can.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_can.h"

#include "user_sw.h"
#include "user_swcurrent.h"
#include "user_bytevec.h"

// private typedefs

typedef enum User_PwmCanId
{
  USER__PWM_CAN_ID__T3 = 0x113FFFFF
, USER__PWM_CAN_ID__T4 = 0x114FFFFF
, USER__PWM_CAN_ID__T5 = 0x115FFFFF
} User_PwmCanId_t;

typedef enum User_CurrentCanId
{
  USER__CURRENT_CAN_ID__T3 = 0x123FFFFF
, USER__CURRENT_CAN_ID__T4_T5 = 0x124FFFFF
} User_CurrentCanId_t;

// private variables

extern CAN_HandleTypeDef hcan1;

/*
CAN_TxHeaderTypeDef User_CanTxHeader = {
  .StdId = 0
, .ExtId = 0
, .RTR = CAN_RTR_DATA // alternative: CAN_RTR_REMOTE
, .IDE = CAN_ID_EXT // alternative: CAN_ID_STD
, .DLC = 8
, .TransmitGlobalTime = 0
};
CAN_RxHeaderTypeDef User_CanRxHeader;
uint8_t User_CanTxData[8] = {0,};
uint8_t User_CanRxData[8] = {0,};
uint32_t User_TxMailbox = 0;
*/

// private function prototypes

//static void rx(void);

// extern functions

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan != &hcan1) return;
  User_ReceiveCanMessage(hcan);
}

// public functions

void User_ReceiveCanMessage(CAN_HandleTypeDef *hcan)
{
/*
  {
    HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(
      hcan
    , CAN_RX_FIFO0
    , &User_CanRxHeader
    , User_CanRxData
    );
    if (status != HAL_OK) return;
  }
  if (User_CanRxHeader.IDE != CAN_ID_EXT) return;

  switch (User_CanRxHeader.ExtId)
  {
  case USER__CURRENT_CAN_ID__T3:
    User_T3PwmBuffer[0] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 0)
    );
    User_T3PwmBuffer[1] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 1)
    );
    break;
  case USER__CURRENT_CAN_ID__T4_T5:
    User_T4PwmBuffer[0] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 0)
    );
    User_T4PwmBuffer[1] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 1)
    );
    User_T4PwmBuffer[2] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 2)
    );
    User_T4PwmBuffer[3] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 3)
    );
    User_T5PwmBuffer[0] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 4)
    );
    User_T5PwmBuffer[1] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 5)
    );
    User_T5PwmBuffer[2] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 6)
    );
    User_T5PwmBuffer[3] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(User_CanRxData, 8, 7)
    );
    break;
  }
*/
}

void User_TransmitCanMessage(CAN_HandleTypeDef *hcan)
{
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
}