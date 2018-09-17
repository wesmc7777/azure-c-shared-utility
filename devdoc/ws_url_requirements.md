STRING TOKENIZER Requirements
================

## Overview
The WS_URL provides functionality parse Websocket URLs.


## Exposed API
```C
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

extern WS_URL* ws_url_create(const char* url);
extern void ws_url_destroy(WS_URL* url);
```


###  ws_url_create

```c
extern WS_URL* ws_url_create(const char* url);
```

**SRS_WS_URL_09_002: [** Memory shall be allocated for an instance of WS_URL (aka `ws_url`) **]**

**SRS_WS_URL_09_003: [** If `ws_url` failed to be allocated, the function shall return NULL **]**

**SRS_WS_URL_09_001: [** If `url` is NULL, `ws_url` shall be returned empty, without any parsing **]**


#### Parsing starts

**SRS_WS_URL_09_004: [** If `url` starts with "ws://" (`protocol`), `ws_url->is_secure` shall be set to false **]**

**SRS_WS_URL_09_005: [** If `url` starts with "wss://" (`protocol`), `ws_url->is_secure` shall be set to true **]**

**SRS_WS_URL_09_024: [** If `protocol` cannot be identified in `url`, the function shall fail and return NULL **]**

**SRS_WS_URL_09_006: [** The pointer to the token starting right after `protocol` (in the `url` string) shall be stored in `ws_url->host` **]**

**SRS_WS_URL_09_007: [** If `ws_url->host` ends up being NULL, the function shall fail and return NULL **]**

**SRS_WS_URL_09_008: [** The length from `ws_url->host` up to the first occurrence of either ":" (`port_delimiter`), "/" (`path_delimiter`), "?" (`query_delimiter`) or `\0` shall be stored in `ws_url->host_length` **]**

**SRS_WS_URL_09_009: [** If `ws_url->host_length` ends up being zero, the function shall fail and return NULL **]**

**SRS_WS_URL_09_010: [** If after `ws_url->host` the `port_delimiter` occurs (not preceeded by `path_delimiter` or `query_delimiter`) the number that follows shall be parsed and stored in `ws_url->port` **]**

**SRS_WS_URL_09_011: [** If the port number fails to be parsed, the function shall fail and return NULL **]**

**SRS_WS_URL_09_012: [** If after `ws_url->host` or the port number the `path_delimiter` occurs (not preceeded by `query_delimiter`) the following pointer address shall be stored in `ws_url->path` **]**

**SRS_WS_URL_09_013: [** If the path component is present and `ws_url->path` ends up being NULL, the function shall fail and return NULL **]**

**SRS_WS_URL_09_014: [** The length from `ws_url->path` up to the first occurrence of either `query_delimiter` or `\0` shall be stored in `ws_url->path_length` **]**

**SRS_WS_URL_09_015: [** If the path component is present and `ws_url->path_length` ends up being zero, the function shall fail and return NULL **]**

**SRS_WS_URL_09_016: [** Next if the `query_delimiter` occurs the following pointer address shall be stored in `ws_url->query` **]**

**SRS_WS_URL_09_017: [** If the query component is present and `ws_url->query` ends up being NULL, the function shall fail and return NULL **]**

**SRS_WS_URL_09_018: [** The length from `ws_url->query` up to `\0` shall be stored in `ws_url->query_length` **]**

**SRS_WS_URL_09_019: [** If the query component is present and `ws_url->query_length` ends up being zero, the function shall fail and return NULL **]**

**SRS_WS_URL_09_020: [** If any component cannot be parsed or is out of order, the function shall fail and return NULL **]**


#### Finally

**SRS_WS_URL_09_021: [** If any failure occurs, all memory allocated by the function shall be released before returning **]**


###  ws_url_destroy

```c
extern void ws_url_destroy(WS_URL* url);
```

**SRS_WS_URL_09_022: [** If `url` is NULL, the function shall return without further action **]**

**SRS_WS_URL_09_023: [** Otherwise, the memory allocated for `url` shall released **]**
