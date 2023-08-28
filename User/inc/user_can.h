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

#define USER__CAN_BUFFER_SIZE 256

typedef uint8_t User_CanIndex_t;

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

void User_ReceiveCanMessage(CAN_HandleTypeDef *hcan);
void User_TransmitCanMessage(CAN_HandleTypeDef *hcan);

#endif /* INC_USER_CAN_H_ */
