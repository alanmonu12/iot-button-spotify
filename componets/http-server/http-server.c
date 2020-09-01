#include "http-server.h"

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static const char *TAG = "example";

/* An HTTP GET handler */
static esp_err_t auth_get_handler(httpd_req_t *req)
{
    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    /* Enviamos una respuesta que redirecciona al usuario a spotify*/
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_status(req, "301 Moved Permanently");
    httpd_resp_set_hdr(req, "Location", "https://accounts.spotify.com/authorize?client_id=e1c01b892437488aa0921f5679a91752&response_type=code&redirect_uri=http://esp32-spotify.local/callback&scope=user-read-playback-state&show_dialog=true");
    httpd_resp_send(req, NULL, 0);

    /* After sending the HTTP response the old HTTP request
    * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    
    return ESP_OK;
}



/* An HTTP GET handler */
static esp_err_t callback_get_handler(httpd_req_t *req)
{
    bool status_callback = false;
    char*  buf;
    size_t buf_len;  

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[220];
            /* Si llego el codigo se guarda */
            if (httpd_query_key_value(buf, "code", param, sizeof(param)) == ESP_OK) {
                status_callback = true;
                ESP_LOGI(TAG, "Found URL query parameter => code=%s", param);
            }
            /* Si no se concedieron permisos */
            else if(httpd_query_key_value(buf, "error", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => error=%s", param);
            }
        }
        free(buf);
    }


    if(status_callback) {
        /* Send response with custom headers and body set as the
         * string passed in user context*/
        const char resp_str[] = "Ya puede usar tu dispositivo";
        httpd_resp_send(req, resp_str, sizeof(resp_str));        
    }
    else {
        /* Send response with custom headers and body set as the
         * string passed in user context*/
        const char resp_str[] = "Debes aceptar los permisos";
        httpd_resp_send(req, resp_str, sizeof(resp_str)); 
    }

    
    /* After sending the HTTP response the old HTTP request
    * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    
    return ESP_OK;
}

static const httpd_uri_t callback = {
    .uri       = "/callback",
    .method    = HTTP_GET,
    .handler   = callback_get_handler
};

/* This handler allows the custom error handling functionality to be
 * tested from client side. For that, when a PUT request 0 is sent to
 * URI /ctrl, the /hello and /echo URIs are unregistered and following
 * custom error handler http_404_error_handler() is registered.
 * Afterwards, when /hello or /echo is requested, this custom error
 * handler is invoked which, after sending an error message to client,
 * either closes the underlying socket (when requested URI is /echo)
 * or keeps it open (when requested URI is /hello). This allows the
 * client to infer if the custom error handler is functioning as expected
 * by observing the socket state.
 */
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } else if (strcmp("/echo", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        //httpd_register_uri_handler(server, &auth);
        httpd_register_uri_handler(server, &callback);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}

void disconnect_handler(void* arg)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

void connect_handler(void* arg)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}


void http_server_mdns_start(void)
{

    char* hostname = "esp32-spotify";
    //initialize mDNS
    ESP_ERROR_CHECK( mdns_init() );
    //set mDNS hostname (required if you want to advertise services)
    ESP_ERROR_CHECK( mdns_hostname_set(hostname) );
    ESP_LOGI(TAG, "mdns hostname set to: [%s]", hostname);
    //set default mDNS instance name
    ESP_ERROR_CHECK( mdns_instance_name_set("Spotify-button") );

    //structure with TXT records
    mdns_txt_item_t serviceTxtData[3] = {
        {"board","esp32"}
    };

    //initialize service
    ESP_ERROR_CHECK( mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData, 1) );
    //add another TXT item
    ESP_ERROR_CHECK( mdns_service_txt_item_set("_http", "_tcp", "path", "/auth") );
    //add another TXT item
    ESP_ERROR_CHECK(mdns_service_txt_item_set("_http", "_tcp", "path", "/callback"));
}
