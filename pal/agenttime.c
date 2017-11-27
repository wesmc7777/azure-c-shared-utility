// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "azure_c_shared_utility/gballoc.h"

#include <time.h>
#include "azure_c_shared_utility/agenttime.h"

time_t get_time(time_t* p)
{
    return time(p);
}

double get_difftime(time_t stopTime, time_t startTime)
{
    return difftime(stopTime, startTime);
}
