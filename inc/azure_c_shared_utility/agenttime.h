// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/** @file agenttime.h
*	@brief Function prototypes for time related functions.
*
*	@details These functions are implemented with C standard functions,
*	and therefore they are platform independent. But then a platform
*	can replace these functions with its own implementation as necessary.
*/

#ifndef AGENTTIME_H
#define AGENTTIME_H

#include <time.h>
#include "azure_c_shared_utility/umock_c_prod.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Get current calendar time.
*
*	@details This function provides nearly the same functionality as the
*	standard C @c time() function. The difference is that this function
*   is required to return epoch time in seconds. Nearly all implementations
*   do this, but the C standard does not require it.
*/
MOCKABLE_FUNCTION(, time_t, get_time, time_t*, currentTime);


/** @brief Gets the difference in seconds between @c stopTime and
*	@c startTime.
*
*	@details This function provides the same functionality as the
*	standard C @c difftime() function.
*/
MOCKABLE_FUNCTION(, double, get_difftime, time_t, stopTime, time_t, startTime);

#ifdef __cplusplus
}
#endif

#endif  // AGENTTIME_H
