#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include "stdlib.h"
#include "esp_log.h"
#include "oled-ssd1306-data.h"
#include "oled-ssd1306-font.h"


typedef struct oled_ssd1306 oled_ssd1306;

struct oled_ssd1306 {
    uint8_t address;
    SSD1306_t display;
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
    void* handler;
};

// Procedure definitions
void oled_ssd1306_create(oled_ssd1306* self, uint8_t address);
void oled_ssd1306_Init(oled_ssd1306 *self);
void oled_ssd1306_Fill(oled_ssd1306 *self, SSD1306_COLOR color);
void oled_ssd1306_UpdateScreen(oled_ssd1306 *self);
// void oled_ssd1306_DrawPixel(oled_ssd1306 *self, uint8_t x, uint8_t y, SSD1306_COLOR color);
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