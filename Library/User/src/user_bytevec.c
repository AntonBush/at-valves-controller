/*
 * user_bytevec.c
 *
 *  Created on: 25 авг. 2023 г.
 *      Author: bushev
 */

#include "user_bytevec.h"

//--- Функция извлечения регулярного поля -----------------------------------------------------------------------------
uint32_t User_GetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index)
{
  uint32_t shift = 32 - bitSize;
  uint32_t param = ByteVector_GetRegularParam(vectorData, bitSize, index);
  return param << shift >> shift;
}

//--- Функция установки регулярного поля ------------------------------------------------------------------------------
void User_SetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index, uint32_t value)
{
  uint32_t shift = 32 - bitSize;
  uint32_t param = value << shift >> shift;
  ByteVector_SetRegularParam(vectorData, bitSize, index, param);
}

//--- Функция извлечения параметра из байтового вектора ---------------------------------------------------------------
uint32_t User_GetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder)
{
  uint32_t shift = 32 - bitSize;
  uint32_t param = ByteVector_GetParam(vectorData, bitSize, bitPosition, byteOrder);
  return param << shift >> shift;
}

//--- Функция установка параметра в байтовый вектор -------------------------------------------------------------------
void User_SetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder, uint32_t value)
{
  uint32_t shift = 32 - bitSize;
  uint32_t param = value << shift >> shift;
  ByteVector_SetParam(vectorData, bitSize, bitPosition, byteOrder, param);
}
