/**
 * @file
 * Regex library
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

#include <a_util/regex/regularexpression.h>

#include <regex>

namespace a_util {
namespace regex {
// using this in member initialization list
#pragma warning(disable : 4355)

// RegularExpression implementation
class RegularExpression::Implementation {
public:
    RegularExpression* _self;
    bool _is_case_sensitive;
    std::unique_ptr<std::regex> _regex;
    std::string _error_description;
    std::string _pattern;

    Implementation(RegularExpression* self)
        : _self(self), _is_case_sensitive(true), _regex(), _error_description(), _pattern()
    {
    }
};

// Default constructor
RegularExpression::RegularExpression() : _impl(new Implementation(this))
{
}

// Constructor with a preinitialized pattern
RegularExpression::RegularExpression(const std::string& pattern, bool case_sensitive)
    : _impl(new Implementation(this))
{
    setPattern(pattern, case_sensitive);
}

// destructor
RegularExpression::~RegularExpression()
{
}

// Copy constructor
RegularExpression::RegularExpression(const RegularExpression& other)
    : _impl(new Implementation(this))
{
    setPattern(other._impl->_pattern, other._impl->_is_case_sensitive);
}

// Copy assignment operator
RegularExpression& RegularExpression::operator=(const RegularExpression& other)
{
    if (&other != this) {
        _impl.reset(new Implementation(this));
        setPattern(other._impl->_pattern, other._impl->_is_case_sensitive);
    }

    return *this;
}

bool RegularExpression::setPattern(const std::string& pattern, bool case_sensitive)
{
    if (pattern.empty()) {
        _impl->_error_description = "Empty pattern is invalid";
        return false;
    }

    try {
        std::unique_ptr<std::regex> regex(new std::regex(pattern));
        if (!case_sensitive) {
            regex->assign(pattern, regex->flags() | std::regex_constants::icase);
        }
        _impl->_regex.reset(regex.release());
    }
    catch (std::regex_error& e) {
        _impl->_error_description = e.what();
        return false;
    }

    _impl->_is_case_sensitive = case_sensitive;
    _impl->_error_description.clear();
    _impl->_pattern = pattern;

    return true;
}

std::string RegularExpression::getPattern() const
{
    return _impl->_regex ? _impl->_pattern : std::string();
}

const std::string& RegularExpression::getError() const
{
    return _impl->_error_description;
}

bool RegularExpression::fullMatch(const std::string& text) const
{
    if (!_impl->_regex)
        return false;
    return std::regex_match(text, *_impl->_regex);
}

bool RegularExpression::fullMatch(const std::string& text,
                                  std::string& arg1,
                                  std::string& arg2,
                                  std::string& arg3,
                                  std::string& arg4,
                                  std::string& arg5,
                                  std::string& arg6,
                                  std::string& arg7,
                                  std::string& arg8,
                                  std::string& arg9,
                                  std::string& arg10,
                                  std::string& arg11,
                                  std::string& arg12,
                                  std::string& arg13,
                                  std::string& arg14,
                                  std::string& arg15,
                                  std::string& arg16) const
{
    if (!_impl->_regex)
        return false;

    std::string arg0;
    const std::vector<std::string*> args{&arg0,
                                         &arg1,
                                         &arg2,
                                         &arg3,
                                         &arg4,
                                         &arg5,
                                         &arg6,
                                         &arg7,
                                         &arg8,
                                         &arg9,
                                         &arg10,
                                         &arg11,
                                         &arg12,
                                         &arg13,
                                         &arg14,
                                         &arg15,
                                         &arg16};

    std::smatch matches;
    bool result = std::regex_match(text, matches, *_impl->_regex);

    if (matches.size() > args.size()) {
        _impl->_error_description = "Too many matches found";
        return false;
    }

    auto arg = args.begin();
    for (auto match: matches) {
        **arg++ = match.str();
    }

    return result && matches.size() > 1; // fist match is always the overall regex match
}

bool RegularExpression::match(const std::string& text, AnchorType anchor, int& consumed) const
{
    if (!_impl->_regex)
        return false;

    bool result;
    std::smatch matches;

    // change regular expression temporary
    if (anchor == AT_Start)
        _impl->_regex->assign("^" + _impl->_pattern);
    if (anchor == AT_Both)
        _impl->_regex->assign("^" + _impl->_pattern + "$");

    result = std::regex_search(text, matches, *_impl->_regex);

    // restore regular expression to origin
    if (anchor != AT_Unanchored)
        _impl->_regex->assign(_impl->_pattern);

    if (result) {
        consumed = static_cast<int>(text.find(matches[0].str()) + matches[0].str().size());
    }

    return result;
}

std::string& RegularExpression::noArg()
{
    static std::string no_arg;
    return no_arg;
}

} // namespace regex
} // namespace a_util
