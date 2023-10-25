/*
 * user_logic.c
 *
 *  Created on: Aug 28, 2023
 *      Author: bushev
 */

#include "user_logic.h"

#include "user_sw.h"
#include "user_swadc.h"
#include "user_swadcinit.h"
#include "user_swcurrent.h"
#include "user_canrx.h"
#include "user_cantx.h"

#include "main.h"
#include "can.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

// private defines

//#define USER__DEBUG_ADC_VOLTAGE

// private variables

static bool User_InterruptUpdateFlag = false;
static bool User_LoopUpdateFlag = false;

// private function prototypes

static void User_updateSwCurrent(void);

// public functions

void User_Init(void)
{
  HAL_CAN_Start(&hcan1);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

  HAL_TIM_Base_Start_IT(&htim6);

  if (User_InitAdc() != HAL_OK)
  {
    Error_Handler();
  }
  User_StartPollingAdc();
}

void User_updateData(void)
{
  if (User_InterruptUpdateFlag == User_LoopUpdateFlag)
  {
    User_InterruptUpdateFlag = !User_InterruptUpdateFlag;
  }
}

void User_update(void)
{
  if (User_isAdcDataUpdated())
  {
    User_updateSwCurrent();
  }

  User_CanRxMessage_t message;
  while (User_TakeCanMessage(&message))
  {
    User_CanRx(&message);
  }

  if (User_LoopUpdateFlag != User_InterruptUpdateFlag)
  {
    User_LoopUpdateFlag = User_InterruptUpdateFlag;
    User_CanTx();
  }
}

// private functions

void User_updateSwCurrent(void)
{
  User_AdcData_t adc_data;
  User_ReadAdcData(&adc_data);
  User_SwCurrentData_t sw_current_data;
  for (uint8_t i = 0; i < USER__SW_COUNT; ++i)
  {
    User_UInt24_t data = {
      .value = 0
    };
    for (uint8_t j = 0; j < USER__ADC_POLL_DATA_LENGTH; ++j)
    {
      data.value = (data.value << 8) | (adc_data.values[i][j]);
    }
#ifdef USER__DEBUG_ADC_VOLTAGE
    // factor = 0.00125 = 1 / 800
    // 2^24 - 1 ~ 0.64 * 100
    sw_current_data.values[i] = ((25 * data.value >> 12) + 1) >> 1;
#else
    sw_current_data.values[i] = User_CalculateSwCurrentFactor125EMin5(data);
#endif
  }
  User_WriteSwCurrentData(&sw_current_data);
}
