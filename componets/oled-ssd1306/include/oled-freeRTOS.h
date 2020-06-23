#ifndef OLED_FREERTOS_H
#define OLED_FREERTOS_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"

#include "oled-ssd1306.h"


TaskHandle_t xDisplayTask;

/**
 * Task para la tarea de mostrar datos en el display
 */
void DisplayTask (void *arg);

#endif