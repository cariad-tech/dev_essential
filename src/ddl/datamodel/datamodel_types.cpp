/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
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

#include <ddl/datamodel/datamodel_types.h>
#include <ddl/dd/dd_infomodel_type.h>
#include <ddl/utilities/std_to_string.h>

#include <exception>
#include <utility>

namespace ddl {

namespace dd {

namespace datamodel {

/*************************************************************************************************************/
// TypeBase
/*************************************************************************************************************/
TypeBase::TypeBase(const std::string& name) : _name(name)
{
}

const std::string& TypeBase::getName() const
{
    return _name;
}

void TypeBase::setName(const std::string& name)
{
    _name = name;
}

/*************************************************************************************************************/
// DataType
/*************************************************************************************************************/
DataType::DataType(const std::string& name,
                   size_t bit_size,
                   const std::string& description,
                   OptionalSize array_size,
                   const std::string& unit_name,
                   const std::string& minimum_value,
                   const std::string& maximum_value,
                   OptionalSize default_type_alignment)
    : TypeBase(name),
      _bit_size(bit_size),
      _description(description),
      _array_size(array_size),
      _unit_name(unit_name),
      _minimum_value(minimum_value),
      _maximum_value(maximum_value),
      _default_type_alignment(default_type_alignment)
{
}

DataType::DataType(const std::initializer_list<std::string>& initializer)
{
    size_t current_idx = 0;
    if (initializer.size() < 2) {
        throw Error("DataType::DataType", "to view arguments");
    }
    for (const auto& current: initializer) {
        switch (current_idx) {
        case 0:
            _name = current;
            break;
        case 1:
            setBitSize(std::stoul(current));
            break;
        case 2:
            setDescription(current);
            break;
        case 3:
            setArraySize(std::stoul(current));
            break;
        case 4:
            setUnitName(current);
            break;
        case 5:
            setMin(current);
            break;
        case 6:
            setMax(current);
            break;
        case 7:
            _default_type_alignment = AlignmentConversion::fromString(current);
            break;
        }
        ++current_idx;
    }
}

bool DataType::operator==(const DataType& other) const
{
    size_t array_size_this = *getArraySize();
    if (array_size_this == 0) {
        array_size_this = 1;
    }
    size_t array_size_other = *other.getArraySize();
    if (array_size_other == 0) {
        array_size_other = 1;
    }
    return (getName() == other.getName() && getBitSize() == other.getBitSize() &&
            array_size_this == array_size_other && getUnitName() == other.getUnitName());
    // we do not compare description
    // we do not compare minimum and maximum, but unit!
    // we do not compare default alignment!
}

bool DataType::operator!=(const DataType& other) const
{
    return !operator==(other);
}

TypeOfType DataType::getTypeOfType() const
{
    return data_type;
}

size_t DataType::getBitSize() const // override
{
    return _bit_size;
}

void DataType::setBitSize(size_t size)
{
    if (_bit_size != size) {
        _bit_size = size;
        notify("bit_size");
    }
}

OptionalSize DataType::getArraySize() const
{
    return _array_size;
}

void DataType::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = TypeBase::getName();
    if (old_name != name) {
        TypeBase::setName(name);
        try {
            utility::TypeAccessMapSubject<DataType>::notifyChanged(
                utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
            ModelSubject<DataType>::notifyChanged(item_renamed, *this, old_name);
        }
        catch (const dd::Error& error) {
            // keep consistent .. this exeption does not invalidate the whole model
            TypeBase::setName(old_name);
            throw error;
        }
        notify("name");
    }
}

void DataType::setArraySize(OptionalSize array_size)
{
    if (array_size) {
        _array_size = array_size;
        notify("array_size");
    }
}

const std::string& DataType::getUnitName() const
{
    return _unit_name;
}

void DataType::setUnitName(const std::string& unit_name)
{
    if (_unit_name != unit_name) {
        _unit_name = unit_name;
        notify("unit_name");
    }
}

const std::string& DataType::getMin() const
{
    return _minimum_value;
}

void DataType::setMin(const std::string& minimum_value)
{
    if (_minimum_value != minimum_value) {
        _minimum_value = minimum_value;
        notify("min");
    }
}

const std::string& DataType::getMax() const
{
    return _maximum_value;
}

void DataType::setMax(const std::string& maximum_value)
{
    if (_maximum_value != maximum_value) {
        _maximum_value = maximum_value;
        notify("max");
    }
}

const std::string& DataType::getDescription() const
{
    return _description;
}

void DataType::setDescription(const std::string& description)
{
    if (_description != description) {
        _description = description;
        notify("description");
    }
}

const OptionalSize& DataType::getDefaultAlignment() const
{
    return _default_type_alignment;
}

void DataType::notify(const std::string& additional_info)
{
    ModelSubject<DataType>::notifyChanged(item_changed, *this, additional_info);
    utility::TypeAccessMapSubject<DataType>::notifyChanged(
        utility::TypeAccessMapEventCode::map_item_changed, *this, additional_info);
}
/*************************************************************************************************************/
// EnumType
/*************************************************************************************************************/
EnumType::EnumType(const std::string& name,
                   const std::string& data_type_name,
                   const std::vector<Element>& elements)
    : TypeBase(name),
      ModelSubject<EnumType>(),
      utility::TypeAccessMapSubject<EnumType>(),
      InfoMap(),
      _data_type_name(data_type_name),
      _elements(this, "dd::EnumTypes::Elements")
{
    for (const auto& elem: elements) {
        _elements.add(elem);
    }
}

EnumType::EnumType(const EnumType& other)
    : TypeBase(),
      ModelSubject<EnumType>(),
      utility::TypeAccessMapSubject<EnumType>(),
      InfoMap(),
      _elements(this, "dd::EnumTypes::Elements")
{
    *this = other;
}

EnumType& EnumType::operator=(const EnumType& other)
{
    TypeBase::operator=(other);
    _data_type_name = other._data_type_name;
    other._elements.deepCopy(_elements, this);
    return *this;
}

EnumType::EnumType(EnumType&& other)
    : TypeBase(),
      ModelSubject<EnumType>(),
      utility::TypeAccessMapSubject<EnumType>(),
      InfoMap(),
      _elements(this, "dd::EnumTypes::Elements")
{
    TypeBase::operator=(other);
    std::swap(_data_type_name, other._data_type_name);
    std::swap(_elements, other._elements);
    _elements._validator = this;
}

EnumType& EnumType::operator=(EnumType&& other)
{
    TypeBase::operator=(other);
    std::swap(_data_type_name, other._data_type_name);
    std::swap(_elements, other._elements);
    _elements._validator = this;
    return *this;
}

bool EnumType::operator==(const EnumType& other) const
{
    return getDataTypeName() == other.getDataTypeName() && getName() == other.getName() &&
           _elements == other._elements;
}

bool EnumType::operator!=(const EnumType& other) const
{
    return !operator==(other);
}

const std::string& EnumType::getDataTypeName() const
{
    return _data_type_name;
}

void EnumType::setDataTypeName(const std::string& data_type_name)
{
    if (_data_type_name != data_type_name) {
        _data_type_name = data_type_name;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "data_type_name");
    }
}

TypeOfType EnumType::getTypeOfType() const
{
    return enum_type;
}

void EnumType::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = TypeBase::getName();
    if (old_name != name) {
        TypeBase::setName(name);
        try {
            utility::TypeAccessMapSubject<EnumType>::notifyChanged(
                utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
            ModelSubject<EnumType>::notifyChanged(item_renamed, *this, old_name);
        }
        catch (const dd::Error& error) {
            // keep consistent .. this exeption does not invalidate the whole model
            TypeBase::setName(old_name);
            throw error;
        }
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "name");
    }
}

const EnumType::Elements& EnumType::getElements() const
{
    return _elements;
}

EnumType::Elements& EnumType::getElements()
{
    return _elements;
}

bool EnumType::validateContains(const Elements::access_type& element) const
{
    return _elements.contains(element.getName());
}

void EnumType::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                       Elements::access_type&,
                                       const std::string& additional_info)
{
    using namespace utility;
    // we need to transfer this to a changed event
    ModelEventCode local_code = item_changed;
    TypeAccessMapEventCode forwarded_code = map_item_changed;
    switch (code) {
    case map_item_added:
        local_code = subitem_added;
        forwarded_code = map_subitem_added;
        break;
    case map_item_removed:
        local_code = subitem_removed;
        forwarded_code = map_subitem_removed;
        break;
    case map_item_changed:
        local_code = subitem_changed;
        forwarded_code = map_subitem_changed;
        break;
    case map_item_renamed:
        local_code = subitem_renamed;
        forwarded_code = map_subitem_renamed;
        break;
    // all others should not appear
    case map_subitem_added:
    case map_subitem_removed:
    case map_subitem_changed:
    case map_subitem_renamed:
    case map_subitem_popped:
    case map_subitem_inserted:
        break;
    }
    notify(local_code, forwarded_code, additional_info);
}

