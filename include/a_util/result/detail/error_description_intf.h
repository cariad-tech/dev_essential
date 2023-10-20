/**
 * @file
 * Private API for @ref a_util::result::detail::IErrorDescription "IErrorDescription"
 * type
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_DETAIL_ERROR_DESCRIPTION_INTERFACE_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_ERROR_DESCRIPTION_INTERFACE_HEADER_INCLUDED

#include <cstdint>

namespace a_util {
namespace result {
namespace detail {
/**
 * The internal interface to ensure binary compatibility of error result descriptions
 * @warning This interface must not be changed to ensure binary compatibility!
 */
class IErrorDescription {
public:
    /**
     * Interface method to get the error code.
     * @return Error code.
     */
    virtual std::int32_t getErrorCode() const = 0;

    /**
     * Interface method to get the line of the source file where the error was reported.
     * @return Line number.
     */
    virtual std::int32_t getLine() const = 0;

    /**
     * Interface method to get the error code as string representation.
     * @return Error code represented as string.
     */
    virtual const char* getErrorDescription() const = 0;

    /**
     * Interface method to get the name of the file the error was reported in.
     * @return File name as string.
     */
    virtual const char* getFileName() const = 0;

    /**
     * Interface method to get the name of the function the error was reported in.
     * @return Function name as string.
     */
    virtual const char* getFunctionName() const = 0;

    /**
     * Interface method to get the error code label.
     * @return Error code as an string object.
     */
    virtual const char* getErrorCodeLabel() const = 0;

protected:
    /// DTOR
    ~IErrorDescription()
    {
    }
};

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_DETAIL_ERROR_DESCRIPTION_INTERFACE_HEADER_INCLUDED
