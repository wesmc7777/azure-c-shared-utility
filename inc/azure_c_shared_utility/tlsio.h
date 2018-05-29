// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TLSIO_H
#define TLSIO_H

#include "xio.h"
#include "azure_c_shared_utility/option_store.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct TLSIO_CONFIG_TAG
{
    const char* hostname;
    int port;
    const IO_INTERFACE_DESCRIPTION* underlying_io_interface;
    void* underlying_io_parameters;
    OPTION_STORE* option_store;
} TLSIO_CONFIG;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TLSIO_H */
