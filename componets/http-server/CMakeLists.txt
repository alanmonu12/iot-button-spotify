idf_component_register(SRCS "http-server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_eth mdns esp_http_server)
