/**
 * @file
 * Implementation of legacy access_element
 *
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */
#ifndef DDL_STRUCT_ELEMENT_ACCESS_LEGACY_CLASS_HEADER
#define DDL_STRUCT_ELEMENT_ACCESS_LEGACY_CLASS_HEADER

#include <a_util/result.h>
#include <ddl/codec/legacy/struct_element.h>

namespace ddl {
namespace access_element {

/**
 * @brief Creates an a_util error ERR_AUTIL_NOT_FOUND
 */
_MAKE_RESULT(-20, ERR_STRUCT_ELEMENT_NOT_FOUND);

/**
 * @brief Legacy helper for the ddl::StructElement.
 *
 * @tparam AccessType The type to get the ElementLayout from to convert to the ddl::StructElement
 * structure.
 */
template <typename AccessType>
class StructElementLegacy {
public:
    /**
     * @brief Get the converted StructElement pointer
     * @remark The content of this pointer is only valid as long as no other getStructElement call
     * @return const ddl::StructElement*
     */
    const ddl::StructElement* getStructElement() const
    {
        return &_legacy_struct_element;
    }
    /**
     * @brief Fill the local Struct Element and retrieves the Struct Element pointer for legacy
     *
     * @param access The access instance to get the layout information from.
     * @param leaf_index The leaf index.
     * @return const ddl::StructElement*
     */
    const ddl::StructElement* getStructElement(const AccessType& access, size_t leaf_index) const
    {
        auto codec_index = access.resolve(leaf_index);
        const auto& layout = codec_index.getLayout();
        const auto& type_info = *(layout.type_info);
        _legacy_struct_element.p_enum = type_info.getLegacyAccessEnumType();
        _legacy_struct_element.type =
            static_cast<const a_util::variant::VariantType>(type_info.getType());
        _legacy_struct_element.name = access.getElementFullName(codec_index);
        return getStructElement();
    }
    /**
     * @brief Fill the local Struct Element and retrieves the Struct Element pointer for legacy in
     * @p struct_element
     *
     * @param access The access instance to get the layout information from.
     * @param leaf_index The leaf index
     * @param struct_element the pointer reference to return the StructElement
     * @return a_util::result::Result
     */
    a_util::result::Result getStructElement(const AccessType& access,
                                            size_t leaf_index,
                                            const ddl::StructElement*& struct_element) const
    {
        try {
            struct_element = getStructElement(access, leaf_index);
            return {};
        }
        catch (...) {
            return ERR_STRUCT_ELEMENT_NOT_FOUND;
        }
    }

private:
    mutable ddl::StructElement _legacy_struct_element = {};
};

} // namespace access_element
} // namespace ddl

#endif // DDL_STRUCT_ELEMENT_ACCESS_LEGACY_CLASS_HEADER
