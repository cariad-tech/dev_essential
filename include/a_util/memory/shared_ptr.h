/**
 * @file
 * Public API for compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/memory/shared_ptr">std::shared_ptr</a> from header
 * <a href="https://en.cppreference.com/w/cpp/header/memory">\<memory\></a>
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_MEMORY_SHARED_PTR_HEADER_INCLUDED
#define A_UTIL_UTIL_MEMORY_SHARED_PTR_HEADER_INCLUDED

#include <memory>

namespace a_util {
namespace memory {
/**
 * Compatibility to C++11
 * <a href="https://en.cppreference.com/w/cpp/memory/shared_ptr">std::shared_ptr</a>
 */
using std::shared_ptr;

} // namespace memory
} // namespace a_util

#endif // A_UTIL_UTIL_MEMORY_SHARED_PTR_HEADER_INCLUDED
