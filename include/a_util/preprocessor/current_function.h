/**
 * @file
 * Definition of preprocessor macro @ref A_UTIL_CURRENT_FUNCTION
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_PREPROCESSOR_CURRENT_FUNCTION_HEADER_INCLUDED
#define A_UTIL_UTIL_PREPROCESSOR_CURRENT_FUNCTION_HEADER_INCLUDED

#include <3rdparty/boost/boost_1_64_0/boost/current_function.hpp>

/**
 * @def A_UTIL_CURRENT_FUNCTION
 * Resolves to the name of the function this macro is used in.
 * @ingroup a_util_preprocessor
 */
#ifndef A_UTIL_CURRENT_FUNCTION
#define A_UTIL_CURRENT_FUNCTION BOOST_CURRENT_FUNCTION
#endif // !A_UTIL_CURRENT_FUNCTION

#endif // A_UTIL_UTIL_PREPROCESSOR_CURRENT_FUNCTION_HEADER_INCLUDED
