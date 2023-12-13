/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <ddl/datamodel/datamodel_streams.h>
#include <ddl/dd/dd_infomodel_type.h>

#include <exception>
#include <utility>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief StreamStruct
 *
 */
Stream::Struct::Struct(const std::string& name, // empty name is allowed (says xsd)
                       const std::string& type_name,
                       OptionalSize byte_pos)
    : _name(name), _type_name(type_name), _byte_pos(byte_pos)
{
}

bool Stream::Struct::operator==(const Struct& other) const
{
    return getBytePos() == other.getBytePos() && getName() == other.getName() &&
           getTypeName() == other.getTypeName();
}

bool Stream::Struct::operator!=(const Struct& other) const
{
    return !operator==(other);
}

const std::string& Stream::Struct::getName() const
{
    return _name;
}

void Stream::Struct::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = _name;
    _name = name;
    try {
        notifyChanged(utility::TypeAccessListEventCode::list_item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        _name = old_name;
        throw error;
    }

    notifyChanged({}, *this, "name");
}

const std::string& Stream::Struct::getTypeName() const
{
    return _type_name;
}

void Stream::Struct::setTypeName(const std::string& type_name)
{
    _type_name = type_name;
    notifyChanged({}, *this, "type_name");
}

OptionalSize Stream::Struct::getBytePos() const
{
    return _byte_pos;
}

void Stream::Struct::setBytePos(OptionalSize byte_pos)
{
    _byte_pos = byte_pos;
    notifyChanged({}, *this, "byte_pos");
}

class NamedContainerInfoStream : public datamodel::Info<NamedContainerInfoStream> {
public:
    /// the type info id to use the @ref datamodel::Info template.
    static constexpr const uint8_t INFO_TYPE_ID = ddl::dd::InfoType::named_container_info;

    const Stream::Structs::container_named_type* getContainer() const
    {
        return &_named_container;
    }
    Stream::Structs::container_named_type* getContainer()
    {
        return &_named_container;
    }

private:
    Stream::Structs::container_named_type _named_container;
};
/**
 * @brief Stream
 *
 */
Stream::Stream(const Stream& other)
    : ModelSubject<Stream>(),
      utility::TypeAccessMapSubject<Stream>(),
      InfoMap(),
      _structs(this, "Stream::Struct")
{
    setInfo<NamedContainerInfoStream>(std::make_shared<NamedContainerInfoStream>());
    operator=(other);
}

Stream& Stream::operator=(const Stream& other)
{
    _name = other._name;
    _stream_type_name = other._stream_type_name;
    _description = other._description;
    other._structs.deepCopy(_structs, this);
    return *this;
}

Stream::Stream(Stream&& other)
    : ModelSubject<Stream>(),
      utility::TypeAccessMapSubject<Stream>(),
      InfoMap(),
      _structs(this, "Stream::Struct")
{
    setInfo<NamedContainerInfoStream>(std::make_shared<NamedContainerInfoStream>());
    std::swap(_name, other._name);
    std::swap(_stream_type_name, other._stream_type_name);
    std::swap(_description, other._description);
    std::swap(_structs, other._structs);
    _structs.setValidator(this);
}

Stream& Stream::operator=(Stream&& other)
{
    std::swap(_name, other._name);
    std::swap(_stream_type_name, other._stream_type_name);
    std::swap(_description, other._description);
    std::swap(_structs, other._structs);
    _structs.setValidator(this);
    return *this;
}

Stream::Stream(const std::string& name,
               const std::string& stream_type_name,
               const std::string& description,
               const std::vector<Struct>& structs)
    : ModelSubject<Stream>(),
      utility::TypeAccessMapSubject<Stream>(),
      InfoMap(),
      _name(name),
      _stream_type_name(stream_type_name),
      _description(description),
      _structs(this, "Stream::Struct")
{
    setInfo<NamedContainerInfoStream>(std::make_shared<NamedContainerInfoStream>());
    for (const auto& stream_struct: structs) {
        getStructs().add(stream_struct);
    }
}

bool Stream::operator==(const Stream& other) const
{
    return getStreamTypeName() == other.getStreamTypeName() && getName() == other.getName() &&
           _structs == other._structs;
}

bool Stream::operator!=(const Stream& other) const
{
    return !operator==(other);
}

void Stream::setName(const std::string& name)
{
    // Keep old name for reset
    std::string old_name = _name;
    _name = name;
    try {
        utility::TypeAccessMapSubject<Stream>::notifyChanged(
            utility::TypeAccessMapEventCode::map_item_renamed, *this, old_name);
        ModelSubject<Stream>::notifyChanged(item_renamed, *this, old_name);
    }
    catch (const dd::Error& error) {
        // keep consistent .. this exeption does not invalidate the whole model
        _name = old_name;
        throw error;
    }
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "name");
}

const std::string& Stream::getName() const
{
    return _name;
}

void Stream::setStreamTypeName(const std::string& stream_type_name)
{
    _stream_type_name = stream_type_name;
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "stream_type_name");
}

const std::string& Stream::getStreamTypeName() const
{
    return _stream_type_name;
}

const std::string& Stream::getDescription() const
{
    return _description;
}

void Stream::setDescription(const std::string& description)
{
    _description = description;
    notify(item_changed, utility::TypeAccessMapEventCode::map_item_changed, "description");
}

const Stream::Structs& Stream::getStructs() const
{
    return _structs;
}

Stream::Structs& Stream::getStructs()
{
    return _structs;
}

bool Stream::validateContains(const Structs::access_type& stream_struct) const
{
    return _structs.contains(stream_struct.getName());
}

void Stream::notifyChangedListContent(utility::TypeAccessListEventCode code,
                                      Structs::access_type&,
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

void Stream::notify(ModelEventCode code,
                    utility::TypeAccessMapEventCode map_code,
                    const std::string& additional_info)
{
    ModelSubject<Stream>::notifyChanged(code, *this, additional_info);
    utility::TypeAccessMapSubject<Stream>::notifyChanged(map_code, *this, additional_info);
}

const Stream::Structs::container_named_type* Stream::getNamedItemList() const
{
    return getInfo<NamedContainerInfoStream>()->getContainer();
}

Stream::Structs::container_named_type* Stream::getNamedItemList()
{
    return getInfo<NamedContainerInfoStream>()->getContainer();
}

} // namespace datamodel
} // namespace dd
} // namespace ddl
