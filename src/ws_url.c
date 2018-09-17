// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include "azure_c_shared_utility/ws_url.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/string_token.h"

#define WS_PROTOCOL  "ws://"
#define WSS_PROTOCOL "wss://"

static int parse_ws_url(const char* url, WS_URL* ws_url)
{
    int result = 0;
    size_t url_length = strlen(url);

    if (url_length < 7)
    {
        LogError("Invalid url (unexpected length)");
        result = __FAILURE__;
    }
    // Codes_SRS_WS_URL_09_004: [ If `url` starts with "ws://" (`protocol`), `ws_url->is_secure` shall be set to false ]
    if (strncmp(url, WS_PROTOCOL, 5) == 0)
    {
        ws_url->is_secure = false;
    }
    // Codes_SRS_WS_URL_09_005: [ If `url` starts with "wss://" (`protocol`), `ws_url->is_secure` shall be set to true ]
    else if (strncmp(url, WSS_PROTOCOL, 6) == 0)
    {
        ws_url->is_secure = true;
    }
    else
    {
        // Codes_SRS_WS_URL_09_024: [ If `protocol` cannot be identified in `url`, the function shall fail and return NULL ]
        LogError("Url protocol prefix not recognized");
        result = __FAILURE__;
    }

    if (result == 0)
    {
        size_t host_begin = (ws_url->is_secure ? 6 : 5);

        char* port_delimiter = ":";
        char* path_delimiter = "/";
        char* query_delimiter = "?";
        
        char* delimiters1[3];
        delimiters1[0] = port_delimiter; 
        delimiters1[1] = path_delimiter;
        delimiters1[2] = query_delimiter;
        
        char* delimiters2[1];
        delimiters2[0] = query_delimiter;
        
        size_t delimiter_count = 3;
        char** current_delimiters = delimiters1;
        char* previous_delimiter = NULL;

        bool host_parsed = false;
        bool port_parsed = false;
        bool path_parsed = false;
        bool query_parsed = false;

        STRING_TOKEN_HANDLE token = StringToken_GetFirst(url + host_begin, url_length - host_begin, current_delimiters, delimiter_count);

        if (token == NULL)
        {
            LogError("Failed getting first url token");
            result = __FAILURE__;
        }
        else
        {
            while (token != NULL)
            {
                char* current_delimiter = (char*)StringToken_GetDelimiter(token);

                if (previous_delimiter == NULL && !host_parsed && !port_parsed && !path_parsed && !query_parsed)
                {
                    // Codes_SRS_WS_URL_09_006: [ The pointer to the token starting right after `protocol` (in the `url` string) shall be stored in `ws_url->host` ]
                    ws_url->host = (char*)StringToken_GetValue(token);
                    // Codes_SRS_WS_URL_09_008: [ The length from `ws_url->host` up to the first occurrence of either ":" (`port_delimiter`), "/" (`path_delimiter`), "?" (`query_delimiter`) or `\0` shall be stored in `ws_url->host_length` ]
                    ws_url->host_length = StringToken_GetLength(token);

                    // Codes_SRS_WS_URL_09_007: [ If `ws_url->host` ends up being NULL, the function shall fail and return NULL ]
                    // Codes_SRS_WS_URL_09_009: [ If `ws_url->host_length` ends up being zero, the function shall fail and return NULL ]
                    if (ws_url->host == NULL || ws_url->host_length == 0)
                    {
                        LogError("Failed parsing websocket url host");
                        result = __FAILURE__;
                        StringToken_Destroy(token);
                        break;
                    }
                    else
                    {
                        host_parsed = true;
                    }
                }
                // Codes_SRS_WS_URL_09_010: [ If after `ws_url->host` the `port_delimiter` occurs (not preceeded by `path_delimiter` or `query_delimiter`) the number that follows shall be parsed and stored in `ws_url->port` ]
                else if (previous_delimiter == port_delimiter && host_parsed && !port_parsed && !path_parsed && !query_parsed)
                {
                    char* port = (char*)StringToken_GetValue(token);
                    size_t port_length = StringToken_GetLength(token);

                    // Codes_SRS_WS_URL_09_011: [ If the port number fails to be parsed, the function shall fail and return NULL ]
                    if (port == NULL || port_length == 0)
                    {
                        LogError("Failed parsing websocket url port");
                        result = __FAILURE__;
                        StringToken_Destroy(token);
                        break;
                    }
                    else
                    {
                        char port_copy[10];
                        (void*)memset(port_copy, 0, sizeof(char) * 10);
                        (void*)memcpy(port_copy, port, port_length);

                        ws_url->port = (size_t)atoi(port_copy);

                        port_parsed = true;
                    }
                }
                // Codes_SRS_WS_URL_09_012: [ If after `ws_url->host` or the port number the `path_delimiter` occurs (not preceeded by `query_delimiter`) the following pointer address shall be stored in `ws_url->path` ]
                else if (previous_delimiter == path_delimiter && host_parsed && !path_parsed && !query_parsed)
                {
                    ws_url->path = (char*)StringToken_GetValue(token);
                    // Codes_SRS_WS_URL_09_014: [ The length from `ws_url->path` up to the first occurrence of either `query_delimiter` or `\0` shall be stored in `ws_url->path_length` ]
                    ws_url->path_length = StringToken_GetLength(token);

                    // Codes_SRS_WS_URL_09_013: [ If the path component is present and `ws_url->path` ends up being NULL, the function shall fail and return NULL ]
                    // Codes_SRS_WS_URL_09_015: [ If the path component is present and `ws_url->path_length` ends up being zero, the function shall fail and return NULL ]
                    if (ws_url->path == NULL || ws_url->path_length == 0)
                    {
                        LogError("Failed parsing websocket url path");
                        result = __FAILURE__;
                        StringToken_Destroy(token);
                        break;
                    }
                    else
                    {
                        path_parsed = true;
                    }
                }
                // Codes_SRS_WS_URL_09_016: [ Next if the `query_delimiter` occurs the following pointer address shall be stored in `ws_url->query` ]
                else if (previous_delimiter == query_delimiter && current_delimiter == NULL && host_parsed && !query_parsed)
                {
                    ws_url->query = (char*)StringToken_GetValue(token);
                    // Codes_SRS_WS_URL_09_018: [ The length from `ws_url->query` up to `\0` shall be stored in `ws_url->query_length` ]
                    ws_url->query_length = StringToken_GetLength(token);

                    // Codes_SRS_WS_URL_09_017: [ If the query component is present and `ws_url->query` ends up being NULL, the function shall fail and return NULL ]
                    // Codes_SRS_WS_URL_09_019: [ If the query component is present and `ws_url->query_length` ends up being zero, the function shall fail and return NULL ]
                    if (ws_url->query == NULL || ws_url->query_length == 0)
                    {
                        LogError("Failed parsing websocket url query");
                        result = __FAILURE__;
                        StringToken_Destroy(token);
                        break;
                    }
                    else
                    {
                        query_parsed = true;
                    }
                }
                else
                {
                    LogError("Failed parsing websocket url (format not recognized)");
                    result = __FAILURE__;
                    StringToken_Destroy(token);
                    break;
                }

                if (current_delimiter == path_delimiter)
                {
                    current_delimiters = delimiters2;
                    delimiter_count = 1;
                }

                previous_delimiter = current_delimiter;

                token = StringToken_GetNext(token, current_delimiters, delimiter_count);
            }
        }
    }

    return result;
}

