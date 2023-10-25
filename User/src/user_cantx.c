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
#ifdef USER__BOARD_1
  USER__CURRENT_CAN_ID__SW_1_4 = 0x14
#else
  USER__CURRENT_CAN_ID__SW_5_8 = 0x58
, USER__CURRENT_CAN_ID__SW_9   = 0x09
#endif
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
//extern uint16_t User_AdcCheckCounter;

//static uint16_t User_CanTxLifeCounter = 0;

static void User_CanTxInitMessage(uint32_t id);

void User_CanTx(void)
{
  User_SwCurrentData_t sw_current_data;
  User_ReadSwCurrentData(&sw_current_data);

#ifdef USER__BOARD_1
  User_CanTxInitMessage(USER__CURRENT_CAN_ID__SW_1_4);
  User_SetRegularParam(User_CanTxMessage.content, 16, 0, sw_current_data.values[0]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 1, sw_current_data.values[1]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 2, sw_current_data.values[2]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 3, sw_current_data.values[3]);
//  User_SetRegularParam(User_CanTxMessage.content, 16, 3, User_CanRxLifeCounter * 8);
  User_AddCanMessage(&User_CanTxMessage);
#else
  User_CanTxInitMessage(USER__CURRENT_CAN_ID__SW_5_8);
  User_SetRegularParam(User_CanTxMessage.content, 16, 0, sw_current_data.values[0]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 1, sw_current_data.values[1]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 2, sw_current_data.values[2]);
  User_SetRegularParam(User_CanTxMessage.content, 16, 3, sw_current_data.values[3]);
  User_AddCanMessage(&User_CanTxMessage);

  User_CanTxInitMessage(USER__CURRENT_CAN_ID__SW_9);
  User_SetRegularParam(User_CanTxMessage.content, 16, 0, sw_current_data.values[4]);

  //User_SetRegularParam(User_CanTxMessage.content, 16, 3, User_AdcCheckCounter * 8);
  User_AddCanMessage(&User_CanTxMessage);
#endif
}

static void User_CanTxInitMessage(uint32_t id)
{
  User_CanTxMessage.header.ExtId = id;
  ByteVector_SetDataArray(User_CanTxMessage.content, 8, 0);
}
