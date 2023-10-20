/**
 * @file
 * Public API for @ref a_util::result::Result "Result" type and functions
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_RESULT_TYPE_DECL_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_RESULT_TYPE_DECL_HEADER_INCLUDED

#include <a_util/result/detail/result_description_decl.h>
#include <a_util/result/result_info.h>

#include <utility> //std::swap

namespace a_util {
namespace result {
namespace detail {
// forward declaration
class IErrorDescription;

} // namespace detail

/**
 * A common result class usable as return value throughout.
 */
class Result {
public: // construction
    /**
     * Construct empty which basically means no error occurred.
     * @post a_util::result::SUCCESS == getErrorCode() &&
     * @post -1 == getLine() &&
     * @post "No error occurred" == getDescription() &&
     * @post "" == getFile() &&
     * @post "" == getFunction() &&
     * @post "" == getErrorLabel()
     */
    Result();

    /**
     * Construct from bool
     * @param[in] val Bool value to construct with
     * @post <tt>true == a_util::result::Result(true);</tt>
     * @post <tt>false == a_util::result::Result(false);</tt>
     * @post No dynamic memory is allocated
     */
    Result(bool val);

    /**
     * Construct with a_util::result::SUCCESS
     * @details This constructors sole intention is to provide the best match during compilation
     * time if a function call returns with a_util::result::SUCCESS. With
     * decltype(a_util::result::SUCCESS) being another type than all other error codes the
     * compiler deduces this constructor as best fit.
     * @post @see Result()
     */
    Result(const ResultInfo<void>&);

    /**
     * Assign error code to result object
     * @param[in] error The error code
     */
    template <typename ErrorType>
    Result(const ResultInfo<ErrorType>& error);

    /**
     * Construct result object with complete set of error information
     * @tparam ErrorCode Error code (to deduce specialized template to stringify the error code)
     * @param[in] error The The concrete error object
     * @param[in] error_description Complete description of the error (default: "")
     * @param[in] line Line in source file the error was reported (default: -1)
     * @param[in] file Source file the error was reported in (default: "")
     * @param[in] function Function the error was reported in (default: "")
     */
    template <typename ErrorType>
    explicit Result(ResultInfo<ErrorType> error,
                    const char* error_description,
                    std::int32_t line,
                    const char* file,
                    const char* function);

    /**
     * Create result object with just the error code type.
     * @param[in] error_code The error code
     */
    Result(std::int32_t error_code);

    /**
     * Create result object with complete set of error information from just an error code
     * @param[in] error_code The error code
     * @param[in] error_description Complete description of the error (default: "")
     * @param[in] line Line in source file the error was reported (default: -1)
     * @param[in] file Source file the error was reported in (default: "")
     * @param[in] function Function the error was reported in (default: "")
     */
    explicit Result(std::int32_t error_code,
                    const char* error_description,
                    std::int32_t line,
                    const char* file,
                    const char* function);

    /**
     * Create result object from another result, creating new detailed information
     * @param[in] other_result Result to copy the error code and error code string information from.
     * @param[in] error_description Complete description of the error.
     * @param[in] line Line in source file the error was reported.
     * @param[in] file Source file the error was reported in.
     * @param[in] function Function the error was reported in.
     */
    explicit Result(const Result& other_result,
                    const char* error_description,
                    std::int32_t line,
                    const char* file,
                    const char* function);

    /**
     * Destructor.
     */
    ~Result();

    /**
     * Copy constructor.
     * @param[in] other Where this gets copied from.
     * @note No new memory is allocated, the error information from @c other are safely
     *       referenced in the new created object of type @c Result.
     */
    Result(const Result& other);

    /**
     * Assign with a_util::result::SUCCESS
     * @details This assignments sole intention is to provide the best match during compilation
     * time if a function call returns with a_util::result::SUCCESS. With
     * decltype(a_util::result::SUCCESS) being another type than all other error codes the compiler
     * deduces this assignment operator as best fit.
     * @post @see Result()
     * @return *this
     */
    Result& operator=(const ResultInfo<void>&);

    /**
     * Assignment operator
     * @param[in] other Result type being assigned to the this object.
     * @note Passing @c other by value is fully intentional here!
     * @return *this
     */
    Result& operator=(Result other);

    /**
     * Assignment from bool
     * @param[in] val Bool value to construct with
     * @post No dynamic memory is allocated.
     * @return Reference to *this
     */
    Result& operator=(bool val);

    /**
     * Assignment from numeric
     * @param[in] val Numeric value to construct with
     * @post No dynamic memory is allocated.
     * @return Reference to *this
     */
    Result& operator=(std::int32_t val);

    /**
     * Move operator.
     * @param[in] other rvalue of type Result moved to *this. Left in an empty but valid state.
     */
    Result(Result&& other);

public: // access
    /**
     * Get error code.
     * @return Error code.
     */
    std::int32_t getErrorCode() const;

    /**
     * Get error label as string representation.
     * @return Error code represented as string.
     */
    const char* getErrorLabel() const;

    /**
     * Get user provided error description.
     * @return Error description as string.
     */
    const char* getDescription() const;

