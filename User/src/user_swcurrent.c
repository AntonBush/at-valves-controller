/*
 * user_swcurrent.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swcurrent.h"

// private defines

#define USER__SW_CURRENT_BUFFER_SIZE_EXPONENT 1U
#define USER__SW_CURRENT_BUFFER_SIZE (1 << USER__SW_CURRENT_BUFFER_SIZE_EXPONENT)

#define USER__MAX_SW_FEEDBACK_VOLTAGE 5.12
#define USER__MAX_ADC_DATA ((1 << 24) - 1)

#define USER__SW_FEEDBACK_RESISTANCE_OHM 0.25
#define USER__SW_CURRENT_FACTOR_MODIFIER (100U * 8U)

#define USER__CAST_DATA_TO_VOLTAGE(data) \
( \
  (data) * USER__MAX_SW_FEEDBACK_VOLTAGE / USER__MAX_ADC_DATA \
)
#define USER__CAST_FB_VOLTAGE_TO_SW_CURRENT(fb_voltage) \
((fb_voltage) / USER__SW_FEEDBACK_RESISTANCE_OHM)
#define USER__APPLY_FACTOR_TO_SW_CURRENT(sw_current) \
((sw_current) * USER__SW_CURRENT_FACTOR_MODIFIER)

// private variables

static User_SwCurrentData_t User_SwCurrentBuffer[USER__SW_CURRENT_BUFFER_SIZE] = {0};
static User_SwCurrentData_t User_SwAverageCurrent = {0};
static struct User_SwCurrentBufferIndex_t {
  unsigned value : USER__SW_CURRENT_BUFFER_SIZE_EXPONENT;
} User_SwCurrentBufferIndex = {0};

// public functions

uint16_t User_CalculateSwCurrentFactor125EMin5(User_UInt24_t adc_data)
{
  float feedback_voltage = USER__CAST_DATA_TO_VOLTAGE(adc_data.value);
  float switch_current = USER__CAST_FB_VOLTAGE_TO_SW_CURRENT(feedback_voltage);
  return USER__APPLY_FACTOR_TO_SW_CURRENT(switch_current);
}

void User_ReadSwCurrentData(User_SwCurrentData_t *data)
{
  (*data) = User_SwAverageCurrent;
}

void User_WriteSwCurrentData(User_SwCurrentData_t *data)
{
  User_SwCurrentBuffer[User_SwCurrentBufferIndex.value] = (*data);
  User_SwCurrentBufferIndex.value += 1;

  for (int i = 0; i < USER__SW_COUNT; ++i)
  {
    uint32_t sum = 0;
    for (int buffer_i = 0; buffer_i < USER__SW_CURRENT_BUFFER_SIZE; ++buffer_i)
    {
      sum += User_SwCurrentBuffer[buffer_i].values[i];
    }
    User_SwAverageCurrent.values[i] = sum / USER__SW_CURRENT_BUFFER_SIZE;
  }
}
