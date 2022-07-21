/**
 * @file
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

#include <ddl/dd/dd_common_types.h>
#include <ddl/dd/dd_error.h>
#include <ddl/utilities/std_to_string.h>

#include <sstream>

namespace ddl {
namespace dd {

std::string AlignmentConversion::toString(Alignment alignment)
{
    if (alignment != e_invalid) {
        return std::to_string((int32_t)alignment);
    }

    return "";
}

Alignment AlignmentConversion::fromString(const std::string& alignment,
                                          const Alignment& default_val)
{
    int alignment_int = 0;
    try {
        alignment_int = std::stoi(alignment);
    }
    catch (const std::exception&) {
        return default_val;
    }

    if (alignment_int <= 0) {
        return e1;
    }
    else {
        switch (alignment_int) {
        case 1: {
            return e1;
        }
        case 2: {
            return e2;
        }
        case 4: {
            return e4;
        }
        case 8: {
            return e8;
        }
        case 16: {
            return e16;
        }
        case 32: {
            return e32;
        }
        case 64: {
            return e64;
        }
        default: {
            break;
        }
        }
    }

    return e_invalid;
}

bool AlignmentValidation::isValid(size_t alignment)
{
    switch (alignment) {
    case 0:
    case e1:
    case e2:
    case e4:
    case e8:
    case e16:
    case e32:
    case e64:
        return true;
        break;
    default:
        break;
    }
    return false;
}

std::string ByteOrderConversion::toString(ByteOrder byte_order)
{
    switch (byte_order) {
    case e_le: {
        return std::string("LE");
    }
    case e_be: {
        return std::string("BE");
    }
    default: {
        // error case
        return {};
    }
    }
}

ByteOrder ByteOrderConversion::fromString(const std::string& byte_order,
                                          const ByteOrder& default_val)
{
    if (byte_order == "LE") {
        return e_le;
    }
    if (byte_order == "BE") {
        return e_be;
    }
    if (byte_order == "Motorola") {
        return e_be;
    }
    if (byte_order == "Intel") {
        return e_le;
    }
    // fallback to default
    return default_val;
}

ByteOrder ByteOrderDefault::getPlatformDefault()
{
    union {
        uint16_t number;
        uint8_t bytes[2];
    } test_union;
    test_union.number = 0x1;
    if (test_union.bytes[0] == 1) {
        return ByteOrder::e_le;
    }
    else {
        return ByteOrder::e_be;
    }
}

const Version Version::ddl_version_invalid = Version(0, 0);
const Version Version::ddl_version_notset = Version(0, 0);
const Version Version::ddl_version_10 = Version(1, 0);
const Version Version::ddl_version_11 = Version(1, 1);
const Version Version::ddl_version_12 = Version(1, 2);
const Version Version::ddl_version_20 = Version(2, 0);
const Version Version::ddl_version_30 = Version(3, 0);
const Version Version::ddl_version_40 = Version(4, 0);
const Version Version::ddl_version_41 = Version(4, 1);
const Version Version::ddl_version_current = ddl_version_41;

Version::Version() : Version(Version::getDefaultVersion())
{
}

Version::Version(uint32_t major, uint32_t minor) : _major(major), _minor(minor)
{
}

Version::Version(const std::string& version)
{
    *this = VersionConversion::fromString(version, Version());
}

uint32_t Version::getMajor() const
{
    return _major;
}

uint32_t Version::getMinor() const
{
    return _minor;
}

std::string Version::toString() const
{
    if (*this == Version(1, 1)) {
        return std::string("1.0+");
    }
    std::ostringstream outstr;
    outstr << _major << "." << std::string((_minor < 10) ? "0" : "") << _minor;
    return outstr.str();
}
bool Version::isValidVersion() const
{
    if (*this < ddl_version_10) {
        return false;
    }

    if (*this > ddl_version_current) {
        return false;
    }

    return true;
}

bool Version::operator==(const Version& other) const
{
    if (_major == other._major && _minor == other._minor) {
        return true;
    }
    return false;
}

bool Version::operator!=(const Version& other) const
{
    return !(*this == other);
}

bool Version::operator>(const Version& other) const
{
    if (_major > other._major) {
        return true;
    }
    else if (_major < other._major) {
        return false;
    }

    if (_minor > other._minor) {
        return true;
    }
    return false;
}

bool Version::operator<(const Version& other) const
{
    if (_major < other._major) {
        return true;
    }
    else if (_major > other._major) {
        return false;
    }

    if (_minor < other._minor) {
        return true;
    }
    return false;
}

bool Version::operator<=(const Version& other) const
{
    return !(*this > other);
}

bool Version::operator>=(const Version& other) const
{
    return !(*this < other);
}

std::string VersionConversion::toString(const Version& version)
{
    return version.toString();
}

Version VersionConversion::fromString(const std::string& version, const Version& default_val)
{
    if (version.size() == 0) {
        return default_val;
    }
    auto dotpos = version.find_first_of('.');

    std::string major_token;
    std::string minor_token;
    if (dotpos == std::string::npos) {
        major_token = version;
    }
    else {
        major_token = version.substr(0, dotpos);
        auto rst = version.size() - (dotpos + 1);
        if (rst > 0) {
            minor_token = version.substr(dotpos + 1, version.size() - rst);
        }
    }

    uint32_t major = 0, minor = 0;

    try {
        major = std::stoul(major_token);
    }
    catch (const std::exception& ex) {
        throw Error("Version::fromString",
                    {version},
                    "Major version " + major_token + " invalid" + ex.what());
    }

    if (major == 0) {
        throw Error("Version::fromString", {version}, "Major version " + major_token + " invalid");
    }

    if (!minor_token.empty()) {
        // special case 1.0+
        if (minor_token == "0+" || minor_token == "00+") {
            minor = 1;
        }
        else {
            try {
                minor = std::stoul(minor_token);
            }
            catch (const std::exception& ex) {
                throw Error("Version::fromString",
                            {version},
                            "Minor version " + minor_token + " invalid" + ex.what());
            }
        }
    }
    return Version(major, minor);
}

ArraySize::ArraySize(size_t array_size) : _array_size(array_size), _array_size_element_name()
{
}

ArraySize& ArraySize::operator=(size_t array_size)
{
    _array_size = array_size;
    _array_size_element_name = {};
    return *this;
}

ArraySize::ArraySize(const std::string& array_size_element_name)
    : _array_size(0), _array_size_element_name(array_size_element_name)
{
}

ArraySize& ArraySize::operator=(const std::string& array_size_element_name)
{
    _array_size = 0;
    _array_size_element_name = array_size_element_name;
    return *this;
}

void ArraySize::setArraySizeValue(size_t array_size)
{
    operator=(array_size);
}

size_t ArraySize::getArraySizeValue() const
{
    return _array_size;
}

void ArraySize::setArraySizeElementName(const std::string& array_size_element_name)
{
    operator=(array_size_element_name);
}

const std::string& ArraySize::getArraySizeElementName() const
{
    return _array_size_element_name;
}

bool ArraySize::isDynamicArraySize() const
{
    return !_array_size_element_name.empty();
}

bool ArraySize::operator==(const ArraySize& other) const
{
    if (isDynamicArraySize()) {
        return _array_size_element_name == other.getArraySizeElementName();
    }
    else {
        return _array_size == other.getArraySizeValue();
    }
}

bool ArraySize::operator!=(const ArraySize& other) const
{
    return !operator==(other);
}

} // namespace dd
} // namespace ddl
