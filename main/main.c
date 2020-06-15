#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_err.h"

#include <sys/param.h>

#include "esp_netif.h"
#include "esp_eth.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "../componets/wifi/include/wifi.h"
#include "oled-freeRTOS.h"

/* Handles for the tasks create by main(). */
//static TaskHandle_t xServerTask = NULL;
//static TaskHandle_t xSpotifyTask = NULL;
const char *TAG = "Main app";


void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    xTaskCreatePinnedToCore(wifi_Task, "wifi_task", 2048*2, NULL, 10, &xWifiTask, 1);
    xTaskCreatePinnedToCore(DisplayTask, "DisplayTask", 2048*2, NULL, 10, &xDisplayTask, 0);
    //xTaskCreatePinnedToCore(server_Task, "server_task", 2048, NULL, 10, &xServerTask, 1);
    //xTaskCreatePinnedToCore(spotify_client_task, "spotify_client_task", 2048, NULL, 10, &xSpotifyTask, 0);
    
}

