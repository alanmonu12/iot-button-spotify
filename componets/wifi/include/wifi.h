#ifndef WIFI_H
#define WIFI_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"

#include <sys/param.h>

#include "esp_netif.h"
#include "esp_eth.h"

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define ESP_WIFI_SSID      "INFINITUM526A_2.4"
#define ESP_WIFI_PASS      "9Yux5gpaP7"
//#define ESP_WIFI_SSID      "Redmi"
//#define ESP_WIFI_PASS      "81939cb1ecb5"
#define ESP_MAXIMUM_RETRY  3

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAIL_BIT           BIT1
#define WIFI_DISCONNECTED_BIT   BIT2
#define WIFI_ENABLE_COFIG       BIT3

TaskHandle_t xWifiTask;

/**
 * Task para el wifi
 */
void wifi_Task (void *arg);

#endif