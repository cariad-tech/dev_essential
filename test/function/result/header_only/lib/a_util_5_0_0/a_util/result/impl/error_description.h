/**
 * @file
 * Declare the default error description class for @c Result return values
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_HEADER_INCLUDED

#include <a_util/base/std/types.h>
#include <a_util/result/detail/error_description_intf.h>

#include <string>

namespace a_util {
namespace result {
namespace detail {

class ErrorDescription : public IErrorDescription {
private:
    ErrorDescription& operator=(const ErrorDescription& other); //=delete; (const members)

public:
    /**
     * @brief Construct error description object with complete set of error information
     * @tparam ErrorCode Error code (to deduce specialized template to stringify the error code)
     * @param[in] error_code The The concrete error object
     * @param[in] error_description Complete description of the error
     * @param[in] line Line in source file the error was reported (default: -1)
     * @param[in] file Source file the error was reported in
     * @param[in] function Function the error was reported in
     * @param[in] error_label The error code as an string object
     */
    ErrorDescription(std::int32_t error_code,
                     const char* error_description,
                     std::int32_t line,
                     const char* file,
                     const char* function,
                     const char* error_label);

    /// DTOR
    ~ErrorDescription();

    /**
     * Copy Constructor
     * @param[in] other Other error description object to copy from
     */
    ErrorDescription(const ErrorDescription& other);

public:
    /**
     * @brief Get error code.
     * @return Error code.
     */
    const std::int32_t getErrorCode() const;

    /**
     * @brief Get line in source file where the error was reported.
     * @return Line number.
     */
    const std::int32_t getLine() const;

    /**
     * @brief Get error code as string representation.
     * @return Error code represented as string.
     */
    const char* getErrorDescription() const;

    /**
     * @brief Get name of the file the error was reported in.
     * @return File name as string.
     */
    const char* getFileName() const;

    /**
     * @brief Get name of the function the error was reported in.
     * @return Function name as string.
     */
    const char* getFunctionName() const;

    /**
     * @brief Get error code.
     * @return Error code as an string object.
     */
    const char* getErrorCodeLabel() const;

private:
    const std::int32_t _error_code;       //!< Currently held error code
    const std::int32_t _line_number;      //!< Line number in which the error occurred
    const std::string _error_description; //!< Currently held error description
    const std::string _file_name;         //!< File in which the error occurred
    const std::string _function_name;     //!< Function in which the error occurred
    const std::string _error_label;       //!< Error code as string representation
};                                        // class ErrorDescription

typedef ErrorDescription ErrorDescriptionType;

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_HEADER_INCLUDED