void EnumType::notify(ModelEventCode code,
                      utility::TypeAccessMapEventCode code_forward,
                      const std::string& additional_info)
{
    ModelSubject<EnumType>::notifyChanged(code, *this, additional_info);
    utility::TypeAccessMapSubject<EnumType>::notifyChanged(code_forward, *this, additional_info);
}

/*************************************************************************************************************/
// StructType SerializedInfo
/*************************************************************************************************************/
StructType::SerializedInfo::SerializedInfo(OptionalSize byte_pos,
                                           ByteOrder byte_order,
                                           OptionalSize bit_pos,
                                           OptionalSize num_bits)
    : _byte_pos(byte_pos), _byte_order(byte_order), _bit_pos(bit_pos), _num_bits(num_bits)
{
}

bool StructType::SerializedInfo::operator==(const SerializedInfo& other) const
{
    return getBytePos() == other.getBytePos() && getByteOrder() == other.getByteOrder() &&
           getBitPos() == other.getBitPos() && getNumBits() == other.getNumBits();
}

StructType::SerializedInfo::SerializedInfo(const std::initializer_list<size_t>& intializer)
    : SerializedInfo()
{
    size_t current_idx = 0;
    for (const auto& current: intializer) {
        switch (current_idx) {
        case 0:
            setBytePos(current);
            break;
        case 1:
            setByteOrder(static_cast<ByteOrder>(current));
            break;
        case 2:
            setBitPos(current);
            break;
        case 3:
            setNumBits(current);
            break;
        default:
            break;
        }
        current_idx++;
    }
}

