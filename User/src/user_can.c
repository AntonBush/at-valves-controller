/*
 * user_can.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_can.h"

//static CAN_TxHeaderTypeDef TxHeader;
//static CAN_RxHeaderTypeDef RxHeader;
//static uint8_t TxData[8] = {0,};
//static uint8_t RxData[8] = {0,};
//static uint32_t TxMailbox = 0;

/*
  if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
  {
    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  }
*/
void User_ReceiveCanMessage(CAN_HandleTypeDef *hcan)
{
  // @todo
}

/*
  //HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
 */
void User_TransmitCanMessage(CAN_HandleTypeDef *hcan)
{
  // @todo
}

