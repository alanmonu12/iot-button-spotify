#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "mdns.h"
#include <esp_http_server.h>

void http_server_mdns_start(void);
void connect_handler(void* arg);
void disconnect_handler(void* arg);
void stop_webserver(httpd_handle_t server);
httpd_handle_t start_webserver(void);
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err);

#endif