/*
 * CANLib_HelpFunctions.h
 *
 *  Created on: 23 апр. 2021 г.
 *      Author: Elijah
 */

#ifndef LIB_CANLIB_CANLIB_HELPFUNCTIONS_H_
#define LIB_CANLIB_CANLIB_HELPFUNCTIONS_H_

#include "stdint.h"

/* Для передачи и при изщвлечении параметра необходимо производить его нормализирование (наложение маски) */

//--- Посоледовательность байт в векторе ------------------------------------------------------------------------------
typedef enum ByteVector_ByteOrder
{
    BYTEVECTOR_INTEL        = 0,        // Последовательность байтов от меньшего к большему
    BYTEVECTOR_MOTOTOLA     = 1         // Последовательность байтов от большего к меньшему
} ByteVector_ByteOrder_t;

//--- Функция извлечения регулярного поля -----------------------------------------------------------------------------
uint32_t ByteVector_GetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index);
//--- Функция установки регулярного поля ------------------------------------------------------------------------------
void ByteVector_SetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index, uint32_t value);
//--- Функция извлечения параметра из байтового вектора ---------------------------------------------------------------
uint32_t ByteVector_GetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder);
//--- Функция установка параметра в байтовый вектор -------------------------------------------------------------------
void ByteVector_SetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder, uint32_t value);

//--- Предустановка массива данных ------------------------------------------------------------------------------------
void ByteVector_SetDataArray(uint8_t *vectorData, uint32_t size, uint8_t value);

#endif /* LIB_CANLIB_CANLIB_HELPFUNCTIONS_H_ */
