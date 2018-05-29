// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdint.h>
#include "azure_c_shared_utility/option_store.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/crt_abstractions.h"

void option_store_safe_invoke_delete(OPTION_STORE* store)
{
    if (store != NULL && store->delete_option_data != NULL)
    {
        store->delete_option_data(store->option_data);
    }
}

int option_store_set_option(CONCRETE_IO_HANDLE xio, const char* optionName, void* value, CONCRETE_IO_HANDLE downstream_xio,
    OPTION_STORE_NAME_AND_SETTER_PAIR* pairs, size_t pair_count)
{
    int result;
    if (xio == NULL || optionName == NULL || pairs == NULL)
    {
        LogInfo("bad parameter");
        result = __FAILURE__;
    }
    else
    {
        size_t i;
        bool failed = false;
        for (i = 0; i < pair_count; i++)
        {
            if (strcmp(*pairs[i].name, optionName) == 0)
            {
                if (pairs[i].setter == NULL)
                {
                    LogInfo("null setter");
                    failed = true;
                }
                else
                {
                    result = pairs[i].setter(xio, value);
                }
                break;
            }
        }

        if (failed)
        {
            result = __FAILURE__;
        }
        else if (i < pair_count)
        {
            result = pairs[i].setter(xio, value);
        }
        else if (downstream_xio == NULL)
        {
            result = __FAILURE__;
            LogError("unsupported option");
        }
        else
        {
            result = xio_setoption(downstream_xio, optionName, value);
        }
    }

    return result;
}



