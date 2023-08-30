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
  USER__PWM_CAN_ID__T3    = 0x113FFFFF
, USER__PWM_CAN_ID__T4_T5 = 0x114FFFFF
} User_PwmCanId_t;

void User_CanRx(User_CanRxMessage_t *message)
{
  if (message->header.IDE != CAN_ID_EXT) return;

  switch (message->header.ExtId)
  {
  case USER__PWM_CAN_ID__T3:
    User_T3PwmBuffer[0] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 0)
    );
    User_T3PwmBuffer[1] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 1)
    );
    break;
  case USER__PWM_CAN_ID__T4_T5:
    User_T4PwmBuffer[0] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 0)
    );
    User_T4PwmBuffer[1] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 1)
    );
    User_T4PwmBuffer[2] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 2)
    );
    User_T4PwmBuffer[3] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 3)
    );
    User_T5PwmBuffer[0] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 4)
    );
    User_T5PwmBuffer[1] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 5)
    );
    User_T5PwmBuffer[2] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 6)
    );
    User_T5PwmBuffer[3] = USER__PWM_VALUE_FROM_PERCENTAGES(
        User_GetRegularParam(message->content, 8, 7)
    );
    break;
  }
}
