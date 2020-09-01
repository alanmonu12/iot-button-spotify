#include "include/wifi.h"
#include "oled-freeRTOS.h"
#include "spotify-client-freeRTOS.h"

static const char *TAG = "wifi station";

static int s_retry_num = 0;

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static void event_wifi_task_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG,"WIFI_EVENT: WIFI_EVENT_STA_START");
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START) {
        ESP_LOGI(TAG,"WIFI_EVENT: WIFI_EVENT_AP_START");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG,"WIFI_EVENT: WIFI_EVENT_STA_DISCONNECTED");
        s_retry_num++;
        
        if(s_retry_num > ESP_MAXIMUM_RETRY) {
            ESP_LOGI(TAG,"WIFI_EVENT: WIFI_ENABLE_COFIG");
            xEventGroupSetBits(s_wifi_event_group, WIFI_ENABLE_COFIG);
        }
        else{
            xEventGroupSetBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);
        }
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        ESP_LOGI(TAG,"WIFI_EVENT: WIFI_EVENT_AP_STACONNECTED");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        ESP_LOGI(TAG,"WIFI_EVENT: WIFI_EVENT_AP_STADISCONNECTED");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG,"IP_EVENT: IP_EVENT_STA_GOT_IP");
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

static void wifi_ap_config(void) {

    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    wifi_config_t wifi_config_ap = {
        .ap = {
            .ssid = "spotify-iot-button",
            .ssid_len = strlen("spotify-iot-button"),
            .channel = 1,
            .password = "Mtx184356",
            .max_connection = 5,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    if (strlen("Mtx184356") == 0) {
        wifi_config_ap.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_ap));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void wifi_sta_config(void) {

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASS,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );
}

// Task for monitoring WiFi connection
void wifi_Task (void *arg) {


    static const char *TAG = "wifi_Task";

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* FreeRTOS event group to signal when we are connected*/
    s_wifi_event_group = xEventGroupCreate();

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_wifi_task_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_wifi_task_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_sta_config();
    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Loop for task*/
    for(;;) {
        /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
        * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                WIFI_CONNECTED_BIT | WIFI_FAIL_BIT | WIFI_DISCONNECTED_BIT | WIFI_ENABLE_COFIG,
                pdFALSE,
                pdFALSE,
                portMAX_DELAY);

        /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
        * happened. */
        if (bits & WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", ESP_WIFI_SSID, ESP_WIFI_PASS);
            /**
             * Cuando se obtiene la conexión con AP se liberan las tareas 
             */
            /* Send a notification to prvTask2(), bringing it out of the Blocked state. */
            xTaskNotifyGive( xDisplayTask );
            xTaskNotifyGive(xSpotifyClientTask);
        }
        else if (bits & WIFI_FAIL_BIT)
        {
            ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
            ESP_WIFI_SSID, ESP_WIFI_PASS);
        }
        else if (bits & WIFI_DISCONNECTED_BIT) {
            ESP_LOGI(TAG, "WIFI DISCONNECTED");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            esp_wifi_connect();
        }
        else if (bits & WIFI_ENABLE_COFIG) {
            ESP_LOGI(TAG, "WIFI CONFIG LIKE AP");
            wifi_ap_config();
            ESP_LOGI(TAG, "wifi_init_ap finished.");
        }
        else {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
        }

        /* Se limpian los bits del Event Group */
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT | WIFI_DISCONNECTED_BIT | WIFI_ENABLE_COFIG);
    }
}