OptionalSize StructType::SerializedInfo::getBytePos() const
{
    return _byte_pos;
}

void StructType::SerializedInfo::setBytePos(OptionalSize byte_pos, bool prevent_notification)
{
    if (_byte_pos != byte_pos) {
        _byte_pos = byte_pos;
        if (!prevent_notification) {
            notifyChangeSerialized("byte_pos");
        }
    }
}

OptionalSize StructType::SerializedInfo::getBitPos() const
{
    return _bit_pos;
}

void StructType::SerializedInfo::setBitPos(OptionalSize bit_pos, bool prevent_notification)
{
    if (_bit_pos != bit_pos) {
        _bit_pos = bit_pos;
        if (!prevent_notification) {
            notifyChangeSerialized("bit_pos");
        }
    }
}

OptionalSize StructType::SerializedInfo::getNumBits() const
{
    return _num_bits;
}

void StructType::SerializedInfo::setNumBits(OptionalSize num_bits)
{
    if (_num_bits != num_bits) {
        _num_bits = num_bits;
        notifyChangeSerialized("num_bits");
    }
}

ByteOrder StructType::SerializedInfo::getByteOrder() const
{
    return _byte_order;
}

void StructType::SerializedInfo::setByteOrder(ByteOrder byte_order)
{
    if (_byte_order != byte_order) {
        _byte_order = byte_order;
        notifyChangeSerialized("byte_order");
    }
}

/*************************************************************************************************************/
// StructType DeserializedInfo
/*************************************************************************************************************/
StructType::DeserializedInfo::DeserializedInfo(size_t alignment) : _alignment(alignment)
{
}

StructType::DeserializedInfo::DeserializedInfo(const std::initializer_list<size_t>& intializer)
    : DeserializedInfo()
{
    auto first = intializer.begin();
    if (first != intializer.end()) {
        setAlignment(*first);
    }
}

bool StructType::DeserializedInfo::operator==(const DeserializedInfo& other) const
{
    return _alignment == other.getAlignment();
}

