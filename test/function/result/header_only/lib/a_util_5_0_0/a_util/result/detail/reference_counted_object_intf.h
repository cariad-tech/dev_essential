/**
 * @file
 * Declare an internal interface for basic binary compatible reference counting
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

#ifndef A_UTIL_UTIL_RESULT_DETAIL_REFERENCE_COUNTED_OBJECT_INTERFACE_HEADER_INCLUDED
#define A_UTIL_UTIL_RESULT_DETAIL_REFERENCE_COUNTED_OBJECT_INTERFACE_HEADER_INCLUDED

namespace a_util {
namespace result {
namespace detail {

/**
 * @short An internal stable interface for binary compatibility of reference counting.
 * @internal
 */
template <typename Resource>
class ReferenceCountedObjectInterface {
public:
    virtual void addReference() const = 0;
    virtual void removeReference() const = 0;

    virtual const Resource& getObject() const = 0;
    virtual Resource& getObject() = 0;

protected:
    ~ReferenceCountedObjectInterface()
    {
    }
}; // template <typename Resource> class ReferenceCountedObjectInterface

} // namespace detail
} // namespace result
} // namespace a_util

#endif // A_UTIL_UTIL_RESULT_DETAIL_REFERENCE_COUNTED_OBJECT_INTERFACE_HEADER_INCLUDED