void ws_url_destroy(WS_URL* url)
{
    // Codes_SRS_WS_URL_09_022: [ If `url` is NULL, the function shall return without further action ]
    if (url != NULL)
    {
        // Codes_SRS_WS_URL_09_023: [ Otherwise, the memory allocated for `url` shall released ]
        free(url);
    }
}

WS_URL* ws_url_create(const char* url)
{
    WS_URL* result;

    // Codes_SRS_WS_URL_09_002: [ Memory shall be allocated for an instance of WS_URL (aka `ws_url`) ]
    if ((result = malloc(sizeof(WS_URL))) == NULL)
    {
        // Codes_SRS_WS_URL_09_003: [ If `ws_url` failed to be allocated, the function shall return NULL ]
        LogError("Failed to allocate the websockets url");
    }
    else
    {
        memset(result, 0, sizeof(WS_URL));

        // Codes_SRS_WS_URL_09_001: [ If `url` is NULL, `ws_url` shall be returned empty, without any parsing ]
        if (url != NULL)
        {
            if (parse_ws_url(url, result) != 0)
            {
                // Codes_SRS_WS_URL_09_020: [ If any component cannot be parsed or is out of order, the function shall fail and return NULL ]
                // Codes_SRS_WS_URL_09_021: [ If any failure occurs, all memory allocated by the function shall be released before returning ]
                LogError("Failed parsing the websockets url");
                ws_url_destroy(result);
                result = NULL;
            }
        }
    }

    return result;
}
