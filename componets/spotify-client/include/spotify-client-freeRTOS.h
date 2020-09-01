#ifndef SPOTIFY_CLIENT_FREERTOS_H
#define SPOTIFY_CLIENT_FREERTOS_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"

#include "spotify-client.h"

TaskHandle_t xSpotifyClientTask;

void spotifyClientTask(void *arg);

#endif