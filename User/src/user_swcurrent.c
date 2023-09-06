/*
 * user_swcurrent.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swcurrent.h"

// private defines

#define USER__CALCULATE_CURRENTS_IN_INTEGERS

#define USER__MAX_SW_FEEDBACK_VOLTAGE 0.64
#define USER__MAX_ADC_DATA ((1 << 24) - 1)
#define USER__CAST_TO_VOLTAGE_MODIFIER (1.0 * USER__MAX_SW_FEEDBACK_VOLTAGE / USER__MAX_ADC_DATA)

#define USER__SW_FEEDBACK_RESISTANCE_OHM 620
#define USER__SW_FEEDBACK_CURRENT_MODIFIER (10 * 1000)
#define USER__SW_CURRENT_FACTOR_MODIFIER (1000 * 5)

/* @unused_reentrance
// private typedefs

typedef enum User_SwCurrentBufferIndex
{
  USER__SW_CURRENT_BUFFER__A
, USER__SW_CURRENT_BUFFER__B
, USER__SW_CURRENT_BUFFER__COUNT
, USER__SW_CURRENT_NO_BUFFER
} User_SwCurrentBufferIndex_t;
*/

// private variables

/* @unused_reentrance
static User_SwCurrentData_t User_SwCurrentBuffers[USER__SW_CURRENT_BUFFER__COUNT];
static User_SwCurrentBufferIndex_t User_ActiveSwCurrentBuffer = USER__SW_CURRENT_BUFFER__A;
*/
static User_SwCurrentData_t User_SwCurrentBuffer;

// public functions

uint16_t User_CalculateSwCurrentFactor2EMin4(User_UInt24_t adc_data)
{
#ifdef USER__CALCULATE_CURRENTS_IN_INTEGERS
  // adc_data * 5^5 / (31 * 2^15) = adc_data * 5^3 / 31 * 5^2 / 2^15
  uint32_t temp = (adc_data.value * 125 / 31 * 25) >> 14;
  // shift value to make average error ~ 0
  return (temp + temp % 2) >> 1;
#else
  float adc_voltage = USER__CAST_TO_VOLTAGE_MODIFIER * adc_data.value;
  float switch_feedback_current = adc_voltage / USER__SW_FEEDBACK_RESISTANCE_OHM;
  float switch_current = USER__SW_FEEDBACK_CURRENT_MODIFIER * switch_feedback_current;
  return USER__SW_CURRENT_FACTOR_MODIFIER * switch_current;
#endif
}

void User_ReadSwCurrentData(User_SwCurrentData_t *data)
{
/* @unused_reentrance
  (*data) = User_SwCurrentBuffers[User_ActiveSwCurrentBuffer];
*/
  (*data) = User_SwCurrentBuffer;
}

void User_WriteSwCurrentData(User_SwCurrentData_t *data)
{
/* @unused_reentrance
  User_SwCurrentBufferIndex_t new_buffer
    = User_ActiveSwCurrentBuffer == USER__SW_CURRENT_BUFFER__A
    ? USER__SW_CURRENT_BUFFER__B
    : USER__SW_CURRENT_BUFFER__A;
  User_SwCurrentBuffers[new_buffer] = (*data);
  User_ActiveSwCurrentBuffer = new_buffer;
*/
  User_SwCurrentBuffer = (*data);
}