size_t StructType::DeserializedInfo::getAlignment() const
{
    return _alignment;
}

void StructType::DeserializedInfo::setAlignment(size_t alignment)
{
    if (_alignment != alignment) {
        _alignment = alignment;
        notifyChangeDeserialized("alignment");
    }
}

/*************************************************************************************************************/
// StructType Element
/*************************************************************************************************************/
StructType::Element::Element(const std::string& name,
                             const std::string& type_name,
                             const DeserializedInfo& deserialized_info,
                             const SerializedInfo& serialized_info,
                             const dd::ArraySize& array_size,
                             const std::string& description,
                             const std::string& comment,
                             const std::string& unit_name,
                             const std::string& value,
                             const std::string& minimum_value,
                             const std::string& maximum_value,
                             const std::string& default_value,
                             const std::string& scale,
                             const std::string& offset)
    : _name(name),
      _type_name(type_name),
      DeserializedInfo(deserialized_info),
      SerializedInfo(serialized_info),
      _array_size(array_size),
      _description(description),
      _comment(comment),
      _unit_name(unit_name),
      _value(value),
      _minimum_value(minimum_value),
      _maximum_value(maximum_value),
      _default_value(default_value),
      _scale(scale),
      _offset(offset)
{
}

bool StructType::Element::operator==(const Element& other) const
{
    return SerializedInfo::operator==(other) && DeserializedInfo::operator==(other) &&
           getArraySize() == other.getArraySize() && getUnitName() == other.getUnitName() &&
           getTypeName() == other.getTypeName() && getScale() == other.getScale() &&
           getOffset() == other.getOffset();
}

bool StructType::Element::operator!=(const Element& other) const
{
    return !operator==(other);
}

const std::string& StructType::Element::getName() const
{
    return _name;
}

void StructType::Element::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = _name;
    if (old_name != name) {
        _name = name;
        try {
            notifyChanged(utility::TypeAccessListEventCode::list_item_renamed, *this, old_name);
        }
        catch (const dd::Error& error) {
            // keep consistent .. this exeption does not invalidate the whole model
            _name = old_name;
            throw error;
        }

        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "name");
    }
}

const std::string& StructType::Element::getTypeName() const
{
    return _type_name;
}

void StructType::Element::setTypeName(const std::string& type_name)
{
    if (_type_name != type_name) {
        _type_name = type_name;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "type_name");
    }
}

// description String optional Description of the created data type
const std::string& StructType::Element::getDescription() const
{
    return _description;
}

void StructType::Element::setDescription(const std::string& description)
{
    if (_description != description) {
        _description = description;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "description");
    }
}

// comment Text optional Additional comments
const std::string& StructType::Element::getComment() const
{
    return _comment;
}

void StructType::Element::setComment(const std::string& comment)
{
    if (_comment != comment) {
        _comment = comment;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "comment");
    }
}

const ArraySize& StructType::Element::getArraySize() const
{
    return _array_size;
}

void StructType::Element::setArraySize(const ArraySize& array_size)
{
    if (_array_size != array_size) {
        _array_size = array_size;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "array_size");
    }
}

const std::string& StructType::Element::getUnitName() const
{
    return _unit_name;
}
void StructType::Element::setUnitName(const std::string& unit_name)
{
    if (_unit_name != unit_name) {
        _unit_name = unit_name;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "unit_name");
    }
}

const std::string& StructType::Element::getValue() const
{
    return _value;
}
void StructType::Element::setValue(const std::string& value)
{
    if (_value != value) {
        _value = value;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "value");
    }
}

const std::string& StructType::Element::getMin() const
{
    return _minimum_value;
}
void StructType::Element::setMin(const std::string& minimum_value)
{
    if (_minimum_value != minimum_value) {
        _minimum_value = minimum_value;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "min");
    }
}

const std::string& StructType::Element::getMax() const
{
    return _maximum_value;
}
void StructType::Element::setMax(const std::string& maximum_value)
{
    if (_maximum_value != maximum_value) {
        _maximum_value = maximum_value;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "max");
    }
}

const std::string& StructType::Element::getDefault() const
{
    return _default_value;
}
void StructType::Element::setDefault(const std::string& default_value)
{
    if (_default_value != default_value) {
        _default_value = default_value;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "default");
    }
}

