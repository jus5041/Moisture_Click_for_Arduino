#ifndef _FDC2112_H_
#define _FDC2112_H_

#if ARDUINO >= 100
#include "Arduino.h"
#define WIRE_WRITE Wire.write
#else
#include "WProgram.h"
#define WIRE_WRITE Wire.send
#endif

#if defined(__SAM3X8E__)
typedef volatile RwReg PortReg;
 typedef uint32_t PortMask;
#define HAVE_PORTREG
#elif defined(ARDUINO_ARCH_SAMD)
// not supported
#elif defined(ESP8266) || defined(ESP32) || defined(ARDUINO_STM32_FEATHER) || defined(__arc__)
typedef volatile uint32_t PortReg;
  typedef uint32_t PortMask;
#elif defined(__AVR__)
typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
#define HAVE_PORTREG
#else
// chances are its 32 bit so assume that
typedef volatile uint32_t PortReg;
typedef uint32_t PortMask;
#endif

#define FDC2112_I2C_ADDR_0   0x2A
#define FDC2112_I2C_ADDR_1   0x2B
// Address is 0x2A (default) or 0x2B (if ADDR is high)

#define MOISTURE_SAMPLE_NUM 16
#define MOISTURE_12_BIT_DATA 0x0FFF

/**
 * \defgroup error_code Error Code
 * \{
 */
#define MOISTURE_RETVAL  uint8_t

#define MOISTURE_OK           0x00
#define MOISTURE_INIT_ERROR   0xFF
/** \} */

/**
 * \defgroup slave_address Device slave address
 * \{
 */
#define MOISTURE_DEV_ADR_GND                      0x2A
#define MOISTURE_DEV_ADR_VCC                      0x2B
/** \} */

/**
 * \defgroup def_offset Default offset
 * \{
 */
#define MOISTURE_DEFAULT_OFFSET                   0x00AA
/** \} */

/**
 * \defgroup register Register
 * \{
 */ 
#define MOISTURE_REG_DATA                         0x00
#define MOISTURE_REG_RCNT                         0x08
#define MOISTURE_REG_OFFSET                       0x0C
#define MOISTURE_REG_SETTLE_CNT                   0x10
#define MOISTURE_REG_CLOCK_DIVIDERS               0x14
#define MOISTURE_REG_STATUS                       0x18
#define MOISTURE_REG_ERR_CFG                      0x19
#define MOISTURE_REG_CFG                          0x1A
#define MOISTURE_REG_MUX_CFG                      0x1B
#define MOISTURE_REG_RESET_DEV                    0x1C
#define MOISTURE_REG_DRIVE_CURR                   0x1E
#define MOISTURE_REG_DEVICE_ID                    0x7F
#define MOISTURE_REG_MFC_ID                       0x7E
/** \} */

/**
 * \defgroup drive_current Drive current
 * \{
 */
#define MOISTURE_DRIVE_CURR_0_016mA               0x0000
#define MOISTURE_DRIVE_CURR_0_018mA               0x0800
#define MOISTURE_DRIVE_CURR_0_021mA               0x1000
#define MOISTURE_DRIVE_CURR_0_025mA               0x1800
#define MOISTURE_DRIVE_CURR_0_028mA               0x2000
#define MOISTURE_DRIVE_CURR_0_033mA               0x2800
#define MOISTURE_DRIVE_CURR_0_038mA               0x3000
#define MOISTURE_DRIVE_CURR_0_044mA               0x3800
#define MOISTURE_DRIVE_CURR_0_052mA               0x4000
#define MOISTURE_DRIVE_CURR_0_060mA               0x4800
#define MOISTURE_DRIVE_CURR_0_069mA               0x5000
#define MOISTURE_DRIVE_CURR_0_081mA               0x5800
#define MOISTURE_DRIVE_CURR_0_093mA               0x6000
#define MOISTURE_DRIVE_CURR_0_108mA               0x6800
#define MOISTURE_DRIVE_CURR_0_126mA               0x7000
#define MOISTURE_DRIVE_CURR_0_146mA               0x7800
#define MOISTURE_DRIVE_CURR_0_169mA               0x8000
#define MOISTURE_DRIVE_CURR_0_196mA               0x8800
#define MOISTURE_DRIVE_CURR_0_228mA               0x9000
#define MOISTURE_DRIVE_CURR_0_264mA               0x9800
#define MOISTURE_DRIVE_CURR_0_307mA               0xA000
#define MOISTURE_DRIVE_CURR_0_356mA               0xA800
#define MOISTURE_DRIVE_CURR_0_413mA               0xB000
#define MOISTURE_DRIVE_CURR_0_479mA               0xB800
#define MOISTURE_DRIVE_CURR_0_555mA               0xC000
#define MOISTURE_DRIVE_CURR_0_644mA               0xC800
#define MOISTURE_DRIVE_CURR_0_747mA               0xD000
#define MOISTURE_DRIVE_CURR_0_867mA               0xD800
#define MOISTURE_DRIVE_CURR_1_006mA               0xE000
#define MOISTURE_DRIVE_CURR_1_167mA               0xE800
#define MOISTURE_DRIVE_CURR_1_354mA               0xF000
#define MOISTURE_DRIVE_CURR_1_571mA               0xF800
/** \} */

/**
 * \defgroup reset Reset device
 * \{
 */
