/**
 * @file
 * Public API for @ref a_util::system::AddressInfo "AddressInfo" class
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef A_UTIL_SYSTEM_ADDRESS_INFO_HEADER_INCLUDED
#define A_UTIL_SYSTEM_ADDRESS_INFO_HEADER_INCLUDED

#include <a_util/base/types.h> // handle_t
#include <a_util/filesystem/path.h>

namespace a_util {
namespace system {

/**
 * Address info class
 * This class can be used to query information about a memory address (e.g. the file path of the
 * binary where the address is located in)
 */
class AddressInfo {
public:
#if ((defined _MSC_VER) && (_MSVC_LANG >= 201510L)) || (__cplusplus >= 201510L)
    // The noexcept-specification in C++17 is a part of the function type and
    // may appear as part of any function declarator.
    // See https://en.cppreference.com/w/cpp/language/noexcept_spec
    /**
     * Constructor for function addresses
     */
    template <typename ReturnType, typename... Args>
    explicit AddressInfo(ReturnType (*const function)(Args...) noexcept) noexcept
        : _address{toDataPointer(function)}
    {
    }
#endif

    /**
     * Constructor for function addresses
     */
    template <typename ReturnType, typename... Args>
    explicit AddressInfo(ReturnType (*const function)(Args...)) noexcept
        : _address{toDataPointer(function)}
    {
    }

    /**
     * Constructor for global or static variables addresses
     * @warning @c variable must not be a heap or stack variable!
     */
    template <typename T>
    explicit AddressInfo(const T& variable) noexcept : _address(&variable)
    {
        static_assert(!std::is_function<T>::value,
                      "ERROR: CTOR for data pointer is called with function pointer");
    }

public:
    /**
     * Get the fully qualified file path to the executable or shared library the address is located
     * in.
     * @return File path of the binary the address is located in (e.g. "C:\dir\test.exe",
     * "C:\dir\test.dll", "netdrive\test.exe" or
     * "\\netdrive\test.so"). Might be empty if an error occurs.
     */
    filesystem::Path getFilePath() const;

private:
    /**
     * Casts a function pointer to a data pointer
     * @author [Johannes Schaub - litb](https://stackoverflow.com/users/34509/johannes-schaub-litb)
     *         here: https://stackoverflow.com/a/1306125/411641
     * @copyright [CC BY-SA 2.5](https://creativecommons.org/licenses/by-sa/2.5/)
     *            (See: [What is the license for the content I post?]
     *            (https://stackoverflow.com/help/licensing))
     */
    template <typename Return, typename... Args>
    const_handle_t toDataPointer(Return (*const function)(Args...)) noexcept
    {
        // According to the C-standard, casting a function pointer to a void* is not allowed.
        // It is however allowed to cast a function pointer to a function pointer of a different
        // type. Taking the address of the function pointer will return a data pointer.
        //
        // Read explanation from inner to outer static_cast.
        // Type punning, cast the data pointer back to a pointer to a data pointer and return the
        // address of the data pointer (const correctness)
        return *static_cast<void* const*>(
            // Type punning, cast the const data pointer (pointing to the function pointer) to
            // const void*
            static_cast<const void*>(
                // cast temporary function pointer to lvalue reference to const, which we can take
                // the address of. The address is the resulting data pointer pointing to the
                // function pointer
                &static_cast<Return (*const&)(Args...)>(function)));
    }

private:
    const_handle_t _address;
};

} // namespace system
} // namespace a_util

#endif // A_UTIL_SYSTEM_ADDRESS_INFO_HEADER_INCLUDED
