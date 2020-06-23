#include "oled-freeRTOS.h"
#include "Spotify_icon.h"

static const char *TAG = "Display Task";

// static const uint8_t image_data_Test[32] = {
//     // ████████████████
//     // ████████████████
//     // ████████████████
//     // ████████████████
//     // ████████████████
//     // ████████████████
//     // ████████████████
//     // █∙∙∙∙∙∙∙∙███████
//     // ████████████████
//     // █∙█∙∙∙∙█∙███████
//     // █∙█∙████∙███████
//     // █∙████∙█∙███████
//     // █∙█∙∙∙██∙███████
//     // ████████∙███████
//     // █∙∙∙∙∙∙∙████████
//     // ████████████████
//     0xff, 0xff, 
//     0xff, 0xff, 
//     0xff, 0xff, 
//     0xff, 0xff, 
//     0xff, 0xff, 
//     0xff, 0xff, 
//     0xff, 0xff, 
//     0x80, 0x7f, 
//     0xff, 0xff, 
//     0xa1, 0x7f, 
//     0xaf, 0x7f, 
//     0xbd, 0x7f, 
//     0xa3, 0x7f, 
//     0xff, 0x7f, 
//     0x80, 0xff, 
//     0xff, 0xff
// };

void DisplayTask (void *arg) {

    oled_ssd1306 oled;

    ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
    
    oled_ssd1306_create(&oled, SSD1306_I2C_ADDR);
    oled_ssd1306_Init(&oled);

    oled_ssd1306_draw_bitmap(&oled, image_data_spo, 128, 32, 0, 0);
    oled_ssd1306_UpdateScreen(&oled);
    
    ESP_LOGI(TAG, "Display %s", oled.display.Initialized? "Initialized": "Not Initialized");
    for(;;) {

    }
}