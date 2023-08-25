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

extern CAN_TxHeaderTypeDef User_CanTxHeader;
extern CAN_RxHeaderTypeDef User_CanRxHeader;
extern uint8_t User_CanTxData[8];
extern uint8_t User_CanRxData[8];
extern uint32_t User_TxMailbox;

void User_ReceiveCanMessage(CAN_HandleTypeDef *hcan);
void User_TransmitCanMessage(CAN_HandleTypeDef *hcan);

#endif /* INC_USER_CAN_H_ */
