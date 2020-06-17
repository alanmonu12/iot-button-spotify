#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include "stdlib.h"
#include "string.h"
#include "esp_log.h"
#include "oled-ssd1306-data.h"
#include "oled-ssd1306-font.h"

#define CTRL_CMD                                    0x00
#define CTRL_DATA                                   0x40

#define SET_DISPLAY_ON_CMD                          0xAF
#define SET_DISPLAY_OFF_CMD                         0xAE
#define SET_PAGE_ADDRESS_CMD                        0x22
#define SET_MULTIPLEX_RATIO_CMD                     0xA8
#define SET_DISPLAY_OFFSET_CMD                      0xD3
#define SET_COM_PINS_HW_CONFIG_CMD                  0xDA
#define SET_CONTRAST_CONTROL_CMD                    0x81
#define ENTIRE_DISPLAY_ON_RESUME_CMD                0xA4
#define ENTIRE_DISPLAY_ON_CMD                       0xA5
#define SET_NORMAL_DISPLAY_MODE_CMD                 0xA6
#define SET_INVERSE_DISPLAY_MODE_CMD                0xA7
#define SET_CLOCK_FQ_CMD                            0xD5
#define SET_CHARGE_PUMP_SETTING_CMD                 0x8D
#define SET_MEMORY_ADDRESSING_MODE_CMD              0x20
#define SET_COLUMN_ADDRESS_CMD                      0x21

typedef struct oled_ssd1306 oled_ssd1306;

struct oled_ssd1306 {
    uint8_t address;
    SSD1306_t display;
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
    void* handler;
};

// Procedure definitions
void oled_ssd1306_create(oled_ssd1306 *self, uint8_t address);
void oled_ssd1306_Init(oled_ssd1306 *self);
void oled_ssd1306_Fill(oled_ssd1306 *self, SSD1306_COLOR color);
void oled_ssd1306_UpdateScreen(oled_ssd1306 *self);
void oled_ssd1306_display_on(oled_ssd1306 *self);
void oled_ssd1306_display_off(oled_ssd1306 *self);
void oled_ssd1306_set_multiplex_ratio(oled_ssd1306 *self, SSD1306_MUX_RATIO_TypeDef mux_ratio);
void oled_ssd1306_set_display_offset(oled_ssd1306 *self, uint8_t off_set);
void oled_ssd1306_set_display_start_line(oled_ssd1306 *self, SSD1306_START_LINE_TypeDef start_line);
void oled_ssd1306_set_segment_remap(oled_ssd1306 *self, SSD1306_REMAP_BIT_TypeDef bit);
void oled_ssd1306_set_com_output_scan_direction(oled_ssd1306 *self, SSD1306_REMAP_BIT_TypeDef bit);
void oled_ssd1306_set_com_pins_hw_config(oled_ssd1306 *self, SSD1306_REMAP_BIT_TypeDef remap_bit, SSD1306_PIN_CONFIG_BIT_TypeDef pin_bit);
void oled_ssd1306_set_contrast_control(oled_ssd1306 *self, uint8_t contrast);
void oled_ssd1306_entire_display_on(oled_ssd1306 *self);
void oled_ssd1306_entire_display_on_resume(oled_ssd1306 *self);
void oled_ssd1306_set_normal_display_mode(oled_ssd1306 *self);
void oled_ssd1306_set_inverse_display_mode(oled_ssd1306 *self);
void oled_ssd1306_set_osc_frenq(oled_ssd1306 *self, uint8_t divider, uint8_t frequency);
void oled_ssd1306_set_charge_pump_config(oled_ssd1306 *self, SSD1306_CHARGE_PUMP_BIT_TypeDef bit);
void oled_ssd1306_set_memory_addressing_mode(oled_ssd1306 *self, SSD1306_ADDRESSING_MODE_TypeDef mode);
void oled_ssd1306_set_column_address(oled_ssd1306 *self, uint8_t column_start, uint8_t column_end);
void oled_ssd1306_set_page_address(oled_ssd1306 *self, uint8_t page_start, uint8_t page_end);

/**
 * Funciones para dibujar en la pantalla
 */
void oled_ssd1306_draw_bitmap(oled_ssd1306 *self, uint8_t *bitmap, uint16_t height, uint16_t width);
void oled_ssd1306_DrawPixel(oled_ssd1306 *self, uint8_t x, uint8_t y, SSD1306_COLOR color);
void oled_ssd1306_test_all_pixel(oled_ssd1306 *self);
// char oled_ssd1306_WriteChar(oled_ssd1306 *self, char ch, FontDef Font, SSD1306_COLOR color);
// char oled_ssd1306_WriteString(oled_ssd1306 *self, char* str, FontDef Font, SSD1306_COLOR color);
// void oled_ssd1306_SetCursor(oled_ssd1306 *self, uint8_t x, uint8_t y);
// void oled_ssd1306_Line(oled_ssd1306 *self, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);
// void oled_ssd1306_DrawArc(oled_ssd1306 *self, uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color);
// void oled_ssd1306_DrawCircle(oled_ssd1306 *self, uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_COLOR color);
// void oled_ssd1306_Polyline(oled_ssd1306 *self, const SSD1306_VERTEX *par_vertex, uint16_t par_size, SSD1306_COLOR color);
// void oled_ssd1306_DrawRectangle(oled_ssd1306 *self, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);

// Low-level procedures
//void oled_ssd1306_Reset(oled_ssd1306 *self);
//void oled_ssd1306_WriteCommand(oled_ssd1306 *self, uint8_t byte);
//void oled_ssd1306_WriteData(oled_ssd1306 *self, uint8_t* buffer, size_t buff_size);

#endif