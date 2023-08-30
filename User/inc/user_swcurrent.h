/*
 * user_swcurrent.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef INC_USER_SWCURRENT_H_
#define INC_USER_SWCURRENT_H_

#include "user_sw.h"
#include "user_int.h"

typedef struct User_SwCurrentData
{
  uint16_t values[USER__SW_COUNT];
} User_SwCurrentData_t;

uint16_t User_CalculateSwCurrentFactor125EMin5(User_UInt24_t adc_data);

void User_ReadSwCurrentData(User_SwCurrentData_t *data);
void User_WriteSwCurrentData(User_SwCurrentData_t *data);

#endif /* INC_USER_SWCURRENT_H_ */
