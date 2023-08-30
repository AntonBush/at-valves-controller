/*
 * user_logic.c
 *
 *  Created on: Aug 28, 2023
 *      Author: bushev
 */

#include "user_logic.h"

#include "user_sw.h"
#include "user_swadc.h"
#include "user_swcurrent.h"

static void User_updateSwCurrent(void);

void User_update(void)
{
  if (User_isAdcDataUpdated())
  {
    User_updateSwCurrent();
  }
}

void User_updateSwCurrent(void)
{
  User_AdcData_t adc_data;
  User_ReadAdcData(&adc_data);
  for (uint8_t i = 0; i < USER__SW_COUNT; ++i)
  {
    User_UInt24_t data = {
      .value = 0
    };
    for (uint8_t j = 0; j < USER__ADC_POLL_DATA_LENGTH; ++j)
    {
      data.value = (data.value << 8) | (adc_data.values[i][j]);
    }
    User_SwCurrentBuffer[i] = User_CalculateSwCurrentFactor125EMin5(data);
  }
}
