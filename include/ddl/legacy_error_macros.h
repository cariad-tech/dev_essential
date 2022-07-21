/**
 * @file
 * This header provides the legacy error macros.
 * Important: This header may not be included in ddl.h!
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef LEGACY_ERROR_MACROS_H_
#define LEGACY_ERROR_MACROS_H_

#include <a_util/result/result_info.h>
#include <a_util/result/result_type.h>

/**
 * @brief legacy error macro
 * @param s the expression
 *
 */
#define RETURN_IF_FAILED(s)                                                                        \
    {                                                                                              \
        a_util::result::Result _errcode(s);                                                        \
        if (a_util::result::isFailed(_errcode)) {                                                  \
            return (_errcode);                                                                     \
        }                                                                                          \
    }

#endif // LEGACY_ERROR_MACROS_H_
