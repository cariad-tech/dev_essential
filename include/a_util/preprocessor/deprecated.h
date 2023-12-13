/**
 * @file
 * Definition of preprocessor macro @ref DEV_ESSENTIAL_DEPRECATED
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_PREPROCESSOR_DEPRECATED_HEADER_INCLUDED
#define A_UTIL_UTIL_PREPROCESSOR_DEPRECATED_HEADER_INCLUDED

#ifndef DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS
/**
 * Adds <tt>[[deprecated("msg")]]</tt> to allowed declarations
 * @param[in] msg Text explaining the rationale for deprecation and/or to suggest a replacing entity
 * @remark Might be disabled by defining preprocessor define
 *         @ref DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS.
 * @remark Allowed declarations are listed here:
           https://en.cppreference.com/w/cpp/language/attributes/deprecated#Explanation.
 * @ingroup a_util_preprocessor
 */
#define DEV_ESSENTIAL_DEPRECATED(msg)                                                              \
    [[deprecated(msg " To disable this warning, define "                                           \
                     "DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS.")]]
#else // DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS
#define DEV_ESSENTIAL_DEPRECATED(_)
#endif // DEV_ESSENTIAL_DISABLE_DEPRECATED_WARNINGS

#endif // A_UTIL_UTIL_PREPROCESSOR_DEPRECATED_HEADER_INCLUDED