const std::string& StructType::Element::getScale() const
{
    return _scale;
}

void StructType::Element::setScale(const std::string& scale)
{
    if (_scale != scale) {
        _scale = scale;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "scale");
    }
}

const std::string& StructType::Element::getOffset() const
{
    return _offset;
}
void StructType::Element::setOffset(const std::string& offset)
{
    if (_offset != offset) {
        _offset = offset;
        notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, "offset");
    }
}

void StructType::Element::notifyChangeSerialized(const std::string& additional_info)
{
    notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, additional_info);
}

void StructType::Element::notifyChangeDeserialized(const std::string& additional_info)
{
    notifyChanged(utility::TypeAccessListEventCode::list_item_changed, *this, additional_info);
}

/*************************************************************************************************************/
// StructType
/*************************************************************************************************************/
class NamedContainerInfoStructType : public datamodel::Info<NamedContainerInfoStructType> {
public:
    /// the type info id to use the @ref datamodel::Info template.
    static constexpr const uint8_t INFO_TYPE_ID = ddl::dd::InfoType::named_container_info;

    const StructType::Elements::container_named_type* getContainer() const
    {
        return &_named_container;
    }
    StructType::Elements::container_named_type* getContainer()
    {
        return &_named_container;
    }

private:
    StructType::Elements::container_named_type _named_container;
};

StructType::StructType(const std::string& name,
                       const std::string& struct_version,
                       OptionalSize alignment,
                       const std::string& comment,
                       dd::Version ddl_version,
                       const std::vector<Element>& elements)
    : TypeBase(name),
      ModelSubject<StructType>(),
      utility::TypeAccessMapSubject<StructType>(),
      InfoMap(),
      _alignment(alignment),
      _struct_version(struct_version),
      _comment(comment),
      _ddl_version(ddl_version),
      _elements(this, "datamodel::StructType::Elements")
{
    setInfo<NamedContainerInfoStructType>(std::make_shared<NamedContainerInfoStructType>());
    for (auto& ref_elem: elements) {
        getElements().add(ref_elem);
    }
}

StructType& StructType::operator=(const StructType& other)
{
    TypeBase::operator=(other);
    _alignment = other._alignment;
    _struct_version = other._struct_version;
    _comment = other._comment;
    _ddl_version = other._ddl_version;
    other._elements.deepCopy(_elements, this);
    return *this;
}

StructType::StructType(const StructType& other)
    : TypeBase(),
      ModelSubject<StructType>(),
      utility::TypeAccessMapSubject<StructType>(),
      InfoMap(),
      _elements(this, "datamodel::StructType::Elements")
{
    setInfo<NamedContainerInfoStructType>(std::make_shared<NamedContainerInfoStructType>());
    *this = other;
}

StructType::StructType(StructType&& other)
    : TypeBase(),
      ModelSubject<StructType>(),
      utility::TypeAccessMapSubject<StructType>(),
      InfoMap(),
      _elements(std::move(other._elements))
{
    setInfo<NamedContainerInfoStructType>(std::make_shared<NamedContainerInfoStructType>());
    TypeBase::operator=(other);
    std::swap(_alignment, other._alignment);
    std::swap(_struct_version, other._struct_version);
    std::swap(_comment, other._comment);
    std::swap(_ddl_version, other._ddl_version);
    _elements.setValidator(this);
}

StructType& StructType::operator=(StructType&& other)
{
    TypeBase::operator=(other);
    std::swap(_alignment, other._alignment);
    std::swap(_struct_version, other._struct_version);
    std::swap(_comment, other._comment);
    std::swap(_ddl_version, other._ddl_version);
    std::swap(_elements, other._elements);
    _elements.setValidator(this);
    return *this;
}

bool StructType::operator==(const StructType& other) const
{
    return _alignment == other.getAlignment() && _struct_version == other.getVersion() &&
           _ddl_version == other.getLanguageVersion();
}

bool StructType::operator!=(const StructType& other) const
{
    return !operator==(other);
}

OptionalSize StructType::getAlignment() const
{
    return _alignment;
}

void StructType::setAlignment(OptionalSize alignment)
{
    if (_alignment != alignment) {
        _alignment = alignment;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "alignment");
    }
}

