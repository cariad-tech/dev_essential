/**
 * @file
 * Private API for
 * @ref a_util::result::detail::ReferenceCountedObjectInterface "ReferenceCountedObjectInterface"
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

#ifndef A_UTIL_UTIL_RESULT_DETAIL_REFERENCE_COUNTED_OBJECT_INTERFACE_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_REFERENCE_COUNTED_OBJECT_INTERFACE_HEADER_INCLUDED

namespace a_util {
namespace result {
namespace detail {
/**
 * An internal stable interface to ensure binary compatibility of reference counting.
 * @warning This interface must not be changed to ensure binary compatibility!
 * @tparam Resource The resource to handle
 */
template <typename Resource /*e.g. a_util::result::detail::IErrorDescription*/>
class ReferenceCountedObjectInterface {
public:
    /// Increase the reference count
    virtual void addReference() const = 0;
    /// Decrease the reference count
    virtual void removeReference() const = 0;

    /**
     * Get a reference to the handled object
     * @return Reference to the handled object
     */
    virtual Resource& getObject() = 0;

    /**
     * Get a reference to the handled object - provided for const correctness
     * @return Const reference to the handled object
     */
    virtual const Resource& getObject() const = 0;

protected:
    /// DTOR
    ~ReferenceCountedObjectInterface()
    {
    }
}; // template <typename Resource> class ReferenceCountedObjectInterface

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_DETAIL_REFERENCE_COUNTED_OBJECT_INTERFACE_HEADER_INCLUDED
