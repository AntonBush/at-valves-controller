/*
 * CANLib_HelpFunctions.c
 *
 *  Created on: 23 апр. 2021 г.
 *      Author: Elijah
 */

#include "ByteVector_HelpFunctions.h"

//--- Функция извлечения регулярного поля -----------------------------------------------------------------------------
uint32_t ByteVector_GetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index)
{
    uint32_t result = 0;
    // Расчет индекса битов
    uint32_t bitIndex = bitSize * index;
    // Смещение выходного значения
    uint32_t shift = bitIndex % 8; // Данная операция дополняет до круглого числа;
	
    uint32_t i;
    // Инициализируем первый биты
    result |= vectorData[bitIndex / 8] >> shift;

    for (i = 8 - shift; i < bitSize; i += 8)
    {
        // Добавляем данные
        result |= (uint32_t)(vectorData[(bitIndex + i) / 8]) << i;
    }

    return result;
}
//--- Функция установки регулярного поля ------------------------------------------------------------------------------
void ByteVector_SetRegularParam(uint8_t *vectorData, uint32_t bitSize, uint32_t index, uint32_t value)
{
	  uint32_t i;
    // Расчет индекса битов
    uint32_t bitIndex = bitSize * index;
    // Смещение выходного значения
    uint32_t shift = bitIndex % 8; // Данная операция дополняет до круглого числа;
    // Инициализируем первый биты
    vectorData[bitIndex / 8] |= value << shift;

    for (i = 8 - shift; i < bitSize; i += 8)
    {
        // Добавляем данные
        vectorData[(bitIndex + i) / 8] |= value >> i;
    }
}

//--- Функция извлечения параметра из байтового вектора ---------------------------------------------------------------
uint32_t ByteVector_GetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder)
{
	  uint32_t i;
	
    uint32_t result = 0;
    // Смещение выходного значения
    uint32_t shift = bitPosition % 8; // Данная операция дополняет до круглого числа;

    if (byteOrder == BYTEVECTOR_INTEL)
    {
        // Инициализируем первый биты
        result |= vectorData[bitPosition / 8] >> shift;

        for (i = 8 - shift; i < bitSize; i += 8)
        {
            // Добавляем данные
            result |= (uint32_t)(vectorData[(bitPosition + i) / 8]) << i;
        }
    }
    else if (byteOrder == BYTEVECTOR_MOTOTOLA)
    {
        // Инициализируем первый биты
        result |= (vectorData[bitPosition / 8] >> shift) << (bitSize - 8 + shift);

        for (i = 8 - shift; i < (bitSize - shift); i += 8)
        {
            // Добавляем данные
            result |= vectorData[(bitPosition + i) / 8] << (bitSize - 8 - i);
        }

        if (shift > 0)
        {
            result |= (vectorData[(bitPosition + bitSize - shift) / 8] << shift) >> shift;
        }
    }

    return result;
}
//--- Функция установка параметра в байтовый вектор -------------------------------------------------------------------
void ByteVector_SetParam(uint8_t *vectorData, uint32_t bitSize, uint32_t bitPosition, ByteVector_ByteOrder_t byteOrder, uint32_t value)
{
	  uint32_t i;
    // Смещение выходного значения
    uint32_t shift = bitPosition % 8; // Данная операция дополняет до круглого числа;

    if (byteOrder == BYTEVECTOR_INTEL)
    {
        // Инициализируем первый биты
        vectorData[bitPosition / 8] |= value << shift;

        for (i = 8 - shift; i < bitSize; i += 8)
        {
            // Добавляем данные
            vectorData[(bitPosition + i) / 8] |= value >> i;
        }
    }
    else if (byteOrder == BYTEVECTOR_MOTOTOLA)
    {
        // Инициализируем первый биты
        vectorData[(bitPosition) / 8] |= (value >> (bitSize - 8 + shift)) << shift;

        for (i = 8 - shift; i < (bitSize - shift); i += 8)
        {
            // Добавляем данные
            vectorData[(bitPosition + i) / 8] |= value >> (bitSize - 8 - i);
        }

        if (shift > 0)
        {
            vectorData[(bitPosition + bitSize - shift) / 8] |= (value << (bitSize - shift)) >> (bitSize - shift);
        }
    }
}

//--- Предустановка массива данных ------------------------------------------------------------------------------------
void ByteVector_SetDataArray(uint8_t *vectorData, uint32_t size, uint8_t value)
{
    uint32_t i = 0;
    for (i = 0; i < size; i++)
    {
        vectorData[i] = value;
    }
}
