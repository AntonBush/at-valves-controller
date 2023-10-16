/*
 * user_swadcinit.c
 *
 *  Created on: 5 сент. 2023 г.
 *      Author: bushev
 */

#include "user_swadcinit.h"

#include "user_swadc.h"
#include "user_swadc_private.h"

#include "main.h"

#include "stm32f1xx_hal_gpio.h"

// private defines

//#define USER__CALIBRATE_ADC
//#define USER__PERFORM_INTERNAL_ADC_CALIBRATION
#define USER__DEBUG_CALIBRATION
#define USER__ENABLE_CHOP
#define USER__DEBUG_READ

#define USER__ADC_INIT_TIMEOUT 5000U
#define USER__ADC_RESET_SEQUENCE_ELEMENT (~((uint8_t)0))
#define USER__ADC_RESET_SEQUENCE_LENGTH 4

// Чем больше сдвиг, тем ниже график
#define USER__ADC_OFFSET \
( \
  AD7718__ADC_DEFAULT_OFFSET \
)

// Чем больше усиление, тем больше наклон графика
#define USER__ADC_GAIN \
( \
  AD7718__ADC_DEFAULT_GAIN \
)

#ifdef USER__ENABLE_CHOP
#define USER__CHOP_FLAG 0
#else
#define USER__CHOP_FLAG AD7718__MR__DISABLE_CHOPPING
#endif
#define USER__ADC_MODE(mode) \
( \
  USER__CHOP_FLAG \
| AD7718__MR__NEGATIVE_INPUT_BUFFERED \
| AD7718__MR__10_CHANNELS \
| (mode) \
)

// private typedefs

// extern variables

extern SPI_HandleTypeDef hspi2;

// private function prototypes

static HAL_StatusTypeDef User_InitAdcN(User_Adc_t adc);

static HAL_StatusTypeDef User_ResetAdc(void);
static HAL_StatusTypeDef User_PreInitAdc(void);
#ifdef USER__CALIBRATE_ADC
static HAL_StatusTypeDef User_PrepareToCalibrateAdc(void);
static HAL_StatusTypeDef User_CalibrateAdc(User_Adc_t adc);
#endif
#ifdef USER__DEBUG_CALIBRATION
static HAL_StatusTypeDef User_DebugCalibration(User_Adc_t adc);
#endif

static HAL_StatusTypeDef User_PostInitAdc(void);
#ifdef USER__DEBUG_READ
static HAL_StatusTypeDef User_DebugRead(User_Adc_t adc);
#endif

__STATIC_INLINE HAL_StatusTypeDef User_TransmitToAdc(uint8_t *commands, uint8_t command_count)
{
  return HAL_SPI_Transmit(
    &hspi2
  , commands
  , command_count
  , USER__ADC_INIT_TIMEOUT
  );
}

__STATIC_INLINE HAL_StatusTypeDef User_TransmitReceiveToAdc(
  uint8_t *commands
, uint8_t *data
, uint8_t command_count
)
{
  return HAL_SPI_TransmitReceive(
    &hspi2
  , commands
  , data
  , command_count
  , USER__ADC_INIT_TIMEOUT
  );
}

__STATIC_INLINE void User_WaitAdcReady(User_Adc_t adc)
{
  while (HAL_GPIO_ReadPin(
    User_AdcInfo[adc].nrdy.port
    , User_AdcInfo[adc].nrdy.pin
  ))
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
  for (unsigned i = 0; i < USER__ADC_COUNT; ++i)
  {
    HAL_NVIC_DisableIRQ(User_AdcInfo[i].interrupt);
  }

  for (unsigned i = 0; i < USER__ADC_COUNT; ++i)
  {
    User_ChooseAdc(i);
    HAL_StatusTypeDef status = User_InitAdcN(i);
    if (status != HAL_OK)
    {
      return status;
    }
  }

  return HAL_OK;
}

// private functions

static HAL_StatusTypeDef User_InitAdcN(User_Adc_t adc)
{
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
  status = User_PrepareToCalibrateAdc();
  if (status != HAL_OK)
  {
    return status;
  }

  status = User_CalibrateAdc(adc);
  if (status != HAL_OK)
  {
    return status;
  }
#endif

#ifdef USER__DEBUG_CALIBRATION
    status = User_DebugCalibration(adc);
  if (status != HAL_OK)
  {
    return status;
  }
#endif

#ifdef USER__DEBUG_READ
  User_DebugRead(adc);
#endif

  return User_PostInitAdc();
}

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
#ifdef USER__ENABLE_CHOP
  , 0x13 // chop min
#else
//  , 0x03 // 1365.33 Hz
  , 0x04 // 1024 Hz
#endif
  };
  return User_TransmitToAdc(commands, sizeof(commands));
}

#ifdef USER__CALIBRATE_ADC
static HAL_StatusTypeDef User_PrepareToCalibrateAdc(void)
{
  uint8_t commands[] = {
    AD7718__CR_ADDR__MODE
  , USER__ADC_MODE(AD7718__MR_MOD__POWER_DOWN)
  };

  return User_TransmitToAdc(commands, sizeof(commands));
}

