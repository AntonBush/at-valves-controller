/*
 * user_int.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef USER_INC_USER_INT_H_
#define USER_INC_USER_INT_H_

#include "stdint.h"

typedef struct User_UInt24
{
  uint32_t value : 24;
} User_UInt24_t;

#endif /* USER_INC_USER_INT_H_ */
