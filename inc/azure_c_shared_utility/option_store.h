// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef OPTION_STORE_H
#define OPTION_STORE_H

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*OPTION_STORE_H*/
