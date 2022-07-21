/**
 * @file
 * Implementation of the private ChildElements and StructAccess
 *
 * Copyright @ 2022 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#ifndef DDL_CODEC_ELEMENTS_CLASS_HEADER
#define DDL_CODEC_ELEMENTS_CLASS_HEADER

#include "named_codec_index.h"

#include <a_util/result/result_type_decl.h>
#include <ddl/codec/data_representation.h>
#include <ddl/dd/dd.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/dd/dd_struct_access.h>

#include <functional>
#include <unordered_map>

namespace ddl {
namespace codec {

struct TypeSize {
    size_t serialized_bits = 0;
    size_t deserialized_bytes_unaligned = 0;
    size_t deserialized_bytes_aligned = 0;
    size_t deserialized_bytes = 0; // this is the size, aligned or not depends on ddl version
    size_t type_alignment = 0;
    dd::Version type_ddl_version = dd::Version::ddl_version_notset;
    bool is_dynamic = false;
};

using ArraySizeResolverFunction = std::function<size_t(const NamedCodecIndex&)>;
using CodecTypes = std::map<std::string, std::unique_ptr<CodecTypeInfo>>;
using CodecConstants = std::map<std::string, std::unique_ptr<CodecConstantInfo>>;
using CodecDefaultValues = std::map<std::string, std::unique_ptr<CodecDefaultValueInfo>>;

/**
 * @internal
 * This class is for internal use only.
 */
class ChildElementAccess {
public:
    void findCodecLayout(const CodecIndex& index,
                         size_t index_pos,
                         ElementLayout& elem_layout,
                         bool only_static) const;
    void findCodecLayout(const CodecIndex& index,
                         size_t index_pos,
                         ElementLayout& elem_layout,
                         std::string& full_element_name,
                         bool only_static) const;

    // find by name
    void findCodecLayoutByName(const NamedCodecIndex& named_index,
                               size_t index_pos,
                               ElementLayout& elem_layout,
                               CodecIndex& find_index,
                               bool only_static) const;
    // find by leaf index
    bool findCodecLayoutByLeafIndex(size_t leaf_index,
                                    ElementLayout& elem_layout,
                                    CodecIndex& index,
                                    bool only_static) const;

    std::string getName(size_t array_pos) const;
    std::string getName(const CodecIndex& codec_index, size_t pos) const;
    const std::string& getBaseName() const;
    const std::string& getBaseName(const CodecIndex& codec_index, size_t pos) const;

    bool hasDynamicElements() const;
    bool isLeaf() const;

    size_t getLeafCount() const;
    size_t getArrayLeafCount() const;

    size_t getSubElementCount(bool only_static) const;
    size_t getSubElementCount(const CodecIndex& codec_index, size_t pos, bool only_static) const;

    void setElementIndex(size_t index);
    size_t getElementIndex() const;

    void setBeginLeafIndex(size_t leaf_index);
    size_t getBeginLeafIndex() const;

    size_t getElementAlignment() const;
    const ElementLayoutBase& getLayoutBase() const;
    const TypeSize& getTypeSize() const;

    const ChildElementAccess& operator[](const CodecIndex::ElementIndex& element_index) const;
    void setNextElement(const std::shared_ptr<ChildElementAccess>& next_element);

public: // needs to be override for resolving dynamics
    virtual size_t resolveArraySize(const NamedCodecIndex& current_index,
                                    size_t index_pos,
                                    const ArraySizeResolverFunction& array_resolver);

    virtual void resolveSizes(const dd::Version& file_dd_version);
    virtual void resolveOffsets(const ChildElementAccess& previous_element);
    virtual bool hasStaticPosition() const;

    void initializeStatic(const ddl::dd::StructElementAccess& element_access,
                          const ddl::dd::StructTypeAccess& struct_type_access,
                          StructAccess& struct_access);

    void resolveDynamics(const NamedCodecIndex& parent_index,
                         const ArraySizeResolverFunction& array_resolver,
                         const dd::Version& dd_file_version);
    void resolveDynamicSize(const dd::Version& file_dd_version);

    a_util::result::Result isBinaryEqual(const ChildElementAccess& other,
                                         bool checking_dynamics) const;
    a_util::result::Result isBinarySubset(const ChildElementAccess& other,
                                          bool checking_dynamics) const;

private:
    void addSubElement(const ddl::dd::StructElementAccess& element_access,
                       StructAccess& struct_access);
    const ChildElementAccess* binaryLeafSearch(size_t leaf_index,
                                               size_t begin_search_index,
                                               size_t end_search_index) const;

protected:
    class SubElements {
    public:
        SubElements() = default;
        SubElements(const SubElements& other);
        SubElements& operator=(const SubElements& other);

        void addStatic(const std::shared_ptr<ChildElementAccess>& element_access);
        void addDynamic(const std::shared_ptr<ChildElementAccess>& element_access);
        size_t getSize() const;
        size_t getStaticCount() const;
        size_t getDynamicCount() const;
        const ChildElementAccess& operator[](const CodecIndex::ElementIndex& index) const;
        const ChildElementAccess& operator[](size_t index) const;

        const ChildElementAccess& operator[](
            const NamedCodecIndex::NamedElementIndex& named_index) const;
        const ChildElementAccess& operator[](const std::string& name) const;