static HAL_StatusTypeDef User_CalibrateAdc(User_Adc_t adc)
{
  User_AdcChannelCommands_t pre_calibration_commands = {
    .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
  };
#ifdef USER__PERFORM_INTERNAL_ADC_CALIBRATION
  uint8_t zero_scale_calibration_commands[] = {
    AD7718__CR_ADDR__MODE
  , USER__ADC_MODE(AD7718__MR_MOD__INTERNAL_ZERO_SCALE_CALIBRATION)
  };
  uint8_t full_scale_calibration_commands[] = {
    AD7718__CR_ADDR__MODE
  , USER__ADC_MODE(AD7718__MR_MOD__INTERNAL_FULL_SCALE_CALIBRATION)
  };
#else
  uint8_t calibration_commands[] = {
    AD7718__CR_ADDR__ADC_OFFSET
  , (uint8_t)(USER__ADC_OFFSET >> 16)
  , (uint8_t)(USER__ADC_OFFSET >>  8)
  , (uint8_t)(USER__ADC_OFFSET >>  0)
  , AD7718__CR_ADDR__ADC_GAIN
  , (uint8_t)(USER__ADC_GAIN >> 16)
  , (uint8_t)(USER__ADC_GAIN >>  8)
  , (uint8_t)(USER__ADC_GAIN >>  0)
  };
#endif

  for (int i = 0; i < USER__SW_COUNT / 2; ++i)
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

#ifdef USER__PERFORM_INTERNAL_ADC_CALIBRATION
    status = User_TransmitToAdc(
        zero_scale_calibration_commands
    , sizeof(zero_scale_calibration_commands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
    User_WaitAdcReady(adc);

    status = User_TransmitToAdc(
        full_scale_calibration_commands
    , sizeof(full_scale_calibration_commands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
    User_WaitAdcReady(adc);
#else
    status = User_TransmitToAdc(
      calibration_commands
    , sizeof(calibration_commands)
    );
    if (status != HAL_OK)
    {
      return status;
    }
#endif
  }

  return HAL_OK;
}
#endif

#ifdef USER__DEBUG_CALIBRATION
struct
{
  uint32_t offset;
  uint32_t gain;
} User_AdcCalibrationInfo[USER__ADC_COUNT][USER__SW_COUNT / USER__ADC_COUNT];

static HAL_StatusTypeDef User_DebugCalibration(User_Adc_t adc)
{
  User_AdcChannelCommands_t pre_calibration_commands = {
    .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
  };
  struct
  {
    uint8_t offset_command;
    uint8_t dummy1[3];
    uint8_t gain_command;
    uint8_t dummy2[3];
  } debug_calibration_commands = {
    .offset_command = AD7718__CR__READ | AD7718__CR_ADDR__ADC_OFFSET
  , .gain_command   = AD7718__CR__READ | AD7718__CR_ADDR__ADC_GAIN
  };
  struct {
    uint8_t dummy1;
    uint8_t offset[3];
    uint8_t dummy2;
    uint8_t gain[3];
  } debug_calibration_data;
  _Static_assert(
    sizeof(debug_calibration_data) == sizeof(debug_calibration_commands)
  , "Size of calibration debug input must be the same as output"
  );

  const int sw_per_adc = USER__SW_COUNT / USER__ADC_COUNT;
  const int init_i = adc * sw_per_adc;
  const int end_i = (adc + 1) * sw_per_adc;
  for (int i = init_i; i < end_i; ++i)
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

    User_TransmitReceiveToAdc(
      (uint8_t *)(&debug_calibration_commands)
    , (uint8_t *)(&debug_calibration_data)
    , sizeof(debug_calibration_commands)
    );

    User_AdcCalibrationInfo[adc][i - init_i].offset = (
        debug_calibration_data.offset[0] << 16
      | debug_calibration_data.offset[1] <<  8
      | debug_calibration_data.offset[2] <<  0
    );
    User_AdcCalibrationInfo[adc][i - init_i].gain = (
        debug_calibration_data.gain[0] << 16
      | debug_calibration_data.gain[1] <<  8
      | debug_calibration_data.gain[2] <<  0
    );
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

  HAL_StatusTypeDef status = User_TransmitToAdc(commands, sizeof(commands));
  if (status != HAL_OK)
  {
    return status;
  }

  return HAL_OK;
}

#ifdef USER__DEBUG_READ
struct {
  uint32_t m;
  uint32_t d;
} User_FirstRead[USER__SW_COUNT];

static HAL_StatusTypeDef User_DebugRead(User_Adc_t adc)
{
  User_AdcChannelCommands_t pre_calibration_commands = {
    .choose_adc_channel_command = AD7718__CR_ADDR__ADC_CONTROL
  };

  const int sw_per_adc = USER__SW_COUNT / USER__ADC_COUNT;
  const int init_i = adc * sw_per_adc;
  const int end_i = (adc + 1) * sw_per_adc;
  for (int i = init_i; i < end_i; ++i)
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

    uint8_t read_commands[] = {
      AD7718__CR__READ | AD7718__CR_ADDR__ADC_DATA
    };
    User_TransmitToAdc(
      (uint8_t *) &read_commands
    , sizeof(read_commands)
    );

    User_WaitAdcReady(adc);

    uint8_t d[3];
    User_TransmitReceiveToAdc(
      (uint8_t *)(&d)
    , (uint8_t *)(&d)
    , sizeof(d)
    );

    User_FirstRead[i].d = (
        d[0] << 16
      | d[1] <<  8
      | d[2] <<  0
    );
  }

  return HAL_OK;
}
#endif
