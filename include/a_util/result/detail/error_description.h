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

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_HEADER_INCLUDED

#include <a_util/result/detail/error_description_intf.h>
#include <a_util/result/result_info_decl.h>

#include <string>

namespace a_util {
namespace result {
namespace detail {
/**
 * Implements the error description interface for heap allocated error cases
 */
class ErrorDescription final : public IErrorDescription {
public: // const members, disable copy and move semantics
    ErrorDescription(const ErrorDescription&) = delete;
    ErrorDescription& operator=(const ErrorDescription&) = delete;

    ErrorDescription(ErrorDescription&&) = delete;
    ErrorDescription& operator=(ErrorDescription&&) = delete;

    /// DTOR
    ~ErrorDescription() = default;

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
    inline ErrorDescription(std::int32_t error_code,
                            const char* error_description,
                            std::int32_t line,
                            const char* file,
                            const char* function,
                            const char* error_label);

    /**
     * @brief Get error code.
     * @return Error code.
     */
    inline std::int32_t getErrorCode() const;

    /**
     * @brief Get line in source file where the error was reported.
     * @return Line number.
     */
    inline std::int32_t getLine() const;

    /**
     * @brief Get error code as string representation.
     * @return Error code represented as string.
     */
    inline const char* getErrorDescription() const;

    /**
     * @brief Get name of the file the error was reported in.
     * @return File name as string.
     */
    inline const char* getFileName() const;

    /**
     * @brief Get name of the function the error was reported in.
     * @return Function name as string.
     */
    inline const char* getFunctionName() const;

    /**
     * @brief Get error code label.
     * @return Error code as an string object.
     */
    inline const char* getErrorCodeLabel() const;

private:
    const std::int32_t _error_code;       //!< Currently held error code
    const std::int32_t _line_number;      //!< Line number in which the error occurred
    const std::string _error_description; //!< Currently held error description
    const std::string _file_name;         //!< File in which the error occurred
    const std::string _function_name;     //!< Function in which the error occurred
    const std::string _error_label;       //!< Error code as string representation
};                                        // class ErrorDescription

/// Alias type needed for reference counting
typedef ErrorDescription ErrorDescriptionType;

/**
 * Implements the error description interface using static data from
 * @ref a_util::result::ResultInfo<T>
 *
 * This implementation only provides useful information about the error code and the corresponding
 * error label, both provided by @c T::getCode() and @c T::getLabel().
 * @tparam T Type for template parameter of @ref a_util::result::ResultInfo<T>
 */
template <typename T, template <typename = T> class = ::a_util::result::ResultInfo>
class ResultInfoDescription final : public IErrorDescription {
public:
    /**
     * Get error code using @ref a_util::result::ResultInfo<T>::getCode()
     * @return Error code.
     */
    inline std::int32_t getErrorCode() const override;

    /**
     * Get error label using @ref a_util::result::ResultInfo<T>::getLabel()
     * @return Error code label as string.
     */
    inline const char* getErrorCodeLabel() const override;

private:
    /**
     * Does not provide any useful information in this context.
     * @retval -1
     */
    inline std::int32_t getLine() const override;

    /**
     * Does not provide any useful information in this context.
     * @retval ""
     */
    inline const char* getErrorDescription() const override;

    /**
     * Does not provide any useful information in this context.
     * @retval ""
     */
    inline const char* getFileName() const override;

    /**
     * Does not provide any useful information in this context.
     * @retval ""
     */
    inline const char* getFunctionName() const override;
};

} // namespace detail
} // namespace result
} // namespace a_util

#include <a_util/result/detail/error_description_impl.h>

#endif // A_UTIL_UTIL_RESULT_IMPL_ERROR_DESCRIPTION_HEADER_INCLUDED
