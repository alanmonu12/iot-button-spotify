#include "oled-freeRTOS.h"

static const char *TAG = "Display Task";

void DisplayTask (void *arg) {

    oled_ssd1306 display;

    oled_ssd1306_create(&display, SSD1306_I2C_ADDR);
    oled_ssd1306_Init(&display);
    
    ESP_LOGI(TAG, "Display initialzed");
    for(;;) {
        ESP_LOGI(TAG, "Display Task");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}