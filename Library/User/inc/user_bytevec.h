/*
 * user_bytevec.h
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#ifndef USER_INC_USER_BYTEVEC_H_
#define USER_INC_USER_BYTEVEC_H_

#include "ByteVector_HelpFunctions.h"

//--- Функция извлечения регулярного поля -----------------------------------------------------------------------------
uint32_t User_GetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index);
//--- Функция установки регулярного поля ------------------------------------------------------------------------------
void User_SetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index, uint32_t value);
//--- Функция извлечения параметра из байтового вектора ---------------------------------------------------------------
uint32_t User_GetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder);
//--- Функция установка параметра в байтовый вектор -------------------------------------------------------------------
void User_SetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder, uint32_t value);


#endif /* USER_INC_USER_BYTEVEC_H_ */
