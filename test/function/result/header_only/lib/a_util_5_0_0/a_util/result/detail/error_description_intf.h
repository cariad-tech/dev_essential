/**
 * @file
 * Declare an interface class for error description class for ADTF customers that use tResult return
 * values.
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

#ifndef A_UTIL_UTIL_RESULT_DETAIL_ERROR_DESCRIPTION_INTERFACE_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_ERROR_DESCRIPTION_INTERFACE_HEADER_INCLUDED

#include "a_util/base/std/types.h"

namespace a_util {
namespace result {
namespace detail {

/**
 * @brief The internal interface for binary compatibility of error result descritpions
 */
class IErrorDescription {
public:
    virtual const std::int32_t getErrorCode() const = 0;
    virtual const std::int32_t getLine() const = 0;
    virtual const char* getErrorDescription() const = 0;
    virtual const char* getFileName() const = 0;
    virtual const char* getFunctionName() const = 0;
    virtual const char* getErrorCodeLabel() const = 0;

protected:
    ~IErrorDescription()
    {
    }
};

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_DETAIL_ERROR_DESCRIPTION_INTERFACE_HEADER_INCLUDED
