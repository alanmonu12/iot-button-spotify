#ifndef SPOTIFY_CLIENT_DATA_H
#define SPOTIFY_CLIENT_DATA_H

#include "stdint.h"

#define SPOTIFY_CLIENT_BUFFER_LEN          512


/**
 * Tipos de token que se pueden usar para acceder a la API
 */
typedef enum {
    BEARER_TYPE = 0U,
}accessTokenType_TypeDef;

/**
 * Tipos de respuesta para la petición de access token
 */
typedef enum
{
    AUTH_CODE_TYPE = 0U,
}grantType_TypeDef;

/**
 * Tipos de acceso
 */
typedef enum
{
    CODE_TYPE = 0U,
}responseType_TypeDef;

/**
 * Estructura que representa al token de acceso que genera
 * spotify para poder hacer uso de su API
 */
typedef struct accessToken_TypeDef accessToken_TypeDef;

struct accessToken_TypeDef
{
    char accessToken[50];
    char refreshToke[50];
    accessTokenType_TypeDef access_token_type;
    uint32_t expire_in;
    bool status;
};

/**
 * Estructura para almacenar la informacion
 * de autenticación para el cliente de spotify
 */
typedef struct spotifyClientAuth_TypeDef spotifyClientAuth_TypeDef;

struct spotifyClientAuth_TypeDef
{
    char scope[50];
    char authCode[50];
    char clientId[20];
    char clientSecret[20];
    accessToken_TypeDef token;
    grantType_TypeDef grantType;
    responseType_TypeDef response_type;
};

/**
 * Estrucutra para tener alamcenar la información 
 * que se obtenga por medio de la API de spotify
 */
typedef struct spotifyClientData_TypeDef spotifyClientData_TypeDef;

struct spotifyClientData_TypeDef
{
    uint32_t progress_ms;
    uint32_t duration_ms;
    char artist_name[25];
    char song_title[25];
    bool is_playing;
    bool is_player_active;
};

/**
 * Estrucutra que represneta a todo el cliente de spotify
 */

typedef struct spotifyClient_TypeDef spotifyClient_TypeDef;

struct spotifyClient_TypeDef
{
    spotifyClientAuth_TypeDef auth;
    spotifyClientData_TypeDef data;
    char buffer[SPOTIFY_CLIENT_BUFFER_LEN];
};



#endif