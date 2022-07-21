/**
 * @file
 * Definition of Data Representation header for Codec API.
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef DDL_DATA_REPRESENTATION_CLASS_HEADER
#define DDL_DATA_REPRESENTATION_CLASS_HEADER

namespace ddl {

#ifndef DDL_DATA_REPRESENTATION_DEFINED
#define DDL_DATA_REPRESENTATION_DEFINED
/**
 * Enumeration for the data representation
 */
enum DataRepresentation {
    serialized,                 ///< serialized data, i.e network, on disks, can msgs, ...
    deserialized,               ///< deserialized data, c-structs, arrays, ...
    Serialized = serialized,    ///< alias names for legacy reasons
    Deserialized = deserialized ///< alias names for legacy reasons
};
/// Typedef provided for compatibility reasons
using tDataRepresentation = DataRepresentation;
#endif

/** @endcond */

} // namespace ddl

#endif // DDL_DATA_REPRESENTATION_CLASS_HEADER