    /**
     * Get line in source file where the error was reported.
     * @return Line number.
     */
    std::int32_t getLine() const;

    /**
     * Get name of the file the error was reported in.
     * @return File name as string.
     */
    const char* getFile() const;

    /**
     * Get name of the function the error was reported in.
     * @return Function name as string.
     */
    const char* getFunction() const;

    /**
     * Convert result to bool
     * @return If error code equals 0, @c true, otherwise @c false
     */
    explicit operator bool() const noexcept;

    /**
     * Specialized friend swap function to enable ADL in std::swap later.
     * @param[in,out] lhs Left hand side result object.
     * @param[in,out] rhs Right hand side result object.
     */
    friend void swap(Result& lhs, Result& rhs);

    /**
     * Comparison operator (shallow comparison)
     * @param[in,out] lhs Left hand side result object.
     * @param[in,out] rhs Right hand side result object.
     * @return @c true if both result objects match, @c false otherwise.
     */
    friend bool operator==(const Result& lhs, const Result& rhs);

private: // private constructor
    /**
     * Construct result object with complete set of error information
     * @param[in] error_code The error code
     * @param[in] error_description Complete description of the error (default: "")
     * @param[in] line Line in source file the error was reported (default: -1)
     * @param[in] file Source file the error was reported in (default: "")
     * @param[in] function Function the error was reported in (default: "")
     * @param[in] label String representation of error code (default: "")
     */
    explicit Result(std::int32_t error_code,
                    const char* error_description,
                    std::int32_t line,
                    const char* file,
                    const char* function,
                    const char* label);

    /**
     * Create a detailed object of type @c Result on the heap from an error code
     * @param[in] error_code The error code to create the result object from.
     * @param[in] error_description Complete description of the error (default: "")
     * @param[in] line Line in source file the error was reported (default: -1)
     * @param[in] file Source file the error was reported in (default: "")
     * @param[in] function Function the error was reported in (default: "")
     * @return Object of type @c Result containing detailed error information.
     */
    static Result createFrom(std::int32_t error_code,
                             const char* error_description,
                             std::int32_t line,
                             const char* file,
                             const char* function);

private: // private member data
    /// Error description type
    typedef detail::ResultDescription<detail::IErrorDescription> DescriptionType;
    DescriptionType _result_handle; //!< The result handler wrapper
};                                  // class Result

/**
 * Negated comparison operator (shallow comparison)
 * @param[in,out] lhs Left hand side result object.
 * @param[in,out] rhs Right hand side result object.
 * @return @c true if both result objects do not match, @c false otherwise.
 */
bool operator!=(const Result& lhs, const Result& rhs);

/**
 * Comparison operator (equality) for objects of type Result and ResultInfo
 * @tparam T Error code type handled by ResultInfo<>
 * @param[in] result Left hand side result object
 * @param[in] error_code Right hand side error object
 * @return @c true if both objects contain the same error code, @c false otherwise
 */
template <typename T>
bool operator==(const Result& result, const ResultInfo<T>& error_code);

/**
 * Comparison operator (inequality) for objects of type Result and ResultInfo
 * @tparam T Error code type handled by ResultInfo<>
 * @param[in] result Left hand side result object
 * @param[in] error_code Right hand side error object
 * @return @c true if both objects contain different error codes, @c false otherwise
 */
template <typename T>
bool operator!=(const Result& result, const ResultInfo<T>& error_code);

/**
 * Comparison operator (equality) for objects of type Result and ResultInfo
 * @tparam T Error code type handled by ResultInfo<>
 * @param[in] error_code Left hand side error object
 * @param[in] result Right hand side result object
 * @return @c true if both objects contain the same error code, @c false otherwise
 */
template <typename T>
bool operator==(const ResultInfo<T>& error_code, const Result& result);

/**
 * Comparison operator (inequality) for objects of type Result and ResultInfo
 * @tparam T Error code type handled by ResultInfo<>
 * @param[in] error_code Left hand side error object
 * @param[in] result Right hand side result object
 * @return @c true if both objects contain different error codes, @c false otherwise
 */
template <typename T>
bool operator!=(const ResultInfo<T>& error_code, const Result& result);

/**
 * Specialization of @c a_util::result::isOk<typename ResultType>() for correct lookup
 * @param[in] result Result instance to check for error
 * @dev_essential_deprecated Please use @ref Result::operator bool().
 * @return If error code equals 0, @c true, otherwise @c false
 */
template <>
DEV_ESSENTIAL_DEPRECATED("Please use the conversion Result::operator bool().")
bool isOk<Result>(const Result& result);

} // namespace result
} // namespace a_util

/** @cond INTERNAL_DOCUMENTATION */
namespace std {
/**
 * swap specialization for ADL
 * @param[in,out] lhs Left hand side result to be swapped
 * @param[in,out] rhs Right hand side result to be swapped
 */
template <>
void swap<a_util::result::Result>(a_util::result::Result& lhs, a_util::result::Result& rhs);
} // namespace std

#include <a_util/result/detail/result_type_impl.h>
/** @endcond */

#endif // A_UTIL_UTIL_RESULT_RESULT_TYPE_DECL_HEADER_INCLUDED
