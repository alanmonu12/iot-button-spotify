#include "spotify-client.h"


// const char redirect_uri[] = "http://esp8266.local/callback";
// const char host[] = "api.spotify.com";
// const int httpsPort = 443;

// /* Constants that aren't configurable in menuconfig */
// #define WEB_SERVER "api.spotify.com"
// #define WEB_PORT "443"
// #define WEB_URL "https://www.howsmyssl.com/a/check"

// static const char *TAG = "example";

// static const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
//     "Host: "WEB_SERVER"\r\n"
//     "User-Agent: esp-idf/1.0 esp32\r\n"
//     "\r\n";

// /********** Funciones estaticas ****************************/
// static void _https_get_request(char* buffer, size_t buffer_size, char* url)
// {
//     int ret, len;

//     esp_tls_cfg_t cfg = {
//         .crt_bundle_attach = esp_crt_bundle_attach,
//     };

//     struct esp_tls *tls = esp_tls_conn_http_new(WEB_URL, &cfg);

//     if(tls != NULL) {
//         ESP_LOGI(TAG, "Connection established...");
//     } else {
//         ESP_LOGE(TAG, "Connection failed...");
//         goto exit;
//     }

//     size_t written_bytes = 0;
//     do {
//         ret = esp_tls_conn_write(tls,
//                                     REQUEST + written_bytes,
//                                     strlen(REQUEST) - written_bytes);
//         if (ret >= 0) {
//             ESP_LOGI(TAG, "%d bytes written", ret);
//             written_bytes += ret;
//         } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
//             ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
//             goto exit;
//         }
//     } while(written_bytes < strlen(REQUEST));

//     ESP_LOGI(TAG, "Reading HTTP response...");

//     do
//     {
//         len = buffer_size - 1;
//         bzero(buffer, buffer_size);
//         ret = esp_tls_conn_read(tls, (char *)buffer, len);

//         if(ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ)
//             continue;

//         if(ret < 0)
//         {
//             ESP_LOGE(TAG, "esp_tls_conn_read  returned -0x%x", -ret);
//             break;
//         }

//         if(ret == 0)
//         {
//             ESP_LOGI(TAG, "connection closed");
//             break;
//         }

//         len = ret;
//         ESP_LOGD(TAG, "%d bytes read", len);
//         /* Print response directly to stdout as it is read */
//         for(int i = 0; i < len; i++) {
//             putchar(buffer[i]);
//         }
//     } while(1);

// exit:
//     esp_tls_conn_delete(tls);
//     putchar('\n'); // JSON output doesn't have a newline at end

//     static int request_count;
//     ESP_LOGI(TAG, "Completed %d requests", ++request_count);

//     for(int countdown = 10; countdown >= 0; countdown--) {
//         ESP_LOGI(TAG, "%d...", countdown);
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     ESP_LOGI(TAG, "Starting again!");
// }


void spotifyClient_init(spotifyClient_TypeDef* self) {

    /**
     * Iniciamos todos los valores del cliente
     */
    memset(self->data.artist_name, '\0', 25);
    memset(self->data.song_title, '\0', 25);
    self->data.duration_ms = 0;
    self->data.progress_ms = 0;
    
    memset(self->buffer, '\0', SPOTIFY_CLIENT_BUFFER_LEN);
    
    memset(self->auth.scope, '\0', 50);
    memset(self->auth.authCode, '\0', 50);
    memset(self->auth.clientId, '\0', 20);
    memset(self->auth.clientSecret, '\0', 20);
    self->auth.grantType = AUTH_CODE_TYPE;
    self->auth.response_type = CODE_TYPE;

    /**
     * TODO: Este paramaetro se debe leer de algun metodo de almacenamiento
     */
    memset(self->auth.token.accessToken, '\0', 50);
    /**
     * TODO: Este paramaetro se debe leer de algun metodo de almacenamiento
     */
    memset(self->auth.token.refreshToke, '\0', 50);
    /**
     * TODO: Este paramaetro se debe leer de algun metodo de almacenamiento
     */
    self->auth.token.expire_in = 0;
    self->auth.token.access_token_type = BEARER_TYPE;

    /**
     * TODO: Es necesario revisar si tenemos un token, de tenerlos 
     * se debe revisar si es vijente
     */
    self->auth.token.status = false;
}

/**
 * @brief 
 */
void spotifyClient_get_token(spotifyClient_TypeDef *self) {

    httpd_handle_t server = NULL;

    //ESP_ERROR_CHECK(esp_event_loop_create_default());

    http_server_mdns_start();

    server = start_webserver();

    while(1) {
        
    }
}
