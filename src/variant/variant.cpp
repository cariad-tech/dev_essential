/**
 * @file
 * Variant container
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

#include <a_util/memory.h>
#include <a_util/strings/strings_convert.h>
#include <a_util/strings/strings_format.h>
#include <a_util/strings/strings_functions.h>
#include <a_util/variant/variant.h>

#include <stdexcept>
#include <vector>

namespace a_util {
namespace variant {
// using this in member initialization list
#pragma warning(disable : 4355)

/// Helper template, to map T to the respective VariantType
template <typename T>
struct VariantTraits {
    static const VariantType value;
};

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic ignored                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

template <>
const VariantType VariantTraits<bool>::value = VT_Bool;
template <>
const VariantType VariantTraits<std::int8_t>::value = VT_Int8;
template <>
const VariantType VariantTraits<std::uint8_t>::value = VT_UInt8;
template <>
const VariantType VariantTraits<std::int16_t>::value = VT_Int16;
template <>
const VariantType VariantTraits<std::uint16_t>::value = VT_UInt16;
template <>
const VariantType VariantTraits<std::int32_t>::value = VT_Int32;
template <>
const VariantType VariantTraits<std::uint32_t>::value = VT_UInt32;
template <>
const VariantType VariantTraits<std::int64_t>::value = VT_Int64;
template <>
const VariantType VariantTraits<std::uint64_t>::value = VT_UInt64;
template <>
const VariantType VariantTraits<float>::value = VT_Float;
template <>
const VariantType VariantTraits<double>::value = VT_Double;

#if defined(__GNUC__) && (__GNUC__ == 5) && defined(__QNX__)
#pragma GCC diagnostic warning                                                                     \
    "-Wattributes" // standard type attributes are ignored when used in templates
#endif

/// Helper array, to map VariantType to its respective sizeof(T)
static const std::int8_t variant_type_sizes[] = {
    0,                     // VT_Empty
    sizeof(bool),          // VT_Bool
    sizeof(std::int8_t),   // VT_Int8
    sizeof(std::uint8_t),  // VT_UInt8
    sizeof(std::int16_t),  // VT_Int16
    sizeof(std::uint16_t), // VT_UInt16
    sizeof(std::int32_t),  // VT_Int32
    sizeof(std::uint32_t), // VT_UInt32
    sizeof(std::int64_t),  // VT_Int64
    sizeof(std::uint64_t), // VT_UInt64
    sizeof(float),         // VT_Float32
    sizeof(double),        // VT_Float64
    -1                     // VT_String
};

// Variant implementation (d-ptr)
class Variant::Implementation {
private:
    Implementation& operator=(const Implementation&) = delete;
    Implementation(Implementation&&) noexcept = default;
    Implementation& operator=(Implementation&&) noexcept = default;

public:
    /// The current variant type
    VariantType _stored_type;
    std::uint8_t* _array_storage;
    std::size_t _array_size;

    /// Union to actually store element values
    union {
        bool bool_val;
        std::int8_t int8_val;
        std::uint8_t uint8_val;
        std::int16_t int16_val;
        std::uint16_t uint16_val;
        std::int32_t int32_val;
        std::uint32_t uint32_val;
        std::int64_t int64_val;
        std::uint64_t uint64_val;
        float float_val;
        double double_val;
    } _value;

    Implementation() : _stored_type(VT_Empty), _array_storage(nullptr), _array_size(0)
    {
    }

    Implementation(const Implementation& other_var)
        : _stored_type(other_var._stored_type),
          _array_storage(nullptr),
          _array_size(other_var._array_size),
          _value(other_var._value)
    {
        if (other_var._array_storage) {
            std::size_t storage_size;
            if (_stored_type == VT_String) {
                storage_size = _array_size + 1;
            }
            else {
                storage_size = _array_size * variant_type_sizes[_stored_type];
            }
            _array_storage = new std::uint8_t[storage_size];
            a_util::memory::copy(
                _array_storage, storage_size, other_var._array_storage, storage_size);
        }
    }

    ~Implementation()
    {
        reset(VT_Empty);
    }

    /// Resets the variant to the empty state
    void reset(VariantType new_type)
    {
        _stored_type = new_type;
        delete[] _array_storage;
        _array_storage = nullptr;
        _array_size = 0;
    }

    /// Resets the variant to a string
    void reset(const char* value)
    {
        if (!value)
            throw std::invalid_argument("value");
        reset(VT_String);

        std::size_t length = strings::getLength(value);

        _array_storage = new std::uint8_t[length + 1];
        _array_storage[length] = 0;
        a_util::memory::copy(_array_storage, length + 1, value, length);

        _array_size = length;
    }

    /// Resets the variant to a single element value
    template <typename T>
    void reset(T value, T& union_ref)
    {
        reset(VariantTraits<T>::value);
        union_ref = value;
    }

    /// Resets the variant to an array value
    template <typename T>
    void ResetArray(const T* new_array_storage, std::size_t new_array_size)
    {
        if (!new_array_storage)
            throw std::invalid_argument("array");
        if (new_array_size == 0)
            throw std::invalid_argument("array_size");
        reset(VariantTraits<T>::value);

        _array_size = new_array_size;
        _array_storage = new std::uint8_t[_array_size * sizeof(T)];
        a_util::memory::copy(
            _array_storage, _array_size * sizeof(T), new_array_storage, new_array_size * sizeof(T));
    }

    /// Get a contained value
    template <typename T>
    T Get(std::size_t array_index, T& union_ref)
    {
        if (_stored_type != VariantTraits<T>::value) {
            throw std::runtime_error("Invalid type");
        }

        if (_array_size == 0) {
            return union_ref;
        }
        else if (array_index >= _array_size) {
            throw std::out_of_range("array_index");
        }
        else {
            return *reinterpret_cast<T*>(_array_storage + array_index * sizeof(T));
        }
    }
};

/// Get a contained value, converted to T
template <typename T>
static T As(const Variant& this_)
{
    switch (this_.getType()) {
    case VT_Empty:
        throw std::runtime_error("Variant is empty!");
    case VT_Bool:
        return static_cast<T>(this_.getBool());
    case VT_Int8:
        return static_cast<T>(this_.getInt8());
    case VT_UInt8:
        return static_cast<T>(this_.getUInt8());
    case VT_Int16:
        return static_cast<T>(this_.getInt16());
    case VT_UInt16:
        return static_cast<T>(this_.getUInt16());
    case VT_Int32:
        return static_cast<T>(this_.getInt32());
    case VT_UInt32:
        return static_cast<T>(this_.getUInt32());
    case VT_Int64:
        return static_cast<T>(this_.getInt64());
    case VT_UInt64:
        return static_cast<T>(this_.getUInt64());
    case VT_Float:
        return static_cast<T>(this_.getFloat());
    case VT_Double:
        return static_cast<T>(this_.getDouble());
    case VT_String:
        return strings::toNumeric<T>(this_.getString());
    }

    throw std::runtime_error("unexpected type");
}

// bool needs an extra wurst to be warning-free
template <>
bool As(const Variant& this_)
{
    switch (this_.getType()) {
    case VT_Empty:
        throw std::runtime_error("Variant is empty!");
    case VT_Bool:
        return this_.getBool();
    case VT_Int8:
        return this_.getInt8() != 0;
    case VT_UInt8:
        return this_.getUInt8() != 0;
    case VT_Int16:
        return this_.getInt16() != 0;
    case VT_UInt16:
        return this_.getUInt16() != 0;
    case VT_Int32:
        return this_.getInt32() != 0;
    case VT_UInt32:
        return this_.getUInt32() != 0;
    case VT_Int64:
        return this_.getInt64() != 0;
    case VT_UInt64:
        return this_.getUInt64() != 0;
    case VT_Float:
        return this_.getFloat() != 0.0f;
    case VT_Double:
        return this_.getDouble() != 0.0;
    case VT_String:
        return (strings::isEqualNoCase(this_.getString(), "true") ||
                strings::isEqualNoCase(this_.getString(), "1") ||
                strings::isEqualNoCase(this_.getString(), "on") ||
                strings::isEqualNoCase(this_.getString(), "tTrue") ||
                strings::isEqualNoCase(this_.getString(), "yes"));
    }

    throw std::runtime_error("unexpected type");
}

Variant::Variant() : _impl(a_util::memory::make_unique<Implementation>())
{
}

Variant::Variant(bool value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.bool_val);
}

Variant::Variant(std::int8_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.int8_val);
}

Variant::Variant(std::uint8_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.uint8_val);
}

Variant::Variant(std::int16_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.int16_val);
}

Variant::Variant(std::uint16_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.uint16_val);
}

Variant::Variant(std::int32_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.int32_val);
}

Variant::Variant(std::uint32_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.uint32_val);
}

Variant::Variant(std::int64_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.int64_val);
}

Variant::Variant(std::uint64_t value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.uint64_val);
}

Variant::Variant(float value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.float_val);
}

Variant::Variant(double value) : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value, _impl->_value.double_val);
}

a_util::variant::Variant::Variant(const char* value)
    : _impl(a_util::memory::make_unique<Implementation>())
{
    _impl->reset(value);
}

Variant::Variant(const Variant& other_var)
    : _impl(a_util::memory::make_unique<Implementation>(*other_var._impl))
{
}

Variant& Variant::operator=(const Variant& other_var)
{
    if (&other_var != this) {
        _impl = a_util::memory::make_unique<Implementation>(*other_var._impl);
    }
    return *this;
}

Variant::~Variant() = default;

Variant::Variant(Variant&& other) : _impl(std::move(other._impl))
{
    other._impl = a_util::memory::make_unique<Implementation>();
}

Variant& Variant::operator=(Variant&& other)
{
    if (this != &other) {
        this->_impl.reset();
        this->_impl = std::move(other._impl);
        other._impl = a_util::memory::make_unique<Implementation>();
    }
    return *this;
}

VariantType Variant::getType() const
{
    return _impl->_stored_type;
}

std::size_t Variant::getArraySize() const
{
    return getType() != VT_String ? _impl->_array_size : 0;
}

bool Variant::isArray() const
{
    return getType() != VT_String && getArraySize() > 1;
}

bool Variant::isEmpty() const
{
    return getType() == VT_Empty;
}

void Variant::reset()
{
    _impl->reset(VT_Empty);
}

void Variant::reset(bool value)
{
    _impl->reset(value, _impl->_value.bool_val);
}

void Variant::reset(std::int8_t value)
{
    _impl->reset(value, _impl->_value.int8_val);
}

void Variant::reset(std::uint8_t value)
{
    _impl->reset(value, _impl->_value.uint8_val);
}

void Variant::reset(std::int16_t value)
{
    _impl->reset(value, _impl->_value.int16_val);
}

void Variant::reset(std::uint16_t value)
{
    _impl->reset(value, _impl->_value.uint16_val);
}

void Variant::reset(std::int32_t value)
{
    _impl->reset(value, _impl->_value.int32_val);
}

void Variant::reset(std::uint32_t value)
{
    _impl->reset(value, _impl->_value.uint32_val);
}

void Variant::reset(std::int64_t value)
{
    _impl->reset(value, _impl->_value.int64_val);
}

void Variant::reset(std::uint64_t value)
{
    _impl->reset(value, _impl->_value.uint64_val);
}

void Variant::reset(float value)
{
    _impl->reset(value, _impl->_value.float_val);
}

void Variant::reset(double value)
{
    _impl->reset(value, _impl->_value.double_val);
}

void Variant::reset(const bool* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::int8_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::uint8_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::int16_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::uint16_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::int32_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::uint32_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::int64_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const std::uint64_t* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const float* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const double* _array_storage, std::size_t _array_size)
{
    _impl->ResetArray(_array_storage, _array_size);
}

void Variant::reset(const char* value)
{
    _impl->reset(value);
}

bool Variant::getBool(std::size_t array_index) const
{
    return _impl->Get<bool>(array_index, _impl->_value.bool_val);
}

std::int8_t Variant::getInt8(std::size_t array_index) const
{
    return _impl->Get<std::int8_t>(array_index, _impl->_value.int8_val);
}

std::uint8_t Variant::getUInt8(std::size_t array_index) const
{
    return _impl->Get<std::uint8_t>(array_index, _impl->_value.uint8_val);
}

std::int16_t Variant::getInt16(std::size_t array_index) const
{
    return _impl->Get<std::int16_t>(array_index, _impl->_value.int16_val);
}

std::uint16_t Variant::getUInt16(std::size_t array_index) const
{
    return _impl->Get<std::uint16_t>(array_index, _impl->_value.uint16_val);
}

std::int32_t Variant::getInt32(std::size_t array_index) const
{
    return _impl->Get<std::int32_t>(array_index, _impl->_value.int32_val);
}

std::uint32_t Variant::getUInt32(std::size_t array_index) const
{
    return _impl->Get<std::uint32_t>(array_index, _impl->_value.uint32_val);
}

std::int64_t Variant::getInt64(std::size_t array_index) const
{
    return _impl->Get<std::int64_t>(array_index, _impl->_value.int64_val);
}

std::uint64_t Variant::getUInt64(std::size_t array_index) const
{
    return _impl->Get<std::uint64_t>(array_index, _impl->_value.uint64_val);
}

float Variant::getFloat(std::size_t array_index) const
{
    return _impl->Get<float>(array_index, _impl->_value.float_val);
}

double Variant::getDouble(std::size_t array_index) const
{
    return _impl->Get<double>(array_index, _impl->_value.double_val);
}

const char* Variant::getString() const
{
    if (getType() != VT_String) {
        throw std::runtime_error("Invalid type");
    }
    return reinterpret_cast<const char*>(_impl->_array_storage);
}

/// Provided for compatibility reasons

float Variant::getFloat32(std::size_t array_index) const
{
    return getFloat(array_index);
}

/// Provided for compatibility reasons

double Variant::getFloat64(std::size_t array_index) const
{
    return getDouble(array_index);
}

bool Variant::asBool() const
{
    return As<bool>(*this);
}

std::int8_t Variant::asInt8() const
{
    return As<std::int8_t>(*this);
}

std::uint8_t Variant::asUInt8() const
{
    return As<std::uint8_t>(*this);
}

std::int16_t Variant::asInt16() const
{
    return As<std::int16_t>(*this);
}

std::uint16_t Variant::asUInt16() const
{
    return As<std::uint16_t>(*this);
}

std::int32_t Variant::asInt32() const
{
    return As<std::int32_t>(*this);
}

std::uint32_t Variant::asUInt32() const
{
    return As<std::uint32_t>(*this);
}

std::int64_t Variant::asInt64() const
{
    return As<std::int64_t>(*this);
}

std::uint64_t Variant::asUInt64() const
{
    return As<std::uint64_t>(*this);
}

float Variant::asFloat() const
{
    return As<float>(*this);
}

double Variant::asDouble() const
{
    return As<double>(*this);
}

std::string Variant::asString() const
{
    switch (getType()) {
    case VT_Bool:
        return (getBool() ? "true" : "false");
    case VT_Int8:
        return strings::toString(getInt8());
    case VT_UInt8:
        return strings::toString(getUInt8());
    case VT_Int16:
        return strings::toString(getInt16());
    case VT_UInt16:
        return strings::toString(getUInt16());
    case VT_Int32:
        return strings::toString(getInt32());
    case VT_UInt32:
        return strings::toString(getUInt32());
    case VT_Int64:
        return strings::toString(getInt64());
    case VT_UInt64:
        return strings::toString(getUInt64());
    case VT_Float:
        return strings::toString(getFloat());
    case VT_Double:
        return strings::toString(getDouble());
    case VT_String:
        return getString();
    case VT_Empty:
        return strings::empty_string;
    }

    return strings::empty_string;
}

Variant::operator bool() const
{
    return asBool();
}

Variant::operator std::int8_t() const
{
    return asInt8();
}

Variant::operator std::uint8_t() const
{
    return asUInt8();
}

Variant::operator std::int16_t() const
{
    return asInt16();
}

Variant::operator std::uint16_t() const
{
    return asUInt16();
}

Variant::operator std::int32_t() const
{
    return asInt32();
}

Variant::operator std::uint32_t() const
{
    return asUInt32();
}

Variant::operator std::int64_t() const
{
    return asInt64();
}

Variant::operator std::uint64_t() const
{
    return asUInt64();
}

Variant::operator float() const
{
    return asFloat();
}

Variant::operator double() const
{
    return asDouble();
}

Variant::operator std::string() const
{
    return asString();
}

Variant& Variant::operator=(bool value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::int8_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::uint8_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::int16_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::uint16_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::int32_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::uint32_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::int64_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(std::uint64_t value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(float value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(double value)
{
    reset(value);
    return *this;
}

Variant& Variant::operator=(const char* value)
{
    reset(value);
    return *this;
}

bool operator==(const Variant& a, const Variant& b)
{
    if (a.getType() != b.getType()) {
        return false;
    }

    if (a.getArraySize() != b.getArraySize()) {
        return false;
    }

    if (a.isArray()) {
        return a_util::memory::compare(a._impl->_array_storage,
                                       a.getArraySize() * variant_type_sizes[a.getType()],
                                       b._impl->_array_storage,
                                       b.getArraySize() * variant_type_sizes[b.getType()]) == 0;
    }

    switch (a.getType()) {
    case VT_Bool:
        return a.getBool() == b.getBool();
    case VT_Int8:
        return a.getInt8() == b.getInt8();
    case VT_UInt8:
        return a.getUInt8() == b.getUInt8();
    case VT_Int16:
        return a.getInt16() == b.getInt16();
    case VT_UInt16:
        return a.getUInt16() == b.getUInt16();
    case VT_Int32:
        return a.getInt32() == b.getInt32();
    case VT_UInt32:
        return a.getUInt32() == b.getUInt32();
    case VT_Int64:
        return a.getInt64() == b.getInt64();
    case VT_UInt64:
        return a.getUInt64() == b.getUInt64();
    case VT_Float:
        return a.getFloat() == b.getFloat();
    case VT_Double:
        return a.getDouble() == b.getDouble();
    case VT_String:
        if (a._impl->_array_size != b._impl->_array_size) {
            return false;
        }
        return a_util::memory::compare(a._impl->_array_storage,
                                       a._impl->_array_size,
                                       b._impl->_array_storage,
                                       b._impl->_array_size) == 0;
    case VT_Empty:
        return true;
    }

    return true;
}

bool operator!=(const Variant& a, const Variant& b)
{
    return !(a == b);
}

} // namespace variant
} // namespace a_util
