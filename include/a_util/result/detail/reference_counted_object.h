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

#include "a_util/result/detail/reference_counted_object_intf.h"

#include <atomic>  //std::atomic
#include <cstdint> //std::int32_t
#include <utility> //std::forward

namespace a_util {
namespace result {
namespace detail {
/**
 * @brief Default implementation of a reference counter
 * @tparam Interface The interface of the resource to handle
 * @tparam Implementation The type implementing the @c Interface type
 */
template <typename Interface, typename Implementation>
class ReferenceCountedObject : public ReferenceCountedObjectInterface<Interface> {
private:
    ReferenceCountedObject(const ReferenceCountedObject&);            //=delete;
    ReferenceCountedObject& operator=(const ReferenceCountedObject&); //=delete;

public:
    /**
     * CTOR constructing the managed resource and the reference counter to an initial value of zero
     * @tparam Args Argument types of the resource's constructor
     * @param[in] args Values to forward to the resource's constructor
     */
    template <typename... Args>
    ReferenceCountedObject(Args&&... args)
        : ReferenceCountedObjectInterface<Interface>(),
          _reference_count(0),
          _resource(std::forward<Args>(args)...)
    {
    }

    /// Increase the reference count by 1
    virtual void addReference() const
    {
        ++_reference_count;
    }

    /// Decrease the reference count by 1, deleting @c *this if it was the last reference
    virtual void removeReference() const
    {
        if (0 == (--_reference_count)) {
            delete this;
        }
    }

    /**
     * Get a reference to the handled object's interface - provided for const correctness
     * @return Const reference to the handled object
     */
    virtual const Interface& getObject() const
    {
        return _resource;
    }

    /**
     * Get a reference to the handled object's interface
     * @return Reference to the handled object
     */
    virtual Interface& getObject()
    {
        return _resource;
    }

private:
    virtual ~ReferenceCountedObject()
    {
    }

private:
#ifndef __QNX__
    mutable std::atomic<std::int32_t> _reference_count; //!< Reference to count atomically
#else
    mutable std::atomic_int_fast32_t _reference_count;
#endif
    Implementation _resource; //!< Resource to manage
};

/**
 * @brief Default implementation of a non-counting reference counter
 * @tparam Interface The interface of the resource to handle
 * @tparam Implementation The type implementing the @c Interface type
 */
template <typename Interface, typename Implementation>
class UncountedObject : public ReferenceCountedObjectInterface<Interface> {
private:
    UncountedObject(const UncountedObject&) = delete;
    UncountedObject& operator=(UncountedObject&) = delete;
    UncountedObject(UncountedObject&&) = delete;
    UncountedObject& operator=(UncountedObject&&) = delete;

public:
    /// Default CTOR
    UncountedObject() = default;

    /**
     * CTOR constructing the managed resource
     * @tparam Args Argument types of the resource's constructor
     * @param[in] args Values to forward to the resource's constructor
     */
    template <typename... Args>
    UncountedObject(Args&&... args)
        : ReferenceCountedObjectInterface<Interface>(), _resource(std::forward<Args>(args)...)
    {
    }

    /// Default DTOR, provided to be consistent with rule-of-five
    virtual ~UncountedObject() = default;

    /// Noop
    void addReference() const override
    {
        // noop
    }

    /// Noop
    void removeReference() const override
    {
        // noop
    }

    /**
     * Get a reference to the handled object's interface - provided for const correctness
     * @return Const reference to the handled object
     */
    const Interface& getObject() const override
    {
        return _resource;
    }

    /**
     * Get a reference to the handled object's interface
     * @return Reference to the handled object
     */
    Interface& getObject() override
    {
        return _resource;
    }

private:
    Implementation _resource; //!< Resource to manage
};

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_IMPL_REFERENCE_COUNTED_OBJECT_HEADER_INCLUDED