const std::string& StructType::getComment() const
{
    return _comment;
}

void StructType::setComment(const std::string& comment)
{
    if (_comment != comment) {
        _comment = comment;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "comment");
    }
}

const std::string& StructType::getVersion() const
{
    return _struct_version;
}

void StructType::setVersion(const std::string& struct_version)
{
    if (_struct_version != struct_version) {
        _struct_version = struct_version;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "version");
    }
}

Version StructType::getLanguageVersion() const
{
    return _ddl_version;
}

void StructType::setLanguageVersion(const Version& ddl_version)
{
    if (_ddl_version != ddl_version) {
        _ddl_version = ddl_version;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "language_version");
    }
}

TypeOfType StructType::getTypeOfType() const
{
    return struct_type;
}

void StructType::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = TypeBase::getName();
    if (old_name != name) {
        TypeBase::setName(name);
        try {
            utility::TypeAccessMapSubject<StructType>::notifyChanged(
                utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
            ModelSubject<StructType>::notifyChanged(item_renamed, *this, old_name);
        }
        catch (const dd::Error& error) {
            // keep consistent .. this exeption does not invalidate the whole model
            TypeBase::setName(old_name);
            throw error;
        }
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "name");
    }
}

const StructType::Elements& StructType::getElements() const
{
    return _elements;
}

StructType::Elements& StructType::getElements()
{
    return _elements;
}

bool StructType::validateContains(const Elements::access_type& element) const
{
    return _elements.contains(element.getName());
}

void StructType::notifyChangedListContent(utility::TypeAccessListEventCode code,
                                          Elements::access_type&,
                                          const std::string& additional_info)
{
    using namespace utility;
    // we need to transfer this to a changed event
    ModelEventCode local_code = item_changed;
    TypeAccessMapEventCode forwarded_code = map_item_changed;
    switch (code) {
    case list_item_added:
        local_code = subitem_added;
        forwarded_code = map_subitem_added;
        break;
    case list_item_removed:
        local_code = subitem_removed;
        forwarded_code = map_subitem_removed;
        break;
    case list_item_changed:
        local_code = subitem_changed;
        forwarded_code = map_subitem_changed;
        break;
    case list_item_renamed:
        local_code = subitem_renamed;
        forwarded_code = map_subitem_renamed;
        break;
    case list_item_popped:
        local_code = subitem_popped;
        forwarded_code = map_subitem_popped;
        break;
    case list_item_inserted:
        local_code = subitem_inserted;
        forwarded_code = map_subitem_inserted;
        break;
        // all others should not appear
    case list_subitem_added:
    case list_subitem_removed:
    case list_subitem_changed:
    case list_subitem_popped:
    case list_subitem_inserted:
        break;
    }
    notify(local_code, forwarded_code, additional_info);
}

void StructType::notify(ModelEventCode code,
                        utility::TypeAccessMapEventCode forward_code,
                        const std::string& additional_info)
{
    ModelSubject<StructType>::notifyChanged(code, *this, additional_info);
    utility::TypeAccessMapSubject<StructType>::notifyChanged(forward_code, *this, additional_info);
}

const StructType::Elements::container_named_type* StructType::getNamedItemList() const
{
    return getInfo<NamedContainerInfoStructType>()->getContainer();
}

StructType::Elements::container_named_type* StructType::getNamedItemList()
{
    return getInfo<NamedContainerInfoStructType>()->getContainer();
}

/*************************************************************************************************************/
// StreamMetaType
/*************************************************************************************************************/
StreamMetaType::StreamMetaType(const std::string& name,
                               const std::string& version,
                               const std::string& parent,
                               const std::vector<Property>& properties)
    : TypeBase(name),
      ModelSubject<StreamMetaType>(),
      utility::TypeAccessMapSubject<StreamMetaType>(),
      InfoMap(),
      _version(version),
      _parent(parent),
      _properties(this, "dd::StreamMetaType::Properties")
{
    for (const auto& prop: properties) {
        _properties.add(prop);
    }
}

StreamMetaType::StreamMetaType(const StreamMetaType& other)
    : TypeBase(),
      ModelSubject<StreamMetaType>(),
      utility::TypeAccessMapSubject<StreamMetaType>(),
      InfoMap(),
      _properties(this, "dd::StreamMetaType::Properties")
{
    *this = other;
}

