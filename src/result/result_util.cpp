/**
 * @file
 * Defining utility functions and classes for the Result class
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <a_util/result/result_type.h>
#include <a_util/result/result_util.h>
#include <a_util/strings/strings_format.h>

namespace a_util {
namespace result {
namespace detail {
class ResultFormatter {
public:
    ResultFormatter(ResultFormatFlags flags);

public: // observers
    const std::string& toString(const Result& result);

private:
    // noncopyable
    ResultFormatter(const ResultFormatter&);            //= delete;
    ResultFormatter& operator=(const ResultFormatter&); //= delete;

private: // modifiers
    bool notSet(ResultFormatFlags flag) const;

    void addCode(const Result& result);
    void addName(const Result& result);
    void addDesc(const Result& result);
    void addLine(const Result& result);
    void addFile(const Result& result);
    void addFunc(const Result& result);

private:                                 // types
    typedef std::int32_t UnderlyingType; //!< Underlying type of the enum

private:                                 // data
    const std::int32_t _flags;           //!< flags to format the string output
    std::string _formatted_string;       //!< The formatted string
    std::string::size_type _size_in_use; //!< Actual size of the formatted string
};

ResultFormatter::ResultFormatter(ResultFormatFlags flags)
    : _flags(static_cast<UnderlyingType>(flags)), _formatted_string(512, 0x00), _size_in_use(0)
{
}

const std::string& ResultFormatter::toString(const Result& result)
{
    addCode(result);
    addName(result);
    addDesc(result);
    addLine(result);
    addFile(result);
    addFunc(result);
    _formatted_string.resize(_size_in_use);
    return _formatted_string;
}

bool ResultFormatter::notSet(ResultFormatFlags flag) const
{
    return !(_flags & static_cast<UnderlyingType>(flag));
}

void ResultFormatter::addCode(const Result& result)
{
    if (notSet(DisableCode)) {
        using a_util::strings::format;
        format(_formatted_string, _size_in_use, "Result code '%d '", result.getErrorCode());
    }
}

void ResultFormatter::addName(const Result& result)
{
    if (notSet(DisableName)) {
        using a_util::strings::format;
        if (_formatted_string.empty()) {
            format(_formatted_string, _size_in_use, "Result: %s ", result.getErrorLabel());
        }
        else {
            format(_formatted_string, _size_in_use, "(%s) ", result.getErrorLabel());
        }
    }
}

void ResultFormatter::addDesc(const Result& result)
{
    if (notSet(DisableMessage)) {
        using a_util::strings::format;
        if (_formatted_string.empty()) {
            format(_formatted_string, _size_in_use, "Description: %s ", result.getDescription());
        }
        else {
            format(_formatted_string, _size_in_use, "- %s ", result.getDescription());
        }
    }
}

void ResultFormatter::addLine(const Result& result)
{
    if (notSet(DisableFile)) {
        using a_util::strings::format;
        format(_formatted_string, _size_in_use, "[File: %s] ", result.getFile());
    }
}

void ResultFormatter::addFile(const Result& result)
{
    if (notSet(DisableLine)) {
        using a_util::strings::format;
        format(_formatted_string, _size_in_use, "[Line: %d] ", result.getLine());
    }
}

void ResultFormatter::addFunc(const Result& result)
{
    if (notSet(DisableFunction)) {
        using a_util::strings::format;
        format(_formatted_string, _size_in_use, "[Func: %s]", result.getFunction());
    }
}

} // namespace detail

std::string toString(const Result& result, ResultFormatFlags formatting_flags, const char*)
{
    detail::ResultFormatter formatter(formatting_flags);
    return formatter.toString(result);
}

} // namespace result
} // namespace a_util
