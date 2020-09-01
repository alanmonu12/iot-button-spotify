// TODO: Solo se debe inicar el el mDNS y el servidor web cuando no se tenga un token de acceso
/* Iniciamos el mDNS para poder inicar sesion del dispositvo en la cuenta del usuario*/
//http_server_mdns_start();
/* Start the server for the first time */
//server = start_webserver();
#include "spotify-client-freeRTOS.h"

static const char *TAG = "Spotify client Task";

void spotifyClientTask(void *arg)
{
    httpd_handle_t server = NULL;
    /**
     * Estructura que representa al cliente de spotify
     */
    spotifyClient_TypeDef spotify_client;
    /**
     * Ponemos la tarea de block state hasta que el WiFi se 
     * conecte de manera correcta
     * TODO: Es necesario saber que pasara con la tarea cuando
     * no se cuente con conexión WiFi
     */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    /**
     * Inicializamos los valores del cliente
     */
    spotifyClient_init(&spotify_client);

    /**
     * Revisamos si tenemos un token valido para acceder a la API
     */
    if(!spotify_client.auth.token.status) {
        ESP_LOGI(TAG, "No se tiene token de acceso");
        /**
         * Si no tenemos token de acceso iniciamos el servidor
         */

        http_server_mdns_start();

        server = start_webserver();
    }

    for (;;)
    {
        /* To do*/
    }
}