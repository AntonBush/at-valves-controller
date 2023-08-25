/*
 * user_swcurrent.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_swcurrent.h"

// private defines

#define USER__CALCULATE_CURRENTS_IN_INTEGERS

#define USER__MAX_SW_FEEDBACK_VOLTAGE 5
#define USER__MAX_ADC_DATA ((1 << 24) - 1)
#define USER__CAST_TO_VOLTAGE_MODIFIER (1.0 * USER__MAX_SW_FEEDBACK_VOLTAGE / USER__MAX_ADC_DATA)

#define USER__SW_FEEDBACK_RESISTANCE_OHM 620
#define USER__SW_FEEDBACK_CURRENT_MODIFIER (10 * 1000)
#define USER__SW_CURRENT_FACTOR_MODIFIER (100 * 8)

// public variables

uint16_t User_SwCurrentBuffer[USER__SW_COUNT];

// public functions

uint16_t User_CalculateSwCurrentFactor125EMin5(uint24_t adc_data)
{
#ifdef USER__CALCULATE_CURRENTS_IN_INTEGERS
  // adc_data * 5^6 / (31 * 2^17) = adc_data * 5^3 / 62 * 5^3 / 2^16
  return (adc_data * 125 / 62 * 125) >> 16;
#else
  float adc_voltage = USER__CAST_TO_VOLTAGE_MODIFIER * adc_data;
  float switch_feedback_current = adc_voltage / USER__SW_FEEDBACK_RESISTANCE_OHM;
  float switch_current = USER__SW_FEEDBACK_CURRENT_MODIFIER * switch_feedback_current;
  return USER__SW_CURRENT_FACTOR_MODIFIER * switch_current;
#endif
}