        std::shared_ptr<ChildElementAccess> operator[](size_t index);
        // std::shared_ptr<ChildElementAccess> operator[](size_t index);

    private:
        void add(std::shared_ptr<ChildElementAccess> element_access);
        std::vector<std::shared_ptr<ChildElementAccess>> _codec_elements_vector = {};
        std::unordered_map<std::string, std::shared_ptr<ChildElementAccess>> _codec_elements_map =
            {};
        size_t _static_count = 0;
    };
    SubElements _sub_elements;
    ElementLayoutBase _static_layout_base;
    TypeSize _static_type_size;
    std::shared_ptr<ChildElementAccess> _next_element;

    size_t _leaf_count = 0;
    size_t _array_leaf_count = 0;

private:
    std::string _base_name;
    size_t _element_alignment = ddl::dd::Alignment::e_invalid;

    size_t _element_index = 0;
    size_t _begin_leaf_index = 0;
};

/**
 * @internal
 * This class is for internal use only.
 */
class StaticCodecElementAccess : public ChildElementAccess {
public:
    StaticCodecElementAccess() = delete;
    StaticCodecElementAccess(const ddl::dd::StructElementAccess& element_access,
                             StructAccess& struct_access);
};

/**
 * @internal
 * This class is for internal use only.
 */
class DynamicCodecElementAccess : public ChildElementAccess {
public:
    DynamicCodecElementAccess() = delete;
    DynamicCodecElementAccess(const ddl::dd::StructElementAccess& element_access,
                              StructAccess& struct_access);

    const std::string& getArrayElementName() const;

private:
    // this is override for Dynamic content
    // if isDynamic for array_size
    // if isAfterDynamic for offset
    size_t resolveArraySize(const NamedCodecIndex& current_index,
                            size_t index_pos,
                            const ArraySizeResolverFunction& array_resolver) override;
    void resolveSizes(const dd::Version& file_dd_version) override;
    void resolveOffsets(const ChildElementAccess& previous_element) override;
    bool hasStaticPosition() const override;

    void initializeDynamic(const ddl::dd::StructElementAccess& element_access);

    std::string _array_element_name = {};
    bool _is_after_dynamic = false;
    bool _is_dynamic = false;
};

/**
 * @internal
 * This class is for internal use only.
 */
class StructAccess {
public:
    StructAccess();
    StructAccess(const StructAccess& other) = default;
    StructAccess& operator=(const StructAccess& other) = default;
    StructAccess(StructAccess&&) = delete;
    StructAccess& operator=(StructAccess&&) = delete;

    StructAccess(const ddl::dd::StructTypeAccess& struct_type_access,
                 ddl::dd::Version dd_version = ddl::dd::Version::ddl_version_current);

    size_t getStaticBufferSize(DataRepresentation representation) const;
    size_t getBufferSize(DataRepresentation representation) const;
    a_util::result::Result getInitResult() const;

    ElementLayout getCodecElementLayout(const CodecIndex& index) const;
    ElementLayout getCodecElementLayout(const CodecIndex& index, std::string& fullname) const;

    CodecIndex resolve(size_t leaf_index) const;
    CodecIndex resolve(const NamedCodecIndex& named_index) const;
    void resolve(CodecIndex& index, bool force_reset) const;

    std::shared_ptr<StructAccess> makeResolvedCodecAccess() const;
    void resolveDynamic(ArraySizeResolverFunction array_resolver);

    bool hasEnums() const;

    size_t getElementChildCount(const CodecIndex& codec_index, bool only_static) const;
    size_t getLeafIndexCount() const;

    std::string getName(const CodecIndex& codec_index) const;
    const std::string& getBaseName(const CodecIndex& codec_index) const;
    bool isDynamic() const;

    const CodecTypeInfo* getOrCreateCodecTypeInfo(const dd::DataType& data_type);
    const CodecTypeInfo* getOrCreateCodecTypeInfo(const dd::EnumType& enum_type);
    const CodecTypeInfo* getOrCreateCodecTypeInfo(const dd::StructType& struct_type);
    const CodecConstantInfo* findConstantInfo(const std::string& constant_name);
    const CodecDefaultValueInfo* getOrCreateDefaultValueInfo(ElementType type_of_element,
                                                             const std::string& default_value);

    a_util::result::Result isBinaryEqual(const StructAccess& other) const;
    a_util::result::Result isBinarySubset(const StructAccess& other) const;

private:
    void resolveDynamicStructSize();

    ChildElementAccess _single_codec_access_element;

    bool _is_dynamic = false;
    a_util::result::Result _init_result;
    ddl::dd::Version _dd_version = ddl::dd::Version::ddl_version_current;

    std::shared_ptr<CodecTypes> _all_types;
    bool _has_enums = false;
    std::shared_ptr<CodecConstants> _all_constants;
    std::shared_ptr<CodecConstantInfo> _empty_constant;
    std::shared_ptr<CodecDefaultValues> _all_default_values;
    std::shared_ptr<CodecDefaultValueInfo> _empty_default;

    TypeSize _static_struct_size;
    TypeSize _dynamic_struct_size;
    bool _resolved_dynamics = false;
};

} // namespace codec
} // namespace ddl

#endif // DDL_CODEC_ELEMENTS_CLASS_HEADER
