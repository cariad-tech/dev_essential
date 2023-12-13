/**
 * @file
 * Specialize an reference counted object class without dynamic memory allocation
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#ifndef A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_DISABLE_DYNAMIC_ALLOC_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_DISABLE_DYNAMIC_ALLOC_HEADER_INCLUDED

#include <a_util/result/detail/error_description_intf.h>
#include <a_util/result/detail/reference_counted_object.h>

#include <cstddef> //std::size_t

namespace a_util {
namespace result {
namespace detail {
/// type needed for reference counting
struct ErrorDescriptionType {
};

/**
 * Specialization to disable dynamic allocation with new() by providing an operator new overload
 */
template <>
class ReferenceCountedObject<IErrorDescription, ErrorDescriptionType> {
public:
    /// CTOR, does nothing and should never be used directly.
    ReferenceCountedObject(...)
    {
    }

    /// Overload operator new() to prevent dynamic memory allocation.
    static void* operator new(std::size_t) throw()
    {
        return nullptr;
    }

    /// Overload operator delete() to prevent dynamic memory freeing.
    static void operator delete(void*, std::size_t)
    {
    }
};

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_DISABLE_DYNAMIC_ALLOC_HEADER_INCLUDED
