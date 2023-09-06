/*
 * user_swadcinit.c
 *
 *  Created on: 5 сент. 2023 г.
 *      Author: bushev
 */

#include "user_swadcinit.h"

#include "user_swadc_private.h"

#include "main.h"

#include "stm32f1xx_hal_gpio.h"

// private defines

//#define USER__CALIBRATE_ADC

#define USER__ADC_INIT_TIMEOUT 5000U
#define USER__ADC_RESET_SEQUENCE_ELEMENT (~((uint8_t)0))
#define USER__ADC_RESET_SEQUENCE_LENGTH 4

#define USER__ADC_MODE(mode) \
( \
  AD7718__MR__DISABLE_CHOPPING \
| AD7718__MR__NEGATIVE_INPUT_BUFFERED \
| AD7718__MR__10_CHANNELS \
| (mode) \
)

// private typedefs

// extern variables

extern SPI_HandleTypeDef hspi2;

// private function prototypes

static HAL_StatusTypeDef User_ResetAdc(void);
static HAL_StatusTypeDef User_PreInitAdc(void);
#ifdef USER__CALIBRATE_ADC
static HAL_StatusTypeDef User_CalibrateAdc(void);
#endif
static HAL_StatusTypeDef User_PostInitAdc(void);

static inline HAL_StatusTypeDef User_TransmitToAdc(uint8_t *commands, uint8_t command_count)
{
  return HAL_SPI_Transmit(
    &hspi2
  , commands
  , command_count
  , USER__ADC_INIT_TIMEOUT
  );
}

static inline void User_WaitAdcCalibaration(void)
{
  while (HAL_GPIO_ReadPin(ADC_NRDY_GPIO_Port, ADC_NRDY_Pin))
  {}
/*
  while (1)
  {
    struct {
      uint8_t command;
      uint8_t dummy;
    } tx = {
      .command = AD7718__CR__READ | AD7718__CR_ADDR__STATUS_ON_READ
    };
    struct {
      uint8_t dummy;
      uint8_t status;
    } rx = {0};
    HAL_SPI_TransmitReceive(
      &hspi2
    , (uint8_t *)(&tx)
    , (uint8_t *)(&rx)
    , sizeof(tx)
    , USER__ADC_INIT_TIMEOUT
    );

    if (rx.status & AD7718__SR__CALIBRATION_COMPLETED) break;
  }
*/
}

// public functions

HAL_StatusTypeDef User_InitAdc(void)
{
//  HAL_NVIC_DisableIRQ(EXTI0_IRQn);

  HAL_StatusTypeDef status = User_ResetAdc();
  if (status != HAL_OK)
  {
    return status;
  }

  status = User_PreInitAdc();
  if (status != HAL_OK)
  {
    return status;
  }

#ifdef USER__CALIBRATE_ADC
  status = User_CalibrateAdc();
  if (status != HAL_OK)
  {
    return status;
  }
#endif

  return User_PostInitAdc();
}

// private functions

static HAL_StatusTypeDef User_ResetAdc(void)
{
  uint8_t commands[1 + USER__ADC_RESET_SEQUENCE_LENGTH] = {0};
  for (int i = 1; i < sizeof(commands); ++i)
  {
    commands[i] = USER__ADC_RESET_SEQUENCE_ELEMENT;
  }
  return User_TransmitToAdc(commands, sizeof(commands));
}

static HAL_StatusTypeDef User_PreInitAdc(void)
{
  uint8_t commands[] = {
    AD7718__CR_ADDR__IO_CONTROL
  , 0
  , AD7718__CR_ADDR__FILTER
  , 0x03 // 1365.33 Hz
//  , 0x13 // chop min
  };
  return User_TransmitToAdc(commands, sizeof(commands));
}

#ifdef USER__CALIBRATE_ADC
static HAL_StatusTypeDef User_CalibrateAdc(void)
{
  User_AdcChannelCommands_t pre_calibration_commands = {
    .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
  };
  uint8_t zero_scale_calibration_commands[] = {
    AD7718__CR_ADDR__MODE
  , USER__ADC_MODE(AD7718__MR_MOD__INTERNAL_ZERO_SCALE_CALIBRATION)
  };
  uint8_t full_scale_calibration_commands[] = {
    AD7718__CR_ADDR__MODE
  , USER__ADC_MODE(AD7718__MR_MOD__INTERNAL_FULL_SCALE_CALIBRATION)
  };

  for (int i = 0; i < USER__SW_COUNT; ++i)
  {
    pre_calibration_commands.adc_channel = User_AdcChannelCommands[i];
    HAL_StatusTypeDef status = User_TransmitToAdc(
      (uint8_t *) &pre_calibration_commands
    , sizeof(pre_calibration_commands)
    );
    if (status != HAL_OK)
    {
      return status;
    }

    status = User_TransmitToAdc(
        zero_scale_calibration_commands
    , sizeof(zero_scale_calibration_commands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
    User_WaitAdcCalibaration();

    status = User_TransmitToAdc(
        full_scale_calibration_commands
    , sizeof(full_scale_calibration_commands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
    User_WaitAdcCalibaration();
  }

  return HAL_OK;
}
#endif

static HAL_StatusTypeDef User_PostInitAdc(void)
{
  uint8_t commands[] = {
    AD7718__CR_ADDR__MODE
  , USER__ADC_MODE(AD7718__MR_MOD__CONTINUOUS_CONVERSION)
  };
  return User_TransmitToAdc(commands, sizeof(commands));
}
