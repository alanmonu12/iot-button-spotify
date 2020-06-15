#ifndef OLED_SSD1306_DATA_H
#define OLED_SSD1306_DATA_H

#include "stdint.h"
#include "driver/i2c.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define SSD1306_I2C_ADDR        0x78

#define I2C_MASTER_SCL_IO 22               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(1)                          /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define BH1750_SENSOR_ADDR CONFIG_BH1750_ADDR   /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START CONFIG_BH1750_OPMODE   /*!< Operation mode */
#define ESP_SLAVE_ADDR CONFIG_I2C_SLAVE_ADDRESS /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

#define SSD1306_WIDTH                           128
#define SSD1306_HEIGHT                          32

// some LEDs don't display anything in first two columns
// #define SSD1306_WIDTH           130

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

typedef enum {
    OFF = 0U,
    ON
}SSD1306_DISPLAY_STATUS;

typedef enum {
    _16MUX = 0b00010000,
    _17MUX,
    _18MUX,
    _19MUX,
    _20MUX,
    _21MUX,
    _22MUX,
    _23MUX,
    _24MUX,
    _25MUX,
    _26MUX,
    _27MUX,
    _28MUX,
    _29MUX,
    _30MUX,
    _31MUX,
    _32MUX,
    _33MUX,
    _34MUX,
    _35MUX,
    _36MUX,
    _37MUX,
    _38MUX,
    _39MUX,
    _40MUX,
    _41MUX,
    _42MUX,
    _43MUX,
    _44MUX,
    _45MUX,
    _46MUX,
    _47MUX,
    _48MUX,
    _49MUX,
    _50MUX,
    _51MUX,
    _52MUX,
    _53MUX,
    _54MUX,
    _55MUX,
    _56MUX,
    _57MUX,
    _58MUX,
    _59MUX,
    _60MUX,
    _61MUX,
    _62MUX,
    _63MUX,
    _64MUX
}SSD1306_MUX_RATIO_TypeDef;

typedef enum {
    START_LINE_00 = 0b01000000,
    START_LINE_01,
    START_LINE_02,
    START_LINE_03,
    START_LINE_04,
    START_LINE_05,
    START_LINE_06,
    START_LINE_07,
    START_LINE_08,
    START_LINE_09,
    START_LINE_10,
    START_LINE_11,
    START_LINE_12,
    START_LINE_13,
    START_LINE_14,
    START_LINE_15,
    START_LINE_16,
    START_LINE_17,
    START_LINE_18,
    START_LINE_19,
    START_LINE_20,
    START_LINE_21,
    START_LINE_22,
    START_LINE_23,
    START_LINE_24,
    START_LINE_25,
    START_LINE_26,
    START_LINE_27,
    START_LINE_28,
    START_LINE_29,
    START_LINE_30,
    START_LINE_31,
    START_LINE_32,
    START_LINE_33,
    START_LINE_34,
    START_LINE_35,
    START_LINE_36,
    START_LINE_37,
    START_LINE_38,
    START_LINE_39,
    START_LINE_40,
    START_LINE_41,
    START_LINE_42,
    START_LINE_43,
    START_LINE_44,
    START_LINE_45,
    START_LINE_46,
    START_LINE_47,
    START_LINE_48,
    START_LINE_49,
    START_LINE_50,
    START_LINE_51,
    START_LINE_52,
    START_LINE_53,
    START_LINE_54,
    START_LINE_55,
    START_LINE_56,
    START_LINE_57,
    START_LINE_58,
    START_LINE_59,
    START_LINE_60,
    START_LINE_61,
    START_LINE_62,
    START_LINE_63,
    START_LINE_64
}SSD1306_START_LINE_TypeDef;


typedef enum {
    NO_REMAP_BIT = 0,
    REMAP_BIT
}SSD1306_REMAP_BIT_TypeDef;

typedef enum {
    SEQUENTIAL_PIN_CONFIG = 0,
    ALTERNATIVE_PIN_CONFIG
}SSD1306_PIN_CONFIG_BIT_TypeDef;

typedef enum {
    CHARGE_PUMP_DISABLE_BIT = 0,
    CHARGE_PUMP_ENABLE_BIT
}SSD1306_CHARGE_PUMP_BIT_TypeDef;

typedef enum {
    HORIZONTAL_ADDRESING_MODE = 0x00,
    VERTICAL_ADDRESSING_MODE,
    PAGE_ADDRESSING_MODE
}SSD1306_ADDRESSING_MODE_TypeDef;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    bool Inverted;
    SSD1306_DISPLAY_STATUS status;
    bool Initialized;
} SSD1306_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} SSD1306_VERTEX;


#endif // !OLED_SSD1306_DATA_H
