// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include "azure_c_shared_utility/option_store.h"
#include "azure_c_shared_utility/xlogging.h"

void option_store_safe_invoke_delete(OPTION_STORE* store)
{
    if (store != NULL && store->delete_option_data != NULL)
    {
        store->delete_option_data(store->option_data);
    }
}