StreamMetaType& StreamMetaType::operator=(const StreamMetaType& other)
{
    // TODO: check if this needs to notify!
    _name = other._name;
    _version = other._version;
    _parent = other._parent;
    other._properties.deepCopy(_properties, this);
    return *this;
}

StreamMetaType::StreamMetaType(StreamMetaType&& other)
    : TypeBase(),
      ModelSubject<StreamMetaType>(),
      utility::TypeAccessMapSubject<StreamMetaType>(),
      InfoMap(),
      _properties(this, "dd::StreamMetaType::Properties")
{
    std::swap(_name, other._name);
    std::swap(_version, other._version);
    std::swap(_parent, other._parent);
    std::swap(_properties, other._properties);
    _properties._validator = this;
}

StreamMetaType& StreamMetaType::operator=(StreamMetaType&& other)
{
    std::swap(_name, other._name);
    std::swap(_version, other._version);
    std::swap(_parent, other._parent);
    std::swap(_properties, other._properties);
    _properties._validator = this;
    return *this;
}

bool StreamMetaType::operator==(const StreamMetaType& other) const
{
    return getName() == other.getName() && getVersion() == other.getVersion() &&
           getParent() == other.getParent() && _properties == other._properties;
}

bool StreamMetaType::operator!=(const StreamMetaType& other) const
{
    return !operator==(other);
}

const std::string& StreamMetaType::getParent() const
{
    return _parent;
}

void StreamMetaType::setParent(const std::string& parent)
{
    if (_parent != parent) {
        _parent = parent;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "parent");
    }
}

const std::string& StreamMetaType::getVersion() const
{
    return _version;
}

void StreamMetaType::setVersion(const std::string& version)
{
    if (_version != version) {
        _version = version;
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "version");
    }
}

TypeOfType StreamMetaType::getTypeOfType() const
{
    return stream_meta_type;
}

void StreamMetaType::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = TypeBase::getName();
    if (old_name != name) {
        TypeBase::setName(name);
        try {
            utility::TypeAccessMapSubject<StreamMetaType>::notifyChanged(
                utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
            ModelSubject<StreamMetaType>::notifyChanged(item_renamed, *this, old_name);
        }
        catch (const dd::Error& error) {
            // keep consistent .. this exeption does not invalidate the whole model
            TypeBase::setName(old_name);
            throw error;
        }
        notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "name");
    }
}

const StreamMetaType::Properties& StreamMetaType::getProperties() const
{
    return _properties;
}

StreamMetaType::Properties& StreamMetaType::getProperties()
{
    return _properties;
}

bool StreamMetaType::validateContains(const Properties::access_type& prop) const
{
    return _properties.contains(prop.getName());
}

void StreamMetaType::notifyChangedMapContent(utility::TypeAccessMapEventCode code,
                                             Properties::access_type&,
                                             const std::string& additional_info)
{
    using namespace utility;
    // we need to transfer this to a changed event
    ModelEventCode local_code = item_changed;
    TypeAccessMapEventCode forwarded_code = map_item_changed;
    switch (code) {
    case map_item_added:
        local_code = subitem_added;
        forwarded_code = map_subitem_added;
        break;
    case map_item_removed:
        local_code = subitem_removed;
        forwarded_code = map_subitem_removed;
        break;
    case map_item_changed:
        local_code = subitem_changed;
        forwarded_code = map_subitem_changed;
        break;
    case map_item_renamed:
        local_code = subitem_renamed;
        forwarded_code = map_subitem_renamed;
        break;
        // all others should not appear
    case map_subitem_added:
    case map_subitem_removed:
    case map_subitem_changed:
    case map_subitem_renamed:
    case map_subitem_popped:
    case map_subitem_inserted:
        break;
    }
    notify(local_code, forwarded_code, additional_info);
}

void StreamMetaType::notify(ModelEventCode code,
                            utility::TypeAccessMapEventCode code_forward,
                            const std::string& additional_info)
{
    ModelSubject<StreamMetaType>::notifyChanged(code, *this, additional_info);
    utility::TypeAccessMapSubject<StreamMetaType>::notifyChanged(
        code_forward, *this, additional_info);
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
