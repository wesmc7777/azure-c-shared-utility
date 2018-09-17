// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef WS_URL_H
#define WS_URL_H

#include "azure_c_shared_utility/umock_c_prod.h"

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

typedef struct WS_URL_TAG
{
    bool is_secure;
    
    char* host;
    size_t host_length;
    
    size_t port;
    
    char* path;
    size_t path_length;

    char* query;
    size_t query_length;
} WS_URL;

MOCKABLE_FUNCTION(, WS_URL*, ws_url_create, const char*, url);
MOCKABLE_FUNCTION(, void, ws_url_destroy, WS_URL*, url);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // WS_URL_H