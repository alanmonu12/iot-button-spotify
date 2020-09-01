idf_component_register(SRCS "spotify-client-freeRTOS.c" "spotify-client.c"
                    INCLUDE_DIRS "include"
                    REQUIRES http-server)
