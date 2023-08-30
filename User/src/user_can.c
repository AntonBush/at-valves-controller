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

// private defines

#define USER__CAN_BUFFER_SIZE_EXPONENT 6
#define USER__CAN_BUFFER_SIZE (1 << 6)

// private typedefs

typedef struct User_CanRxBuffer
{
  User_CanRxMessage_t messages[USER__CAN_BUFFER_SIZE];
  uint8_t read_index  : USER__CAN_BUFFER_SIZE_EXPONENT;
  uint8_t write_index : USER__CAN_BUFFER_SIZE_EXPONENT;
} User_CanRxBuffer_t;

typedef struct User_CanTxBuffer
{
  User_CanTxMessage_t messages[USER__CAN_BUFFER_SIZE];
  uint8_t read_index  : USER__CAN_BUFFER_SIZE_EXPONENT;
  uint8_t write_index : USER__CAN_BUFFER_SIZE_EXPONENT;
} User_CanTxBuffer_t;

// extern variables

extern CAN_HandleTypeDef hcan1;

// private variables

static User_CanRxBuffer_t User_CanRxBuffer = {
  .read_index  = 0
, .write_index = 0
};
static User_CanTxBuffer_t User_CanTxBuffer = {
  .read_index  = 0
, .write_index = 0
};

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
*/
static uint32_t User_TxMailbox = 0;

// private function prototypes

static void User_ReceiveCanMessage(void);
static void User_TransmitCanMessage(void);

// extern functions

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan != &hcan1) return;
  User_ReceiveCanMessage();
}

void HAL_CAN_TxMailbox0CompleteCallback (CAN_HandleTypeDef * hcan)
{
  if (hcan != &hcan1) return;
  User_TransmitCanMessage();
}

void HAL_CAN_TxMailbox1CompleteCallback (CAN_HandleTypeDef * hcan)
{
  if (hcan != &hcan1) return;
  User_TransmitCanMessage();
}

void HAL_CAN_TxMailbox2CompleteCallback (CAN_HandleTypeDef * hcan)
{
  if (hcan != &hcan1) return;
  User_TransmitCanMessage();
}

// public functions

void User_AddCanMessage(User_CanTxMessage_t *message)
{
  User_CanTxBuffer.messages[User_CanTxBuffer.write_index] = *message;
  User_CanTxBuffer.write_index += 1;

  User_TransmitCanMessage();
}

bool User_TakeCanMessage(User_CanRxMessage_t *message)
{
  if (User_CanRxBuffer.read_index == User_CanRxBuffer.write_index)
  {
    return false;
  }

  (*message) = User_CanRxBuffer.messages[User_CanRxBuffer.read_index];
  User_CanRxBuffer.read_index += 1;
  return true;
}

// private

static void User_ReceiveCanMessage(void)
{
  HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(
    &hcan1
  , CAN_RX_FIFO0
  , &User_CanRxBuffer.messages[User_CanRxBuffer.write_index].header
  , User_CanRxBuffer.messages[User_CanRxBuffer.write_index].content
  );

  if (status != HAL_OK) return;

  User_CanRxBuffer.write_index += 1;
}

static void User_TransmitCanMessage(void)
{
  uint32_t free_mailbox_count = HAL_CAN_GetTxMailboxesFreeLevel (&hcan1);

  for (
    int i = 0
  ; (
       i < free_mailbox_count
    && User_CanTxBuffer.read_index != User_CanTxBuffer.write_index
    )
  ; ++i
  )
  {
    HAL_CAN_AddTxMessage(
      &hcan1
    , &User_CanTxBuffer.messages[User_CanTxBuffer.read_index].header
    , User_CanTxBuffer.messages[User_CanTxBuffer.read_index].content
    , &User_TxMailbox
    );
    User_CanTxBuffer.read_index += 1;
  }
}
