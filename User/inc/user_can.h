/*
 * user_can.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_CAN_H_
#define INC_USER_CAN_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_can.h"

#include "stdbool.h"

typedef struct User_CanRxMessage
{
  CAN_RxHeaderTypeDef header;
  uint8_t content[8];
} User_CanRxMessage_t;

typedef struct User_CanTxMessage
{
  CAN_TxHeaderTypeDef header;
  uint8_t content[8];
} User_CanTxMessage_t;

void User_AddCanMessage(User_CanTxMessage_t *message);
bool User_TakeCanMessage(User_CanRxMessage_t *message);

#endif /* INC_USER_CAN_H_ */
