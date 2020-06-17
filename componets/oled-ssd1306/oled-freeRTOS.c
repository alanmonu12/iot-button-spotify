#include "oled-freeRTOS.h"

static const char *TAG = "Display Task";

void DisplayTask (void *arg) {

    oled_ssd1306 oled;

    ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
    
    oled_ssd1306_create(&oled, SSD1306_I2C_ADDR);
    oled_ssd1306_Init(&oled);
    oled_ssd1306_test_all_pixel(&oled);
    
    ESP_LOGI(TAG, "Display %s", oled.display.Initialized? "Initialized": "Not Initialized");
    for(;;) {

    }
}