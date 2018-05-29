// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef OPTION_STORE_H
#define OPTION_STORE_H

#include "azure_c_shared_utility/xio.h"
#include "azure_c_shared_utility/optionhandler.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// An opaque pointer to arbitrary structured option data
typedef struct OPTION_STORE_DATA_HANDLE_TAG* OPTION_STORE_DATA_HANDLE;

// A function that knows how to free the option_data
typedef void(*pfOptionStoreDestroy)(OPTION_STORE_DATA_HANDLE);

typedef struct OPTION_STORE_TAG
{
    OPTION_STORE_DATA_HANDLE option_data;
    pfOptionStoreDestroy delete_option_data;
} OPTION_STORE;

void option_store_safe_invoke_delete(OPTION_STORE* store);

// A function that knows how to set a particular option
typedef int(*pfOptionStoreSetter)(void* handle, const void* value);

// A pairing of an option name and a function that sets the option
typedef struct OPTION_STORE_NAME_AND_SETTER_PAIR_TAG
{
    const char* const* name;
    pfOptionStoreSetter setter;
} OPTION_STORE_NAME_AND_SETTER_PAIR;

#define OPTION_STORE_PAIRS_COUNT  (sizeof(option_store_setters) / sizeof(OPTION_STORE_NAME_AND_SETTER_PAIR))

// Iterates through the name/setter pairs and sets the option value appropriately
int option_store_set_option(CONCRETE_IO_HANDLE xio, const char* name, const void* value, CONCRETE_IO_HANDLE downstream_xio,
    OPTION_STORE_NAME_AND_SETTER_PAIR* pairs, size_t pair_count);

// TODO: Remove this function when optionhandler goes away
OPTIONHANDLER_HANDLE option_store_get_empty_option_handler();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*OPTION_STORE_H*/
