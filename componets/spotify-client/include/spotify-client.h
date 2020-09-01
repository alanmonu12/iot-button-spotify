#ifndef SPOTIFY_CLIENT_H
#define SPOTIFY_CLIENT_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"


#include "spotify-client-data.h"
#include "http-server.h"

/**
 * Funcion para incializar el cliente de spotify
 */
void spotifyClient_init(spotifyClient_TypeDef* self);

/**
 * Funcion para obtener un token de acceso
 */
void spotifyClient_get_token(spotifyClient_TypeDef* self);

#endif