#define MOISTURE_RESET_DEVICE                     0x8000
#define MOISTURE_OUTPUT_GAIN_x1                   0x0000
#define MOISTURE_OUTPUT_GAIN_x4                   0x0200
#define MOISTURE_OUTPUT_GAIN_x8                   0x0400
#define MOISTURE_OUTPUT_GAIN_x16                  0x0600
/** \} */

/**
 * \defgroup mux_cfg Mux configuration
 * \{
 */
#define MOISTURE_MUX_CFG_AUTOSCAN_MODE            0x8000
#define MOISTURE_MUX_CFG_CONTINUOUS_MODE          0x0000
#define MOISTURE_MUX_CFG_RR_SEQUENCE_CH_01        0x0000
#define MOISTURE_MUX_CFG_DEGLITCH_1MHz            0x0001
#define MOISTURE_MUX_CFG_DEGLITCH_3_3MHz          0x0004
#define MOISTURE_MUX_CFG_DEGLITCH_10MHz           0x0005
#define MOISTURE_MUX_CFG_DEGLITCH_33MHz           0x0007
/** \} */

/**
 * \defgroup configuration Configuration
 * \{
 */
#define MOISTURE_CFG_ACTIVE_CHAN_CH0              0x0000
#define MOISTURE_CFG_DEVICE_ACTIVE                0x0000
#define MOISTURE_CFG_DEVICE_IN_SLEEP_MODE         0x2000
#define MOISTURE_CFG_FULL_CURRENT_MODE            0x0000
#define MOISTURE_CFG_LOW_POWER_MODE               0x0800
#define MOISTURE_CFG_INT_PIN_ENABLE               0x0000
#define MOISTURE_CFG_INT_PIN_DISABLE              0x0080
/** \} */

/**
 * \defgroup error_cfg  Error configuration
 * \{
 */
#define MOISTURE_ERR_CFG_WATCHDOG_TIMEOUT         0x2000
#define MOISTURE_ERR_CFG_AMPLITUDE_HIGH           0x1000
#define MOISTURE_ERR_CFG_AMPLITUDE_LOW            0x0800
#define MOISTURE_ERR_CFG_DATA_RDY_IN_PIN          0x0001
/** \} */

/**
 * \defgroup status  Status
 * \{
 */
#define MOISTURE_STATUS_ERR_CHAN_0                0x0000
#define MOISTURE_STATUS_ERR_WD                    0x0800
#define MOISTURE_STATUS_ERR_AHW                   0x0400
#define MOISTURE_STATUS_ERR_AHL                   0x0200
#define MOISTURE_STATUS_DATA_RDY                  0x0040
#define MOISTURE_STATUS_CH0_UNREADCONV            0x0008
/** \} */

/**
 * \defgroup clock_divider Clock divider
 * \{
 */
#define MOISTURE_CLOCK_DIVIDERS_x1                0x00001
#define MOISTURE_CLOCK_DIVIDERS_x2                0x00002
#define MOISTURE_CLOCK_DIVIDERS_x3                0x00003
#define MOISTURE_CLOCK_DIVIDERS_x4                0x00004
#define MOISTURE_CLOCK_DIVIDERS_x5                0x00005
#define MOISTURE_CLOCK_DIVIDERS_x6                0x00006
#define MOISTURE_CLOCK_DIVIDERS_x7                0x00007
#define MOISTURE_CLOCK_DIVIDERS_x8                0x00008
#define MOISTURE_CLOCK_DIVIDERS_x9                0x00009
#define MOISTURE_CLOCK_DIVIDERS_x10               0x0000A
#define MOISTURE_CLOCK_DIVIDERS_x11               0x0000B
#define MOISTURE_CLOCK_DIVIDERS_x12               0x0000C
#define MOISTURE_CLOCK_DIVIDERS_x13               0x0000D
#define MOISTURE_CLOCK_DIVIDERS_x14               0x0000E
#define MOISTURE_CLOCK_DIVIDERS_x15               0x0000F
/** \} */

/**
 * \defgroup device_id Device ID
 * \{
 */
#define MOISTURE_FDC2112_ID                       0x3054
#define MOISTURE_FDC2212_ID                       0x3055
/** \} */

/**
 * \defgroup device_settings Device Settings
 * \{
 */
#define MOISTURE_SETTLE_CNT_CFG_DEFAULT           0x0064
#define MOISTURE_RCNT_CFG_DEFAULT                 0x010F
#define MOISTURE_COMBINED_VAL_CH0_DIV             0x2001
#define MOISTURE_DRIVE_CURR_CH0                   0x7C00
#define MOISTURE_ALL_ERR_ENABLED                  0xFFFF
#define MOISTURE_REG_CFG_DEFAULT_SETTINGS         0x0000
#define MOISTURE_INPUT_DEGLITCH_FILT_BWDTH_33MHZ  0x0007
/** \} */

class FDC2112 {
public:
    FDC2112(uint8_t i2caddr);

    boolean begin();

    unsigned long readConv();

private:
    uint8_t _i2caddr;
    
    void loadSettings();

    void write8FDC(uint16_t address, uint8_t data);
    void write16FDC(uint16_t address, uint16_t data);
    
    uint16_t read16FDC(uint16_t address);
    uint8_t read8FDC(uint16_t address);
    
};

#endif //include guard
