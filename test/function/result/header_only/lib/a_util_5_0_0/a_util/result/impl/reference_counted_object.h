/**
 * @file
 * Implements the default reference counted object
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

#ifndef A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_HEADER_INCLUDED

#include <a_util/base/config.h>
#include <a_util/base/std/types.h>
#include <a_util/base/variadic.h>
#include <a_util/concurrency/atomic.h>
#include <a_util/result/detail/reference_counted_object_intf.h>

namespace a_util {
namespace result {
namespace detail {

/**
 * @short the default implementation of a reference counter
 * @internal
 */
template <typename Interface, typename Implementation>
class ReferenceCountedObject : public ReferenceCountedObjectInterface<Interface> {
private:
    ReferenceCountedObject(const ReferenceCountedObject&);            // =delete;
    ReferenceCountedObject& operator=(const ReferenceCountedObject&); //=delete;

public:
#if A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES
    template <typename... Args>
    ReferenceCountedObject(Args&&... args)
        : ReferenceCountedObjectInterface<Interface>(),
          _reference_count(0),
          _resource(std::forward<Args>(args)...)
    {
    }
#else
    template <typename T, typename Next, template <typename = T, typename = Next> class VaArgs>
    ReferenceCountedObject(const VaArgs<T, Next>& args)
        : ReferenceCountedObjectInterface<Interface>(),
          _reference_count(0),
          _resource(base::make_from_variadic<Implementation>(args))
    {
    }

#endif // A_UTIL_ENABLE_COMPILER_CXX_VARIADIC_TEMPLATES

    virtual void addReference() const
    {
        concurrency::atomicIncrement(_reference_count);
    }

    virtual void removeReference() const
    {
        if (0 == concurrency::atomicDecrement(_reference_count)) {
            delete this;
        }
    }

    virtual const Interface& getObject() const
    {
        return _resource;
    }

    virtual Interface& getObject()
    {
        return _resource;
    }

private:
    virtual ~ReferenceCountedObject()
    {
    }

private:
    mutable std::int32_t _reference_count; //!< Reference to count atomically
    Implementation _resource;              //!< Resource to manage
};

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_HEADER_INCLUDED
