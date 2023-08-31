/*
 * user_cantx.c
 *
 *  Created on: 30 авг. 2023 г.
 *      Author: bushev
 */

#include "user_cantx.h"

#include "user_swcurrent.h"
#include "user_can.h"

#include "user_bytevec.h"

typedef enum User_CurrentCanId
{
  USER__CURRENT_CAN_ID__T3 = 0x123FFFFF
, USER__CURRENT_CAN_ID__T4 = 0x124FFFFF
, USER__CURRENT_CAN_ID__T5 = 0x125FFFFF
} User_CurrentCanId_t;

static User_CanTxMessage_t User_CanTxMessage = {
  .header = {
    .StdId = 0
  , .ExtId = 0
  , .RTR = CAN_RTR_DATA // alternative: CAN_RTR_REMOTE
  , .IDE = CAN_ID_EXT // alternative: CAN_ID_STD
  , .DLC = 8
  , .TransmitGlobalTime = 0
  }
, .content = {0}
};

extern uint16_t User_CanRxLifeCounter;

static uint16_t User_CanTxLifeCounter = 0;

static void User_CanTxInitMessage(uint32_t id);

void User_CanTx(void)
{
  User_SwCurrentData_t sw_current_data;
  User_ReadSwCurrentData(&sw_current_data);

  User_CanTxInitMessage(USER__CURRENT_CAN_ID__T3);
  User_SetRegularParam(User_CanTxMessage.content, 16, 0, sw_current_data.values[0]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 1, sw_current_data.values[1]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 2, User_CanTxLifeCounter++);
  User_SetRegularParam(User_CanTxMessage.content, 16, 3, User_CanRxLifeCounter);
  User_AddCanMessage(&User_CanTxMessage);

  User_CanTxInitMessage(USER__CURRENT_CAN_ID__T4);
  User_SetRegularParam(User_CanTxMessage.content, 16, 0, sw_current_data.values[2]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 1, sw_current_data.values[3]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 2, sw_current_data.values[4]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 3, sw_current_data.values[5]);
  User_AddCanMessage(&User_CanTxMessage);

  User_CanTxInitMessage(USER__CURRENT_CAN_ID__T5);
  User_SetRegularParam(User_CanTxMessage.content, 16, 0, sw_current_data.values[6]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 1, sw_current_data.values[7]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 2, sw_current_data.values[8]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 3, sw_current_data.values[9]);
  User_AddCanMessage(&User_CanTxMessage);
}

static void User_CanTxInitMessage(uint32_t id)
{
  User_CanTxMessage.header.ExtId = id;
  ByteVector_SetDataArray(User_CanTxMessage.content, 8, 0);
}
